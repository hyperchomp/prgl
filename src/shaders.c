#include "glad.h"

#include "shaders.h"
#include "shaders_internal.h"
#include "shaders_init_internal.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

#include "camera.h"
#include "render.h"
#include "types.h"
#include "cglm/vec2.h"

const char *const PRGL_MODEL_UNIFORM = "model";
const char *const PRGL_NORMAL_MATRIX_UNIFORM = "normalMatrix";
const char *const PRGL_NUM_POINT_LIGHTS_UNIFORM = "numPointLights";
const char *const PRGL_LIGHT_COLOR_UNIFORM = "color";
const char *const PRGL_LIGHT_POSITION_UNIFORM = "position";
const char *const PRGL_LIGHT_LINEAR_UNIFORM = "linear";
const char *const PRGL_LIGHT_QUADRATIC_UNIFORM = "quadratic";
const char *const PRGL_RENDER_RESOLUTION_UNIFORM = "renderResolution";
const char *const PRGL_TILE_FACTOR_UNIFORM = "tileFactor";
const char *const PRGL_FILL_COLOR_UNIFORM = "fillColor";
const char *const PRGL_USE_TEXTURE_UNIFORM = "useTexture";

static PRGLShader prgl_shader_pool[PRGL_SHADER_TYPE_COUNT];
static PRGLShader prgl_current_shader_ref;

static GLuint prgl_compile_shader(
    int gl_shader_type, const char *const shader_source[], int num_sources
);

static PRGLShader prgl_create_shader_program(GLuint shaders[], size_t length);

static void prgl_validate_shader(GLuint shader);
static void prgl_validate_shader_program(PRGLShader shader_program);

PRGLShader prgl_shader(enum PRGLShaderType type)
{
    return prgl_shader_pool[type];
}

PRGLShader prgl_current_shader(void) { return prgl_current_shader_ref; }

PRGLShader prgl_create_shader(
    const char *const vertex_source[], int num_vertex_sources,
    const char *const frag_source[], int num_frag_sources,
    const char *const geometry_source[], int num_geometry_sources
)
{
    // Create a basic vertex shader
    GLuint vertex_shader = prgl_compile_shader(
        GL_VERTEX_SHADER, vertex_source, num_vertex_sources
    );

    // Create a fragment shader for color
    GLuint fragment_shader =
        prgl_compile_shader(GL_FRAGMENT_SHADER, frag_source, num_frag_sources);

    // Create geometry shader if one was passed in
    int shader_count = 2;
    GLuint geometry_shader = 0;
    if (geometry_source != NULL)
    {
        shader_count++;
        geometry_shader = prgl_compile_shader(
            GL_GEOMETRY_SHADER, geometry_source, num_geometry_sources
        );
    }

    // Use a shader program to link the shaders together
    GLuint shaders[3] = {vertex_shader, fragment_shader, geometry_shader};
    PRGLShader shader_program =
        prgl_create_shader_program(shaders, shader_count);

    // Delete the shader objects, we're done with them
    for (int i = 0; i < shader_count; i++)
    {
        glDeleteShader(shaders[i]);
    }

    return shader_program;
}

void prgl_use_shader(PRGLShader shader)
{
    glUseProgram(shader.id);
    prgl_current_shader_ref = shader;

    struct PRGLCamera *cam = prgl_active_camera();
    if (cam)
    {
    }
}

void prgl_use_shader_3d(void)
{
    prgl_use_shader(prgl_shader_pool[PRGL_SHADER_TYPE_3D]);
    prgl_set_default_shared_uniforms(true);
}

void prgl_use_shader_unlit(void)
{
    prgl_use_shader(prgl_shader_pool[PRGL_SHADER_TYPE_UNLIT]);
    prgl_set_default_shared_uniforms(true);
}

void prgl_use_shader_2d(void)
{
    prgl_use_shader(prgl_shader_pool[PRGL_SHADER_TYPE_2D]);
    prgl_set_default_shared_uniforms(false);
}

void prgl_delete_shader(PRGLShader shader) { glDeleteProgram(shader.id); }

void prgl_set_shader_uniform_4f(
    PRGLShader shader, const char *const name, float a, float b, float c,
    float d
)
{
    glUniform4f(glGetUniformLocation(shader.id, name), a, b, c, d);
}

void prgl_set_shader_uniform_vec3(
    PRGLShader shader, const char *const name, vec3 vec
)
{
    glUniform3fv(glGetUniformLocation(shader.id, name), 1, vec);
}

