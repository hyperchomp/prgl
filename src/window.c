#include <glad/glad.h>
#include "window.h"
#include "window_internal.h"
#include "common_macros.h"
#include <stdio.h>
#include <stdlib.h>

static void
framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Base resolution to ensure pixel perfect scaling for most 16:9 resolutions
static int BASE_WIDTH = 320;
static int BASE_HEIGHT = 180;

GLFWwindow *PR3D_WINDOW = NULL;

GLFWwindow *pr3d_create_window(enum PR3DPixelScale res_scale_type, char *name)
{
    if (PR3D_WINDOW != NULL)
    {
        fprintf(
            stderr, "new_window: Failed to create GLFW window because a window "
                    "already exists\n"
        );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // PixelScale determines resolution, higher resolution = smaller pixels
    // so 2x should set to 320x180 and STANDARD should be doubled
    int width = BASE_WIDTH;
    int height = BASE_HEIGHT;
    if (res_scale_type == STANDARD)
    {
        width *= 2;
        height *= 2;
    }

    PR3D_WINDOW = glfwCreateWindow(width, height, name, NULL, NULL);
    if (PR3D_WINDOW == NULL)
    {
        fprintf(stderr, "new_window: Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(PR3D_WINDOW);
    glfwSetFramebufferSizeCallback(PR3D_WINDOW, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "new_window: Failed to initialize GLAD\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    return PR3D_WINDOW;
}

void pr3d_destroy_window(void)
{
    glfwDestroyWindow(PR3D_WINDOW);
    glfwTerminate();
}

void pr3d_close_game(void) { glfwSetWindowShouldClose(PR3D_WINDOW, true); }

/**
 * Called by GLFW when resizing the window
 */
static void
framebuffer_size_callback(GLFWwindow *UNUSED(window), int width, int height)
{
    glViewport(0, 0, width, height);
}
