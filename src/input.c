#include "input.h"
#include "screen_internal.h"
#include <GLFW/glfw3.h>

bool prgl_key_pressed(int key)
{
    return glfwGetKey(prgl_screen()->window, key) == GLFW_PRESS;
}

void prgl_mouse_position(double *x_pos, double *y_pos)
{
    glfwGetCursorPos(prgl_screen()->window, x_pos, y_pos);
}
