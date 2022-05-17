#include "customscene.h"
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QCursor>
#include "graphline.h"
#include "border.h"

CustomScene::CustomScene(QObject *parent) : QGraphicsScene(parent)
{
    setBackgroundBrush(QColor(Qt::gray));
}

CustomScene::~CustomScene()
{
    for (auto i : items()) {
        delete i;
    }
}


void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//    qDebug() << "scene mouse press at" << event->scenePos() << "items:" << items(event->scenePos())
//             << "item" << itemAt(event->scenePos(), QTransform());
    sceneMousePressed(( event->scenePos()).toPoint(),event->button());
//    QGraphicsScene::mousePressEvent(event);

//    if (items(event->scenePos()).size() == 0)
    {
//    if (1) {
        for (auto i : items()) {
//            qDebug() << i->boundingRect() << i->type();
            if (i->type() == 1) {
//                if (i->boundingRect().contains(event->scenePos())) {
                    GraphLine *line = (GraphLine*)i;
                    line->mousePressEvent(event);
//                }
            } else if (i->type() == 2){
                Border * border = (Border*)i;
                border->mousePressEvent(event);
            }
        }
    }
//    event->ignore();
}


void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    sceneMouseMoved(event->lastScenePos().toPoint(), event->scenePos().toPoint());
    QGraphicsScene::mouseMoveEvent(event);

    //    if (items(event->scenePos()).size() == 0)
        {
    //    if (1) {
            for (auto i : items()) {
//                qDebug() << i->boundingRect() << i->type();
                if (i->type() == 1) {
    //                if (i->boundingRect().contains(event->scenePos())) {
                        GraphLine *line = (GraphLine*)i;
                        line->mouseMoveEvent(event);
    //                }
                } else if (i->type() == 2){
                    Border * border = (Border*)i;
                    border->mouseMoveEvent(event);
                }
            }
        }
}

void CustomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    sceneMouseReleased(event->scenePos().toPoint(),event->button());
    QGraphicsScene::mouseReleaseEvent(event);

    //    if (items(event->scenePos()).size() == 0)
        {
    //    if (1) {
            for (auto i : items()) {
//                qDebug() << i->boundingRect() << i->type();
                if (i->type() == 1) {
    //                if (i->boundingRect().contains(event->scenePos())) {
                        GraphLine *line = (GraphLine*)i;
                        line->mouseReleaseEvent(event);
    //                }
                } else if (i->type() == 2){
                    Border * border = (Border*)i;
                    border->mouseReleaseEvent(event);
                }
            }
        }
}
