#ifndef LR3_MAINWINDOW_H
#define LR3_MAINWINDOW_H

#include <QMainWindow>
#include "Objects3D/camera.h"
#include "Objects3D/scene3d.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Camera *mainCam;
    Scene3D* scene;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init(Scene3D* scene);

public slots:
    void UpdateView();

private:
    Ui::MainWindow *ui;
};

#endif // LR3_MAINWINDOW_H
