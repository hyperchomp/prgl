#include "input.h"
#include "screen_internal.h"
#include <GLFW/glfw3.h>

bool pr3d_key_pressed(int key)
{
    return glfwGetKey(pr3d_screen()->window, key) == GLFW_PRESS;
}
