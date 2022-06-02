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
    Matrix() : rowCount(0), colCount(0) {data = std::vector<T>();};
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

    /// Append this matrix with arg
    Matrix& MergeVer(const Matrix &oth);
    /// Returns new matrix with copied rows from stRow to stRow+rowC
    Matrix MatrixPart(u32 stRow, u32 rowC);

    friend std::ostream& operator<<  (std::ostream &out, const Matrix<> &m) ;
    operator QString() const ;
};

typedef  Matrix<float> MatrixF;

template class Matrix<float>;
template class Matrix<double>;

#endif // MATRIX_H



