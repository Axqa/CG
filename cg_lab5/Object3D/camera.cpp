#include "camera.h"
#include <QDebug>
#include "../MathEngine/ray.h"
#include "../MathEngine/somemath.h"
#include "object3d.h"
#include <QElapsedTimer>

Camera::Camera(QObject *parent) : QObject(parent)
{
    this->pos     = float3{100,100,100};
    this->normal  = (float3{-1,-1,-1}).Normalized();
    this->width = 400;
    this->height = 400;

//    qDebug() << "camera ctor: normal =" << this->normal;
}

SceneView &Camera::UpdateView(SceneView &view)
{
    vec norm = normal;
//    qDebug() << "updating view in camera: normal =" << normal.x << normal.y << normal.z;

    vec u,v;

    normal.PerpendicularBasis(u,v);

//    qDebug() << "here" << normal << u << v;
    float dx = width / view.width;
    float dy = height / view.height;

    float hh = view.height / 2;
    float hw = view.width / 2;

    uint table[view.height][view.width];

    QElapsedTimer timer;
    timer.start();

    for (int row = 0; row < view.height; ++row) {
        for (int col = 0; col < view.width; ++col) {
//            qDebug() << "cycle row:" << row<< "col:" << col;
//            Ray ray(pos)
            vec curPos = pos + u * (hw - col) * dx + v * ( hh - row) * dy ;
            Ray ray(curPos, normal);

            float minDist = FLOAT_INF;
            float curDist;
            QColor color, minColor = QColor(Qt::gray);
            for (auto i : scene->items()) {
//                qDebug() << "item" << i;
                if (i->RayAABBIntersect(ray)) {
                    if (i->GetColorByRay(ray,color,curDist)) {
                        if (curDist < minDist) {
                            minDist = curDist;
                            minColor = color;
                        }
                    }
                }
            }

            table[row][col] = minColor.rgba();
//            view.image.setPixel(col,row,minColor.rgb());
        }
    }

    qDebug() << "calc pixels:" << timer.elapsed() << "ms";
    timer.restart();

    for (int row = 0; row < view.height; ++row) {
        for (int col = 0; col < view.width; ++col) {
            view.image.setPixel(col,row,table[row][col]);
        }
    }

    qDebug() << "set pixels:" << timer.elapsed() << "ms";

//    view.image.fill(Qt::gray);

    return view;
}

void Camera::MousePressed(Qt::MouseButton btn, QPoint pos)
{

}
