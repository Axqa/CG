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
    /// rect in scene
    QRectF rect;
    /// rect in app
    QRect viewRect;

    float* zValues = nullptr;
    float *aValues = nullptr;

    // image resolution
    int width = 900;
    int height = 600;

    QColor bgColor = QColor(Qt::lightGray);

public:
    void DrawLine(float3 p1, vec p2, QColor col, float width = 3);
    void DrawCircle(int xm, int ym, float z, float r, QColor color);
    void DrawSphere(float3 center, float r, QColor color);
    static bool Line3DClipping(vec &p1, vec &p2, float2 &lt, float2 &rb);
    void PutPixel(int x, int y, QColor col, float z = FLOAT_INF);

    void PlotLineAA(int x0, int y0, float z0, int x1, int y1, float z1, QColor color);
    void PlotLineWidth(int x0, int y0, float z0, int x1, int y1, float z1, QColor color, float wd = 1);
    void PlotThickLine(int x0, int y0, float z0, int x1, int y1, float z1, QColor color, float thickness = 1);
    void PlotLine(int x0, int y0, float z0, int x1, int y1, float z1, QColor color);
    void PlotThickLine2(int x0, int y0, float z0, int x1, int y1, float z1, QColor color, float thickness = 1);

    void DrawPolygon(std::vector<float3> p, QColor color);
    void FillPolygon(std::vector<float3> p, QColor color);

    /// Map view coords to scene coords
    QPointF MapToScene(QPoint p);

    /// Map scene coords to view coords
    QPointF MapFromScene(QPointF p);

public slots:
    void ChangeSceneRect(QRectF sceneRect, QRect viewRect);
signals:
    void UpdateImage();
};

#endif // SCENEIMAGE_H
