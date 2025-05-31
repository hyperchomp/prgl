#ifndef PR3D_WINDOW_H
#define PR3D_WINDOW_H

#include <glad/glad.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include "input.h"
#include "common_macros.h"

extern GLFWwindow *PR3D_WINDOW;

/**
 * Creates a new GLFW window with the given dimensions and name. Will not
 * allow the creation of more than one window.
 * Make sure to destroy it with destroy_window()
 */
GLFWwindow *pr3d_create_window(int width, int height, char *name);

void pr3d_destroy_window();

#endif
