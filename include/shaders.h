#ifndef PRGL_SHADERS_H
#define PRGL_SHADERS_H

#include "cglm/types.h"
#include <stddef.h>
#include <stdbool.h>

#define PRGL_MAX_POINT_LIGHTS 32

extern const char *const PRGL_MODEL_UNIFORM;

extern const char *const PRGL_LIGHT_COLOR_UNIFORM;
extern const char *const PRGL_LIGHT_POSITION_UNIFORM;
extern const char *const PRGL_LIGHT_LINEAR_UNIFORM;
extern const char *const PRGL_LIGHT_QUADRATIC_UNIFORM;

extern const char *const PRGL_RENDER_RESOLUTION_UNIFORM;
extern const char *const PRGL_TILE_FACTOR_UNIFORM;

/**
 * Precompiled shader types. These can be used with prgl_shader() to get the ID
 * of a precompiled shader.
 *
 * The default is PRGL_SHADER_3D for 3D, and PRGL_SHADER_2D for 2D
 */
enum PRGLShader
{
    PRGL_SHADER_SCREEN,
    PRGL_SHADER_2D,
    PRGL_SHADER_3D,
    PRGL_SHADER_UNLIT,
    PRGL_SHADER_COUNT
};

/*
 * Gets the shader ID for the given shader type. This is for shaders that are
 * precompiled with prgl.
 *
 * For custom shaders the owning project should store and track its own IDs.
 *
 * @param type The type of precompiled shader.
 * @return The shader program ID for the shader type.
 */
unsigned int prgl_shader(enum PRGLShader type);

/**
 * Gets the shader ID for the current shader in use.
 *
 * @return The shader program ID for the current shader.
 */
unsigned int prgl_current_shader(void);

/**
 * Creates a shader from the given sources.
 *
 * @param[in] vertex_source The GLSL code for the vertex shader.
 * @param[in] frag_source The GLSL code for the fragment shader.
 * @param[in] geometry_source GLSL code for the geometry shader. Can be NULL.
 * @return The shader program ID.
 */
unsigned int prgl_create_shader(
    const char *const vertex_source, const char *const frag_source,
    const char *const geometry_source
);

/**
 * Activates a shader for use. Keep in mind there are default shaders activated
 * for the render and render_gui loops.
 *
 * @param shader The ID of the shader to use.
 */
void prgl_use_shader(unsigned int shader);

/**
 * Activates the 3D shader for use.
 */
void prgl_use_shader_3d(void);

/**
 * Activates the 2D shader for use.
 */
void prgl_use_shader_2d(void);

/**
 * Flags a shader for deletion.
 *
 * @param shader The ID of the shader to delete.
 */
void prgl_delete_shader(unsigned int shader);

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
void prgl_set_shader_uniform_4f(
    unsigned int shader, const char *const name, float a, float b, float c,
    float d
);

/**
 * Sets the value of shader uniform variable which takes a vec3.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param vec3
 */
void prgl_set_shader_uniform_vec3(
    unsigned int shader, const char *const name, vec3 vec
);

/**
 * Sets the value of shader uniform variable which takes a vec2.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param vec2
 */
void prgl_set_shader_uniform_vec2(
    unsigned int shader, const char *const name, vec2 vec
);

/**
 * Sets the value of shader uniform variable which takes 4 floats.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param matrix The cglm matrix to assign to the uniform.
 */
void prgl_set_shader_uniform_mat4(
    unsigned int shader, const char *const name, mat4 matrix
);

/**
 * Sets the value of shader uniform variable which takes a float.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_float(
    unsigned int shader, const char *const name, float value
);

/**
 * Sets the value of shader uniform variable which takes an int.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_int(
    unsigned int shader, const char *const name, int value
);

/**
 * Sets the value of shader uniform variable which takes a bool.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_bool(
    unsigned int shader, const char *const name, bool value
);

#endif
