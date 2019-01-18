#ifndef DEBUG_H
#define DEBUG_H

#ifndef NDEBUG
    /*
     * Show a debug message.
     */
    extern void debug_printf_internal(const char *format, ...);
    #define debug_printf(...) debug_printf_internal(__FILE__, __VA_ARGS__)
#else
    /*
     * Dummy debug function.
     */
    #define debug_printf(...) do {} while (0)
#endif

#endif /* DEBUG_H */
