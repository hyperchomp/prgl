#ifndef PRGL_TRANSFORM_INTERNAL_H
#define PRGL_TRANSFORM_INTERNAL_H

#include "cglm/types.h"

struct PRGLGameObject;

/**
 * Creates a model matrix which can be passed to a shader transform uniform.
 *
 * @param[out] model
 * @param[in] game_obj The game object to create the model matrix from.
 */
void prgl_create_model_matrix(
    mat4 model, struct PRGLGameObject *const game_obj
);

/**
 * Creates a normal matrix from a given model matrix.
 * Calculating this outside the shader on CPU is better for performance as the
 * calculation can be taxing to do per vertex on the GPU.
 *
 * @param[out] normal_matrix
 * @param[in] model
 */
void prgl_create_normal_matrix(mat3 normal_matrix, mat4 model);

#endif
