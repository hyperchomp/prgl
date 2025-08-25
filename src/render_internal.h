#ifndef PRGL_RENDER_INTERNAL_H
#define PRGL_RENDER_INTERNAL_H

#include "mesh.h"

/**
 * Enables the render texture fbo for rendering. This should be called before
 * doing any rendering to ensure everything is drawn to the internal resolution.
 *
 * The screen should be cleared after calling this using prgl_clear_screen().
 */
void prgl_enable_render_texture(unsigned int fbo);

/**
 * Disable the render texture fbo and render the render texture itself to the
 * default framebuffer.
 *
 * @param render_texture The ID of the texture to draw the screen quad to.
 * @param[in] screen_quad The quad mesh to draw the render texture onto.
 */
void prgl_render_render_texture(
    unsigned int render_texture, struct PRGLMesh *screen_quad
);

#endif
