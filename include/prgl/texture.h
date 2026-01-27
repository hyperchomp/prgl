#ifndef PRGL_TEXTURE_H
#define PRGL_TEXTURE_H

#include "types.h"

/**
 * @brief Empty texture for quick mesh initialization when no texture is wanted.
 *
 * A zero'd PRGLTexture struct. When passed for mesh initialization this will
 * set the texture ID for the mesh to zero causing it to have no texture.
 */
extern const PRGLTexture PRGL_NO_TEXTURE;

PRGLTexture prgl_load_texture(const char *const filename);

#endif
