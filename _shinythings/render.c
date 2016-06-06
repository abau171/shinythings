#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "geometry.h"
#include "color.h"
#include "scene.h"

#include "render.h"

static vector_t camera = {0.0, 0.0, 3.0};
static vector_t camera_look = {0.0, 0.0, -3.0};
static vector_t camera_right = {1.92, 0.0, 0.0};
static vector_t camera_down = {0.0, -1.08, 0.0};

static sphere_t sphere_a = {{0.0, 0.0, -1.0}, 0.5};
static plane_t plane_a = {{0.0, -0.5, 0.0}, {0.0, 1.0, 0.0}};
static light_t light_a = {{1.0, 1.0, 1.0}};

static bool trace_ray_object(vector_t ray_start, vector_t ray_direction, vector_t* hit, vector_t* normal)
{
    float direction_magnitude = vector_magnitude(ray_direction);
    ray_start = vector_add(ray_start, vector_scale(ray_direction, 0.01 / direction_magnitude));

    vector_t sphere_hit;
    bool sphere_success = sphere_intersect(sphere_a, camera, ray_direction, &sphere_hit);
    if (sphere_success) {
        *hit = sphere_hit;
        *normal = vector_normalize(vector_sub(sphere_hit, sphere_a.center));
        return true;
    }

    vector_t plane_hit;
    bool plane_success = plane_intersect(plane_a, camera, ray_direction, &plane_hit);
    if (plane_success) {
        *hit = plane_hit;
        *normal = plane_a.normal;
        return true;
    }

    return false;
}

static color_t get_diffuse_color(vector_t hit, vector_t normal)
{
    vector_t to_light = vector_sub(light_a.position, hit);
    vector_t obj_hit, obj_normal;
    if (!trace_ray_object(hit, to_light, &obj_hit, &obj_normal)) {
        float distance_2 = vector_magnitude_2(to_light);
        float diffuse_scalar = fmaxf(0.0, vector_dot(to_light, normal) / sqrtf(distance_2));
        float intensity = 1.0 / distance_2;
        return (color_t) {diffuse_scalar * intensity, diffuse_scalar * intensity, diffuse_scalar * intensity};
    }
    return (color_t) {0.0, 0.0, 0.0}; // AMBIENT
}

static color_t trace_ray(vector_t ray_start, vector_t ray_direction)
{
    vector_t obj_hit, obj_normal;
    bool obj_success = trace_ray_object(ray_start, ray_direction, &obj_hit, &obj_normal);
    if (obj_success) {
        return get_diffuse_color(obj_hit, obj_normal);
    }
    return (color_t) {0.0, 0.0, 0.0}; // BACKGROUND
}

static color_t get_screen_color(float x, float y)
{
    vector_t ray_direction = vector_add(camera_look,
        vector_add(vector_scale(camera_right, x),
                   vector_scale(camera_down, y)));

    return trace_ray(camera, ray_direction);
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

