#include "game_object.h"
#include "cglm/quat.h"
#include "cglm/vec3.h"
#include "types.h"

void prgl_init_game_object(
    struct PRGLGameObject *const game_obj, const PRGLMeshHandle mesh,
    vec3 position
)
{
    glm_quat_identity(game_obj->orientation);
    glm_vec3_copy(position, game_obj->position);
    glm_vec3_one(game_obj->scale);
    glm_vec3_one(game_obj->color);
    game_obj->mesh = mesh;
}

void prgl_rotate_game_object(
    struct PRGLGameObject *const game_obj, float yaw_d, float pitch_d,
    float roll_d
)
{
    versor yaw_quat;
    versor pitch_quat;
    versor roll_quat;

    // Create rotation axes from yaw, pitch, and roll
    glm_quatv(yaw_quat, glm_rad(yaw_d), (vec3){0.0f, 1.0f, 0.0f});
    glm_quatv(pitch_quat, glm_rad(pitch_d), (vec3){1.0f, 0.0f, 0.0f});
    glm_quatv(roll_quat, glm_rad(roll_d), (vec3){0.0f, 0.0f, 1.0f});

    // Multiply with game object orientation in order yaw->pitch->roll
    glm_quat_mul(roll_quat, game_obj->orientation, game_obj->orientation);
    glm_quat_mul(pitch_quat, game_obj->orientation, game_obj->orientation);
    glm_quat_mul(yaw_quat, game_obj->orientation, game_obj->orientation);
}

void prgl_set_game_object_axis_angle(
    struct PRGLGameObject *const game_obj, vec3 axis, float angle_d
)
{
    glm_quatv(game_obj->orientation, glm_rad(angle_d), axis);
}

void prgl_set_game_object_color(
    struct PRGLGameObject *const game_obj, float r, float g, float b
)
{
    game_obj->color[0] = r;
    game_obj->color[1] = g;
    game_obj->color[2] = b;
}
