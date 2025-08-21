#ifndef PR3D_GAME_H
#define PR3D_GAME_H

/**
 * Runs the main loop of the game. This should be called first in any game.
 *
 * Callbacks are called in the same order as the arguments.
 *
 * @param[in] title Title of the game, this will be used as the window header
 * @param[in] pr3d_init Initialize callback function, runs before the main game
 * loop starts.
 * @param[in] pr3d_update The update callback function, must accept a delta time
 * @param[in] pr3d_render Render callback function
 * @param[in] pr3d_render_gui Render gui callback function
 */
void pr3d_run_game(
    char *title, void (*pr3d_init)(void),
    void (*pr3d_update)(double delta_time), void (*pr3d_render)(void),
    void (*pr3d_render_gui)(void)
);

/**
 * Gets the time elapsed in seconds from the start of the last update until the
 * start of the current update.
 *
 * @param[out] delta_time
 */
double pr3d_delta_time(void);

#endif
