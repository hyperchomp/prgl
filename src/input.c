#include "input.h"

bool pr3d_key_pressed(int key)
{
    return glfwGetKey(PR3D_WINDOW, key) == GLFW_PRESS;
}
