#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include "config.h"
#include "file.h"
#include "error.h"
#include "version.h"
#include "debug.h"

/* General constants */
#define ASSET_DIRNAME "assets"
#define CONFIG_DIRNAME PROGRAM_NAME
#define CONFIG_FILENAME "settings.txt"
#define WINDOW_TITLE "base code " PROGRAM_VERSION
#define SETTING_MAXLEN 32

/* Max path length */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/*
 * On Windows, we store the configuration in a program-specific directory under
 * %APPDATA%. On other (Unix-like) systems, we use a directory located in the
 * user's home directory ($HOME) instead.
 */
#ifdef _WIN32
#define HOME_VAR "APPDATA"
#define PATH_SEP "\\"
#else
#define HOME_VAR "HOME"
#define PATH_SEP "/"
#endif

/* Internal helper macros */
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define BOOL_STR(x) ((x) ? "true" : "false")
#define KEY_NAME(x) (SDL_GetScancodeName(x))

/* Internal helper functions */
static void load_defaults(void);
static void print_config(void);
static const char *get_config_path(void);
static const char *get_config_dir(void);
static const char *get_asset_dir(void);

/* Global configuration */
Config config;

void config_load(void) {
    FILE *f;

    debug_printf("Loading configuration...\n");

    /* Fill the config with the default values first */
    load_defaults();

    /* On the very first run, the file shouldn't even exist */
    if (!(f = fopen(get_config_path(), "rt"))) {
        return;
    }

    char key[SETTING_MAXLEN + 1] = { '\0' };
    int value;

    /* Successfully opened, so read any settings */
    while (fscanf(f, "%" STR(SETTING_MAXLEN) "s = %d\n", key, &value) > 0) {
        if (SDL_strncmp(key, "window_width", SETTING_MAXLEN) == 0) {
            config.window_width = value;
        } else if (SDL_strncmp(key, "window_height", SETTING_MAXLEN) == 0) {
            config.window_height = value;
        } else if (SDL_strncmp(key, "window_borderless", SETTING_MAXLEN) == 0) {
            config.window_borderless = value;
        } else if (SDL_strncmp(key, "window_fullscreen", SETTING_MAXLEN) == 0) {
            config.window_fullscreen = value;
        } else if (SDL_strncmp(key, "window_maximized", SETTING_MAXLEN) == 0) {
            config.window_maximized = value;
        }
    }
    fclose(f);

    /* Solve directories */
    config.config_dir = get_config_dir();
    config.asset_dir = get_asset_dir();

    debug_printf("Configuration loaded.\n");

    print_config();
}

/*
 * Save configuration to file.
 */
void config_save(void) {
    FILE *f;
    const char *config_path = get_config_path();

    debug_printf("Saving configuration...\n");
    debug_printf("Configuration path: %s\n", config_path);

    /*
     * If we can't open the file on the first try, the app directory
     * probably doesn't exist yet.
     */
    if (!(f = fopen(config_path, "wt"))) {
        debug_printf("Configuration does not exist; attempting to create...\n");

        /* Attempt to create the directory */
        file_mkdir(get_config_dir());

        /* Try again */
        if (!(f = fopen(config_path, "wt"))) {
            error("Failed to save configuration.\n");
        }
    }
    /* Successfully created/opened, so write all the settings into it */
    fprintf(f, "#\n# Window settings\n#\n");
    fprintf(f, "window_width = %d\n", config.window_width);
    fprintf(f, "window_height = %d\n", config.window_height);
    fprintf(f, "window_borderless = %d\n", config.window_borderless);
    fprintf(f, "window_fullscreen = %d\n", config.window_fullscreen);
    fprintf(f, "window_maximized = %d\n", config.window_maximized);
    fprintf(f, "\n#\n# Key bindings\n#\n");
    fprintf(f, "key_up = %d\n", config.key_up);
    fprintf(f, "key_down = %d\n", config.key_down);
    fprintf(f, "key_left = %d\n", config.key_left);
    fprintf(f, "key_right = %d\n", config.key_right);
    fprintf(f, "key_accept = %d\n", config.key_accept);
    fprintf(f, "key_cancel = %d\n", config.key_cancel);
    fclose(f);

    debug_printf("Configuration saved.\n");
}

/*
 * Fill the global configuration struct with default values.
 */
