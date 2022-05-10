#ifndef FLOAT3_H
#define FLOAT3_H

#include "../commondec.h"
#include "float2.h"
#include <QDebug>

class float3
{
public:
    enum
    {
        /// Specifies the number of elements in this vector.
        Size = 3
    };
    /// The x component.
    /** A float3 is 12 bytes in size. This element lies in the memory offsets 0-3 of this class. */
    float x;
    /// The y component. [similarOverload: x]
    /** This element is packed to the memory offsets 4-7 of this class. */
    float y;
    /// The z component. [similarOverload: x]
    /** This element is packed to the memory offsets 8-11 of this class. */
    float z;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members x, y and z are all undefined after creating a new float3 using
        this default constructor. Remember to assign to them before use.
        @see x, y, z. */
    float3() {}

    /// The float3 copy constructor.
    float3(const float3 &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; }

    /// Constructs a new float3 with the value (x, y, z).
    /** @see x, y, z. */
    float3(float x, float y, float z);

    /// Constructs a new float3 with the value (scalar, scalar, scalar).
    /** @see x, y, z. */
    explicit float3(float scalar);

    /// Constructs a new float3 with the value (xy.x, xy.y, z).
    float3(const float2 &xy, float z);

    /// Constructs this float3 from a C array, to the value (data[0], data[1], data[2]).
    /** @param data An array containing three elements for x, y and z. This pointer may not be null. */
    explicit float3(const float *data);

    /// Casts this float3 to a C array.
    /** This function does not allocate new memory or make a copy of this float3. This function simply
        returns a C pointer view to this data structure. Use ptr()[0] to access the x component of this float3,
        ptr()[1] to access y, and ptr()[2] to access the z component of this float3.
        @note Since the returned pointer points to this class, do not dereference the pointer after this
            float3 has been deleted. You should never store a copy of the returned pointer.
        @note This function is provided for compatibility with other APIs which require raw C pointer access
            to vectors. Avoid using this function in general, and instead always use the operator [] or
            the At() function to access the elements of this vector by index.
        @return A pointer to the first float element of this class. The data is contiguous in memory.
        @see operator [](), At(). */
    FORCE_INLINE float *ptr() { return &x; }
    FORCE_INLINE const float *ptr() const { return &x; }

    /// Accesses an element of this vector using array notation.
    /** @param index The element to get. Pass in 0 for x, 1 for y and 2 for z.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec[1] = 10.f; would set the y-component of this vector.
        @see ptr(), At(). */
    FORCE_INLINE float &operator [](int index) { return At(index); }
    FORCE_INLINE CONST_WIN32 float operator [](int index) const { return At(index); }

    /// Accesses an element of this vector.
    /** @param index The element to get. Pass in 0 for x, 1 for y, and 2 for z.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec.At(1) = 10.f; would set the y-component of this vector.
        @see ptr(), operator [](). */
    FORCE_INLINE CONST_WIN32 float At(int index) const
    {
        return ptr()[index];
    }

    FORCE_INLINE float &At(int index)
    {
        return ptr()[index];
    }

    /// Adds two vectors. [indexTitle: operators +,-,*,/]
    /** This function is identical to the member function Add().
        @return float3(x + v.x, y + v.y, z + v.z); */
    float3 operator +(const float3 &v) const;
    /// Performs an unary negation of this vector. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Neg().
        @return float3(-x, -y, -z). */
    float3 operator -() const;
    /// Subtracts the given vector from this vector. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Sub().
        @return float3(x - v.x, y - v.y, z - v.z); */
    float3 operator -(const float3 &v) const;
    /// Multiplies this vector by a scalar. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Mul().
        @return float3(x * scalar, y * scalar, z * scalar); */
    float3 operator *(float scalar) const;
    /// Divides this vector by a scalar. [similarOverload: operator+] [hideIndex]
    /** This function is identical to the member function Div().
        @return float3(x / scalar, y / scalar, z / scalar); */
    float3 operator /(float scalar) const;
    /// Unary operator + allows this structure to be used in an expression '+x'.
    float3 operator +() const { return *this; }

    /// Assigns a vector to another.
    /** @return A reference to this. */
    float3 &operator =(const float3 &v);
    /// Adds a vector to this vector, in-place. [indexTitle: operators +=,-=,*=,/=]
    /** @return A reference to this. */
    float3 &operator +=(const float3 &v);
    /// Subtracts a vector from this vector, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float3 &operator -=(const float3 &v);
    /// Multiplies this vector by a scalar, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float3 &operator *=(float scalar);
    /// Divides this vector by a scalar, in-place. [similarOverload: operator+=] [hideIndex]
    /** @return A reference to this. */
    float3 &operator /=(float scalar);

    /// Tests if the length of this vector is one, up to the given epsilon.
    /** @see IsZero(), IsFinite(), IsPerpendicular(). */
    bool IsNormalized(float epsilonSq = 1e-5f) const;

    /// Returns a normalized copy of this vector.
    /** @note If the vector is zero and cannot be normalized, the vector (1, 0, 0) is returned, and an error message is printed.
            If you do not want to generate an error message on failure, but want to handle the failure yourself, use the
            Normalize() function instead.
        @see Normalize(). */
    float3 Normalized() const;

