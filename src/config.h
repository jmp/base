#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    const char *config_dir;
    const char *asset_dir;
    const char *window_title;
    int window_width;
    int window_height;
    int window_borderless;
    int window_fullscreen;
    int window_resizable;
    int window_maximized;
    int draw_w;
    int draw_h;
    int view_x;
    int view_y;
    int view_w;
    int view_h;
    float draw_aspect_ratio;
    int key_up;
    int key_down;
    int key_left;
    int key_right;
    int key_accept;
    int key_cancel;
} Config;

/* Global configuration */
extern Config config;

/*
 * Load configuration from file to the global configuration.
 */
extern void config_load(void);

/*
 * Save the global configuration to file.
 */
extern void config_save(void);

#endif
