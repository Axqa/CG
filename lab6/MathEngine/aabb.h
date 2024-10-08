#ifndef AABB_H
#define AABB_H

#include "ncl.h"
#include "float3.h"

/// A 3D axis-aligned bounding box.
/** This data structure can be used to represent coarse bounds of objects, in situations where detailed triangle-level
    computations can be avoided. In physics systems, bounding boxes are used as an efficient early-out test for geometry
    intersection queries.

    The 'axis-aligned' part in the name means that the local axes of this bounding box are restricted to align with the
    axes of the world space coordinate system. This makes computations involving AABB's very fast, since AABB's cannot
    be arbitrarily oriented in the space with respect to each other.

    If you need to represent a box in 3D space with arbitrary orientation, see the class OBB. */
class AABB
{
public:

    /// Specifies the minimum extent of this AABB in the world space x, y and z axes.
    vec minPoint;
    /// Specifies the maximum extent of this AABB in the world space x, y and z axes. [similarOverload: minPoint]
    vec maxPoint;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members minPoint and maxPoint are undefined after creating a new AABB using this
        default constructor. Remember to assign to them before use.
        @see minPoint, maxPoint. */
    AABB() {}

    /// Constructs this AABB by specifying the minimum and maximum extending corners of the box.
    /** @see minPoint, maxPoint. */
    AABB(const vec &minPoint, const vec &maxPoint);

    FORCE_INLINE static int NumFaces() { return 6; }
    FORCE_INLINE static int NumEdges() { return 12; }
    FORCE_INLINE static int NumVertices() { return 8; }

    /// Returns the minimum world-space coordinate along the given axis.
    float MinX() const { return minPoint.x; }
    float MinY() const { return minPoint.y; }
    float MinZ() const { return minPoint.z; }
    /// Returns the maximum world-space coordinate along the given axis.
    float MaxX() const { return maxPoint.x; }
    float MaxY() const { return maxPoint.y; }
    float MaxZ() const { return maxPoint.z; }

    /// Sets this structure to a degenerate AABB that does not have any volume.
    /** This function is useful for initializing the AABB to "null" before a loop of calls to Enclose(),
        which incrementally expands the bounds of this AABB to enclose the given objects.
        @see Enclose(). */
    void SetNegativeInfinity();

    /// Sets this AABB by specifying its center and size.
    /** @param center The center point of this AABB.
        @param size A vector that specifies the size of this AABB in x, y and z directions.
        @see SetFrom(), FromCenterAndSize(). */
    void SetFromCenterAndSize(const vec &center, const vec &size);

    /// Sets this AABB to enclose the given set of points.
    /** @param pointArray A pointer to an array of points to enclose inside an AABB.
        @param numPoints The number of elements in the pointArray list.
        @see MinimalEnclosingAABB(). */
    void SetFrom(const vec *pointArray, int numPoints);

//	/// Tests if this AABB is finite.
//	/** @return True if the member variables of this AABB are valid floats and do not contain NaNs or infs, and false otherwise.
//		@see IsDegenerate(), minPoint, maxPoint. */
//	bool IsFinite() const;

    /// Tests if this AABB is degenerate.
    /** @return True if this AABB does not span a strictly positive volume.
        @see IsFinite(), Volume(), minPoint, maxPoint. */
    bool IsDegenerate() const;

    /// @return The center point of this AABB.
    vec CenterPoint() const;

    /// Returns an edge of this AABB.
    /** @param edgeIndex The index of the edge line segment to get, in the range [0, 11].
        @todo Specify which index generates which edge.
        @see PointInside(), CornerPoint(), PointOnEdge(), FaceCenterPoint(), FacePoint(). */
    LineSegment Edge(int edgeIndex) const;

    /// Returns a corner point of this AABB.
    /** This function generates one of the eight corner points of this AABB.
        @param cornerIndex The index of the corner point to generate, in the range [0, 7].
            The points are returned in the order 0: ---, 1: --+, 2: -+-, 3: -++, 4: +--, 5: +-+, 6: ++-, 7: +++. (corresponding the XYZ axis directions).
        @todo Draw which index generates which corner point.
        @see PointInside(), Edge(), PointOnEdge(), FaceCenterPoint(), FacePoint(), GetCornerPoints(). */
    vec CornerPoint(int cornerIndex) const;

    /// Computes an extreme point of this AABB in the given direction.
    /** An extreme point is a farthest point of this AABB in the given direction. Given a direction,
        this point is not necessarily unique.
        @param direction The direction vector of the direction to find the extreme point. This vector may
            be unnormalized, but may not be null.
        @return An extreme point of this AABB in the given direction. The returned point is always a
            corner point of this AABB.
        @see CornerPoint(). */
    vec ExtremePoint(const vec &direction) const;
    vec ExtremePoint(const vec &direction, float &projectionDistance) const;

