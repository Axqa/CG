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

private slots:
    void on_nIncr_clicked();

    void on_mIncr_clicked();

    void on_nDecr_clicked();

    void on_mDecr_clicked();

    void on_xSlider_valueChanged(int value);

    void on_ySlider_valueChanged(int value);

    void on_zSlider_valueChanged(int value);

    void on_xSlider_sliderReleased();

    void on_ySlider_sliderReleased();

    void on_zSlider_sliderReleased();

    void on_checkBox_stateChanged(int arg1);

signals:
    void SetN(int newN);
    void SetM(int newM);
    void XSlider(int value);
    void YSlider(int value);
    void ZSlider(int value);
    void SetShowControlLines(bool state);

private:
    int prevDeg = 0;


private:
    Ui::MainWindow *ui;
};

#endif // LR3_MAINWINDOW_H
