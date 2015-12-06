#ifndef VERTEX_H
#define VERTEX_H

// ## Data Type ## //
// 0. pos - vec4 type and it represents position of this vertex
// 1. edge - points to any halfedge that points to this vertex
// 2. id - integer, unique identifier

#include<halfedge.h>
#include<la.h>
#include<QListWidgetItem>

class halfEdge; //Known as forward declaration, learn from stack overflow

class vertex : public QListWidgetItem
{
public:
    //0
    glm::vec4 pos;
    //1
    halfEdge *edge;
    //2
    int id;

public:
    vertex(int inId);
    vertex(int inId, float x, float y, float z);
    ~vertex();
};

#endif // VERTEX_H
