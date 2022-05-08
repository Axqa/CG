#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete scene;
    delete ui;
}

void MainWindow::init(Scene3D* scene) {
    this->scene = scene;

    if (this->scene->cams.size() > 0){
        mainCam = this->scene->cams[0];
        connect(mainCam, &Camera::ViewChanged, this, &MainWindow::UpdateView);
    }
}

void MainWindow::UpdateView()
{
//    ui->graphicsView->viewport()->removeEventFilter(ui->graphicsView->scene());
    delete ui->graphicsView->scene();
    auto scene = (mainCam->CameraView());
    ui->graphicsView->setScene(scene);
//    ui->graphicsView->viewport()->installEventFilter(scene);
}
