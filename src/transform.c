#include "transform_internal.h"
#include "game_object.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"

void prgl_create_model_matrix(mat4 model, struct PRGLGameObject *const game_obj)
{
    glm_mat4_identity(model);
    glm_translate(model, game_obj->position);
    glm_quat_rotate(model, game_obj->orientation, model);
    glm_scale(model, game_obj->scale);
}

void prgl_create_normal_matrix(mat3 normal_matrix, mat4 model)
{
    mat4 normal_mat4;
    glm_mat4_copy(model, normal_mat4);
    glm_inv_tr(normal_mat4);
    glm_mat4_pick3(normal_mat4, normal_matrix);
}
