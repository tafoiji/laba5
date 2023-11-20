#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), polygon()
{
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  scr = screen->geometry();
    screenSize = QSize(scr.width() / 30 * 30, scr.height() / 30 * 30);
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menu = new QMenu("Open..", this);
    QAction* opFile = new QAction("Open File", this);
    menu->addAction(opFile);
    connect(opFile, SIGNAL(triggered(bool)), this, SLOT(openFile(bool)));

    menuBar->addMenu(menu);
    this->setMenuBar(menuBar);

    koenBtn = new QPushButton("Cohen–Sutherland algorithm", this->centralWidget());
    connect(koenBtn, SIGNAL(pressed()), this, SLOT(coen()));

    convexBtn = new QPushButton("cutting off segments with a convex polygon algorithm", this->centralWidget());
    connect(convexBtn, SIGNAL(pressed()), this, SLOT(convex()));

    koenBtn->setDisabled(true);
    convexBtn->setDisabled(true);
    QVBoxLayout* layout = new QVBoxLayout(this->centralWidget());
    layout->addWidget(koenBtn);
    layout->addWidget(convexBtn);

    QString defautlFilePath = qApp->applicationDirPath() + "/segments.txt";
    readTextFile(defautlFilePath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile(bool)
{
    data.clear();
    polygon.clear();
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::AnyFile);
    QString fileName = fileDialog->getOpenFileName(this, tr("Open Image"), "", tr("Text File (*.txt)"));

    readTextFile(fileName);

}

void MainWindow::showMessage()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Data in file is not correct or file can't be opened");
    msgBox.setStandardButtons(QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    msgBox.exec();
}

void MainWindow::readTextFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        showMessage();
        koenBtn->setDisabled(true);
        convexBtn->setDisabled(true);
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList temp = line.split(" ");
        if (!in.atEnd())
        {
            bool b1, b2, b3, b4;

            if (temp.size() != 4)
            {
                showMessage();
                koenBtn->setDisabled(true);
                convexBtn->setDisabled(true);
                return;
            }

            data.push_back({QPoint(temp[0].toInt(&b1), temp[1].toInt(&b2)), QPoint(temp[2].toInt(&b3), temp[3].toInt(&b4))});
            if (b1 && b2 && b3 && b4 == false)
            {
                showMessage();
                koenBtn->setDisabled(true);
                convexBtn->setDisabled(true);
                return;
            }
        }
        else
        {
            if (temp.size() <= 5 || temp.size() % 2 == 1)
            {
                showMessage();
                koenBtn->setDisabled(true);
                convexBtn->setDisabled(true);
                return;
            }

            for (int i = 0; i < temp.size() - 1; i+=2)
            {
                bool b1, b2;
                polygon.push_back(QPoint(temp[i].toInt(&b1), temp[i + 1].toInt(&b2)));
                if (b1 && b2 == false)
                {
                    showMessage();
                    koenBtn->setDisabled(true);
                    convexBtn->setDisabled(true);
                    return;
                }
            }

            polygon.push_back(QPoint(temp[0].toInt(), temp[1].toInt()));
        }
    }

    if (data.size() < 2)
    {
        showMessage();
        koenBtn->setDisabled(true);
        convexBtn->setDisabled(true);
    }
    else
    {
        koenBtn->setDisabled(false);
        convexBtn->setDisabled(false);
    }

    file.close();
}

void MainWindow::coen()
{
    Plane* plane = new Plane(screenSize);
    plane->coen(data);
    plane->show();
}

void MainWindow::convex()
{
    Plane* plane = new Plane(screenSize);
    plane->convex(data, polygon);
    plane->show();
}
