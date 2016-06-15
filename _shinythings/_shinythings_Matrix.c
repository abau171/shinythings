#include <Python.h>

#include "py_parse.h"
#include "matrix.h"

#include "_shinythings_Matrix.h"

static PyObject*
Matrix_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    _shinythings_MatrixObject* self;
    self = (_shinythings_MatrixObject*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

static PyObject*
Matrix_init(_shinythings_MatrixObject* self, PyObject* args)
{
    self->matrix = identity_matrix;
    Py_RETURN_NONE;
}

static PyObject*
Matrix_mult(_shinythings_MatrixObject* self, PyObject* args)
{
    PyObject* py_other_obj;
    if (!PyArg_ParseTuple(args, "O", &py_other_obj))
        return NULL;

    _shinythings_MatrixObject* py_other = (_shinythings_MatrixObject*) py_other_obj;
    self->matrix = matrix_mult(&self->matrix, &py_other->matrix);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_translate(_shinythings_MatrixObject* self, PyObject* args)
{
    vector_t translation;
    if (!parse_vector(args, &translation))
        return NULL;

    self->matrix = matrix_translate(&self->matrix, translation);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_scale(_shinythings_MatrixObject* self, PyObject* args)
{
    float scalar;
    if (!PyArg_ParseTuple(args, "f", &scalar))
        return NULL;

    self->matrix = matrix_scale(&self->matrix, scalar);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_rotate_x(_shinythings_MatrixObject* self, PyObject* args)
{
    float radians;
    if (!PyArg_ParseTuple(args, "f", &radians))
        return NULL;

    self->matrix = matrix_rotate_x(&self->matrix, radians);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_rotate_y(_shinythings_MatrixObject* self, PyObject* args)
{
    float radians;
    if (!PyArg_ParseTuple(args, "f", &radians))
        return NULL;

    self->matrix = matrix_rotate_y(&self->matrix, radians);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_rotate_z(_shinythings_MatrixObject* self, PyObject* args)
{
    float radians;
    if (!PyArg_ParseTuple(args, "f", &radians))
        return NULL;

    self->matrix = matrix_rotate_z(&self->matrix, radians);

    Py_RETURN_NONE;
}

static PyObject*
Matrix_test(_shinythings_MatrixObject* self)
{
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", self->matrix.cells[i][j]);
        }
        putchar('\n');
    }
    Py_RETURN_TRUE;
}

static PyMethodDef Matrix_methods[] = {
    {"test", (PyCFunction) Matrix_test, METH_NOARGS,
     "tests a test"},
    {"mult", (PyCFunction) Matrix_mult, METH_VARARGS,
     "multiplies this matrix by another matrix."},
    {"translate", (PyCFunction) Matrix_translate, METH_VARARGS,
     "translates a matrix."},
    {"scale", (PyCFunction) Matrix_scale, METH_VARARGS,
     "scales a matrix."},
    {"rotate_x", (PyCFunction) Matrix_rotate_x, METH_VARARGS,
     "rotates a matrix about the X axis."},
    {"rotate_y", (PyCFunction) Matrix_rotate_y, METH_VARARGS,
     "rotates a matrix about the Y axis."},
    {"rotate_z", (PyCFunction) Matrix_rotate_z, METH_VARARGS,
     "rotates a matrix about the Z axis."},
    {NULL} /* Sentinel */
};

PyTypeObject _shinythings_MatrixType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_shinythings.Matrix",   /* tp_name */
    sizeof(_shinythings_MatrixObject),   /* tp_basicsize */
    0,                       /* tp_itemsize */
    0,                       /* tp_dealloc */
    0,                       /* tp_print */
    0,                       /* tp_getattr */
    0,                       /* tp_setattr */
    0,                       /* tp_reserved */
    0,                       /* tp_repr */
    0,                       /* tp_as_number */
    0,                       /* tp_as_sequence */
    0,                       /* tp_as_mapping */
    0,                       /* tp_hash */
    0,                       /* tp_call */
    0,                       /* tp_str */
    0,                       /* tp_getattro */
    0,                       /* tp_setattro */
    0,                       /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,      /* tp_flags */
    "Shiny Things Matrix",   /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    Matrix_methods,          /* tp_methods */
    0,                       /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    0,                       /* tp_dictoffset */
    (initproc) Matrix_init,  /* tp_init */
    0,                       /* tp_alloc */
    Matrix_new,              /* tp_new */
};