    /// Tests if this is the null vector, up to the given epsilon.
    /** @see IsNormalized(), IsFinite(), IsPerpendicular(). */
    bool IsZero(float epsilonSq = 1e-7f) const;

    /// Adds a vector to this vector. [IndexTitle: Add/Sub/Mul/Div]
    /// @return (x+v.x, y+v.y, z+v.z).
    float3 Add(const float3 &v) const { return *this + v; }

    /// Adds the vector (s,s,s) to this vector.
    /// @note Mathematically, the addition of a vector and scalar is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x+s, y+s, z+s).
    float3 Add(float s) const;

    /// Subtracts a vector from this vector. [similarOverload: Add] [hideIndex]
    /// @return (x-v.x, y-v.y, z-v.z).
    float3 Sub(const float3 &v) const { return *this - v; }

    /// Subtracts the vector (s,s,s) from this vector. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the subtraction of a vector by a scalar is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x-s, y-s, z-s).
    float3 Sub(float s) const;

    /// Subtracts this vector from the vector (s,s,s). [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the subtraction of a scalar by a vector is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (s-x, s-y, s-z).
    float3 SubLeft(float s) const;

    /// Multiplies this vector by a vector, element-wise. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the multiplication of two vectors is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x*v.x, y*v.y, z*v.z).
    float3 Mul(const float3 &v) const;

    /// Multiplies this vector by a scalar. [similarOverload: Add] [hideIndex]
    /// @return (x*s, y*s, z*s).
    float3 Mul(float s) const { return *this * s; }

    /// Divides this vector by a vector, element-wise. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the division of two vectors is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (x/v.x, y/v.y, z/v.z).
    float3 Div(const float3 &v) const;

    /// Divides this vector by a scalar. [similarOverload: Add] [hideIndex]
    /// @return (x/s, y/s, z/s).
    float3 Div(float s) const { return *this / s; }

    /// Divides the vector (s,s,s) by this vector, element-wise. [similarOverload: Add] [hideIndex]
    /// @note Mathematically, the division of a scalar by a vector is not defined in linear space structures,
    ///	 but this function is provided here for syntactical convenience.
    /// @return (s/x, s/y, s/z).
    float3 DivLeft(float s) const;

    /// Computes the length of this vector.
    /** @return Sqrt(x*x + y*y + z*z). */
    float Length() const;

    /// Computes the squared length of this vector.
    /** Calling this function is faster than calling Length(), since this function avoids computing a square root.
        If you only need to compare lengths to each other, but are not interested in the actual length values,
        you can compare by using LengthSq(), instead of Length(), since Sqrt() is an order-preserving
        (monotonous and non-decreasing) function.
        @return x*x + y*y + z*z. */
    float LengthSq() const;

    /// Computes the dot product of this and the given vector.
    /** The dot product has a geometric interpretation of measuring how close two direction vectors are to pointing
        in the same direction, computing angles between vectors, or the length of a projection of one vector to another.
        @return x*v.x + y*v.y + z*v.z.*/
    float Dot(const float3 &v) const;

    /// Computes the cross product of this and the given vector.
    /** Unless this vector and the given vector are linearly dependent, the cross product returns a vector that is perpendicular
        to both vectors.
        @return float3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x).*/
    float3 Cross(const float3 &v) const;

    /// Returns the angle between this vector and the specified vector, in radians.
    /** @note This function takes into account that this vector or the other vector can be unnormalized, and normalizes the computations.
            If you are computing the angle between two normalized vectors, it is better to use AngleBetweenNorm().
        @see AngleBetweenNorm(). */
    float AngleBetween(const float3 &other) const;

    /// Returns the angle between this vector and the specified normalized vector, in radians.
    /** @param normalizedVector The direction vector to compute the angle against. This vector must be normalized.
        @note This vector must be normalized to call this function.
        @see AngleBetween(). */
    float AngleBetweenNorm(const float3 &normalizedVector) const;

    /// Normalizes this float3.
    float Normalize();

    float DistanceSq(const float3 &rhs) const;
    /// Computes the distance between this point and the given object.
    float Distance(const float3 &point) const;

    /** This function computes two new vectors b and c which are both orthogonal to this vector and to each other.
        That is, the set { this, b, c} is an orthogonal set. The vectors b and c that are outputted are also normalized.
        @param outB [out] Receives vector b.
        @param outC [out] Receives vector c.
        @note When calling this function, this vector should not be zero! */
    void PerpendicularBasis(float3 &outB, float3 &outC) const;


    /// Sets all elements of this vector.
    void Set(float x, float y, float z);

    /// Rotates Counter-Clock WIse
    void RotateAroundX(float angle);
    void RotateAroundY(float angle);
    void RotateAroundZ(float angle);

    /// Return rotated CCW around normalized unit vector, on angle in rad
    float3 RodriguesRotation(float3 unit, float angle);

    operator QString() const { return "float3("+ QString::number(x)+", "+QString::number(y)+", "+QString::number(z)+")"; }
};

inline float Dot(const float3 &a, const float3 &b) { return a.Dot(b); }

#endif // FLOAT3_H
