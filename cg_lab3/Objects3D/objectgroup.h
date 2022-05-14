#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include "object3d.h"
#include "selectable.h"

class ObjectGroup : public Object3D
{
    Q_OBJECT
protected:
    QList<Object3D*> groupItems;

public:
    ObjectGroup();

    // Object3D interface
public:
    Object3D* addItem(Object3D* obj);
    QList<Object3D*> items();
//    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;
//    Matrix<float> ToMatrix() override;
//    void FromMatrix(MatrixF m) override;
//    void Normalize() override;
//    void Transform(MatrixF m) override;
//    MatrixF MatrixTransform(MatrixF m) override;


};

#endif // OBJECTGROUP_H
