#ifndef FLOAT2_H
#define FLOAT2_H

#include "../commondec.h"
#include "QDebug"

class float2
{
public:
    enum
    {
        /// Specifies the number of elements in this vector.
        Size = 2
    };
    /// The x component.
    /** A float2 is 8 bytes in size. This element lies in the memory offsets 0-3 of this class. */
    float x;
    /// The y component. [similarOverload: x]
    /** This element is packed to the memory offsets 4-7 of this class. */
    float y;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members x and y are both undefined after creating a new float2 using
        this default constructor. Remember to assign to them before use.
        @see x, y. */
    float2() {}

    /// The float2 copy constructor.
    float2(const float2 &rhs) { x = rhs.x; y = rhs.y; }

    /// Constructs a new float2 with the value (x, y).
    /** @see x, y. */
    float2(float x, float y);

    /// Constructs a new float2 with the value (scalar, scalar).
    /** @see x, y. */
    explicit float2(float scalar);

    /// Constructs this float2 from a C array, to the value (data[0], data[1]).
    /** @param data An array containing two elements for x and y. This pointer may not be null. */
    explicit float2(const float *data);

    /// Casts this float2 to a C array.
    /** This function does not allocate new memory or make a copy of this float2. This function simply
        returns a C pointer view to this data structure. Use ptr()[0] to access the x component of this float2
        and ptr()[1] to access the y component.
        @note Since the returned pointer points to this class, do not dereference the pointer after this
            float2 has been deleted. You should never store a copy of the returned pointer.
        @note This function is provided for compatibility with other APIs which require raw C pointer access
            to vectors. Avoid using this function in general, and instead always use the operator []
            or the At() function to access the elements of this vector by index.
        @return A pointer to the first float element of this class. The data is contiguous in memory.
        @see operator [](), At(). */
    FORCE_INLINE float *ptr() { return &x; }
    FORCE_INLINE const float *ptr() const { return &x; }

    /// Accesses an element of this vector using array notation.
    /** @param index The element to get. Pass in 0 for x and 1 for y.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec[1] = 10.f; would set the y-component of this vector.
        @see ptr(), At(). */
    FORCE_INLINE float &operator [](int index) { return At(index); }
    FORCE_INLINE CONST_WIN32 float operator [](int index) const { return At(index); }

    /// Accesses an element of this vector.
    /** @param index The element to get. Pass in 0 for x and 1 for y.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec.At(1) = 10.f; would set the y-component of this vector.
        @see ptr(), operator [](). */
    FORCE_INLINE CONST_WIN32 float At(int index) const
    {
        assert(index >= 0);
        assert(index < Size);
        return ptr()[index];
    }

    FORCE_INLINE float &At(int index)
    {
        assert(index >= 0);
        assert(index < Size);
        return ptr()[index];
    }

    /// Adds two vectors. [indexTitle: operators +,-,*,/]
    /** This function is identical to the member function Add().
        @return float2(x + v.x, y + v.y); */
    float2 operator +(const float2 &v) const;
    /// Performs an unary negation of this vector. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Neg().
        @return float2(-x, -y). */
    float2 operator -() const;
    /// Subtracts the given vector from this vector. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Sub().
        @return float2(x - v.x, y - v.y); */
    float2 operator -(const float2 &v) const;
    /// Multiplies this vector by a scalar. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Mul().
        @return float2(x * scalar, y * scalar); */
    float2 operator *(float scalar) const;
    /// Divides this vector by a scalar. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Div().
        @return float2(x / scalar, y / scalar); */
    float2 operator /(float scalar) const;
    /// Unary operator + allows this structure to be used in an expression '+x'.
    float2 operator +() const { return *this; }

    /// Assigns a vector to another.
    /** @return A reference to this. */
    float2 &operator =(const float2 &v);
    /// Adds a vector to this vector, in-place. [indexTitle: operators +=,-=,*=,/=]
    /** @return A reference to this. */
    float2 &operator +=(const float2 &v);
    /// Subtracts a vector from this vector, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float2 &operator -=(const float2 &v);
    /// Multiplies this vector by a scalar, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float2 &operator *=(float scalar);
    /// Divides this vector by a scalar, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float2 &operator /=(float scalar);

    /// Adds a vector to this vector. [IndexTitle: Add/Sub/Mul/Div]
    /// @return (x+v.x, y+v.y).
    float2 Add(const float2 &v) const { return *this + v; }

    /// Adds the vector (s,s) to this vector.
    /// @note Mathematically, the addition of a vector and scalar is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x+s, y+s).
    float2 Add(float s) const;

    /// Subtracts a vector from this vector. [similarOverload: Add] [hideIndex]
    /// @return (x-v.x, y-v.y).
    float2 Sub(const float2 &v) const { return *this - v; }

    /// Subtracts the vector (s,s) from this vector. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the subtraction of a vector by a scalar is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x-s, y-s).
    float2 Sub(float s) const;

