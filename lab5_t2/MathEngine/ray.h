#ifndef RAY_H
#define RAY_H

#include "ncl.h"
#include "float3.h"

class Ray
{
public:
    /// Specifies the origin of this ray.
    float3 pos;

    /// The normalized direction vector of this ray. [similarOverload: pos]
    /** @note For proper functionality, this direction vector needs to always be normalized. If you set to this
        member manually, remember to make sure you only assign normalized direction vectors. */
    float3 dir;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members pos and dir are undefined after creating a new Ray using this
        default constructor. Remember to assign to them before use.
        @see pos, dir. */
    Ray() {}

    /// Constructs a new ray by explicitly specifying the member variables.
    /** @param pos The origin position of the ray.
        @param dir The direction of the ray. This vector must be normalized, this function will not normalize
            the vector for you (for performance reasons).
        @see pos, dir. */
    Ray(const float3 &pos, const float3 &dir);

    /// Gets a point along the ray at the given distance.
    /** Use this function to convert a 1D parametric point along the Ray to a 3D point in the linear space.
        @param distance The point to compute. GetPoint(0) will return pos. GetPoint(t) will return a point
            at distance |t| from pos. Passing in negative values is allowed, but in that case, the
            returned point does not actually lie on this Ray.
        @return pos + distance * dir.
        @see pos, dir. */
    float3 GetPoint(float distance) const;

    /// Computes the distance between this ray and the given object.
    /** This function finds the nearest pair of points on this and the given object, and computes their distance.
        If the two objects intersect, or one object is contained inside the other, the returned distance is zero.
        @param d [out] If specified, receives the parametric distance along this ray that
            specifies the closest point on this ray to the given object. The value returned here can be negative.
        @see Contains(), Intersects(), ClosestPoint(), GetPoint(). */
    float Distance(const float3 &point) const { float d; return Distance(point, d); }
    float Distance(const float3 &point, float &d) const;

    float3 ClosestPoint(const float3 &targetPoint) const { float d; return ClosestPoint(targetPoint, d); }
    float3 ClosestPoint(const float3 &targetPoint, float &d) const;

    /** @param d2 [out] If specified, receives the parametric distance along the other line that specifies the
        closest point on that line to this ray. The value returned here can be negative. */
    float Distance(const Ray &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d, float &d2) const;
    float Distance(const Line &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d, float &d2) const;
    float Distance(const LineSegment &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d, float &d2) const;

    /** @param d2 [out] If specified, receives the parametric distance along the other line that specifies the
        closest point on that line to this ray. The value returned here can be negative. */
    vec ClosestPoint(const Ray &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d, float &d2) const;
    vec ClosestPoint(const Line &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d, float &d2) const;
    vec ClosestPoint(const LineSegment &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d, float &d2) const;

};

#endif // RAY_H
