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

bool sphere_intersect(sphere_t s, vector_t ray_start, vector_t ray_direction, vector_t* hit)
{
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

bool plane_intersect(plane_t p, vector_t ray_start, vector_t ray_direction, vector_t* hit)
{
    float denominator = vector_dot(p.normal, ray_direction);
    if (denominator == 0)
        return false;
    float t = vector_dot(p.normal, vector_sub(p.point, ray_start)) / denominator;
    if (t < 0.0)
        return false;
    *hit = vector_add(ray_start, vector_scale(ray_direction, t));
    return true;
}

static void barycentric(vector_t a, vector_t b, vector_t c, vector_t normal, vector_t plane_hit, float* u, float* v, float* w)
{
    vector_t ab = vector_sub(b, a);
    vector_t ac = vector_sub(c, a);
    vector_t ah = vector_sub(plane_hit, a);
    float ab_ab = vector_dot(ab, ab);
    float ab_ac = vector_dot(ab, ac);
    float ac_ac = vector_dot(ac, ac);
    float ab_ah = vector_dot(ab, ah);
    float ac_ah = vector_dot(ac, ah);
    float inv_denom = 1.0 / (ab_ab * ac_ac - ab_ac * ab_ac);
    float tmp_v = (ac_ac * ab_ah - ab_ac * ac_ah) * inv_denom;
    float tmp_w = (ab_ab * ac_ah - ab_ac * ab_ah) * inv_denom;
    *u = 1.0 - tmp_v - tmp_w;
    *v = tmp_v;
    *w = tmp_w;
}

bool triangle_intersect(vector_t a, vector_t b, vector_t c, vector_t normal, vector_t ray_start, vector_t ray_direction, vector_t* hit)
{
    if (vector_dot(normal, ray_direction) > 0.0)
        return false;

    vector_t plane_hit;
    if (!plane_intersect((plane_t) {a, normal}, ray_start, ray_direction, &plane_hit))
        return false;

    float u, v, w;
    barycentric(a, b, c, normal, plane_hit, &u, &v, &w);
    if (u < 0.0 || v < 0.0 || w < 0.0)
        return false;

    *hit = plane_hit;
    return true;
}

float box_intersects(box_t box, vector_t ray_start, vector_t ray_direction)
{
    vector_t high_diff = vector_sub(box.high, ray_start);
    vector_t low_diff = vector_sub(box.low, ray_start);

    float td1, td2, t_max, t_min;

    // X
    td1 = high_diff.x / ray_direction.x;
    td2 = low_diff.x / ray_direction.x;
    t_max = fmaxf(td1, td2);
    t_min = fminf(td1, td2);

    // Y
    td1 = high_diff.y / ray_direction.y;
    td2 = low_diff.y / ray_direction.y;
    t_max = fminf(t_max, fmaxf(td1, td2));
    t_min = fmaxf(t_min, fminf(td1, td2));

    // Z
    td1 = high_diff.z / ray_direction.z;
    td2 = low_diff.z / ray_direction.z;
    t_max = fminf(t_max, fmaxf(td1, td2));
    t_min = fmaxf(t_min, fminf(td1, td2));

    if (t_max < t_min) {
        return -1.0;
    } else if (t_min >= 0.0) {
        return t_min;
    } else {
        return t_max;
    }
}

