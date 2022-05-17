#include "border.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

Border::Border()
{
//    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setZValue(-1);
}

Border::Border(QVector<QPoint> points)
    : points(points)
{
}


QRectF Border::boundingRect() const
{
    if (points.size() == 0) {
        return QRectF();
    }

    float minX = points.at(0).x() - borderRad;
    float maxX = points.at(0).x() + borderRad;
    float minY = points.at(0).y() - borderRad;
    float maxY = points.at(0).y() + borderRad;

    for (auto p : points) {
        minX = __min(minX, p.x() - borderRad);
        maxX = __max(maxX, p.x() + borderRad);
        minY = __min(minY, p.y() - borderRad);
        maxY = __max(maxY, p.y() + borderRad);
    }
    return QRectF(minX,minY, maxX-minX, maxY-minY);
}

void Border::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (points.size() == 0) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    painter->setPen(QPen(borderLineColor,2));
    for (int i = 0; i < points.size(); ++i) {
        painter->drawLine(points.at(i), points.at((i+1)%points.size()));
    }

    painter->setBrush(QBrush(borderPointBrush));
    for (int i = 0; i < points.size(); ++i) {
        painter->setPen(QPen(i == selected ? borderSelPointColor : borderPointColor,2));
        painter->drawEllipse(RectForCircle(points.at(i), borderRad));
    }
//    painter->drawRect(boundingRect());
}

void Border::AddPoint(QPoint point)
{
    prepareGeometryChange();
    int idx = 1;
    if (points.size() < 2) {
        points << point;
        return;
    }
    float2 vec = VecFromPoint(point);
    float minD = VecFromPoint(points.at(0)).Distance(vec) + VecFromPoint(points.at(1)).Distance(vec)
            - VecFromPoint(points.at(0)).Distance(VecFromPoint(points.at(1)));

    for (int i = 1; i < points.size(); ++i) {
        float dist = VecFromPoint(points.at(i)).Distance(vec) + VecFromPoint(points.at((i+1)%points.size())).Distance(vec)
                - VecFromPoint(points.at(i)).Distance(VecFromPoint(points.at((i+1)%points.size())));
        if (dist < minD) {
            minD = dist;
            idx = i+1;
        }
    }
    points.insert(idx, point);
}

bool Border::isBorderPoint(QPoint p)
{
    for (int i = 0; i < points.size(); ++i) {
        if (VecFromPoint(p).DistanceSq(VecFromPoint(points[i])) < borderRad*borderRad ) {
            return true;
        }
    }
    return false;
}

void Border::ClearSelection()
{
    selected = -1;
    scene()->update();
}

void Border::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "border receive mouse press";
    if (event->button() != Qt::MouseButton::LeftButton) {
        event->ignore();
        return;
    }

    if (!isBorderPoint(event->scenePos().toPoint())) {
//        qDebug() << "try to ignore by border";
        event->ignore();
//        QGraphicsItem::mousePressEvent(event);
        return;
    }

//    event->ignore();
    qDebug() << "item pressed at" << event->scenePos();
    for (int i = 0; i < points.size(); ++i) {
        if (VecFromPoint(event->scenePos()).DistanceSq(VecFromPoint(points[i])) < borderRad*borderRad ) {
            selected = i;
            scene()->update();
            return;
        }
    }
    selected = -1;
    scene()->update();
    event->ignore();
//    QGraphicsItem::mousePressEvent(event);
}

void Border::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isBorderPoint(event->lastScenePos().toPoint())) {
//        qDebug() << "try to ignore by border";
        event->ignore();
//        QGraphicsItem::mousePressEvent(event);
        return;
    }

    if (selected == -1) {
//        QGraphicsItem::mouseMoveEvent(event);
        event->ignore();
        return;
    }
    if (selected >= 0 && selected < points.size()) {
        points[selected] += event->scenePos().toPoint() - event->lastScenePos().toPoint();
        scene()->update();
    }
}

void Border::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
    selected = -1;
//    qDebug() << "mouse release by border";
}
QRect RectForCircle(QPoint center, int rad)
{
    return QRect(center.x() - rad, center.y() - rad, rad*2, rad*2);
}

float2 VecFromPoint(QPointF p)
{
    return float2(p.x(), p.y());
}


int Border::type() const
{
    return 2;
}
