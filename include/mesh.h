#ifndef PRGL_MESH_H
#define PRGL_MESH_H

#include "cglm/types.h"

struct PRGLMesh;

/**
 * @brief Defines the geometry of a 3D object.
 *
 * One mesh can be re-used to render many of the same object.
 */
typedef struct PRGLMesh *PRGLMeshHandle;

/**
 * @brief Creates a triangle mesh.
 *
 * @param texture_id The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_triangle(unsigned int texture_id);

/**
 * @brief Creates a quad mesh.
 *
 * @param texture_id The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_quad(unsigned int texture_id);

/**
 * @brief Creates a pyramid mesh.
 *
 * @param texture_id The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_pyramid(unsigned int texture_id);

/**
 * @brief Creates a cube mesh.
 *
 * @param texture_id The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_cube(unsigned int texture_id);

/**
 * @brief Creates a cube sphere.
 *
 * Generates a sphere constructed from a cube by subdividing and expanding the
 * six faces.
 *
 * @param resolution The resolution for the subdivision of each face. The number
 * of quads per face will be this value squared, for example if resolution is 2
 * there will be 4 quads per face.
 * @param texture_id The ID of the texture to assign to the mesh, or zero.
 */
PRGLMeshHandle prgl_create_cube_sphere(int resolution, unsigned int texture_id);

/**
 * @brief Cleans up the GL objects associated with the mesh and frees it.
 *
 * @param mesh[in,out]
 */
void prgl_delete_mesh(PRGLMeshHandle mesh);

#endif
