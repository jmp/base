#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

/*
 * Allocate memory. This is just a wrapper around malloc(), also keeping track
 * of the number of allocations and the number of bytes allocated, for  debug
 * purposes. If there is not enough memory, an error message is shown and the
 * program will exit.
 */
extern void *memory_alloc(size_t bytes);

/*
 * Reallocates memory. This is a wrapper around realloc(), but also does some
 * book-keeping for debugging purposes. If there is not enough memory, an
 * error message is shown and the program will exit.
 */
extern void *memory_realloc(void *memory, size_t size);

/*
 * Allocates an array. This function is meant specifically for allocating
 * arrays safely by checking for the overflow of size * count.
 */
extern void *memory_allocarray(size_t count, size_t size);

/*
 * Reallocates an array. This function is meant specifically for reallocating
 * arrays safely by checking for the overflow of size * count.
 */
extern void *memory_reallocarray(void *memory, size_t count, size_t size);

/*
 * Free a previously allocated block of memory. Also counts the number of times
 * free() has been called and throws an error if we accidentally try to free
 * a NULL pointer.
 */
extern void memory_free(void *memory);

/*
 * Print some useful stats about memory allocations.
 */
extern void memory_stats(void);

#endif
