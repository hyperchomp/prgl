#ifndef PR3D_SCREEN_INTERNAL_H
#define PR3D_SCREEN_INTERNAL_H

#include <GLFW/glfw3.h>

/**
 * Contains data needed for the window and monitors.
 */
struct PR3DScreen
{
    GLFWwindow *window;
    int desktop_width;
    int desktop_height;
};

/**
 * Creates a new window with the given name and saves it to a PR3DScreen struct.
 *
 * The window needs to be freed by calling pr3d_destroy_window().
 */
void pr3d_create_window(char *name);

void pr3d_destroy_window(void);

struct PR3DScreen *pr3d_screen_data(void);

#endif
