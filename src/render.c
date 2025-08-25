#include "glad.h"
#include "common_macros.h"
#include "screen_internal.h"
#include "shaders.h"
#include "render.h"
#include "render_internal.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/types.h"
#include "mesh.h"
#include "transform_internal.h"
#include <complex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

const vec2 PR3D_RENDER_RESOLUTION = {320.0f, 180.0f};

void pr3d_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

struct PR3DMesh *pr3d_create_screen_quad(void)
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
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    struct PR3DMesh *mesh_pointer = malloc(sizeof(struct PR3DMesh));
    if (mesh_pointer == NULL)
    {
        printf("pr3d_create_screen_quad: Error allocating mesh "
               "pointer memory!");
    }

    // Texture zero'd, must be set after using load_texture()
    *mesh_pointer = (struct PR3DMesh){
        .num_vertices = ARR_LEN(indices),
        .vao = vao,
        .vbo = vbo,
        .ebo = ebo,
        .texture = 0,
    };
    return mesh_pointer;
}

void pr3d_render_mesh(
    struct PR3DMesh *mesh, vec3 position, vec3 rotation_axis, float degrees,
    vec3 scale
)
{
    glBindVertexArray(mesh->vao);

    if (mesh->texture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, mesh->texture);
    }

    // Transform the mesh to the render position.
    mat4 model;
    pr3d_create_model_matrix(model, position, rotation_axis, degrees, scale);
    pr3d_set_shader_uniform_mat4(
        pr3d_current_shader(), PR3D_MODEL_UNIFORM, model
    );

    if (mesh->ebo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh->num_vertices, GL_UNSIGNED_INT, 0);
    }
}

void pr3d_render_mesh_2d(
    struct PR3DMesh *mesh, vec2 position, float rotation_degrees, vec2 scale
)
{
    glBindVertexArray(mesh->vao);

    if (mesh->texture != 0)
    {
        glBindTexture(GL_TEXTURE_2D, mesh->texture);
    }

    // Transform the mesh to the render position.
    mat4 trans;
    glm_mat4_identity(trans);
    glm_translate(trans, (vec3){position[0], position[1], 0.0f});

    glm_translate(trans, (vec3){0.5f * scale[0], 0.5f * scale[1], 0.0f});
    glm_rotate(trans, glm_rad(rotation_degrees), (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(trans, (vec3){-0.5f * scale[0], -0.5f * scale[1], 0.0f});

    glm_scale(trans, (vec3){scale[0], scale[1], 1.0f});
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_2D), PR3D_MODEL_UNIFORM, trans
    );

    if (mesh->ebo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, mesh->num_vertices, GL_UNSIGNED_INT, 0);
    }
}

void pr3d_update_lighting(struct PR3DPointLight point_lights[], int num_lights)
{
    if (num_lights > PR3D_MAX_POINT_LIGHTS)
    {
        num_lights = PR3D_MAX_POINT_LIGHTS;
    }

    char uniform_name_buffer[64];
    for (int i = 0; i < num_lights; i++)
    {
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PR3D_LIGHT_COLOR_UNIFORM
        );
        pr3d_set_shader_uniform_vec3(
            pr3d_current_shader(), uniform_name_buffer,
            point_lights[i].lightColor
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PR3D_LIGHT_POSITION_UNIFORM
        );
        pr3d_set_shader_uniform_vec3(
            pr3d_current_shader(), uniform_name_buffer, point_lights[i].position
        );

        float linear_constant = 0.09f;
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PR3D_LIGHT_LINEAR_UNIFORM
        );
        pr3d_set_shader_uniform_vec3(
            pr3d_current_shader(), uniform_name_buffer, &linear_constant
        );

        float quadratic_constant = 0.032f;
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PR3D_LIGHT_QUADRATIC_UNIFORM
        );
        pr3d_set_shader_uniform_vec3(
            pr3d_current_shader(), uniform_name_buffer, &quadratic_constant
        );
    }
}

void pr3d_enable_render_texture(unsigned int fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, PR3D_RENDER_RESOLUTION[0], PR3D_RENDER_RESOLUTION[1]);
    pr3d_clear_screen(0.1f, 0.1f, 0.1f, 1.0f);
}

void pr3d_render_render_texture(
    unsigned int render_texture, struct PR3DMesh *screen_quad
)
{
    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set the viewport to the actual window size
    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(pr3d_screen()->window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    // Clear just the color buffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the screen quad to the window
    pr3d_use_shader(pr3d_shader(PR3D_SHADER_SCREEN));
    glBindVertexArray(screen_quad->vao);
    glBindTexture(GL_TEXTURE_2D, render_texture);
    glDrawElements(GL_TRIANGLES, screen_quad->num_vertices, GL_UNSIGNED_INT, 0);
}
