#include "plane.h"
#include <cmath>
#include <QDebug>

\

Plane::Plane(QSize scrSize, QWidget *parent)
    : QWidget{parent}
{
    scene = new QGraphicsScene(this);
    view = new MyGraphicsView(scene, this);

    int x = scrSize.width() / 2;
    int y = scrSize.height() / 2;
    int div = 30;

    for (int i = -x; i <= x; i += div)
    {
        QGraphicsTextItem *text = scene->addText(QString::number(i / div));
        text->setPos(i, 0);
    }

    for (int i = -y; i <= y; i += div)
    {
        QGraphicsTextItem *text = scene->addText(QString::number(- i / div));
        text->setPos(0, i);
    }

    for (int i = -x; i <= x; i += div)
    {
        for (int h = -y; h <= y; h += div)
        {
            scene->addRect(i, h, div, div, QPen(Qt::gray));
        }
    }


    scene->addLine(-x, 0, x, 0, QPen(Qt::black));
    scene->addLine(0, -y, 0, y, QPen(Qt::black));

    view->setGeometry(QRect({0, 0}, scrSize));
}

void Plane::drawData(QVector<std::pair<QPoint, QPoint> >& data)
{
    QPen linesPen = QPen(Qt::red);
    linesPen.setWidth(3);
    for (auto it = ++data.rbegin(); it != data.rend(); it++)
    {
        scene->addLine(QLine(QPoint(it->first.x(), -it->first.y()) * 30, QPoint(it->second.x(), -it->second.y()) * 30), linesPen);
    }
}

void Plane::drawRect(QVector<std::pair<QPoint, QPoint> >& data)
{
    QPen rectPen(Qt::green, 3);
    auto it = data.rbegin();
    xmin = it->first.x();
    ymin = it->first.y();
    xmax = it->second.x();
    ymax = it->second.y();

    scene->addLine(QLine(QPoint(xmin, -ymin) * 30, QPoint(xmin, -ymax) * 30), rectPen);
    scene->addLine(QLine(QPoint(xmin * 30, -ymax * 30), QPoint(xmax, -ymax) * 30), rectPen);
    scene->addLine(QLine(QPoint(xmax, -ymax) * 30, QPoint(xmax, -ymin) * 30), rectPen);
    scene->addLine(QLine(QPoint(xmax, -ymin) * 30, QPoint(xmin, -ymin) * 30), rectPen);
}

void Plane::drawPolygon(QPolygon& polygon)
{
    QPen linesPen = QPen(Qt::green, 3);
    QPoint prev = *polygon.rbegin();
    for (auto it = ++polygon.rbegin(); it != polygon.rend(); it++)
    {
        scene->addLine(QLine(QPoint(it->x(), -it->y()) * 30, QPoint(prev.x(), -prev.y()) * 30), linesPen);
        prev = *it;
    }
}

QVector<bool> Plane::getCode(QPoint f)
{
    QVector<bool> res(4, false);
    res[0] = f.x() < xmin;
    res[1] = f.y() < ymin;
    res[2] = f.x() > xmax;
    res[3] = f.y() > ymax;
    return res;
}

bool Plane::checkCode(double x, double y)
{
    return (x >= xmin && y>=ymin && x <= xmax && y <= ymax);
}

