#include "shaders_init_internal.h"
#include "shaders.h"
#include "common_macros.h"

unsigned int pr3d_init_shader_screen(void)
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

    return pr3d_create_shader(VERTEX_SHADER_SOURCE, FRAG_SHADER_SOURCE);
}

unsigned int pr3d_init_shader_2d(void)
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

        // Affine texture mapping - we need both regular and noperspective 
        // texture coordinates to prevent problems with polygon warping.
        "out vec2 texCoord;\n"
        "out float originalW;\n"
        "noperspective out vec2 affineTexCoord;\n"

        // Flat is so the flag is constant across the triangle
        "flat out int useAffineFlag;\n"

        // Calculate lighting per vertex for gouraud shading
        "out vec3 vertexColor;\n"
        "uniform PointLight pointLights[NR_POINT_LIGHTS];"

        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"

        "uniform vec2 renderResolution;\n"

        "void main()\n"
        "{\n"

        // view * model brings into camera view space, the below operation
        // transforms to clip space.
        "    vec4 pos = projection * view * model * vec4(aPos, 1.0);\n"

        // Store non-wobbled w for texture mapping or textures slide around
        "    originalW = pos.w;\n"

        // Pixel wobble - redo the perspective divide and floor it to int value
        "    vec3 perspective_divide = pos.xyz / vec3(pos.w);\n"
        "    vec2 render_coords = (perspective_divide.xy + vec2(1.0, 1.0)) * renderResolution * 0.5;\n"
        "    vec2 int_coords = floor(render_coords);\n"
        // Convert back to unit space clip range -1 to 1
        "    vec2 reclipped = (int_coords * 2.0 / renderResolution) - vec2(1, 1);\n" 
        // Rebuild clip space projection with new imprecise values
        "    vec4 wobble_pos = vec4(reclipped.x * pos.w, reclipped.y * pos.w, perspective_divide.z * pos.w, pos.w);\n"

        // Frustum check for affine texture mapping
        "    bool outsideFrustum =\n"
        "        wobble_pos.x < -wobble_pos.w || wobble_pos.x > wobble_pos.w || \n"
        "        wobble_pos.y < -wobble_pos.w || wobble_pos.y > wobble_pos.w || \n"
        "        wobble_pos.z < -wobble_pos.w || wobble_pos.z > wobble_pos.w;\n"
        "    useAffineFlag = !outsideFrustum ? 1 : 0;\n"

        // Align pixels
        "    vec2 half_pixel_offset = 0.5 / renderResolution;\n"
        "    wobble_pos.xy += half_pixel_offset * wobble_pos.w;\n"
        "    gl_Position = wobble_pos;\n"

        // For affine we need to divide out w, we'll multiply it back in later
        "    texCoord = aTexCoord;\n"
        "    affineTexCoord = aTexCoord / pos.w;\n"

        // For non-uniform scaling need to multiply by the normal matrix.
        "    mat4 wobbleModel = model;\n"
        "    wobbleModel[3].xyz = vec3(wobble_pos) / (projection * view)[3].w;\n" 
        "    mat3 normalMatrix = mat3(transpose(inverse(wobbleModel)));\n"
        "    vec3 normal = normalize(normalMatrix * aNormal);\n"

        // Get world space position of vertex and calculate light direction from
        // the light source to the vertex.
        "    vec3 vertexPosition = vec3(wobbleModel * vec4(aPos, 1.0));\n"
        "    vec3 result = vec3(0.0);\n"
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

    // If nothing is passed for alpha it defaults to 1.0
    const char *const FRAG_SHADER_SOURCE =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 texCoord;\n"
        "noperspective in vec2 affineTexCoord;\n"
        "flat in int useAffineFlag;\n"
        "in float originalW;\n"
        "in vec3 vertexColor;\n"

        "uniform float alpha;\n"
        "uniform sampler2D imageTexture;\n"
        "uniform vec2 tileFactor;\n"

        "void main()\n"
        "{\n"
        "   vec2 texCoordFinal = texCoord;\n"
        "   if (useAffineFlag > 0)\n"
        "   {\n"
        // Reapply the w coordinate here, has to do with perspective correction
        "       texCoordFinal = affineTexCoord * originalW;\n"
        "   }\n"
        "   FragColor = texture(imageTexture, texCoordFinal * tileFactor) * vec4(vertexColor, "
        "alpha);\n"
        "}\0";
    // clang-format on

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
