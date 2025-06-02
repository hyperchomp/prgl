#ifndef PR3D_GAME_H
#define PR3D_GAME_H

/**
 * Runs the main loop of the game.
 *
 * @param[in] pr3d_update The update callback function, must accept a delta time
 * @param[in] pr3d_draw Draw callback function used for rendering
 */
void pr3d_run_game(
    void (*pr3d_update)(double delta_time), void (*pr3d_draw)(void)
);

/**
 * Gets the time elapsed in seconds from the start of the last update until the
 * start of the current update.
 *
 * @param[out] delta_time
 */
double pr3d_delta_time(void);

#endif
