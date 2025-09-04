#ifndef PRGL_CAMERA_H
#define PRGL_CAMERA_H

#include "cglm/types.h"

/**
 * A camera for viewing the scene.
 * The struct values should be initialized and updated with the framework's
 * provided helper functions. Doing otherwise may cause undefined behavior.
 */
struct CGLM_ALIGN_MAT PRGLCamera
{
    mat4 view; ///< The position of the camera stored as a view matrix.
    mat4 projection_perspective; ///< Projection matrix for fov, clipping, etc.
    mat4 projection_orthogonal;  ///< Projection matrix for 2D/Ortho

    vec3 position;
    vec3 front;
    vec3 right;
    vec3 up;

    float yaw;
    float pitch;
    float fov; ///< Camera fov in degrees
    float move_speed;
    float look_sensitivity;
};

/**
 * Camera movement directions to obfuscate movement from input.
 */
enum PRGLCameraMoveDirection
{
    PRGL_CAMERA_MOVE_DIR_FORWARD,
    PRGL_CAMERA_MOVE_DIR_BACKWARD,
    PRGL_CAMERA_MOVE_DIR_RIGHT,
    PRGL_CAMERA_MOVE_DIR_LEFT
};

enum PRGLCameraProjectionType
{
    PRGL_CAMERA_PROJECTION_PERSPECTIVE,
    PRGL_CAMERA_PROJECTION_ORTHOGONAL
};

/**
 * Initializes the view (position) and projection matrices for the camera. This
 * sets the camera's position to {0, 0, 0} and sets it as the active camera.
 *
 * @param cam[in,out] An uninitialized PRGLCamera struct.
 * @param fov_degrees The vertical fov for the camera in degrees. For orthogonal
 * cameras this can be set to zero since it's not needed.
 * @param move_speed Speed value to use when moving the camera.
 */
void prgl_init_camera(
    struct PRGLCamera *const cam, float fov_degrees, float move_speed,
    enum PRGLCameraProjectionType projection_type
);

/**
 * Sends the view matrix to the shader, should be called each frame for
 * perspective rendering. Note that orthogonal projection doesn't use the view
 * matrix, so this update is not needed when using the 2D shader.
 *
 * @param cam[in,out]
 */
void prgl_update_camera(struct PRGLCamera *const cam);

/**
 * Smoothly moves the camera in a given direction using its speed.
 * This uses fly camera movement, meaning forward and backward are relative to
 * the camera's pitch and yaw.
 *
 * @param cam[in,out]
 * @param move_dir
 * @param delta_time
 */
void prgl_move_camera_fly(
    struct PRGLCamera *const cam, enum PRGLCameraMoveDirection move_dir,
    double delta_time
);

/**
 * Smoothly moves the camera by the given amounts in yaw and pitch.
 *
 * @param cam[in,out]
 * @param yaw The amount to move the camera in yaw.
 * @param pitch The amount to move the camera in pitch.
 */
void prgl_move_camera_look(
    struct PRGLCamera *const cam, float yaw, float pitch
);

/**
 * Updates the camera's vertical fov.
 *
 * @param cam[in,out]
 * @param fov_degrees The vertical fov for the camera in degrees.
 */
void prgl_set_camera_projection(
    struct PRGLCamera *const cam, float fov_degrees,
    enum PRGLCameraProjectionType projection_type
);

/**
 * Returns the currently active camera.
 */
struct PRGLCamera *prgl_active_camera(void);

#endif
