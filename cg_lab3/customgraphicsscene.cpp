#include "customgraphicsscene.h"
#include "QEvent"
#include "QKeyEvent"
#include "QGraphicsSceneMouseEvent"
#include "QDebug"
#include "QCoreApplication"
#include "QPainter"
#include <QGraphicsItem>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QColor(192,192,192));
     setItemIndexMethod(NoIndex);
//    qApp->installEventFilter(this);
}

CustomGraphicsScene::~CustomGraphicsScene()
{
//    qApp->removeEventFilter(this);
    for (auto i : items()) {
        delete i;
    }
}


bool CustomGraphicsScene::eventFilter(QObject *watched, QEvent *event)
{
//    if (event->type() == QEvent::KeyPress)
//    {
////        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

//        return false;
//    }
//    else if (event->type() == QEvent::MouseButtonPress) {
////        QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent *>(event);
//        QMouseEvent *me = static_cast<QMouseEvent *>(event);

//        if (me->button() == Qt::LeftButton) {
//                qDebug() << "Mouse pressed in " << me->pos();
//                return true;
//            }
//        return false;
//    }
//    else if (event->type() == QEvent::MouseButtonRelease) {
//        qDebug() << "Mouse released";
//        return true;
//    }
//    else if (event->type() == QEvent::MouseMove) {
////        qDebug() << "Mouse move";
//        return false;
//    }
//    else
        return QObject::eventFilter(watched, event);

}


void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "Mouse pressed at " << event->scenePos() << items(event->scenePos());
    btnPressed = event->button();
    emit mousePress(event->button(), event->scenePos());
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "Mouse moved" << event->buttons() << "from" << event->lastScenePos() << "to" << event->scenePos();
    emit mouseMove(event->buttons(), event->lastScenePos(), event->scenePos());
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "Release mouse button";
    btnPressed = Qt::NoButton;
    emit mouseRelease(event->button());
}

void CustomGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
//    qDebug() << "Wheel event";
}


