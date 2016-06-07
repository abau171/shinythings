#include <Python.h>

#include <string.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/ndarrayobject.h>

#include "render.h"
#include "py_parse.h"

#include "_shinythings_Scene.h"

static PyObject*
Scene_new(PyTypeObject* type, PyObject* args, PyObject* kwargs)
{
    _shinythings_SceneObject* self;
    self = (_shinythings_SceneObject*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

static PyObject*
Scene_init(_shinythings_SceneObject* self, PyObject* args)
{
    self->scene.num_spheres = 0;
    self->scene.num_planes = 0;
    self->scene.num_lights = 0;
    self->scene.num_models = 0;
    self->scene.spheres = NULL;
    self->scene.planes = NULL;
    self->scene.lights = NULL;
    self->scene.models = NULL;
    Py_RETURN_NONE;
}

#warning DONT FORGET THE DEALLOC

static PyObject*
Scene_add_sphere(_shinythings_SceneObject* self, PyObject* args)
{
    sphere_model_t tmp_sphere;
    if (!parse_sphere_model(args, &tmp_sphere))
        return NULL;
    sphere_model_t* new_spheres = malloc((self->scene.num_spheres + 1) * sizeof(sphere_model_t));
    memcpy(new_spheres, self->scene.spheres, self->scene.num_spheres * sizeof(sphere_model_t));
    new_spheres[self->scene.num_spheres] = tmp_sphere;
    if (self->scene.spheres != NULL)
        free(self->scene.spheres);
    self->scene.spheres = new_spheres;
    self->scene.num_spheres++;

    Py_RETURN_NONE;
}

static PyObject*
Scene_add_light(_shinythings_SceneObject* self, PyObject* args)
{
    light_t tmp_light;
    if (!parse_light(args, &tmp_light))
        return NULL;
    light_t* new_lights = malloc((self->scene.num_lights + 1) * sizeof(light_t));
    memcpy(new_lights, self->scene.lights, self->scene.num_lights * sizeof(light_t));
    new_lights[self->scene.num_lights] = tmp_light;
    if (self->scene.lights != NULL)
        free(self->scene.lights);
    self->scene.lights = new_lights;
    self->scene.num_lights++;

    Py_RETURN_NONE;
}

static PyObject*
Scene_add_plane(_shinythings_SceneObject* self, PyObject* args)
{
    plane_model_t tmp_plane;
    if (!parse_plane_model(args, &tmp_plane))
        return NULL;
    plane_model_t* new_planes = malloc((self->scene.num_planes + 1) * sizeof(plane_model_t));
    memcpy(new_planes, self->scene.planes, self->scene.num_planes * sizeof(plane_model_t));
    new_planes[self->scene.num_planes] = tmp_plane;
    if (self->scene.planes != NULL)
        free(self->scene.planes);
    self->scene.planes = new_planes;
    self->scene.num_planes++;

    Py_RETURN_NONE;
}

static PyObject*
Scene_add_model(_shinythings_SceneObject* self, PyObject* args)
{
    tri_model_t tmp_model;
    if (!parse_tri_model(args, &tmp_model))
        return NULL;
    tri_model_t* new_models = malloc((self->scene.num_models + 1) * sizeof(tri_model_t));
    memcpy(new_models, self->scene.models, self->scene.num_models * sizeof(tri_model_t));
    new_models[self->scene.num_models] = tmp_model;
    if (self->scene.models != NULL)
        free(self->scene.models);
    self->scene.models = new_models;
    self->scene.num_models++;

    Py_RETURN_NONE;
}

static PyObject*
Scene_render(_shinythings_SceneObject* self)
{
    uint8_t* image_data = render(&self->scene, 960, 540);

    npy_intp dims[3] = {540, 960, 3};
    PyObject* output = (PyObject*) PyArray_SimpleNewFromData(3, dims, NPY_UINT8, image_data);

    return output;
}

static PyObject*
Scene_test(_shinythings_SceneObject* self)
{
    Py_RETURN_TRUE;
}

static PyMethodDef Scene_methods[] = {
    {"test", (PyCFunction) Scene_test, METH_NOARGS,
     "tests a test"},
    {"add_sphere", (PyCFunction) Scene_add_sphere, METH_VARARGS,
     "adds a sphere"},
    {"add_plane", (PyCFunction) Scene_add_plane, METH_VARARGS,
     "adds a plane"},
    {"add_light", (PyCFunction) Scene_add_light, METH_VARARGS,
     "adds a light"},
    {"add_model", (PyCFunction) Scene_add_model, METH_VARARGS,
     "adds a model"},
    {"render", (PyCFunction) Scene_render, METH_NOARGS,
     "renders the scene"},
    {NULL} /* Sentinel */
};

PyTypeObject _shinythings_SceneType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_shinythings.Scene",    /* tp_name */
    sizeof(_shinythings_SceneObject),   /* tp_basicsize */
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
    "Shiny Things Scene",    /* tp_doc */
    0,                       /* tp_traverse */
    0,                       /* tp_clear */
    0,                       /* tp_richcompare */
    0,                       /* tp_weaklistoffset */
    0,                       /* tp_iter */
    0,                       /* tp_iternext */
    Scene_methods,           /* tp_methods */
    0,                       /* tp_members */
    0,                       /* tp_getset */
    0,                       /* tp_base */
    0,                       /* tp_dict */
    0,                       /* tp_descr_get */
    0,                       /* tp_descr_set */
    0,                       /* tp_dictoffset */
    (initproc) Scene_init,   /* tp_init */
    0,                       /* tp_alloc */
    Scene_new,               /* tp_new */
};

void* _shinythings_SceneType_init(void) {
    import_array();
    return NULL;
}

