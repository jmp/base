#include <SDL2/SDL.h>
#include "font.h"
#include "file.h"
#include "config.h"
#include "memory.h"
#include "window.h"
#include "rwops.h"
#include "error.h"
#include "debug.h"

#define FONT_SCALE 2
#define FONT_ROWS 7
#define FONT_COLS 32
#define GLYPH_START 32 /* ASCII space */
#define GLYPH_MAX 255
#define GLYPH_COUNT (FONT_ROWS * FONT_COLS)
#define GLYPH_WIDTH 8
#define GLYPH_HEIGHT 8
#define GLYPH_PAD_H 0
#define GLYPH_PAD_V 4

/* Struct representing a bitmap font */
struct Font {
    const char *filename;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color color;
    SDL_Rect glyphs[GLYPH_MAX + 1];
};

Font *font_load(const char *filename) {
    Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_RWops *rwops;
    Uint32 color_key;
    Uint32 i;

    /* Open file for reading */
    if (!(rwops = rwops_open_read(filename))) {
        error("Failed to open %s for reading: %s\n",
              filename, SDL_GetError());
    }

    /* Load surface for writing */
    if (!(surface = SDL_LoadBMP_RW(rwops, 1))) {
        error("Failed to load surface: %s\n", SDL_GetError());
    }

    /* Set the color key for transparent color */
    color_key = SDL_MapRGB(surface->format, 255, 255, 254);
    if (SDL_SetColorKey(surface, SDL_TRUE, color_key) < 0) {
        error("Failed to set color key: %s\n", SDL_GetError());
    }

    /* Create texture for the font */
    if (!(texture = SDL_CreateTextureFromSurface(window_renderer, surface))) {
        error("Failed to create texture: %s\n", SDL_GetError());
    }

    /* Enable blending for the texture */
    if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) < 0) {
        error("Failed to set blend mode: %s\n", SDL_GetError());
    }

    /* Fill basic font information */
    font = memory_alloc(sizeof(Font));
    font->filename = filename;
    font->surface = surface;
    font->texture = texture;
    font->color.r = 255;
    font->color.g = 255;
    font->color.b = 255;

    /* Fill glyph data */
    for (i = 0; i < GLYPH_MAX; i++) {
        Uint32 j = i < GLYPH_COUNT ? i : '?';
        SDL_Rect *glyph = font->glyphs + j + GLYPH_START;
        glyph->x = ((GLYPH_WIDTH + GLYPH_PAD_H) * j) %
                    (FONT_COLS * GLYPH_WIDTH);
        glyph->y = GLYPH_WIDTH * j / (FONT_COLS * GLYPH_WIDTH) *
                   (GLYPH_HEIGHT + GLYPH_PAD_V);
        glyph->w = GLYPH_WIDTH;
        glyph->h = GLYPH_HEIGHT;
    }

#ifndef NDEBUG
    /* List glyph data for debugging purposes */
    debug_printf("Listing font glyphs...\n");
    for (i = GLYPH_START; i < GLYPH_COUNT; i++) {
        SDL_Rect glyph = font->glyphs[i];
        debug_printf("  Glyph %d: x=%d y=%d w=%d h=%d\n", i, glyph.x, glyph.y,
                     glyph.w, glyph.h);
    }
    debug_printf("End of glyphs.\n");
#endif

    debug_printf("Font %s loaded.\n", filename);

    return font;
}

void font_free(Font *font) {
    const char *filename;
    if (font == NULL) {
        /* Exit early to prevent bugs later */
        error("Attempting to free a NULL font.\n");
    }
    filename = font->filename;
    SDL_DestroyTexture(font->texture);
    SDL_FreeSurface(font->surface);
    memory_free(font);
    debug_printf("Font %s freed.\n", filename);
}

void font_draw(Font *font, const char *text, int x, int y) {
    int offset_x = 0;
    int offset_y = 0;
    while (*text) {
        SDL_Rect *src = font->glyphs + *text;
        SDL_Rect dst;

        dst.x = x + offset_x;
        dst.y = y + offset_y;
        dst.w = src->w * FONT_SCALE;
        dst.h = src->h * FONT_SCALE;

        SDL_RenderCopy(window_renderer, font->texture, src, &dst);
        if (*text == '\n') {
            offset_x = 0;
            offset_y += (GLYPH_HEIGHT + GLYPH_PAD_V) * FONT_SCALE;
        } else {
            offset_x += dst.w + GLYPH_PAD_H;
        }
        ++text;
    }
}

void font_set_color(Font *font, unsigned char r, unsigned char g,
                    unsigned char b) {
    font->color.r = r;
    font->color.g = g;
    font->color.b = b;
    if (SDL_SetTextureColorMod(font->texture, r, g, b) < 0) {
        error("Failed to set color mod: %s\n", SDL_GetError());
    }
}
