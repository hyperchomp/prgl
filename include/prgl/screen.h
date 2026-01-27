#ifndef PRGL_SCREEN_H
#define PRGL_SCREEN_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

enum PRGLMouseCaptureMode
{
    PRGL_MOUSE_DISABLED,
    PRGL_MOUSE_CAPTURED
};

/**
 * Signals the game to stop execution after this game loop. All callbacks in the
 * current loop will finish and then the game window will be closed.
 */
void prgl_close_game(void);

void prgl_toggle_fullscreen(void);

bool prgl_vsync(void);

void prgl_set_vsync(bool enabled);

void prgl_set_mouse_capture_mode(enum PRGLMouseCaptureMode mode);

#endif
