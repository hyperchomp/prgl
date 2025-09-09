#ifndef PRGL_TEXTURE_INTERNAL_H
#define PRGL_TEXTURE_INTERNAL_H

#include "glad.h"
#include "types.h"

struct PRGLRenderTexture
{
    GLuint fbo;
    PRGLTexture texture;
};

/**
 * Creates a 320x180 render texture to draw the game to.
 *
 * @return The render texture struct.
 */
struct PRGLRenderTexture prgl_create_render_texture(void);

#endif
