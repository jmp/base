#include <SDL2/SDL.h>
#include "timer.h"

uint32_t timer_get_ticks(void) {
    return SDL_GetTicks();
}

void timer_sleep(uint32_t milliseconds) {
    SDL_Delay(milliseconds);
}
