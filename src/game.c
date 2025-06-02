#include "game.h"
#include "window.h"
#include "window_internal.h"
#include <GLFW/glfw3.h>

static double last_update_start = 0;
static double dt = 0;

void pr3d_run_game(
    void (*pr3d_update)(double delta_time), void (*pr3d_draw)(void)
)
{
    while (!glfwWindowShouldClose(PR3D_WINDOW))
    {
        dt = glfwGetTime() - last_update_start;
        last_update_start = glfwGetTime();

        (*pr3d_update)(dt);
        (*pr3d_draw)();

        glfwPollEvents();
        glfwSwapBuffers(PR3D_WINDOW);
    }

    pr3d_destroy_window();
}

double pr3d_delta_time(void) { return dt; }
