#include "bakafetch.h"
#include "theme.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "bakafetch_data.h"

static int art_width(const char *line) {
    int w = 0;
    while (*line) {
        unsigned char c = (unsigned char)*line;
        if ((c & 0xF8) == 0xF0) { w += 2; line += 4; }
        else if ((c & 0xF0) == 0xE0) { w += 2; line += 3; }
        else if ((c & 0xE0) == 0xC0) { w += 1; line += 2; }
        else { w += 1; line += 1; }
    }
    return w;
}

void bakafetch_show(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    srand((unsigned)time(NULL));
    int art_idx = rand() % ART_COUNT;
    int tsun_idx = rand() % TSUN_COUNT;

    const char **art = ARTS[art_idx];
    const char *tsun = TSUNDERE[tsun_idx];

    int max_w = 0;
    for (int i = 0; art[i] != NULL; i++) {
        int w = art_width(art[i]);
        if (w > max_w) max_w = w;
    }

    int art_lines = 0;
    while (art[art_lines] != NULL) art_lines++;

    printf("\n");
    for (int i = 0; i < art_lines; i++) {
        const char *line = art[i];
        int w = art_width(line);
        int pad = max_w - w;
        printf("  %s", retro_accent(line));
        if (pad > 0) printf("%*s", pad, "");
        printf("\n");
    }
    printf("\n");
    printf("  %s\n", retro_dim(tsun));
    printf("\n");
}
