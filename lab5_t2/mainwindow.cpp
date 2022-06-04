#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Graphics_view_zoom.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

//    setMouseTracking(true);
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

    ui->graphicsView->scale(1,-1);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    Graphics_view_zoom* z = new Graphics_view_zoom(ui->graphicsView);
    z->set_modifiers(Qt::NoModifier);

    connect(z, &Graphics_view_zoom::zoomedRect, &mainCam->sImage, &SceneImage::ChangeSceneRect);

    QPolygonF pol = ui->graphicsView->mapToScene(ui->graphicsView->rect());
    mainCam->sImage.rect = QRectF(-300,-200,600,400);
//    mainCam->sImage.rect.moveCenter(mainCam->sImage.rect.center());
    qDebug() << "image.rect in init" << mainCam->sImage.rect << "label rect" << ui->imageLabel->rect();
    mainCam->sImage.viewRect = ui->imageLabel->rect();
    qDebug() << "image viewRect" << mainCam->sImage.viewRect ;
    initConnections();

    UpdateView();
}

void MainWindow::initConnections()
{
    connect(ui->imageLabel, &ImageLabel::mousePress   , mainCam, &Camera::labelMousePress   );
    connect(ui->imageLabel, &ImageLabel::mouseRelease , mainCam, &Camera::labelMouseRelease );
    connect(ui->imageLabel, &ImageLabel::mouseMove    , mainCam, &Camera::labelMouseMove    );
    connect(ui->imageLabel, &ImageLabel::wheel        , mainCam, &Camera::labelWheel        );
    connect(ui->imageLabel, &ImageLabel::keyPress     , mainCam, &Camera::labelKeyPress     );
    connect(ui->imageLabel, &ImageLabel::keyRelease   , mainCam, &Camera::labelKeyRelease   );
    connect(ui->imageLabel, &ImageLabel::resize       , mainCam, &Camera::labelResize       );
}

void MainWindow::UpdateView()
{
//    ui->graphicsView->viewport()->removeEventFilter(ui->graphicsView->scene());

    if (ui->graphicsView->scene())
        ui->graphicsView->scene()->deleteLater();
    auto scene = (mainCam->CameraView());
    ui->graphicsView->setScene(scene);

    QPolygonF pol = ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect());
//    QRect pixMapRect = QRect(pol.at(0).toPoint(), pol.at(2).toPoint());
    QRect tmp = ui->graphicsView->viewport()->rect();
    QRect pixMapRect = QRect(ui->graphicsView->mapToScene(tmp.bottomLeft()).toPoint(),
                             ui->graphicsView->mapToScene(tmp.topRight()).toPoint());
//    qDebug() << "set rect of image" << pol << "rect" << pixMapRect;


//    auto pm = scene->addPixmap(QPixmap::fromImage(mainCam->sImage.image.scaled(
//                                                      pixMapRect.width(),
//                                                      pixMapRect.height(),
//                                                      Qt::KeepAspectRatioByExpanding)));
//    pm->setX(-pixMapRect.width()/2);
//    pm->setY(-pixMapRect.height()/2);


    ui->imageLabel->setPixmap(QPixmap::fromImage(mainCam->sImage.image.scaled(
                                                     ui->imageLabel->width(),
                                                     ui->imageLabel->height(),
                                                     Qt::KeepAspectRatio)));
//    qDebug() << "View update";
    ui->imageLabel->update();
    ui->graphicsView->update();
//    ui->graphicsView->viewport()->installEventFilter(scene);
}

void MainWindow::on_nIncr_clicked()
{
    int n = ui->nLe->text().toInt();
    n++;
    ui->nLe->setText(QString::number(n));
    if (!ui->nDecr->isEnabled())
        ui->nDecr->setEnabled(true);
    if (n == 10)
        ui->nIncr->setEnabled(false);
    SetN(n);
}


void MainWindow::on_mIncr_clicked()
{
    int m = ui->mLe->text().toInt();
    m++;
    ui->mLe->setText(QString::number(m));
    if (!ui->mDecr->isEnabled())
        ui->mDecr->setEnabled(true);
    if (m == 10)
        ui->mIncr->setEnabled(false);
    SetM(m);
}


void MainWindow::on_nDecr_clicked()
{
    int n = ui->nLe->text().toInt();
    n--;
    ui->nLe->setText(QString::number(n));
    if (n == 2)
        ui->nDecr->setEnabled(false);
    if (!ui->nIncr->isEnabled())
        ui->nIncr->setEnabled(true);

    SetN(n);
}


void MainWindow::on_mDecr_clicked()
{
    int m = ui->mLe->text().toInt();
    m--;
    ui->mLe->setText(QString::number(m));
    if (m == 2)
        ui->mDecr->setEnabled(false);
    if (!ui->mIncr->isEnabled())
        ui->mIncr->setEnabled(true);
    SetM(m);
}


void MainWindow::on_xSlider_valueChanged(int value)
{
    ui->xLbl->setText(QString::number(value));
    XSlider(value - prevDeg);
    prevDeg = value;
}


void MainWindow::on_ySlider_valueChanged(int value)
{
    ui->yLbl->setText(QString::number(value));
    YSlider(value - prevDeg);
    prevDeg = value;
}


void MainWindow::on_zSlider_valueChanged(int value)
{
    ui->zLbl->setText(QString::number(value));
    ZSlider(value - prevDeg);
    prevDeg = value;
}


void MainWindow::on_xSlider_sliderReleased()
{
    ui->xLbl->setText("0");
    ui->xSlider->blockSignals(true);
    ui->xSlider->setValue(0);
    ui->xSlider->blockSignals(false);
    prevDeg = 0;
}


void MainWindow::on_ySlider_sliderReleased()
{
    ui->yLbl->setText("0");
    ui->ySlider->blockSignals(true);
    ui->ySlider->setValue(0);
    ui->ySlider->blockSignals(false);
    prevDeg = 0;
}


void MainWindow::on_zSlider_sliderReleased()
{
    ui->zLbl->setText("0");
    ui->zSlider->blockSignals(true);
    ui->zSlider->setValue(0);
    ui->zSlider->blockSignals(false);
    prevDeg = 0;
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    SetShowControlLines(arg1);
}



QPaintEngine *MainWindow::paintEngine() const
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
//    mainCam->sImage.viewRect = ui->graphicsView->viewport()->rect();
    UpdateView();
}
