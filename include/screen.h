#ifndef PR3D_SCREEN_H
#define PR3D_SCREEN_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

enum PR3DMouseCaptureMode
{
    PR3D_MOUSE_DISABLED,
    PR3D_MOUSE_CAPTURED
};

/**
 * Signals the game to stop execution after this game loop. All callbacks in the
 * current loop will finish and then the game window will be closed.
 */
void pr3d_close_game(void);

void pr3d_toggle_fullscreen(void);

bool pr3d_vsync(void);

void pr3d_set_vsync(bool enabled);

void pr3d_set_mouse_capture_mode(enum PR3DMouseCaptureMode mode);

#endif
