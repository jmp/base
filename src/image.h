#ifndef IMAGE_H
#define IMAGE_H

typedef struct Image Image;

extern Image *image_load(const char *filename);
extern void image_free(Image * image);
extern void image_dimensions(Image *image, int *w, int *h);
extern int image_get_width(Image *image);
extern int image_get_height(Image *image);
extern void image_draw(Image *image, float x, float y, float w, float h,
                       float angle, float cx, float cy);

#endif
