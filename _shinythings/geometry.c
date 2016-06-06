#include <stdbool.h>
#include <math.h>

#include "geometry.h"

vector_t vector_add(vector_t a, vector_t b)
{
    return (vector_t) {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z};
}

vector_t vector_sub(vector_t a, vector_t b)
{
    return (vector_t) {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z};
}

vector_t vector_scale(vector_t v, float scalar)
{
    return (vector_t) {
        v.x * scalar,
        v.y * scalar,
        v.z * scalar};
}

vector_t vector_normalize(vector_t v)
{
    return vector_scale(v, 1.0 / vector_magnitude(v));
}

vector_t vector_cross(vector_t a, vector_t b)
{
    return (vector_t) {
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    };
}

float vector_magnitude_2(vector_t v)
{
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

float vector_magnitude(vector_t v)
{
    return sqrtf(vector_magnitude_2(v));
}

float vector_distance_2(vector_t a, vector_t b)
{
    return vector_magnitude_2(vector_sub(a, b));
}

float vector_distance(vector_t a, vector_t b)
{
    return vector_magnitude(vector_sub(a, b));
}

float vector_dot(vector_t a, vector_t b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

