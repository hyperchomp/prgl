#include "game.h"
#include "screen_internal.h"
#include "shaders_internal.h"
#include <GLFW/glfw3.h>

static double last_update_start = 0;
static double dt = 0;

void pr3d_run_game(
    char *title, void (*pr3d_init)(void),
    void (*pr3d_update)(double delta_time), void (*pr3d_render)(void)
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

        glfwSwapBuffers(screen.window);
        glfwPollEvents();
    }

    pr3d_delete_shader_pool();
    pr3d_destroy_window();
}

double pr3d_delta_time(void) { return dt; }