    /// Subtracts this vector from the vector (s,s). [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the subtraction of a scalar by a vector is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (s-x, s-y).
    float2 SubLeft(float s) const;

    /// Multiplies this vector by a vector, element-wise. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the multiplication of two vectors is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x*v.x, y*v.y).
    float2 Mul(const float2 &v) const;

    /// Multiplies this vector by a scalar. [similarOverload: Add] [hideIndex]
    /// @return (x*s, y*s).
    float2 Mul(float s) const { return *this * s; }

    /// Divides this vector by a vector, element-wise. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the division of two vectors is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x/v.x, y/v.y).
    float2 Div(const float2 &v) const;

    /// Divides this vector by a scalar. [similarOverload: Add] [hideIndex]
    /// @return (x/s, y/s).
    float2 Div(float s) const { return *this / s; }

    /// Sets all elements of this vector.
    /** @see x, y, At().. */
    void Set(float x, float y);
    
    /// Returns the aimed angle direction of this vector, in radians.
    /** The aimed angle of a 2D vector corresponds to the theta part (or azimuth) of the polar coordinate representation of this vector. Essentially,
        describes the direction this vector is pointing at. A vector pointing towards +X returns 0, vector pointing towards +Y returns pi/2, vector
        pointing towards -X returns pi, and a vector pointing towards -Y returns -pi/2 (equal to 3pi/2).
        @note This vector does not need to be normalized for this function to work, but it DOES need to be non-zero (unlike the function ToPolarCoordinates).
        @return The aimed angle in the range ]-pi/2, pi/2].
        @see ToPolarCoordinates, FromPolarCoordinates, SetFromPolarCoordinates. */
    float AimedAngle() const;

    /// Computes the length of this vector.
    /** @return Sqrt(x*x + y*y).
        @see LengthSq(), Distance(), DistanceSq(). */
    float Length() const;

    /// Computes the squared length of this vector.
    /** Calling this function is faster than calling Length(), since this function avoids computing a square root.
        If you only need to compare lengths to each other, but are not interested in the actual length values,
        you can compare by using LengthSq(), instead of Length(), since Sqrt() is an order-preserving
        (monotonous and non-decreasing) function.
        @return x*x + y*y.
        @see LengthSq(), Distance(), DistanceSq(). */
    float LengthSq() const;

    /// Normalizes this float2.
    /** In the case of failure, this vector is set to (1, 0), so calling this function will never result in an
        unnormalized vector.
        @note If this function fails to normalize the vector, no error message is printed, the vector is set to (1,0) and
            an error code 0 is returned. This is different than the behavior of the Normalized() function, which prints an
            error if normalization fails.
        @note This function operates in-place.
        @return The old length of this vector, or 0 if normalization failed.
        @see Normalized(). */
    float Normalize();

    /// Returns a normalized copy of this vector.
    /** @note If the vector is zero and cannot be normalized, the vector (1, 0) is returned, and an error message is printed.
            If you do not want to generate an error message on failure, but want to handle the failure yourself, use the
            Normalize() function instead.
        @see Normalize(). */
    float2 Normalized() const;

    /// Scales this vector so that its new length is as given.
    /** Calling this function is effectively the same as normalizing the vector first and then multiplying by newLength.
        In the case of failure, this vector is set to (newLength, 0), so calling this function will never result in an
        unnormalized vector.
        @note This function operates in-place.
        @return The old length of this vector. If this function returns 0, the scaling failed, and this vector is arbitrarily
            reset to (newLength, 0). In case of failure, no error message is generated. You are expected to handle the failure
            yourself.
        @see ScaledToLength(). */
    float ScaleToLength(float newLength);

    /// Returns a scaled copy of this vector which has its new length as given.
    /** This function assumes the length of this vector is not zero. In the case of failure, an error message is printed,
        and the vector (newLength, 0) is returned.
        @see ScaleToLength(). */
    float2 ScaledToLength(float newLength) const;

    /// Computes the distance between this and the given float2.
    /** @see DistanceSq(), Length(), LengthSq(). */
    float Distance(const float2 &point) const;

    /// Computes the squared distance between this and the given point.
    /** Calling this function is faster than calling Distance(), since this function avoids computing a square root.
        If you only need to compare distances to each other, but are not interested in the actual distance values,
        you can compare by using DistanceSq(), instead of Distance(), since Sqrt() is an order-preserving
        (monotonous and non-decreasing) function.
        @see Distance(), Length(), LengthSq(). */
    float DistanceSq(const float2 &point) const;

    /// Computes the dot product of this and the given vector.
    /** The dot product has a geometric interpretation of measuring how close two direction vectors are to pointing
        in the same direction, computing angles between vectors, or the length of a projection of one vector to another.
        @return x*v.x + y*v.y.
        @see AngleBetween(), ProjectTo(), ProjectToNorm(), Perp(), PerpDot(). */
    float Dot(const float2 &v) const;

