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

    /// Reverses the direction of the plane normal, while still representing the same set of points.
    /** This function sets normal = -normal and d = -d for this plane.
        @see normal, d. */
    void ReverseNormal();

    /// Returns a point on this plane.
    /** @note This point has the special property that the line passing through the vector space origin (0,0,0)
            and the returned point is perpendicular to this plane (directed towards the normal vector of this plane).
        @see Point(). */
    vec PointOnPlane() const;

    /// Returns a point on this plane, parameterized at the given coordinates.
    /** The basis directions for U and V are arbitrarily (but consistently) defined.
        Calling Point(0,0) is the same as calling PointOnPlane().
        @see PointOnPlane(). */
    vec Point(float u, float v) const;

    /// Returns the distance of this plane to the given object.
    /** If the given object intersects or lies in this plane, then the returned distance is zero.
        @note This function always returns a positive distance value, even when the given object lies on the negative side
            of this plane. See the SignedDistance() function to produce a distance value that differentiates between the
            front and back sides of this plane.
        @see SignedDistance(), Intersects(), Contains(). */
    float Distance(const vec &point) const;
    /// Returns the signed distance of this plane to the given point.
    /** If this function returns a negative value, the given point lies in the negative halfspace of this plane.
        Conversely, if a positive value is returned, then the given point lies in the positive halfspace of this plane.
        @see Distance(), IsOnPositiveSide(), AreOnSameSide(). */
    float SignedDistance(const vec &point) const;


    /// Returns a point on this plane, parameterized at the given coordinates.
    /** The basis directions for U and V are arbitrarily (but consistently) defined.
        @param referenceOrigin A point that defines an origin for the returned points. This point does not have to lie
            on this plane.
        Calling Point(0, 0, referenceOrigin) returns the point referenceOrigin projected onto this plane.
        Calling Point(u, v) is the same as calling Point(u, v, PointOnPlane()).
        @see PointOnPlane(). */
//	vec Point(float u, float v, const vec &referenceOrigin) const;

    /// Translates this Plane in world space.
    /** @param offset The amount of displacement to apply to this Plane, in world space coordinates.
        @see Transform(). */
    void Translate(const vec &offset);

    MatrixF ProjectionMatrix(float3 camPoint, float perspectiveDist, bool addPersp);

    Plane &operator=(const Plane& p);


private:
    MatrixF projMatrix;
    void CalculateProjMatrix(float3 camPoint, float perspectiveDist, bool addPersp);
    void CalculateProjMatrix2(float3 camPoint, float perspectiveDist, bool addPersp);
};

#endif // PLANE_H
