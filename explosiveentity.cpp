#include "explosiveentity.h"
#include "vertex.h"

#include <QDebug>
#include <queue>

ExplosiveEntity::ExplosiveEntity(Mesh* mesh)
{
    m_mesh = mesh;
}

ExplosiveEntity::~ExplosiveEntity()
{
	//m_initVelocities
	if (m_initVelocities != NULL)
		delete[] m_initVelocities;
	
	if (m_centersMass != NULL)
		delete[] m_centersMass;

	if (m_shrapnel != NULL)
		delete[] m_shrapnel;
}

void ExplosiveEntity::genExplosionData(){
    breakMesh();
    computeCentersOfMass();
    getMeshCOM();
    assignVelocities();
    assignAngVelocities();
}

void* ExplosiveEntity::getCOMs(){
    return m_centersMass;
}
void* ExplosiveEntity::getVelocities(){
    return m_initVelocities;
}
void* ExplosiveEntity::getangVelocities(){
    return m_angVelocities;
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

    //create adjacency list to do BF walk over the vertices
    struct node {
        int partionNumber =0;
        vector<Face*> adjacentFaces;
    };

    unsigned int partCount = 0;

    node* adjList = new node[m_mesh->vertexCount() + 1];//vertices go from 1 to vertexCount inclusive

    //populate adj list
    Face*  faces     = (Face*) m_mesh->faceData();
    Face * faces_end=  faces + m_mesh->faceCount();
	//qDebug() << "mesh count:" << m_mesh->faceCount();
	//int fn = 0;
    for(Face* f = faces; f < faces_end;++f){
		//qDebug() << "processing face number: " << fn;
        adjList[f->vertex1].adjacentFaces.push_back(f);
        adjList[f->vertex2].adjacentFaces.push_back(f);
        adjList[f->vertex3].adjacentFaces.push_back(f);
		//++fn;
    }

    //partition vertices and remove faces between partitions
	unsigned int partNumber = 1;

    while(true){

        //look for vertices that haven't been visited that is partitionNumber == 0
        int i=1;
        for(i;i<=m_mesh->vertexCount();++i){
            if(adjList[i].partionNumber == 0){
                break;
            }
        }

        if(i > m_mesh->vertexCount()){//if all vertices have been visited
            break;
        }

        //traverse the graph in a BFS pattern starting from selected node

        //mark the rest of the vertices walk_distance away from the initialvertex
        unsigned int walk_distance = 4;
        queue<int> bfs_queues [2];
        uchar current_queue = 0;

        //push first vertex in to back of queue
        bfs_queues[current_queue].push(i);
        for(int k=0;k<walk_distance;++k){

            //mark the vertices in the queues
            while(bfs_queues[current_queue].size() > 0){

                int currentnode = bfs_queues[current_queue].front();
				bfs_queues[current_queue].pop();
                for(vector<Face*>::iterator it = adjList[currentnode].adjacentFaces.begin();it < adjList[currentnode].adjacentFaces.end();++it){
                    //mark vertces that have not been visited and add to next queue
                    Face* face = *it;
                    if(adjList[face->vertex1].partionNumber == 0){
                        adjList[face->vertex1].partionNumber = partNumber;
                        bfs_queues[!current_queue].push(face->vertex1);
                    }
                    if(adjList[face->vertex2].partionNumber == 0){
                        adjList[face->vertex2].partionNumber = partNumber;
                        bfs_queues[!current_queue].push(face->vertex2);
                    }
                    if(adjList[face->vertex3].partionNumber == 0){
                        adjList[face->vertex3].partionNumber = partNumber;
                        bfs_queues[!current_queue].push(face->vertex3);
                    }

                }

            }


            //swap queues
            current_queue = !current_queue;
        }

        //after walk is complete delete faces that are adjacent to nodes that where not visited
		//a more useful definition is that these are faces that have both visited and non-visited verteces
        
		//therefore
		//for every vertex visited while creating the current partition
		//if it has a face with non visited node remove that face from the list
		//we just need to check the faces that are in the current queue
		while (bfs_queues[current_queue].size() > 0) {
			int currentnode = bfs_queues[current_queue].front();
			bfs_queues[current_queue].pop();
			for (vector<Face*>::iterator it = adjList[currentnode].adjacentFaces.begin(); it < adjList[currentnode].adjacentFaces.end(); it=it) {
				Face* face = *it;
				if (adjList[face->vertex1].partionNumber != 0 && 
					adjList[face->vertex2].partionNumber != 0 &&
					adjList[face->vertex2].partionNumber != 0) {
					
					//face won't be deleted
					++it;
				}
				else {
					//delete face
					qDebug() << "current node:" << currentnode;
					it = adjList[currentnode].adjacentFaces.erase(it);
	
				}
			
			}
		}

        //increment partition number
        ++partNumber;

    }

    //put vertices in shrapnel array
	//allocate memory for the partitions
	m_shrapnel = new vector<unsigned int>[partNumber];//element 0 won't be used
	m_shrapnelCount = partNumber - 1;
	node current_node;
	for (int i=1; i <= m_mesh->vertexCount(); ++i) {//for every node in the adjacency list
		//add it to the correct partition in m_shrapnel
		current_node = adjList[i];
		m_shrapnel[current_node.partionNumber].push_back(i);
	}


    //put faces in faces array
    //CODE HERE

    delete [] adjList;

}


