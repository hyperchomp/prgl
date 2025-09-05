#ifndef PRGL_MESH_H
#define PRGL_MESH_H

/**
 * @brief Defines the geometry of a 3D object.
 *
 * One mesh can be re-used to render many of the same object.
 * Any IDs which are optional should be set to 0 if unused.
 *
 * The texture_id must be set manually after init to a loaded texture.
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
 * @brief Creates a triangle mesh.
 */
struct PRGLMesh *prgl_create_triangle(void);

/**
 * @brief Creates a quad mesh.
 */
struct PRGLMesh *prgl_create_quad(void);

/**
 * @brief Creates a cube mesh.
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
