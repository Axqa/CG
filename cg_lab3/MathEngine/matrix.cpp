#include "matrix.h"
#include "somemath.h"
#include <QDebug>


template<typename T>
Matrix<T>::Matrix(u32 size)
{
    Initialize(size,size);
}

template<typename T>
Matrix<T>::Matrix(u32 rowCount, u32 colCount)
{
    Initialize(rowCount, colCount);
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &m)
{
    Initialize(m.RowCount(), m.ColCount());

    data = m.data;
}

template<typename T>
Matrix<T>::Matrix(std::vector<std::vector<T> > vec)
{
    u32 rows = vec.size();
    massert(rows, "RowCount must be > 0");
    u32 cols = vec[0].size();
    Initialize(rows, cols);
    for (u32 i = 0; i  < rows; ++i) {
        massert(cols == vec[i].size(), "Rows must have same size");
        for (u32 j = 0; j < cols; ++j) {
            at(i,j) = vec[i][j];
        }
    }

}

template<typename T>
Matrix<T> &Matrix<T>::operator =(const Matrix<T> &m)
{
    FreeData();
    rowCount = m.rowCount;
    colCount = m.colCount;
    data = m.data;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator =(Matrix<T> &&m)
{
    FreeData();
    rowCount = m.rowCount;
    colCount = m.colCount;
    data = std::move(m.data);
    return *this;
}

template<typename T>
void Matrix<T>::Initialize(u32 size)
{
    Initialize(size,size);
}

template<typename T>
void Matrix<T>::Initialize(u32 rowC, u32 colC)
{
    massert(rowC > 0, "Row Count must be > 0");
    massert(colC > 0, "Col Count must be > 0");

    FreeData();

    rowCount = rowC;
    colCount = colC;

    data = std::vector<T>(rowCount*colCount); // should fill with zeros

}

template<typename T>
Matrix<T>::~Matrix()
{
    FreeData();
}

template<typename T>
void Matrix<T>::FreeData()
{
    data.clear();
}

template<typename T>
u32 Matrix<T>::RowCount() const
{
    return rowCount;
}

template<typename T>
u32 Matrix<T>::ColCount() const
{
    return colCount;
}

template<typename T>
bool Matrix<T>::isDimEqual(const Matrix &oth) const
{
    return rowCount == oth.rowCount && colCount == oth.colCount;
}

template<typename T>
Matrix<T> Matrix<T>::operator +(const Matrix &oth)
{
    massert(isDimEqual(oth), "Dimensials must equals");
    Matrix<T> res(rowCount, colCount);
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            res[i][j] = at(i,j) + oth.at(i,j);
        }
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator -(const Matrix &oth)
{
    massert(isDimEqual(oth), "Dimensials must equals");
    Matrix<T> res(rowCount, colCount);
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            res[i][j] = at(i,j) - oth.at(i,j);
        }
    }
    return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator *(const Matrix &oth)
{
//    qDebug() << "multiplying (" << rowCount<<colCount << ") on (" << oth.rowCount << oth.colCount << ")";
    massert(oth.rowCount == colCount, "ColCount must equal other.RowCount");
    Matrix<T> res(rowCount, oth.colCount);

    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < oth.colCount; ++j) {
            T sum = 0;

            for ( u32 k = 0; k < oth.rowCount; k++ ) {
                sum += at(i,k) * oth.at(k,j);
            }
            res.at(i,j) = sum;
        }
    }
    return res;
}
template <typename T>
Matrix<T> Matrix<T>::operator * (float b)
{
    Matrix<T> res(rowCount, colCount);
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            res[i][j] = at(i,j) * b;
        }
    }
    return res;
}

template<typename T>
Matrix<T> &Matrix<T>::operator +=(const Matrix &oth)
{
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            at(i,j) += oth.at(i,j);
        }
    }
    return *this;
}

