#include "camera.h"
#include "screen_internal.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/vec3.h>

void pr3d_init_camera(struct PR3DCamera *cam, float fov_degrees)
{
    pr3d_move_camera(cam, GLM_VEC3_ZERO);
    pr3d_set_camera_fov(cam, fov_degrees);
}

void pr3d_update_camera(struct PR3DCamera *cam)
{
    // The view matrix is the camera position
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_TEXTURE), "view", cam->position
    );
}

void pr3d_move_camera(struct PR3DCamera *cam, vec3 position)
{
    glm_mat4_identity(cam->position);
    glm_translate(cam->position, position);
}

void pr3d_set_camera_fov(struct PR3DCamera *cam, float fov_degrees)
{
    // Projection needs fov in radians, aspect, and near/far clip
    cam->fov = fov_degrees;
    float fov = glm_rad(fov_degrees);

    struct PR3DScreen *screen = pr3d_screen();
    float aspect_ratio = screen->aspect_ratio;

    glm_perspective(fov, aspect_ratio, 0.1f, 100.0f, cam->projection);
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_TEXTURE), "projection", cam->projection
    );
}
