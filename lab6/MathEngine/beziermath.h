#ifndef BEZIERMATH_H
#define BEZIERMATH_H

#include <vector>
#include <list>
#include <deque>
#include <math.h>
#include <QDebug>

std::deque<std::vector<long long>> coeffs {
    {1},                    // n = 0
    {1,1},                  // n = 1
    {1,2,1},                // n = 2
    {1,3,3,1},              // n = 3
    {1,4,6,4,1},            // n = 4
    {1,5,10,10,5,1},        // n = 5
    {1,6,15,20,15,6,1}      // n = 6
};

// k < n
long long BinomialCoeff(int n, int k) {
    if (k > n-k) k = n-k;
    int coefSize = n+1;
    if (n < coeffs.size()) {
        return coeffs[n][k];
    }

//    qDebug() << "create new vector with size " << coefSize+2;
    std::vector<long long> newRow(coefSize+2);
    newRow[0] = 1;
//    qDebug() << "before cycle";
    for (int i = 0; i < coefSize-2; ++i) {

        newRow[i+1] = (BinomialCoeff(n-1,i) + BinomialCoeff(n-1,i+1));

        // cout << newRow[i+1] << " ";
    }
//    qDebug() << "after cycle";
    newRow[coefSize] = 1;
    coeffs.push_back(newRow);
//    qDebug() << "After push_back("<<n<<")";
    return coeffs[n][k];
}
double BinaryPower(double b, unsigned long long e) {
       double v = 1.0;
       while(e != 0) {
              if((e & 1) != 0) {
                     v *= b;
              }
              b *= b;
              e >>= 1;
       }
       return v;
}

// n = v.size()-1
float BezierN(int n, float t, std::vector<int> v) {
    float sum = 0;

    for (int i = 0; i <= n; ++i) {
        sum += (long long)v[i]*BinomialCoeff(n,i) * BinaryPower(1-t,n-i) * BinaryPower(t,i);
    }

    return sum;
}


#endif // BEZIERMATH_H
