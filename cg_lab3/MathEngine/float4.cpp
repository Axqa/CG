#include "float4.h"

float4::float4(float x_, float y_, float z_, float w_)
:x(x_), y(y_), z(z_), w(w_)
{

}

float4::float4(const float3 &xyz, float w_)
:x(xyz.x), y(xyz.y), z(xyz.z), w(w_)
{

}

float4::float4(float x_, float y_, const float2 &zw)
:x(x_), y(y_), z(zw.x), w(zw.y)
{

}

float4::float4(float x_, const float2 &yz, float w_)
:x(x_), y(yz.x), z(yz.y), w(w_)
{

}

float4::float4(float x_, const float3 &yzw)
:x(x_), y(yzw.x), z(yzw.y), w(yzw.z)
{

}

float4::float4(const float2 &xy, float z_, float w_)
:x(xy.x), y(xy.y), z(z_), w(w_)
{

}

float4::float4(const float2 &xy, const float2 &zw)
:x(xy.x), y(xy.y), z(zw.x), w(zw.y)
{

}

float4::float4(const float *data)
{
    x = data[0];
    y = data[1];
    z = data[2];
    w = data[3];
}

void float4::Set(const float4 &rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}
