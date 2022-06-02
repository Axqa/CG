#include "sceneview.h"

SceneView::SceneView(QObject *parent) : QObject(parent)
{
    image = QImage(width, height, QImage::Format_RGB32);
}

void SceneView::resize(int newWidth, int newHeight)
{
    image = image.scaled(newWidth, newHeight, Qt::AspectRatioMode::IgnoreAspectRatio);
    width = newWidth;
    height = newHeight;
}
