#include "asset.h"
#include "error.h"
#include "debug.h"

static void load_all_images(void);
static void free_all_images(void);

static void load_all_fonts(void);
static void free_all_fonts(void);

static void load_all_sounds(void);
static void free_all_sounds(void);

/* Global assets */
struct Assets assets;

void asset_init(void) {
    debug_printf("Loading all assets...\n");
    load_all_images();
    load_all_fonts();
    load_all_sounds();
    debug_printf("All assets loaded.\n");
}

void asset_quit(void) {
    debug_printf("Freeing all assets...\n");
    free_all_sounds();
    free_all_fonts();
    free_all_images();
    debug_printf("All assets freed.\n");
}

/*
 * Internal helper functions.
 */

void load_all_images(void) {
    debug_printf("Loading images...\n");
    assets.image_smile = image_load("images/smile.bmp");
    assets.image_another = image_load("images/another.bmp");
    debug_printf("Images loaded.\n");
}

void free_all_images(void) {
    debug_printf("Freeing all images...\n");
    image_free(assets.image_smile);
    image_free(assets.image_another);
    debug_printf("All images freed.\n");
}

void load_all_fonts(void) {
    debug_printf("Loading all fonts...\n");
    assets.font_basic = font_load("images/font.bmp");
    debug_printf("All fonts fonts loaded.\n");
}

void free_all_fonts(void) {
    debug_printf("Freeing all fonts...\n");
    font_free(assets.font_basic);
    debug_printf("All fonts fonts freed.\n");
}

void load_all_sounds(void) {
    debug_printf("Loading sounds...\n");
    assets.sound_pick = sound_load("sounds/pick.wav");
    debug_printf("Sounds loaded.\n");
}

void free_all_sounds(void) {
    debug_printf("Freeing sounds...\n");
    sound_free(assets.sound_pick);
    debug_printf("Sounds freed.\n");
}
