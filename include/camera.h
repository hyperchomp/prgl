#ifndef PR3D_CAMERA_H
#define PR3D_CAMERA_H

#include <cglm/cglm.h>

/**
 * Set the position and fov for the camera.
 *
 * @param position
 * @param fov_degrees The vertical fov for the camera in degrees.
 */
void pr3d_set_camera(vec3 position, float fov_degrees);

#endif
