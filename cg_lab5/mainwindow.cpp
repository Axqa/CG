#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "Object3D/scene3d.h"
#include "Object3D/camera.h"
#include <QElapsedTimer>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::init(Scene3D* scene)
{
    this->scene = scene;

    connect(scene, &Scene3D::SceneChanged, this, &MainWindow::NeedToUpdate);

    if (this->scene->cams.size() > 0){
        mainCam = this->scene->cams[0];
//        connect(mainCam, &Camera::ViewChanged, this, &MainWindow::UpdateView);
    }

    connect(this, &MainWindow::MousePressed, mainCam, &Camera::MousePressed);

    UpdateView();

}

void MainWindow::UpdateView()
{
//    qDebug() << "updating view in mainwindow";
    QElapsedTimer timer;
    timer.start();
    mainCam->UpdateView(view);
//    qDebug() << "setting pixels: " << timer.elapsed() <<"ms";
    timer.restart();
    ui->label->setPixmap(QPixmap::fromImage( view.image.scaled(ui->label->width(), ui->label->height()) ));
    //    qDebug() << "display image:" << timer.elapsed() << "ms";
}

QPoint MainWindow::MapPointToScreen(QPoint pos)
{
    QPoint point;
    point.setX(pos.x() - width() / 2.0);
    point.setY(height() / 2.0 - pos.y());

    return point;
}

void MainWindow::NeedToUpdate()
{
    UpdateView();
}


QPaintEngine *MainWindow::paintEngine() const
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << "mouse press at" << event->pos() << "mapped:" << MapPointToScreen(event->pos());
    MousePressed(event->button(),  MapPointToScreen(event->pos()));
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
}
