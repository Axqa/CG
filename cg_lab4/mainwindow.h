#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customscene.h"
#include "border.h"
#include "graphline.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void init();

public slots:
    void OnMousePress(QPoint p, Qt::MouseButton btn);
    void OnMouseRelease(QPoint p, Qt::MouseButton btn);
    void OnMouseMove(QPoint from, QPoint to);

private:
    CustomScene *scene;
    Ui::MainWindow *ui;

    Border* border = nullptr;
    QGraphicsEllipseItem* curP = nullptr;
    QGraphicsEllipseItem* prevP = nullptr;

    QList<GraphLine*> lines;
    GraphLine *lastLine = nullptr;

    int pointRad = 5;
};
#endif // MAINWINDOW_H
