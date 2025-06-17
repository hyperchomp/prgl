#include "camera_internal.h"
#include "screen_internal.h"
#include "shaders.h"
#include <cglm/cglm.h>

static mat4 view;
static mat4 projection;

void pr3d_init_camera(void)
{
    // For the view back up from the origin
    glm_mat4_identity(view);
    vec3 trans_z = {0.0f, 0.0f, -3.0f};
    glm_translate(view, trans_z);

    // Projection needs fov, aspect, and near/far clip
    float fov = glm_rad(45.0f);

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

void pr3d_update_camera(void)
{
    pr3d_set_shader_uniform_mat4(
        pr3d_shader(PR3D_SHADER_TEXTURE), "view", view
    );
}
