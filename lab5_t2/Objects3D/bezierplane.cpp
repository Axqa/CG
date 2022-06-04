#include "bezierplane.h"
#include "selectablepoint.h"
#include "../MathEngine/beziermath.h"
#include "../MathEngine/somemath.h"
#include "line3d.h"

BezierPlane::BezierPlane()
    : m(2), n(2)
{
    controlPoints.resize(n+1);
    for (int i = 0; i < n+1;++i ) {
        controlPoints[i] = QVector<SelectablePoint*>(m+1);
        for (int j = 0 ; j < m+1; ++j) {
            controlPoints[i][j] = (SelectablePoint*) addItem( new SelectablePoint(50 + 50 * i, 20, 50 + 50 * j, 5) );
            connect(controlPoints[i][j], &SelectablePoint::ObjectChanged, this, &BezierPlane::ObjectChanged);
        }
    }
    nSub = 25;
    mSub = 25;
}

BezierPlane::BezierPlane(int n, int m)
{
    this->n = n; this->m = m;

    controlPoints.resize(n+1);
    for (int i = 0; i < n+1;++i ) {
        controlPoints[i] = QVector<SelectablePoint*>(m+1);
        for (int j = 0 ; j < m+1; ++j) {
            controlPoints[i][j] = (SelectablePoint*) addItem( new SelectablePoint(50 + 50 * i, 20, 50 + 50 * j, 5) );
        }
    }

    nSub = 25;
    mSub = 25;

}

BezierPlane::~BezierPlane()
{
    for (auto i : controlPoints) {
        qDeleteAll(i);
//        for (auto j : i)
//            delete j;
    }

    if (surfPoints) {
        for (int i = 0; i < nSub; ++i) {
            delete[] surfPoints[i];
        }
    }
    delete[] surfPoints;
}

void BezierPlane::RecalcPoints()
{
    if (surfPoints) {
        for (int i = 0; i < nSub; ++i) {
            delete[] surfPoints[i];
        }
    }
    delete[] surfPoints;

    surfPoints = new float3*[nSub];
    for (int i = 0; i < nSub; ++i) {
        surfPoints[i] = new float3[mSub];
    }

    int i,j,k,l;
    float u, v; // [0.0 ; 1.0]
    float* bu, *bv; // Bernstein polynomial

    bu = new float[n+2];
    bv = new float[m+2];

    for (int k = 0 ; k < nSub; ++k) {

        u = (float) k / (nSub - 1);

        for(i=0;i<(n+2);i++) bu[i]=0.0f;

        bu[1]=1.0f;
        for(i=1;i<n+1;i++)
        {
            for(j=i+1;j>0;j--)
            {
                bu[j]=(1-u)*bu[j]+u*bu[j-1];
            }
        }
//        for (i = 0; i < n + 1; ++i) {
//            bu[i] = BinomialCoeff(n,i) * BinaryPower(u, i) * BinaryPower(1-u, n-i);
//        }

        for (l = 0 ; l < mSub; ++l) {
            v = (float) l / (mSub - 1);

            for(j=0;j<(m+2);j++) bv[j]=0.0f;

//            for (j = 0; j < m + 1; ++j) {
//                bv[j] = BinomialCoeff(m,j) * BinaryPower(v, j) * BinaryPower(1-v, m-j);
//            }
            bv[1]=1.0f;
            for(j=1;j<m+1;j++)
            {
                for(i=j+1;i>0;i--)
                {
                    bv[i]=(1-v)*bv[i]+v*bv[i-1];
                }
            }

            surfPoints[k][l] = float3(0,0,0);
            for(i=0;i<n+1;i++)
            {
                for(j=0;j<m+1;j++)
                {
                    surfPoints[k][l] += controlPoints[i][j]->pos * bu[i+1] * bv[j+1];
                }
            }
        }
    }

    delete[] bu;
    delete[] bv;
}

void BezierPlane::setN(int newN)
{
    controlPoints.resize(newN+1);
    if (newN > n) {
        for (int i = n+1; i < newN+1; ++i) {
            controlPoints[i] = QVector<SelectablePoint*>(m+1);
            for (int j = 0 ; j < m+1; ++j) {
                controlPoints[i][j] = (SelectablePoint*) addItem( new SelectablePoint(50 + 50 * i, 20, 50 + 50 * j, 5) );
                connect(controlPoints[i][j], &SelectablePoint::ObjectChanged, this, &BezierPlane::ObjectChanged);
            }
        }
    }
    n = newN;
    nSub = (n+1)*10;
    ObjectChanged();
}

void BezierPlane::setM(int newM)
{
    for (int i = 0; i < n+1; ++i) {
        controlPoints[i].resize(newM+1);
        if (newM > m) {
            for (int j = m+1; j < newM+1; ++j) {
                controlPoints[i][j] = (SelectablePoint*) addItem( new SelectablePoint(50 + 50 * i, 20, 50 + 50 * j, 5) );
                connect(controlPoints[i][j], &SelectablePoint::ObjectChanged, this, &BezierPlane::ObjectChanged);
            }
        }
    }
    m = newM;
    mSub = (m+1)*10;
    ObjectChanged();
}

void BezierPlane::RotateXDeg(int value)
{
    qDebug() << "rotate x " << value << ToRad(value);
    Transform(MatrixF::RotateX(ToRad(value)));
//    ObjectChanged();
}

void BezierPlane::RotateYDeg(int value)
{
    Transform(MatrixF::RotateY(ToRad(value)));
//    ObjectChanged();
}

