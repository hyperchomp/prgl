#include "glad.h"
#include "camera.h"
#include "mathx.h"
#include "screen_internal.h"
#include "shaders.h"
#include "render.h"
#include "cglm/types.h"
#include "cglm/cam.h"
#include "cglm/vec3.h"
#include <stddef.h>
#include <stdio.h>

static vec3 PRGL_WORLD_UP = {0.0f, 1.0f, 0.0f};
static struct PRGLCamera *prgl_active_camera_ref = NULL;

void prgl_init_camera(
    struct PRGLCamera *const cam, float fov_degrees, float move_speed,
    enum PRGLCameraProjectionType projection_type
)
{
    prgl_active_camera_ref = cam;

    glm_vec3_zero(cam->position);
    glm_vec3_zero(cam->up);
    glm_vec3_zero(cam->front);
    glm_vec3_zero(cam->right);

    cam->up[1] = 1.0f;     // Up is +y
    cam->front[2] = -1.0f; // Forward is -z

    // With angles 0 degrees is pointing straight to the right. For yaw we are
    // looking down the y axis, so in a graph view we are looking at horizontal
    // x and vertical z. Our forward axis is -z, so we need to rotate -90
    // degrees to be looking "forward".
    cam->yaw = -90.0f;

    cam->pitch = 0.0f;
    cam->move_speed = move_speed;
    cam->look_sensitivity = 0.1f;
    cam->projection_type = projection_type;

    prgl_set_camera_projection(cam, fov_degrees, projection_type);
}

void prgl_update_camera(struct PRGLCamera *const cam)
{
    // Calculate front vector, this is the z-axis
    vec3 front;
    const float yaw_rads = glm_rad(cam->yaw);
    const float pitch_rads = glm_rad(cam->pitch);
    const float cos_pitch = cosf(pitch_rads);
    front[0] = cosf(yaw_rads) * cos_pitch;
    front[1] = sinf(pitch_rads);
    front[2] = sinf(yaw_rads) * cos_pitch;
    glm_normalize(front);
    glm_vec3_copy(front, cam->front);

    // Right and Up vector as well
    glm_cross(cam->front, PRGL_WORLD_UP, cam->right); // This is the x-axis
    glm_normalize(cam->right);

    glm_cross(cam->right, cam->front, cam->up); // This is the y-axis
    glm_normalize(cam->up);

    // Now calculate the direction of the camera, and adjust the view matrix
    vec3 direction;
    glm_vec3_add(cam->position, cam->front, direction);
    glm_lookat(cam->position, direction, cam->up, cam->view);

    // The view matrix is the camera position, which we calculated above
    prgl_set_shader_uniform_mat4(prgl_current_shader(), "view", cam->view);
}

void prgl_move_camera_fly(
    struct PRGLCamera *const cam, enum PRGLCameraMoveDirection move_dir,
    double delta_time
)
{
    const float velocity = cam->move_speed * (float)delta_time;
    vec3 front_velocity;
    glm_vec3_scale(cam->front, velocity, front_velocity);
    vec3 right_velocity;
    glm_vec3_scale(cam->right, velocity, right_velocity);

    switch (move_dir)
    {
        case PRGL_CAMERA_MOVE_DIR_FORWARD:
            glm_vec3_add(cam->position, front_velocity, cam->position);
            break;
        case PRGL_CAMERA_MOVE_DIR_BACKWARD:
            glm_vec3_sub(cam->position, front_velocity, cam->position);
            break;
        case PRGL_CAMERA_MOVE_DIR_LEFT:
            glm_vec3_sub(cam->position, right_velocity, cam->position);
            break;
        case PRGL_CAMERA_MOVE_DIR_RIGHT:
            glm_vec3_add(cam->position, right_velocity, cam->position);
            break;
        default:
            fprintf(
                stderr,
                "prgl_move_camera: Unspecified camera move direction type\n"
            );
            exit(EXIT_FAILURE);
    }
}

void prgl_move_camera_look(struct PRGLCamera *const cam, float yaw, float pitch)
{
    yaw *= cam->look_sensitivity;
    pitch *= cam->look_sensitivity;

    cam->yaw += yaw;
    cam->pitch += pitch;

    // Clamp the camera pitch to prevent flipping
    cam->pitch = prgl_clampf(cam->pitch, -89.0f, 89.0f);
}

void prgl_set_camera_projection(
    struct PRGLCamera *const cam, float fov_degrees,
    enum PRGLCameraProjectionType projection_type
)
{
    struct PRGLScreen *const screen = prgl_screen();
    if (projection_type == PRGL_CAMERA_PROJECTION_PERSPECTIVE)
    {
        cam->projection_type = PRGL_CAMERA_PROJECTION_PERSPECTIVE;

        // Perspective needs fov in radians, aspect, and near/far clip
        cam->fov = fov_degrees;
        const float fov = glm_rad(fov_degrees);

        const float aspect_ratio = screen->aspect_ratio;

        glm_perspective(
            fov, aspect_ratio, 0.1f, 100.0f, cam->projection_perspective
        );
        prgl_set_shader_uniform_mat4(
            prgl_current_shader(), "projection", cam->projection_perspective
        );
    }
    else if (projection_type == PRGL_CAMERA_PROJECTION_ORTHOGONAL)
    {
        cam->projection_type = PRGL_CAMERA_PROJECTION_ORTHOGONAL;

        // Orthogonal is 2D so uses width/height, the clipping plane is the
        // OpenGL coordinate plane which goes from -1.0 to 1.0
        glm_ortho(
            0.0f, PRGL_RENDER_RESOLUTION[0], PRGL_RENDER_RESOLUTION[1], 0.0f,
            -1.0f, 1.0f, cam->projection_orthogonal
        );
        prgl_set_shader_uniform_mat4(
            prgl_current_shader(), "projection", cam->projection_orthogonal
        );
    }
}

struct PRGLCamera *prgl_active_camera(void) { return prgl_active_camera_ref; }
