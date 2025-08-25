#ifndef PR3D_MESH_H
#define PR3D_MESH_H

#include "cglm/types.h"

/**
 * Defines the geometry of a 3D object. Only one Mesh is needed to draw many
 * of the same object.
 * Any IDs which are optional should be set to 0 if unused.
 */
struct PR3DMesh
{
    vec3 min_bounds;
    vec3 max_bounds;

    int num_vertices;

    // Vertex Buffer Object stores raw vertex data.
    unsigned int vbo;

    // Vertex Array Object stores vertex data state for how to render objects.
    unsigned int vao;

    // Optional - Element Buffer Object to draw vertices with indices.
    unsigned int ebo;

    // Optional - Stores the texture image if the mesh is textured.
    unsigned int texture;
};

/**
 * Creates a triangle mesh with the given vertex positions.
 *
 * @param[in] vertices
 */
struct PR3DMesh *pr3d_create_triangle(mat3 vertices);

/**
 * Creates a quad mesh. For 3D the normals assume XY orientation.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 *
 * @param[in] vertices
 */
struct PR3DMesh *pr3d_create_quad(mat4 vertices);

/**
 * Creates a cube mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 */
struct PR3DMesh *pr3d_create_cube(void);

/**
 * Cleans up the GL objects associated with the mesh, call before freeing.
 */
void pr3d_delete_mesh(struct PR3DMesh *mesh);

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
