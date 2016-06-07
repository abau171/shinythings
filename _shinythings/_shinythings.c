#include <Python.h>

#include <stdint.h>

#include "render.h"
#include "_shinythings_Scene.h"

static PyMethodDef _shinythings_methods[] = {
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
    if (PyType_Ready(&_shinythings_SceneType) < 0)
        return NULL;

    PyObject* m = PyModule_Create(&_shinythingsmodule);

    void* result = _shinythings_SceneType_init();
    if (result != NULL)
        return result;
    Py_INCREF(&_shinythings_SceneType);
    PyModule_AddObject(m, "Scene", (PyObject*) &_shinythings_SceneType);

    return m;
}

