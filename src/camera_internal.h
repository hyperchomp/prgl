#ifndef PR3D_CAMERA_INTERNAL_H
#define PR3D_CAMERA_INTERNAL_H

/**
 * Initializes the view and projection matrices for the camera.
 */
void pr3d_init_camera(void);

/**
 * Sends the view and projection matrices to the shader every frame.
 */
void pr3d_update_camera(void);

#endif
