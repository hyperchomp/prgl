#ifndef PR3D_SCREEN_H
#define PR3D_SCREEN_H

#include <GLFW/glfw3.h>

/**
 * Signals the game to stop execution after this game loop. All callbacks in the
 * current loop will finish and then the game window will be closed.
 */
void pr3d_close_game(void);

void pr3d_toggle_fullscreen(void);

bool pr3d_vsync(void);

void pr3d_set_vsync(bool enabled);

#endif
