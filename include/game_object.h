#ifndef PRGL_GAME_OBJECT_H
#define PRGL_GAME_OBJECT_H

#include "cglm/types.h"

struct PRGLGameObject
{
    versor orientation; ///< Quaternion rotation.
    vec3 position;
    vec3 min_bounds;
    vec3 max_bounds;
    struct PR3DMesh *mesh;
};

#endif
