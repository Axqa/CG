#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>

class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = nullptr);
    ~CustomGraphicsScene();

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;

    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

signals:
    void mousePress(Qt::MouseButton btn, QPointF pos);
    void mouseRelease(Qt::MouseButton btn);
    void mouseMove(Qt::MouseButtons btns, QPointF from, QPointF to);

private:
    Qt::MouseButton btnPressed = Qt::NoButton;
    // QGraphicsScene interface
//protected:
//    void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget) override;
};


#endif // CUSTOMGRAPHICSSCENE_H
