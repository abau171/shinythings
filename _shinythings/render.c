#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "geometry.h"
#include "color.h"

#include "render.h"

static vector_t camera = {0.0, 0.0, 3.0};
static vector_t camera_look = {0.0, 0.0, -3.0};
static vector_t camera_right = {1.92, 0.0, 0.0};
static vector_t camera_down = {0.0, -1.08, 0.0};

static sphere_t sphere_a = {{0.0, 0.0, -1.0}, 0.5};

static color_t get_screen_color(float x, float y)
{
    vector_t ray_direction = vector_add(camera_look,
        vector_add(vector_scale(camera_right, x),
                   vector_scale(camera_down, y)));
    vector_t sphere_hit;
    bool sphere_success = sphere_intersect(sphere_a, camera, ray_direction, &sphere_hit);
    if (sphere_success) {
        return (color_t) {0.0, 0.4, 1.0};
    } else {
        return (color_t) {0.0, 0.0, 0.0};
    }
}

static color_t get_pixel(int width, int height, int px, int py)
{
    float x = (px + 0.5) / width - 0.5;
    float y = (py + 0.5) / height - 0.5;
    return get_screen_color(x, y);
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