void BezierPlane::RotateZDeg(int value)
{
    Transform(MatrixF::RotateZ(ToRad(value)));
    //    ObjectChanged();
}

void BezierPlane::SetShowLines(bool state)
{
    showControlLines = state;
    ObjectChanged();
}

void BezierPlane::NeedRecalc()
{

}


QGraphicsItemGroup *BezierPlane::DrawOnCameraView(Camera &cam)
{
//    qDebug() << "start drawing bezier";
    MatrixF prPoint = cam.ProjectOnScreen(this);
//    qDebug() << "matrix" << prPoint;

    BezierPlane p (n,m);
////    qDebug() << prPoint;
    p.FromMatrix(prPoint);
    p.RecalcPoints();

    for(int i = 0; i < n+1 ; i++)
    {
        for(int j = 0; j < m+1; j++)
        {
            p.controlPoints[i][j]->setSelected( controlPoints[i][j]->isSelected());
        }
    }

//    if (!visible) {
//        return new QGraphicsItemGroup();
//    }

    QGraphicsItemGroup *group = new QGraphicsItemGroup();

    for (auto row : p.controlPoints) {
        for (auto el : row) {
            auto curGr = el->ToGraphGroup(cam);
//            for (auto item : curGr->childItems()) {
//                group->addToGroup(item);
//            }
            delete curGr;

        }
    }

    if (showControlLines) {
        for (int i = 0; i < n+1; ++i) {
            for (int j = 0; j < m+1; ++j) {
                if (i > 0){
//                    Line3D::LineToSegments(p.controlPoints[i][j]->pos, p.controlPoints[i-1][j]->pos, 2, QPen(QColor(Qt::white),1), group);
                    cam.sImage.DrawLine(p.controlPoints[i][j]->pos, p.controlPoints[i-1][j]->pos, QColor(Qt::white), 1);
                }
                if (i < n){
//                    Line3D::LineToSegments(p.controlPoints[i][j]->pos, p.controlPoints[i+1][j]->pos, 2, QPen(QColor(Qt::white),1), group);
                    cam.sImage.DrawLine(p.controlPoints[i][j]->pos, p.controlPoints[i+1][j]->pos, QColor(Qt::white), 1);
                }
                if (j > 0){
//                    Line3D::LineToSegments(p.controlPoints[i][j]->pos, p.controlPoints[i][j-1]->pos, 2, QPen(QColor(Qt::white),1), group);
                    cam.sImage.DrawLine(p.controlPoints[i][j]->pos, p.controlPoints[i][j-1]->pos, QColor(Qt::white), 1);
                }
                if (j < m){
//                    Line3D::LineToSegments(p.controlPoints[i][j]->pos, p.controlPoints[i][j+1]->pos, 2, QPen(QColor(Qt::white),1), group);
                    cam.sImage.DrawLine(p.controlPoints[i][j]->pos, p.controlPoints[i][j+1]->pos, QColor(Qt::white), 1);
                }
            }
        }
    }

    for (int i = 0; i < p.nSub; ++i) {
        for (int j = 0; j < p.mSub; ++j) {
//            qDebug() << p.surfPoints[i][j];
            if (i > 0){
//                Line3D::LineToSegments(p.surfPoints[i][j], p.surfPoints[i-1][j], 2, QPen(QColor(Qt::black),1), group);
                cam.sImage.DrawLine(p.surfPoints[i][j], p.surfPoints[i-1][j], QColor(Qt::black), 1);
            }
            if (i < p.nSub-1){
//                Line3D::LineToSegments(p.surfPoints[i][j], p.surfPoints[i+1][j], 2, QPen(QColor(Qt::black),1), group);
                cam.sImage.DrawLine(p.surfPoints[i][j], p.surfPoints[i+1][j], QColor(Qt::black), 1);
            }
            if (j > 0){
//                Line3D::LineToSegments(p.surfPoints[i][j], p.surfPoints[i][j-1], 2, QPen(QColor(Qt::black),1), group);
                cam.sImage.DrawLine(p.surfPoints[i][j], p.surfPoints[i][j-1], QColor(Qt::black), 1);
            }
            if (j < p.mSub-1){
//                Line3D::LineToSegments(p.surfPoints[i][j], p.surfPoints[i][j+1], 2, QPen(QColor(Qt::black),1), group);
                cam.sImage.DrawLine(p.surfPoints[i][j], p.surfPoints[i][j+1], QColor(Qt::black), 1);
            }
        }
    }
//    qDebug() << "after lines";

    return group;
}

MatrixF BezierPlane::ToMatrix()
{
    MatrixF res;
    for (auto row : controlPoints) {
        for (auto el : row) {
            if (res.ColCount() == 0) {
                res = el->ToMatrix();
            }
            else {
                res.MergeVer(el->ToMatrix());
            }
        }
    }
    return res;
}

void BezierPlane::FromMatrix(MatrixF m)
{
    int r = 0;
    for (auto row : controlPoints) {
        int c = 0;
        for (auto el : row) {
            el->FromMatrix(m.MatrixPart((r*row.size() + c)*7, 7));
            c++;
        }
        r++;
    }
}

void BezierPlane::Normalize()
{
}


bool BezierPlane::isIntersects(const Ray &ray, float &dist)
{
//    float minDist = FLOAT_INF;
//    float curDist;
//    for (auto row : controlPoints) {
//        for (auto p : row) {

//        }
//    }
}

void BezierPlane::MovingRay(Ray &from, Ray &to)
{
}


MatrixF BezierPlane::MatrixTransform(MatrixF m)
{
    return ToMatrix()*m;
}
