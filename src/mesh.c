#include "glad.h"

#include "mesh.h"
#include "mesh_internal.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "common_macros.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include "texture.h"
#include "types.h"

// Constants for meshes with vertices, normals, and texture coordinates. If not
// all then these values will be different, only use in that scenario.
static const GLint VERTEX_STRIDE_LENGTH = 8;
static const GLint FLOAT_VERTEX_STRIDE = VERTEX_STRIDE_LENGTH * sizeof(GLfloat);
static const GLint NORMALS_OFFSET = 3 * sizeof(GLfloat);
static const GLint TEX_COORD_OFFSET = 6 * sizeof(GLfloat);

// Normal for 2D shapes, assumes positioning on XY, thus +Z normal
static const vec3 NORMAL_POS_Z = {0.0f, 0.0f, 1.0f};

static void prgl_setup_vertex_attributes(void);
static void prgl_generate_cube_sphere_point(
    vec3 point, float u, float v, vec3 face_right, vec3 face_up,
    vec3 face_normal, vec3 quad_right, vec3 quad_up
);

void prgl_init_mesh(
    struct PRGLMesh *mesh, GLuint num_vertices, GLuint vao, GLuint vbo,
    GLuint ebo, PRGLTexture texture, GLenum primitive_type
)
{
    *mesh = (struct PRGLMesh){
        .num_vertices = num_vertices,
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture = {.id = texture.id},
        .primitive_type = primitive_type,
    };
}

struct PRGLMesh *prgl_create_screen_quad(PRGLTexture texture)
{
    // clang-format off
    mat4 vertices = {
        {-1.0f,  1.0f, 0.0f}, // Top-left
        {-1.0f, -1.0f, 0.0f}, // Bottom-left
        { 1.0f, -1.0f, 0.0f}, // Bottom-right
        { 1.0f,  1.0f, 0.0f}, // Top-right
    };
    mat4x2 texture_coords = {
        {0.0f, 1.0f}, // Top-left (y=1)
        {0.0f, 0.0f}, // Bottom-left (y=0)
        {1.0f, 0.0f}, // Bottom-right (y=0)
        {1.0f, 1.0f}  // Top-right (y=1)
    };
    
    const GLfloat vertex_data[20] = {
              vertices[0][0],       vertices[0][1], vertices[0][2], 
        texture_coords[0][0], texture_coords[0][1],
              vertices[1][0],       vertices[1][1], vertices[1][2], 
        texture_coords[1][0], texture_coords[1][1],
              vertices[2][0],       vertices[2][1], vertices[2][2], 
        texture_coords[2][0], texture_coords[2][1],
              vertices[3][0],       vertices[3][1], vertices[3][2], 
        texture_coords[3][0], texture_coords[3][1]
    };
    // clang-format on

    // EBO stops us from needing overlapping vertices, but we need to tell
    // OpenGL the order to go over the existing ones again to create enough
    // triangles to create our mesh (in this case 2 triangles, 6 indices)
    const GLuint indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid *)0
    );
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to interpret the texture coordinate data
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
        (const GLvoid *)(intptr_t)(3 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_screen_quad: Error allocating mesh "
                    "pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(
        mesh, (GLuint)ARR_LEN(indices), vao, vbo, ebo, texture, GL_TRIANGLES
    );
    return mesh;
}

PRGLMeshHandle prgl_create_triangle(PRGLTexture texture)
{
    // clang-format off
    mat3 vertices = {
        {-0.5f, -0.5f, 0.0f}, // Bottom left
        { 0.5f, -0.5f, 0.0f}, // Bottom right
        { 0.0f,  0.5f, 0.0f}, // Top center
    };
    mat3x2 texture_coords = {
        {0.0f, 0.0f}, 
        {1.0f, 0.0f}, 
        {0.5f, 1.0f}, 
    };
    // Combine vertex position and texture coordinate data
    const GLfloat vertex_data[24] = {
              vertices[0][0],       vertices[0][1], vertices[0][2],
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[0][0], texture_coords[0][1],
              vertices[1][0],       vertices[1][1], vertices[1][2],
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[1][0], texture_coords[1][1],
              vertices[2][0],       vertices[2][1], vertices[2][2],
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[2][0], texture_coords[2][1],
    };
    // clang-format on

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    GLuint vbo;
    GLuint vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_triangle: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(mesh, (GLuint)3, vao, vbo, 0, texture, GL_TRIANGLES);
    return mesh;
}

