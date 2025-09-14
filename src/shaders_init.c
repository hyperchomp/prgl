#include "shaders_init_internal.h"
#include "shaders.h"

#include "common_macros.h"
#include "lighting.h"
#include "types.h"

// clang-format off
static const char *const SHARED_VERTEX_SHADER_SOURCE_3D =
    "#version 330 core\n"
    "#define NR_POINT_LIGHTS " STRINGIFY(PRGL_MAX_POINT_LIGHTS) "\n"

    "struct PointLight {\n"
    "    vec3 position;\n"
    "    vec3 color;\n"
    "    float ambient;\n"

    // Constants for calculating light attenuation (fade distance).
    "    float linear;\n"
    "    float quadratic;\n"
    "};\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform mat3 normalMatrix;\n"
    "uniform vec2 renderResolution;\n"
    "uniform int numPointLights = 0;\n"
    "uniform PointLight pointLights[NR_POINT_LIGHTS];\n"

    "vec4 calculateVertexWobble(vec4 clipSpacePos)\n"
    "{\n"
         // Redo perspective divide and floor for imprecision
         // Perspective divide gives us NDC/unit space, since that is -1 to 1
         // we add 2 to get 0 to 2, then multiply to get to our render res
    "    vec3 perspectiveDiv = clipSpacePos.xyz / vec3(clipSpacePos.w);\n"
    "    vec2 render_coords = (perspectiveDiv.xy + vec2(1.0, 1.0)) * renderResolution * 0.5;\n"
    "    vec2 int_coords = floor(render_coords);\n"

         // Convert back to NDC
    "    vec2 reclip = (int_coords * 2.0 / renderResolution) - vec2(1, 1);\n"

         // Rebuild clip space projection with new, imprecise values
    "    vec4 wobblePos = vec4(reclip.x * clipSpacePos.w," 
    "                          reclip.y * clipSpacePos.w," 
    "                          perspectiveDiv.z * clipSpacePos.w,"
    "                          clipSpacePos.w);\n"

         // Align vertices with render resolution pixels, not full window resolution 
    "    vec2 halfPixelOffset = 0.5 / renderResolution;\n"
    "    wobblePos.xy += halfPixelOffset * wobblePos.w;\n"
    "    return wobblePos;\n"
    "}\n"

    // Calculates lighting at vertices.
    "vec3 calculateGouraudShading(vec3 pos, vec3 normal)\n"
    "{\n"
    "    vec3 worldNormal = normalize(normalMatrix * normal);\n"

         // Get world space position of vertex and calculate light direction 
         // from the light source to the vertex.
    "    vec3 vertexPosition = vec3(model * vec4(pos, 1.0));\n"
    "    vec3 lightColor = vec3(0.0);\n"
    "    for (int i = 0; i < numPointLights; i++)\n"
    "    {\n"
    "        vec3 distanceVec = pointLights[i].position - vertexPosition;\n"
    "        vec3 lightDir = normalize(distanceVec);\n"

             // Calculate diffuse, darkens the greater the angle between vectors.
             // max() is used to prevent negatives when the angle is > 90 degrees.
    "        float diffuse = max(dot(worldNormal, lightDir), 0.0);\n"
    "        float distance = length(distanceVec);\n"
    "        float attenuation = 1.0 "
                "/ (1.0 + (pointLights[i].linear * distance) "
                "+ (pointLights[i].quadratic * (distance * distance)));\n"
    "        float ambient = pointLights[i].ambient;\n"
    "        ambient *= attenuation;\n"
    "        diffuse *= attenuation;\n"
    "        lightColor += (ambient + diffuse) * pointLights[i].color;\n"
    "    }\n"
    "    return lightColor;\n"
    "}\n";

const char *const SHARED_FRAG_SHADER_SOURCE_3D =
    "#version 330 core\n"
    "out vec4 FragColor;\n"

    // Data received from the geometry shader
    "flat in int useAffineFlag;\n"
    "in vec2 perspectiveUV;\n"
    "noperspective in vec2 affineUV;\n"
    "in vec3 fragLightColor;\n"

    "uniform bool useTexture = true;\n"
    "uniform sampler2D imageTexture;\n"
    "uniform vec2 tileFactor = vec2(1.0, 1.0);\n"
    "uniform vec3 fillColor = vec3(1.0, 1.0, 1.0);"
    "uniform float alpha = 1.0;\n"

    "void main()\n"
    "{\n"
    "   vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "   if (useTexture)\n"
    "   {\n"
            // Switch between UV sets based on the flag
    "       vec2 finalUV = (useAffineFlag == 1) ? affineUV : perspectiveUV;\n"
    "       textureColor = texture(imageTexture, finalUV * tileFactor);\n" 
    "   }\n"
    "   FragColor = textureColor * vec4(fragLightColor * fillColor, alpha);\n"
    "}\0";
// clang-format on

PRGLShader prgl_init_shader_screen(void)
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

    return prgl_create_shader(
        &VERTEX_SHADER_SOURCE, 1, &FRAG_SHADER_SOURCE, 1, NULL, 0
    );
}

