#ifndef MATRIX_H
#define MATRIX_H

#include "geometry.h"

typedef struct {
    float cells[3][4];
} matrix_t;

extern const matrix_t identity_matrix;

matrix_t matrix_mult(const matrix_t* a, const matrix_t* b);
matrix_t matrix_translate(const matrix_t* m, vector_t translation);
matrix_t matrix_scale(const matrix_t* m, float scalar);
matrix_t matrix_rotate_x(const matrix_t* m, float radians);
matrix_t matrix_rotate_y(const matrix_t* m, float radians);
matrix_t matrix_rotate_z(const matrix_t* m, float radians);
vector_t matrix_apply(const matrix_t* m, vector_t v);

#endif

