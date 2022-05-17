#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    init();
}

MainWindow::~MainWindow()
{
    delete border;
    delete ui;
}

void MainWindow::init()
{

    scene = new CustomScene();
    ui->graphicsView->setScene( scene );
//    qDebug() << ui->graphicsView->width() << " " << ui->graphicsView->height() ;
    scene->setSceneRect(-400,-300,800, 600);

    connect(scene, &CustomScene::sceneMouseMoved, this, &MainWindow::OnMouseMove);
    connect(scene, &CustomScene::sceneMousePressed, this, &MainWindow::OnMousePress);
    connect(scene, &CustomScene::sceneMouseReleased, this, &MainWindow::OnMouseRelease);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    border = new Border();
    scene->addItem(border);
}

void MainWindow::OnMousePress(QPoint p, Qt::MouseButton btn)
{
    for (auto i : lines) {
        if (i->isLinePoint(p))
            return;
    }
//    qDebug() << "after line check";
    if (border->isBorderPoint(p)) {
        return;
    }
    qDebug() << "window mouse press";

    GraphLine *newLine;
    switch (btn) {
    case Qt::MouseButton::LeftButton:

        newLine = new GraphLine(p, border);
        lines << newLine;
        scene->addItem(newLine);
//        newLine->grabMouse();

        break;

    case Qt::MouseButton::RightButton:
        border->AddPoint(p);
        scene->update();
        break;
    }
}

void MainWindow::OnMouseRelease(QPoint p, Qt::MouseButton btn)
{
    if (btn == Qt::MouseButton::LeftButton) {
//        scene->removeItem(curP);
        curP = nullptr;
//        scene->removeItem(prevP);
        prevP = nullptr;
    }
}

void MainWindow::OnMouseMove(QPoint from, QPoint to)
{
//    if (curP != nullptr) {
//        prevP->moveBy(to.x()-from.x(), to.y()-from.y());
//    }
}

