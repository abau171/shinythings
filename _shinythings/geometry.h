#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdbool.h>

typedef struct {
    float x, y, z;
} vector_t;

vector_t vector_add(vector_t a, vector_t b);
vector_t vector_sub(vector_t a, vector_t b);
vector_t vector_scale(vector_t v, float scalar);
vector_t vector_normalize(vector_t v);
vector_t vector_cross(vector_t a, vector_t b);
float vector_magnitude_2(vector_t v);
float vector_magnitude(vector_t v);
float vector_distance_2(vector_t a, vector_t b);
float vector_distance(vector_t a, vector_t b);
float vector_dot(vector_t a, vector_t b);

typedef struct {
    vector_t center;
    float radius;
} sphere_t;

bool sphere_intersect(sphere_t s, vector_t ray_start, vector_t ray_direction, vector_t* hit);

typedef struct {
    vector_t point;
    vector_t normal;
} plane_t;

bool plane_intersect(plane_t p, vector_t ray_start, vector_t ray_direction, vector_t* hit);

bool triangle_intersect(vector_t a, vector_t b, vector_t c, vector_t normal, vector_t ray_start, vector_t ray_direction, vector_t* hit);

typedef struct {
    vector_t high, low;
} box_t;

bool box_intersects(box_t box, vector_t ray_start, vector_t ray_direction);

#endif

