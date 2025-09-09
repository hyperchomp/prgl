#ifndef PRGL_GAME_OBJECT_H
#define PRGL_GAME_OBJECT_H

#include "cglm/types.h"
#include "types.h"

struct CGLM_ALIGN_MAT PRGLGameObject
{
    versor orientation; ///< Quaternion rotation.
    vec3 position;
    vec3 scale;
    vec3 color;
    PRGLMeshHandle mesh;
};

/**
 * @brief Initializes game object values to defaults and assigns the given mesh.
 *
 * @param game_obj[out]
 * @param mesh[out]
 * @param position
 */
void prgl_init_game_object(
    struct PRGLGameObject *const game_obj, const PRGLMeshHandle mesh,
    vec3 position
);

/**
 * @brief Rotates a game object.
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
 * @brief Sets the orientation of a game object using an axis-angle rotation.
 *
 * @param game_obj[out]
 * @param axis The XYZ rotation axis.
 * @param angle_d The rotation angle in degrees.
 */
void prgl_set_game_object_axis_angle(
    struct PRGLGameObject *const game_obj, vec3 axis, float angle_d
);

/**
 * @brief Sets the RGB color of a game object.
 *
 * @param game_obj[out]
 * @param r
 * @param g
 * @param b
 */
void prgl_set_game_object_color(
    struct PRGLGameObject *const game_obj, float r, float g, float b
);

#endif
