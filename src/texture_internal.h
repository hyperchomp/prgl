#ifndef PRGL_TEXTURE_INTERNAL_H
#define PRGL_TEXTURE_INTERNAL_H

struct PRGLRenderTexture
{
    unsigned int fbo;
    unsigned int texture;
};

/**
 * Creates a 320x180 render texture to draw the game to.
 *
 * @return The render texture struct.
 */
struct PRGLRenderTexture prgl_create_render_texture(void);

#endif