    /// Returns a point on an edge of this AABB.
    /** @param edgeIndex The index of the edge to generate a point to, in the range [0, 11]. @todo Document which index generates which one.
        @param u A normalized value between [0,1]. This specifies the relative distance of the point along the edge.
        @see PointInside(), CornerPoint(), CornerPoint(), FaceCenterPoint(), FacePoint(). */
    vec PointOnEdge(int edgeIndex, float u) const;

    /// Returns the point at the center of the given face of this AABB.
    /** @param faceIndex The index of the AABB face to generate the point at. The valid range is [0, 5].
            This index corresponds to the planes in the order (-X, +X, -Y, +Y, -Z, +Z).
        @see PointInside(), CornerPoint(), PointOnEdge(), PointOnEdge(), FacePoint(). */
    vec FaceCenterPoint(int faceIndex) const;

    /// Generates a point at the surface of the given face of this AABB.
    /** @param faceIndex The index of the AABB face to generate the point at. The valid range is [0, 5].
            This index corresponds to the planes in the order (-X, +X, -Y, +Y, -Z, +Z).
        @param u A normalized value between [0, 1].
        @param v A normalized value between [0, 1].
        @see PointInside(), CornerPoint(), PointOnEdge(), PointOnEdge(), FaceCenterPoint(). */
    vec FacePoint(int faceIndex, float u, float v) const;

    /// Returns the surface normal direction vector the given face points towards.
    /** @param faceIndex The index of the AABB face to generate the point at. The valid range is [0, 5].
            This index corresponds to the planes in the order (-X, +X, -Y, +Y, -Z, +Z).
        @see FacePoint(), FacePlane(). */
    vec FaceNormal(int faceIndex) const;

    /// Computes the plane equation of the given face of this AABB.
    /** @param faceIndex The index of the AABB face. The valid range is [0, 5].
            This index corresponds to the planes in the order (-X, +X, -Y, +Y, -Z, +Z).
        @return The plane equation the specified face lies on. The normal of this plane points outwards from this AABB.
        @see FacePoint(), FaceNormal(), GetFacePlanes(). */
    Plane FacePlane(int faceIndex) const;

    /// Fills an array with all the eight corner points of this AABB.
    /** @param outPointArray [out] The array to write the points to. Must have space for 8 elements.
        @see CornerPoint(). */
    void GetCornerPoints(vec *outPointArray) const;

    /// Fills an array with all the six planes of this AABB.
    /** @param outPlaneArray [out] The array to write the planes to. Must have space for 6 elements.
        @see FacePlane(). */
    void GetFacePlanes(Plane *outPlaneArray) const;

    /// Generates an AABB that encloses the given point set.
    /** This function finds the smallest AABB that contains the given set of points.
        @param pointArray A pointer to an array of points to enclose inside an AABB.
        @param numPoints The number of elements in the pointArray list.
        @see SetFrom(). */
    static AABB MinimalEnclosingAABB(const vec *pointArray, int numPoints);
    AABB MinimalEnclosingAABB() const { return *this; }

    /// Creates a new AABB given is center position and size along the X, Y and Z axes.
    /** @see SetCenter(). */
    static AABB FromCenterAndSize(const vec &aabbCenterPos, const vec &aabbSize);

    /// Returns the side lengths of this AABB in x, y and z directions.
    /** The returned vector is equal to the diagonal vector of this AABB, i.e. it spans from the
        minimum corner of the AABB to the maximum corner of the AABB.
        @see HalfSize(), Diagonal(). */
    vec Size() const;

    /// [similarOverload: Size]
    /** Returns Size()/2.
        @see Size(), HalfDiagonal(). */
    vec HalfSize() const;

    /// Returns the diameter vector of this AABB.
    /** @note For AABB, Diagonal() and Size() are the same concept. These functions are provided for symmetry
        with the OBB class.
        @see Size(), HalfDiagonal(). */
    vec Diagonal() const { return Size(); }

    /// [similarOverload: Diagonal]
    /** Returns Diagonal()/2.
        @see Diagonal(), HalfSize(). */
    vec HalfDiagonal() const { return HalfSize(); }

    /// Computes the volume of this AABB.
    /** @see SurfaceArea(), IsDegenerate(). */
    float Volume() const;

    /// Computes the surface area of the faces of this AABB.
    /** @see Volume(). */
    float SurfaceArea() const;

    /// Applies a uniform scale to this AABB.
    /** This function scales this AABB structure in-place, using the given center point as the origin
        for the scaling operation.
        @param centerPoint Specifies the center of the scaling operation, in world space.
        @param scaleFactor The uniform scale factor to apply to each world space axis.
        @see Translate(), Transform(). */
    void Scale(const vec &centerPoint, float scaleFactor);

    /// Computes the closest point inside this AABB to the given point.
    /** If the target point lies inside this AABB, then that point is returned.
        @see Distance(), Contains(), Intersects().
        @todo Add ClosestPoint(Line/Ray/LineSegment/Plane/Triangle/Polygon/Circle/Disc/AABB/OBB/Sphere/Capsule/Frustum/Polyhedron). */
    vec ClosestPoint(const vec &targetPoint) const;

