#ifndef BORDER_H
#define BORDER_H

#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
#include "MathEngine/float2.h"

class Border : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Border();
    Border(QVector<QPoint> points);
public:
    QVector<QPoint> points;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void AddPoint(QPoint point);
    bool isBorderPoint(QPoint p);

public slots:
    void ClearSelection();

public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int borderRad = 10;
    QColor borderPointColor = QColor(Qt::red);
    QColor borderSelPointColor = QColor(Qt::blue);
    QColor borderPointBrush = QColor(Qt::gray);
    QColor borderLineColor = QColor(Qt::red);
    int selected = -1;


    // QGraphicsItem interface
public:
    int type() const override;
};

QRect RectForCircle(QPoint center, int rad);
float2 VecFromPoint(QPointF p);

#endif // BORDER_H
