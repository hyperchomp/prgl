#include "game_object.h"
#include "glad.h"
#include "screen_internal.h"
#include "shaders.h"
#include "render.h"
#include "render_internal.h"
#include "cglm/quat.h"
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

void prgl_render_game_object_3d(struct PRGLGameObject *const game_obj)
{
    glBindVertexArray(game_obj->mesh->vao);

    if (game_obj->mesh->texture_id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, game_obj->mesh->texture_id);
    }

    // Transform the mesh to the render position.
    mat4 model;
    prgl_create_model_matrix(&model, game_obj);
    prgl_set_shader_uniform_mat4(
        prgl_current_shader(), PRGL_MODEL_UNIFORM, model
    );

    if (game_obj->mesh->ebo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, game_obj->mesh->num_vertices);
    }
    else
    {
        glDrawElements(
            GL_TRIANGLES, game_obj->mesh->num_vertices, GL_UNSIGNED_INT, 0
        );
    }
}

void prgl_render_game_object_2d(struct PRGLGameObject *const game_obj)
{
    glBindVertexArray(game_obj->mesh->vao);

    if (game_obj->mesh->texture_id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, game_obj->mesh->texture_id);
    }

    // Transform the mesh to the render position.
    mat4 trans;
    vec2 position = {game_obj->position[0], game_obj->position[1]};
    vec2 scale = {game_obj->scale[0], game_obj->scale[1]};

    glm_mat4_identity(trans);
    glm_translate(trans, (vec3){position[0], position[1], 0.0f});

    // Perform the rotation about the center of the game object
    glm_translate(trans, (vec3){0.5f * scale[0], 0.5f * scale[1], 0.0f});
    glm_quat_rotate(trans, game_obj->orientation, trans);
    glm_translate(trans, (vec3){-0.5f * scale[0], -0.5f * scale[1], 0.0f});

    // Negate y-axis scale, cglm quats expects 3D right hand coordinate with +y
    // up, but our 2D orthogonal projection has 0,0 at top left so -y is up
    glm_scale(trans, (vec3){scale[0], -scale[1], 1.0f});
    prgl_set_shader_uniform_mat4(
        prgl_shader(PRGL_SHADER_2D), PRGL_MODEL_UNIFORM, trans
    );

    if (game_obj->mesh->ebo == 0)
    {
        glDrawArrays(GL_TRIANGLES, 0, game_obj->mesh->num_vertices);
    }
    else
    {
        glDrawElements(
            GL_TRIANGLES, game_obj->mesh->num_vertices, GL_UNSIGNED_INT, 0
        );
    }
}

void prgl_update_lighting(
    struct PRGLPointLight *const point_lights, int num_lights
)
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
    unsigned int render_texture, struct PRGLMesh *const screen_quad
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
