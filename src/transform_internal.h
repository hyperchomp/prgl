#ifndef PRGL_TRANSFORM_INTERNAL_H
#define PRGL_TRANSFORM_INTERNAL_H

#include "cglm/types.h"

/**
 * Creates a model matrix which can be passed to a shader transform uniform.
 *
 * @param[in,out] dest
 * @param[in] position
 * @param[in] rotation_axis
 * @param degrees
 * @param[in] scale
 */
void prgl_create_model_matrix(
    mat4 dest, vec3 position, vec3 rotation_axis, float degrees, vec3 scale
);

#endif
