#include "glad.h"
#include "game.h"
#include "camera.h"
#include "mesh.h"
#include "render.h"
#include "render_internal.h"
#include "shaders.h"
#include "texture_internal.h"
#include "screen_internal.h"
#include "shaders_internal.h"
#include <stdlib.h>
#include <GLFW/glfw3.h>

static double last_update_start = 0;
static double dt = 0;
struct PR3DRenderTexture render_texture;
struct PR3DMesh *screen_render_quad;

void pr3d_run_game(
    char *title, void (*pr3d_init)(void),
    void (*pr3d_update)(double delta_time), void (*pr3d_render)(void),
    void (*pr3d_render_gui)(void)
)
{
    glfwInit();
    pr3d_create_window(title);

    pr3d_init_shader_pool();
    render_texture = pr3d_create_render_texture();
    screen_render_quad = pr3d_create_screen_quad();
    pr3d_init();

    struct PR3DScreen screen = *pr3d_screen();
    while (!glfwWindowShouldClose(screen.window))
    {
        vec2 render_res = {
            PR3D_RENDER_RESOLUTION[0], PR3D_RENDER_RESOLUTION[1]
        };

        dt = glfwGetTime() - last_update_start;
        last_update_start = glfwGetTime();

        pr3d_enable_render_texture(render_texture.fbo);
        glEnable(GL_DEPTH_TEST);
        pr3d_use_shader_3d();
        pr3d_set_shader_uniform_vec2(
            pr3d_current_shader(), PR3D_TILE_FACTOR_UNIFORM, (vec2){1.0f, 1.0f}
        );
        pr3d_set_shader_uniform_vec2(
            pr3d_current_shader(), PR3D_RENDER_RESOLUTION_UNIFORM, render_res
        );
        pr3d_set_camera_projection(
            pr3d_active_camera(), pr3d_active_camera()->fov,
            PR3D_CAMERA_PROJECTION_PERSPECTIVE
        );
        pr3d_update(dt);

        pr3d_render();

        glDisable(GL_DEPTH_TEST);
        pr3d_use_shader_2d();
        pr3d_set_shader_uniform_vec2(
            pr3d_current_shader(), PR3D_RENDER_RESOLUTION_UNIFORM, render_res
        );
        pr3d_set_camera_projection(
            pr3d_active_camera(), 0.0f, PR3D_CAMERA_PROJECTION_ORTHOGONAL
        );
        pr3d_render_gui();

        pr3d_render_render_texture(render_texture.texture, screen_render_quad);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();
    }

    pr3d_delete_mesh(screen_render_quad);
    free(screen_render_quad);

    pr3d_delete_shader_pool();
    pr3d_destroy_window();
}

double pr3d_delta_time(void) { return dt; }
