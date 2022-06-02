#ifndef FLOAT4_H
#define FLOAT4_H

#include "ncl.h"
#include "float2.h"
#include "float3.h"

class float4
{
public:
    enum
    {
        /// Specifies the number of elements in this vector.
        Size = 4
    };
            /// The x component.
            /** A float4 is 16 bytes in size. This element lies in the memory offsets 0-3 of this class. */
            float x;
            /// The y component. [similarOverload: x]
            /** This element is packed to the memory offsets 4-7 of this class. */
            float y;
            /// The z component. [similarOverload: x]
            /** This element is packed to the memory offsets 8-11 of this class. */
            float z;
            /// The w component. [similarOverload: x]
            /** This element is packed to the memory offsets 12-15 of this class. */
            float w;

    /// The default constructor does not initialize any members of this class.
    /** This means that the values of the members x, y, z and w are all undefined after creating a new float4 using
        this default constructor. Remember to assign to them before use.
        @see x, y, z, w. */
    float4() {}

    /// The float4 copy constructor.
    float4(const float4 &rhs) { Set(rhs); }

    /// Constructs a new float4 with the value (x, y, z, w).
    /** @note If you are constructing a float4 from an array of consecutive values, always prefer calling "float4(ptr);" instead of "float4(ptr[0], ptr[1], ptr[2], ptr[3]);"
            because there is a considerable SIMD performance benefit in the first form.
        @see x, y, z, w. */
    float4(float x, float y, float z, float w);

    /// Constructs a new float3 with the value (xyz.x, xyz.y, xyz.z, w).
    /** @see x, y, z, w. */
    float4(const float3 &xyz, float w);

    float4(float x, float y, const float2 &zw);
    float4(float x, const float2 &yz, float w);
    float4(float x, const float3 &yzw);
    float4(const float2 &xy, const float2 &zw);

    /// Constructs a new float3 with the value (xy.x, xy.y, z, w).
    /** @see x, y, z, w. */
    float4(const float2 &xy, float z, float w);

    /// Constructs this float4 from a C array, to the value (data[0], data[1], data[2], data[3]).
    /** @param data An array containing four elements for x, y, z and w. This pointer may not be null. */
    explicit float4(const float *data);

    /// Casts this float4 to a C array.
    /** This function does not allocate new memory or make a copy of this float4. This function simply
        returns a C pointer view to this data structure. Use ptr()[0] to access the x component of this float4,
        ptr()[1] to access y, ptr()[2] to access z, and ptr()[3] to access the w component of this float4.
        @note Since the returned pointer points to this class, do not dereference the pointer after this
            float3 has been deleted. You should never store a copy of the returned pointer.
        @note This function is provided for compatibility with other APIs which require raw C pointer access
            to vectors. Avoid using this function in general, and instead always use the operator [] of this
            class to access the elements of this vector by index.
        @return A pointer to the first float element of this class. The data is contiguous in memory.
        @see operator [](). */
    FORCE_INLINE float *ptr() { return &x; }
    FORCE_INLINE const float *ptr() const { return &x; }

    /// Accesses an element of this vector using array notation.
    /** @param index The element to get. Pass in 0 for x, 1 for y, 2 for z and 3 for w.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec[1] = 10.f; would set the y-component of this vector. */
    FORCE_INLINE float &operator [](int index) { return At(index); }
    FORCE_INLINE CONST_WIN32 float operator [](int index) const { return At(index); }

    /// Accesses an element of this vector.
    /** @param index The element to get. Pass in 0 for x, 1 for y, 2 for z and 3 for w.
        @note If you have a non-const instance of this class, you can use this notation to set the elements of
            this vector as well, e.g. vec.At(1) = 10.f; would set the y-component of this vector. */
    FORCE_INLINE CONST_WIN32 float At(int index) const
    {
        return ptr()[index];
    }

    FORCE_INLINE float &At(int index)
    {
        return ptr()[index];
    }

    void Set(const float4 &rhs);
};


#endif // FLOAT4_H
