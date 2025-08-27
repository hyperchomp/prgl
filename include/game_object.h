#ifndef PRGL_GAME_OBJECT_H
#define PRGL_GAME_OBJECT_H

#include "cglm/types.h"
#include "mesh.h"

struct PRGLGameObject
{
    versor orientation; ///< Quaternion rotation.
    vec3 position;
    struct PRGLMesh *mesh;
};

#endif
