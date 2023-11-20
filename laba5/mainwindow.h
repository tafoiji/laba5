#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <plane.h>
#include <QScreen>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void readTextFile(const QString& filePath);
    void showMessage();
    QVector<std::pair<QPoint, QPoint> > data;
    QPolygon polygon;
    QPushButton* koenBtn;
    QPushButton* convexBtn;
    QSize screenSize;
public slots:
    void openFile(bool);
    void coen();
    void convex();
};
#endif // MAINWINDOW_H
