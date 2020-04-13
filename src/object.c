#include "object.h"
#include "sprite.h"
#include "memory.h"

struct Object {
    float x, y;
    float prev_x, prev_y;
    Sprite *sprite;
};

static float lerp(float v0, float v1, float t) {
    return v1 * t + v0 * (1.0f - t);
}

Object *object_create(Image *image) {
    Object *object = memory_alloc(sizeof(Object));
    object->x = 0.0f;
    object->y = 0.0f;
    object->prev_x = 0.0f;
    object->prev_y = 0.0f;
    object->sprite = sprite_create(image);
    return object;
}

void object_destroy(Object *object) {
    sprite_destroy(object->sprite);
    memory_free(object);
}

void object_update(Object *object) {
    object->prev_x = object->x;
    object->prev_y = object->y;
}

void object_draw(Object *object) {
    sprite_draw(object->sprite, object->x, object->y);
}

void object_draw_lerp(Object *object, float fraction) {
    float x = lerp(object->prev_x, object->x, fraction);
    float y = lerp(object->prev_y, object->y, fraction);
    sprite_draw(object->sprite, x, y);
}

void object_move(Object *object, float dx, float dy) {
    object->x += dx;
    object->y += dy;
}

void object_get_pos(Object *object, float *x, float *y) {
    *x = object->x;
    *y = object->y;
}

void object_get_pos_lerped(Object *object, float *x, float *y, float fraction) {
    *x = lerp(object->prev_x, object->x, fraction);
    *y = lerp(object->prev_y, object->y, fraction);
}

void object_set_pos(Object *object, float x, float y) {
    object->x = x;
    object->y = y;
}
