#include "state.h"
#include "config.h"
#include "input.h"
#include "window.h"
#include "error.h"
#include "debug.h"

/* Keep track of the current state */
static State *state;

/*
 * Set the next state
 */
void state_set(State *new_state) {
    debug_printf("Changing state...\n");

    /* Quit the current state if it exists */
    if (state) {
        state->quit();
    }

    state = new_state;

    /* The state might be NULL, which means we want to quit */
    if (state) {
        state->init();
    }

    debug_printf("State changed.\n");
}

/*
 * Update the given state by first updating user input and then running the
 * update function of the current state. Returns 1 if the game should exit,
 * 0 otherwise.
 */
int state_update(void) {
    // Update user input and window events
    input_update();

    // In case the user wants to close the window, quit as fast as possible
    if (!state || window_handle_events() ||
        input_scancode_pressed(config.key_cancel)) {
        return 1;
    }

    state->update();
    return 0;
}

/*
 * Render the given state
 */
void state_draw(float fraction) {
    if (state) {
        state->draw(fraction);
    }
}

/*
 * Quit the current state.
 */
void state_quit(void) {
    debug_printf("Quitting state...\n");

    if (state) {
        state->quit();
    }

    debug_printf("State quit.\n");
}
