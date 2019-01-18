#include <stdio.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "config.h"
#include "error.h"
#include "debug.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Global renderer and render window_target */
SDL_Renderer *window_renderer;
SDL_Texture *window_target;

/* Global window */
SDL_Window *window;

/* Source rectangle */
static SDL_Rect source;

/*
 * Resize the window and adjust viewport based on the new size.
 */
static void resize(void) {
    int window_w, window_h;
    float aspect_ratio;

    /* Default values for the source rectangle */
    source.x = 0;
    source.y = 0;
    source.w = config.draw_w;
    source.h = config.draw_h;
    config.view_x = 0;
    config.view_y = 0;
    config.view_w = config.draw_w;
    config.view_h = config.draw_h;

    SDL_GetWindowSize(window, &window_w, &window_h);

    aspect_ratio = window_w / (float) window_h;

    /* Resize the crop rectangle */
    if (aspect_ratio > config.draw_aspect_ratio) {
        source.h *= window_h / (window_w / config.draw_aspect_ratio);
        source.y = (config.draw_h - source.h) / 2;
    } else if (aspect_ratio < config.draw_aspect_ratio) {
        source.w *= window_w / (window_h * config.draw_aspect_ratio);
        source.x = (config.draw_w - source.w) / 2;
    }

    /* Update viewport */
    config.view_x = source.x;
    config.view_y = source.y;
    config.view_w = source.w;
    config.view_h = source.h;
}

/*
 * Toggle fullscreen mode.
 */
static void toggle_fullscreen(void) {
    Uint32 flags = SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
        flags = 0;
    }

    SDL_SetWindowFullscreen(window, flags);

    config.window_fullscreen = flags ? 1 : 0;
    config.window_borderless = config.window_fullscreen;

    if (!config.window_fullscreen && config.window_maximized) {
        SDL_MaximizeWindow(window);
    }
}

/*
 * Handle key down events.
 */
static void handle_key_down(SDL_Event * event) {
    /* Alt+Enter toggles fullscreen */
    if (event->key.keysym.sym == SDLK_RETURN &&
        (event->key.keysym.mod & KMOD_LALT)) {
        toggle_fullscreen();
    }
}

/*
 * Handle general window events.
 */
static void handle_window_event(SDL_Event * event) {
    switch (event->window.event) {
        /* Window resized */
        case SDL_WINDOWEVENT_RESIZED: {
            Uint32 flags = SDL_GetWindowFlags(window);
            Uint32 maximized = flags & SDL_WINDOW_MAXIMIZED;
            Uint32 fullscreen = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;
            if (!maximized && !fullscreen) {
                config.window_width = event->window.data1;
                config.window_height = event->window.data2;
            }
            resize();
            break;
        }
        /* Window maximized */
        case SDL_WINDOWEVENT_MAXIMIZED: {
            config.window_maximized = 1;
            break;
        }
        /* Window restored after being iconified */
        case SDL_WINDOWEVENT_RESTORED: {
            Uint32 flags = SDL_GetWindowFlags(window);
            Uint32 fullscreen = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;
            if (!fullscreen) {
                config.window_maximized = 0;
            }
            break;
        }
        /* By default, do nothing */
        default:
            break;
    }
}

/*
 * Create and initialize the main window, renderer and target texture.
 */
void window_init(void) {
    Uint32 flags = 0;

    debug_printf("Initializing window...\n");

    /* Set window flags based on the user's configuration */
    flags |= config.window_borderless ? SDL_WINDOW_BORDERLESS : 0;
    flags |= config.window_resizable ? SDL_WINDOW_RESIZABLE : 0;
    flags |= config.window_maximized ? SDL_WINDOW_MAXIMIZED : 0;
    flags |= config.window_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    flags |= SDL_WINDOW_HIDDEN;

    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        error("Failed to initialize SDL: %s\n", SDL_GetError());
    }

    debug_printf("Creating window....\n");

    /* Create the game window */
    window = SDL_CreateWindow(config.window_title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              config.window_width,
                              config.window_height, flags);

    /* Make sure the window was opened successfully */
    if (!window) {
        error("Failed to create window: %s\n", SDL_GetError());
    }

    debug_printf("Window created.\n");
    debug_printf("Creating renderer...\n");

    /* Create the window_renderer */
    if (!(window_renderer = SDL_CreateRenderer(window, -1, 0))) {
        error("Failed to create renderer: %s\n", SDL_GetError());
    }

    debug_printf("Renderer created.\n");
    debug_printf("Creating render target...\n");

    /* Create the render target */
    if (!(window_target = SDL_CreateTexture(window_renderer,
                                            SDL_GetWindowPixelFormat(window),
                                            SDL_TEXTUREACCESS_TARGET,
                                            config.draw_w, config.draw_h))) {
        error("Failed to create render target: %s\n", SDL_GetError());
    }

    debug_printf("Render target created.\n");
    debug_printf("Setting render target...\n");

    /* Set render target */
    if (SDL_SetRenderTarget(window_renderer, window_target) < 0) {
        error("Failed to set render target: %s\n", SDL_GetError());
    }

    debug_printf("Render target set.\n");

    resize();

    debug_printf("Window initialized.\n");
}

/*
 * Destroy the target texture, renderer and main window.
 */
void window_quit(void) {
    debug_printf("Shutting down window...\n");

    debug_printf("Destroying render target...\n");
    SDL_DestroyTexture(window_target);
    debug_printf("Render target destroyed.\n");

    debug_printf("Destroying renderer...\n");
    SDL_DestroyRenderer(window_renderer);
    debug_printf("Renderer destroyed.\n");

    debug_printf("Destroying window...\n");
    SDL_DestroyWindow(window);
    debug_printf("Window destroyed.\n");

    SDL_Quit();
    debug_printf("Window shut down.\n");
}

/*
 * Show the game window.
 */
void window_show(void) {
    /*
       SDL_MaximizeWindow(window);
       SDL_Delay(1000);
       if (!config.window_maximized) {
       SDL_RestoreWindow(window);
       SDL_Delay(1000);
       }
       if (config.window_fullscreen) {
       SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
       SDL_Delay(1000);
       }
       SDL_Delay(1000);
     */
    /* SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best"); */
    SDL_ShowWindow(window);
}

/*
 * Hide the game window.
 */
void window_hide(void) {
    SDL_HideWindow(window);
}

/*
 * Handle events.
 */
int window_handle_events(void) {
    SDL_Event event;
    /* Handle events in the queue */
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            /* Window closed; exit the game */
            return 1;
        } else if (event.type == SDL_KEYDOWN) {
            /* Handle key press */
            handle_key_down(&event);
        } else if (event.type == SDL_WINDOWEVENT) {
            /* Handle window events like resize */
            handle_window_event(&event);
        }
    }

    return 0; /* Continue running */
}

/*
 * Fill the whole window with the given color.
 */
void window_clear(unsigned char r, unsigned char g, unsigned char b) {
    SDL_SetRenderDrawColor(window_renderer, r, g, b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window_renderer);
}

/*
 * Show everything drawn to the target texture on the screen.
 */
void window_flip(void) {
    const SDL_Rect rect = {
        config.view_x,
        config.view_y,
        config.view_w,
        config.view_h
    };

    SDL_SetRenderTarget(window_renderer, NULL);
    SDL_RenderCopy(window_renderer, window_target, &rect, NULL);
    SDL_RenderPresent(window_renderer);
    SDL_SetRenderTarget(window_renderer, window_target);
}
