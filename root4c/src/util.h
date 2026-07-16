#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#ifdef _WIN32
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

char* path_join(const char *a, const char *b);
bool file_exists(const char *path);
bool dir_exists(const char *path);
bool dir_create(const char *path);
bool dir_remove(const char *path);
bool dir_remove_recursive(const char *path);
char* read_file(const char *path);
bool write_file(const char *path, const char *content);
char* get_home_dir(void);
char* get_emty_dir(void);
char* get_dev_dir(const char *name);
char* get_runtimes_dir(void);
char* get_temp_dir(void);
int  exec_cmd(const char *cmd, char *out, int out_size);
bool exec_cmd_silent(const char *cmd);
bool is_in_path(const char *name);
void add_to_user_path(const char *dir);
bool is_windows(void);

#endif