void prgl_set_shader_uniform_vec2(
    PRGLShader shader, const char *const name, vec2 vec
)
{
    glUniform2fv(glGetUniformLocation(shader.id, name), 1, vec);
}

void prgl_set_shader_uniform_mat4(
    PRGLShader shader, const char *const name, mat4 matrix
)
{
    glUniformMatrix4fv(
        glGetUniformLocation(shader.id, name), 1, GL_FALSE, (float *)matrix
    );
}

void prgl_set_shader_uniform_mat3(
    PRGLShader shader, const char *const name, mat3 matrix
)
{
    glUniformMatrix3fv(
        glGetUniformLocation(shader.id, name), 1, GL_FALSE, (float *)matrix
    );
}

void prgl_set_shader_uniform_float(
    PRGLShader shader, const char *const name, float value
)
{
    glUniform1f(glGetUniformLocation(shader.id, name), value);
}

void prgl_set_shader_uniform_int(
    PRGLShader shader, const char *const name, int value
)
{
    glUniform1i(glGetUniformLocation(shader.id, name), value);
}

void prgl_set_shader_uniform_bool(
    PRGLShader shader, const char *const name, bool value
)
{
    glUniform1i(glGetUniformLocation(shader.id, name), (int)value);
}

void prgl_set_default_shared_uniforms(bool is_3d)
{
    PRGLShader current_shader = prgl_current_shader();
    prgl_set_shader_uniform_vec2(
        current_shader, PRGL_TILE_FACTOR_UNIFORM, GLM_VEC2_ONE
    );

    vec2 render_res = {PRGL_RENDER_RESOLUTION[0], PRGL_RENDER_RESOLUTION[1]};
    prgl_set_shader_uniform_vec2(
        current_shader, PRGL_RENDER_RESOLUTION_UNIFORM, render_res
    );

    struct PRGLCamera *cam = prgl_active_camera();
    if (cam == NULL)
    {
        fprintf(
            stderr, "prgl_set_default_shared_uniforms: Camera is NULL, can't "
                    "set projection."
        );
    }
    else if (is_3d)
    {
        prgl_set_shader_uniform_mat4(current_shader, "view", cam->view);
        prgl_set_camera_projection(
            prgl_active_camera(), prgl_active_camera()->fov,
            PRGL_CAMERA_PROJECTION_PERSPECTIVE
        );
    }
    else
    {
        prgl_set_camera_projection(
            prgl_active_camera(), 0.0f, PRGL_CAMERA_PROJECTION_ORTHOGONAL
        );
    }
}

void prgl_init_shader_pool(void)
{
    PRGLShader shader_screen = prgl_init_shader_screen();
    PRGLShader shader_3d = prgl_init_shader_3d();
    PRGLShader shader_2d = prgl_init_shader_2d();
    PRGLShader shader_unlit = prgl_init_shader_unlit();

    // Don't do loop so if we remove any it doesn't break even if out of order
    prgl_shader_pool[PRGL_SHADER_TYPE_SCREEN] = shader_screen;
    prgl_shader_pool[PRGL_SHADER_TYPE_2D] = shader_2d;
    prgl_shader_pool[PRGL_SHADER_TYPE_3D] = shader_3d;
    prgl_shader_pool[PRGL_SHADER_TYPE_UNLIT] = shader_unlit;
}

void prgl_delete_shader_pool(void)
{
    for (int i = 0; i < PRGL_SHADER_TYPE_COUNT; i++)
    {
        glDeleteProgram(prgl_shader_pool[i].id);
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
static GLuint prgl_compile_shader(
    int gl_shader_type, const char *const shader_source[], int num_sources
)
{
    GLuint shader = glCreateShader(gl_shader_type);
    glShaderSource(shader, num_sources, shader_source, NULL);
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
static PRGLShader prgl_create_shader_program(GLuint shaders[], size_t length)
{
    PRGLShader shader_program = {.id = glCreateProgram()};
    for (size_t i = 0; i < length; i++)
    {
        glAttachShader(shader_program.id, shaders[i]);
    }
    glLinkProgram(shader_program.id);
    prgl_validate_shader_program(shader_program);

    return shader_program;
}

/**
 * Checks if a shader had any compilation errors and logs them if it did.
 *
 * @param shader The ID of the shader to validate
 */
static void prgl_validate_shader(GLuint shader)
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
static void prgl_validate_shader_program(PRGLShader shader_program)
{
    int success;
    char info_log[512];
    glGetProgramiv(shader_program.id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program.id, 512, NULL, info_log);
        printf("ERROR::SHADER::LINK_FAILED\n%s\n", info_log);
    }
}
