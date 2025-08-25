#ifndef PRGL_SHADERS_INTERNAL_H
#define PRGL_SHADERS_INTERNAL_H

/**
 * Precompiles built in shaders and adds them to the shader pool.
 */
void prgl_init_shader_pool(void);

/**
 * Deletes built in shaders from the shader pool.
 */
void prgl_delete_shader_pool(void);

#endif
