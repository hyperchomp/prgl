#ifndef PR3D_CAMERA_H
#define PR3D_CAMERA_H

#include <cglm/cglm.h>

/**
 * A camera for viewing the scene.
 * The struct values should be initialized and updated with the framework's
 * provided helper functions. Doing otherwise may cause undefined behavior.
 */
struct PR3DCamera
{
    mat4 position;   ///< The position of the camera stored as a view matrix.
    mat4 projection; ///< The camera's projection matrix for fov, clipping, etc.

    float fov; ///< Camera fov in degrees
};

/**
 * Initializes the view (position) and projection matrices for the camera. This
 * sets the camera's position to {0, 0, 0}.
 *
 * @param cam[in,out] An uninitialized PR3DCamera struct.
 * @param fov_degrees The vertical fov for the camera in degrees.
 */
void pr3d_init_camera(struct PR3DCamera *cam, float fov_degrees);

/**
 * Sends the view and projection matrices to the shader every frame.
 *
 * @param cam[in,out]
 */
void pr3d_update_camera(struct PR3DCamera *cam);

/**
 * Moves the camera to a new position.
 *
 * @param cam[in,out]
 * @param position
 */
void pr3d_move_camera(struct PR3DCamera *cam, vec3 position);

/**
 * Updates the camera's vertical fov.
 *
 * @param cam[in,out]
 * @param fov_degrees The vertical fov for the camera in degrees.
 */
void pr3d_set_camera_fov(struct PR3DCamera *cam, float fov_degrees);

#endif
