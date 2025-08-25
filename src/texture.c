#include "glad.h"
#include "texture.h"
#include "render.h"
#include "texture_internal.h"
#include "stb_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

unsigned int pr3d_load_texture(const char *const filename)
{
    int width;
    int height;
    int num_color_channels;

    unsigned char *image =
        stbi_load(filename, &width, &height, &num_color_channels, 0);
    if (image == NULL)
    {
        fprintf(
            stderr, "pr3d_load_texture: Failed to load image file \"%s\": %s\n",
            filename, stbi_failure_reason()
        );
        exit(EXIT_FAILURE);
    }

    unsigned int texture;
    glGenTextures(1, &texture);

    // Bind texture so OpenGL knows we're configuring this one
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate texture using loaded image
    GLenum format = (num_color_channels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(
        GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
        image
    );

    stbi_image_free(image);

    return texture;
}

struct PR3DRenderTexture pr3d_create_render_texture(void)
{
    // Create a framebuffer object
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create the texture for rendering to
    unsigned int render_texture;
    glGenTextures(1, &render_texture);
    glBindTexture(GL_TEXTURE_2D, render_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, PR3D_RENDER_RESOLUTION[0],
        PR3D_RENDER_RESOLUTION[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Attach the texture to the FBO
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0
    );

    // Attach a combined depth and stencil render buffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, PR3D_RENDER_RESOLUTION[0],
        PR3D_RENDER_RESOLUTION[1]
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(
            stderr, "pr3d_create_render_texture: Tried to create framebuffer "
                    "but framebuffer is not complete!\n"
        );
        exit(EXIT_FAILURE);
    }

    struct PR3DRenderTexture render_tex = {
        .fbo = fbo, .texture = render_texture
    };
    return render_tex;
}
