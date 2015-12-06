#ifndef FACE_H
#define FACE_H


// ## Data Type ## //
//0. startEdge -  point to (arbitrary) halfEdge of this polygon
//1. color -  vec4, represent color of this face
//2. id - integer, unique identifiers

#include<halfedge.h>
#include<la.h>
#include <QListWidgetItem>

class halfEdge;   //Known as forward declaration, learn from stack overflow

class face : public QListWidgetItem
{

public:
    //0
    halfEdge *startEdge;
    //1
    glm::vec4 color;
    //2
    int id;
public:
    face(int inId);
    ~face();
};

#endif // FACE_H
