#include "transform_internal.h"
#include "game_object.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/quat.h"

void prgl_create_model_matrix(mat4 *model, struct PRGLGameObject *game_obj)
{
    glm_mat4_identity(*model);
    glm_translate(*model, game_obj->position);
    glm_quat_rotate(*model, game_obj->orientation, *model);
    glm_scale(*model, game_obj->scale);
}