template<typename T>
Matrix<T> &Matrix<T>::operator -=(const Matrix &oth)
{
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            at(i,j) -= oth.at(i,j);
        }
    }
    return *this;
}

template<typename T>
Matrix<T> &Matrix<T>::operator *=(const Matrix &oth)
{
//    massert(oth.rowCount == colCount, "ColCount must equal other.RowCount");

//    Matrix<T> res(rowCount, oth.colCount);

//    for (u32 i = 0; i < rowCount; ++i) {
//        for (u32 j = 0; j < oth.colCount; ++j) {
//            T sum = 0;

//            for ( u32 k = 0; k < oth.rowCount; k++ ) {
//                sum += at(j,k) * oth.at(k,j);
//            }
//            res.at(i,j) = sum;
//        }
//    }

    operator= (*this * oth);
    return *this;
}

template<typename T>
Matrix<T> &Matrix<T>::operator *=(float b)
{
    for (u32 i = 0; i < rowCount; ++i) {
        for (u32 j = 0; j < colCount; ++j) {
            at(i,j) *= b;
        }
    }
    return *this;
}

template<typename T>
T *Matrix<T>::operator[](u32 idx)
{
    return at(idx);
}

template<typename T>
T &Matrix<T>::at(u32 row, u32 col)
{
//    qDebug() << "dims:" << rowCount << colCount << "idx:" << row << col;
    massert(row < rowCount, "Row index out of range");
    massert(col < colCount, "Col index out of range");

    return data[row * colCount + col];
}

template<typename T>
T *Matrix<T>::at(u32 row)
{
    massert(row < rowCount, "Row index out of range");
    return &data[row*colCount];
}

template<typename T>
const T *Matrix<T>::operator[](u32 idx) const
{
    return at(idx);
}

template<typename T>
const T &Matrix<T>::at(u32 row, u32 col) const
{
    massert(row < rowCount, "Row index out of range");
    massert(col < colCount, "Col index out of range");

    return data[row * colCount + col];
}

template<typename T>
const T *Matrix<T>::at(u32 row) const
{
    massert(row < rowCount, "Row index out of range");
    return &data[row*colCount];
}

template<typename T>
MatrixF Matrix<T>::CreateEye(u32 size)
{
    MatrixF res(size);
    for (u32 i = 0; i < size; ++i) {
        res.at(i,i) = 1;
    }
    return res;
}

template<typename T>
MatrixF Matrix<T>::MoveBy(float dx, float dy, float dz)
{
    MatrixF res  ({
          {1,0,0,0},
          {0,1,0,0},
          {0,0,1,0},
          {dx,dy,dz,1}
      });
    return res;
}

template<typename T>
MatrixF Matrix<T>::RotateX(float angle)
{
    float sA, cA;
    SinCos(angle, sA, cA);
    MatrixF res ({
        {1,0,0,0},
        {0,cA,sA,0},
        {0,-sA,cA,0},
        {0,0,0,1}
    });
        return res;
}

template<typename T>
MatrixF Matrix<T>::RotateY(float angle)
{
    float sA, cA;
    SinCos(angle, sA, cA);
    MatrixF res ({
        {cA,0,-sA,0},
        {0,1,0,0},
        {sA,0,cA,0},
        {0,0,0,1}
    });
    return res;
}

template<typename T>
MatrixF Matrix<T>::RotateZ(float angle)
{
    float sA, cA;
    SinCos(angle, sA, cA);
    MatrixF res ({
       {cA,sA,0,0},
       {-sA,cA,0,0},
       {0,0,1,0},
       {0,0,0,1}
   });
   return res;
}

template <typename T>
std::ostream& operator<< (std::ostream &out, const Matrix<T> &m)
{

    for (u32 i = 0; i < m.rowCount; ++i) {
        for (u32 j = 0; j < m.colCount; ++j) {
            out << m.at(i,j) << " ";
        }
        out << std::endl;
    }

    return out;
}
