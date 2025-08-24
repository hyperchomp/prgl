#include "glad.h"
#include "screen.h"
#include "screen_internal.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct PR3DScreen *pr3d_screen(void);

static struct PR3DScreen pr3d_screen_data;

void pr3d_create_window(char *name)
{
    if (pr3d_screen_data.window)
    {
        fprintf(
            stderr, "new_window: Failed to create GLFW window because a window "
                    "already exists\n"
        );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create borderless fullscreen window, this is done by getting the current
    // video mode and using that to create the window
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    int desktop_height = mode->height;
    int desktop_width = mode->width;

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow *window =
        glfwCreateWindow(mode->width, mode->height, name, monitor, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "new_window: Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "new_window: Failed to initialize GLAD\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    pr3d_screen_data = (struct PR3DScreen
    ){.window = window,
      .desktop_width = desktop_width,
      .desktop_height = desktop_height,
      .aspect_ratio = (float)desktop_width / (float)desktop_height};

    pr3d_set_vsync(true);
}

void pr3d_destroy_window(void)
{
    glfwDestroyWindow(pr3d_screen_data.window);
    glfwTerminate();
}

void pr3d_close_game(void)
{
    glfwSetWindowShouldClose(pr3d_screen_data.window, true);
}

void pr3d_toggle_fullscreen(void)
{
    // If monitor is NULL we are in fullscreen, and if glfwSetWindowMonitor
    // receives NULL for monitor it will change to window mode
    GLFWwindow *window = pr3d_screen_data.window;
    GLFWmonitor *monitor = glfwGetWindowMonitor(window);
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(
        window, monitor, 0, 0, pr3d_screen_data.desktop_width,
        pr3d_screen_data.desktop_height, mode->refreshRate
    );
}

struct PR3DScreen *pr3d_screen(void) { return &pr3d_screen_data; }

void pr3d_set_vsync(bool enabled)
{
    glfwSwapInterval(enabled);
    pr3d_screen_data.vsync_enabled = enabled;
}

bool pr3d_vsync(void) { return pr3d_screen_data.vsync_enabled; }

void pr3d_set_mouse_capture_mode(enum PR3DMouseCaptureMode mode)
{
    switch (mode)
    {
        case PR3D_MOUSE_DISABLED:
            glfwSetInputMode(
                pr3d_screen_data.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED
            );
            break;
        case PR3D_MOUSE_CAPTURED:
            glfwSetInputMode(
                pr3d_screen_data.window, GLFW_CURSOR, GLFW_CURSOR_CAPTURED
            );
            break;
        default:
            fprintf(
                stderr, "pr3d_set_mouse_capture_mode: Unspecified mouse "
                        "capture mode type.\n"
            );
    }
}
