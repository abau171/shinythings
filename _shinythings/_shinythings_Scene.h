#ifndef _SHINYTHINGS_SCENE_H
#define _SHINYTHINGS_SCENE_H

#include <Python.h>

#include "scene.h"

PyTypeObject _shinythings_SceneType;

typedef struct {
    PyObject_HEAD;
    scene_t scene;
} _shinythings_SceneObject;

void* _shinythings_SceneType_init(void);

#endif

