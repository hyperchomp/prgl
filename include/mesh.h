#ifndef PRGL_MESH_H
#define PRGL_MESH_H

#include "cglm/types.h"
#include "types.h"

/**
 * @brief Creates a triangle mesh.
 *
 * @param texture The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_triangle(PRGLTexture texture);

/**
 * @brief Creates a quad mesh.
 *
 * @param texture The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_quad(PRGLTexture texture);

/**
 * @brief Creates a pyramid mesh.
 *
 * @param texture The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_pyramid(PRGLTexture texture);

/**
 * @brief Creates a cube mesh.
 *
 * @param texture The texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_cube(PRGLTexture texture);

/**
 * @brief Creates a cube sphere.
 *
 * Generates a sphere constructed from a cube by subdividing and expanding the
 * six faces.
 *
 * @param resolution The resolution for the subdivision of each face. The number
 * of quads per face will be this value squared, for example if resolution is 2
 * there will be 4 quads per face.
 * @param texture The texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_cube_sphere(int resolution, PRGLTexture texture);

/**
 * @brief Creates a line strip.
 *
 * Generates a series of interconnected line segments.
 *
 * @param points[in] The points the lines will be drawn between.
 * @param num_points
 */
PRGLMeshHandle prgl_create_line_strip(vec3 points[], int num_points);

/**
 * @brief Cleans up the GL objects associated with the mesh and frees it.
 *
 * @param mesh[in,out]
 */
void prgl_delete_mesh(PRGLMeshHandle mesh);

#endif
