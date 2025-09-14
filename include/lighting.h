#ifndef PRGL_LIGHTING_H
#define PRGL_LIGHTING_H

#include "cglm/types.h"

#define PRGL_MAX_POINT_LIGHTS 32

/**
 * @brief Determines the distance and fall-off for a light.
 *
 * Internally maps to preset linear and quadratic values to adjust the light
 * attenuation using the following formula:
 * 1.0 / (1.0 + linear * distance + quadratic * (distance * distance))
 *
 * Note if you want pitch black the light's ambient needs to be zero'd as well.
 */
enum PRGLLightIntensity
{
    PRGL_LIGHT_INTENSITY_ZERO,
    PRGL_LIGHT_INTENSITY_VERY_LOW,
    PRGL_LIGHT_INTENSITY_LOW,
    PRGL_LIGHT_INTENSITY_MEDIUM,
    PRGL_LIGHT_INTENSITY_HIGH,
    PRGL_LIGHT_INTENSITY_VERY_HIGH,
    PRGL_LIGHT_INTENSITY_COUNT,
};

/**
 * @brief A light which can be positioned in the world and emits a color.
 */
struct CGLM_ALIGN(8) PRGLPointLight
{
    vec3 position;
    vec3 lightColor;

    /// Used to determine distance and fall-off for a light.
    enum PRGLLightIntensity intensity;

    /// Constant 0-1 lighting value applied even if intensity is zero.
    float ambient;
};

/**
 * @brief Initializes point light values to defaults.
 *
 * Sets point light position to provided value, and gives default values for
 * other variables for a medium intensity white light.
 *
 * @param light[out]
 * @param position
 */
void prgl_init_point_light(struct PRGLPointLight *const light, vec3 position);

/**
 * @brief Passes lighting data from lights to the active shader.
 *
 * @param point_lights[in] PRGLPointLight The point lights for the scene.
 * @param num_lights
 */
void prgl_update_lighting(
    struct PRGLPointLight *const point_lights, int num_lights
);

/**
 * @brief Gets the linear constant for light attenuation from the intensity.
 *
 * @param intensity
 * @return The linear constant value for the given enum value.
 */
float prgl_light_attenuation_linear_constant(enum PRGLLightIntensity intensity);

/**
 * @brief Gets the quadratic constant for light attenuation from the intensity.
 *
 * @param intensity
 * @return The quadratic constant value for the given enum value.
 */
float prgl_light_attenuation_quadratic_constant(
    enum PRGLLightIntensity intensity
);

#endif
