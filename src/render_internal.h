#ifndef PR3D_RENDER_INTERNAL_H
#define PR3D_RENDER_INTERNAL_H

#include "render.h"

struct PR3DRenderTexture
{
    unsigned int fbo;
    unsigned int texture;
};

/**
 * Creates a 320x180 render texture to draw the game to.
 *
 * @return The render texture struct.
 */
struct PR3DRenderTexture pr3d_create_render_texture(void);

/**
 * Enables the render texture fbo for rendering. This should be called before
 * doing any rendering to ensure everything is drawn to the internal resolution.
 *
 * The screen should be cleared after calling this using pr3d_clear_screen().
 */
void pr3d_enable_render_texture(unsigned int fbo);

/**
 * Disable the render texture fbo and render the render texture itself to the
 * default framebuffer.
 *
 * @param render_texture The ID of the texture to draw the screen quad to.
 * @param[in] screen_quad The quad mesh to draw the render texture onto.
 */
void pr3d_render_render_texture(
    unsigned int render_texture, struct PR3DMesh *screen_quad
);

#endif
