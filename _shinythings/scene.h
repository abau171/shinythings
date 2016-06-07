#ifndef SCENE_H
#define SCENE_H

#include "geometry.h"
#include "color.h"

typedef struct {
    color_t color;
    float specular, shininess;
} surface_t;

typedef struct {
    sphere_t sphere;
    surface_t surface;
} sphere_model_t;

typedef struct {
    plane_t plane;
    surface_t surface;
} plane_model_t;

typedef struct {
    vector_t position;
    color_t color;
} light_t;

typedef struct {
    int num_spheres, num_planes, num_lights;
    sphere_model_t* spheres;
    plane_model_t* planes;
    light_t* lights;
} scene_t;

#endif

