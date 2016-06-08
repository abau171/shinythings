#include <stdlib.h>

#include "kd_tree.h"
#include "scene.h"

void init_scene_components(scene_t* scene)
{
    scene->num_spheres = 0;
    scene->num_planes = 0;
    scene->num_lights = 0;
    scene->num_models = 0;
    scene->spheres = NULL;
    scene->planes = NULL;
    scene->lights = NULL;
    scene->models = NULL;
}

void free_model_components(tri_model_t* model)
{
    free(model->vertices);
    free(model->triangles);
    free(model->triangle_normals);
    free(model->vertex_normals);
    free_kd_tree(model->kd_tree);
}

void free_scene_components(scene_t* scene)
{
    if (scene->spheres != NULL) {
        free(scene->spheres);
    }
    if (scene->planes != NULL) {
        free(scene->planes);
    }
    if (scene->lights != NULL) {
        free(scene->lights);
    }
    if (scene->models != NULL) {
        for (int i = 0; i < scene->num_models; i++) {
            free_model_components(&scene->models[i]);
        }
    }
}

