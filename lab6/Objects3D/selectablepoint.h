#ifndef SELECTABLEPOINT_H
#define SELECTABLEPOINT_H

#include "point3.h"
#include <QObject>
#include "selectable.h"

class SelectablePoint : public Point3, public Selectable
{
    Q_OBJECT
public:

    Point3 projX;
    Point3 projY;
    Point3 projZ;
    float projRad = 3;
    QColor projColor = Qt::gray;
    QColor projLineCol = Qt::yellow;

    Point3 unitX;
    Point3 unitY;
    Point3 unitZ;
    float unitRad = 1;
    QColor supColor = Qt::gray;

public:
    SelectablePoint();
    SelectablePoint(float x, float y, float z) : Point3(x,y,z) {initProj();selected = false;}
    SelectablePoint(float x, float y, float z, QColor color): Point3(x,y,z,color) {initProj();selected = false;}
    SelectablePoint(float x, float y, float z, float rad): Point3(x,y,z, rad) {initProj();selected = false;}
    SelectablePoint(float x, float y, float z, float rad, QColor color): Point3(x,y,z,rad,color) {initProj();selected = false;}

    void initProj();

    // Object3D interface
public:
    QGraphicsItemGroup *DrawOnCameraView(Camera &cam) override;
    MatrixF ToMatrix() override;
    void FromMatrix(MatrixF m) override;
    void Normalize() override;

    QGraphicsItemGroup* ToGraphGroup(Camera &cam);

    // Selectable interface
public:
    void setSelected(bool state) override;
    bool isIntersects(const Ray &ray, float &dist) override;

    void MovingRay(Ray &from, Ray &to) override;
public slots:
    float MousePressRay(Ray &ray);
signals:
    void SelectionChanged(bool state);
    void PosChanged(vec pos);

};

#endif // SELECTABLEPOINT_H
