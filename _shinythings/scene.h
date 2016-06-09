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
    int a, b, c;
} triangle_t;

typedef struct {
    bool is_leaf;
    box_t bbox;
} kd_node_t;

typedef struct {
    int num_vertices, num_triangles;
    vector_t* vertices;
    triangle_t* triangles;
    vector_t* triangle_normals;
    vector_t* vertex_normals;
    kd_node_t* kd_tree;
    surface_t surface;
} tri_model_t;

typedef struct {
    int num_spheres, num_planes, num_lights, num_models;
    sphere_model_t* spheres;
    plane_model_t* planes;
    light_t* lights;
    tri_model_t* models;
    vector_t camera, camera_look, camera_right, camera_down;
    color_t background_color, ambient_color;
} scene_t;

void init_scene_components(scene_t* scene);
void free_scene_components(scene_t* scene);

#endif