    /// Returns this vector with the "perp" operator applied to it.
    /** The perp operator rotates a vector 90 degrees ccw (around the "z axis"), i.e.
        for a 2D vector (x,y), this function returns the vector (-y, x).
        @note This function is identical to Rotated90CCW().
        @return (-y, x). The returned vector is perpendicular to this vector.
        @see PerpDot(), Rotated90CCW(). */
    float2 Perp() const;

    /// Tests if the length of this vector is one, up to the given epsilon.
    /** @see IsZero(), IsFinite(), IsPerpendicular(). */
    bool IsNormalized(float epsilonSq = 1e-5f) const;

    /// Tests if this is the null vector, up to the given epsilon.
    /** @see IsNormalized(), IsFinite(), IsPerpendicular(). */
    bool IsZero(float epsilonSq = 1e-6f) const;

    /// Tests if this vector contains valid finite elements.
    /** @see IsNormalized(), IsZero(), IsPerpendicular(). */
    bool IsFinite() const;

    /// Tests if two vectors are perpendicular to each other.
    /** @see IsNormalized(), IsZero(), IsPerpendicular(), Equals(). */
    bool IsPerpendicular(const float2 &other, float epsilonSq = 1e-5f) const;


    /// Rotates this vector 90 degrees clock-wise.
    /** This rotation is interpreted in a coordinate system on a plane where +x extends to the right, and +y extends upwards.
        @see Perp(), Rotated90CW(), Rotate90CCW(), Rotated90CCW(). */
    void Rotate90CW();

    /// Returns a vector that is perpendicular to this vector (rotated 90 degrees clock-wise).
    /** @note This function is identical to Perp().
        @see Perp(), Rotate90CW(), Rotate90CCW(), Rotated90CCW(). */
    float2 Rotated90CW() const;

    /// Rotates this vector 90 degrees counterclock-wise .
    /// This is in a coordinate system on a plane where +x extends to the right, and +y extends upwards.
    /** @see Perp(), Rotate90CW(), Rotated90CW(), Rotated90CCW(). */
    void Rotate90CCW();

    /// Returns a vector that is perpendicular to this vector (rotated 90 degrees counter-clock-wise).
    /** @see Perp(), Rotate90CW(), Rotated90CW(), Rotate90CCW(). */
    float2 Rotated90CCW() const;

    /// Returns the angle between this vector and the specified vector, in radians.
    /** @note This function takes into account that this vector or the other vector can be unnormalized, and normalizes the computations.
            If you are computing the angle between two normalized vectors, it is better to use AngleBetweenNorm().
        @see AngleBetweenNorm(). */
    float AngleBetween(const float2 &other) const;

    /// Returns the angle between this vector and the specified normalized vector, in radians.
    /** @param normalizedVector The direction vector to compute the angle against. This vector must be normalized.
        @note This vector must be normalized to call this function.
        @see AngleBetween(). */
    float AngleBetweenNorm(const float2 &normalizedVector) const;

    /// Linearly interpolates between this and the vector b.
    /** @param b The target endpoint to lerp towards to.
        @param t The interpolation weight, in the range [0, 1].
        @return Lerp(b, 0) returns this vector, Lerp(b, 1) returns the vector b.
            Lerp(b, 0.5) returns the vector half-way in between the two vectors, and so on.
            Lerp(b, t) returns (1-t)*this + t*b. */
    float2 Lerp(const float2 &b, float t) const;
    /// This function is the same as calling a.Lerp(b, t).
    static float2 Lerp(const float2 &a, const float2 &b, float t);

    float Cross(const float2 &v) const;

    /// Specifies a compile-time constant float2 with value (0, 0).
    /** @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 zero;
    /// Specifies a compile-time constant float2 with value (1, 1). [similarOverload: zero]
    /** @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 one;
    /// Specifies a compile-time constant float2 with value (1, 0).
    /** @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 unitX;
    /// Specifies a compile-time constant float2 with value (0, 1). [similarOverload: unitX]
    /** @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 unitY;
    /// A compile-time constant float2 with value (NaN, NaN).
    /** For this constant, each element has the value of quiet NaN, or Not-A-Number.
        @note Never compare a float2 to this value! Due to how IEEE floats work, "nan == nan" returns false!
              That is, nothing is equal to NaN, not even NaN itself!
        @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 nan;
    /// A compile-time constant float2 with value (+infinity, +infinity). [similarOverload: nan]
    /** @note Due to static data initialization order being undefined in C++, do NOT use this
            member to initialize other static data in other compilation units! */
    static const float2 inf;

    operator QString() const { return "float2("+ QString::number(x)+", "+QString::number(y)+")"; }

};

float2 operator *(float scalar, const float2 &rhs);

#endif // FLOAT2_H
