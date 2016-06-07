#ifndef PY_PARSE_H
#define PY_PARSE_H

#include <Python.h>

#include "geometry.h"
#include "color.h"
#include "scene.h"

bool parse_vector(PyObject* py_vector, vector_t* v);
bool parse_sphere(PyObject* py_sphere, sphere_t* s);
bool parse_color(PyObject* py_color, color_t* c);
bool parse_surface(PyObject* py_surface, surface_t* s);
bool parse_sphere_model(PyObject* py_sphere_model, sphere_model_t* sm);
bool parse_light(PyObject* py_light, light_t* l);

#endif

