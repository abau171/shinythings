#include <Python.h>

#include "geometry.h"
#include "color.h"
#include "scene.h"

bool parse_vector(PyObject* py_vector, vector_t* v)
{
    if (!PyArg_ParseTuple(py_vector, "fff", &v->x, &v->y, &v->z))
        return false;
    return true;
}

bool parse_sphere(PyObject* py_sphere, sphere_t* s)
{
    PyObject* py_center;
    if (!PyArg_ParseTuple(py_sphere, "Of", &py_center, &s->radius))
        return false;

    if (!parse_vector(py_center, &s->center))
        return false;

    return true;
}

bool parse_plane(PyObject* py_plane, plane_t* p)
{
    PyObject* py_point;
    PyObject* py_normal;
    if (!PyArg_ParseTuple(py_plane, "OO", &py_point, &py_normal))
        return false;

    if (!parse_vector(py_point, &p->point))
        return false;

    if (!parse_vector(py_normal, &p->normal))
        return false;

    return true;
}

bool parse_color(PyObject* py_color, color_t* c)
{
    if (!PyArg_ParseTuple(py_color, "fff", &c->r, &c->g, &c->b))
        return false;
    return true;
}

bool parse_surface(PyObject* py_surface, surface_t* s)
{
    PyObject* py_color;
    if (!PyArg_ParseTuple(py_surface, "Off", &py_color, &s->specular, &s->shininess))
        return false;

    if (!parse_color(py_color, &s->color))
        return false;

    return true;
}


bool parse_sphere_model(PyObject* py_sphere_model, sphere_model_t* sm)
{
    PyObject* py_sphere;
    PyObject* py_surface;
    if (!PyArg_ParseTuple(py_sphere_model, "OO", &py_sphere, &py_surface))
        return false;

    if (!parse_sphere(py_sphere, &sm->sphere))
        return false;

    if (!parse_surface(py_surface, &sm->surface))
        return false;

    return true;
}

bool parse_plane_model(PyObject* py_plane_model, plane_model_t* pm)
{
    PyObject* py_plane;
    PyObject* py_surface;
    if (!PyArg_ParseTuple(py_plane_model, "OO", &py_plane, &py_surface))
        return false;

    if (!parse_plane(py_plane, &pm->plane))
        return false;

    if (!parse_surface(py_surface, &pm->surface))
        return false;

    return true;
}

bool parse_light(PyObject* py_light, light_t* l)
{
    PyObject* py_position;
    PyObject* py_color;
    if (!PyArg_ParseTuple(py_light, "OO", &py_position, &py_color))
        return false;

    if (!parse_vector(py_position, &l->position))
        return false;

    if (!parse_color(py_color, &l->color))
        return false;

    return true;
}

bool parse_triangle(PyObject* py_triangle, triangle_t* t)
{
    if (!PyArg_ParseTuple(py_triangle, "iii", &t->a, &t->b, &t->c))
        return false;

    return true;
}

bool parse_tri_model(PyObject* py_tri_model, tri_model_t* tm)
{
    PyObject* py_vertices;
    PyObject* py_triangles;
    PyObject* py_surface;
    if (!PyArg_ParseTuple(py_tri_model, "OOO", &py_vertices, &py_triangles, &py_surface))
        return false;

    int num_vertices = PyList_Size(py_vertices);
    tm->num_vertices = num_vertices;
    vector_t* vertices = malloc(num_vertices * sizeof(vector_t));
    for (int i = 0; i < num_vertices; i++) {
        if (!parse_vector(PyList_GetItem(py_vertices, i), &vertices[i]))
            return false;
    }
    tm->vertices = vertices;

    int num_triangles = PyList_Size(py_triangles);
    tm->num_triangles = num_triangles;
    triangle_t* triangles = malloc(num_triangles * sizeof(triangle_t));
    for (int i = 0; i < num_triangles; i++) {
        if (!parse_triangle(PyList_GetItem(py_triangles, i), &triangles[i]))
            return false;
    }
    tm->triangles = triangles;

    vector_t* triangle_normals = malloc(num_triangles * sizeof(vector_t));
    for (int i = 0; i < num_triangles; i++) {
        vector_t a = vertices[triangles[i].a];
        vector_t b = vertices[triangles[i].b];
        vector_t c = vertices[triangles[i].c];
        triangle_normals[i] = vector_normalize(vector_cross(vector_sub(b, a), vector_sub(c, a)));
    }
    tm->triangle_normals = triangle_normals;

    if (!parse_surface(py_surface, &tm->surface))
        return false;

    return true;
}

