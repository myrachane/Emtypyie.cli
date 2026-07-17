#include "fetch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <winhttp.h>
#ifdef _MSC_VER
#pragma comment(lib, "winhttp.lib")
#endif
#else
#include <curl/curl.h>
#endif

struct WriteBuf {
    char *data;
    size_t len;
    size_t cap;
};

static size_t write_cb(void *ptr, size_t size, size_t nmemb, struct WriteBuf *buf) {
    size_t total = size * nmemb;
    if (buf->len + total >= buf->cap) {
        buf->cap = buf->len + total + 4096;
        char *newdata = realloc(buf->data, buf->cap);
        if (!newdata) return 0;
        buf->data = newdata;
    }
    memcpy(buf->data + buf->len, ptr, total);
    buf->len += total;
    buf->data[buf->len] = '\0';
    return total;
}

#ifdef _WIN32
static FetchResult* fetch_winhttp(const char *url, int timeout_sec) {
    FetchResult *res = calloc(1, sizeof(FetchResult));
    res->status_code = 0;

    URL_COMPONENTS uc = { sizeof(uc) };
    wchar_t host[256], path[2048], scheme[16];
    uc.lpszHostName = host; uc.dwHostNameLength = 256;
    uc.lpszUrlPath = path; uc.dwUrlPathLength = 2048;
    uc.lpszScheme = scheme; uc.dwSchemeLength = 16;

    wchar_t wurl[4096];
    mbstowcs(wurl, url, 4096);

    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) {
        return res;
    }

    BOOL use_ssl = (wcscmp(scheme, L"https") == 0);
    DWORD port = uc.nPort ? uc.nPort : (use_ssl ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT);

    HINTERNET hSession = WinHttpOpen(L"emtypyie-cli/2.1.3",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
    if (!hSession) return res;

    if (timeout_sec > 0) {
        int ms = timeout_sec * 1000;
        WinHttpSetTimeouts(hSession, ms, ms, -1, -1);
    }

    HINTERNET hConnect = WinHttpConnect(hSession, host, port, 0);
    if (!hConnect) { WinHttpCloseHandle(hSession); return res; }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path, NULL,
        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
        use_ssl ? WINHTTP_FLAG_SECURE : 0);
    if (!hRequest) { WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession); return res; }

    if (timeout_sec > 0) {
        int ms = timeout_sec * 1000;
        WinHttpSetTimeouts(hRequest, -1, -1, ms, ms);
    }

    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
        WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(hRequest, NULL)) {

        DWORD status = 0, status_size = sizeof(status);
        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            NULL, &status, &status_size, NULL);
        res->status_code = (int)status;

        struct WriteBuf buf = {0};
        buf.cap = 8192;
        buf.data = malloc(buf.cap);
        buf.data[0] = '\0';

        char tmp[8192];
        DWORD bytes_read;
        while (WinHttpReadData(hRequest, tmp, sizeof(tmp) - 1, &bytes_read) && bytes_read > 0) {
            tmp[bytes_read] = '\0';
            write_cb(tmp, 1, bytes_read, &buf);
        }

        res->body = buf.data;
        res->body_size = buf.len;
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return res;
}
#else
static FetchResult* fetch_curl(const char *url, int timeout_sec) {
    FetchResult *res = calloc(1, sizeof(FetchResult));
    res->status_code = 0;

    CURL *curl = curl_easy_init();
    if (!curl) return res;

    struct WriteBuf buf = {0};
    buf.cap = 8192;
    buf.data = malloc(buf.cap);
    buf.data[0] = '\0';

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buf);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "emtypyie-cli/2.1.3");
    if (timeout_sec > 0) curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)timeout_sec);

    CURLcode rc = curl_easy_perform(curl);
    if (rc == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res->status_code);
        res->body = buf.data;
        res->body_size = buf.len;
    } else {
        free(buf.data);
    }

    curl_easy_cleanup(curl);
    return res;
}
#endif

FetchResult* fetch_get(const char *url) {
    return fetch_get_with_timeout(url, 30);
}

FetchResult* fetch_get_with_timeout(const char *url, int timeout_sec) {
#ifdef _WIN32
    return fetch_winhttp(url, timeout_sec);
#else
    return fetch_curl(url, timeout_sec);
#endif
}

void fetch_free(FetchResult *res) {
    if (res) {
        free(res->body);
        free(res);
    }
}
