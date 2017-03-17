#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QFileDialog>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setMinimumWidth(800);
    w.setMinimumHeight(500);
    w.show();

    return a.exec();
}
