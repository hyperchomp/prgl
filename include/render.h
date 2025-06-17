#ifndef PR3D_RENDER_H
#define PR3D_RENDER_H

#include <cglm/cglm.h>

/**
 * Mesh object made up of vertices.
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
 * Clears the screen using the given color. Values are from 0-1.
 *
 * @param r
 * @param g
 * @param b
 * @param a
 */
void pr3d_clear_screen(float r, float g, float b, float a);

/**
 * Changes the color used to render objects.
 * This is designed to work with the default shader type specified in
 * PR3DShader. If you have changed the shader using pr3d_use_shader() using this
 * function may cause undefined behavior or segfault.
 *
 * @param r
 * @param g
 * @param b
 * @param a
 */
void pr3d_set_render_color(float r, float g, float b, float a);

/**
 * Creates a triangle mesh with the given vertex positions.
 *
 * @param[in] vertices
 */
struct PR3DMesh *pr3d_create_triangle(mat3 vertices);

/**
 * Creates a triangle mesh with the given vertex positions and colors.
 *
 * @param[in] vertices
 * @param[in] colors
 */
struct PR3DMesh *pr3d_create_triangle_vertex_color(mat3 vertices, mat3 colors);

/**
 * Creates a rectangle mesh.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 *
 * @param[in] colors
 */
struct PR3DMesh *pr3d_create_rectangle(void);

/**
 * Creates a rectangle mesh with the given vertex colors.
 * Make sure to attach a texture to the returned mesh struct after creating it.
 *
 * @param[in] colors
 */
struct PR3DMesh *pr3d_create_rectangle_vertex_color(mat4 colors);

/**
 * Renders a mesh to the screen.
 *
 * @param[in] mesh
 */
void pr3d_render_mesh(struct PR3DMesh *mesh);

/**
 * Cleans up the GL objects associated with the mesh and then frees it.
 */
void pr3d_delete_mesh(struct PR3DMesh *mesh);

#endif
