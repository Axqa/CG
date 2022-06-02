#ifndef LINE3D_H
#define LINE3D_H

#include "object3d.h"
#include <QObject>
#include <QColor>

class Line3D : public Object3D
{
    Q_OBJECT
public:
    vec p1, p2;
    float width = 2;
    QColor color = QColor(Qt::black);

public:
    explicit Line3D(QObject *parent = nullptr);
    Line3D(vec p1, vec p2) : p1(p1), p2(p2) {init();}
    Line3D(vec p1, vec p2, QColor color) : p1(p1), p2(p2),color(color) {init();}
    void init();

    // Object3D interface
public:
    bool GetColorByRay(Ray &ray, QColor &color, float &dist) override;

    // Object3D interface
public:
    void RecalcBB() override;
};

#endif // LINE3D_H
