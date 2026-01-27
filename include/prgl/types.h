#ifndef PRGL_TYPES_H
#define PRGL_TYPES_H

struct PRGLMesh;

/**
 * @brief Defines the geometry of a 3D object.
 *
 * One mesh can be re-used to render many of the same object.
 */
typedef struct PRGLMesh *PRGLMeshHandle;

/**
 * @brief Stores an ID for a shader program.
 *
 * Note that shader initialization code may use the terms "shader" and
 * "shader_program" but generally after the initialization is complete a shader
 * program will be referred to as a "shader".
 */
typedef struct PRGLShader
{
    unsigned int id;
} PRGLShader;

/**
 * @brief Stores an ID for a texture.
 */
typedef struct PRGLTexture
{
    unsigned int id;
} PRGLTexture;

#endif
