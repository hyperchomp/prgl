#ifndef PR3D_TEXTURE_INTERNAL_H
#define PR3D_TEXTURE_INTERNAL_H

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

#endif
