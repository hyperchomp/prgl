#include "shaders_init_internal.h"
#include "shaders.h"
#include "common_macros.h"

unsigned int pr3d_init_shader_2d(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"

        "out vec2 texCoord;\n"

        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"
        "    gl_Position = projection * model * vec4(aPos.xy, 0.0f, 1.0);\n"
        "    texCoord = aTexCoord;\n"
        "}\0";

    // If nothing is passed for alpha it defaults to 1.0
    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 texCoord;\n"

        "uniform float alpha;\n"
        "uniform sampler2D imageTexture;\n"

        "void main()\n"
        "{\n"
        "   FragColor = texture(imageTexture, texCoord) * vec4(1.0, 1.0, 1.0, "
        "alpha);\n"
        "}\0";

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}

unsigned int pr3d_init_shader_3d(void)
{
    // clang-format off
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "struct PointLight {\n"
        "    vec3 position;\n"
        "    vec3 color;\n"

        // Constants for calculating light attenuation (fade distance).
        "    float linear;\n"
        "    float quadratic;\n"
        "};\n"

        "#define NR_POINT_LIGHTS " STRINGIFY(PR3D_MAX_POINT_LIGHTS) "\n"

        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"

        "out vec2 texCoord;\n"

        // Calculate lighting per vertex for gouraud shading
        "out vec3 vertexColor;\n"
        "uniform PointLight pointLights[NR_POINT_LIGHTS];"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"

        // view * model brings into camera view space, the below operation
        // transforms to clip space.
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "    texCoord = aTexCoord;\n"

        // For non-uniform scaling need to multiply by the normal matrix.
        "    vec3 normal = mat3(transpose(inverse(model))) * aNormal;\n"

        // Get world space position of vertex and calculate light direction from
        // the light source to the vertex.
        "    normal = normalize(vec3(model * vec4(aNormal, 0.0)));\n"
        "    vec3 vertexPosition = vec3(model * vec4(aPos, 1.0));\n"
        "    vec3 result;\n"
        "    for (int i = 0; i < NR_POINT_LIGHTS; i++)\n"
        "    {\n"
        "        vec3 lightDir = normalize(pointLights[i].position - vertexPosition);\n"

                 // Calculate diffuse, darkens the greater the angle between vectors.
                 // max() is used to prevent negatives when the angle is > 90 degrees.
        "        float diffuse = max(dot(normal, lightDir), 0.0);\n"
        "        float ambient = 0.1;\n"
        
        "        float distance = length(pointLights[i].position - vertexPosition);\n" 
        "        float attenuation = 1.0 / "
                    "(1.0 + (pointLights[i].linear * distance) "
                    "+ (pointLights[i].quadratic * (distance * distance)));\n"
        "        ambient *= attenuation;\n"
        "        diffuse *= attenuation;\n"

        "        result += (ambient + diffuse) * pointLights[i].color;\n"
        "    }\n"
        "    vertexColor = result;\n"
        "}\0";
    // clang-format on

    // If nothing is passed for alpha it defaults to 1.0
    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 texCoord;\n"
        "in vec3 vertexColor;\n"

        "uniform float alpha;\n"
        "uniform sampler2D imageTexture;\n"

        "void main()\n"
        "{\n"
        "   FragColor = texture(imageTexture, texCoord) * vec4(vertexColor, "
        "alpha);\n"
        "}\0";

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}

unsigned int pr3d_init_shader_unlit(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";

    // If nothing is passed for alpha it defaults to 1.0
    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"

        "uniform float alpha;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0, 1.0, 1.0, alpha);\n"
        "}\0";

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}
