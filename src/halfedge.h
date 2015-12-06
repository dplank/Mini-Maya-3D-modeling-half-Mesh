#ifndef HALFEDGE_H
#define HALFEDGE_H

// ## Data Type ## //
//0. face - points to face of its left
//1. vert - points to vertex lie between this and next halfEdge
//2. next - points to halfEdge next to this halfEdge
//3. sym - points to symmetric halfEdge to this (that lie parallel to this and travell opposite and part of adjecent face)
//4. id - integer, unique identifier

#include<face.h>
#include<vertex.h>
#include<halfedge.h>
#include<QListWidgetItem>


class face; //Known as forward declaration, learn from stack overflow
class vertex;
class halfEdge;

class halfEdge  : public QListWidgetItem
{
public:
    //0
    face *Face;
    //1
    vertex *vert;
    //2
    halfEdge *next;
    //3
    halfEdge *sym;
    //4
    int id;

public:
    halfEdge(int inid);
    ~halfEdge();
};

#endif // HALFEDGE_H
