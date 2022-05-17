#ifndef GRAPHLINE_H
#define GRAPHLINE_H

#include <QGraphicsItem>
#include <QObject>
#include "border.h"

class GraphLine : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    GraphLine();
    GraphLine(QPoint p1, QPoint p2, Border* border);
    GraphLine(QPoint p1, Border* border);

    bool isLinePoint(QPoint p);

public:
    QPoint p1,p2;
    QPoint *inMove = nullptr;
    Border *border;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;



private:
    int rad = 10;
    QColor line = QColor(Qt::black);
    QColor colorBorder = QColor(Qt::black);
    QColor colorBrush = QColor(Qt::gray);

    QColor clippedColor = QColor(Qt::green);

    // QGraphicsItem interface
public:
    int type() const override;
};

enum IntersectionType {
    NoIntersect,
    OnePoint,
    Collinear,
    RayIntersect
};

IntersectionType IsIntersect(float2 p1, float2 p2, float2 q1, float2 q2, float &resT, float &tForCollinear) ;
QPointF PointFromVec(float2 vec);


#endif // GRAPHLINE_H
