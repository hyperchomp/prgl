#ifndef PRGL_MESH_H
#define PRGL_MESH_H

#include "cglm/types.h"

/**
 * @brief Defines the geometry of a 3D object.
 *
 * One mesh can be re-used to render many of the same object.
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
 * @brief Creates a triangle mesh with the given vertex positions.
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
 * @brief Creates a cube mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 */
struct PRGLMesh *prgl_create_cube(void);

/**
 * @brief Creates a cube sphere.
 * Generates a sphere constructed from a cube by subdividing and expanding the
 * six faces.
 *
 * @param resolution The resolution for the subdivision of each face. The number
 * of quads per face will be this value squared, for example if resolution is 2
 * there will be 4 quads per face.
 */
struct PRGLMesh *prgl_create_cube_sphere(int resolution);

/**
 * @brief Cleans up the GL objects associated with the mesh and frees it.
 *
 * @param mesh[in,out]
 */
void prgl_delete_mesh(struct PRGLMesh *mesh);

#endif
