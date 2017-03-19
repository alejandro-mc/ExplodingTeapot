#include "worldviewport.h"
#include <QMatrix4x4>

enum{TEAPOT_VERTS,TEAPOT_FACES,INIT_VELOCITY,CENTER_MASS,ANG_VELOCITY};//used to reference VBOs
enum{PROJ,MODEL,VIEW,TIME,DELTA_T};//used for uniform locations
enum{TEAPOT,EXPLOSIVE_TEAPOT};
enum{ATT_VERTS,ATT_V0,ATT_CENTERMASS,ATT_ANG_VELOCITY};

WorldViewport::WorldViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
    //init time
    m_t = 0;

    //TODO connect mouse wheel signals


}

void WorldViewport::initializeGL(){

    initializeOpenGLFunctions();

    glClearColor(0.0,0.0,0.0,1.0);

    glGenBuffers(20,m_VBOs);
    glGenVertexArrays(2,m_VAOs);

    //initStationaryTeapotProgram();
    initSimulationProgram();



}
void WorldViewport::resizeGL(int w, int h){
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}
void WorldViewport::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT);

    //paintStatic();
    paintSimulation();

    timer.start(0, this);
}

void WorldViewport::timerEvent(QTimerEvent *e){
    incrementTime();
    update();
}

void WorldViewport::initStationaryTeapotProgram(){
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

    //populate teapot vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
    qDebug() << "Creating Buffer For: " << m_vertexcount << "objects.";
    glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_vertexdata,GL_STATIC_DRAW);

    glBindVertexArray(m_VAOs[TEAPOT]);

    //enable vertex attribute arrays
    glEnableVertexAttribArray(TEAPOT_VERTS);

    glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
    glVertexAttribPointer(TEAPOT_VERTS,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

    glUseProgram(0);
}
void WorldViewport::initSimulationProgram(){
        // init vertex and fragment shaders
        m_simulationprogram = new QOpenGLShaderProgram(this);

        //compile and add shaders to program
        m_simulationprogram->addShaderFromSourceFile(QOpenGLShader::Vertex,":/simulation.vert");
        m_simulationprogram->addShaderFromSourceFile(QOpenGLShader::Fragment,":/passthrough.frag");

        //bind attributes
        glBindAttribLocation(m_simulationprogram->programId(),ATT_VERTS,        "a_Vertex");
        glBindAttribLocation(m_simulationprogram->programId(),ATT_V0,           "a_initVelocity");
        glBindAttribLocation(m_simulationprogram->programId(),ATT_CENTERMASS,   "a_centerMass");
        glBindAttribLocation(m_simulationprogram->programId(),ATT_ANG_VELOCITY, "a_angularVelocity");


        //link opengl program
        m_simulationprogram->link();

        glUseProgram(m_simulationprogram->programId());

        //set uniforms
        m_uniformLocs[PROJ]     = glGetUniformLocation(m_simulationprogram->programId(),"u_proj");
        m_uniformLocs[VIEW]     = glGetUniformLocation(m_simulationprogram->programId(),"u_view");
        m_uniformLocs[MODEL]    = glGetUniformLocation(m_simulationprogram->programId(),"u_model");
        m_uniformLocs[TIME]     = glGetUniformLocation(m_simulationprogram->programId(),"u_time");
        //later: m_uniformLocs[DELTA_T] = glGetUniformLocation(m_simulationprogram->programId(),"u_delta");

        //populate buffers
        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
        glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_vertexdata,GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[INIT_VELOCITY]);
        glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_initVelocities,GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[CENTER_MASS]);
        glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_centersMass,GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[ANG_VELOCITY]);
        glBufferData(GL_ARRAY_BUFFER,3*m_vertexcount*sizeof(float),m_initAngVelocities,GL_STATIC_DRAW);


        glBindVertexArray(m_VAOs[EXPLOSIVE_TEAPOT]);

        //Enable vertex attribute arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);


        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[TEAPOT_VERTS]);
        glVertexAttribPointer(ATT_VERTS,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[INIT_VELOCITY]);
        glVertexAttribPointer(ATT_V0,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[CENTER_MASS]);
        glVertexAttribPointer(ATT_CENTERMASS,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

        glBindBuffer(GL_ARRAY_BUFFER,m_VBOs[ANG_VELOCITY]);
        glVertexAttribPointer(ATT_ANG_VELOCITY,3,GL_FLOAT,GL_FALSE,0,(GLvoid *) NULL);

        glUseProgram(0);
}

void WorldViewport::paintStatic(){
    glUseProgram(m_currentprogram->programId());
    glBindVertexArray(m_VAOs[TEAPOT]);

    m_view.setToIdentity();
    m_view.translate(0,-0.5,-10);
    m_view.rotate(-45,1,0,0);//roation along the x axis

    //set uniforms
    glUniformMatrix4fv(m_uniformLocs[PROJ],1,GL_FALSE,m_proj.constData());
    glUniformMatrix4fv(m_uniformLocs[VIEW],1,GL_FALSE,m_view.constData());

    qDebug() << "Vertex Count: " << m_vertexcount;
    glDrawArrays(GL_POINTS,0,m_vertexcount);

    if(glGetError() != 0)
        qDebug() << "Shader Error";



    glUseProgram(0);
}
void WorldViewport::paintSimulation(){

    glUseProgram(m_simulationprogram->programId());
    glBindVertexArray(m_VAOs[EXPLOSIVE_TEAPOT]);

    m_model.setToIdentity();
    m_model.translate(0,0,2);

    m_view.setToIdentity();
    m_view.translate(0,-0.5,-10);
    m_view.rotate(-45,1,0,0);//roation along the x axis

    //set uniforms
    glUniformMatrix4fv(m_uniformLocs[PROJ],1,GL_FALSE,m_proj.constData());
    glUniformMatrix4fv(m_uniformLocs[VIEW],1,GL_FALSE,m_view.constData());
    glUniformMatrix4fv(m_uniformLocs[MODEL],1,GL_FALSE,m_model.constData());
    glUniform1f(m_uniformLocs[TIME],m_t);

    qDebug() << "Vertex Count: " << m_vertexcount;
    glDrawArrays(GL_POINTS,0,m_vertexcount);

    if(glGetError() != 0)
        qDebug() << "Shader Error";



    glUseProgram(0);
}

void WorldViewport::addVertexData(void * vertexdata,int vertexcount){
    m_vertexdata  = vertexdata;
    m_vertexcount = vertexcount;
}

void WorldViewport::addFaces(void* indexes,int indexcount){
    m_indexes    = indexes;
    m_indexcount = indexcount;
}

void WorldViewport::addSimData(void* COM,void* initV, void* angV){
    m_centersMass       = COM;
    m_initVelocities    = initV;
    m_initAngVelocities = angV;
}


void WorldViewport::incrementTime(){
    m_t += 0.01f;
}
