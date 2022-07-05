#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget *parent) : QLabel(parent)
{
    setMouseTracking(true);
}


QPaintEngine *ImageLabel::paintEngine() const
{
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    mousePress(event);
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    mouseRelease(event);
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    mouseMove(event);
}

void ImageLabel::wheelEvent(QWheelEvent *event)
{
    wheel(event);
}

void ImageLabel::keyPressEvent(QKeyEvent *event)
{
    keyPress(event);
}

void ImageLabel::keyReleaseEvent(QKeyEvent *event)
{
    keyRelease(event);
}

void ImageLabel::resizeEvent(QResizeEvent *event)
{
    resize(event);
}
