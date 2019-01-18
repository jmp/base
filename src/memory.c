/*
 * The original reallocarray was written by Otto Moerbeek, and the below
 * implementation has been taken from the OpenBSD project. It is therefore
 * under the following license:
 *
 * Copyright (c) 2008 Otto Moerbeek <otto@drijf.net>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"
#include "error.h"
#include "debug.h"

/*
 * This is sqrt(SIZE_MAX+1), as s1*s2 <= SIZE_MAX
 * if both s1 < MUL_NO_OVERFLOW and s2 < MUL_NO_OVERFLOW.
 */
#define MUL_NO_OVERFLOW ((size_t) 1 << (sizeof(size_t) * 4))

/*
 * Returns 1 if nmemb * size overflows, 0 otherwise.
 */
#define CHECK_OVERLOW(count, size) ( \
        (count >= MUL_NO_OVERFLOW || size >= MUL_NO_OVERFLOW) \
        && count > 0 \
        && SIZE_MAX / count < size \
    )

static unsigned long long num_allocs = 0ULL;
static unsigned long long num_reallocs = 0ULL;
static unsigned long long num_frees = 0ULL;

void *memory_alloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) {
        error("Out of memory.\n");
    } else {
        ++num_allocs;
    }
    return p;
}

void *memory_realloc(void *memory, size_t size)  {
    void *p = realloc(memory, size);
    if (!p) {
        error("Failed to reallocate a memory block to %d bytes.\n", size);
    } else {
        ++num_reallocs;
    }
    return p;
}

void *memory_allocarray(size_t count, size_t size) {
    if (CHECK_OVERLOW(count, size)) {
        error("Overflow when allocating an array.\n");
    }
    return memory_alloc(size * count);
}

void *memory_reallocarray(void *memory, size_t count, size_t size)  {
    if (CHECK_OVERLOW(count, size)) {
        error("Overflow when reallocating an array.\n");
    }
    return memory_realloc(memory, size * count);
}

void memory_free(void *p) {
    if (p) {
        free(p);
        ++num_frees;
    } else {
        /*
         * Freeing a NULL pointer is not dangerous, but
         * might be an indication of a bug somewhere else.
         * So, just quit before something weird happens.
         */
        error("Attempted to free a NULL pointer.\n");
    }
}

void memory_stats(void) {
    debug_printf("Listing memory statistics...\n");
    debug_printf("  %llu allocations\n", num_allocs);
    debug_printf("  %llu frees\n", num_frees);
    debug_printf("  %llu reallocations\n", num_reallocs);
    debug_printf("End of memory statistics.\n");
    if (num_allocs != num_frees) {
        debug_printf("WARNING: The number of allocations"
                     "does not match the number of frees!");
    }
}
