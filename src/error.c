#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

void error(const char *format, ...) {
    va_list args;
    fprintf(stderr, "**************************************************************************\n");
    fprintf(stderr, "An unexpected error has occurred:\n");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "The program will now exit.\n");
    fprintf(stderr, "**************************************************************************\n");
    exit(EXIT_FAILURE);
}
