#include "project.h"
#include "theme.h"
#include "fetch.h"
#include "download.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define API_BASE "https://cdn.emtypyie.in/dev"
#define MAX_LINE 1024

typedef struct {
    char name[128];
    char version[64];
    char description[512];
    char author[128];
} ProjectInfo;

static int parse_project_json(const char *json, ProjectInfo *info) {
    memset(info, 0, sizeof(ProjectInfo));

    char *p;
    p = strstr(json, "\"name\":");
    if (p) {
        p += 7; while (*p && *p != '"') p++;
        if (*p) { p++; int i = 0; while (*p && *p != '"' && i < 127) info->name[i++] = *p++; }
    }
    p = strstr(json, "\"version\":");
    if (p) {
        p += 10; while (*p && *p != '"') p++;
        if (*p) { p++; int i = 0; while (*p && *p != '"' && i < 63) info->version[i++] = *p++; }
    }
    p = strstr(json, "\"description\":");
    if (p) {
        p += 14; while (*p && *p != '"') p++;
        if (*p) { p++; int i = 0; while (*p && *p != '"' && i < 511) info->description[i++] = *p++; }
    }
    return (strlen(info->name) > 0) ? 1 : 0;
}

static void print_project(ProjectInfo *info) {
    printf("  %s %s %s\n", retro_accent(info->name), retro_dim("v"), retro_dim(info->version));
    if (strlen(info->description) > 0)
        printf("    %s\n", retro_dim(info->description));
}

void project_list(void) {
    printf("\n");

    char *emty_dev = get_emty_dir();
    char dev_path[1024];
    snprintf(dev_path, sizeof(dev_path), "%s%cdev", emty_dev, PATH_SEP);

    if (!dir_exists(dev_path)) {
        printf("  %s\n", retro_dim("No projects installed yet."));
        return;
    }

    DIR *d = opendir(dev_path);
    if (!d) {
        printf("  %s\n", retro_dim("No projects installed."));
        free(emty_dev);
        return;
    }

    printf("  %s\n", retro_accent("Installed projects:"));
    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(d)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        char meta_path[1024];
        snprintf(meta_path, sizeof(meta_path), "%s%c%s%c.emtypyie.json", dev_path, PATH_SEP, entry->d_name, PATH_SEP);
        char *json = read_file(meta_path);
        if (json) {
            ProjectInfo info;
            parse_project_json(json, &info);
            print_project(&info);
            free(json);
        } else {
            printf("  %s %s\n", retro_accent(entry->d_name), retro_dim("(unknown)"));
        }
        count++;
    }
    if (count == 0) {
        printf("  %s\n", retro_dim("No projects installed."));
    }
    closedir(d);
    printf("\n");
}

void project_info(const char *name) {
    printf("\n");
    char url[512];
    snprintf(url, sizeof(url), "%s/info/%s", API_BASE, name);

    printf("  %s %s...\n", retro_dim("Fetching info for"), retro_accent(name));
    FetchResult *res = fetch_get(url);
    if (!res || res->status_code != 200) {
        printf("  %s %s\n", retro_err("Project not found:"), retro_dim(name));
        fetch_free(res);
        return;
    }

    ProjectInfo info;
    if (parse_project_json(res->body, &info)) {
        printf("  %s  %s\n", retro_accent("Name:"),        retro_dim(info.name));
        printf("  %s  %s\n", retro_accent("Version:"),     retro_dim(info.version));
        printf("  %s  %s\n", retro_accent("Description:"), retro_dim(info.description));
    } else {
        printf("  %s\n", res->body);
    }
    fetch_free(res);
    printf("\n");
}

void project_get(const char *name) {
    printf("\n");

    char *dev_dir = get_dev_dir(name);

    char url[512];
    snprintf(url, sizeof(url), "%s/get/%s", API_BASE, name);

    char meta_url[512];
    snprintf(meta_url, sizeof(meta_url), "%s/info/%s", API_BASE, name);

    printf("  %s %s %s\n", retro_dim("Fetching"), retro_accent(name), retro_dim("from registry..."));
    FetchResult *meta = fetch_get(meta_url);
    if (!meta || meta->status_code != 200) {
        printf("  %s %s\n", retro_err("Project not found:"), retro_dim(name));
        fetch_free(meta);
        return;
    }

    ProjectInfo info;
    char *meta_body = strdup(meta->body);
    parse_project_json(meta_body, &info);
    fetch_free(meta);

    if (strlen(info.version) > 0) {
        printf("  %s %s\n", retro_dim("Latest version:"), retro_accent(info.version));
    }

    char dest_zip[1024];
    snprintf(dest_zip, sizeof(dest_zip), "%s%cproject.zip", dev_dir, PATH_SEP);

    if (download_file(url, dest_zip)) {
        printf("  %s\n", retro("Extracting..."));
#ifdef _WIN32
        char extract_cmd[2048];
        snprintf(extract_cmd, sizeof(extract_cmd), "powershell -Command \"Expand-Archive -Path '%s' -DestinationPath '%s' -Force\"", dest_zip, dev_dir);
        exec_cmd_silent(extract_cmd);
#else
        char extract_cmd[2048];
        snprintf(extract_cmd, sizeof(extract_cmd), "unzip -o '%s' -d '%s'", dest_zip, dev_dir);
        system(extract_cmd);
#endif
        remove(dest_zip);

        char meta_file[1024];
        snprintf(meta_file, sizeof(meta_file), "%s%c.emtypyie.json", dev_dir, PATH_SEP);
        write_file(meta_file, meta_body);

        printf("  %s %s %s\n", retro_accent(name), retro_dim("installed to"), retro_dim(dev_dir));
    } else {
        printf("  %s %s\n", retro_err("Failed to download:"), retro_dim(name));
    }
    free(meta_body);
    printf("\n");
}

void project_flash(const char *name) {
    char *dev_dir = get_dev_dir(name);
    if (dir_exists(dev_dir)) {
        dir_remove_recursive(dev_dir);
    }
    project_get(name);
}

void project_remove(const char *name) {
    char *dev_dir = get_dev_dir(name);
    if (dir_exists(dev_dir)) {
        dir_remove_recursive(dev_dir);
        printf("  %s %s\n", retro_accent(name), retro_dim("removed."));
    } else {
        printf("  %s %s\n", retro_dim("Project not found:"), retro_dim(name));
    }
}

void project_docs(const char *name) {
    printf("  %s\n", retro_dim("Opening docs..."));
    char url[512];
    snprintf(url, sizeof(url), "https://wiki.emtypyie.in/docs/%s", name);
    char cmd[1024];
#ifdef _WIN32
    snprintf(cmd, sizeof(cmd), "start %s", url);
#else
    snprintf(cmd, sizeof(cmd), "xdg-open %s", url);
#endif
    system(cmd);
}
