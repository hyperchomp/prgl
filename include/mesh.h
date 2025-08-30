#ifndef PRGL_MESH_H
#define PRGL_MESH_H

#include "cglm/types.h"

/**
 * Defines the geometry of a 3D object. Only one Mesh is needed to draw many
 * of the same object.
 * Any IDs which are optional should be set to 0 if unused.
 */
struct PRGLMesh
{
    int num_vertices;

    // Vertex Buffer Object stores raw vertex data.
    unsigned int vbo;

    // Vertex Array Object stores vertex data state for how to render objects.
    unsigned int vao;

    // Optional - Element Buffer Object to draw vertices with indices.
    unsigned int ebo;

    // Optional - Stores the texture ID for the mesh.
    unsigned int texture_id;
};

/**
 * Creates a triangle mesh with the given vertex positions.
 *
 * @param[in] vertices
 */
struct PRGLMesh *prgl_create_triangle(mat3 vertices);

/**
 * Creates a quad mesh. For 3D the normals assume XY orientation.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 *
 * @param[in] vertices
 */
struct PRGLMesh *prgl_create_quad(mat4 vertices);

/**
 * Creates a cube mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 */
struct PRGLMesh *prgl_create_cube(void);

/**
 * Cleans up the GL objects associated with the mesh and frees it.
 *
 * @param mesh[in,out]
 */
void prgl_delete_mesh(struct PRGLMesh *mesh);

#endif
