#include "mathx.h"

float pr3d_clampf(float val, float min, float max)
{
    const float clamped = val < min ? min : val;
    return clamped > max ? max : clamped;
}
