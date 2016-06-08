#ifndef KD_TREE_H
#define KD_TREE_H

#include "scene.h"

kd_node_t* make_kd_tree(tri_model_t* model);
bool kd_intersect(tri_model_t* model, kd_node_t* node, vector_t ray_start, vector_t ray_direction, vector_t* hit, vector_t* normal);

#endif

