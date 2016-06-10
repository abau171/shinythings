#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "geometry.h"
#include "color.h"
#include "scene.h"
#include "kd_tree.h"

#include "render.h"

static bool model_intersect(tri_model_t* model, vector_t ray_start, vector_t ray_direction, vector_t* hit, vector_t* normal)
{
    float t = box_intersects(model->kd_tree->bbox, ray_start, ray_direction);
    if (t >= 0.0)
        return kd_intersect(model, model->kd_tree, ray_start, ray_direction, hit, normal);
    return false;
}

static bool trace_ray_object(scene_t* scene, vector_t ray_start, vector_t ray_direction, vector_t* hit, vector_t* normal, surface_t** surface)
{
    ray_start = vector_add(ray_start, vector_scale(ray_direction, 0.0001));

    float closest_distance_2 = -1.0;
    vector_t closest_hit, closest_normal;
    surface_t* closest_surface = NULL;

    vector_t sphere_hit;
    for (int i = 0; i < scene->num_spheres; i++) {
        if (sphere_intersect(scene->spheres[i].sphere, ray_start, ray_direction, &sphere_hit)) {
            float distance_2 = vector_distance_2(ray_start, sphere_hit);
            if (closest_distance_2 < 0.0 || distance_2 < closest_distance_2) {
                closest_hit = sphere_hit;
                closest_normal = vector_normalize(vector_sub(sphere_hit, scene->spheres[i].sphere.center));
                closest_surface = &scene->spheres[i].surface;
                closest_distance_2 = distance_2;
            }
        }
    }

    vector_t plane_hit;
    for (int i = 0; i < scene->num_planes; i++) {
        if (plane_intersect(scene->planes[i].plane, ray_start, ray_direction, &plane_hit)) {
            float distance_2 = vector_distance_2(ray_start, plane_hit);
            if (closest_distance_2 < 0.0 || distance_2 < closest_distance_2) {
                closest_hit = plane_hit;
                closest_normal = scene->planes[i].plane.normal;
                closest_surface = &scene->planes[i].surface;
                closest_distance_2 = distance_2;
            }
        }
    }

    vector_t model_hit, model_normal;
    for (int i = 0; i < scene->num_models; i++) {
        tri_model_t* model = &scene->models[i];
        if (model_intersect(model, ray_start, ray_direction, &model_hit, &model_normal)) {
            float distance_2 = vector_distance_2(ray_start, model_hit);
            if (closest_distance_2 < 0.0 || distance_2 < closest_distance_2) {
                closest_hit = model_hit;
                closest_normal = model_normal;
                closest_surface = &scene->models[i].surface;
                closest_distance_2 = distance_2;
            }
        }
    }

    if (closest_distance_2 < 0.0) {
        return false;
    } else {
        *hit = closest_hit;
        *normal = closest_normal;
        *surface = closest_surface;
        return true;
    }
}

static color_t get_specular_color(scene_t* scene, vector_t hit, vector_t normal, vector_t incoming, surface_t* surface)
{
    color_t specular_sum = {0.0, 0.0, 0.0};
    for (int i = 0; i < scene->num_lights; i++) {
        vector_t to_light = vector_normalize(vector_sub(scene->lights[i].position, hit));
        float light_dot_normal = vector_dot(to_light, normal);
        if (light_dot_normal < 0.0)
            continue;
        vector_t obj_hit, obj_normal;
        surface_t* obj_surface;
        if (!trace_ray_object(scene, hit, to_light, &obj_hit, &obj_normal, &obj_surface)) {
            vector_t light_reflection = vector_sub(
                vector_scale(normal, 2 * light_dot_normal),
                to_light);
            float specular_scalar = surface->specular * powf(fmaxf(0.0, -vector_dot(incoming, light_reflection)), surface->shininess);
            specular_sum = color_add(specular_sum, color_scale(scene->lights[i].color, specular_scalar));
        }
    }
    return specular_sum;
}

static color_t get_diffuse_color(scene_t* scene, vector_t hit, vector_t normal, surface_t* surface)
{
    color_t diffuse_sum = {0.0, 0.0, 0.0};
    for (int i = 0; i < scene->num_lights; i++) {
        vector_t to_light = vector_sub(scene->lights[i].position, hit);
        float light_distance_2 = vector_magnitude_2(to_light);
        to_light = vector_normalize(to_light);
        vector_t obj_hit, obj_normal;
        surface_t* obj_surface;
        if (!trace_ray_object(scene, hit, to_light, &obj_hit, &obj_normal, &obj_surface)) {
            float diffuse_scalar = fmaxf(0.0, vector_dot(to_light, normal)) / light_distance_2;
            diffuse_sum = color_add(diffuse_sum, color_scale(color_mult(scene->lights[i].color, surface->color), diffuse_scalar));
        }
    }
    return diffuse_sum;
}

static color_t get_ambient_color(scene_t* scene, surface_t* surface)
{
    return color_mult(scene->ambient_color, surface->color);
}

static color_t trace_ray(scene_t* scene, vector_t ray_start, vector_t ray_direction)
{
    vector_t obj_hit, obj_normal;
    surface_t* obj_surface;
    if (trace_ray_object(scene, ray_start, ray_direction, &obj_hit, &obj_normal, &obj_surface)) {
        return color_add(color_add(get_ambient_color(scene, obj_surface),
                                   get_diffuse_color(scene, obj_hit, obj_normal, obj_surface)),
                                   get_specular_color(scene, obj_hit, obj_normal, ray_direction, obj_surface));
    }
    return scene->background_color;
}

static color_t get_screen_color(scene_t* scene, float x, float y)
{
    vector_t ray_direction = vector_normalize(
        vector_add(vector_add(
            scene->camera_look,
            vector_scale(scene->camera_right, x)),
            vector_scale(scene->camera_down, y)));

    return trace_ray(scene, scene->camera, ray_direction);
}

static color_t get_pixel(scene_t* scene, int width, int height, int px, int py)
{
    float x = (px + 0.5) / width - 0.5;
    float y = (py + 0.5) / height - 0.5;
    return color_clamp(get_screen_color(scene, x, y));
}

uint8_t* render(scene_t* scene, int width, int height)
{
    uint8_t* image_data = malloc(width * height * 3 * sizeof(uint8_t));

    for (int px = 0; px < width; px++) {
        for (int py = 0; py < height; py++) {
            color_t pixel_color = get_pixel(scene, width, height, px, py);
            int base = 3 * (px + width * py);
            image_data[base+0] = 255 * pixel_color.r;
            image_data[base+1] = 255 * pixel_color.g;
            image_data[base+2] = 255 * pixel_color.b;
        }
    }

    return image_data;
}

