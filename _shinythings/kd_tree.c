#include <stdlib.h>
#include <math.h>

#include "geometry.h"
#include "scene.h"

#include "kd_tree.h"

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
        bbox.low.x = fmaxf(bbox.low.x, v.x);
        bbox.low.y = fmaxf(bbox.low.y, v.y);
        bbox.low.z = fmaxf(bbox.low.z, v.z);

        v = model->vertices[triangle->b];
        bbox.high.x = fmaxf(bbox.high.x, v.x);
        bbox.high.y = fmaxf(bbox.high.y, v.y);
        bbox.high.z = fmaxf(bbox.high.z, v.z);
        bbox.low.x = fmaxf(bbox.low.x, v.x);
        bbox.low.y = fmaxf(bbox.low.y, v.y);
        bbox.low.z = fmaxf(bbox.low.z, v.z);

        v = model->vertices[triangle->c];
        bbox.high.x = fmaxf(bbox.high.x, v.x);
        bbox.high.y = fmaxf(bbox.high.y, v.y);
        bbox.high.z = fmaxf(bbox.high.z, v.z);
        bbox.low.x = fmaxf(bbox.low.x, v.x);
        bbox.low.y = fmaxf(bbox.low.y, v.y);
        bbox.low.z = fmaxf(bbox.low.z, v.z);
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