PRGLShader prgl_init_shader_2d(void)
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
        "    vec2 halfPixelOffset = 0.5 / renderResolution;\n"
        "    vec4 pos = projection * model * vec4(aPos.xy, 0.0f, 1.0);\n"
        "    pos.xy += halfPixelOffset * pos.w;\n"
        "    gl_Position = pos;\n"
        "    texCoord = aTexCoord;\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 texCoord;\n"

        "uniform bool useTexture = true;\n"
        "uniform vec2 tileFactor = vec2(1.0, 1.0);\n"
        "uniform vec3 fillColor = vec3(1.0, 1.0, 1.0);"
        "uniform float alpha = 1.0;\n"
        "uniform sampler2D imageTexture;\n"

        "void main()\n"
        "{\n"
        "   vec4 textureColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "   if (useTexture)\n"
        "   {\n"
        "       textureColor = texture(imageTexture, texCoord * tileFactor);\n"
        "   }\n"
        "   FragColor = textureColor * vec4(fillColor, alpha);\n"
        "}\0";

    return prgl_create_shader(
        &VERTEX_SHADER_SOURCE, 1, &FRAG_SHADER_SOURCE, 1, NULL, 0
    );
}

PRGLShader prgl_init_shader_3d(void)
{
    // clang-format off
    const char *const VERTEX_SHADER_SOURCE =
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "layout (location = 2) in vec2 aTexCoord;\n"

        // Data to be sent to the geometry shader
        "out VS_OUT {\n"
        "    vec2 texCoord;\n"
        "    vec3 vertexColor;\n"
        "} vs_out;\n"

        "void main()\n"
        "{\n"
        "    vs_out.texCoord = aTexCoord;\n"

             // For retro accuracy is calculated using un-wobbled position
        "    vs_out.vertexColor = calculateGouraudShading(aPos, aNormal);\n"

        "    vec4 clipSpacePos = projection * view * model * vec4(aPos, 1.0);\n"
        "    gl_Position = calculateVertexWobble(clipSpacePos);\n"
        "}\0";

    const char *const GEOMETRY_SHADER_SOURCE =
        "#version 330 core\n"
        "layout (triangles) in;\n"
        "layout (triangle_strip, max_vertices = 3) out;\n"

        "in VS_OUT {\n"
        "    vec2 texCoord;\n"
        "    vec3 vertexColor;\n"
        "} gs_in[];\n"

        "flat out int useAffineFlag;\n"
        "out vec2 perspectiveUV;\n"
        "noperspective out vec2 affineUV;\n"
        "out vec3 fragLightColor;\n"

        "void main()\n"
        "{\n" 
             // ***AFFINE TEXTURE MAPPING*** - only apply if all vertices in view.
             // Fixes an extreme warping problem when triangles are partly outside view.
        "    useAffineFlag = 1;"
        "    for (int i = 0; i < 3; i++)\n" 
        "    {\n"
        "        vec4 pos = gl_in[i].gl_Position;\n"
        "        if (useAffineFlag == 1\n"
        "            && (pos.x < -pos.w || pos.x > pos.w\n"
        "             || pos.y < -pos.w || pos.y > pos.w\n"
        "             || pos.z < -pos.w || pos.z > pos.w))\n" 
        "        {\n"
        "            useAffineFlag = 0;\n"
        "        }\n"

                 // Must always update position for each vertex in geometry shader
        "        gl_Position = gl_in[i].gl_Position;\n"

                 // UV Coordinates for perspective correct and affine 
        "        perspectiveUV = gs_in[i].texCoord;\n"
        "        affineUV = gs_in[i].texCoord;\n"
        "        fragLightColor = gs_in[i].vertexColor;\n"
        "        EmitVertex();\n"
        "    }\n"

        "    EndPrimitive();\n"
        "}\0";
    //clang-format on

    const char *const vertexSources[] = {
        SHARED_VERTEX_SHADER_SOURCE_3D, VERTEX_SHADER_SOURCE
    };
    return prgl_create_shader(
        vertexSources, 2, &SHARED_FRAG_SHADER_SOURCE_3D, 1, &GEOMETRY_SHADER_SOURCE, 1
    );
}

PRGLShader prgl_init_shader_unlit(void)
{
    const char *const VERTEX_SHADER_SOURCE =
        "layout (location = 0) in vec3 aPos;\n"

        "void main()\n"
        "{\n"
        "    vec4 clipPos = projection * view * model * vec4(aPos, 1.0);\n"
        "    gl_Position = calculateVertexWobble(clipPos);\n"
        "}\0";

    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"

        "uniform vec3 fillColor = vec3(1.0, 1.0, 1.0);"
        "uniform float alpha = 1.0;\n"

        "void main()\n"
        "{\n"
        "   FragColor = vec4(fillColor, alpha);\n"
        "}\0";

    const char *const vertexSources[] = {
        SHARED_VERTEX_SHADER_SOURCE_3D, VERTEX_SHADER_SOURCE
    };
    return prgl_create_shader(
        vertexSources, 2, &FRAG_SHADER_SOURCE, 1, NULL, 0
    );
}
