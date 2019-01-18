#include "game.h"
#include "state.h"
#include "config.h"
#include "window.h"
#include "timer.h"
#include "input.h"
#include "object.h"
#include "font.h"
#include "sound.h"
#include "asset.h"

static Object *player;

static void init(void) {
    player = object_create(assets.image_smile);
    /*sound_play(sound_pick);*/
}

static void quit(void) {
    object_destroy(player);
}

static void update(void) {
    object_update(player);
    if (input_scancode_pressed(config.key_up))
        object_move(player, 0, -10);
    if (input_scancode_pressed(config.key_down))
        object_move(player, 0, 10);
    if (input_scancode_pressed(config.key_left))
        object_move(player, -10, 0);
    if (input_scancode_pressed(config.key_right))
        object_move(player, 10, 0);
}

static void draw(float fraction) {
    float x, y;
    object_get_pos_lerped(player, &x, &y, fraction);
    window_clear(50, 50, 50);
    object_draw_lerp(player, fraction);
    font_draw(assets.font_basic, "hi!", (int) x, (int) y - 20);
    font_set_color(assets.font_basic, 0, 0, 0);
    font_draw(assets.font_basic, "testing!\nthis is a test...", 102, 102);
    font_set_color(assets.font_basic, 255, 255, 255);
    font_draw(assets.font_basic, "testing!\nthis is a test...", 100, 100);
    window_flip();
}

static State state = { init, quit, update, draw };

State *game_state = &state;
