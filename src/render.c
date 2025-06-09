#include "glad.h"
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

    struct PR3DMesh mesh = {.vao = vao, .vbo = vbo};
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

    struct PR3DMesh mesh = {.vao = vao, .vbo = vbo};
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void pr3d_delete_mesh(struct PR3DMesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
}
