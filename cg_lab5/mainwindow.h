#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Object3D/sceneview.h"
#include "Object3D/classes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    SceneView view;


    Camera *mainCam;
    Scene3D* scene;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init(Scene3D* scene);

    void UpdateView();

    QPoint MapPointToScreen(QPoint pos);
public slots:
    void NeedToUpdate();

private:
    Ui::MainWindow *ui;

    // QPaintDevice interface
public:
    QPaintEngine *paintEngine() const override;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

signals:
    void MousePressed(Qt::MouseButton btn, QPoint pos);
};
#endif // MAINWINDOW_H
