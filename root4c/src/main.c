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
    printf("  /help           Show help\n");
    printf("  /get <project>  Install a project\n");
    printf("  /get gcc        Install GCC/G++ compiler\n");
    printf("  /info <project> Show project details\n");
    printf("  /flash <proj>   Re-download latest version\n");
    printf("  /rm  <project>  Remove project\n");
    printf("  /theme <name>   Change theme\n");
    printf("  /list           List installed projects\n");
    printf("  /about          About emtypyie\n");
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
        char slash_cmd[256] = "/";
        strncat(slash_cmd, cmd, 254);

        if (strcmp(slash_cmd, "/help") == 0) print_usage();
        else if (strcmp(slash_cmd, "/about") == 0) {
            printf("EMTYPYIE CLI v2.1.3 (C port)\n");
        } else {
            printf("Unrecognized. Use /help\n");
        }
        return 0;
    }

    if (strcmp(cmd, "/help") == 0) {
        print_usage();
    } else if (strcmp(cmd, "/about") == 0) {
        printf("  %s\n", retro_accent("EMTYPYIE CLI v2.1.3 (C port)"));
        printf("  %s\n", retro_dim("Copyright 2026 EMTYPYIE. All rights reserved."));
    } else if (strcmp(cmd, "/get") == 0) {
        if (strlen(arg) == 0) { printf("Specify a project.\n"); return 1; }
        extern void project_get(const char*);
        project_get(arg);
    } else if (strcmp(cmd, "/info") == 0) {
        if (strlen(arg) == 0) { printf("Specify a project.\n"); return 1; }
        extern void project_info(const char*);
        project_info(arg);
    } else if (strcmp(cmd, "/list") == 0) {
        extern void project_list(void);
        project_list();
    } else {
        printf("Unknown command. Use /help\n");
    }

    return 0;
}
