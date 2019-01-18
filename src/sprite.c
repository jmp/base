#include <math.h>
#include "sprite.h"
#include "memory.h"
#include "window.h"
#include "error.h"
#include "timer.h"

struct Sprite {
    float x, y;
    float w, h;
    float angle;
    float scale;
    Image *image;
};

Sprite *sprite_create(Image * image) {
    /* Allocate memory for the sprite */
    Sprite *sprite = memory_alloc(sizeof(Sprite));

    /* Fill in the default values */
    sprite->image = image;
    sprite->angle = 0.0f;
    sprite->scale = 1.0f;
    sprite->x = 0;
    sprite->y = 0;
    sprite->w = image_get_width(image);
    sprite->h = image_get_height(image);

    return sprite;
}

void sprite_destroy(Sprite * sprite) {
    memory_free(sprite);
}

void sprite_draw(Sprite * sprite, float x, float y) {
    const float angle = sin(timer_get_ticks() * 0.01f) * 15.0f;

    image_draw(sprite->image, x, y, sprite->w, sprite->h, angle,
               sprite->w / 2, sprite->w / 2);
}
