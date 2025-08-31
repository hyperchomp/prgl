#include "shaders_init_internal.h"
#include "shaders.h"
#include "common_macros.h"

unsigned int prgl_init_shader_screen(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"

        "out vec2 texCoord;\n"

        "void main()\n"
        "{\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "    texCoord = aTexCoord;\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 texCoord;\n"

        "uniform sampler2D imageTexture;\n"

        "void main()\n"
        "{\n"
        "   FragColor = texture(imageTexture, texCoord);\n"
        "}\0";

    return prgl_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE, NULL);
}

unsigned int prgl_init_shader_2d(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"

        "out vec2 texCoord;\n"

        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"

        "uniform vec2 renderResolution;\n"

        "void main()\n"
        "{\n"
        // Half pixel offset to align with pixel grid
        "    vec2 half_pixel_offset = 0.5 / renderResolution;\n"
        "    vec4 pos = projection * model * vec4(aPos.xy, 0.0f, 1.0);\n"
        "    pos.xy += half_pixel_offset * pos.w;\n"
        "    gl_Position = pos;\n"
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

    return prgl_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE, NULL);
}

unsigned int prgl_init_shader_3d(void)
{
    // clang-format off
    const char *const VERTEX_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        // Data to be sent to the geometry shader
        "out VS_OUT {\n"
        "    vec3 normal;\n"
        "    vec2 texCoord;\n"
        "} vs_out;\n"

        "void main()\n"
        "{\n"
        "    vs_out.normal = aNormal;\n"
        "    vs_out.texCoord = aTexCoord;\n"

        // view * model brings into camera view space, the below operation
        // transforms to clip space.
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";

    const char *const GEOMETRY_SHADER_SOURCE =
        "#version 330 core\n"
        "struct PointLight {\n"
        "    vec3 position;\n"
        "    vec3 color;\n"

        // Constants for calculating light attenuation (fade distance).
        "    float linear;\n"
        "    float quadratic;\n"
        "};\n"

        "#define NR_POINT_LIGHTS " STRINGIFY(PRGL_MAX_POINT_LIGHTS) "\n"

        "layout (triangles) in;\n"
        "layout (triangle_strip, max_vertices = 3) out;\n"

        "in VS_OUT {\n"
        "    vec3 normal;\n"
        "    vec2 texCoord;\n"
        "} gs_in[];\n"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "uniform vec2 renderResolution;\n"
        "uniform PointLight pointLights[NR_POINT_LIGHTS];"

        "flat out int useAffineFlag;\n"
        "out vec2 perspectiveUV;\n"
        "noperspective out vec2 affineUV;\n"
        "out vec3 vertexColor;\n"

        "void main() {\n" 

        // Affine texture mapping - only apply if all vertices in view. Fixes an
        // extreme warping problem when triangles are partly outside view.
        "    bool allVerticesInFrustum = true;\n"
        "    for (int i = 0; i < 3; i++) {\n"
        "        vec4 pos = gl_in[i].gl_Position;\n"
        "        if (pos.x < -pos.w || pos.x > pos.w ||\n"
        "            pos.y < -pos.w || pos.y > pos.w ||\n"
        "            pos.z < -pos.w || pos.z > pos.w) {\n"
        "            allVerticesInFrustum = false;\n"
        "            break;\n"
        "        }\n"
        "    }\n"
        "    useAffineFlag = allVerticesInFrustum ? 1 : 0;\n"

        "    for (int i = 0; i < 3; i++)\n"
        "    {\n"
                 // Vertex Wobble - redo perspective divide and floor for imprecision
        "        vec4 pos = gl_in[i].gl_Position;\n"
        "        vec3 pdiv = pos.xyz / pos.w;\n"
        "        vec2 render_coords = (pdiv.xy + 1.0) * renderResolution * 0.5;\n"
        "        vec2 int_coords = floor(render_coords);\n"

                 // Convert back to unit space -1.0 to 1.0
        "        vec2 reclip = (int_coords * 2.0 / renderResolution) - 1.0;\n"

                 // Rebuild clip space projection with new, imprecise values
        "        vec4 wobble_pos = vec4(reclip.x * pos.w, reclip.y * pos.w, pdiv.z * pos.w, pos.w);\n"

                 // Align vertices with render resolution pixels, not full window resolution 
        "        vec2 half_pixel_offset = 0.5 / renderResolution;\n"
        "        wobble_pos.xy += half_pixel_offset * wobble_pos.w;\n"
        "        gl_Position = wobble_pos;\n"

                 // Gouraud Shading - calculate lighting at vertices 
        "        mat3 normalMatrix = mat3(transpose(inverse(model)));\n"
        "        vec3 normal = normalize(normalMatrix * gs_in[i].normal);\n"

                 // Get world space position of vertex and calculate light
                 // direction from the light source to the vertex.
        "        vec3 vertexPosition = vec3(model * vec4(gl_in[i].gl_Position.xyz, 1.0));\n"
        "        vec3 result = vec3(0.0);\n"
        "        for (int j = 0; j < NR_POINT_LIGHTS; j++)\n"
        "        {\n"
        "            vec3 lightDir = normalize(pointLights[j].position - vertexPosition);\n"

                     // Calculate diffuse, darkens the greater the angle between vectors.
                     // max() is used to prevent negatives when the angle is > 90 degrees.
        "            float diffuse = max(dot(normal, lightDir), 0.0);\n"
        "            float ambient = 0.1;\n"
        "            float distance = length(pointLights[j].position - vertexPosition);\n"
        "            float attenuation = 1.0\n"
        "                / (1.0 + (pointLights[j].linear * distance)\n" 
        "                + (pointLights[j].quadratic * (distance * distance)));\n"
        "            ambient *= attenuation;\n"
        "            diffuse *= attenuation;\n"
        "            result += (ambient + diffuse) * pointLights[j].color;\n"
        "        }\n"
        "        vertexColor = result;\n"

                 // UV Coordinates for perspective correct and affine 
        "        perspectiveUV = gs_in[i].texCoord;\n"
        "        affineUV = gs_in[i].texCoord;\n"
        "        EmitVertex();\n"
        "    }\n"
        "    EndPrimitive();\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE =
    "#version 330 core\n"
    "out vec4 FragColor;\n"

    // Data received from the geometry shader
        "flat in int useAffineFlag;\n"
        "in vec2 perspectiveUV;\n"
        "noperspective in vec2 affineUV;\n"
        "in vec3 vertexColor;\n"

        "uniform sampler2D imageTexture;\n"
        "uniform vec2 tileFactor;\n"
        "uniform float alpha;\n"

        "void main()\n"
        "{\n"
        // Switch between UV sets based on the flag
        "   vec2 finalUV = (useAffineFlag == 1) ? affineUV : perspectiveUV;\n"
        "   FragColor = texture(imageTexture, finalUV * tileFactor) * vec4(vertexColor, alpha);\n"
        "}\0";
    // clang-format on

    return prgl_create_shader(
        VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE, GEOMETRY_SHADER_SOURCE
    );
}

unsigned int prgl_init_shader_unlit(void)
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

    return prgl_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE, NULL);
}
