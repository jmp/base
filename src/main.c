#include "config.h"
#include "window.h"
#include "asset.h"
#include "sound.h"
#include "state.h"
#include "game.h"
#include "timer.h"
#include "rwops.h"
#include "memory.h"
#include "debug.h"

#define TIMESTEP 16 /* Milliseconds */

/*
 * Main loop.
 */
static void loop(void) {
    int quit = 0;

    /* Timing-related variables, in milliseconds */
    uint32_t accumulator = 0;
    uint32_t current_time = timer_get_ticks();
    uint32_t previous_time = current_time - TIMESTEP;

    debug_printf("Entering main loop...\n");
    window_show();

    /* Loop for as long as the current state remains unchanged */
    while (!quit) {
        current_time = timer_get_ticks();
        accumulator += current_time - previous_time;
        previous_time = current_time;

        /*
         * The accumulator must be exhausted before drawing the state.
         * This usually makes the game run at equal speed on slower
         * computers, even if the graphics card can't render as fast.
         */
        while (accumulator >= TIMESTEP) {
            quit = state_update();
            accumulator -= TIMESTEP;
        }

        /* Render everything */
        state_draw(accumulator / (float) TIMESTEP);

        /* Don't hog all CPU time */
        timer_sleep(1);
    }

    debug_printf("Main loop finished.\n");

    /* Clean up if needed */
    debug_printf("Cleaning up final state...\n");
    state_quit();
    debug_printf("Final state cleaned up.\n");

    window_hide();
}

/*
 * Initialize everything
 */
static void init(char *program_name) {
    debug_printf("Initializing all modules...\n");
    config_load();
    rwops_init(program_name);
    window_init();
    sound_init();
    asset_init();
    debug_printf("All modules initialized.\n");

    /* Set initial state */
    debug_printf("Setting initial state...\n");
    state_set(game_state);
    debug_printf("Initial state set.\n");
}

/*
 * Clean up everything
 */
static void quit(void) {
    debug_printf("Shutting down all modules...\n");
    asset_quit();
    sound_quit();
    window_quit();
    rwops_quit();
    debug_printf("All modules shut down.\n");
    config_save();

    /* Print some memory stats */
    memory_stats();
}

/*
 * Program entry point.
 */
int main(int argc, char *argv[]) {
    debug_printf("Let's go!\n");
    init(argv[0]);
    loop();
    quit();
    debug_printf("All done!\n");

    return 0;
}
