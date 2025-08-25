#ifndef PRGL_SCREEN_INTERNAL_H
#define PRGL_SCREEN_INTERNAL_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

/**
 * Contains data needed for the window and monitors.
 */
struct PRGLScreen
{
    GLFWwindow *window;
    int desktop_width;
    int desktop_height;
    float aspect_ratio;
    bool vsync_enabled;
};

/**
 * Creates a new window with the given name and saves it to a PRGLScreen struct.
 *
 * The window needs to be freed by calling prgl_destroy_window().
 */
void prgl_create_window(char *name);

void prgl_destroy_window(void);

struct PRGLScreen *prgl_screen(void);

#endif
