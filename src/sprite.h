#ifndef SPRITE_H
#define SPRITE_H

#include "image.h"

typedef struct Sprite Sprite;

extern Sprite *sprite_create(Image *image);
extern void sprite_destroy(Sprite *sprite);
extern void sprite_draw(Sprite *sprite, float x, float y);

#endif
