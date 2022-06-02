#include "mainwindow.h"

#include <QApplication>
#include "mainwindow.h"
#include "styleloader.h"
#include <QDebug>
#include "Object3D/camera.h"
#include "Object3D/scene3d.h"
#include "Object3D/line3d.h"

int main(int argc, char *argv[])
{
    QApplication a( argc, argv );
    MainWindow w;

    StyleLoader::attach( ":/styles/style.qss" );

    Scene3D *scene = new Scene3D();
    Camera *cam = new Camera();
    scene->addCamera(cam);

    w.init(scene);
    w.show();

    scene->addObject(new Line3D({0,0,0},{0,0,100}, QColor(Qt::blue)));
    scene->addObject(new Line3D({0,0,0},{0,100,0}, QColor(Qt::red)));
    scene->addObject(new Line3D({0,0,0},{100,0,0}, QColor(Qt::green)));

    return a.exec();
}
