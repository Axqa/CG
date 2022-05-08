#ifndef FLOAT2_H
#define FLOAT2_H

#include "../commondec.h"

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
            return ptr()[index];
        }

        FORCE_INLINE float &At(int index)
        {
            return ptr()[index];
        }

    
};

#endif // FLOAT2_H
