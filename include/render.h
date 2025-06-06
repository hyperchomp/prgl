#ifndef PR3D_RENDER_H
#define PR3D_RENDER_H

#include <glad/glad.h>

struct PR3DVec3
{
    float x;
    float y;
    float z;
};

struct PR3DMesh
{
    unsigned int vbo;
    unsigned int vao;
    unsigned int shader_program;
};

/**
 * Clears the screen using the given color.
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
 * @param[in] v1
 * @param[in] v2
 * @param[in] v3
 */
struct PR3DMesh *pr3d_create_triangle(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3
);

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
