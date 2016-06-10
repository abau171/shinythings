#ifndef COLOR_H
#define COLOR_H

typedef struct {
    float r, g, b;
} color_t;

color_t color_add(color_t a, color_t b);
color_t color_mult(color_t a, color_t b);
color_t color_exp(float base, color_t c);
color_t color_scale(color_t c, float scalar);
color_t color_clamp(color_t c);

#endif

