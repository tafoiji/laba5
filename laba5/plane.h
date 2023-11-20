#ifndef PLANE_H
#define PLANE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QWidget>
#include <QFile>
#include "mygraphicsview.h"

class Plane : public QWidget
{
    Q_OBJECT
public:
    explicit Plane(QSize scrSize = QSize(1920, 1080), QWidget *parent = nullptr);
    void iterations(QPoint f, QPoint s);
    void dda(QPoint f, QPoint s);
    void bresenham(QPoint f, QPoint s);
    void circle(QPoint f, int radius);
    void coen(QVector<std::pair<QPoint, QPoint> >);
    void convex(QVector<std::pair<QPoint, QPoint> >& data, QPolygon& polygon);


private:
    QGraphicsScene *scene;
    MyGraphicsView *view;
    void drawData(QVector<std::pair<QPoint, QPoint> >& data);
    void drawRect(QVector<std::pair<QPoint, QPoint> >& data);
    void drawPolygon(QPolygon& polygon);

    QVector<bool> getCode(QPoint f);
    bool checkCode(double x, double y);
    int xmin;
    int ymin;
    int xmax;
    int ymax;
public:
    QFile data;

signals:

};

#endif // PLANE_H
