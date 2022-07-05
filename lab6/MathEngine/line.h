#ifndef LINE_H
#define LINE_H

#include "ncl.h"
#include "float3.h"

class Line
{
public:
    /// Specifies the origin of this line.
    vec pos;

    /// The normalized direction vector of this ray. [similarOverload: pos]
    /** @note For proper functionality, this direction vector needs to always be normalized. If you set to this
        member manually, remember to make sure you only assign normalized direction vectors. */
    vec dir;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members pos and dir are undefined after creating a new Line using this
        default constructor. Remember to assign to them before use.
        @see pos, dir. */
    Line() {}

    /// Constructs a new line by explicitly specifying the member variables.
    /** @param pos The origin position of the line.
        @param dir The direction of the line. This vector must be normalized, this function will not normalize
            the vector for you (for performance reasons).
        @see pos, dir. */
    Line(const vec &pos, const vec &dir);

    /// Converts a Ray to a Line.
    /** This conversion simply copies the members pos and dir over from the given Ray to this Line.
        This means that the new Line starts at the same position, but extends to two directions in space,
        instead of one.
        @see class Ray, ToRay(). */
    explicit Line(const Ray &ray);

    /// Converts a LineSegment to a Line.
    /** This constructor sets pos = lineSegment.a, and dir = (lineSegment.b - lineSegment.a).Normalized().
        @see class LineSegment, ToLineSegment(). */
    explicit Line(const LineSegment &lineSegment);

    /// Gets a point along the line at the given distance.
    /** Use this function to convert a 1D parametric point along the Line to a 3D point in the linear space.
        @param distance The point to compute. GetPoint(0) will return pos. GetPoint(t) will return a point
            at distance |t| from pos, towards the direction specified by dir. If a negative value is specified,
            a point towards the direction -dir is returned.
        @return pos + distance * dir.
        @see pos, dir. */
    vec GetPoint(float distance) const;

    /// Computes the distance between this line and the given object.
    /** This function finds the nearest pair of points on this and the given object, and computes their distance.
        If the two objects intersect, or one object is contained inside the other, the returned distance is zero.
        @param d [out] If specified, receives the parametric distance along this line that
            specifies the closest point on this line to the given object. The value returned here can be negative.
        @see Contains(), Intersects(), ClosestPoint(), GetPoint(). */
    float Distance(const vec &point) const { float d; return Distance(point, d); }
    float Distance(const vec &point, float &d) const;
    /** @param d2 [out] If specified, receives the parametric distance along the other line that specifies the
        closest point on that line to this line. The value returned here can be negative. */
    float Distance(const Ray &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Ray &other, float &d, float &d2) const;
    float Distance(const Line &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const Line &other, float &d, float &d2) const;
    float Distance(const LineSegment &other) const { float d, d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d) const { float d2; return Distance(other, d, d2); }
    float Distance(const LineSegment &other, float &d, float &d2) const;

    /// Computes the closest point on this line to the given object.
    /** If the other object intersects this line, this function will return an arbitrary point inside
        the region of intersection.
        @param d [out] If specified, receives the parametric distance along this line that
            specifies the closest point on this line to the given object. The value returned here can be negative.
        @see Contains(), Distance(), Intersects(), GetPoint(). */
    vec ClosestPoint(const vec &targetPoint) const { float d; return ClosestPoint(targetPoint, d); }
    vec ClosestPoint(const vec &targetPoint, float &d) const;
    /** @param d2 [out] If specified, receives the parametric distance along the other line that specifies the
        closest point on that line to this line. The value returned here can be negative. */
    vec ClosestPoint(const Ray &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Ray &other, float &d, float &d2) const;
    vec ClosestPoint(const Line &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const Line &other, float &d, float &d2) const;
    vec ClosestPoint(const LineSegment &other) const { float d, d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d) const { float d2; return ClosestPoint(other, d, d2); }
    vec ClosestPoint(const LineSegment &other, float &d, float &d2) const;

    /// Converts this Line to a Ray.
    /** The pos and dir members of the returned Ray will be equal to this Line. The only difference is
        that a Line extends to infinity in two directions, whereas the returned Ray spans only in
        the positive direction.
        @see dir, Line::Line, class Ray, ToLineSegment(). */
    Ray ToRay() const;

    /// Converts this Line to a LineSegment.
    /** @param d Specifies the position of the other endpoint along this Line. This parameter may be negative.
        @return A LineSegment with point a at pos, and point b at pos + d * dir.
        @see pos, dir, Line::Line, class LineSegment, ToRay(). */
    LineSegment ToLineSegment(float d) const;

    /// Converts this Line to a LineSegment.
    /** @param dStart Specifies the position of the first endpoint along this Line. This parameter may be negative,
        in which case the starting point lies to the opposite direction of the Line.
        @param dEnd Specifies the position of the second endpoint along this Line. This parameter may also be negative.
        @return A LineSegment with point a at pos + dStart * dir, and point b at pos + dEnd * dir.
        @see pos, dir, Line::Line, class LineSegment, ToLine(). */
    LineSegment ToLineSegment(float dStart, float dEnd) const;

    static void ClosestPointLineLine(const vec &start0, const vec &dir0, const vec &start1, const vec &dir1, float &d, float &d2);

};

#endif // LINE_H
