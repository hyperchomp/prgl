#include "glad.h"

#include "texture.h"
#include "texture_internal.h"

#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "render.h"
#include "stb_image.h"
#include "types.h"

const PRGLTexture PRGL_NO_TEXTURE = {0};

PRGLTexture prgl_load_texture(const char *const filename)
{
    int width;
    int height;
    int num_color_channels;

    unsigned char *image =
        stbi_load(filename, &width, &height, &num_color_channels, 0);
    if (image == NULL)
    {
        fprintf(
            stderr, "prgl_load_texture: Failed to load image file \"%s\": %s\n",
            filename, stbi_failure_reason()
        );
        exit(EXIT_FAILURE);
    }

    GLuint texture;
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

    PRGLTexture final_texture = {.id = texture};
    return final_texture;
}

struct PRGLRenderTexture prgl_create_render_texture(void)
{
    // Create a framebuffer object
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create the texture for rendering to
    GLuint render_texture;
    glGenTextures(1, &render_texture);
    glBindTexture(GL_TEXTURE_2D, render_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, PRGL_RENDER_RESOLUTION[0],
        PRGL_RENDER_RESOLUTION[1], 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Attach the texture to the FBO
    glFramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0
    );

    // Attach a combined depth and stencil render buffer object
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, PRGL_RENDER_RESOLUTION[0],
        PRGL_RENDER_RESOLUTION[1]
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo
    );

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        fprintf(
            stderr, "prgl_create_render_texture: Tried to create framebuffer "
                    "but framebuffer is not complete!\n"
        );
        exit(EXIT_FAILURE);
    }

    struct PRGLRenderTexture render_tex = {
        .fbo = fbo, .texture = {.id = render_texture}
    };
    return render_tex;
}
