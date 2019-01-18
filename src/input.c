#include <SDL2/SDL.h>
#include "input.h"

static const Uint8 *key_states;

void input_update(void) {
    key_states = SDL_GetKeyboardState(NULL);
}

int input_key_pressed(int key) {
    return input_scancode_pressed(SDL_GetScancodeFromKey(key));
}

int input_scancode_pressed(int scancode) {
    if (key_states == NULL) {
        input_update();
    }
    return key_states[scancode];
}
