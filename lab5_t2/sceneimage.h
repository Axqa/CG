#ifndef SCENEIMAGE_H
#define SCENEIMAGE_H

#include <QObject>
#include <QImage>
#include "MathEngine/MathAll.h"

class SceneImage : public QObject
{
    Q_OBJECT
public:
    explicit SceneImage(QObject *parent = nullptr);
    ~SceneImage();

    void init();
    void Clear();

    QImage image;
    QRectF rect;
    QRect viewRect;

    float* zValues = nullptr;

    // image resolution
    int width = 640;
    int height = 480;

    QColor bgColor = QColor(Qt::gray);

public:
    void DrawLine(float3 &p1, vec &p2, QColor &col, int width = 3);
    static bool Line3DClipping(vec &p1, vec &p2, float2 &lt, float2 &rb);
    void PutPixel(int x, int y, QColor col, float z = FLOAT_INF);


public slots:
    void ChangeSceneRect(QRectF sceneRect, QRect viewRect);
signals:
    void UpdateImage();
};

#endif // SCENEIMAGE_H
