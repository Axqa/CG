#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H
#include <QGraphicsScene>
#include <QObject>

class CustomScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CustomScene(QObject *parent = nullptr);
    ~CustomScene();
signals:
    void sceneMousePressed(QPoint p, Qt::MouseButton btn);
    void sceneMouseReleased(QPoint p, Qt::MouseButton btn);
    void sceneMouseMoved(QPoint from, QPoint to);


    // QGraphicsScene interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CUSTOMSCENE_H
