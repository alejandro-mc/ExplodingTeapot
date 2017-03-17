#include "mesh.h"
#include <QFile>
#include <QDebug>

Mesh::Mesh()
{

}

//Mesh::loadDataFromFile
void Mesh::loadDataFromFile(QString filename){
    QFile objFile(filename);

    if (!objFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!objFile.atEnd()){
        QByteArray line = objFile.readLine();

        QList<QByteArray> tokens = line.replace('\n',"").split(' ');
		
		if (tokens.at(0) == "v") {
			qDebug() << line;
			addVertexFromFile(tokens.at(1),tokens.at(2),tokens.at(3));
		}
		else if (tokens.at(0) == "vt") {
			qDebug() << line;
			addTextCoordFromFile(tokens.at(1),tokens.at(2));
			
		}
		else if (tokens.at(0) == "vn") {
			qDebug() << line;
			addNormalFromFile(tokens.at(1),tokens.at(2),tokens.at(3));
			
		}
		else if (tokens.at(0) == "f") {
			qDebug() << line;
			addFaceFromFile(tokens.at(1),tokens.at(2),tokens.at(3));
			
		}

		
    }
    qDebug() <<"Normal Count: "<< m_normals.size();

}


void Mesh::addVertexFromFile(QByteArray x,QByteArray y,QByteArray z){
    Vertex vert;
    vert.x = x.toFloat();
    vert.y = y.toFloat();
    vert.z = z.toFloat();
    m_vertices.push_back(vert);
}
void Mesh::addNormalFromFile(QByteArray x,QByteArray y,QByteArray z){
    Normal norm;
    norm.x = x.toFloat();
    norm.y = y.toFloat();
    norm.z = z.toFloat();
    m_normals.push_back(norm);
}
void Mesh::addFaceFromFile(QByteArray v1,QByteArray v2,QByteArray v3){
    //parse vertex data
    QList<QByteArray> v1_data = v1.split('/');
    QList<QByteArray> v2_data = v2.split('/');
    QList<QByteArray> v3_data = v3.split('/');

    Face fc;
    //add vertex indexes
    fc.vertex1 = v1_data.at(0).toInt();
    fc.vertex2 = v2_data.at(0).toInt();
    fc.vertex3 = v3_data.at(0).toInt();

    //TODO: Load texture data

    //add normal indexes
    fc.normal1 = v1_data.at(2).toInt();
    fc.normal2 = v2_data.at(2).toInt();
    fc.normal3 = v3_data.at(2).toInt();

    m_faces.push_back(fc);

}

void Mesh::addTextCoordFromFile(QByteArray s,QByteArray t){
    TexCoord tcoord;
    tcoord.s = s.toFloat();
    tcoord.t = t.toFloat();
    m_texcoords.push_back(tcoord);
}


void* Mesh::vertexData(){
    return m_vertices.data();
}

int Mesh::vertexCount(){
    return m_vertices.size();
}
