#include "customgraphicsscene.h"
#include "QEvent"
#include "QKeyEvent"
#include "QGraphicsSceneMouseEvent"
#include "QDebug"
#include "QCoreApplication"

CustomGraphicsScene::CustomGraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
//    qApp->installEventFilter(this);
}

CustomGraphicsScene::~CustomGraphicsScene()
{
//    qApp->removeEventFilter(this);
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
    qDebug() << "Mouse pressed at " << event->scenePos();
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Mouse moved";
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Release mouse button";
}

void CustomGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
}
