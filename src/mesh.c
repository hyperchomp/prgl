#include "glad.h"
#include "common_macros.h"
#include "mesh.h"
#include "cglm/types.h"
#include <stdio.h>
#include <stdlib.h>

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
        .texture = 0,
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

    vec3 min_bounds;
    vec3 max_bounds;
    int num_vertices = ARR_LEN(indices);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_rectangle: Error allocating mesh "
               "pointer memory!");
    }

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = num_vertices,
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture = 0,
        .min_bounds = {min_bounds[0], min_bounds[1], min_bounds[2]},
        .max_bounds = {max_bounds[0], max_bounds[1], max_bounds[2]},
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

    // Bounding box
    vec3 min_bounds;
    vec3 max_bounds;
    int num_vertices = 36;

    // Create array of just vertex positions without the other data
    float positions[num_vertices * 3];
    for (int i = 0; i < 36; ++i)
    {
        positions[i * 3 + 0] = vertices[i * 8 + 0];
        positions[i * 3 + 1] = vertices[i * 8 + 1];
        positions[i * 3 + 2] = vertices[i * 8 + 2];
    }
    prgl_calculate_aabb(vertices, num_vertices, min_bounds, max_bounds);

    struct PRGLMesh *mesh_pointer = malloc(sizeof(struct PRGLMesh));
    if (mesh_pointer == NULL)
    {
        printf("prgl_create_cube: Error allocating mesh pointer memory!");
    }

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PRGLMesh){
        .num_vertices = num_vertices,
        .vao = vao,
        .vbo = vbo,
        .ebo = 0,
        .texture = 0,
        .min_bounds = {min_bounds[0], min_bounds[1], min_bounds[2]},
        .max_bounds = {max_bounds[0], max_bounds[1], max_bounds[2]},
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
}

void prgl_calculate_aabb(
    const float *vertices, int num_vertices, vec3 min_bounds, vec3 max_bounds
)
{
    // Initialize bounds using first vertex
    min_bounds[0] = vertices[0];
    min_bounds[1] = vertices[1];
    min_bounds[2] = vertices[2];

    max_bounds[0] = vertices[0];
    max_bounds[1] = vertices[1];
    max_bounds[2] = vertices[2];

    for (int i = 0; i < num_vertices; i++)
    {
        // Find min bounds from the points in the vertices
        for (int p = 0; p < 3; p++)
        {
            if (vertices[i * 3 + p] < min_bounds[p])
            {
                min_bounds[p] = vertices[i * 3 + p];
            }
        }

        // Find max bounds
        for (int p = 0; p < 3; p++)
        {
            if (vertices[i * 3 + p] > max_bounds[p])
            {
                max_bounds[p] = vertices[i * 3 + p];
            }
        }
    }
}
