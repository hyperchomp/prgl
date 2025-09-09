/**
 * @file shaders.h
 * @brief Functions for creation and usage of shader programs.
 *
 * Shader functions are exposed for advanced users. If you create a custom
 * shader to override or enhance rendering, ensure your shader is compatible
 * with the uniforms and vertex attributes set by prgl functions.
 *
 * Note that most of the uniforms are set automatically during the game loop. So
 * if you want to override uniforms permanently or on a portion of each loop
 * you'll need to set those uniforms on every loop as well.
 *
 * Values shown in the shader contract are copied from the shader code for
 * clarity. If you would like more information on how these are used refer to
 * the prgl source code on GitHub: github.com/hyperchomp/prgl/
 *
 * For 2D shaders the uniforms are the same except with the absence of the view
 * and normalMatrix. Layout is the same as well except there is no aNormal, note
 * that EVERYTHING else is the same including the layout location numbers.
 *
 * == Shader Contract ==
 *
 * Uniforms:
 *
 * VERTEX SHADER UNIFORMS:
 * - uniform mat4 model;
 * - uniform mat4 view;
 * - uniform mat4 projection;
 * - uniform mat3 normalMatrix;
 * - uniform vec2 renderResolution;
 *
 * FRAGMENT SHADER UNIFORMS:
 * - uniform bool useTexture = true;
 * - uniform vec2 tileFactor = vec2(1.0, 1.0);
 * - uniform vec3 fillColor = vec3(1.0, 1.0, 1.0);
 * - uniform float alpha;
 * - uniform sampler2D imageTexture;
 *
 * VERTEX ATTRIBUTES LAYOUT:
 * - layout (location = 0) in vec3 aPos;
 * - layout (location = 1) in vec3 aNormal;
 * - layout (location = 2) in vec2 aTexCoord;
 */

#ifndef PRGL_SHADERS_H
#define PRGL_SHADERS_H

#include <stddef.h>
#include <stdbool.h>

#include "types.h"
#include "cglm/types.h"

#define PRGL_MAX_POINT_LIGHTS 32

extern const char *const PRGL_MODEL_UNIFORM;
extern const char *const PRGL_NORMAL_MATRIX_UNIFORM;

extern const char *const PRGL_NUM_POINT_LIGHTS_UNIFORM;
extern const char *const PRGL_LIGHT_COLOR_UNIFORM;
extern const char *const PRGL_LIGHT_POSITION_UNIFORM;
extern const char *const PRGL_LIGHT_LINEAR_UNIFORM;
extern const char *const PRGL_LIGHT_QUADRATIC_UNIFORM;

extern const char *const PRGL_RENDER_RESOLUTION_UNIFORM;
extern const char *const PRGL_TILE_FACTOR_UNIFORM;
extern const char *const PRGL_FILL_COLOR_UNIFORM;
extern const char *const PRGL_USE_TEXTURE_UNIFORM;

/**
 * Precompiled shader types. These can be used with prgl_shader() to get the ID
 * of a precompiled shader.
 *
 * The default is PRGL_SHADER_TYPE_3D for 3D, and PRGL_SHADER_TYPE_2D for 2D
 */
enum PRGLShaderType
{
    PRGL_SHADER_TYPE_SCREEN,
    PRGL_SHADER_TYPE_2D,
    PRGL_SHADER_TYPE_3D,
    PRGL_SHADER_TYPE_UNLIT,
    PRGL_SHADER_TYPE_COUNT
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
PRGLShader prgl_shader(enum PRGLShaderType type);

/**
 * Gets the shader ID for the current shader in use.
 *
 * @return The shader program ID for the current shader.
 */
PRGLShader prgl_current_shader(void);

/**
 * Creates a shader from the given sources.
 *
 * @param[in] vertex_source The GLSL code for the vertex shader.
 * @param[in] frag_source The GLSL code for the fragment shader.
 * @param[in] geometry_source GLSL code for the geometry shader. Can be NULL.
 * @return The shader program ID.
 */
PRGLShader prgl_create_shader(
    const char *const vertex_source[], int num_vertex_sources,
    const char *const frag_source[], int num_frag_sources,
    const char *const geometry_source[], int num_geometry_sources
);

/**
 * Activates a shader for use. Keep in mind there are default shaders activated
 * for the render and render_gui loops.
 *
 * @param shader The ID of the shader to use.
 */
void prgl_use_shader(PRGLShader shader);

/**
 * @brief Activates the 3D shader for use.
 *
 * Activates the 3D shader and sets necessary uniforms needed for use.
 */
void prgl_use_shader_3d(void);

/**
 * @brief Activates the unlit 3D shader for use.
 *
 * Activates the unlit 3D shader and sets necessary uniforms needed for use.
 */
void prgl_use_shader_unlit(void);

/**
 * Activates the 2D shader for use.
 *
 * Activates the 2D shader and sets necessary uniforms needed for use.
 */
void prgl_use_shader_2d(void);

/**
 * Flags a shader for deletion.
 *
 * @param shader The ID of the shader to delete.
 */
void prgl_delete_shader(PRGLShader shader);

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
    PRGLShader shader, const char *const name, float a, float b, float c,
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
    PRGLShader shader, const char *const name, vec3 vec
);

/**
 * Sets the value of shader uniform variable which takes a vec2.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param vec2
 */
void prgl_set_shader_uniform_vec2(
    PRGLShader shader, const char *const name, vec2 vec
);

/**
 * Sets the value of shader uniform variable which takes 4x4 floats.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param matrix The cglm matrix to assign to the uniform.
 */
void prgl_set_shader_uniform_mat4(
    PRGLShader shader, const char *const name, mat4 matrix
);

/**
 * Sets the value of shader uniform variable which takes 3x3 floats.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param matrix The cglm matrix to assign to the uniform.
 */
void prgl_set_shader_uniform_mat3(
    PRGLShader shader, const char *const name, mat3 matrix
);

/**
 * Sets the value of shader uniform variable which takes a float.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_float(
    PRGLShader shader, const char *const name, float value
);

/**
 * Sets the value of shader uniform variable which takes an int.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_int(
    PRGLShader shader, const char *const name, int value
);

/**
 * Sets the value of shader uniform variable which takes a bool.
 *
 * @param shader The shader ID to set the uniform on.
 * @param name[in] The name of the uniform to set.
 * @param value
 */
void prgl_set_shader_uniform_bool(
    PRGLShader shader, const char *const name, bool value
);

/**
 * @brief Sets values for uniforms which are needed for default shaders.
 *
 * Should be called after switching to a shader if not using one of the built-in
 * shader functions like prgl_use_shader_3d(), as those functions will also call
 * this one.
 *
 * @param is_3d
 */
void prgl_set_default_shared_uniforms(bool is_3d);

#endif
