#include <SDL2/SDL.h>
#include "image.h"
#include "config.h"
#include "memory.h"
#include "window.h"
#include "file.h"
#include "rwops.h"
#include "error.h"
#include "debug.h"

struct Image {
    const char *filename;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

static SDL_RWops *open_rwops(const char *filename) {
    SDL_RWops *rwops = rwops_open_read(filename);
    if (rwops == NULL) {
        error("Failed to open %s for reading: %s\n", filename, SDL_GetError());
    }
    return rwops;
}

static SDL_Surface *load_surface(SDL_RWops *rwops) {
    SDL_Surface *surface = SDL_LoadBMP_RW(rwops, 1);
    if (surface == NULL) {
        error("Failed to load surface: %s\n", SDL_GetError());
    }
    return surface;
}

static SDL_Texture *create_texture(SDL_Renderer *renderer, SDL_Surface *surface) {
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        error("Failed to create texture: %s\n", SDL_GetError());
    }
    return texture;
}

static void set_texture_blending(SDL_Texture *texture) {
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) < 0) {
        error("Failed to set blend mode: %s\n", SDL_GetError());
    }
}

Image *image_load(const char *filename) {
    SDL_RWops *rwops = open_rwops(filename);
    SDL_Surface *surface = load_surface(rwops);
    SDL_Texture *texture = create_texture(window_renderer, surface);

    set_texture_blending(texture);

    Image *image = memory_alloc(sizeof(Image));
    image->surface = surface;
    image->texture = texture;
    image->filename = filename;

    debug_printf("Image %s loaded.\n", filename);

    return image;
}

void image_free(Image *image) {
    const char *filename;

    if (image == NULL) {
        error("Attempting to free an already freed image.\n");
        return;
    }

    filename = image->filename;
    SDL_DestroyTexture(image->texture);
    SDL_FreeSurface(image->surface);
    memory_free(image);
    debug_printf("Image %s freed.\n", filename);
}

void image_dimensions(Image *image, int *w, int *h) {
    *w = image->surface->w;
    *h = image->surface->h;
}

int image_get_width(Image *image) {
    return image->surface->w;
}

int image_get_height(Image *image) {
    return image->surface->h;
}

void image_draw(Image *image, float x, float y, float w, float h,
                float angle, float cx, float cy) {
    SDL_Point center;
    SDL_Rect rect;

    center.x = cx;
    center.y = cy;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_RenderCopyEx(window_renderer, image->texture, NULL, &rect, angle,
                     &center, SDL_FLIP_NONE);
}
