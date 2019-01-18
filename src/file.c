#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include "file.h"
#include "config.h"
#include "debug.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int file_exists(const char *path) {
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

int file_mkdir(const char *path) {
    int result;

    debug_printf("Creating directory %s...\n", path);

#ifdef _WIN32
    result = mkdir(path) == 0;
#else
    result = mkdir(path, S_IRUSR | S_IWUSR) == 0;
#endif

    debug_printf("Directory %s created.\n", path);

    return result;
}

const char *file_abspath(const char *path) {
    static char tmp[PATH_MAX] = { '\0' };

    /* Concatenate the base path with the relative path */
    SDL_memset(tmp, 0, PATH_MAX);
    SDL_strlcat(tmp, config.asset_dir, PATH_MAX);
    SDL_strlcat(tmp, path, PATH_MAX);

#ifdef _WIN32
    {
        size_t i;
        /* Not necessary, but makes the paths look a bit nicer on Windows */
        for (i = 0; i < strlen(tmp); i++) {
            if (tmp[i] == '/') {
                tmp[i] = '\\';
            }
        }
    }
#endif

    return tmp;
}
