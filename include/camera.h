#ifndef PR3D_CAMERA_H
#define PR3D_CAMERA_H

#include "cglm/types.h"

/**
 * A camera for viewing the scene.
 * The struct values should be initialized and updated with the framework's
 * provided helper functions. Doing otherwise may cause undefined behavior.
 */
struct PR3DCamera
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
enum PR3DCameraMoveDirection
{
    PR3D_CAMERA_MOVE_DIR_FORWARD,
    PR3D_CAMERA_MOVE_DIR_BACKWARD,
    PR3D_CAMERA_MOVE_DIR_RIGHT,
    PR3D_CAMERA_MOVE_DIR_LEFT
};

enum PR3DCameraProjectionType
{
    PR3D_CAMERA_PROJECTION_PERSPECTIVE,
    PR3D_CAMERA_PROJECTION_ORTHOGONAL
};

/**
 * Initializes the view (position) and projection matrices for the camera. This
 * sets the camera's position to {0, 0, 0} and sets it as the active camera.
 *
 * @param cam[in,out] An uninitialized PR3DCamera struct.
 * @param fov_degrees The vertical fov for the camera in degrees. For orthogonal
 * cameras this can be set to zero since it's not needed.
 * @param move_speed Speed value to use when moving the camera.
 */
void pr3d_init_camera(
    struct PR3DCamera *cam, float fov_degrees, float move_speed,
    enum PR3DCameraProjectionType projection_type
);

/**
 * Sends the view matrix to the shader, should be called each frame for
 * perspective rendering. Note that orthogonal projection doesn't use the view
 * matrix, so this update is not needed when using the 2D shader.
 *
 * @param cam[in,out]
 */
void pr3d_update_camera(struct PR3DCamera *cam);

/**
 * Smoothly moves the camera in a given direction using its speed.
 * This uses fly camera movement, meaning forward and backward are relative to
 * the camera's pitch and yaw.
 *
 * @param cam[in,out]
 * @param move_dir
 * @param delta_time
 */
void pr3d_move_camera_fly(
    struct PR3DCamera *cam, enum PR3DCameraMoveDirection move_dir,
    double delta_time
);

/**
 * Smoothly moves the camera by the given amounts in yaw and pitch.
 *
 * @param cam[in,out]
 * @param yaw The amount to move the camera in yaw.
 * @param pitch The amount to move the camera in pitch.
 */
void pr3d_move_camera_look(struct PR3DCamera *cam, float yaw, float pitch);

/**
 * Updates the camera's vertical fov.
 *
 * @param cam[in,out]
 * @param fov_degrees The vertical fov for the camera in degrees.
 */
void pr3d_set_camera_projection(
    struct PR3DCamera *cam, float fov_degrees,
    enum PR3DCameraProjectionType projection_type
);

/**
 * Returns the currently active camera.
 */
struct PR3DCamera *pr3d_active_camera(void);

#endif
