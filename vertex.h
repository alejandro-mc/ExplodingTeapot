#ifndef VERTEX_H
#define VERTEX_H
#include <vector>

struct vec3D{
    float x;
    float y;
    float z;
};

/*
struct vec2D{
    float x;
    float y;
};*/


struct Face{
    int vertex1;
    int vertex2;
    int vertex3;

    int tcoord1;
    int tcoord2;
    int tcoord3;

    int normal1;
    int normal2;
    int normal3;
};

struct Vertex{
    float x;
    float y;
    float z;
};

struct TexCoord{
    float s;
    float t;
};

struct Normal{
    float x;
    float y;
    float z;
};



#endif // VERTEX_H
