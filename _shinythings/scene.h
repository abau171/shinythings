#ifndef SCENE_H
#define SCENE_H

#include "geometry.h"

typedef struct {
    color_t color;
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
} light_t;

#endif

