#include "glad.h"
#include "game.h"
#include "camera.h"
#include "mesh.h"
#include "mesh_internal.h"
#include "render.h"
#include "render_internal.h"
#include "shaders.h"
#include "texture_internal.h"
#include "screen_internal.h"
#include "shaders_internal.h"
#include "cglm/vec2.h"
#include <GLFW/glfw3.h>

static double last_update_start = 0;
static double dt = 0;
struct PRGLRenderTexture render_texture;
struct PRGLMesh *screen_render_quad;

void prgl_run_game(
    const char *const title, void (*prgl_init)(void), void (*prgl_update)(void),
    void (*prgl_draw_3d)(void), void (*prgl_draw_2d)(void),
    void (*prgl_cleanup)(void)
)
{
    glfwInit();
    prgl_create_window(title);

    prgl_init_shader_pool();
    render_texture = prgl_create_render_texture();
    screen_render_quad = prgl_create_screen_quad(render_texture.texture);
    prgl_init();

    struct PRGLScreen screen = *prgl_screen();
    while (!glfwWindowShouldClose(screen.window))
    {
        vec2 render_res = {
            PRGL_RENDER_RESOLUTION[0], PRGL_RENDER_RESOLUTION[1]
        };

        dt = glfwGetTime() - last_update_start;
        last_update_start = glfwGetTime();

        prgl_enable_render_texture(render_texture.fbo);
        glEnable(GL_DEPTH_TEST);
        prgl_use_shader_3d();
        prgl_set_shader_uniform_vec2(
            prgl_current_shader(), PRGL_TILE_FACTOR_UNIFORM, GLM_VEC2_ONE
        );
        prgl_set_shader_uniform_vec2(
            prgl_current_shader(), PRGL_RENDER_RESOLUTION_UNIFORM, render_res
        );
        prgl_set_camera_projection(
            prgl_active_camera(), prgl_active_camera()->fov,
            PRGL_CAMERA_PROJECTION_PERSPECTIVE
        );
        prgl_update();

        prgl_draw_3d();

        glDisable(GL_DEPTH_TEST);
        prgl_use_shader_2d();
        prgl_set_shader_uniform_vec2(
            prgl_current_shader(), PRGL_TILE_FACTOR_UNIFORM, GLM_VEC2_ONE
        );
        prgl_set_shader_uniform_vec2(
            prgl_current_shader(), PRGL_RENDER_RESOLUTION_UNIFORM, render_res
        );
        prgl_set_camera_projection(
            prgl_active_camera(), 0.0f, PRGL_CAMERA_PROJECTION_ORTHOGONAL
        );
        prgl_draw_2d();

        prgl_render_render_texture(screen_render_quad);

        prgl_cleanup();

        glfwSwapBuffers(screen.window);
        glfwPollEvents();
    }

    prgl_delete_mesh(screen_render_quad);

    prgl_delete_shader_pool();
    prgl_destroy_window();
}

double prgl_delta_time(void) { return dt; }
