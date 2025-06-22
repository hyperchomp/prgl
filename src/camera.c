#include "camera_internal.h"
#include "screen_internal.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <cglm/vec3.h>

static mat4 view;
static mat4 projection;

void pr3d_set_camera(vec3 position, float fov_degrees)
{
    // View is the position of the camera
    glm_mat4_identity(view);
    glm_translate(view, position);

    // Projection needs fov, aspect, and near/far clip
    float fov = glm_rad(fov_degrees);

    struct PR3DScreen *screen = pr3d_screen();
    float aspect_ratio =
        (float)screen->desktop_width / (float)screen->desktop_height;
    printf(
        "pr3d_init_camera: Setting aspect ratio from resolution %dx%d: %f\n",
        screen->desktop_width, screen->desktop_height, aspect_ratio
    );

    glm_perspective(fov, aspect_ratio, 0.1f, 100.0f, projection);
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_TEXTURE), "projection", projection
    );
}

void pr3d_init_camera(void) { pr3d_set_camera(GLM_VEC3_ZERO, 60.0f); }

void pr3d_update_camera(void)
{
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_TEXTURE), "view", view
    );
}
