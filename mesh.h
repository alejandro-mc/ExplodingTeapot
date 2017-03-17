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

private:
    void addVertexFromFile(QByteArray,QByteArray,QByteArray);
    void addNormalFromFile(QByteArray,QByteArray,QByteArray);
    void addFaceFromFile(QByteArray,QByteArray,QByteArray);
    void addTextCoordFromFile(QByteArray,QByteArray);

    vector<Vertex>   m_vertices;
    vector<TexCoord> m_texcoords;
    vector<Normal>   m_normals;
    vector<Face>     m_faces;

};

#endif // MESH_H
