#ifndef PRGL_GAME_H
#define PRGL_GAME_H

/**
 * Runs the main loop of the game. This should be called first in any game.
 *
 * Callbacks are called in the same order as the arguments. All callbacks except
 * init are called every game loop, init is called once before the game loop
 * starts the first time.
 *
 * @param[in] title Title of the game, this will be used as the window header
 * @param[in] prgl_init Initialize callback function, runs before the main game
 * loop starts.
 * @param[in] prgl_update The update callback function, must accept a delta time
 * @param[in] prgl_render Render callback function
 * @param[in] prgl_render_gui Render GUI callback function
 * @param[in] prgl_cleanup Cleanup callback for handling memory, etc.
 */
void prgl_run_game(
    const char *const title, void (*prgl_init)(void),
    void (*prgl_update)(double delta_time), void (*prgl_render)(void),
    void (*prgl_render_gui)(void), void (*prgl_cleanup)(void)
);

/**
 * Gets the time elapsed in seconds from the start of the last update until the
 * start of the current update.
 *
 * @param[out] delta_time
 */
double prgl_delta_time(void);

#endif