PRGLMeshHandle prgl_create_circle(PRGLTexture texture, int num_edges)
{
    num_edges = (num_edges < 3) ? 3 : num_edges;

    // Need one extra vertex for the center
    GLuint num_vertices = num_edges + 1;
    GLfloat *vertex_data =
        malloc(sizeof(GLfloat) * num_vertices * VERTEX_STRIDE_LENGTH);
    if (vertex_data == NULL)
    {
        fprintf(
            stderr, "prgl_create_circle: Error allocating vertex data memory!"
        );
        return NULL;
    }

    // Allocate memory for indices
    GLuint num_indices = num_edges * 3;
    GLuint *indices = malloc(sizeof(GLuint) * num_indices);
    if (indices == NULL)
    {
        fprintf(stderr, "prgl_create_circle: Error allocating indices memory!");
        free(vertex_data);
        return NULL;
    }

    // Center vertex
    vertex_data[0] = 0.0f; // x
    vertex_data[1] = 0.0f; // y
    vertex_data[2] = 0.0f; // z
    vertex_data[3] = NORMAL_POS_Z[0];
    vertex_data[4] = NORMAL_POS_Z[1];
    vertex_data[5] = NORMAL_POS_Z[2];
    vertex_data[6] = 0.5f; // u
    vertex_data[7] = 0.5f; // v

    // Outer vertices
    float angle_change = 2.0f * GLM_PI / num_edges;
    for (int i = 0; i < num_edges; i++)
    {
        float angle = i * angle_change;
        int attribute = (i + 1) * VERTEX_STRIDE_LENGTH;

        vertex_data[attribute++] = cosf(angle) * 0.5f; // x
        vertex_data[attribute++] = sinf(angle) * 0.5f; // y
        vertex_data[attribute++] = 0.0f;               // z
        vertex_data[attribute++] = NORMAL_POS_Z[0];
        vertex_data[attribute++] = NORMAL_POS_Z[1];
        vertex_data[attribute++] = NORMAL_POS_Z[2];
        vertex_data[attribute++] = (cosf(angle) * 0.5f) + 0.5f; // u
        vertex_data[attribute++] = (sinf(angle) * 0.5f) + 0.5f; // v

        // Set up the indices for the triangle
        indices[i * 3 + 0] = 0; // Center vertex
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = (i + 1) % num_edges + 1;
    }

    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(GLfloat) * num_vertices * VERTEX_STRIDE_LENGTH,
        vertex_data, GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_edges * 3, indices,
        GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();

    free(vertex_data);

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_circle: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(mesh, num_indices, vao, vbo, ebo, texture, GL_TRIANGLES);
    return mesh;
}

PRGLMeshHandle prgl_create_quad(PRGLTexture texture)
{
    // clang-format off
    mat4 vertices = {
        {-0.5f,  0.5f, 0.0f}, // Top-left
        {-0.5f, -0.5f, 0.0f}, // Bottom-left
        { 0.5f, -0.5f, 0.0f}, // Bottom-right
        { 0.5f,  0.5f, 0.0f}, // Top-right
    };
    mat4x2 texture_coords = {
        {0.0f, 1.0f},
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    };

    // Assumes quad is oriented on XY plane 
    const GLfloat vertex_data[32] = {
              vertices[0][0],       vertices[0][1],  vertices[0][2], 
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[0][0], texture_coords[0][1],
              vertices[1][0],       vertices[1][1],  vertices[1][2], 
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[1][0], texture_coords[1][1],
              vertices[2][0],       vertices[2][1],  vertices[2][2], 
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[2][0], texture_coords[2][1],
              vertices[3][0],       vertices[3][1],  vertices[3][2], 
             NORMAL_POS_Z[0],      NORMAL_POS_Z[1], NORMAL_POS_Z[2],
        texture_coords[3][0], texture_coords[3][1]
    };
    // clang-format on

    // EBO stops us from needing overlapping vertices, but we need to tell
    // OpenGL the order to go over the existing ones again to create enough
    // triangles to create our mesh (in this case 2 triangles, 6 indices)
    GLuint indices[] = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    GLuint vbo;
    GLuint vao;
    GLuint ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_quad: Error allocating mesh "
                    "pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(
        mesh, (GLuint)ARR_LEN(indices), vao, vbo, ebo, texture, GL_TRIANGLES
    );
    return mesh;
}

