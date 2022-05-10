#ifndef MATRIX_H
#define MATRIX_H

#include "../commondec.h"
#include "../myassert.h"
#include <vector>
#include <iostream>
#include <QString>

template <typename T = float>
class Row : public std::vector<T>
{
    // This is needed for vector constructors
    using std::vector<T>::vector;

};


template <typename T = float>
class Matrix
{
    std::vector<T> data; // data[row * num_cols + col]
    u32 rowCount = 0;
    u32 colCount = 0;
public:
    Matrix() {};
    Matrix(u32 size);
    Matrix(u32 rowCount, u32 colCount);
    Matrix(const Matrix<T> &m);
    Matrix(std::vector<std::vector<T>> vec);
//    Matrix(Row<T> row);
    Matrix& operator = (const Matrix<T> &m);
    Matrix& operator = (Matrix<T> &&m);

    void Initialize(u32 size);
    void Initialize(u32 rowC, u32 colC);

    ~Matrix();
    void FreeData();

    u32 RowCount() const;
    u32 ColCount() const;

    bool isDimEqual(const Matrix &oth) const;

    Matrix operator + (const Matrix &oth);
    Matrix operator - (const Matrix &oth);
    Matrix operator * (const Matrix &oth);
    Matrix operator * (float b);
    Matrix &operator += (const Matrix &oth);
    Matrix &operator -= (const Matrix &oth);
    Matrix &operator *= (const Matrix &oth);
    Matrix &operator *= (float b);

    T* operator[] (u32 idx) ;
    T& at(u32 row, u32 col) ;
    T* at(u32 row) ;
    const T* operator[] (u32 idx) const;
    const T& at(u32 row, u32 col) const;
    const T* at(u32 row) const;

    static Matrix<float> CreateEye(u32 size);

    static Matrix<float> MoveBy(float dx, float dy, float dz);

    // angle in rad
    /// Rotate CounterClockWise around X-axis on angle in rad
    static Matrix<float> RotateX(float angle);
    /// Rotate CounterClockWise around Y-axis on angle in rad
    static Matrix<float> RotateY(float angle);
    /// Rotate CounterClockWise around Z-axis on angle in rad
    static Matrix<float> RotateZ(float angle);

    // to have foreach-like loop
    decltype(data.begin()) begin() {return data.begin();};
    decltype(data.end()) end() {return data.end();};

    /// Returns normalized rows by 4th column
    /// Need to have 4 cols;
    /// If W == 0, row stay unchanged
    Matrix NormalizedW();

    friend std::ostream& operator<<  (std::ostream &out, const Matrix<> &m) ;
    operator QString() const ;
};

typedef  Matrix<float> MatrixF;

template class Matrix<float>;
template class Matrix<double>;

#endif // MATRIX_H

/*
 *
 * struct Matrix {
private:
    u32 m, n;

public:
    enum EMatrixErrors {
        ME_None			= 0x0000,
        ME_Init			= 0x0001,
        ME_Dimensions	= 0x0002,
        ME_NotSquare	= 0x0004,
        ME_Singular		= 0x0008,

        ME_Size			= ME_Dimensions
    };

    u32 error = ME_None;

    void ClearError( void ) {
        error = ME_None;
    }

    // Ultrafast, but you must be a bit more careful to use it. C-style, in other words.
    double **data = NULL;

    // All the pointers are saved as-is (without copying the content).
    Matrix( u32 x, u32 y );
    Matrix( u32 x, u32 y, double **cells );
    Matrix( u32 x, u32 y, float **cells );
    Matrix( const Matrix& ); // Copying.

    explicit Matrix( u32 squareside ) : Matrix( squareside, squareside ) {};
    Matrix( u32 squareside, double **cells ) : Matrix( squareside, squareside, cells ) {};

    //Matrix( std::initializer_list<std::initializer_list<double>> );
    explicit Matrix( std::vector<std::vector<double>> );

    Matrix& operator= ( const Matrix& ) noexcept;	// Moving.
    Matrix operator= ( const std::vector<std::vector<double>>& );
    Matrix operator+ ( const Matrix& );
    Matrix operator- ( const Matrix& );
    Matrix operator* ( const Matrix& );
    Matrix operator* ( double );

    const Matrix* operator* () { return this; }

    bool operator== ( const Matrix& );
    bool operator!= ( const Matrix& );

    u32 GetRows( void ) { return n; }
    u32 GetCols( void ) { return m; }

    void DebugPrint( void );


    double Det( void );
    void Transpose( void );
    void Inverse( void );

    void Rotate( double angle, double pitch );
    void Move( double x, double y, double z );
    //void Project( ? );


    ~Matrix();

    static Matrix MoveBy(float dx, float dy, float dz);

    // angle in rad
    static Matrix RotateX(float angle);
    static Matrix RotateY(float angle);
    static Matrix RotateZ(float angle);

};
*/

