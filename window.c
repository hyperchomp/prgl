#include "window.h"

static void
framebuffer_size_callback(GLFWwindow *window, int width, int height);

GLFWwindow *PR3D_WINDOW = NULL;

GLFWwindow *pr3d_create_window(int width, int height, char *name)
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

    PR3D_WINDOW = glfwCreateWindow(width, height, name, NULL, NULL);
    if (PR3D_WINDOW == NULL)
    {
        fprintf(stderr, "new_window: Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(PR3D_WINDOW);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "new_window: Failed to initialize GLAD\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(PR3D_WINDOW, framebuffer_size_callback);
    return PR3D_WINDOW;
}

void destroy_window()
{
    glfwDestroyWindow(PR3D_WINDOW);
    glfwTerminate();
}

/**
 * Called by GLFW when resizing the window
 */
static void
framebuffer_size_callback(GLFWwindow *UNUSED(window), int width, int height)
{
    glViewport(0, 0, width, height);
}