PRGLMeshHandle prgl_create_pyramid(PRGLTexture texture)
{
    // clang-format off
    
    const GLfloat vertex_data[144] = {
        // Vertices           // Normals               // Texture coords
        // Bottom
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,       0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,       0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,       1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,       0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,       1.0f, 1.0f,

        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.4472f, 0.8944f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.4472f, 0.8944f,  1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  0.0f, 0.4472f, 0.8944f,  0.5f, 1.0f,

        // Back face
         0.5f, -0.5f, -0.5f,  0.0f, 0.4472f, -0.8944f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.4472f, -0.8944f, 1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  0.0f, 0.4472f, -0.8944f, 0.5f, 1.0f,

        // Right face
         0.5f, -0.5f,  0.5f,  0.8944f, 0.4472f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.8944f, 0.4472f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f,  0.0f,  0.8944f, 0.4472f, 0.0f,  0.5f, 1.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, -0.8944f, 0.4472f, 0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -0.8944f, 0.4472f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f,  0.0f, -0.8944f, 0.4472f, 0.0f,  0.5f, 1.0f 
    };
    // clang-format on

    GLuint vbo;
    GLuint vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_pyramid: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(mesh, (GLuint)18, vao, vbo, 0, texture, GL_TRIANGLES);
    return mesh;
}

PRGLMeshHandle prgl_create_cube(PRGLTexture texture)
{
    // clang-format off
    const GLfloat vertices[] = {
        // vertices           // normals           // texture coords
        // Front Face 
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        // Back Face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

        // Left Face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        // Right Face
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

        // Bottom Face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        // Top Face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    // clang-format on

    GLuint vbo;
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    prgl_setup_vertex_attributes();

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_cube: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(mesh, (GLuint)36, vao, vbo, 0, texture, GL_TRIANGLES);
    return mesh;
}

PRGLMeshHandle prgl_create_cube_sphere(int resolution, PRGLTexture texture)
{
    resolution = resolution > 1 ? resolution : 1;

    // Six faces, resolution squared quads, six vertices per quad
    const int NUM_FACES = 6;
    GLuint num_vertices = NUM_FACES * (resolution * resolution) * NUM_FACES;
    GLuint vertex_data_length = num_vertices * VERTEX_STRIDE_LENGTH;

    GLfloat *vertex_data = malloc(sizeof(GLfloat) * vertex_data_length);
    if (vertex_data == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_cube_sphere: Error allocating vertex data memory!"
        );
        return NULL;
    }

    // clang-format off
    vec3 face_normals[6] = {
        { 0.0f,  0.0f,  1.0f}, // front
        { 0.0f,  0.0f, -1.0f}, // back
        { 0.0f,  1.0f,  0.0f}, // up
        { 0.0f, -1.0f,  0.0f}, // down
        { 1.0f,  0.0f,  0.0f}, // right
        {-1.0f,  0.0f,  0.0f},  // left
    };

    vec3 face_rights[6] = {
        { 1.0f,  0.0f,  0.0f}, // front
        {-1.0f,  0.0f,  0.0f}, // back
        { 1.0f,  0.0f,  0.0f}, // up
        { 1.0f,  0.0f,  0.0f}, // down
        { 0.0f,  0.0f, -1.0f}, // right
        { 0.0f,  0.0f,  1.0f},  // left
    };
    // clang-format on

    int attribute = 0;
    for (int face = 0; face < NUM_FACES; face++)
    {
        vec3 normal;
        glm_vec3_copy(face_normals[face], normal);

        vec3 right;
        glm_vec3_copy(face_rights[face], right);

        vec3 up;
        glm_vec3_cross(normal, right, up);

        // Iterate quads for the current face
        for (int y = 0; y < resolution; y++)
        {
            for (int x = 0; x < resolution; x++)
            {
                // Texture UV coordinate values, the plus ones are to fix seams
                float u = (float)x / resolution;
                float v = (float)y / resolution;
                float u1 = (float)(x + 1) / resolution;
                float v1 = (float)(y + 1) / resolution;

                // Four corner points on the current quad
                vec3 points[4];
                vec3 quad_right;
                vec3 quad_up;

                prgl_generate_cube_sphere_point(
                    points[0], u, v, right, up, normal, quad_right, quad_up
                );
                prgl_generate_cube_sphere_point(
                    points[1], u1, v, right, up, normal, quad_right, quad_up
                );
                prgl_generate_cube_sphere_point(
                    points[2], u1, v1, right, up, normal, quad_right, quad_up
                );
                prgl_generate_cube_sphere_point(
                    points[3], u, v1, right, up, normal, quad_right, quad_up
                );

                // Create two triangles from the points to form a quad
                vec3 triangle_points[6] = {
                    {points[0][0], points[0][1], points[0][2]},
                    {points[1][0], points[1][1], points[1][2]},
                    {points[2][0], points[2][1], points[2][2]},
                    {points[0][0], points[0][1], points[0][2]},
                    {points[2][0], points[2][1], points[2][2]},
                    {points[3][0], points[3][1], points[3][2]},
                };
                vec2 uvs[6] = {
                    {u, v}, {u1, v}, {u1, v1}, {u, v}, {u1, v1}, {u, v1},
                };

                // Normalize and project the points to vertices on the sphere
                for (int vert = 0; vert < 6; vert++)
                {
                    vec3 position;
                    glm_vec3_normalize_to(triangle_points[vert], position);

                    // Position XYZ coordinates
                    vertex_data[attribute++] = position[0];
                    vertex_data[attribute++] = position[1];
                    vertex_data[attribute++] = position[2];

                    // Normal XYZ coordinates
                    vertex_data[attribute++] = position[0];
                    vertex_data[attribute++] = position[1];
                    vertex_data[attribute++] = position[2];

                    // Texture UV coordinates
                    vertex_data[attribute++] = uvs[vert][0];
                    vertex_data[attribute++] = uvs[vert][1];
                }
            }
        }
    }

    GLuint vbo;
    GLuint vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(GLfloat) * vertex_data_length, vertex_data,
        GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();
    free(vertex_data);

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_cube_sphere: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(mesh, num_vertices, vao, vbo, 0, texture, GL_TRIANGLES);
    return mesh;
}

