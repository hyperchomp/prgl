#include "glad.h"
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
#include <GLFW/glfw3.h>

const vec2 PRGL_RENDER_RESOLUTION = {320.0f, 180.0f};

void prgl_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void prgl_render_mesh(
    struct PRGLMesh *mesh, vec3 position, vec3 rotation_axis, float degrees,
    vec3 scale
)
{
    glBindVertexArray(mesh->vao);

    if (mesh->texture_id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, mesh->texture_id);
    }

    // Transform the mesh to the render position.
    mat4 model;
    prgl_create_model_matrix(model, position, rotation_axis, degrees, scale);
    prgl_set_shader_uniform_mat4(
        prgl_current_shader(), PRGL_MODEL_UNIFORM, model
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

void prgl_render_mesh_2d(
    struct PRGLMesh *mesh, vec2 position, float rotation_degrees, vec2 scale
)
{
    glBindVertexArray(mesh->vao);

    if (mesh->texture_id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, mesh->texture_id);
    }

    // Transform the mesh to the render position.
    mat4 trans;
    glm_mat4_identity(trans);
    glm_translate(trans, (vec3){position[0], position[1], 0.0f});

    glm_translate(trans, (vec3){0.5f * scale[0], 0.5f * scale[1], 0.0f});
    glm_rotate(trans, glm_rad(rotation_degrees), (vec3){0.0f, 0.0f, 1.0f});
    glm_translate(trans, (vec3){-0.5f * scale[0], -0.5f * scale[1], 0.0f});

    glm_scale(trans, (vec3){scale[0], scale[1], 1.0f});
    prgl_set_shader_uniform_mat4(
        prgl_shader(PRGL_SHADER_2D), PRGL_MODEL_UNIFORM, trans
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

void prgl_update_lighting(struct PRGLPointLight point_lights[], int num_lights)
{
    if (num_lights > PRGL_MAX_POINT_LIGHTS)
    {
        num_lights = PRGL_MAX_POINT_LIGHTS;
    }

    char uniform_name_buffer[64];
    for (int i = 0; i < num_lights; i++)
    {
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_COLOR_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer,
            point_lights[i].lightColor
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_POSITION_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer, point_lights[i].position
        );

        float linear_constant = 0.09f;
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_LINEAR_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer, &linear_constant
        );

        float quadratic_constant = 0.032f;
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_QUADRATIC_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer, &quadratic_constant
        );
    }
}

void prgl_enable_render_texture(unsigned int fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, PRGL_RENDER_RESOLUTION[0], PRGL_RENDER_RESOLUTION[1]);
    prgl_clear_screen(0.1f, 0.1f, 0.1f, 1.0f);
}

void prgl_render_render_texture(
    unsigned int render_texture, struct PRGLMesh *screen_quad
)
{
    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set the viewport to the actual window size
    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(prgl_screen()->window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    // Clear just the color buffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the screen quad to the window
    prgl_use_shader(prgl_shader(PRGL_SHADER_SCREEN));
    glBindVertexArray(screen_quad->vao);
    glBindTexture(GL_TEXTURE_2D, render_texture);
    glDrawElements(GL_TRIANGLES, screen_quad->num_vertices, GL_UNSIGNED_INT, 0);
}
