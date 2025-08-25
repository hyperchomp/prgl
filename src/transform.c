#include "transform_internal.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"

void pr3d_create_model_matrix(
    mat4 dest, vec3 position, vec3 rotation_axis, float degrees, vec3 scale
)
{
    glm_mat4_identity(dest);
    glm_translate(dest, position);
    glm_rotate(dest, glm_rad(degrees), rotation_axis);
    glm_scale(dest, scale);
}
