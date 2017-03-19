#ifndef EXPLOSIVEENTITY_H
#define EXPLOSIVEENTITY_H

#include "mesh.h"

class ExplosiveEntity
{
public:
    ExplosiveEntity(Mesh*);
    //void setShrapnelCount();
    void genExplosionData();

    void* getCOMs();
    void* getVelocities();
    void* getangVelocities();

private:

    void getMeshCOM();
    void breakMesh();
    void computeCentersOfMass();
    void assignVelocities();
    void assignAngVelocities();

    //explosive object parameter
    //unsigned int   m_shrapnelCount;


    //mesh properties
    Mesh           *m_mesh;
    vec3D          m_meshCOM;//mesh center of mass to find direction of init velocity vector
    vector<Vertex>  m_shrapnel;//will contain partition of vertices and faces
                               //that will act as rigid projectiles

    //properties to be shared among vertices in one piece of shrapnel
    vector<vec3D>  m_centersMass;
    vector<vec3D>  m_initVelocities;
    vector<vec3D>  m_angVelocities;

};

#endif // EXPLOSIVEENTITY_H
