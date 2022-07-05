#ifndef LIGHT_H
#define LIGHT_H

#include "object3d.h"
#include "../MathEngine/float3.h"
#include "../MathEngine/plane.h"
#include "material.h"

class Light
{
public:
    Light();

    virtual float IntensityForPointOnPlane(vec p, Plane plane, vec cam, Material &mat) = 0;
};

#endif // LIGHT_H
