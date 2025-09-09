#include "glad.h"
#include "game.h"
#include "mesh.h"
#include "mesh_internal.h"
#include "render_internal.h"
#include "shaders.h"
#include "texture_internal.h"
#include "screen_internal.h"
#include "shaders_internal.h"
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
    prgl_use_shader(prgl_shader(PRGL_SHADER_TYPE_3D));
    render_texture = prgl_create_render_texture();
    screen_render_quad = prgl_create_screen_quad(render_texture.texture);
    prgl_init();

    struct PRGLScreen screen = *prgl_screen();
    while (!glfwWindowShouldClose(screen.window))
    {
        dt = glfwGetTime() - last_update_start;
        last_update_start = glfwGetTime();

        prgl_enable_render_texture(render_texture.fbo);
        glEnable(GL_DEPTH_TEST);
        prgl_use_shader_3d();
        prgl_update();

        prgl_draw_3d();

        glDisable(GL_DEPTH_TEST);
        prgl_use_shader_2d();
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
