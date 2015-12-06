#ifndef MESH_H
#define MESH_H

/*Mesh class use to store your face, halfEdge and Vertices.
 */

// ## Data Type ## //
//0. halfEdges - vector of all halfEdges
//1. vertices - vector of all vertices
//2. faces - vector of all faces

// ** Methods **//
//1. create, create VAO for vertices from faces

#include<shaderprogram.h>
#include<la.h>
#include<vector>
#include<halfedge.h>
#include<vertex.h>
#include<face.h>


class mesh : public ShaderProgram::Drawable
{
public:
    std::vector <halfEdge *> halfEdges;
    std::vector <vertex *> vertices;
    std::vector <face *> faces;

private: //for drawing
    int count;
    QOpenGLBuffer bufIdx;
    QOpenGLBuffer bufPos;
    QOpenGLBuffer bufNor;
    QOpenGLBuffer bufCol;

public:
    void create();
    void destroy();

    virtual GLenum drawMode();
    virtual int elemCount();
    virtual bool bindIdx();
    virtual bool bindPos();
    virtual bool bindNor();
    virtual bool bindCol();

    mesh();
    ~mesh();


    //HW5:special for resend data to GPU of updated values
    void update();
};

#endif // MESH_H
