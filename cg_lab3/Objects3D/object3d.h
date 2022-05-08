#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "clss.h"

#include "../MathEngine/ncl.h"
#include "../MathEngine/matrix.h"
#include "camera.h"
#include <QObject>
#include <QGraphicsItem>

class Object3D : public QObject
{
    Q_OBJECT
public:
    Object3D();

    virtual QGraphicsItemGroup* DrawOnCameraView(Camera& cam) =0;
    virtual Matrix<float> ToMatrix() = 0;
    virtual void FromMatrix(MatrixF m ) = 0;
    virtual void Normalize() = 0;

    virtual void Transform(MatrixF m);
    virtual MatrixF MatrixTransform(MatrixF m);


signals:
    void ObjectChanged();
};

#endif // OBJECT3D_H
