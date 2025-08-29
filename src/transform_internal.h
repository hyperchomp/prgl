#ifndef PRGL_TRANSFORM_INTERNAL_H
#define PRGL_TRANSFORM_INTERNAL_H

#include "cglm/types.h"

struct PRGLGameObject;

/**
 * Creates a model matrix which can be passed to a shader transform uniform.
 *
 * @param[in,out] dest
 * @param[in] game_obj The game object to create the model matrix from.
 */
void prgl_create_model_matrix(mat4 *dest, struct PRGLGameObject *game_obj);

#endif
