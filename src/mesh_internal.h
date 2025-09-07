#ifndef PRGL_MESH_INTERNAL_H
#define PRGL_MESH_INTERNAL_H

#include "glad.h"
#include "types.h"

/**
 * @brief Defines the geometry of a 3D object.
 *
 * One mesh can be re-used to render many of the same object.
 * Any IDs which are optional should be set to 0 if unused.
 */
struct PRGLMesh
{
    GLuint num_vertices;

    /// @brief Vertex Buffer Object stores raw vertex data.
    GLuint vbo;

    /// @brief Vertex Array Object stores vertex state for how to render.
    GLuint vao;

    /// @brief Optional - Element Buffer Object to draw vertices with indices.
    GLuint ebo;

    /// @brief Optional - Stores the texture ID for the mesh.
    PRGLTexture texture;

    /**
     * @brief The type of of primitive to render.
     *
     * This is the OpenGL constant e.g. GL_TRIANGLES or GL_LINES.
     */
    GLenum primitive_type;
};

/**
 * Initializes a mesh struct with the given values.
 *
 * @param mesh[in,out]
 * @param num_vertices
 * @param vao
 * @param vbo
 * @param ebo
 * @param texture
 * @param primitive_type
 */
void prgl_init_mesh(
    struct PRGLMesh *mesh, GLuint num_vertices, GLuint vao, GLuint vbo,
    GLuint ebo, PRGLTexture texture, GLenum primitive_type
);

/**
 * Creates a quad for drawing the screen's render texture to.
 *
 * @param texture_id The ID of the texture to draw the screen quad to.
 */
struct PRGLMesh *prgl_create_screen_quad(PRGLTexture texture);

#endif
