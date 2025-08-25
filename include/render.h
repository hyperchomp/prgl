#ifndef PRGL_RENDER_H
#define PRGL_RENDER_H

#include "mesh.h"
#include "cglm/types.h"

extern const vec2 PRGL_RENDER_RESOLUTION;

/**
 * A light which can be positioned in the world and emits a color.
 */
struct PRGLPointLight
{
    vec3 position;
    vec3 lightColor;
};

/**
 * Clears the screen using the given color. Values are from 0-1.
 *
 * @param r
 * @param g
 * @param b
 * @param a
 */
void prgl_clear_screen(float r, float g, float b, float a);

/**
 * Creates a quad for drawing the screen's render texture to.
 */
struct PRGLMesh *prgl_create_screen_quad(void);

/**
 * Renders a mesh to the screen at a position.
 *
 * @param[in] mesh
 * @param vec3 position The XYZ position to render the mesh at.
 * @param vec3 rotation_axis The XYZ axis of the rotation.
 * @param float degrees The angle of rotation in degrees.
 * @param vec3 scale The XYZ scale to apply to the mesh.
 */
void prgl_render_mesh(
    struct PRGLMesh *mesh, vec3 position, vec3 rotation_axis, float degrees,
    vec3 scale
);

/**
 * Renders a mesh to the screen at a 2D position.
 * Rotation will be about the Z axis.
 *
 * @param[in] mesh
 * @param vec2 position The XY position to render the mesh at.
 * @param float degrees The angle of rotation in degrees.
 * @param vec2 scale The XY scale to apply to the mesh.
 */
void prgl_render_mesh_2d(
    struct PRGLMesh *mesh, vec2 position, float rotation_degrees, vec2 scale
);

/**
 * Passes lighting data from lights to the active shader.
 *
 * @param struct[in] PRGLPointLight[] The point lights for the scene.
 */
void prgl_update_lighting(struct PRGLPointLight point_lights[], int num_lights);

#endif
