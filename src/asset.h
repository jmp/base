#ifndef ASSET_H
#define ASSET_H

#include "image.h"
#include "font.h"
#include "sound.h"

struct Assets {
    /* Images */
    Image *image_smile;
    Image *image_another;

    /* Fonts */
    Font *font_basic;

    /* Sounds */
    Sound *sound_pick;
};

/* Global assets */
extern struct Assets assets;

/*
 * Load all the assets (images, fonts, etc.) required for the program to run.
 * This calls the asset loading functions of each asset type. If any of the
 * assets fails to load, the program will exit.
 */
extern void asset_init(void);

/*
 * Free all the loaded assets (images, fonts, etc.). This calls the asset
 * freeing functions of each asset type.
 */
extern void asset_quit(void);

#endif
