#ifndef PR3D_RENDER_H
#define PR3D_RENDER_H

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
 * @param[in] v1
 * @param[in] v2
 * @param[in] v3
 */
struct PR3DMesh *pr3d_create_triangle(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3
);

/**
 * Creates a triangle mesh with the given vertex positions and colors.
 *
 * @param[in] v1
 * @param[in] v2
 * @param[in] v3
 * @param[in] c1
 * @param[in] c2
 * @param[in] c3
 */
struct PR3DMesh *pr3d_create_triangle_vertex_color(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3,
    struct PR3DVec3 *c1, struct PR3DVec3 *c2, struct PR3DVec3 *c3
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
