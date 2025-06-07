#ifndef PR3D_SHADERS_INTERNAL_H
#define PR3D_SHADERS_INTERNAL_H

/**
 * Precompiles built in shaders and adds them to the shader pool.
 */
void pr3d_init_shader_pool(void);

/**
 * Deletes built in shaders from the shader pool.
 */
void pr3d_delete_shader_pool(void);

#endif
