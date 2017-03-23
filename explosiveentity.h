#ifndef EXPLOSIVEENTITY_H
#define EXPLOSIVEENTITY_H

#include <map>
#include "mesh.h"
#include <set>

class ExplosiveEntity
{
public:
    ExplosiveEntity(Mesh*);
	~ExplosiveEntity();
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
    unsigned int   m_shrapnelCount;


    //mesh properties
    Mesh                   *m_mesh;
    vec3D                   m_meshCOM;//mesh center of mass to find direction of init velocity vector
    vector<unsigned int>*   m_shrapnel;//will contain partition of vertices and faces
                               //that will act as rigid projectiles

    multimap<pair<int,int>,Face*> m_edges;//keeps the faces indexed by edges. used for finding adjacent faces


    //properties to be shared among vertices in one piece of shrapnel
    vec3D*                  m_centersMass;
    vec3D*                  m_initVelocities;
    vec3D*                  m_angVelocities;

};

#endif // EXPLOSIVEENTITY_H
