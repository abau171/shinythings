#include <math.h>

#include "color.h"

color_t color_add(color_t a, color_t b)
{
    return (color_t) {a.r + b.r,
                      a.g + b.g,
                      a.b + b.b};
}

color_t color_mult(color_t a, color_t b)
{
    return (color_t) {a.r * b.r,
                      a.g * b.g,
                      a.b * b.b};
}

color_t color_scale(color_t c, float scalar)
{
    return (color_t) {c.r * scalar,
                      c.g * scalar,
                      c.b * scalar};
}

static float clampf(float x)
{
    return fminf(1.0, fmaxf(0.0, x));
}

color_t color_clamp(color_t c)
{
    return (color_t) {clampf(c.r),
                      clampf(c.g),
                      clampf(c.b)};
}

