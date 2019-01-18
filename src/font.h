#ifndef FONT_H
#define FONT_H

typedef struct Font Font;

/*
 * Load a bitmap font.
 */
extern Font *font_load(const char *filename);

/*
 * Destroy the font and free all the memory used by it.
 */
extern void font_free(Font *font);

/*
 * Draw text at the given coordinates using the given bitmap font.
 */
extern void font_draw(Font *font, const char *text, int x, int y);


/*
 * Set the color used for drawing the given bitmap font.
 */
extern void font_set_color(Font *font, unsigned char r, unsigned char g,
                           unsigned char b);

#endif
