#include "glad.h"
#include "common_macros.h"
#include "shaders.h"
#include "render.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

void pr3d_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void pr3d_set_render_color(float r, float g, float b, float a)
{
    pr3d_set_shader_uniform_4f(
        pr3d_shader(PR3D_SHADER_SOLID_COLOR), "fillColor", r, g, b, a
    );
}

struct PR3DMesh *pr3d_create_triangle(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3
)
{
    float vertices[] = {
        v1->x, v1->y, v1->z, //
        v2->x, v2->y, v2->z, //
        v3->x, v3->y, v3->z  //
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret our vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    struct PR3DMesh mesh = {
        .num_vertices = 3, .vao = vao, .vbo = vbo, .ebo = 0, .texture = 0
    };
    struct PR3DMesh *mesh_pointer = malloc(sizeof(struct PR3DMesh));
    if (mesh_pointer == NULL)
    {
        printf("pr3d_create_triangle: Error allocating mesh pointer memory!");
    }
    *mesh_pointer = mesh;
    return mesh_pointer;
}

struct PR3DMesh *pr3d_create_triangle_vertex_color(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3,
    struct PR3DVec3 *c1, struct PR3DVec3 *c2, struct PR3DVec3 *c3
)
{
    float vertices[] = {
        v1->x, v1->y, v1->z, c1->x, c1->y, c1->z, //
        v2->x, v2->y, v2->z, c2->x, c2->y, c2->z, //
        v3->x, v3->y, v3->z, c3->x, c3->y, c3->z  //
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to interpret the color data
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    struct PR3DMesh mesh = {
        .num_vertices = 3, .vao = vao, .vbo = vbo, .ebo = 0, .texture = 0
    };
    struct PR3DMesh *mesh_pointer = malloc(sizeof(struct PR3DMesh));
    if (mesh_pointer == NULL)
    {
        printf("pr3d_create_triangle: Error allocating mesh pointer memory!");
    }
    *mesh_pointer = mesh;
    return mesh_pointer;
}

struct PR3DMesh *pr3d_create_rectangle_textured(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3,
    struct PR3DVec3 *v4, struct PR3DVec3 *c1, struct PR3DVec3 *c2,
    struct PR3DVec3 *c3, struct PR3DVec3 *c4, struct PR3DVec2 *t1,
    struct PR3DVec2 *t2, struct PR3DVec2 *t3, struct PR3DVec2 *t4
)
{
    float vertices[] = {
        // vertices          // colors            // texture coordinates
        v1->x, v1->y, v1->z, c1->x, c1->y, c1->z, t1->x, t1->y, //
        v2->x, v2->y, v2->z, c2->x, c2->y, c2->z, t2->x, t2->y, //
        v3->x, v3->y, v3->z, c3->x, c3->y, c3->z, t3->x, t3->y, //
        v4->x, v4->y, v4->z, c4->x, c4->y, c4->z, t4->x, t4->y  //
    };

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    // Tell OpenGL how to interpret the vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // Tell OpenGL how to interpret the color data
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // Tell OpenGL how to interpret the texture coordinate data
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // Texture zero'd, must be set after using load_texture()
    struct PR3DMesh mesh = {
        .num_vertices = ARR_LEN(indices),
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture = 0
    };
    struct PR3DMesh *mesh_pointer = malloc(sizeof(struct PR3DMesh));
    if (mesh_pointer == NULL)
    {
        printf("pr3d_create_triangle: Error allocating mesh pointer memory!");
    }
    *mesh_pointer = mesh;
    return mesh_pointer;
}

void pr3d_render_mesh(struct PR3DMesh *mesh)
{
    glBindVertexArray(mesh->vao);

    if (mesh->texture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, mesh->texture);
    }

    if (mesh->ebo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh->num_vertices, GL_UNSIGNED_INT, 0);
    }
}

void pr3d_delete_mesh(struct PR3DMesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);

    if (mesh->ebo != 0)
    {
        glDeleteBuffers(1, &mesh->ebo);
    }
}
