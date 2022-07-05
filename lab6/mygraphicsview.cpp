#include "mygraphicsview.h"
#include "ui_mygraphicsview.h"
#include <QElapsedTimer>
#include <QDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::MyGraphicsView)
{
    ui->setupUi(this);
}

MyGraphicsView::~MyGraphicsView()
{
    delete ui;
}


void MyGraphicsView::paintEvent(QPaintEvent *e)
{
    QElapsedTimer timer;
    timer.start();

    // call the "real" paint event
    QGraphicsView::paintEvent(e);

    qDebug() << "paint time:" << timer.elapsed() << "ms";
}
