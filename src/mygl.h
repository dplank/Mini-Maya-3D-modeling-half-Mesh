#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <glwidget277.h>
#include <la.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include <scene/camera.h>
#include <la.h>

//HW5
#include <mesh.h>
#include<QListWidgetItem>

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    QOpenGLVertexArrayObject vao;

    Cylinder geom_cylinder;
    Sphere geom_sphere;
    ShaderProgram prog_lambert;
    ShaderProgram prog_wire;

    Camera camera;

    //HW5
    mesh cubeMesh;
    face *currentFace; //current face for drawing explicit outline
    QOpenGLBuffer faceIdx;
    GLuint faceEdgeCount;
    vertex *currentVertex; //current vertex for drawing explicitly
    QOpenGLBuffer vertIdx;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    //HW5
    void buildCubeMesh();
    void sendVertFaceEdge();
    void drawSelVertFace();

protected:
    void keyPressEvent(QKeyEvent *e);

    //HW5
signals:
    void sig_storeVertex(QListWidgetItem *item);
    void sig_storeFace(QListWidgetItem *item);
    void sig_storeEdge(QListWidgetItem *item);

    //HW5
private slots:
    void slot_updateDataOnGPU(); //resend the updated data to GPU changed due to either
                            // 'position' of 'vertex' or 'color' of 'face'
    void slot_getVertex(QListWidgetItem *v);    //store current vertex for draw explicitly
    void slot_getFace(QListWidgetItem *f);      //store current face for draw explicitly

    void slot_addVertex2Edge(halfEdge *he, glm::vec4 pos = glm::vec4(0,0,0,1)); //3.4 //gets the signal to add vertex on inputted halfEdge
    void slot_splitQuadInTriangle(face *f); //3.5 //gets the signal to split inputted face in triangle, if it is quad
    void slot_deleteVertex(vertex *v); //3.6 //gets the signal to delete vertex in mesh, it make hole and also connect
                                             //hole making half edges in reverse order to their symeetrical halfEdges
    void slot_catmullClarkSubdiv();
    void quadrangulateCatClar(face *f, glm::vec4 centroid, int originalNoVert ); //Helper funtion that makes quads of faces from catmulClark
};
