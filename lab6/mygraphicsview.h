#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>

namespace Ui {
class MyGraphicsView;
}

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyGraphicsView(QWidget *parent = nullptr);
    ~MyGraphicsView();

private:
    Ui::MyGraphicsView *ui;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MYGRAPHICSVIEW_H
