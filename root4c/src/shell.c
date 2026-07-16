#include "shell.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "util.h"
#include "fetch.h"
#include "project.h"
#include "bakafetch.h"
#include "runtime.h"
#include "auth.h"

#define MAX_HISTORY 100
#define MAX_LINE 4096
#define MAX_COMMANDS 64

static const char* COMMANDS[] = {
    "help", "about", "wiki", "setenv", "theme", "info", "get", "flash", "rm",
    "issue", "bakafetch", "bf", "wrap", "clear", "update", "list", "docs",
    "changelog", "exit", "quit", NULL
};

static char history[MAX_HISTORY][MAX_LINE];
static int hist_count = 0;
static int hist_pos = 0;

static const char BANNER[] =
    "___________        __                         .__                  .__  .__ \n"
    "\\_   _____/ ______/  |_ ___.__. ______ ___.__.|__| ____       ____ |  | |__|\n"
    " |    __)_ /     \\   __<   |  | \\____ <   |  ||  |/ __ \\    _/ ___\n"
    " |        \\  Y Y  \\  |  \\___  | |  |_> >___  ||  \\  ___/    \\  \\___|  |_|  |\n"
    "/_______  /__|_|  /__|  / ____| |   __// ____||__|\\___  > /\\ \\___  >____/__|\n"
    "        \\/      \\/      \\/      |__|   \\/             \\/  \\/     \\/ \n";

static void add_history(const char *line) {
    if (hist_count < MAX_HISTORY) {
        strncpy(history[hist_count], line, MAX_LINE - 1);
        hist_count++;
    } else {
        memmove(history, history + 1, (MAX_HISTORY - 1) * MAX_LINE);
        strncpy(history[MAX_HISTORY - 1], line, MAX_LINE - 1);
    }
    hist_pos = hist_count;
}

static void handle_command(const char *input) {
    char cmd[256] = {0}, arg[2048] = {0};
    const char *p = input;
    if (*p == '/') p++;
    while (*p && !isspace(*p) && strlen(cmd) < 255) { cmd[strlen(cmd)] = *p; p++; }
    while (*p && isspace(*p)) p++;
    strncpy(arg, p, 2047);

    if (strcmp(cmd, "help") == 0) {
        printf("\n");
        printf("  %s\n", retro_dim("─── Commands ───"));
        printf("\n");
        printf("  %s  %s\n", retro("/get <project>"), retro_dim("   install a project"));
        printf("  %s  %s\n", retro("/get gcc"), retro_dim("         auto-install GCC/G++ compiler"));
        printf("  %s  %s\n", retro("/flash <project>"), retro_dim("  re-download latest version"));
        printf("  %s  %s\n", retro("/info <project>"), retro_dim("   show project details"));
        printf("  %s  %s\n", retro("/rm <project>"), retro_dim("     delete project files"));
        printf("  %s  %s\n", retro("/issue <project>"), retro_dim("  open issue tracker"));
        printf("  %s  %s\n", retro("/setenv"), retro_dim("          set env variables"));
        printf("  %s  %s\n", retro("/theme <name>"), retro_dim("      change CLI color theme"));
        printf("  %s  %s\n", retro("/bakafetch"), retro_dim("        system info with style"));
        printf("  %s  %s\n", retro("/bf"), retro_dim("              shortcut for /bakafetch"));
        printf("  %s  %s\n", retro("/clear"), retro_dim("             clear screen"));
        printf("  %s  %s\n", retro("/update"), retro_dim("           update emtypyie"));
        printf("  %s  %s\n", retro("/list"), retro_dim("             list projects"));
        printf("  %s  %s\n", retro("/docs <project>"), retro_dim("   open project docs"));
        printf("  %s  %s\n", retro("/changelog"), retro_dim("        what's new"));
        printf("  %s  %s\n", retro("/about"), retro_dim("            about emtypyie"));
        printf("  %s  %s\n", retro("/wiki"), retro_dim("             open wiki.emtypyie.in"));
        printf("  %s  %s\n", retro("/help"), retro_dim("            this screen"));
        printf("  %s  %s\n", retro("/exit"), retro_dim("            quit"));
        printf("\n");
        return;
    }

    if (strcmp(cmd, "about") == 0) {
        printf("\n");
        printf("%s", retro(BANNER));
        printf("  %s\n", retro("EMTYPYIE CLI v2.1.3"));
        printf("  %s\n", retro_dim("Released 2026-07-16"));
        printf("\n");
        printf("  %s\n", retro_dim("DESIGNED AND ENGINEERED BY  EMTYPYIE"));
        printf("  %s\n", retro_dim("Copyright 2026 EMTYPYIE. All rights reserved."));
        printf("\n");
        return;
    }

    if (strcmp(cmd, "clear") == 0) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        return;
    }

    if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        printf("  %s\n", retro_dim("System halted."));
        exit(0);
    }

    if (strcmp(cmd, "wiki") == 0) {
        printf("  %s\n", retro("Opening wiki.emtypyie.in..."));
#ifdef _WIN32
        system("start https://wiki.emtypyie.in");
#else
        system("xdg-open https://wiki.emtypyie.in");
#endif
        return;
    }

    if (strcmp(cmd, "theme") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s %s\n", retro("Usage: /theme <name>"), retro_dim("Themes: "));
            printf("  %s\n", retro(theme_list()));
            return;
        }
        theme_init(arg);
        printf("  %s %s\n", retro("Theme set to"), retro_accent(arg));
        return;
    }

    if (strcmp(cmd, "bakafetch") == 0 || strcmp(cmd, "bf") == 0) {
        bakafetch_show();
        return;
    }

    if (strcmp(cmd, "list") == 0) {
        project_list();
        return;
    }

    if (strcmp(cmd, "get") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s\n", retro_err("Specify a project: /get <project>"));
            return;
        }
        if (strcmp(arg, "gcc") == 0 || strcmp(arg, "g++") == 0) {
            runtime_install_compiler(arg);
        } else {
            project_get(arg);
        }
        return;
    }

    if (strcmp(cmd, "info") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s\n", retro_err("Specify a project: /info <project>"));
            return;
        }
        project_info(arg);
        return;
    }

    if (strcmp(cmd, "flash") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s\n", retro_err("Specify a project: /flash <project>"));
            return;
        }
        project_flash(arg);
        return;
    }

    if (strcmp(cmd, "rm") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s\n", retro_err("Specify a project: /rm <project>"));
            return;
        }
        project_remove(arg);
        return;
    }

    if (strcmp(cmd, "docs") == 0) {
        if (strlen(arg) == 0) {
            printf("  %s\n", retro_err("Specify a project: /docs <project>"));
            return;
        }
        project_docs(arg);
        return;
    }

    if (strcmp(cmd, "changelog") == 0) {
        printf("  %s\n", retro("Opening changelog..."));
#ifdef _WIN32
        system("start https://github.com/myrachane/emtypyie-cli/releases");
#else
        system("xdg-open https://github.com/myrachane/emtypyie-cli/releases");
#endif
        return;
    }

    if (strcmp(cmd, "update") == 0) {
        printf("  %s\n", retro("Update not yet implemented in C version."));
        printf("  %s\n", retro_dim("Use: npm update -g emtypyie-cli"));
        return;
    }

    if (strcmp(cmd, "setenv") == 0) {
        auth_setenv();
        return;
    }

    printf("  %s %s\n", retro_err("Unknown command:"), retro_dim(cmd));
    printf("  %s\n", retro_dim("Type /help for available commands."));
}

