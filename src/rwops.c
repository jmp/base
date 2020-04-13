#include <stdio.h>
#include <physfs.h>
#include "rwops.h"
#include "config.h"
#include "error.h"
#include "debug.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#endif

static Sint64 SDLCALL physfsrwops_size(struct SDL_RWops *rw) {
    PHYSFS_File *handle = (PHYSFS_File*) rw->hidden.unknown.data1;
    return (Sint64) PHYSFS_fileLength(handle);
}

static Sint64 SDLCALL physfsrwops_seek(struct SDL_RWops *rw, Sint64 offset,
                                       int whence) {
    PHYSFS_File *handle = (PHYSFS_File*) rw->hidden.unknown.data1;
    PHYSFS_sint64 pos = 0;

    if (whence == RW_SEEK_SET) {
        pos = (PHYSFS_sint64) offset;
    } else if (whence == RW_SEEK_CUR) {
        const PHYSFS_sint64 current = PHYSFS_tell(handle);
        if (current == -1) {
            SDL_SetError("Can't find position in file: %s",
                         PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            return -1;
        }

        if (offset == 0) {
            /* This is a "tell" call. We're done. */
            return (Sint64) current;
        }

        pos = current + ((PHYSFS_sint64) offset);
    } else if (whence == RW_SEEK_END) {
        const PHYSFS_sint64 len = PHYSFS_fileLength(handle);
        if (len == -1) {
            SDL_SetError("Can't find end of file: %s",
                         PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            return -1;
        }

        pos = len + ((PHYSFS_sint64) offset);
    } else {
        SDL_SetError("Invalid 'whence' parameter.");
        return -1;
    }

    if (pos < 0) {
        SDL_SetError("Attempt to seek past start of file.");
        return -1;
    }

    if (!PHYSFS_seek(handle, (PHYSFS_uint64) pos)) {
        SDL_SetError(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return -1;
    }

    return (Sint64) pos;
}

static size_t SDLCALL physfsrwops_read(struct SDL_RWops *rw, void *ptr,
                                       size_t size, size_t maxnum) {
    PHYSFS_File *handle = (PHYSFS_File*) rw->hidden.unknown.data1;
    const PHYSFS_uint64 readlen = (PHYSFS_uint64) (maxnum * size);
#if PHYSFS_VER_MAJOR < 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR < 1
    const PHYSFS_sint64 rc = PHYSFS_read(handle, ptr, 1, readlen);
#else
    const PHYSFS_sint64 rc = PHYSFS_readBytes(handle, ptr, readlen);
#endif
    if (rc != ((PHYSFS_sint64) readlen)) {
        if (!PHYSFS_eof(handle)) {
            SDL_SetError(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
            return 0;
        }
    }
    return (size_t) rc / size;
}

static size_t SDLCALL physfsrwops_write(struct SDL_RWops *rw,
                                        const void *ptr, size_t size,
                                        size_t num) {
    PHYSFS_File *handle = (PHYSFS_File*) rw->hidden.unknown.data1;
    const PHYSFS_uint64 writelen = (PHYSFS_uint64) (num * size);
#if PHYSFS_VER_MAJOR < 2 || PHYSFS_VER_MAJOR == 2 && PHYSFS_VER_MINOR < 1
    const PHYSFS_sint64 rc = PHYSFS_write(handle, ptr, 1, writelen);
#else
    const PHYSFS_sint64 rc = PHYSFS_writeBytes(handle, ptr, writelen);
#endif
    if (rc != ((PHYSFS_sint64) writelen)) {
        SDL_SetError(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
    return (size_t) rc;
}

static int physfsrwops_close(SDL_RWops * rw) {
    PHYSFS_File *handle = (PHYSFS_File*) rw->hidden.unknown.data1;
    if (!PHYSFS_close(handle)) {
        SDL_SetError(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
        return -1;
    }
    SDL_FreeRW(rw);
    return 0;
}

static SDL_RWops *create_rwops(PHYSFS_File * handle) {
    SDL_RWops *retval = NULL;

    if (handle == NULL) {
        SDL_SetError(PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    } else if ((retval = SDL_AllocRW()) != NULL) {
        retval->size = physfsrwops_size;
        retval->seek = physfsrwops_seek;
        retval->read = physfsrwops_read;
        retval->write = physfsrwops_write;
        retval->close = physfsrwops_close;
        retval->hidden.unknown.data1 = handle;
    }
    return retval;
}

SDL_RWops *rwops_open_read(const char *fname) {
    return create_rwops(PHYSFS_openRead(fname));
}

static const char *get_exe_path(const char *default_path) {
#ifdef _WIN32
    static TCHAR exe_path[MAX_PATH] = { '\0' };
    DWORD buffer_size = sizeof(exe_path) / sizeof(*exe_path);
    if (GetModuleFileName(NULL, exe_path, buffer_size) == buffer_size) {
        return default_path; /* Buffer too small, so just use default */
    }
    return exe_path;
#endif
    return default_path;
}

static void mount_path(const char *path, int append) {
    PHYSFS_mount(path, NULL, append ? 1 : 0);
    debug_printf("Path %s mounted.\n", path);
}

void rwops_init(const char *program_name) {
    debug_printf("Initializing R/W operations...\n");

    if (!PHYSFS_init(get_exe_path(program_name))) {
        error("Failed to initialize R/W operations: %s\n",
              PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }

    debug_printf("Mounting paths...\n");
    mount_path(get_exe_path(program_name), 0);
    mount_path(config.asset_dir, 1);
    debug_printf("Paths mounted.\n");

    debug_printf("R/W operations initialized.\n");
}

void rwops_quit(void) {
    debug_printf("Shutting down R/W operations...\n");
    PHYSFS_deinit();
    debug_printf("R/W operations shut down.\n");
}
