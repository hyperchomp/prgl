#ifndef PRGL_GAME_H
#define PRGL_GAME_H

/**
 * @brief Runs the core loop of the game.
 *
 * Handles the core game loop, and calls the given callback functions at the
 * appropriate times. This should be the first function called in your game's
 * main function to ensure prgl is initialized before using it.
 *
 * `prgl_init` is called one time before the core game loop starts.
 * All other callbacks are called once per frame in the following order:
 * `prgl_update` -> `prgl_draw_3d` -> `prgl_draw_2d -> `prgl_cleanup`
 *
 * @param[in] title Title of the game, this will be used as the window header
 * @param[in] prgl_init Initialize callback, runs one time before the
 * game loop starts.
 * @param[in] prgl_update Update callback.
 * @param[in] prgl_draw_3d Draws the 3D scene.
 * @param[in] prgl_draw_2d Draws a 2D scene on top of the 3D scene.
 * @param[in] prgl_cleanup Cleanup callback for handling memory at end of frame.
 */
void prgl_run_game(
    const char *const title, void (*prgl_init)(void), void (*prgl_update)(void),
    void (*prgl_draw_3d)(void), void (*prgl_draw_2d)(void),
    void (*prgl_cleanup)(void)
);

/**
 * Gets the time elapsed in seconds from the start of the last update until the
 * start of the current update.
 *
 * @return The delta time of the previous frame.
 */
double prgl_delta_time(void);

#endif
