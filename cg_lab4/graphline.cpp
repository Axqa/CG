#include "graphline.h"
#include "MathEngine/somemath.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

GraphLine::GraphLine()
{

}

GraphLine::GraphLine(QPoint p1, QPoint p2, Border *border)
    :p1(p1), p2(p2), border(border)
{

}

GraphLine::GraphLine(QPoint p1, Border *border)
    : p1(p1), p2(p1), border(border)
{
    inMove = &p2;
    //    grabMouse();
}

bool GraphLine::isLinePoint(QPoint p)
{
    if (VecFromPoint(p).DistanceSq(VecFromPoint(p1)) < rad*rad ) {
        return true;
    }
    if (VecFromPoint(p).DistanceSq(VecFromPoint(p2)) < rad*rad ) {
        return true;
    }
    return false;
}


QRectF GraphLine::boundingRect() const
{
//    qDebug() << "calling line bounds, pos ="<< pos();
    return QRectF (
                       __min(p1.x()-rad, p2.x()-rad),
                       __min(p1.y()-rad, p2.y()-rad),
                       Abs(p1.x() - p2.x()) + rad * 2,
                       Abs(p1.y() - p2.y()) + rad * 2
                   );
}

void GraphLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);

    painter->setPen(QPen(line, 2));
    painter->drawLine(p1,p2);

    painter->setPen(QPen(colorBorder,2));
    painter->setBrush(QBrush(colorBrush));
    painter->drawEllipse(RectForCircle(p1, rad));
    painter->drawEllipse(RectForCircle(p2, rad));

    painter->setBrush(QBrush());
//    painter->drawRect(boundingRect());

    if (border->points.size() < 2) {
        return;
    }

    QList<float> ts;
    QList<QPair<float, float>>collinearInters;
    float t1,t2;
    float2 p1 = VecFromPoint(this->p1), p2 = VecFromPoint(this->p2);
    int rayIntersects = 0, lineIntersects = 0;

    for (int i = 0; i < border->points.size(); ++i) {
        float2 q1 = VecFromPoint(border->points[i]);
        float2 q2 = VecFromPoint(border->points[(i+1)%border->points.size()]);

        switch (IsIntersect(p1,p2,q1,q2,t1,t2)) {
        case NoIntersect:
            break;
        case OnePoint:
            ts << t1;
            rayIntersects++;
            lineIntersects++;
            break;
        case Collinear:
//            qDebug() << "collinear";
            collinearInters << QPair<float,float>{t1,t2};
            break;
        case RayIntersect:
            rayIntersects++;
            break;
        }
    }
    qDebug() << "ray ints=" << rayIntersects << "line ints=" << lineIntersects << ts;
    if (rayIntersects == 0) {
        return;
    }
    if (lineIntersects % 2 == 0 && rayIntersects % 2 == 0){
    } else if (lineIntersects % 2 == 0 && rayIntersects % 2 != 0) {
        ts << 0.0 << 1.0;
    } else if (lineIntersects % 2 != 0 && rayIntersects % 2 == 0) {
        ts << 1.0;
    } else if (lineIntersects % 2 != 0 && rayIntersects % 2 != 0) {
        ts << 0.0;
    }
    qDebug() << "ts after adding" << ts;
    qSort(ts);
    QList<float>::iterator i;
    painter->setPen(QPen(clippedColor,2));
    for (i = ts.begin(); (i+1) != ts.end() && (i) != ts.end(); i+=2) {
        float2 p_i = p1 + *i * (p2-p1);
        float2 p_i2= p1 + *(i+1) * (p2-p1);
        painter->drawLine(PointFromVec(p_i), PointFromVec(p_i2));
    }
    for (auto i : collinearInters) {
        float2 p_i = p1 + i.first * (p2-p1);
        float2 p_i2= p1 + i.second * (p2-p1);
        painter->drawLine(PointFromVec(p_i), PointFromVec(p_i2));
    }
}

void GraphLine::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "line recieve mouse press";
    if (!isLinePoint(event->scenePos().toPoint())) {
//        qDebug() << "trying to ignore by line";
//        QGraphicsItem::mousePressEvent(event);
        event->ignore();
        return;
    }
//    qDebug() << "line press event" << (long long)this;
//    qDebug() << "line" << p1 << p2 << "point:" << event->scenePos();
    if (VecFromPoint(event->scenePos()).DistanceSq(VecFromPoint(p1)) < rad*rad ) {
        inMove = &p1;
        return;
    }
    if (VecFromPoint(event->scenePos()).DistanceSq(VecFromPoint(p2)) < rad*rad ) {
        inMove = &p2;
        return;
    }

    inMove = nullptr;
//    event->ignore();
}

void GraphLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (inMove == nullptr) {
        event->ignore();
        return;
    }

    *inMove += event->scenePos().toPoint() - event->lastScenePos().toPoint();
    scene()->update();
}

void GraphLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    if (inMove) {
//        event->ignore();
//        return;
//    }
    inMove = nullptr;
//    ungrabMouse();
}


/// https://stackoverflow.com/a/565282
IntersectionType IsIntersect(float2 p1, float2 p2, float2 q1, float2 q2, float &resT, float &tForCollinear) {
    float2 p = p1, q = q1;
    float2 r = p2 - p1, s = q2 - q1;

    float t, u;

    float rs = r.Cross(s);
    if (rs == 0 && (q-p).Cross(r) == 0) {
        float t0 = (q - p).Dot(r) / (r.Dot(r));
        float t1 = t0 + (s.Dot(r)) / (r.Dot(r));

        if (s.Dot(r) < 0) {
            std::swap(t0,t1);
        }
        if (t0 > 1 || t1 < 0) {
            return NoIntersect;
        } else {
            t0 = __max(t0,0);
            t1 = __min(t1,1);

            resT = t0;
            tForCollinear = t1;
            return Collinear;
        }
    }
    if (rs == 0 && (q-p).Cross(r) != 0) {
        return NoIntersect;
    }

    t = (q-p).Cross(s) / rs;
    u = (q-p).Cross(r) / rs;

    if (rs != 0 && u >= 0 && u <= 1 && t >= 0) {
        if (t <= 1) {
            resT = t;
            return OnePoint;
        } else {
            resT = t;
            return RayIntersect;
        }
    }
    return NoIntersect;
}

int GraphLine::type() const
{
    return 1;
}

QPointF PointFromVec(float2 vec)
{
    return QPointF(vec.x, vec.y);
}
