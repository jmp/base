#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "error.h"

#define MODULE_LENGTH 32
#define TIMESTAMP_LENGTH 26

static char *basename(const char *path) {
    static char bname[MODULE_LENGTH];
    const char *endp, *startp;
    size_t len;

    /* Empty or NULL string gets treated as "." */
    if (path == NULL || *path == '\0') {
        bname[0] = '.';
        bname[1] = '\0';
        return bname;
    }

    /* Strip any trailing slashes */
    endp = path + strlen(path) - 1;
    while (endp > path && *endp == '/') {
        endp--;
    }

    /* All slashes becomes "/" */
    if (endp == path && *endp == '/') {
        bname[0] = '/';
        bname[1] = '\0';
        return bname;
    }

    /* Find the start of the base */
    startp = endp;
    while (startp > path && *(startp - 1) != '/') {
        startp--;
    }

    len = endp - startp + 1;
    if (len >= sizeof(bname)) {
        return NULL;
    }

    memcpy(bname, startp, len);
    bname[len] = '\0';

    return bname;
}

void debug_printf_internal(const char *module, const char *format, ...) {
    int milliseconds;
    char timestamp[TIMESTAMP_LENGTH];
    struct tm *tm_info;
    struct timeval tv;
    time_t timer;
    va_list args;
    char *module_name = basename(module);
    char *dot_pos = strrchr(module_name, '.');

    if (dot_pos != NULL) {
        *dot_pos = '\0';
    }

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_usec + 500) / 1000;
    if (milliseconds >= 1000) {
        milliseconds -= 1000;
        tv.tv_sec++;
    }
    timer = tv.tv_sec;
    tm_info = localtime(&timer);
    strftime(timestamp, TIMESTAMP_LENGTH, "%H:%M:%S", tm_info);

    fprintf(stdout, "%s.%03d [%s] ", timestamp, milliseconds, module_name);
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}
