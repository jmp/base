#ifndef OBJECT_H
#define OBJECT_H

#include "image.h"

typedef struct Object Object;

extern Object *object_create(Image *image);
extern void object_destroy(Object *object);
extern void object_update(Object *object);
extern void object_draw(Object *object);
extern void object_draw_lerp(Object *object, float fraction);
extern void object_move(Object *object, float dx, float dy);
extern void object_get_pos(Object *object, float *x, float *y);
extern void object_get_pos_lerped(Object *object, float *x, float *y,
    float fraction);
extern void object_set_pos(Object *object, float x, float y);

#endif
