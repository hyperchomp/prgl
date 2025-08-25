#ifndef PR3D_MESH_H
#define PR3D_MESH_H

#include "cglm/types.h"

/**
 * Calculates a 3D AABB bounding box from the vertices of a mesh.
 *
 * @param *vertices
 * @param num_vertices
 * @param min_bounds
 * @param max_bounds
 */
void pr3d_calculate_aabb(
    const float *vertices, int num_vertices, vec3 min_bounds, vec3 max_bounds
);

#endif
