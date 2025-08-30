#include "glad.h"
#include "common_macros.h"
#include "mesh.h"
#include "cglm/types.h"
#include <stdio.h>
#include <stdlib.h>

struct PRGLMesh *prgl_create_screen_quad(void)
{
    // clang-format off
    mat4 vertices = {
        {-1.0f,  1.0f, 0.0f}, // Top-left
        { 1.0f,  1.0f, 0.0f}, // Top-right
        { 1.0f, -1.0f, 0.0f}, // Bottom-right
        {-1.0f, -1.0f, 0.0f}  // Bottom-left
    };
    mat4x2 texture_coords = {
        {0.0f, 1.0f}, // Top-left
        {1.0f, 1.0f}, // Top-right
        {1.0f, 0.0f}, // Bottom-right
        {0.0f, 0.0f}  // Bottom-left
    };
    
    float combined_data[20] = {
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
    unsigned int indices[] = {
        0, 1, 3, // First triangle
        1, 2, 3  // Second triangle
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(combined_data), combined_data, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to interpret the texture coordinate data
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_screen_quad: Error allocating mesh "
               "pointer memory!");
    }

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = ARR_LEN(indices),
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture_id = 0,
    };
    return mesh_pointer;
}

struct PRGLMesh *prgl_create_triangle(mat3 vertices)
{
    // clang-format off
    
    mat3x2 texture_coords = {
        {0.0f, 0.0f}, 
        {1.0f, 0.0f}, 
        {0.5f, 1.0f}, 
    };
    // Combine vertex position and texture coordinate data
    float combined_data[15] = {
              vertices[0][0],       vertices[0][1], vertices[0][2],
        texture_coords[0][0], texture_coords[0][1],
              vertices[1][0],       vertices[1][1], vertices[1][2],
        texture_coords[1][0], texture_coords[1][1],
              vertices[2][0],       vertices[2][1], vertices[2][2],
        texture_coords[2][0], texture_coords[2][1],
    };
    // clang-format on

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(combined_data), combined_data, GL_STATIC_DRAW
    );

    // Tell OpenGL how to interpret our vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_triangle: Error allocating mesh pointer memory!");
    }

    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = 3,
        .vao = vao,
        .vbo = vbo,
        .ebo = 0,
        .texture_id = 0,
    };
    return mesh_pointer;
}

struct PRGLMesh *prgl_create_quad(mat4 vertices)
{
    // clang-format off
    mat4x2 texture_coords = {
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 1.0f}
    };

    // Assumes quad is oriented on XY plane
    vec3 normal = {0.0f, 0.0f, 1.0f};
    
    float combined_data[32] = {
              vertices[0][0],       vertices[0][1], vertices[0][2], 
                   normal[0],            normal[1],      normal[2],
        texture_coords[0][0], texture_coords[0][1],
              vertices[1][0],       vertices[1][1], vertices[1][2], 
                   normal[0],            normal[1],      normal[2],
        texture_coords[1][0], texture_coords[1][1],
              vertices[2][0],       vertices[2][1], vertices[2][2], 
                   normal[0],            normal[1],      normal[2],
        texture_coords[2][0], texture_coords[2][1],
              vertices[3][0],       vertices[3][1], vertices[3][2], 
                   normal[0],            normal[1],      normal[2],
        texture_coords[3][0], texture_coords[3][1]
    };
    // clang-format on

    // EBO stops us from needing overlapping vertices, but we need to tell
    // OpenGL the order to go over the existing ones again to create enough
    // triangles to create our mesh (in this case 2 triangles, 6 indices)
    unsigned int indices[] = {
        0, 1, 3, // First triangle
        1, 2, 3  // Second triangle
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(combined_data), combined_data, GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to interpret the normal data
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Tell OpenGL how to interpret the texture coordinate data
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_rectangle: Error allocating mesh "
               "pointer memory!");
    }

    int num_vertices = ARR_LEN(indices);

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = num_vertices,
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture_id = 0,
    };
    return mesh_pointer;
}

struct PRGLMesh *prgl_create_cube(void)
{
    // clang-format off
    const float vertices[] = {
        // vertices           // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    // clang-format on

    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // normals attribute
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_cube: Error allocating mesh pointer memory!");
    }

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = 36,
        .vao = vao,
        .vbo = vbo,
        .ebo = 0,
        .texture_id = 0,
    };
    return mesh_pointer;
}

void prgl_delete_mesh(struct PRGLMesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);

    if (mesh->ebo != 0)
    {
        glDeleteBuffers(1, &mesh->ebo);
    }

    free(mesh);
}
