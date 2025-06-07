#ifndef PR3D_SHADERS_H
#define PR3D_SHADERS_H

#include <stddef.h>

enum PR3DShader
{
    PR3D_SHADER_BASIC,
    PR3D_SHADER_COUNT
};

/**
 * Gets the shader for the given shader type. This is for shaders that are
 * precompiled with pr3d.
 *
 * For custom shaders the owning project should store and track its own IDs.
 *
 * @param type The type of precompiled shader.
 * @return The shader program ID for the shader type.
 */
unsigned int pr3d_shader(enum PR3DShader type);

/**
 * Creates a shader from the given sources.
 *
 * @param[in] vertex_source The GLSL code for the vertex shader.
 * @param[in] frag_source The GLSL code for the fragment shader.
 * @return The shader program ID.
 */
unsigned int
pr3d_create_shader(const char *vertex_source, const char *frag_source);

/**
 * Activates a shader for use. It will be used until another shader is
 * activated.
 *
 * @param shader The ID of the shader to use.
 */
void pr3d_use_shader(unsigned int shader);

/**
 * Flags a shader for deletion.
 *
 * @param shader The ID of the shader to delete.
 */
void pr3d_delete_shader(unsigned int shader);

#endif
