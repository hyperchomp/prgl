#include "glad.h"
#include "shaders.h"
#include "shaders_internal.h"
#include "shaders_init_internal.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

const char *const PRGL_MODEL_UNIFORM = "model";
const char *const PRGL_NORMAL_MATRIX_UNIFORM = "normalMatrix";
const char *const PRGL_NUM_POINT_LIGHTS_UNIFORM = "numPointLights";
const char *const PRGL_LIGHT_COLOR_UNIFORM = "color";
const char *const PRGL_LIGHT_POSITION_UNIFORM = "position";
const char *const PRGL_LIGHT_LINEAR_UNIFORM = "linear";
const char *const PRGL_LIGHT_QUADRATIC_UNIFORM = "quadratic";
const char *const PRGL_RENDER_RESOLUTION_UNIFORM = "renderResolution";
const char *const PRGL_TILE_FACTOR_UNIFORM = "tileFactor";

static unsigned int prgl_shader_pool[PRGL_SHADER_COUNT];
static unsigned int prgl_current_shader_id;

static unsigned int
prgl_compile_shader(int gl_shader_type, const char *const shader_source);

static unsigned int
prgl_create_shader_program(unsigned int shaders[], size_t length);

static void prgl_validate_shader(unsigned int shader);
static void prgl_validate_shader_program(unsigned int shader_program);

unsigned int prgl_shader(enum PRGLShader type)
{
    return prgl_shader_pool[type];
}

unsigned int prgl_current_shader(void) { return prgl_current_shader_id; }

unsigned int prgl_create_shader(
    const char *const vertex_source, const char *const frag_source,
    const char *const geometry_source
)
{
    // Create a basic vertex shader
    unsigned int vertex_shader =
        prgl_compile_shader(GL_VERTEX_SHADER, vertex_source);

    // Create a fragment shader for color
    unsigned int fragment_shader =
        prgl_compile_shader(GL_FRAGMENT_SHADER, frag_source);

    // Create geometry shader if one was passed in
    int shader_count = 2;
    unsigned int geometry_shader = 0;
    if (geometry_source != NULL)
    {
        shader_count++;
        geometry_shader =
            prgl_compile_shader(GL_GEOMETRY_SHADER, geometry_source);
    }

    // Use a shader program to link the shaders together
    unsigned int shaders[3] = {vertex_shader, fragment_shader, geometry_shader};
    unsigned int shader_program =
        prgl_create_shader_program(shaders, shader_count);

    // Delete the shader objects, we're done with them
    for (int i = 0; i < shader_count; i++)
    {
        glDeleteShader(shaders[i]);
    }

    return shader_program;
}

void prgl_use_shader(unsigned int shader)
{
    glUseProgram(shader);
    prgl_current_shader_id = shader;
}

void prgl_use_shader_3d(void)
{
    prgl_use_shader(prgl_shader_pool[PRGL_SHADER_3D]);
}

void prgl_use_shader_2d(void)
{
    prgl_use_shader(prgl_shader_pool[PRGL_SHADER_2D]);
}

void prgl_delete_shader(unsigned int shader) { glDeleteProgram(shader); }

void prgl_set_shader_uniform_4f(
    unsigned int shader, const char *const name, float a, float b, float c,
    float d
)
{
    glUniform4f(glGetUniformLocation(shader, name), a, b, c, d);
}

void prgl_set_shader_uniform_vec3(
    unsigned int shader, const char *const name, vec3 vec
)
{
    glUniform3fv(glGetUniformLocation(shader, name), 1, vec);
}

void prgl_set_shader_uniform_vec2(
    unsigned int shader, const char *const name, vec2 vec
)
{
    glUniform2fv(glGetUniformLocation(shader, name), 1, vec);
}

void prgl_set_shader_uniform_mat4(
    unsigned int shader, const char *const name, mat4 matrix
)
{
    glUniformMatrix4fv(
        glGetUniformLocation(shader, name), 1, GL_FALSE, (float *)matrix
    );
}

void prgl_set_shader_uniform_mat3(
    unsigned int shader, const char *const name, mat3 matrix
)
{
    glUniformMatrix3fv(
        glGetUniformLocation(shader, name), 1, GL_FALSE, (float *)matrix
    );
}

void prgl_set_shader_uniform_float(
    unsigned int shader, const char *const name, float value
)
{
    glUniform1f(glGetUniformLocation(shader, name), value);
}

void prgl_set_shader_uniform_int(
    unsigned int shader, const char *const name, int value
)
{
    glUniform1i(glGetUniformLocation(shader, name), value);
}

void prgl_set_shader_uniform_bool(
    unsigned int shader, const char *const name, bool value
)
{
    glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void prgl_init_shader_pool(void)
{
    unsigned int shader_screen = prgl_init_shader_screen();
    unsigned int shader_3d = prgl_init_shader_3d();
    unsigned int shader_2d = prgl_init_shader_2d();
    unsigned int shader_unlit = prgl_init_shader_unlit();

    // Don't do loop so if we remove any it doesn't break even if out of order
    prgl_shader_pool[PRGL_SHADER_SCREEN] = shader_screen;
    prgl_shader_pool[PRGL_SHADER_2D] = shader_2d;
    prgl_shader_pool[PRGL_SHADER_3D] = shader_3d;
    prgl_shader_pool[PRGL_SHADER_UNLIT] = shader_unlit;

    prgl_use_shader_3d();
}

void prgl_delete_shader_pool(void)
{
    for (int i = 0; i < PRGL_SHADER_COUNT; i++)
    {
        glDeleteProgram(prgl_shader_pool[i]);
    }
}

/**
 * Creates a shader using the given shader source code string.
 *
 * @param gl_shader_type The GL macro for the type of shader
 *                           e.g. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER
 * @param shader_source[in] String containing the source code for the shader
 * @return shader
 */
static unsigned int
prgl_compile_shader(int gl_shader_type, const char *const shader_source)
{
    unsigned int shader = glCreateShader(gl_shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
    prgl_validate_shader(shader);

    return shader;
}

/**
 * Creates a shader program from an array of shader IDs.
 *
 * @param shaders An array of shader IDs to attach to the program
 * @param length The length of the shaders array
 */
static unsigned int
prgl_create_shader_program(unsigned int shaders[], size_t length)
{
    unsigned int shader_program = glCreateProgram();
    for (size_t i = 0; i < length; i++)
    {
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);
    prgl_validate_shader_program(shader_program);

    return shader_program;
}

/**
 * Checks if a shader had any compilation errors and logs them if it did.
 *
 * @param shader The ID of the shader to validate
 */
static void prgl_validate_shader(unsigned int shader)
{
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", info_log);
    }
}

/**
 * Checks if a shader program has any link errors and logs them if it did.
 *
 * @param shader The ID of the shader program to validate
 */
static void prgl_validate_shader_program(unsigned int shader_program)
{
    int success;
    char info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::LINK_FAILED\n%s\n", info_log);
    }
}
