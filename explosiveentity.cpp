#include "explosiveentity.h"
#include "vertex.h"

#include <QDebug>

ExplosiveEntity::ExplosiveEntity(Mesh* mesh)
{
    m_mesh = mesh;
}

void ExplosiveEntity::genExplosionData(){
    breakMesh();
    computeCentersOfMass();
    getMeshCOM();
    assignVelocities();
    assignAngVelocities();
}

void* ExplosiveEntity::getCOMs(){
    return m_centersMass.data();
}
void* ExplosiveEntity::getVelocities(){
    return m_initVelocities.data();
}
void* ExplosiveEntity::getangVelocities(){
    return m_angVelocities.data();
}


void ExplosiveEntity::getMeshCOM(){
    //TEMP IMPLEMENTATION
    float sumx,sumy,sumz;
    sumx =0;
    sumy =0;
    sumz =0;

    Vertex currentVertex;

    for(std::vector<Vertex>::iterator it = m_mesh->m_vertices.begin(); it != m_mesh->m_vertices.end();++it){
        currentVertex = *it;

        sumx += currentVertex.x;
        sumy += currentVertex.y;
        sumz += currentVertex.z;
    }

    m_meshCOM.x = sumx / m_mesh->vertexCount();
    m_meshCOM.y = sumy / m_mesh->vertexCount();
    m_meshCOM.z = sumz / m_mesh->vertexCount();

    //m_meshCOM.x=0;
    //m_meshCOM.y=0;
    //m_meshCOM.z=-1;

}

void ExplosiveEntity::breakMesh(){

    //TEMP IMPLEMENTATION
    for(std::vector<Vertex>::iterator it = m_mesh->m_vertices.begin(); it != m_mesh->m_vertices.end();++it){
        m_shrapnel.push_back(*it);
    }
}
void ExplosiveEntity::computeCentersOfMass(){
    vec3D  currentCOM;
    Vertex currentVertex;
    for(std::vector<Vertex>::iterator it = m_shrapnel.begin(); it != m_shrapnel.end();++it){

        currentVertex = *it;

        currentCOM.x = currentVertex.x;
        currentCOM.y = currentVertex.y;
        currentCOM.z = currentVertex.z;

        m_centersMass.push_back(currentCOM);

    }
}
void ExplosiveEntity::assignVelocities(){


    for(std::vector<Vertex>::iterator it = m_shrapnel.begin(); it != m_shrapnel.end();++it){

        Vertex currentVertex;
        vec3D currentVelocity;
        vec3D d;
        float magSqrd=0;

        currentVertex = *it;

        //compute velocity vector
        d.x = currentVertex.x - m_meshCOM.x;
        d.y = currentVertex.x - m_meshCOM.y;
        d.z = currentVertex.x - m_meshCOM.z;

        if(d.x*d.x > 0.0001)
            magSqrd += d.x*d.x;
        if(d.y*d.y > 0.0001)
            magSqrd += d.y*d.y;
        if(d.z*d.z > 0.0001)
            magSqrd += d.z*d.z;

        currentVelocity.x = 10 * currentVertex.x / sqrt(magSqrd);
        currentVelocity.y = 10 * currentVertex.y / sqrt(magSqrd);
        currentVelocity.z = 10 * currentVertex.z / sqrt(magSqrd);

        qDebug() << "magsqrd:" << magSqrd;
        qDebug() << "<" << currentVelocity.x << "," << currentVelocity.y << "," << currentVelocity.z << ">";
        m_initVelocities.push_back(currentVelocity);

    }
}
void ExplosiveEntity::assignAngVelocities(){
    vec3D angVelocity;
    angVelocity.x = 0;
    angVelocity.y = 0;
    angVelocity.y = 0;

    for(std::vector<Vertex>::iterator it = m_shrapnel.begin(); it != m_shrapnel.end();++it){

        m_angVelocities.push_back(angVelocity);
    }
}
