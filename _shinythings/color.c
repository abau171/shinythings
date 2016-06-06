#include "color.h"

color_t color_add(color_t a, color_t b) {
    return (color_t) {a.r + b.r,
                      a.g + b.g,
                      a.b + b.b};
}

