#include "objectgroup.h"
#include "../myassert.h"

ObjectGroup::ObjectGroup()
{

}

Object3D *ObjectGroup::addItem(Object3D *obj)
{
    groupItems << obj;
    return obj;
}

QList<Object3D *> ObjectGroup::items()
{
    return groupItems;
}


//QGraphicsItemGroup *ObjectGroup::DrawOnCameraView(Camera &cam)
//{
//    QGraphicsItemGroup *group = new QGraphicsItemGroup();
//    for (auto el : groupItems) {
//        for (auto grEl : el->DrawOnCameraView(cam)->childItems()) {
//            group->addToGroup(grEl);
//        }
//    }
//    return group;
//}

//Matrix<float> ObjectGroup::ToMatrix()
//{
//    massert(0, "ObjectGroup::ToMatrix called");
//}

//void ObjectGroup::FromMatrix(MatrixF m)
//{
//    massert(1, "ObjectGroup::ToMatrix called");
//}

//void ObjectGroup::Normalize()
//{
//}

//void ObjectGroup::Transform(MatrixF m)
//{
//}

//MatrixF ObjectGroup::MatrixTransform(MatrixF m)
//{
//}