void load_defaults(void) {
    debug_printf("Loading default configuration...\n");

    /* Constants */
    config.config_dir = get_config_dir();
    config.asset_dir = get_asset_dir();
    config.window_title = WINDOW_TITLE;

    /* Window settings */
    config.window_width = 800;
    config.window_height = 600;
    config.window_borderless = 0;
    config.window_fullscreen = 0;
    config.window_resizable = 1;
    config.window_maximized = 0;

    /* Viewport */
    config.draw_w = config.window_width;
    config.draw_h = config.window_height;
    config.draw_aspect_ratio = config.draw_w / (float) config.draw_h;
    config.view_w = config.draw_w;
    config.view_h = config.draw_h;
    config.view_x = 0;
    config.view_y = 0;

    /* Key bindings */
    config.key_up = SDL_SCANCODE_W;
    config.key_down = SDL_SCANCODE_S;
    config.key_left = SDL_SCANCODE_A;
    config.key_right = SDL_SCANCODE_D;
    config.key_accept = SDL_SCANCODE_RETURN;
    config.key_cancel = SDL_SCANCODE_ESCAPE;

    debug_printf("Default configuration loaded.\n");
}

/*
 * Print the contents of the current global configuration to standard output.
 */
void print_config(void) {
    debug_printf("Listing configuration...\n");
    debug_printf("  Write directory:   %s\n", config.config_dir);
    debug_printf("  Asset directory:   %s\n", config.asset_dir);
    debug_printf("  Window width:      %d\n", config.window_width);
    debug_printf("  Window height:     %d\n", config.window_height);
    debug_printf("  Window borderless: %s\n", BOOL_STR(config.window_borderless));
    debug_printf("  Window fullscreen: %s\n", BOOL_STR(config.window_fullscreen));
    debug_printf("  Window maximized:  %s\n", BOOL_STR(config.window_maximized));
    debug_printf("  Viewport width:    %d\n", config.draw_w);
    debug_printf("  Viewport height:   %d\n", config.draw_h);
    debug_printf("  Key up:            %s\n", KEY_NAME(config.key_up));
    debug_printf("  Key down:          %s\n", KEY_NAME(config.key_down));
    debug_printf("  Key left:          %s\n", KEY_NAME(config.key_left));
    debug_printf("  Key right:         %s\n", KEY_NAME(config.key_right));
    debug_printf("  Key accept:        %s\n", KEY_NAME(config.key_accept));
    debug_printf("  Key cancel:        %s\n", KEY_NAME(config.key_cancel));
    debug_printf("End of configuration.\n");
}

/*
 * Get the absolute path to the configuration file.
 */
const char *get_config_path(void) {
    static char config_path[PATH_MAX] = { '\0' };

    if (SDL_strlen(config_path) == 0) {
        SDL_strlcat(config_path, get_config_dir(), PATH_MAX);
        SDL_strlcat(config_path, CONFIG_FILENAME, PATH_MAX);
    }

    return config_path;
}

/*
 * Get the absolute path to the write directory.
 */
const char *get_config_dir(void) {
    static char config_dir[PATH_MAX] = { '\0' };

    if (SDL_strlen(config_dir) == 0) {
        SDL_strlcat(config_dir, SDL_getenv(HOME_VAR), PATH_MAX);
        SDL_strlcat(config_dir, PATH_SEP, PATH_MAX);
        SDL_strlcat(config_dir, CONFIG_DIRNAME, PATH_MAX);
        SDL_strlcat(config_dir, PATH_SEP, PATH_MAX);
    }

    return config_dir;
}

/*
 * Get the absolute path to the asset directory.
 */
const char *get_asset_dir(void) {
    static char asset_dir[PATH_MAX] = { '\0' };

    /* Check if asset directory has already been calculated */
    if (SDL_strlen(asset_dir) > 0) {
        return asset_dir;
    }

    /* Try $ASSET_DIRNAME in the binary directory */
    SDL_strlcat(asset_dir, SDL_GetBasePath(), PATH_MAX);
    SDL_strlcat(asset_dir, ASSET_DIRNAME, PATH_MAX);
    SDL_strlcat(asset_dir, PATH_SEP, PATH_MAX);

#ifdef _WIN32
    return asset_dir;
#else
    if (file_exists(asset_dir)) {
        return asset_dir;
    }
    /* Try /var/lib/$ASSET_DIRNAME */
    SDL_memset(asset_dir, 0, PATH_MAX);
    SDL_strlcat(asset_dir, "/var/lib/", PATH_MAX);
    SDL_strlcat(asset_dir, ASSET_DIRNAME, PATH_MAX);
    SDL_strlcat(asset_dir, PATH_SEP, PATH_MAX);
    if (file_exists(asset_dir)) {
        return asset_dir;
    }
    /* Try /usr/share/$ASSET_DIRNAME */
    SDL_memset(asset_dir, 0, PATH_MAX);
    SDL_strlcat(asset_dir, "/usr/share/", PATH_MAX);
    SDL_strlcat(asset_dir, ASSET_DIRNAME, PATH_MAX);
    SDL_strlcat(asset_dir, PATH_SEP, PATH_MAX);
    if (file_exists(asset_dir)) {
        return asset_dir;
    }
#endif

    error("Could not find asset directory.\n");

    return NULL;
}
