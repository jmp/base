#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

extern SDL_Renderer *window_renderer;
extern SDL_Texture *window_target;
extern SDL_Window *window;

extern void window_init(void);
extern void window_quit(void);
extern void window_show(void);
extern void window_hide(void);
extern int window_handle_events(void);
extern void window_clear(unsigned char r, unsigned char g, unsigned char b);
extern void window_flip(void);


#endif
