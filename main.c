#include "window.h"
#include "input.h"
#include <GLFW/glfw3.h>

int main(void)
{
    const int WINDOW_WIDTH = 320;
    const int WINDOW_HEIGHT = 180;

    pr3d_create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "PR3D");

    // TODO: Move this into a callback for drawing?
    while (!glfwWindowShouldClose(PR3D_WINDOW))
    {
        if (pr3d_key_pressed(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(PR3D_WINDOW, true);
        }
        glfwSwapBuffers(PR3D_WINDOW);
        glfwPollEvents();
    }

    destroy_window();
    return EXIT_SUCCESS;
}
