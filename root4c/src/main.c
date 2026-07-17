#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "theme.h"
#include "shell.h"
#include "util.h"

static void print_usage(void) {
    printf("Usage: emtypyie [command] [args]\n");
    printf("\n");
    printf("Commands:\n");
    printf("  /help             Show help\n");
    printf("  /about            About emtypyie\n");
    printf("  /list             List available projects\n");
    printf("  /get    <project> Install a project\n");
    printf("  /get    gcc       Install GCC/G++ compiler\n");
    printf("  /info   <project> Show project details\n");
    printf("  /flash  <project> Re-download latest version\n");
    printf("  /rm     <project> Remove project\n");
    printf("  /theme  <name>    Change theme\n");
    printf("  /docs   <project> Open documentation\n");
    printf("  /bf               System info (bakafetch)\n");
    printf("  /clear            Clear screen\n");
    printf("  /shell            Interactive mode\n");
    printf("\n");
    printf("Run without arguments to enter interactive mode.\n");
}

int main(int argc, char *argv[]) {
    const char *theme_name = "violet";
    char *emty_dir = get_emty_dir();

    char theme_path[1024];
    snprintf(theme_path, sizeof(theme_path), "%s%ctheme.txt", emty_dir, PATH_SEP);
    if (file_exists(theme_path)) {
        char *saved = read_file(theme_path);
        if (saved) {
            char *nl = strchr(saved, '\n');
            if (nl) *nl = '\0';
            theme_name = saved;
        }
    }

    theme_init(theme_name);

    if (argc == 1) {
        shell_run();
        return 0;
    }

    const char *cmd = argv[1];
    const char *arg = argc > 2 ? argv[2] : "";

    if (cmd[0] != '/') {
        if (strcmp(cmd, "help") == 0) print_usage();
        else if (strcmp(cmd, "about") == 0)
            printf("EMTYPYIE CLI v2.2.0 (C port)\n");
        else
            printf("Unrecognized. Use /help\n");
        return 0;
    }

    if      (strcmp(cmd, "/help") == 0)  print_usage();
    else if (strcmp(cmd, "/about") == 0) {
        printf("  %s\n", retro_accent("EMTYPYIE CLI v2.2.0 (C port)"));
        printf("  %s\n", retro_dim("Copyright 2026 EMTYPYIE. All rights reserved."));
    }
    else if (strcmp(cmd, "/get") == 0) {
        if (strlen(arg) == 0) { printf("  Specify a project.\n"); return 1; }
        if (strcmp(arg, "gcc") == 0 || strcmp(arg, "g++") == 0) {
            extern void runtime_install_compiler(const char*);
            runtime_install_compiler(arg);
        } else {
            extern void project_get(const char*);
            project_get(arg);
        }
    }
    else if (strcmp(cmd, "/info") == 0) {
        if (strlen(arg) == 0) { printf("  Specify a project.\n"); return 1; }
        extern void project_info(const char*);
        project_info(arg);
    }
    else if (strcmp(cmd, "/list") == 0) {
        extern void project_list(void);
        project_list();
    }
    else if (strcmp(cmd, "/flash") == 0) {
        if (strlen(arg) == 0) { printf("  Specify a project.\n"); return 1; }
        extern void project_flash(const char*);
        project_flash(arg);
    }
    else if (strcmp(cmd, "/rm") == 0) {
        if (strlen(arg) == 0) { printf("  Specify a project.\n"); return 1; }
        extern void project_remove(const char*);
        project_remove(arg);
    }
    else if (strcmp(cmd, "/theme") == 0) {
        if (strlen(arg) == 0) {
            printf("  Current theme: %s\n", theme_current());
            printf("  Available: %s\n", theme_list());
        } else {
            theme_init(arg);
            printf("  Theme set to %s\n", retro(theme_current()));
            char theme_path[1024];
            snprintf(theme_path, sizeof(theme_path), "%s%ctheme.txt", get_emty_dir(), PATH_SEP);
            write_file(theme_path, arg);
        }
    }
    else if (strcmp(cmd, "/docs") == 0) {
        if (strlen(arg) == 0) { printf("  Specify a project.\n"); return 1; }
        extern void project_docs(const char*);
        project_docs(arg);
    }
    else if (strcmp(cmd, "/bf") == 0 || strcmp(cmd, "/bakafetch") == 0) {
        extern void bakafetch_show(void);
        bakafetch_show();
    }
    else if (strcmp(cmd, "/clear") == 0) {
        printf("\033[2J\033[H");
    }
    else if (strcmp(cmd, "/shell") == 0) {
        shell_run();
    }
    else if (strcmp(cmd, "/setenv") == 0) {
        extern void auth_setenv(void);
        auth_setenv();
    }
    else {
        printf("  Unknown command. Use /help\n");
    }

    return 0;
}
