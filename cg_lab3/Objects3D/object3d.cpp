#include "object3d.h"

Object3D::Object3D()
{

}

void Object3D::Transform(MatrixF m)
{
    FromMatrix((ToMatrix()*m).NormalizedW());
//    Normalize();
    ObjectChanged();
}

MatrixF Object3D::MatrixTransform(MatrixF m)
{
    return ToMatrix()*m;
}
