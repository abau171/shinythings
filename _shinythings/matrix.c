#include <math.h>

#include "geometry.h"

#include "matrix.h"

const matrix_t identity_matrix = {
    {{1.0, 0.0, 0.0, 0.0},
     {0.0, 1.0, 0.0, 0.0},
     {0.0, 0.0, 1.0, 0.0}}};

matrix_t matrix_mult(const matrix_t* a, const matrix_t* b)
{
    matrix_t c;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            float sum = 0.0;
            for (int k = 0; k < 3; k++) {
                sum += a->cells[i][k] * b->cells[k][j];
            }
            if (j == 3)
                sum += a->cells[i][3];
            c.cells[i][j] = sum;
        }
    }
    return c;
}

matrix_t matrix_translate(const matrix_t* m, vector_t translation)
{
    matrix_t translate_matrix = {
        {{1.0, 0.0, 0.0, translation.x},
         {0.0, 1.0, 0.0, translation.y},
         {0.0, 0.0, 1.0, translation.z}}};
    return matrix_mult(m, &translate_matrix);
}

matrix_t matrix_scale(const matrix_t* m, float scalar)
{
    matrix_t scale_matrix = {
        {{scalar, 0.0, 0.0, 0.0},
         {0.0, scalar, 0.0, 0.0},
         {0.0, 0.0, scalar, 0.0}}};
    return matrix_mult(m, &scale_matrix);
}

matrix_t matrix_rotate_x(const matrix_t* m, float radians)
{
    matrix_t rotate_matrix = {
        {{1.0, 0.0, 0.0, 0.0},
         {0.0, cos(radians), -sin(radians), 0.0},
         {0.0, sin(radians), cos(radians), 0.0}}};
    return matrix_mult(m, &rotate_matrix);
}

matrix_t matrix_rotate_y(const matrix_t* m, float radians)
{
    matrix_t rotate_matrix = {
        {{cos(radians), 0.0, sin(radians), 0.0},
         {0.0, 1.0, 0.0, 0.0},
         {-sin(radians), 0.0, cos(radians), 0.0}}};
    return matrix_mult(m, &rotate_matrix);
}

matrix_t matrix_rotate_z(const matrix_t* m, float radians)
{
    matrix_t rotate_matrix = {
        {{cos(radians), -sin(radians), 0.0, 0.0},
         {sin(radians), cos(radians), 0.0, 0.0},
         {0.0, 0.0, 1.0, 0.0}}};
    return matrix_mult(m, &rotate_matrix);
}

vector_t matrix_apply(const matrix_t* m, vector_t v)
{
    return (vector_t) {
        m->cells[0][0] * v.x + m->cells[0][1] * v.y + m->cells[0][2] * v.z + m->cells[0][3],
        m->cells[1][0] * v.x + m->cells[1][1] * v.y + m->cells[1][2] * v.z + m->cells[1][3],
        m->cells[2][0] * v.x + m->cells[2][1] * v.y + m->cells[2][2] * v.z + m->cells[2][3]};
}

