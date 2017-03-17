#ifndef WORLDVIEWPORT_H
#define WORLDVIEWPORT_H
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class WorldViewport : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    WorldViewport(QWidget* parent=0);
    void addVertexData(void *,int);

protected:
    void initializeGL();
    void resizeGL(int,int);
    void paintGL();

    QOpenGLShaderProgram* m_currentprogram;

private:
    GLuint m_VBOs[100];
    GLuint m_VAOs[100];

    void*  m_vertexdata;
    int    m_vertexcount;

    //model view and projectionmatrices
    GLint m_uniformLocs[20];
    QMatrix4x4 m_model;
    QMatrix4x4 m_view;
    QMatrix4x4 m_proj;
};

#endif // WORLDVIEWPORT_H
