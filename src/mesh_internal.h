#ifndef PRGL_MESH_INTERNAL_H
#define PRGL_MESH_INTERNAL_H

struct PRGLMesh;

/**
 * Initializes a mesh struct with the given values.
 * Zeroes texture_id, it must be set with an ID from a loaded texture after.
 *
 * @param mesh[in,out]
 * @param num_vertices
 * @param vao
 * @param vbo
 * @param ebo
 */
void prgl_init_mesh(
    struct PRGLMesh *mesh, int num_vertices, unsigned int vao, unsigned int vbo,
    unsigned int ebo
);

/**
 * Creates a quad for drawing the screen's render texture to.
 */
struct PRGLMesh *prgl_create_screen_quad(void);

#endif
