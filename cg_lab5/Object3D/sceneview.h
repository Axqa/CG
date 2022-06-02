#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QObject>
#include <QImage>

class SceneView : public QObject
{
    Q_OBJECT
public:
    explicit SceneView(QObject *parent = nullptr);

    QImage image;
    int width = 800;
    int height = 600;

    void resize(int newWidth, int newHeight);
signals:

};

#endif // SCENEVIEW_H
