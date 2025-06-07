#include "glad.h"
#include "render.h"
#include "common_macros.h"
#include <GLFW/glfw3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned int
pr3d_create_shader(int gl_shader_type, const char *const shader_source);

static unsigned int
pr3d_create_shader_program(unsigned int shaders[], size_t length);

static void pr3d_validate_shader(unsigned int shader);
static void pr3d_validate_shader_program(unsigned int shader_program);

static unsigned int pr3d_shader_pool[PR3D_SHADER_COUNT];

void pr3d_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

struct PR3DMesh *pr3d_create_triangle(
    struct PR3DVec3 *v1, struct PR3DVec3 *v2, struct PR3DVec3 *v3
)
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

    // Create a basic vertex shader
    unsigned int vertex_shader =
        pr3d_create_shader(GL_VERTEX_SHADER, VERTEX_SHADER_SOURCE);

    // Create a fragment shader for color
    unsigned int fragment_shader =
        pr3d_create_shader(GL_FRAGMENT_SHADER, FRAG_SHADER_SOURCE);

    // Use a shader program to link the shaders together
    unsigned int shaders[] = {vertex_shader, fragment_shader};
    unsigned int shader_program =
        pr3d_create_shader_program(shaders, ARR_LEN(shaders));

    // Delete the shader objects, we're done with them
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    float vertices[] = {
        v1->x, v1->y, v1->z, //
        v2->x, v2->y, v2->z, //
        v3->x, v3->y, v3->z  //
    };

    // Create a vertex buffer object and vertex array object, the VBO is to
    // generate the initial data, the VAO is so we can re-use it later
    unsigned int vbo;
    unsigned int vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    // Bind VAO, then bind and set buffers, then configure the vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret our vertex data
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    // Unbind the array buffer since glVertexAttribPointer() set it as the
    // vertex attributes buffer object, we don't need to unbind the VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    struct PR3DMesh mesh = {
        .vao = vao, .vbo = vbo, .shader_program = shader_program
    };
    struct PR3DMesh *mesh_pointer = malloc(sizeof(struct PR3DMesh));
    if (mesh_pointer == NULL)
    {
        printf("pr3d_create_triangle: Error allocating mesh pointer memory!");
    }
    *mesh_pointer = mesh;
    return mesh_pointer;
}

void pr3d_render_mesh(struct PR3DMesh *mesh)
{
    glUseProgram(mesh->shader_program);
    glBindVertexArray(mesh->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void pr3d_delete_mesh(struct PR3DMesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteProgram(mesh->shader_program);
}

unsigned int pr3d_shader(enum PR3DShader type)
{
    return pr3d_shader_pool[type];
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
pr3d_create_shader(int gl_shader_type, const char *shader_source)
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
