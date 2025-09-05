#include "mesh.h"
#include "mesh_internal.h"
#include "glad.h"
#include "common_macros.h"
#include "cglm/types.h"
#include "cglm/vec3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Constants for meshes with vertices, normals, and texture coordinates. If not
// all then these values will be different, only use in that scenario.
static const int VERTEX_STRIDE_LENGTH = 8;
static const GLint FLOAT_VERTEX_STRIDE = VERTEX_STRIDE_LENGTH * sizeof(float);
static const GLint NORMALS_OFFSET = 3 * sizeof(float);
static const GLint TEX_COORD_OFFSET = 6 * sizeof(float);

static void prgl_setup_vertex_attributes(void);
static void prgl_generate_cube_sphere_point(
    vec3 point, float u, float v, vec3 face_right, vec3 face_up,
    vec3 face_normal, vec3 quad_right, vec3 quad_up
);

void prgl_init_mesh(
    struct PRGLMesh *mesh, int num_vertices, unsigned int vao, unsigned int vbo,
    unsigned int ebo
)
{
    *mesh = (struct PRGLMesh){
        .num_vertices = num_vertices,
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture_id = 0,
    };
}

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
        2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        (const GLvoid *)(intptr_t)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_screen_quad: Error allocating mesh "
                    "pointer memory!"
        );
    }

    prgl_init_mesh(mesh, ARR_LEN(indices), vao, vbo, ebo);
    return mesh;
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
        2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        (const GLvoid *)(intptr_t)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_triangle: Error allocating mesh pointer memory!"
        );
    }

    prgl_init_mesh(mesh, 3, vao, vbo, 0);
    return mesh;
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

    prgl_setup_vertex_attributes();

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_rectangle: Error allocating mesh "
                    "pointer memory!"
        );
    }

    prgl_init_mesh(mesh, ARR_LEN(indices), vao, vbo, ebo);
    return mesh;
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

    prgl_setup_vertex_attributes();

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr, "prgl_create_cube: Error allocating mesh pointer memory!"
        );
    }

    prgl_init_mesh(mesh, 36, vao, vbo, 0);
    return mesh;
}

struct PRGLMesh *prgl_create_cube_sphere(int resolution)
{
    resolution = resolution > 1 ? resolution : 1;

    // Six faces, resolution squared quads, six vertices per quad
    const int NUM_FACES = 6;
    int num_vertices = NUM_FACES * (resolution * resolution) * NUM_FACES;
    int vertex_data_length = num_vertices * VERTEX_STRIDE_LENGTH;

    float *vertex_data = malloc(sizeof(float) * vertex_data_length);
    if (vertex_data == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_cube_sphere: Error allocating vertex data memory!"
        );
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

    int component_index = 0;
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
                    vertex_data[component_index++] = position[0];
                    vertex_data[component_index++] = position[1];
                    vertex_data[component_index++] = position[2];

                    // Normal XYZ coordinates
                    vertex_data[component_index++] = position[0];
                    vertex_data[component_index++] = position[1];
                    vertex_data[component_index++] = position[2];

                    // Texture UV coordinates
                    vertex_data[component_index++] = uvs[vert][0];
                    vertex_data[component_index++] = uvs[vert][1];
                }
            }
        }
    }

    unsigned int vbo;
    unsigned int vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(float) * vertex_data_length, vertex_data,
        GL_STATIC_DRAW
    );

    prgl_setup_vertex_attributes();
    free(vertex_data);

    struct PRGLMesh *mesh = malloc(sizeof(struct PRGLMesh));
    if (mesh == NULL)
    {
        fprintf(
            stderr,
            "prgl_create_cube_sphere: Error allocating mesh pointer memory!"
        );
    }

    prgl_init_mesh(mesh, num_vertices, vao, vbo, 0);
    return mesh;
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

static void prgl_setup_vertex_attributes(void)
{
    // position attribute
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, FLOAT_VERTEX_STRIDE, (void *)0
    );
    glEnableVertexAttribArray(0);

    // normals attribute
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, FLOAT_VERTEX_STRIDE,
        (const GLvoid *)(intptr_t)(NORMALS_OFFSET)
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
