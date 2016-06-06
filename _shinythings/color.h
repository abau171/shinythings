#ifndef COLOR_H
#define COLOR_H

typedef struct {
    float r, g, b;
} color_t;

color_t color_add(color_t a, color_t b);

#endif

