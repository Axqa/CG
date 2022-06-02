#include "mainwindow.h"

#include <QApplication>

#include "Objects3D/clss.h"
#include "Objects3D/point3.h"
#include "Objects3D/line3d.h"
#include "Objects3D/selectablepoint.h"
#include "Objects3D/bezierplane.h"
#include "styleloader.h"
#include <QDebug>

int main( int argc, char **argv ) {
    QApplication a( argc, argv );
    MainWindow w;

    StyleLoader::attach( ":/styles/style.qss" );

    Scene3D *scene = new Scene3D();
    Camera *cam = new Camera();
    scene->addCamera(cam);

    w.init(scene);
    w.show();

//    scene->addObject(new Point3(0,0,0, 5, QColor(Qt::black)));
//    scene->addObject(new Point3(50,0,0));
//    scene->addObject(new Point3(100,0,0, QColor(Qt::green)));
//    scene->addObject(new Point3(0,50,0));
//    scene->addObject(new Point3(0,100,0, QColor(Qt::red)));
//    scene->addObject(new Point3(0,0,50));
//    scene->addObject(new Point3(0,0,100, QColor(Qt::blue)));

    scene->addObject(new Line3D({0,0,0}, {200,0,0}, QColor(Qt::green)));
    scene->addObject(new Line3D({0,0,0}, {0,200,0}, QColor(Qt::red)));
    scene->addObject(new Line3D({0,0,0}, {0,0,200}, QColor(Qt::blue)));

//    scene->addObject(new SelectablePoint(50,50,50, 5));
//    scene->addObject(new SelectablePoint(100,50,50, 10));

    auto bPlane = new BezierPlane();
    scene->addObject(bPlane);

    QObject::connect(&w, &MainWindow::SetM, bPlane, &BezierPlane::setM);
    QObject::connect(&w, &MainWindow::SetN, bPlane, &BezierPlane::setN);
    QObject::connect(&w, &MainWindow::XSlider, bPlane, &BezierPlane::RotateXDeg);
    QObject::connect(&w, &MainWindow::YSlider, bPlane, &BezierPlane::RotateYDeg);
    QObject::connect(&w, &MainWindow::ZSlider, bPlane, &BezierPlane::RotateZDeg);
    QObject::connect(&w, &MainWindow::SetShowControlLines, bPlane, &BezierPlane::SetShowLines);

//    scene->addObject(new Line3D(float3{1,0,0} * cam->plane.d,
//                                float3{0,1,0} * cam->plane.d));
//    scene->addObject(new Line3D(float3{0,1,0} * cam->plane.d,
//                                float3{0,0,1} * cam->plane.d));
//    scene->addObject(new Line3D(float3{0,0,1} * cam->plane.d,
//                                float3{1,0,0} * cam->plane.d));

//    scene->addObject(new Line3D({-500,0,0}, {500, 0, 0}, QColor(Qt::cyan)));
//    scene->addObject(new Line3D({-500,0,50}, {500, 0, 50}, QColor(Qt::cyan)));

//    qDebug() << float3(1,0,0).AngleBetween({1,0,0});
//    qDebug() << float3(1,0,0).AngleBetween({1,1,0});
//    qDebug() << float3(1,0,0).AngleBetween({-1,1,0});
//    qDebug() << float3(1,0,0).AngleBetween({-1,0,0});
//    qDebug() << float3(1,0,0).AngleBetween({-1,-1,0});
//    qDebug() << float3(1,0,0).AngleBetween({1,-1,0});
    vec x,y;
    vec z (0,0,-1); z.Normalize();
    z.PerpendicularBasis(x,y);
    qDebug()  << "x" << x << "y" << y;
    qDebug() << float3(1,-1,0).Normalized().Dot(float3(1,0,0));
    return a.exec();
}
