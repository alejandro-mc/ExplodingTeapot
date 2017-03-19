#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QAction>
#include "mesh.h"
#include "explosiveentity.h"

#include "worldviewport.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadMesh();
    void runSimulation();


private:
    void createActions();
    void createMenus();

    QMenu         *m_menufile;
    QAction       *m_actionloadmodel;
    WorldViewport *m_worldviewport;

    Mesh          *m_teapot;
};

#endif // MAINWINDOW_H
