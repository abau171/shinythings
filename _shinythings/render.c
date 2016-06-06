#include <stdlib.h>
#include <stdint.h>

#include "render.h"

typedef struct {
    float r, g, b;
} color_t;

color_t get_pixel(int width, int height, int px, int py)
{
    return (color_t) {(float) px / width,
                      (float) py / height,
                      0.0};
}

uint8_t* render(int width, int height)
{
    uint8_t* image_data = malloc(width * height * 3 * sizeof(uint));

    for (int px = 0; px < width; px++) {
        for (int py = 0; py < height; py++) {
            color_t pixel_color = get_pixel(width, height, px, py);
            int base = 3 * (px + width * py);
            image_data[base+0] = 255 * pixel_color.r;
            image_data[base+1] = 255 * pixel_color.g;
            image_data[base+2] = 255 * pixel_color.b;
        }
    }

    return image_data;
}

