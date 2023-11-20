#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>


class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    ~MyGraphicsView();
protected:
    void wheelEvent(QWheelEvent* event) override;
private:
    int cntScale;
};

#endif // MYGRAPHICSVIEW_H
