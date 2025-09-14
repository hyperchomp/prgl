#include "glad.h"

#include "render.h"
#include "render_internal.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"
#include "cglm/types.h"
#include "game_object.h"
#include "mesh_internal.h"
#include "screen_internal.h"
#include "shaders.h"
#include "transform_internal.h"

const vec2 PRGL_RENDER_RESOLUTION = {320.0f, 180.0f};

void prgl_clear_screen(float r, float g, float b, float a)
{
    glClearColor((GLfloat)r, (GLfloat)g, (GLfloat)b, (GLfloat)a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void prgl_draw_game_object_3d(struct PRGLGameObject *const game_obj)
{
    struct PRGLMesh *const mesh = (struct PRGLMesh *)game_obj->mesh;

    glBindVertexArray(mesh->vao);

    // Transform the mesh to the render position.
    mat4 model;
    mat3 normal_matrix;
    prgl_create_model_matrix(model, game_obj);
    prgl_set_shader_uniform_mat4(
        prgl_current_shader(), PRGL_MODEL_UNIFORM, model
    );
    prgl_set_shader_uniform_vec3(
        prgl_current_shader(), PRGL_FILL_COLOR_UNIFORM, game_obj->color
    );
    if (prgl_current_shader().id != prgl_shader(PRGL_SHADER_TYPE_UNLIT).id)
    {
        prgl_create_normal_matrix(normal_matrix, model);
        prgl_set_shader_uniform_mat3(
            prgl_current_shader(), PRGL_NORMAL_MATRIX_UNIFORM, normal_matrix
        );
        if (mesh->texture.id == 0)
        {
            prgl_set_shader_uniform_bool(
                prgl_current_shader(), PRGL_USE_TEXTURE_UNIFORM, false
            );
        }
        else
        {
            prgl_set_shader_uniform_bool(
                prgl_current_shader(), PRGL_USE_TEXTURE_UNIFORM, true
            );
            glBindTexture(GL_TEXTURE_2D, (GLuint)mesh->texture.id);
        }
    }

    if (mesh->ebo == 0)
    {
        glDrawArrays(mesh->primitive_type, 0, mesh->num_vertices);
    }
    else
    {
        glDrawElements(
            mesh->primitive_type, mesh->num_vertices, GL_UNSIGNED_INT, 0
        );
    }
}

void prgl_draw_game_object_2d(struct PRGLGameObject *const game_obj)
{
    struct PRGLMesh *const mesh = (struct PRGLMesh *)game_obj->mesh;

    glBindVertexArray(mesh->vao);

    if (mesh->texture.id == 0)
    {
        prgl_set_shader_uniform_bool(
            prgl_current_shader(), PRGL_USE_TEXTURE_UNIFORM, false
        );
    }
    else
    {
        prgl_set_shader_uniform_bool(
            prgl_current_shader(), PRGL_USE_TEXTURE_UNIFORM, true
        );
        glBindTexture(GL_TEXTURE_2D, (GLuint)mesh->texture.id);
    }

    // Transform the mesh to the render position.
    mat4 trans;
    vec2 position = {game_obj->position[0], game_obj->position[1]};
    vec2 scale = {game_obj->scale[0], game_obj->scale[1]};

    glm_mat4_identity(trans);

    // Same meshes are used for 2D/3D, and they are center aligned unit meshes.
    // For 2D we want top left for position, so we need to offset.
    vec2 center_offset = {scale[0] / 2.0f, scale[1] / 2.0f};
    glm_translate(trans, (vec3){position[0], position[1], 0.0f});
    glm_translate(trans, (vec3){center_offset[0], center_offset[1], 0.0f});

    // Perform the rotation about the center of the game object
    glm_quat_rotate(trans, game_obj->orientation, trans);

    // Negate y-axis scale, cglm quats expects 3D right hand coordinate with +y
    // up, but our 2D orthogonal projection has 0,0 at top left so -y is up
    glm_scale(trans, (vec3){scale[0], -scale[1], 1.0f});
    prgl_set_shader_uniform_mat4(
        prgl_current_shader(), PRGL_MODEL_UNIFORM, trans
    );
    prgl_set_shader_uniform_vec3(
        prgl_current_shader(), PRGL_FILL_COLOR_UNIFORM, game_obj->color
    );

    if (mesh->ebo == 0)
    {
        glDrawArrays(mesh->primitive_type, 0, mesh->num_vertices);
    }
    else
    {
        glDrawElements(
            mesh->primitive_type, mesh->num_vertices, GL_UNSIGNED_INT, 0
        );
    }
}

void prgl_enable_render_texture(GLuint fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, PRGL_RENDER_RESOLUTION[0], PRGL_RENDER_RESOLUTION[1]);
    prgl_clear_screen(0.1f, 0.1f, 0.1f, 1.0f);
}

void prgl_render_render_texture(struct PRGLMesh *const screen_quad)
{
    // Switch back to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Set the viewport to the actual window size
    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(prgl_screen()->window, &windowWidth, &windowHeight);
    glViewport(0, 0, (GLint)windowWidth, (GLint)windowHeight);

    // Render the screen quad to the window
    prgl_use_shader(prgl_shader(PRGL_SHADER_TYPE_SCREEN));
    glBindVertexArray(screen_quad->vao);
    glBindTexture(GL_TEXTURE_2D, (GLuint)screen_quad->texture.id);
    glDrawElements(
        screen_quad->primitive_type, screen_quad->num_vertices, GL_UNSIGNED_INT,
        0
    );
}
