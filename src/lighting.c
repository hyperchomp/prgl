#include "lighting.h"

#include <stdio.h>

#include "cglm/vec3.h"
#include "shaders.h"

void prgl_init_point_light(struct PRGLPointLight *const light, vec3 position)
{
    glm_vec3_copy(position, light->position);
    glm_vec3_one(light->lightColor);
    light->intensity = PRGL_LIGHT_INTENSITY_MEDIUM;
    light->ambient = 0.1f;
}

void prgl_update_lighting(
    struct PRGLPointLight *const point_lights, int num_lights
)
{
    if (num_lights > PRGL_MAX_POINT_LIGHTS)
    {
        num_lights = PRGL_MAX_POINT_LIGHTS;
    }

    prgl_set_shader_uniform_int(
        prgl_current_shader(), PRGL_NUM_POINT_LIGHTS_UNIFORM, num_lights
    );

    char uniform_name_buffer[64];
    for (int i = 0; i < num_lights; i++)
    {
        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_COLOR_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer,
            point_lights[i].lightColor
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_POSITION_UNIFORM
        );
        prgl_set_shader_uniform_vec3(
            prgl_current_shader(), uniform_name_buffer, point_lights[i].position
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_LINEAR_UNIFORM
        );
        prgl_set_shader_uniform_float(
            prgl_current_shader(), uniform_name_buffer,
            prgl_light_attenuation_linear_constant(point_lights[i].intensity)
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_QUADRATIC_UNIFORM
        );
        prgl_set_shader_uniform_float(
            prgl_current_shader(), uniform_name_buffer,
            prgl_light_attenuation_quadratic_constant(point_lights[i].intensity)
        );

        snprintf(
            uniform_name_buffer, sizeof(uniform_name_buffer),
            "pointLights[%d].%s", i, PRGL_LIGHT_AMBIENT_UNIFORM
        );
        prgl_set_shader_uniform_float(
            prgl_current_shader(), uniform_name_buffer,
            prgl_light_attenuation_quadratic_constant(point_lights[i].ambient)
        );
    }
}

float prgl_light_attenuation_linear_constant(enum PRGLLightIntensity intensity)
{
    switch (intensity)
    {
        case PRGL_LIGHT_INTENSITY_ZERO:
            return 0.0f;
        case PRGL_LIGHT_INTENSITY_VERY_LOW:
            return 0.14f;
        case PRGL_LIGHT_INTENSITY_LOW:
            return 0.07f;
        case PRGL_LIGHT_INTENSITY_MEDIUM:
            return 0.027f;
        case PRGL_LIGHT_INTENSITY_HIGH:
            return 0.014f;
        case PRGL_LIGHT_INTENSITY_VERY_HIGH:
            return 0.0014f;
        default:
            fprintf(
                stderr,
                "prgl_light_attenuation_linear_constant: Value provided for "
                "intensity was not a valid PRGLLightIntensity enum value."
            );
    }

    return 0.0f;
}

float prgl_light_attenuation_quadratic_constant(
    enum PRGLLightIntensity intensity
)
{
    switch (intensity)
    {
        case PRGL_LIGHT_INTENSITY_ZERO:
            return 0.0f;
        case PRGL_LIGHT_INTENSITY_VERY_LOW:
            return 0.07f;
        case PRGL_LIGHT_INTENSITY_LOW:
            return 0.017f;
        case PRGL_LIGHT_INTENSITY_MEDIUM:
            return 0.0028f;
        case PRGL_LIGHT_INTENSITY_HIGH:
            return 0.0007f;
        case PRGL_LIGHT_INTENSITY_VERY_HIGH:
            return 0.000007f;
        default:
            fprintf(
                stderr,
                "prgl_light_attenuation_quadratic_constant: Value provided for "
                "intensity was not a valid PRGLLightIntensity enum value."
            );
    }

    return 0.0f;
}
