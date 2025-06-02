#include "input.h"
#include "window.h"
#include <GLFW/glfw3.h>

bool pr3d_key_pressed(int key)
{
    return glfwGetKey(PR3D_WINDOW, key) == GLFW_PRESS;
}
