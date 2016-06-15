#ifndef _SHINYTHINGS_MATRIX_H
#define _SHINYTHINGS_MATRIX_H

#include <Python.h>

#include "matrix.h"

PyTypeObject _shinythings_MatrixType;

typedef struct {
    PyObject_HEAD;
    matrix_t matrix;
} _shinythings_MatrixObject;

#endif

