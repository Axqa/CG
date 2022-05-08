#ifndef PLANE_H
#define PLANE_H

#include "ncl.h"
#include "float3.h"
#include "matrix.h"

class Plane
{
public:
    float3 normal;
    float d;

public:
    Plane();
    /// Constructs a plane by directly specifying the normal and distance parameters.
    /** @param normal The direction the plane is facing. This vector must have been normalized in advance.
        @param d The offset of this plane from the origin. The value -d gives the signed distance of this plane from the origin.
        @see normal, d. */
    Plane(const float3 &normal, float d);
    /// Constructs a plane by specifying three points on the plane.
    /** The normal of the plane will point to
        the halfspace from which the points are observed to be oriented in counter-clockwise order.
        @note The points v1, v2 and v3 must not all lie on the same line.
        @see Set(). */
    Plane(const float3 &v1, const float3 &v2, const float3 &v3);
    /// Constructs a plane by specifying a single point on the plane, and the surface normal.
    /** @param normal The direction the plane is facing. This vector must have been normalized in advance.
        @see Set(). */
    Plane(const float3 &point, const float3 &normal);

    /// Sets this plane by specifying three points on the plane.
    /** The normal of the plane will point to the halfspace from which the points are observed to be oriented in
        counter-clockwise order.
        @note The points v1, v2 and v3 must not all lie on the same line. */
    void Set(const float3 &v1, const float3 &v2, const float3 &v3);
    /// Sets this plane by specifying a single point on the plane, and the surface normal.
    /** @param normal The direction the plane is facing. This vector must have been normalized in advance. */
    void Set(const float3 &point, const float3 &normal);


    MatrixF ProjectionMatrix();
};

#endif // PLANE_H
