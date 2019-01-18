#ifndef PHYSFSRWOPS_H
#define PHYSFSRWOPS_H

#include <SDL2/SDL.h>

extern void rwops_init(const char *program_name);
extern void rwops_quit(void);
extern SDL_RWops *rwops_open_read(const char *fname);

#endif
