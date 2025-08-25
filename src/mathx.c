#include "mathx.h"

float prgl_clampf(float val, float min, float max)
{
    const float clamped = val < min ? min : val;
    return clamped > max ? max : clamped;
}
