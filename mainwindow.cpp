#include <QMenuBar>
#include <QFileDialog>


#include "mainwindow.h"
#include "mesh.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    //delete memory

}


void MainWindow::createActions(){

    m_actionloadmodel = new QAction(tr("&Open"),this);
    m_actionloadmodel->setShortcuts(QKeySequence::Open);
    connect(m_actionloadmodel,SIGNAL(triggered(bool)),this,SLOT(loadMesh(void)));

}


void MainWindow::createMenus(){
    m_menufile = menuBar()->addMenu(tr("&File"));
    m_menufile->addAction(m_actionloadmodel);
}


void MainWindow::loadMesh(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Meshes (*.obj)"));

    m_teapot = new Mesh();
    m_teapot->loadDataFromFile(fileName);

    m_worldviewport = new WorldViewport();
    m_worldviewport->addVertexData(m_teapot->vertexData(),m_teapot->vertexCount());

    this->setCentralWidget(m_worldviewport);
}
