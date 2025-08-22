#include "camera.h"
#include "glad.h"
#include "game.h"
#include "shaders.h"
#include "screen_internal.h"
#include "shaders_internal.h"
#include <GLFW/glfw3.h>

static double last_update_start = 0;
static double dt = 0;

// static struct PR3DCamera gui_camera;

void pr3d_run_game(
    char *title, void (*pr3d_init)(void),
    void (*pr3d_update)(double delta_time), void (*pr3d_render)(void),
    void (*pr3d_render_gui)(void)
)
{
    glfwInit();
    pr3d_create_window(title);

    pr3d_init_shader_pool();
    pr3d_init();

    struct PR3DScreen screen = *pr3d_screen();
    while (!glfwWindowShouldClose(screen.window))
    {
        dt = glfwGetTime() - last_update_start;
        last_update_start = glfwGetTime();

        pr3d_update(dt);

        pr3d_render();

        glDisable(GL_DEPTH_TEST);
        pr3d_use_shader_2d();
        pr3d_set_camera_projection(
            pr3d_active_camera(), 0.0f, PR3D_CAMERA_PROJECTION_ORTHOGONAL
        );
        pr3d_render_gui();
        pr3d_set_camera_projection(
            pr3d_active_camera(), pr3d_active_camera()->fov,
            PR3D_CAMERA_PROJECTION_PERSPECTIVE
        );
        pr3d_use_shader_3d();
        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();
    }

    pr3d_delete_shader_pool();
    pr3d_destroy_window();
}

double pr3d_delta_time(void) { return dt; }
