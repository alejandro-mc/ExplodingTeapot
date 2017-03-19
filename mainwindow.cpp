#include <QMenuBar>
#include <QFileDialog>
#include <QTime>

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

    //create exploding teapot entity
    ExplosiveEntity ex_teapot = ExplosiveEntity(m_teapot);
    qDebug() << "Generating explosion data...";
    ex_teapot.genExplosionData();
    qDebug() << "Done.";

    m_worldviewport = new WorldViewport();
    m_worldviewport->addVertexData(m_teapot->vertexData(),m_teapot->vertexCount());
    m_worldviewport->addSimData(ex_teapot.getCOMs(),ex_teapot.getVelocities(),ex_teapot.getangVelocities());

    this->setCentralWidget(m_worldviewport);
}

void MainWindow::runSimulation(){

}
