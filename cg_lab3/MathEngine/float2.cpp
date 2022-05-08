#include "float2.h"

float2::float2(float x_, float y_)
:x(x_), y(y_)
{
}

float2::float2(float scalar)
:x(scalar), y(scalar)
{
}

float2::float2(const float *data)
{
    if (data != nullptr) {
        x = data[0];
        y = data[1];
    }
}
