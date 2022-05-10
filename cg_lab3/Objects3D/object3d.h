#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "clss.h"

#include "../MathEngine/ncl.h"
#include "../MathEngine/matrix.h"
#include "camera.h"
#include <QObject>
#include <QGraphicsItem>


enum class Type3d {
    Base,
    Point,
    Line,
    SelectablePoint
};


class Object3D : public QObject
{
    Q_OBJECT
public:
    Object3D();

    virtual QGraphicsItemGroup* DrawOnCameraView(Camera& cam) =0;
    virtual Matrix<float> ToMatrix() = 0;
    virtual void FromMatrix(MatrixF m ) = 0;
    virtual void Normalize() = 0;

    /// Transform this object
    virtual void Transform(MatrixF m);
    /// Returns matrix of transformed this
    virtual MatrixF MatrixTransform(MatrixF m);

    virtual Type3d type() {return _type;}

protected:
    QGraphicsItem *itemOnScene = nullptr;
    Type3d _type = Type3d::Base;

signals:
    void ObjectChanged();
};

#endif // OBJECT3D_H
