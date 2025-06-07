#include "glad.h"
#include "shaders.h"
#include "shaders_internal.h"
#include "common_macros.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

static unsigned int pr3d_shader_pool[PR3D_SHADER_COUNT];

static unsigned int
pr3d_compile_shader(int gl_shader_type, const char *const shader_source);

static unsigned int
pr3d_create_shader_program(unsigned int shaders[], size_t length);

static void pr3d_validate_shader(unsigned int shader);
static void pr3d_validate_shader_program(unsigned int shader_program);

unsigned int pr3d_shader(enum PR3DShader type)
{
    return pr3d_shader_pool[type];
}

unsigned int
pr3d_create_shader(const char *vertex_source, const char *frag_source)
{
    // Create a basic vertex shader
    unsigned int vertex_shader =
        pr3d_compile_shader(GL_VERTEX_SHADER, vertex_source);

    // Create a fragment shader for color
    unsigned int fragment_shader =
        pr3d_compile_shader(GL_FRAGMENT_SHADER, frag_source);

    // Use a shader program to link the shaders together
    unsigned int shaders[] = {vertex_shader, fragment_shader};
    unsigned int shader_program =
        pr3d_create_shader_program(shaders, ARR_LEN(shaders));

    // Delete the shader objects, we're done with them
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}

void pr3d_use_shader(unsigned int shader) { glUseProgram(shader); }

void pr3d_delete_shader(unsigned int shader) { glDeleteProgram(shader); }

void pr3d_init_shader_pool(void)
{
    const char *VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char *FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    unsigned int basic_shader =
        pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);

    pr3d_shader_pool[PR3D_SHADER_BASIC] = basic_shader;

    pr3d_use_shader(basic_shader);
}

void pr3d_delete_shader_pool(void)
{
    for (int i = 0; i < PR3D_SHADER_COUNT; i++)
    {
        glDeleteProgram(pr3d_shader_pool[i]);
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
pr3d_compile_shader(int gl_shader_type, const char *shader_source)
{
    unsigned int shader = glCreateShader(gl_shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);
    pr3d_validate_shader(shader);

    return shader;
}

/**
 * Creates a shader program from an array of shader IDs.
 *
 * @param shaders An array of shader IDs to attach to the program
 * @param length The length of the shaders array
 */
static unsigned int
pr3d_create_shader_program(unsigned int shaders[], size_t length)
{
    unsigned int shader_program = glCreateProgram();
    for (size_t i = 0; i < length; i++)
    {
        glAttachShader(shader_program, shaders[i]);
    }
    glLinkProgram(shader_program);
    pr3d_validate_shader_program(shader_program);

    return shader_program;
}

/**
 * Checks if a shader had any compilation errors and logs them if it did.
 *
 * @param shader The ID of the shader to validate
 */
static void pr3d_validate_shader(unsigned int shader)
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
static void pr3d_validate_shader_program(unsigned int shader_program)
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
