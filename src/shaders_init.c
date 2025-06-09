#include "shaders_init_internal.h"
#include "shaders.h"

unsigned int pr3d_init_solid_color_shader(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE = "#version 330 core\n"
                                           "out vec4 FragColor;\n"
                                           "uniform vec4 fillColor;\n"
                                           "void main()\n"
                                           "{\n"
                                           "   FragColor = fillColor;\n"
                                           "}\0";

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}

unsigned int pr3d_init_vertex_color_shader(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vertexColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    vertexColor = aColor;\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 vertexColor;\n"
        "uniform float alpha;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(vertexColor, alpha);\n"
        "}\0";

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}