    /// Computes the distance between this AABB and the given object.
    /** This function finds the nearest pair of points on this and the given object, and computes their distance.
        If the two objects intersect, or one object is contained inside the other, the returned distance is zero.
        @todo Add AABB::Distance(Line/Ray/LineSegment/Plane/Triangle/Polygon/Circle/Disc/AABB/OBB/Capsule/Frustum/Polyhedron).
        @see Contains(), Intersects(), ClosestPoint(). */
    float Distance(const vec &point) const;

    /// Tests if the given object is fully contained inside this AABB.
    /** This function returns true if the given object lies inside this AABB, and false otherwise.
        @note The comparison is performed using less-or-equal, so the faces of this AABB count as being inside, but
            due to float inaccuracies, this cannot generally be relied upon.
        @todo Add Contains(Circle/Disc/Sphere/Capsule).
        @see Distance(), Intersects(), ClosestPoint(). */
    bool Contains(const vec &point) const;
    bool Contains(const LineSegment &lineSegment) const;
    bool Contains(const vec &aabbMinPoint, const vec &aabbMaxPoint) const;
    bool Contains(const AABB &aabb) const { return Contains(aabb.minPoint, aabb.maxPoint); }

    /// Tests whether this AABB and the given object intersect.
    /** Both objects are treated as "solid", meaning that if one of the objects is fully contained inside
        another, this function still returns true. (e.g. in case a line segment is contained inside this AABB,
        or this AABB is contained inside a Sphere, etc.)
        @param ray The first parameter of this function specifies the other object to test against.
        @param dNear [out] If specified, receives the parametric distance along the line denoting where the
            line entered this AABB.
        @param dFar [out] If specified, receives the parametric distance along the line denoting where the
            line exited this AABB.
        @see Contains(), Distance(), ClosestPoint().
        @note If you do not need the intersection intervals, you should call the functions without these
            parameters in the function signature for optimal performance.
        @todo Add Intersects(Circle/Disc). */
    bool Intersects(const Ray &ray, float &dNear, float &dFar) const;
    bool Intersects(const Ray &ray) const;
    bool Intersects(const Line &line, float &dNear, float &dFar) const;
    bool Intersects(const Line &line) const;
    bool Intersects(const LineSegment &lineSegment, float &dNear, float &dFar) const;
    bool Intersects(const LineSegment &lineSegment) const;
    bool Intersects(const AABB &aabb) const;

    /// Projects this AABB onto the given axis.
    /** @param axis The axis to project onto. This vector can be unnormalized.
        @param dMin [out] Returns the minimum extent of this AABB on the given axis.
        @param dMax [out] Returns the maximum extent of this AABB on the given axis. */
    void ProjectToAxis(const vec &axis, float &dMin, float &dMax) const;

    /// Expands this AABB to enclose the given object.
    /** This function computes an AABB that encloses both this AABB and the specified object, and stores the resulting
        AABB into this.
        @note The generated AABB is not necessarily the optimal enclosing AABB for this AABB and the given object. */
    void Enclose(const vec &point);
    void Enclose(const vec &aabbMinPoint, const vec &aabbMaxPoint);
    void Enclose(const LineSegment &lineSegment);
    void Enclose(const AABB &aabb) { Enclose(aabb.minPoint, aabb.maxPoint); }

    void Enclose(const vec *pointArray, int numPoints);

    /// Finds the set intersection of this and the given AABB.
    /** @return This function returns the AABB that is contained in both this and the given AABB.
        @todo Add Intersection(OBB/Polyhedron). */
    AABB Intersection(const AABB &aabb) const;

    /// Computes the intersection of a line, ray or line segment and an AABB.
    /** Based on "T. Kay, J. Kajiya. Ray Tracing Complex Scenes. SIGGRAPH 1986 vol 20, number 4. pp. 269-"
        http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm
        @param linePos The starting position of the line.
        @param lineDir The direction of the line. This direction vector must be normalized!
        @param tNear [in, out] For the test, the input line is treated as a line segment. Pass in the signed distance
            from the line origin to the start of the line. For a Line-AABB test, -FLOAT_INF is typically passed here.
            For a Ray-AABB test, 0.0f should be inputted. If intersection occurs, the signed distance from line origin
            to the line entry point in the AABB is returned here.
        @param tFar [in, out] Pass in the signed distance from the line origin to the end of the line. For Line-AABB and
            Ray-AABB tests, pass in FLOAT_INF. For a LineSegment-AABB test, pass in the length of the line segment here.
            If intersection occurs, the signed distance from line origin to the line exit point in the AABB
            is returned here.
        @return True if an intersection occurs, false otherwise.
        @note This is a low level utility function. It may be more convenient to use one of the AABB::Intersects()
            functions instead.
        @see Intersects(). */
    bool IntersectLineAABB(const vec &linePos, const vec &lineDir, float &tNear, float &tFar) const;

    bool IntersectLineAABB_CPP(const vec &linePos, const vec &lineDir, float &tNear, float &tFar) const;
};
#endif // AABB_H
