#include "input.h"
#include "screen_internal.h"
#include <GLFW/glfw3.h>

bool pr3d_key_pressed(int key)
{
    return glfwGetKey(pr3d_screen()->window, key) == GLFW_PRESS;
}

void pr3d_mouse_position(double *x_pos, double *y_pos)
{
    glfwGetCursorPos(pr3d_screen()->window, x_pos, y_pos);
}
