#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QObject>
#include <QLabel>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = nullptr);

    // QPaintDevice interface
public:
    QPaintEngine *paintEngine() const override;

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void mousePress     (QMouseEvent *event)  ;
    void mouseRelease   (QMouseEvent *event);
    void mouseMove      (QMouseEvent *event)   ;
    void wheel          (QWheelEvent *event)       ;
    void keyPress       (QKeyEvent *event)      ;
    void keyRelease     (QKeyEvent *event)    ;
    void resize         (QResizeEvent *event)     ;

};

#endif // IMAGELABEL_H
