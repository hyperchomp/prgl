#ifndef PRGL_RENDER_H
#define PRGL_RENDER_H

#include "cglm/types.h"

struct PRGLGameObject;

extern const vec2 PRGL_RENDER_RESOLUTION;

/**
 * A light which can be positioned in the world and emits a color.
 */
struct PRGLPointLight
{
    vec3 position;
    vec3 lightColor;
};

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
 * Renders a game object to the screen.
 *
 * @param[in] game_obj
 */
void prgl_render_game_object_3d(struct PRGLGameObject *const game_obj);

/**
 * Renders a game object to the screen at a 2D position.
 * Rotation will be about the Z axis.
 *
 * @param[in] game_obj
 */
void prgl_render_game_object_2d(struct PRGLGameObject *const game_obj);

/**
 * Passes lighting data from lights to the active shader.
 *
 * @param struct[in] PRGLPointLight The point lights for the scene.
 */
void prgl_update_lighting(
    struct PRGLPointLight *const point_lights, int num_lights
);

#endif
