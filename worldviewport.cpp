#include "worldviewport.h"
#include <QMatrix4x4>

enum{TEAPOT_VERTS,TEAPOT_FACES};
enum{TEAPOT};
enum{PROJ,MODEL,VIEW};

WorldViewport::WorldViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{

    //connect mouse wheel signals


}

void WorldViewport::initializeGL(){

    initializeOpenGLFunctions();

    glClearColor(0.0,0.0,0.0,1.0);

    // init vertex and fragment shaders
    m_currentprogram = new QOpenGLShaderProgram(this);

    //compile and add shaders to program
    m_currentprogram->addShaderFromSourceFile(QOpenGLShader::Vertex,":/dispteapot.vert");
    m_currentprogram->addShaderFromSourceFile(QOpenGLShader::Fragment,":/teapotpassthrough.frag");

    //bind attributes
    glBindAttribLocation(m_currentprogram->programId(),0,"a_Vertex");

    //link opengl program
    m_currentprogram->link();

    glUseProgram(m_currentprogram->programId());

    //set uniforms
    m_uniformLocs[PROJ] = glGetUniformLocation(m_currentprogram->programId(),"u_proj");
    m_uniformLocs[VIEW] = glGetUniformLocation(m_currentprogram->programId(),"u_view");

    glGenBuffers(20,m_VBOs);

    //populate teapot vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
    qDebug() << "Creating Buffer For: " << m_vertexcount << "objects.";
    glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_vertexdata,GL_STATIC_DRAW);

    glGenVertexArrays(1,m_VAOs);
    glBindVertexArray(m_VAOs[TEAPOT]);

    //enable vertex attribute arrays
    glEnableVertexAttribArray(TEAPOT_VERTS);

    glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
    glVertexAttribPointer(TEAPOT_VERTS,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

    glUseProgram(0);


}
void WorldViewport::resizeGL(int w, int h){
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 1000.0f);
}
void WorldViewport::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_currentprogram->programId());
    glBindVertexArray(m_VAOs[TEAPOT]);

    m_view.setToIdentity();
    m_view.translate(0,-0.5,-2);
    m_view.rotate(-45,1,0,0);//roation along the x axis

    //set uniforms
    glUniformMatrix4fv(m_uniformLocs[PROJ],1,GL_FALSE,m_proj.constData());
    glUniformMatrix4fv(m_uniformLocs[VIEW],1,GL_FALSE,m_view.constData());

    qDebug() << "Vertex Count: " << m_vertexcount;
    glDrawArrays(GL_POINTS,0,m_vertexcount);
    if(glGetError() != 0)
        qDebug() << "Shader Error";
    //glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glUseProgram(0);

}

void WorldViewport::addVertexData(void * vertexdata,int vertexcount){
    m_vertexdata = vertexdata;
    m_vertexcount = vertexcount;
}
