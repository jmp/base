#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

/*
 * Returns the number of milliseconds since SDL initialization.
 */
extern uint32_t timer_get_ticks(void);

/*
 * Delays execution for the given number of milliseconds.
 */
extern void timer_sleep(uint32_t milliseconds);

#endif /* TIMER_H */
