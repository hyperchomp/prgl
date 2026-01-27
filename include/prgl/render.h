#ifndef PRGL_RENDER_H
#define PRGL_RENDER_H

#include "cglm/types.h"

struct PRGLGameObject;

extern const vec2 PRGL_RENDER_RESOLUTION;

/**
 * Clears the screen using the given color. Values are from 0-1.
 *
 * @param r
 * @param g
 * @param b
 * @param a
 */
void prgl_clear_screen(float r, float g, float b, float a);

/**
 * Draws a game object to the screen.
 *
 * @param[in] game_obj
 */
void prgl_draw_game_object_3d(struct PRGLGameObject *const game_obj);

/**
 * Draws a game object to the screen at a 2D position.
 * Rotation will be about the Z axis.
 *
 * @param[in] game_obj
 */
void prgl_draw_game_object_2d(struct PRGLGameObject *const game_obj);

#endif
