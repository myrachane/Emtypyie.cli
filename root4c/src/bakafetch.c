#include "bakafetch.h"
#include "theme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#endif

static const char* get_os(void) {
#ifdef _WIN32
    static char buf[64];
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    GetVersionEx((OSVERSIONINFO*)&osvi);
    snprintf(buf, sizeof(buf), "Windows %lu.%lu.%lu", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);
    return buf;
#else
    return "Linux";
#endif
}

static const char* get_cpu(void) {
#ifdef _WIN32
    static char buf[256];
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD size = sizeof(buf);
        RegQueryValueEx(hKey, "ProcessorNameString", NULL, NULL, (BYTE*)buf, &size);
        RegCloseKey(hKey);
        return buf;
    }
    return "Unknown CPU";
#else
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) return "Unknown CPU";
    static char buf[256];
    while (fgets(buf, sizeof(buf), f)) {
        if (strncmp(buf, "model name", 10) == 0) {
            char *p = strchr(buf, ':');
            if (p) {
                p += 2;
                char *nl = strchr(p, '\n');
                if (nl) *nl = '\0';
                fclose(f);
                return p;
            }
        }
    }
    fclose(f);
    return "Unknown CPU";
#endif
}

static long long get_total_ram(void) {
#ifdef _WIN32
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatusEx(&ms);
    return (long long)(ms.ullTotalPhys / (1024 * 1024));
#else
    struct sysinfo si;
    sysinfo(&si);
    return (long long)(si.totalram * si.mem_unit / (1024 * 1024));
#endif
}

static long long get_free_ram(void) {
#ifdef _WIN32
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatusEx(&ms);
    return (long long)(ms.ullAvailPhys / (1024 * 1024));
#else
    struct sysinfo si;
    sysinfo(&si);
    return (long long)(si.freeram * si.mem_unit / (1024 * 1024));
#endif
}

static const char* get_disk_usage(void) {
    static char buf[64];
#ifdef _WIN32
    ULARGE_INTEGER free_bytes, total_bytes;
    if (GetDiskFreeSpaceEx(".", &free_bytes, &total_bytes, NULL)) {
        double free_gb = (double)free_bytes.QuadPart / (1024*1024*1024);
        double total_gb = (double)total_bytes.QuadPart / (1024*1024*1024);
        snprintf(buf, sizeof(buf), "%.1f/%.1f GB", free_gb, total_gb);
    } else {
        snprintf(buf, sizeof(buf), "N/A");
    }
#else
    struct statvfs vfs;
    if (statvfs(".", &vfs) == 0) {
        double free_gb = (double)vfs.f_bfree * vfs.f_frsize / (1024*1024*1024);
        double total_gb = (double)vfs.f_blocks * vfs.f_frsize / (1024*1024*1024);
        snprintf(buf, sizeof(buf), "%.1f/%.1f GB", free_gb, total_gb);
    }
#endif
    return buf;
}

static int get_cpu_cores(void) {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwNumberOfProcessors;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

static const char* get_shell(void) {
#ifdef _WIN32
    return "PowerShell 5.1";
#else
    const char *s = getenv("SHELL");
    return s ? s : "unknown";
#endif
}

static const char* get_terminal(void) {
#ifdef _WIN32
    return "Windows Terminal";
#else
    const char *t = getenv("TERM");
    return t ? t : "unknown";
#endif
}

static const char* get_resolution(void) {
    static char buf[64];
#ifdef _WIN32
    HDC hdc = GetDC(NULL);
    int w = GetDeviceCaps(hdc, HORZRES);
    int h = GetDeviceCaps(hdc, VERTRES);
    ReleaseDC(NULL, hdc);
    snprintf(buf, sizeof(buf), "%dx%d", w, h);
#else
    FILE *f = popen("xrandr --current | grep '*' | head -1 | awk '{print $1}'", "r");
    if (f && fgets(buf, sizeof(buf), f)) {
        char *nl = strchr(buf, '\n'); if (nl) *nl = '\0';
    } else {
        snprintf(buf, sizeof(buf), "N/A");
    }
    if (f) pclose(f);
#endif
    return buf;
}

void bakafetch_show(void) {
    const char *os = get_os();
    const char *cpu = get_cpu();
    int cores = get_cpu_cores();
    long long total_ram = get_total_ram();
    long long free_ram = get_free_ram();
    const char *disk = get_disk_usage();
    const char *shell = get_shell();
    const char *term = get_terminal();
    const char *res = get_resolution();

    char cpu_line[512];
    snprintf(cpu_line, sizeof(cpu_line), "%s (%d cores)", cpu, cores);
    char mem_line[64];
    snprintf(mem_line, sizeof(mem_line), "%lld / %lld MB", free_ram, total_ram);

    printf("\n");
    printf("        %s\n", retro_dim("          .---."));
    printf("        %s\n", retro_dim("         /     \\"));
    printf("        %s%s%s\n", retro_dim("        |  ("), retro_accent("o"), retro_dim(")"));
    printf("        %s\n", retro_dim("        |     / \\"));
    printf("        %s\n", retro_dim("        |   /   |"));
    printf("        %s\n", retro_dim("        '--'    |"));
    printf("        %s\n", retro_dim("             __/"));
    printf("\n");
    printf("  %s  %s\n", retro_accent("OS:"),         retro_dim(os));
    printf("  %s  %s\n", retro_accent("CPU:"),        retro_dim(cpu_line));
    printf("  %s  %s\n", retro_accent("Memory:"),     retro_dim(mem_line));
    printf("  %s  %s\n", retro_accent("Disk:"),       retro_dim(disk));
    printf("  %s  %s\n", retro_accent("Shell:"),      retro_dim(shell));
    printf("  %s  %s\n", retro_accent("Terminal:"),   retro_dim(term));
    printf("  %s  %s\n", retro_accent("Resolution:"), retro_dim(res));
    printf("\n");
}