void Plane::coen(QVector<std::pair<QPoint, QPoint> > data)
{
    drawData(data);
    drawRect(data);

    for (auto it = ++data.rbegin(); it != data.rend(); it++)
    {
        int a = it->second.y() - it->first.y();
        int b = -(it->second.x() - it->first.x());
        int c = - a * it->first.x() - b * it->first.y();
        QVector<bool> fCode = getCode(it->first);
        QVector<bool> sCode = getCode(it->second);
        bool checkAnd = 0;
        for (int i = 0; i < 4; i++)
        {
            checkAnd |= (fCode[i] && sCode[i]);
        }

        if (checkAnd)
        {
            continue;
        }

        QVector<std::pair<double, double> >ans;
        for (int i = 0; i < 4; i++)
        {
            double x;
            double y;
            if (fCode[i] || sCode[i])
            {
                switch (i)
                {
                case 0:
                    x = xmin;
                    y = (-a*x-c)/b;
                    if (checkCode(x, y))
                    {
                        ans.push_back({x, y});
                    }
                    break;
                case 1:
                    y = ymin;
                    x = (-b*y-c)/a;
                    if (checkCode(x, y))
                    {
                        ans.push_back({x, y});
                    }
                    break;
                case 2:
                    x = xmax;
                    y = (-a*x-c)/b;
                    if (checkCode(x, y))
                    {
                        ans.push_back({x, y});
                    }
                    break;
                case 3:
                    y = ymax;
                    x = (-b*y-c)/a;
                    if (checkCode(x, y))
                    {
                        ans.push_back({x, y});
                    }
                    break;
                }


            }
        }

        QPen linesPen(Qt::yellow, 3, Qt::DashDotLine);
        if (ans.size() == 0)
        {
            scene->addLine(QLine(QPoint(it->first.x(), -it->first.y()) * 30, QPoint(it->second.x(), -it->second.y()) * 30), linesPen);
        }
        else if (ans.size() == 1 && checkCode(it->first.x(), it->first.y()))
        {
            scene->addLine(QLine(QPoint(it->first.x(), -it->first.y()) * 30, QPoint((double)ans[0].first * 30, -1. * ans[0].second * (double)30)), linesPen);
        }
        else if (ans.size() == 1)
        {
            scene->addLine(QLine(QPoint(it->second.x(), -it->second.y()) * 30, QPoint((double)ans[0].first * 30, -1. * ans[0].second * (double)30)), linesPen);
        }
        else
        {
            scene->addLine(QLine(QPoint((double)ans[0].first * 30, -1. * ans[0].second * (double)30),
                                 QPoint((double)ans[1].first * 30, -1. * ans[1].second * (double)30)), linesPen);

        }
        //scene->addLine(QLine(QPoint(it->first.x(), -it->first.y()) * 30, QPoint(it->second.x(), -it->second.y()) * 30), linesPen);
    }
}

void Plane::convex(QVector<std::pair<QPoint, QPoint> >& data, QPolygon& polygon)
{
    drawData(data);
    drawPolygon(polygon);

    for (auto it = ++data.rbegin(); it != data.rend(); it++)
    {
        int a = it->second.x() - it->first.x();
        int b = it->second.y() - it->first.y();
        double tmin = 0., tmax = 1.;
        bool find = true;
        for (int i = 0; i < polygon.size() - 1; i++)
        {
            QPoint c1c2 = polygon[i + 1] - polygon[i];
            QPoint c1a = it->first - polygon[i];
            int v1 = c1c2.x() * b - c1c2.y() * a;
            int v2 = c1c2.x() * c1a.y() - c1c2.y() * c1a.x();
            if (v1 > 0)
            {
                double temp = -1.*(double)v2/v1;
                tmin = std::max(tmin, temp);
            }
            else if (v1 < 0)
            {
                double temp = -1.*(double)v2/v1;
                tmax = std::min(tmax, temp);
            }
            else if (v1 == 0 && v2 < 0)
            {
                find = false;
                break;
            }

            if (tmin > tmax)
            {
                find = false;
                break;
            }
        }

        QPen linesPen(Qt::yellow, 3, Qt::DashDotLine);
        if (find)
        {
            std::pair<double, double> ans1 = {it->first.x() + a * tmin, it->first.y() + b * tmin};
            std::pair<double, double> ans2 = {it->first.x() + a * tmax, it->first.y() + b * tmax};
            scene->addLine(QLine(QPoint((double)ans1.first * 30, -1. * ans1.second * (double)30),
                                 QPoint((double)ans2.first * 30, -1. * ans2.second * (double)30)), linesPen);
        }
    }
}
