#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <mygraphicsview.h>


MyGraphicsView:: MyGraphicsView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(parent), cntScale(1)
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void MyGraphicsView:: wheelEvent(QWheelEvent* event)
{
    qreal scaleFactor = 1.15;

    if (event->delta() > 0)
    {
        scale(scaleFactor, scaleFactor);
        cntScale++;
    }

    else if (cntScale > 0)
    {
        scale(1 / scaleFactor, 1 / scaleFactor);
        cntScale--;
    }
}


MyGraphicsView::~MyGraphicsView() {}