PRGLMeshHandle prgl_create_line_strip(vec3 points[], int num_points)
{
    if (num_points < 2)
    {
        fprintf(
            stderr,
            "prgl_create_line_strip: Can't make a line with less than 2 points!"
        );
        return NULL;
    }

    GLfloat *const vertex_data = malloc(sizeof(GLfloat) * 3 * num_points);
    if (vertex_data == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_line_strip: Error allocating vertex_data memory!"
        );
        return NULL;
    }

    for (int p = 0; p < num_points; p++)
    {
        vertex_data[p * 3 + 0] = points[p][0];
        vertex_data[p * 3 + 1] = points[p][1];
        vertex_data[p * 3 + 2] = points[p][2];
    }

    GLuint vbo;
    GLuint vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * num_points, vertex_data,
        GL_STATIC_DRAW
    );

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
    glEnableVertexAttribArray(0);

    free(vertex_data);

    PRGLMeshHandle mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_line_strip: Error allocating mesh pointer memory!"
        );
        return NULL;
    }

    prgl_init_mesh(
        mesh, (GLuint)num_points, vao, vbo, 0, PRGL_NO_TEXTURE, GL_LINE_STRIP
    );
    return mesh;
}

void prgl_delete_mesh(PRGLMeshHandle mesh)
{
    struct PRGLMesh *internal_mesh = (struct PRGLMesh *)mesh;
    glDeleteVertexArrays(1, &internal_mesh->vao);
    glDeleteBuffers(1, &internal_mesh->vbo);

    if (internal_mesh->ebo != 0)
    {
        glDeleteBuffers(1, &internal_mesh->ebo);
    }

    free(internal_mesh);
}

static void prgl_setup_vertex_attributes(void)
{
    // position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, FLOAT_VERTEX_STRIDE, (const GLvoid *)0
    );
    glEnableVertexAttribArray(0);

    // normals attribute
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, FLOAT_VERTEX_STRIDE,
        (const GLvoid *)(intptr_t)NORMALS_OFFSET
    );
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, FLOAT_VERTEX_STRIDE,
        (const GLvoid *)(intptr_t)TEX_COORD_OFFSET
    );
    glEnableVertexAttribArray(2);
}

/**
 * Used to generate each point for a quad while generating a cube sphere.
 */
static void prgl_generate_cube_sphere_point(
    vec3 point, float u, float v, vec3 face_right, vec3 face_up,
    vec3 face_normal, vec3 quad_right, vec3 quad_up
)
{
    glm_vec3_scale(face_right, u - 0.5f, quad_right);
    glm_vec3_scale(face_up, v - 0.5f, quad_up);
    glm_vec3_scale(face_normal, 0.5f, point);
    glm_vec3_add(point, quad_right, point);
    glm_vec3_add(point, quad_up, point);
}
