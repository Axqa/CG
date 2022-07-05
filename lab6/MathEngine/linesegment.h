#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include "ncl.h"
#include "float3.h"

class LineSegment
{
public:
    /// The starting point of this line segment.
    vec a;
    /// The end point of this line segment. [similarOverload: a]
    vec b;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members a and b are undefined after creating a new LineSegment using this
        default constructor. Remember to assign to them before use.
        @see a, b. */
    LineSegment() {}

    /// Constructs a line segment through the given end points.
    /** @see a, b. */
    LineSegment(const vec &a, const vec &b);

    /// Constructs a line segment from a ray or a line.
    /** This constructor takes the ray/line origin position as the starting point of this line segment, and defines the end point
        of the line segment using the given distance parameter.
        @param d The distance along the ray/line for the end point of this line segment. This will set b = ray.pos + d * ray.dir
            as the end point. When converting a ray to a line segment, it is possible to pass in a d value < 0, but in that case
            the resulting line segment will not lie on the ray.
        @see a, b, classes Ray, Line, Line::GetPoint(), Ray::GetPoint(). */
    explicit LineSegment(const Ray &ray, float d);
    explicit LineSegment(const Line &line, float d);

    /// Returns a point on the line.
    /** @param d The normalized distance along the line segment to compute. If a value in the range [0, 1] is passed, then the
            returned point lies along this line segment. If some other value is specified, the returned point lies on the
            line defined by this line segment, but not inside the interval from a to b.
        @note The meaning of d here differs from Line::GetPoint and Ray::GetPoint. For the class LineSegment,
            GetPoint(0) returns a, and GetPoint(1) returns b. This means that GetPoint(1) will not generally be exactly one unit
            away from the starting point of this line segment, as is the case with Line and Ray.
        @return (1-d)*a + d*b.
        @see a, b, Line::GetPoint(), Ray::GetPoint(). */
    vec GetPoint(float d) const;

    /// Returns the center point of this line segment.
    /** This function is the same as calling GetPoint(0.5f), but provided here as conveniency.
        @see GetPoint(). */
    vec CenterPoint() const;

    /// Reverses the direction of this line segment.
    /** This function swaps the start and end points of this line segment so that it runs from b to a.
        This does not have an effect on the set of points represented by this line segment, but it reverses
        the direction of the vector returned by Dir().
        @note This function operates in-place.
        @see a, b, Dir(). */
    void Reverse();

    /// Returns the normalized direction vector that points in the direction a->b.
    /** @note The returned vector is normalized, meaning that its length is 1, not |b-a|.
        @see a, b. */
    vec Dir() const;

    /// Computes an extreme point of this LineSegment in the given direction.
    /** An extreme point is a farthest point along this LineSegment in the given direction. Given a direction,
        this point is not necessarily unique.
        @param direction The direction vector of the direction to find the extreme point. This vector may
            be unnormalized, but may not be null.
        @return An extreme point of this LineSegment in the given direction. The returned point is always
            either a or b.
        @see a, b.*/
    vec ExtremePoint(const vec &direction) const;
    vec ExtremePoint(const vec &direction, float &projectionDistance) const;

    /// Computes the length of this line segment.
    /** @return |b-a|.
        @see a, b. */
    float Length() const;
    /// Computes the squared length of this line segment.
    /** Calling this function is faster than calling Length(), since this function avoids computing a square root.
        If you only need to compare lengths to each other and are not interested in the actual length values,
        you can compare by using LengthSq(), instead of Length(), since Sqrt() is an order-preserving
        (monotonous and non-decreasing) function. [similarOverload: Length] */
    float LengthSq() const;

    /// Computes the closest point on this line segment to the given object.
    /** @param d [out] If specified, this parameter receives the normalized distance along
            this line segment which specifies the closest point on this line segment to
            the specified point.
        @return The closest point on this line segment to the given object.
        @see Contains(), Distance(), Intersects(). */
    vec ClosestPoint(const vec &point) const { float d; return ClosestPoint(point, d); }
    vec ClosestPoint(const vec &point, float &d) const;
    /** @param d2 [out] If specified, this parameter receives the (normalized, in case of line segment)
            distance along the other line object which specifies the closest point on that line to
            this line segment. */
    vec ClosestPoint(const Ray &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d, float &d2) const;
    vec ClosestPoint(const Line &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d, float &d2) const;
    vec ClosestPoint(const LineSegment &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d, float &d2) const;

    /// Computes the distance between this line segment and the given object.
    /** @param d [out] If specified, this parameter receives the normalized distance along
            this line segment which specifies the closest point on this line segment to
            the specified point.
        @return The distance between this line segment and the given object.
        @see Constains(), ClosestPoint(), Intersects(). */
    float Distance(const vec &point) const { float d; return Distance(point, d); }
    float Distance(const vec &point, float &d) const;
    /** @param d2 [out] If specified, this parameter receives the (normalized, in case of line segment)
            distance along the other line object which specifies the closest point on that line to
            this line segment. */
    float Distance(const Ray &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d, float &d2) const;
    float Distance(const Line &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d, float &d2) const;
    float Distance(const LineSegment &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d, float &d2) const;

    float DistanceSq(const vec &point) const;
    float DistanceSq(const LineSegment &other) const;

    /** @param epsilon If testing intersection between two line segments, a distance threshold value is used to account
            for floating-point inaccuracies. */
    bool Intersects(const LineSegment &lineSegment, float epsilon = 1e-3f) const;

    /// Converts this LineSegment to a Ray.
    /** The pos member of the returned Ray will be equal to a, and the dir member equal to Dir().
        @see class Ray, ToLine(). */
    Ray ToRay() const;
    /// Converts this LineSegment to a Line.
    /** The pos member of the returned Line will be equal to a, and the dir member equal to Dir().
        @see class Line, ToRay(). */
    Line ToLine() const;
};

#endif // LINESEGMENT_H
