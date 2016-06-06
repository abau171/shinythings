#include <Python.h>

#include <stdint.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

#include "render.h"

static PyObject*
_shinythings_render(PyObject* self, PyObject* args)
{
    int width, height;
    if (!PyArg_ParseTuple(args, "ii", &width, &height))
        return NULL;

    uint8_t* image_data = render(width, height);

    npy_intp dims[3] = {height, width, 3};
    PyObject* output = (PyObject*) PyArray_SimpleNewFromData(3, dims, NPY_UINT8, image_data);

    return output;
}

static PyMethodDef _shinythings_methods[] = {
    {"render", _shinythings_render, METH_VARARGS, "Renders an image."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef _shinythingsmodule = {
    PyModuleDef_HEAD_INIT,
    "_shinythings",
    "Shiny Things C Extensions.",
    -1,
    _shinythings_methods
};

PyMODINIT_FUNC
PyInit__shinythings(void)
{
    import_array();
    PyObject* m = PyModule_Create(&_shinythingsmodule);
    return m;
}

