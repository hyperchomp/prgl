#include "glad.h"
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
