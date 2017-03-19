#ifndef MESH_H
#define MESH_H
#include <vector>
#include <vertex.h>
#include <QString>

using namespace std;

class Mesh
{
public:
    Mesh();
    void loadDataFromFile(QString);
    void* vertexData();
    int vertexCount();

    vector<Vertex>   m_vertices;//TEMP made public

private:
    void addVertexFromFile(QByteArray,QByteArray,QByteArray);
    void addNormalFromFile(QByteArray,QByteArray,QByteArray);
    void addFaceFromFile(QByteArray,QByteArray,QByteArray);
    void addTextCoordFromFile(QByteArray,QByteArray);


    vector<TexCoord> m_texcoords;
    vector<Normal>   m_normals;
    vector<Face>     m_faces;

};

#endif // MESH_H