#ifndef _WIN32
static void enable_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void disable_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag |= ECHO | ICANON;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
#endif

static int str_prefix(const char *str, const char *prefix) {
    while (*prefix) {
        if (tolower(*str) != tolower(*prefix)) return 0;
        str++; prefix++;
    }
    return 1;
}

static int find_completions(const char *input, char out[MAX_COMMANDS][256]) {
    int count = 0;
    const char *p = input;
    if (*p == '/') p++;
    for (int i = 0; COMMANDS[i] != NULL && count < MAX_COMMANDS; i++) {
        if (str_prefix(COMMANDS[i], p)) {
            strncpy(out[count], COMMANDS[i], 255);
            count++;
        }
    }
    return count;
}

void shell_run(void) {
    printf("%s", retro(BANNER));
    printf("\n");
    printf("  %s\n", retro_dim("─────────────────────────────────────────────"));
    printf("  %s", retro("\"code. create. conquer.\""));
    printf("\n");
    printf("  %s\n", retro_dim("─────────────────────────────────────────────"));
    printf("\n");
    printf("  %s %s %s\n", retro("Type"), retro_accent("/help"), retro_dim("for available commands."));
    printf("  %s\n", retro_dim("─────────────────────────────────────────────"));
    printf("\n");

    char buf[MAX_LINE];
    int pos = 0;
    hist_pos = 0;
    int tab_count = 0;
    int tab_cmd_count = 0;
    char tab_matches[MAX_COMMANDS][256];
    int tab_idx = 0;

    printf(" %s", retro(">> "));
    fflush(stdout);

#ifdef _WIN32
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD old_mode;
    GetConsoleMode(hIn, &old_mode);
    SetConsoleMode(hIn, old_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

    INPUT_RECORD rec;
    DWORD read_count;
    while (1) {
        ReadConsoleInput(hIn, &rec, 1, &read_count);
        if (rec.EventType != KEY_EVENT || !rec.Event.KeyEvent.bKeyDown) continue;
        KEY_EVENT_RECORD key = rec.Event.KeyEvent;

        if (key.wVirtualKeyCode == VK_RETURN) {
            printf("\n");
            buf[pos] = '\0';
            if (pos > 0) {
                add_history(buf);
                handle_command(buf);
            }
            pos = 0;
            tab_count = 0;
            printf(" %s", retro(">> "));
            fflush(stdout);
        } else if (key.wVirtualKeyCode == VK_BACK) {
            if (pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            tab_count = 0;
        } else if (key.wVirtualKeyCode == VK_TAB) {
            buf[pos] = '\0';
            char full_input[256];
            strncpy(full_input, buf, 255);
            int count = find_completions(full_input, tab_matches);
            if (count == 0) {
                tab_count = 0;
            } else if (count == 1) {
                int cmd_len = strlen(tab_matches[0]);
                int in_len = pos;
                if (buf[0] == '/') in_len--;
                for (int i = in_len; i < cmd_len; i++) {
                    buf[pos++] = tab_matches[0][i];
                    printf("%c", buf[pos-1]);
                }
                buf[pos++] = ' ';
                printf(" ");
                fflush(stdout);
                tab_count = 0;
            } else {
                if (tab_count == 0) {
                    tab_cmd_count = count;
                    tab_idx = 0;
                }
                tab_idx = (tab_idx + 1) % tab_cmd_count;

                int len_so_far = pos;
                if (buf[0] == '/') len_so_far--;
                int cmd_len = strlen(tab_matches[tab_idx]);

                for (int i = 0; i < len_so_far; i++) printf("\b \b");
                pos -= len_so_far;
                for (int i = 0; i < cmd_len; i++) {
                    buf[pos++] = tab_matches[tab_idx][i];
                    printf("%c", buf[pos-1]);
                }
                fflush(stdout);
                tab_count++;
            }
        } else if (key.wVirtualKeyCode == VK_UP) {
            if (hist_pos > 0) {
                hist_pos--;
                while (pos > 0) { pos--; printf("\b \b"); }
                int len = strlen(history[hist_pos]);
                strncpy(buf, history[hist_pos], MAX_LINE - 1);
                pos = len;
                printf("%s", buf);
                fflush(stdout);
            }
        } else if (key.wVirtualKeyCode == VK_DOWN) {
            if (hist_pos < hist_count) {
                hist_pos++;
                while (pos > 0) { pos--; printf("\b \b"); }
                if (hist_pos < hist_count) {
                    int len = strlen(history[hist_pos]);
                    strncpy(buf, history[hist_pos], MAX_LINE - 1);
                    pos = len;
                    printf("%s", buf);
                }
                fflush(stdout);
            }
        } else if (key.uChar.AsciiChar != 0) {
            if (pos < MAX_LINE - 1) {
                buf[pos++] = key.uChar.AsciiChar;
                printf("%c", key.uChar.AsciiChar);
                fflush(stdout);
            }
            tab_count = 0;
        }
    }
    SetConsoleMode(hIn, old_mode);
#else
    enable_raw_mode();
    while (1) {
        int c = getchar();
        if (c == '\n') {
            printf("\n");
            buf[pos] = '\0';
            if (pos > 0) {
                add_history(buf);
                handle_command(buf);
            }
            pos = 0;
            tab_count = 0;
            printf(" %s", retro(">> "));
            fflush(stdout);
        } else if (c == 127 || c == 8) {
            if (pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            tab_count = 0;
        } else if (c == '\t') {
            buf[pos] = '\0';
            char full_input[256];
            strncpy(full_input, buf, 255);
            int count = find_completions(full_input, tab_matches);
            if (count == 0) {
                tab_count = 0;
            } else if (count == 1) {
                int cmd_len = strlen(tab_matches[0]);
                int in_len = pos;
                for (int i = in_len; i < cmd_len; i++) {
                    buf[pos++] = tab_matches[0][i];
                    printf("%c", buf[pos-1]);
                }
                buf[pos++] = ' ';
                printf(" ");
                fflush(stdout);
                tab_count = 0;
            } else {
                if (tab_count == 0) {
                    tab_cmd_count = count;
                    tab_idx = 0;
                }
                tab_idx = (tab_idx + 1) % tab_cmd_count;
                int len_so_far = pos;
                int cmd_len = strlen(tab_matches[tab_idx]);
                for (int i = 0; i < len_so_far; i++) printf("\b \b");
                pos -= len_so_far;
                for (int i = 0; i < cmd_len; i++) {
                    buf[pos++] = tab_matches[tab_idx][i];
                    printf("%c", buf[pos-1]);
                }
                fflush(stdout);
                tab_count++;
            }
        } else if (c == 27) {
            getchar();
            c = getchar();
            if (c == 'A' && hist_pos > 0) {
                hist_pos--;
                while (pos > 0) { pos--; printf("\b \b"); }
                int len = strlen(history[hist_pos]);
                strncpy(buf, history[hist_pos], MAX_LINE - 1);
                pos = len;
                printf("%s", buf);
                fflush(stdout);
            } else if (c == 'B' && hist_pos < hist_count) {
                hist_pos++;
                while (pos > 0) { pos--; printf("\b \b"); }
                if (hist_pos < hist_count) {
                    int len = strlen(history[hist_pos]);
                    strncpy(buf, history[hist_pos], MAX_LINE - 1);
                    pos = len;
                    printf("%s", buf);
                }
                fflush(stdout);
            }
        } else if (c > 31) {
            if (pos < MAX_LINE - 1) {
                buf[pos++] = c;
                printf("%c", c);
                fflush(stdout);
            }
            tab_count = 0;
        }
    }
    disable_raw_mode();
#endif
}
