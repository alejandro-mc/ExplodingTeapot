#ifndef WORLDVIEWPORT_H
#define WORLDVIEWPORT_H
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QBasicTimer>

class WorldViewport : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    WorldViewport(QWidget* parent=0);
    void addVertexData(void *,int);
    void addFaces(void*,int);

    void addSimData(void*,void*,void*);

    void incrementTime();

protected:
    void initializeGL();
    void resizeGL(int,int);
    void paintGL();

    void timerEvent(QTimerEvent *e);

    QOpenGLShaderProgram* m_currentprogram;
    QOpenGLShaderProgram* m_simulationprogram;

private:
    void initStationaryTeapotProgram();
    void initSimulationProgram();

    void paintStatic();
    void paintSimulation();

    QBasicTimer timer;

    GLuint m_VBOs[100];
    GLuint m_VAOs[100];

    void*  m_vertexdata;
    int    m_vertexcount;
    void*  m_indexes;
    int    m_indexcount;

    //attribute data pointers for physics simulation
    void*  m_centersMass;
    void*  m_initVelocities;
    void*  m_initAngVelocities;
    float  m_t; //time
    float  m_dt;//delta t


    //model view and projectionmatrices
    GLint m_uniformLocs[20];
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_proj;
};

#endif // WORLDVIEWPORT_H
