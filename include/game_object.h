#ifndef PRGL_GAME_OBJECT_H
#define PRGL_GAME_OBJECT_H

#include "cglm/types.h"

struct PRGLMesh;

struct PRGLGameObject
{
    versor orientation; ///< Quaternion rotation.
    vec3 position;
    vec3 scale;
    struct PRGLMesh *mesh;
};

/**
 * Initializes the game object values to defaults and assigns the given mesh.
 *
 * @param game_obj[out]
 * @param mesh[out]
 * @param position
 */
void prgl_init_game_object(
    struct PRGLGameObject *const game_obj, struct PRGLMesh *const mesh,
    vec3 position
);

/**
 * Rotates a game object.
 *
 * @param game_obj[in,out]
 * @param yaw_d Yaw in degrees.
 * @param pitch_d Pitch in degrees.
 * @param roll_d Roll in degrees.
 */
void prgl_rotate_game_object(
    struct PRGLGameObject *const game_obj, float yaw_d, float pitch_d,
    float roll_d
);

/**
 * Directly sets the orientation of a game object using an axis-angle rotation.
 *
 * @param game_obj[out]
 * @param axis The XYZ rotation axis.
 * @param angle_d The rotation angle in degrees.
 */
void prgl_set_orientation_axis_angle(
    struct PRGLGameObject *const game_obj, vec3 axis, float angle_d
);

#endif
