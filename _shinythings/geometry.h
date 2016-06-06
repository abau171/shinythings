#ifndef GEOMETRY_H
#define GEOMETRY_H

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

#endif

