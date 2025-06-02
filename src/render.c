#include "render.h"
#include <glad/glad.h>
#include <GL/gl.h>

void pr3d_clear_screen(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}
