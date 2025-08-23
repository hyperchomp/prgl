#ifndef PR3D_RENDER_H
#define PR3D_RENDER_H

#include "cglm/types.h"

/**
 * Defines the geometry of a 3D object. Only one Mesh is needed to draw many
 * of the same object.
 * Any IDs which are optional should be set to 0 if unused.
 */
struct PR3DMesh
{
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
 * A light which can be positioned in the world and emits a color.
 */
struct PR3DPointLight
{
    vec3 position;
    vec3 lightColor;
};

/**
 * Clears the screen using the given color. Values are from 0-1.
 *
 * @param r
 * @param g
 * @param b
 * @param a
 */
void pr3d_clear_screen(float r, float g, float b, float a);

/**
 * Creates a triangle mesh with the given vertex positions.
 *
 * @param[in] vertices
 */
struct PR3DMesh *pr3d_create_triangle(mat3 vertices);

/**
 * Creates a rectangle mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 *
 * @param[in] vertices
 */
struct PR3DMesh *pr3d_create_rectangle(mat4 vertices);

/**
 * Creates a cube mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 */
struct PR3DMesh *pr3d_create_cube(void);

/**
 * Renders a mesh to the screen at a position.
 *
 * @param[in] mesh
 * @param vec3 position The XYZ position to render the mesh at.
 * @param vec3 rotation_axis The XYZ axis of the rotation.
 * @param float degrees The angle of rotation in degrees.
 * @param vec3 scale The XYZ scale to apply to the mesh.
 */
void pr3d_render_mesh(
    struct PR3DMesh *mesh, vec3 position, vec3 rotation_axis, float degrees,
    vec3 scale
);

/**
 * Renders a mesh to the screen at a 2D position.
 * Rotation will be about the Z axis.
 *
 * @param[in] mesh
 * @param vec2 position The XY position to render the mesh at.
 * @param float degrees The angle of rotation in degrees.
 * @param vec2 scale The XY scale to apply to the mesh.
 */
void pr3d_render_mesh_2d(
    struct PR3DMesh *mesh, vec2 position, float rotation_degrees, vec2 scale
);

/**
 * Cleans up the GL objects associated with the mesh and then frees it.
 */
void pr3d_delete_mesh(struct PR3DMesh *mesh);

#endif