void ExplosiveEntity::computeCentersOfMass(){
	
	vec3D  com;
	unsigned int vertex_count;
	Vertex* verts = (Vertex*) m_mesh->vertexData();
	Vertex* vert_current;

	//allocate center of mass space
	m_centersMass = new vec3D[m_mesh->vertexCount()];

	//for debugging to make sure all the vertices being accessed
	set<int> vert_set;
	
	for (unsigned int i = 1; i <= m_shrapnelCount; ++i) {
		com = vec3D{0,0,0};
		vertex_count = 0;
		qDebug() << "loop 1:" << "i = " << i << " of " << m_shrapnelCount;
		//compute COM for current partition or projectile
		for (vector<unsigned int>::iterator it = m_shrapnel[i].begin(); it < m_shrapnel[i].end(); ++it) {
			
			vert_current = &verts[*it - 1];//*it - 1 because vertex indexes start from 1
			com.x += vert_current->x;
			com.y += vert_current->y;
			com.z += vert_current->z;

			++vertex_count;
		}

		com.x = com.x / vertex_count;
		com.y = com.y / vertex_count;
		com.z = com.z / vertex_count;

		//add COMs to the centers of mass vector
		//qDebug() << "loop 2:";
		for (vector<unsigned int>::iterator it = m_shrapnel[i].begin(); it < m_shrapnel[i].end(); ++it) {

			m_centersMass[*it - 1] = com;
			vert_set.insert(*it);
		}

	}

	//qDebug() << "vertices:" <<vert_set.size();

}
void ExplosiveEntity::assignVelocities(){

	m_initVelocities = new  vec3D[m_mesh->vertexCount()];

	vec3D* com = m_centersMass;
	vec3D* com_end = m_centersMass + m_mesh->vertexCount();
	vec3D* v0 = m_initVelocities;

	//compute velocities for each center of mass
	vec3D d;
	float magSqrd;
	for (; com < com_end; ++com) {
		
		//compute velocity vector
		d.x = com->x - m_meshCOM.x;
		d.y = com->y - m_meshCOM.y;
		d.z = com->z - m_meshCOM.z;

		magSqrd =0;
		if (d.x*d.x > 0.0001)
			magSqrd += d.x*d.x;
		if (d.y*d.y > 0.0001)
			magSqrd += d.y*d.y;
		if (d.z*d.z > 0.0001)
			magSqrd += d.z*d.z;

		v0->x = 10 * com->x / sqrt(magSqrd);
		v0->y = 10 * com->y / sqrt(magSqrd);
		v0->z = 10 * com->z / sqrt(magSqrd);

		++v0;
	}

	//qDebug() << "yeah!!";

    /*for(std::vector<Vertex>::iterator it = m_shrapnel.begin(); it != m_shrapnel.end();++it){

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

    }*/
}
void ExplosiveEntity::assignAngVelocities(){
	/*
    vec3D angVelocity;
    angVelocity.x = 0;
    angVelocity.y = 0;
    angVelocity.y = 0;

    for(std::vector<Vertex>::iterator it = m_shrapnel.begin(); it != m_shrapnel.end();++it){

        m_angVelocities.push_back(angVelocity);
    }
	*/
}
