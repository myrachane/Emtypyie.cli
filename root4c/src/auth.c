#include "auth.h"
#include "theme.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void auth_setenv(void) {
    printf("\n");
    printf("  %s\n", retro_accent("Set environment variables"));
    printf("  %s\n", retro_dim("These are stored in ~/.emtypyie/env.json"));
    printf("\n");

    char *emty = get_emty_dir();
    char env_path[1024];
    snprintf(env_path, sizeof(env_path), "%s%cenv.json", emty, PATH_SEP);

    char input[4096];

    printf("  %s ", retro("NPM Token:"));
    if (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) > 0) {
            char content[4096];
            snprintf(content, sizeof(content), "{\"npm_token\":\"%s\"}", input);
            write_file(env_path, content);
        }
    }

    printf("  %s ", retro_dim("(More tokens coming soon)"));
    printf("\n");

    if (file_exists(env_path)) {
        printf("  %s\n", retro_accent("Environment set!"));
    }
    printf("\n");
}
