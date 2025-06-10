#ifndef PR3D_SHADERS_H
#define PR3D_SHADERS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Precompiled shader types. These can be used with pr3d_shader() to get the ID
 * of a precompiled shader.
 *
 * The default is PR3D_SHADER_SOLID_COLOR and is set after shaders are
 * initialized before the game loop starts.
 */
enum PR3DShader
{
    PR3D_SHADER_SOLID_COLOR,
    PR3D_SHADER_VERTEX_COLOR,
    PR3D_SHADER_TEXTURE,
    PR3D_SHADER_COUNT
};

/*
 * Gets the shader ID for the given shader type. This is for shaders that are
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
unsigned int pr3d_create_shader(
    const char *const vertex_source, const char *const frag_source
);

/**
 * Activates a shader for use. It will be used until another shader is
 * activated.
 * You may want to call pr3d_use_default_shader after you're done rendering with
 * this unless you intend to replace the default.
 *
 * @param shader The ID of the shader to use.
 */
void pr3d_use_shader(unsigned int shader);

/**
 * Re-activates the default shader for use.
 */
void pr3d_use_default_shader(void);

/**
 * Flags a shader for deletion.
 *
 * @param shader The ID of the shader to delete.
 */
void pr3d_delete_shader(unsigned int shader);

/**
 * Sets the value of shader uniform variable which takes 4 floats.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param a
 * @param b
 * @param c
 * @param d
 */
void pr3d_set_shader_uniform_4f(
    unsigned int shader, char *name, float a, float b, float c, float d
);

/**
 * Sets the value of shader uniform variable which takes a float.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void pr3d_set_shader_uniform_float(
    unsigned int shader, char *name, float value
);

/**
 * Sets the value of shader uniform variable which takes an int.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void pr3d_set_shader_uniform_int(unsigned int shader, char *name, int value);

/**
 * Sets the value of shader uniform variable which takes a bool.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void pr3d_set_shader_uniform_bool(unsigned int shader, char *name, bool value);

#endif
