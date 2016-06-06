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

bool sphere_intersect(sphere_t s, vector_t ray_start, vector_t ray_direction, vector_t* hit) {
    float a = vector_magnitude_2(ray_direction);
    vector_t recentered = vector_sub(ray_start, s.center);
    float b = 2 * vector_dot(ray_direction, recentered);
    float c = vector_magnitude_2(recentered) - (s.radius * s.radius);
    float discrim = (b * b) - (4.0 * a * c);
    if (discrim < 0.0)
        return false;
    float sqrt_discrim = sqrtf(discrim);
    float t1 = (-b + sqrt_discrim) / (2.0 * a);
    float t2 = (-b - sqrt_discrim) / (2.0 * a);
    float t;
    if (t1 > 0.0 && t2 > 0.0) {
        t = fminf(t1, t2);
    } else {
        t = fmaxf(t1, t2);
    }
    if (t <= 0.0)
        return false;
    *hit = vector_add(ray_start, vector_scale(ray_direction, t));
    return true;
}

