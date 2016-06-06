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

static color_t ambient = {0.03, 0.03, 0.03};

static sphere_model_t sphere_a = {{{0.0, 0.0, -1.0}, 0.5}, {{0.0, 0.4, 1.0}, 0.3, 20.0}};
static plane_model_t plane_a = {{{0.0, -0.5, 0.0}, {0.0, 1.0, 0.0}}, {{1.0, 0.4, 0.0}, 0.0, 1.0}};
static light_t light_a = {{3.0, 3.0, 3.0}, {10.0, 10.0, 10.0}};

static bool trace_ray_object(vector_t ray_start, vector_t ray_direction, vector_t* hit, vector_t* normal, surface_t** surface)
{
    ray_start = vector_add(ray_start, vector_scale(ray_direction, 0.01));

    vector_t sphere_hit;
    bool sphere_success = sphere_intersect(sphere_a.sphere, camera, ray_direction, &sphere_hit);
    if (sphere_success) {
        *hit = sphere_hit;
        *normal = vector_normalize(vector_sub(sphere_hit, sphere_a.sphere.center));
        *surface = &sphere_a.surface;
        return true;
    }

    vector_t plane_hit;
    bool plane_success = plane_intersect(plane_a.plane, camera, ray_direction, &plane_hit);
    if (plane_success) {
        *hit = plane_hit;
        *normal = plane_a.plane.normal;
        *surface = &plane_a.surface;
        return true;
    }

    return false;
}

static color_t get_specular_color(vector_t hit, vector_t normal, vector_t incoming, surface_t* surface)
{
    vector_t to_light = vector_normalize(vector_sub(light_a.position, hit));
    vector_t light_reflection = vector_sub(
        vector_scale(normal, 2 * vector_dot(to_light, normal)),
        to_light);
    float specular_scalar = surface->specular * powf(fmaxf(0.0, -vector_dot(incoming, light_reflection)), surface->shininess);
    return color_scale(light_a.color, specular_scalar);
}

static color_t get_diffuse_color(vector_t hit, vector_t normal, surface_t* surface)
{
    vector_t to_light = vector_sub(light_a.position, hit);
    float light_distance_2 = vector_magnitude_2(to_light);
    to_light = vector_normalize(to_light);
    vector_t obj_hit, obj_normal;
    surface_t* obj_surface;
    if (!trace_ray_object(hit, to_light, &obj_hit, &obj_normal, &obj_surface)) {
        float diffuse_scalar = fmaxf(0.0, vector_dot(to_light, normal)) / light_distance_2;
        return color_scale(color_mult(light_a.color, surface->color), diffuse_scalar);
    }
    return (color_t) {0.0, 0.0, 0.0};
}

static color_t get_ambient_color(surface_t* surface)
{
    return color_mult(ambient, surface->color);
}

static color_t trace_ray(vector_t ray_start, vector_t ray_direction)
{
    vector_t obj_hit, obj_normal;
    surface_t* obj_surface;
    bool obj_success = trace_ray_object(ray_start, ray_direction, &obj_hit, &obj_normal, &obj_surface);
    if (obj_success) {
        return color_add(color_add(get_ambient_color(obj_surface),
                                   get_diffuse_color(obj_hit, obj_normal, obj_surface)),
                                   get_specular_color(obj_hit, obj_normal, ray_direction, obj_surface));
    }
    return (color_t) {0.0, 0.0, 0.0};
}

static color_t get_screen_color(float x, float y)
{
    vector_t ray_direction = vector_normalize(
        vector_add(vector_add(
            camera_look,
            vector_scale(camera_right, x)),
            vector_scale(camera_down, y)));

    return trace_ray(camera, ray_direction);
}

static color_t get_pixel(int width, int height, int px, int py)
{
    float x = (px + 0.5) / width - 0.5;
    float y = (py + 0.5) / height - 0.5;
    return color_clamp(get_screen_color(x, y));
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

