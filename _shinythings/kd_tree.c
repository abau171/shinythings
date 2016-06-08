#include <stdlib.h>
#include <math.h>

#include "geometry.h"
#include "scene.h"

#include "kd_tree.h"

typedef struct {
    bool is_leaf;
    box_t bbox;
    kd_node_t* left;
    kd_node_t* right;
} kd_internal_node_t;

typedef struct {
    bool is_leaf;
    box_t bbox;
    int num_triangle_ids;
    int* triangle_ids;
} kd_leaf_node_t;

static bool boxes_touching(box_t a, box_t b)
{
    return (a.high.x >= b.low.x && a.low.x <= b.high.x) &&
           (a.high.y >= b.low.y && a.low.y <= b.high.y) &&
           (a.high.z >= b.low.z && a.low.z <= b.high.z);
}

static box_t get_bounding_box(tri_model_t* model, triangle_t* triangle)
{
    vector_t a = model->vertices[triangle->a];
    vector_t b = model->vertices[triangle->b];
    vector_t c = model->vertices[triangle->c];
    return (box_t) {
        {fmaxf(fmaxf(a.x, b.x), c.x),
         fmaxf(fmaxf(a.y, b.y), c.y),
         fmaxf(fmaxf(a.z, b.z), c.z)},
        {fminf(fminf(a.x, b.x), c.x),
         fminf(fminf(a.y, b.y), c.y),
         fminf(fminf(a.z, b.z), c.z)}};
}

static box_t get_bounding_box_all(tri_model_t* model, int num_triangle_ids, int* triangle_ids)
{
    box_t bbox = {{-INFINITY, -INFINITY, -INFINITY}, {INFINITY, INFINITY, INFINITY}};
    for (int i = 0; i < num_triangle_ids; i++) {
        triangle_t* triangle = &model->triangles[triangle_ids[i]];

        vector_t v = model->vertices[triangle->a];
        bbox.high.x = fmaxf(bbox.high.x, v.x);
        bbox.high.y = fmaxf(bbox.high.y, v.y);
        bbox.high.z = fmaxf(bbox.high.z, v.z);
        bbox.low.x = fminf(bbox.low.x, v.x);
        bbox.low.y = fminf(bbox.low.y, v.y);
        bbox.low.z = fminf(bbox.low.z, v.z);

        v = model->vertices[triangle->b];
        bbox.high.x = fmaxf(bbox.high.x, v.x);
        bbox.high.y = fmaxf(bbox.high.y, v.y);
        bbox.high.z = fmaxf(bbox.high.z, v.z);
        bbox.low.x = fminf(bbox.low.x, v.x);
        bbox.low.y = fminf(bbox.low.y, v.y);
        bbox.low.z = fminf(bbox.low.z, v.z);

        v = model->vertices[triangle->c];
        bbox.high.x = fmaxf(bbox.high.x, v.x);
        bbox.high.y = fmaxf(bbox.high.y, v.y);
        bbox.high.z = fmaxf(bbox.high.z, v.z);
        bbox.low.x = fminf(bbox.low.x, v.x);
        bbox.low.y = fminf(bbox.low.y, v.y);
        bbox.low.z = fminf(bbox.low.z, v.z);
    }

    return bbox;
}

static void get_all_in_box(tri_model_t* model, int num_triangle_ids, int* triangle_ids, box_t box, int* num_in_box, int** triangle_ids_in_box)
{
    int tmp_num_in_box = 0;
    for (int i = 0; i < num_triangle_ids; i++) {
        triangle_t* triangle = &model->triangles[triangle_ids[i]];
        if (boxes_touching(get_bounding_box(model, triangle), box))
            tmp_num_in_box++;
    }

    int c = 0;
    int* tmp_triangle_ids_in_box = malloc(tmp_num_in_box * sizeof(int));
    for (int i = 0; i < num_triangle_ids; i++) {
        triangle_t* triangle = &model->triangles[triangle_ids[i]];
        if (boxes_touching(get_bounding_box(model, triangle), box)) {
            tmp_triangle_ids_in_box[c] = triangle_ids[i];
            c++;
        }
    }

    *num_in_box = tmp_num_in_box;
    *triangle_ids_in_box = tmp_triangle_ids_in_box;
}

static kd_node_t* make_kd_tree_rec(tri_model_t* model, int num_triangle_ids, int* triangle_ids, box_t bbox)
{
    float width = bbox.high.x - bbox.low.x;
    float height = bbox.high.y - bbox.low.y;
    float depth = bbox.high.z - bbox.low.z;

    box_t left_box, right_box;
    if (width > height && width > depth) {
        left_box = (box_t) {{bbox.high.x, bbox.high.y, bbox.high.z}, {bbox.low.x + width / 2.0, bbox.low.y, bbox.low.z}};
        right_box = (box_t) {{bbox.high.x - width / 2.0, bbox.high.y, bbox.high.z}, {bbox.low.x, bbox.low.y, bbox.low.z}};
    } else if (height > depth) {
        left_box = (box_t) {{bbox.high.x, bbox.high.y, bbox.high.z}, {bbox.low.x, bbox.low.y + height / 2.0, bbox.low.z}};
        right_box = (box_t) {{bbox.high.x, bbox.high.y - height / 2.0, bbox.high.z}, {bbox.low.x, bbox.low.y, bbox.low.z}};
    } else {
        left_box = (box_t) {{bbox.high.x, bbox.high.y, bbox.high.z}, {bbox.low.x, bbox.low.y, bbox.low.z + depth / 2.0}};
        right_box = (box_t) {{bbox.high.x, bbox.high.y, bbox.high.z - depth / 2.0}, {bbox.low.x, bbox.low.y, bbox.low.z}};
    }

    int num_in_left_box, num_in_right_box;
    int* triangle_ids_in_left_box;
    int* triangle_ids_in_right_box;
    get_all_in_box(model, num_triangle_ids, triangle_ids, left_box, &num_in_left_box, &triangle_ids_in_left_box);
    get_all_in_box(model, num_triangle_ids, triangle_ids, right_box, &num_in_right_box, &triangle_ids_in_right_box);

    int overlap = num_in_left_box + num_in_right_box - num_triangle_ids;
    if (overlap >= num_triangle_ids / 2) {
        kd_leaf_node_t* l_node = malloc(sizeof(kd_leaf_node_t));
        l_node->is_leaf = true;
        l_node->bbox = bbox;
        l_node->num_triangle_ids = num_triangle_ids;
        l_node->triangle_ids = triangle_ids;
        return (kd_node_t*) l_node;
    } else {
        kd_internal_node_t* i_node = malloc(sizeof(kd_internal_node_t));
        i_node->is_leaf = false;
        i_node->bbox = bbox;
        i_node->left = make_kd_tree_rec(model, num_in_left_box, triangle_ids_in_left_box, left_box);
        i_node->right = make_kd_tree_rec(model, num_in_right_box, triangle_ids_in_right_box, right_box);
        return (kd_node_t*) i_node;
    }
}

kd_node_t* make_kd_tree(tri_model_t* model)
{
    int* triangle_ids = malloc(model->num_triangles * sizeof(int));
    for (int i = 0; i < model->num_triangles; i++) {
        triangle_ids[i] = i;
    }
    return make_kd_tree_rec(model, model->num_triangles, triangle_ids, get_bounding_box_all(model, model->num_triangles, triangle_ids));
}

