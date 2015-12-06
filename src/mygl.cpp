#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include <QListWidget>
#include <vector>

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      faceIdx(QOpenGLBuffer::VertexBuffer),
      vertIdx(QOpenGLBuffer::VertexBuffer)
{
    setFocusPolicy(Qt::ClickFocus);

    //HW5
    currentFace = nullptr;
    currentVertex = nullptr;
}

MyGL::~MyGL(){
    makeCurrent();

    vao.destroy();
    geom_cylinder.destroy();
    geom_sphere.destroy();
    cubeMesh.destroy(); //D: HW5
}

//HW5
void MyGL::buildCubeMesh(){

    /* Face and Verticies
      6+-----------+5
      /|  F4      /|
     / |     F2  / |
   3+-----------+2 |
    |F3|        |F1|
    | 7+--------|--+4
    | /  F0     |  /
    |/     F5   | /
   0+----------+/1

    */
    //colors
    glm::vec4 RED = glm::vec4(1,0,0,1);
    glm::vec4 GREEN = glm::vec4(0,1,0,1);
    glm::vec4 BLUE = glm::vec4(0,0,1,1);
    glm::vec4 RnG = glm::vec4(1,1,0,1);
    glm::vec4 RnB = glm::vec4(1,0,1,1);
    glm::vec4 GnB = glm::vec4(0,1,1,1);

    std::cout<<"start of build cube mesh"<<std::endl;   //

    ///###Front Face###///
    //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    face *fTemp = new face(0);
    fTemp->color =  RED;
    cubeMesh.faces.push_back(fTemp);

    //2. Create 4 verticies, initialize its position, id

    //v0
    vertex *vTemp = new vertex(0);
    vTemp->pos = glm::vec4(-0.5, -0.5, 0.5, 1);
    cubeMesh.vertices.push_back(vTemp);
    //v1
    vTemp = new vertex(1);
    vTemp->pos = glm::vec4(0.5, -0.5, 0.5, 1);
    cubeMesh.vertices.push_back(vTemp);

    //v2
    vTemp = new vertex(2);
    vTemp->pos = glm::vec4(0.5, 0.5, 0.5, 1);
    cubeMesh.vertices.push_back(vTemp);

    //v3
    vTemp = new vertex(3);
    vTemp->pos = glm::vec4(-0.5, 0.5, 0.5, 1);
    cubeMesh.vertices.push_back(vTemp);


    //3. Create 4 halfEdges, initialize its face, vert, next, id
    halfEdge *heTemp = new halfEdge(0);
    heTemp->Face = fTemp;
    heTemp->vert = cubeMesh.vertices[0];
    cubeMesh.halfEdges.push_back(heTemp); //Adding it's next after all halfEdges

    std::cout<<"11"<<std::endl;        //

    for(int i=1; i<=3; i++){
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    for(int i=3; i>=1; i--) { // creating edges reversly
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[i];
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i+1)%4];
    }
    cubeMesh.halfEdges[0]->next = cubeMesh.halfEdges[1];

    //4. initialize face's edge
    cubeMesh.faces[0]->startEdge = cubeMesh.halfEdges[0];

    std::cout<<"12"<<std::endl;        //

    //5. initialize verticies edge
    for(int i=0; i<4; i++){
        cubeMesh.vertices[i]->edge = cubeMesh.halfEdges[i];
    }

    std::cout<<"right"<<std::endl;      //

    ///###Right Face###///
        //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    fTemp = new face(1);
    fTemp->color = GREEN;
    cubeMesh.faces.push_back(fTemp);

    //2. Create it's verticies and initialize its position, id
    //v4
    vTemp = new vertex(4);
    vTemp->pos = glm::vec4(0.5, -0.5, -0.5, 1);
    cubeMesh.vertices.push_back(vTemp);

    //v5
    vTemp = new vertex(5);
    vTemp->pos = glm::vec4(0.5, 0.5, -0.5, 1);
    cubeMesh.vertices.push_back(vTemp);


    //3. Create 4 halfEdges, initialize its face, vert, next, id
    //Creating first half Edge of face
    heTemp = new halfEdge(4);
    heTemp->vert = cubeMesh.vertices[1];
    heTemp->Face = fTemp;
    cubeMesh.halfEdges.push_back(heTemp);

    for(int i=5; i<=7; i++){
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    //Loop for remaining 3 edges
    for(int i=7; i>=5; i--) {
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i==7 ? 4 : i+1)];

        switch(i) {
        case 7:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[2];
            break;      //I often forget it!
        case 6:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[5];
            break;      //I often forget it!
        case 5:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[4];
            break;      //I often forget it!
        }

    }

    //initialize first half Edge's next of face
    cubeMesh.halfEdges[4]->next = cubeMesh.halfEdges[5];

    //4. initialize face's edge
    cubeMesh.faces[1]->startEdge = cubeMesh.halfEdges[4];

    //5. initialize verticies edge
    for(int i=4; i<=5; i++){
        cubeMesh.vertices[i]->edge = cubeMesh.halfEdges[i+1];
    }


    std::cout<<"back"<<std::endl;      //

    ///###Back Face###///
        //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    fTemp = new face(2);
    fTemp->color = GnB;
    cubeMesh.faces.push_back(fTemp);

    //2. Create it's verticies, initialize its position, id
    vTemp = new vertex(6);
    vTemp->pos = glm::vec4(-0.5, 0.5, -0.5, 1);
    cubeMesh.vertices.push_back(vTemp);

    vTemp = new vertex(7);
    vTemp->pos = glm::vec4(-0.5, -0.5, -0.5, 1);
    cubeMesh.vertices.push_back(vTemp);


    //3. Create 4 halfEdges, initialize its face, vert, next, id
    //Creating first half Edge of face
    heTemp = new halfEdge(8);
    heTemp->vert = cubeMesh.vertices[4];
    heTemp->Face = fTemp;
    cubeMesh.halfEdges.push_back(heTemp);

    for(int i=9; i<=11; i++){
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    //Loop for remaining 3 edges
    for(int i=11; i>=9; i--) {
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i==11 ? 8 : i+1)];

        switch(i) {
        case 11:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[5];
            break;
        case 10:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[6];
            break;
        case 9:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[7];
            break;
        }

    }

    cubeMesh.halfEdges[8]->next = cubeMesh.halfEdges[9];

    //4. initialize face's edge
    cubeMesh.faces[2]->startEdge = cubeMesh.halfEdges[8];

    //5. initialize verticies edge
    for(int i=6; i<=7; i++){
        cubeMesh.vertices[i]->edge = cubeMesh.halfEdges[(i==6 ? 10 : 9)];
        //std::cout<<"i= "<<i<<" value: "<<(i==6 ? 10 : 9)<<std::endl;
    }


    std::cout<<"left"<<std::endl;      //

    ///###Left Face###///
        //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    fTemp = new face(3);
    fTemp->color = RnB;
    cubeMesh.faces.push_back(fTemp);

    //2. Create it's verticies, initialize its position, id
    /* No need any new verticies*/

    //3. Create 4 halfEdges, initialize its face, vert, next, id
    //Creating first half Edge of face
    heTemp = new halfEdge(12);
    heTemp->vert = cubeMesh.vertices[7];
    heTemp->Face = fTemp;
    cubeMesh.halfEdges.push_back(heTemp);

    for(int i=13; i<=15; i++) {
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    //Loop for remaining 3 edges
    for(int i=15; i>=13; i--) {
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i==15 ? 12 : i+1)];

        switch(i) {
        case 15:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[6];
            break;
        case 14:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[3];
            break;
        case 13:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[0];
            break;
        }
    }

    cubeMesh.halfEdges[12]->next = cubeMesh.halfEdges[13];

    //4. initialize face's edge
    cubeMesh.faces[3]->startEdge = cubeMesh.halfEdges[12];

    //5. initialize verticies edge
        /*Not any new verticies*/


    std::cout<<"top"<<std::endl;      //

    ///###Top Face###///
        //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    fTemp = new face(4);
    fTemp->color = BLUE;
    cubeMesh.faces.push_back(fTemp);

    //2. Create it's verticies, initialize its position, id
    /* No need any new verticies*/

    //3. Create 4 halfEdges, initialize its face, vert, next, id
    //Creating first half Edge of face
    heTemp = new halfEdge(16);
    heTemp->vert = cubeMesh.vertices[3];
    heTemp->Face = fTemp;
    cubeMesh.halfEdges.push_back(heTemp);

    for(int i=17; i<=19; i++){
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    //Loop for remaining 3 edges
    for(int i=19; i>=17; i--) {
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i==19 ? 16 : i+1)];

        switch(i) {
        case 19:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[6];
            break;
        case 18:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[5];
            break;
        case 17:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[2];
            break;
        }

    }

    cubeMesh.halfEdges[16]->next = cubeMesh.halfEdges[17];

    //4. initialize face's edge
    cubeMesh.faces[4]->startEdge = cubeMesh.halfEdges[16];

    //5. initialize verticies edge
        /*Not any new verticies*/

    std::cout<<"bottom"<<std::endl;     //

    ///###Bottom Face###///
        //Follow 5 below steps, but it not initialize symmetry of halfEdges
    //1. Create Face, initialize its color, id
    fTemp = new face(5);
    fTemp->color = RnG;
    cubeMesh.faces.push_back(fTemp);

    //2. Create it's verticies, initialize its position, id
    /* No need any new verticies*/

    //3. Create 4 halfEdges, initialize its face, vert, next, id
    //Creating first half Edge of face
    heTemp = new halfEdge(20);
    heTemp->vert = cubeMesh.vertices[1];
    heTemp->Face = fTemp;
    cubeMesh.halfEdges.push_back(heTemp);

    for(int i=21; i<=23; i++) {
        heTemp = new halfEdge(i);
        cubeMesh.halfEdges.push_back(heTemp);
    }

    //Loop for remaining 3 edges
    for(int i=23; i>=21; i--) {
        cubeMesh.halfEdges[i]->Face = fTemp;
        cubeMesh.halfEdges[i]->next = cubeMesh.halfEdges[(i==23 ? 20 : i+1)];

        switch(i) {
        case 23:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[4];
            break;
        case 22:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[7];
            break;
        case 21:
            cubeMesh.halfEdges[i]->vert = cubeMesh.vertices[0];
        }

    }

    cubeMesh.halfEdges[20]->next = cubeMesh.halfEdges[21];

    //4. initialize face's edge
    cubeMesh.faces[5]->startEdge = cubeMesh.halfEdges[20];

    //5. initialize verticies edge
        /*Not any new verticies*/

    std::cout<<"connecting sym of HEs"<<std::endl;  //

    ///###Connecting every half edge with its sym###///

        ///Front Face
    cubeMesh.halfEdges[0]->sym = cubeMesh.halfEdges[14];    //edge 0 and 14
    cubeMesh.halfEdges[14]->sym = cubeMesh.halfEdges[0];
    cubeMesh.halfEdges[1]->sym = cubeMesh.halfEdges[21];    //edge 1 and 21
    cubeMesh.halfEdges[21]->sym = cubeMesh.halfEdges[1];
    cubeMesh.halfEdges[2]->sym = cubeMesh.halfEdges[4];     //edge 2 and 4
    cubeMesh.halfEdges[4]->sym = cubeMesh.halfEdges[2];
    cubeMesh.halfEdges[3]->sym = cubeMesh.halfEdges[17];    //edge 3 and 17
    cubeMesh.halfEdges[17]->sym = cubeMesh.halfEdges[3];

        ///Right Face
    cubeMesh.halfEdges[5]->sym = cubeMesh.halfEdges[20];   //edge 5 and 20
    cubeMesh.halfEdges[20]->sym = cubeMesh.halfEdges[5];
    cubeMesh.halfEdges[6]->sym = cubeMesh.halfEdges[8];     //edge 6 and 8
    cubeMesh.halfEdges[8]->sym = cubeMesh.halfEdges[6];
    cubeMesh.halfEdges[7]->sym = cubeMesh.halfEdges[18];    //edge 7 and 18
    cubeMesh.halfEdges[18]->sym = cubeMesh.halfEdges[7];

        ///Back Face
    cubeMesh.halfEdges[9]->sym = cubeMesh.halfEdges[23];       //edge 9 and 23
    cubeMesh.halfEdges[23]->sym = cubeMesh.halfEdges[9];
    cubeMesh.halfEdges[10]->sym = cubeMesh.halfEdges[12];       //edge 10 and 12
    cubeMesh.halfEdges[12]->sym = cubeMesh.halfEdges[10];
    cubeMesh.halfEdges[11]->sym = cubeMesh.halfEdges[19];        //edge 11 and 19
    cubeMesh.halfEdges[19]->sym = cubeMesh.halfEdges[11];

        ///Left Face
    cubeMesh.halfEdges[15]->sym = cubeMesh.halfEdges[16];        //edge 15 and 16
    cubeMesh.halfEdges[16]->sym = cubeMesh.halfEdges[15];
    cubeMesh.halfEdges[13]->sym = cubeMesh.halfEdges[22];        //edge 13 and 22
    cubeMesh.halfEdges[22]->sym = cubeMesh.halfEdges[13];

    std::cout<<"end of build cube mesh"<<std::endl;   //
}

//HW5
void MyGL::sendVertFaceEdge(){
    //Send all vertices
    for(int i=0; i<cubeMesh.vertices.size(); i++){
        cubeMesh.vertices[i]->setText(QString::number(i));
        emit sig_storeVertex(cubeMesh.vertices[i]);
    }

    //Send all Faces
    for(int i=0; i<cubeMesh.faces.size(); i++){
        cubeMesh.faces[i]->setText(QString::number(i));
        emit sig_storeFace(cubeMesh.faces[i]);
    }

    //Send all Edges
    for(int i=0; i<cubeMesh.halfEdges.size(); i++){
        cubeMesh.halfEdges[i]->setText(QString::number(i));
        emit sig_storeEdge(cubeMesh.halfEdges[i]);
    }
}


void MyGL::initializeGL()
{
    // Create an OpenGL context
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    // Create the example sphere (you should delete this when you add your own code elsewhere)
    geom_cylinder.create();

    geom_sphere.create();


    //HW:5
    buildCubeMesh();
    cubeMesh.create();
    sendVertFaceEdge();

    //glLineWidth(8.0f);

    //GLint range[2];
    //glGetIntegerv(GL_ALIASED_LINE_WIDTH_RANGE,range);
    //std::cout<<"min: "<<range[0]<<", max: "<<range[1]<<std::endl;


    std::cout<<"cubeMesh created in initGL"<<std::endl;

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the wireframe shader
    prog_wire.create(":/glsl/wire.vert.glsl", ":/glsl/wire.frag.glsl");

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    vao.bind();
}


void MyGL::resizeGL(int w, int h)
{
    camera = Camera(w, h);

    glm::mat4 viewproj = camera.getViewProj();

    // Upload the projection matrix
    prog_lambert.setViewProjMatrix(viewproj);
    prog_wire.setViewProjMatrix(viewproj);

    printGLErrorLog();
}


//HW5 draw selected face and vertex
void MyGL::drawSelVertFace(){

    ///I. DRAW VERTEX
    vertIdx.bind();
    glDrawArrays(GL_POINT,0,1);

    ///II. DRAW FACE

}


// This function is called by Qt any time your GL window is supposed to update
// For example, when the function updateGL is called, paintGL is called implicitly.

void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(camera.getViewProj());
    prog_wire.setViewProjMatrix(camera.getViewProj());

    //HW5
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    prog_lambert.setModelMatrix(glm::mat4(1.0f));
    prog_lambert.draw(*this, cubeMesh);

    if(currentVertex!=nullptr){
        glPointSize(6);
        prog_wire.setModelMatrix(glm::mat4(1.0f));

        prog_wire.prog.enableAttributeArray(prog_wire.attrPos);
        vertIdx.bind();
        glVertexAttribPointer(prog_wire.attrPos, 4, GL_FLOAT, false, 0, NULL);

        prog_wire.prog.bind();
        vertIdx.bind();
        glDrawArrays(GL_POINTS,0,1);
        //std::cout<<"vertex on mygl"<<std::endl; //
        prog_wire.prog.disableAttributeArray(prog_wire.attrPos);
    }
    if(currentFace!=nullptr){

        prog_wire.prog.enableAttributeArray(prog_wire.attrPos);
        faceIdx.bind();
        glVertexAttribPointer(prog_wire.attrPos, 4, GL_FLOAT, false, 0, NULL);
        glDrawArrays(GL_LINE_LOOP,0,faceEdgeCount);
    }


    /*
    // Cylinder
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2, 2, 0)) * glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(0, 0, 1));
    prog_lambert.setModelMatrix(model);
    prog_lambert.draw(*this, geom_cylinder);
    */
}


void MyGL::keyPressEvent(QKeyEvent *e){
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        camera.theta += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Left) {
        camera.theta -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Up) {
        camera.phi -= 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_Down) {
        camera.phi += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_I) {
        camera.zoom -= 3.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_O) {
        camera.zoom += 3.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_1) {
        camera.fovy += 5.0f * DEG2RAD;
    } else if (e->key() == Qt::Key_2) {
        camera.fovy -= 5.0f * DEG2RAD;
    }
    camera.RecomputeEye();
    update();  // Calls paintGL, among other things
}

//HW5
void MyGL::slot_updateDataOnGPU(){
    cubeMesh.update();
    //std::cout<<"updated"<<std::endl;
    if(currentFace!=nullptr)
        slot_getFace((QListWidgetItem*)currentFace);
    if(currentVertex!=nullptr)
        slot_getVertex((QListWidgetItem*)currentVertex);
}

void MyGL::slot_getFace(QListWidgetItem *f){

    if(faceIdx.isCreated()){
        faceIdx.destroy();
    }

    currentFace = (face*)f;
    //std::cout<<"face on mygl"<<std::endl;
    std::vector<glm::vec4> lineLoop;
    halfEdge *firstHE,*currentHE;
    firstHE = currentHE = currentFace->startEdge;

    do {
        lineLoop.push_back(currentHE->vert->pos);
        currentHE = currentHE->next;
    }while(currentHE != firstHE);

    faceEdgeCount = lineLoop.size();
    faceIdx.create();
    faceIdx.bind();
    faceIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    faceIdx.allocate(lineLoop.data(),lineLoop.size()*sizeof(glm::vec4));

}

void MyGL::slot_getVertex(QListWidgetItem *v){
    if(vertIdx.isCreated()){
        vertIdx.destroy();
    }

    currentVertex = (vertex*)v;
    //std::cout<<"vertex on mygl"<<std::endl;
    vertIdx.create();
    vertIdx.bind();
    vertIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertIdx.allocate(&(currentVertex->pos), sizeof(glm::vec4));
}

void MyGL::slot_addVertex2Edge(halfEdge *he, glm::vec4 pos /*for Catmull-Clark*/){

    //std::cout<<"called addVert2Edge func"<<std::endl;
    halfEdge *HE1 = he, *HE2 = he->sym;
    vertex *V1 = HE1->vert, *V2 = HE2->vert;

    //Create new vertex V3, and assign it's 'position' to average of V1, V2
    vertex *V3 = new vertex(cubeMesh.vertices.size());
    if (pos.x==0 && pos.y==0 && pos.z==0) {
        V3->pos = (V1->pos + V2->pos)/2.0f;
    }else {
        V3->pos = pos;
    }

    //Create 2 new half-edges HE1B and HE2B needed surround V3 and assign their 'vert' and 'Face'
    halfEdge *HE1B = new halfEdge(cubeMesh.halfEdges.size());
    halfEdge *HE2B = new halfEdge(cubeMesh.halfEdges.size()+1);

    HE1B->vert = V1; V1->edge = HE1B;
    HE2B->vert = V2; V2->edge = HE2B;

    HE1B->Face = HE1->Face;
    HE2B->Face = HE2->Face;

    //Adjust 'sym', 'next' and 'vert' of HE1, HE2, HEB1, HEB2
    HE1->sym = HE2B; HE2B->sym = HE1;
    HE2->sym = HE1B; HE1B->sym = HE2;

    HE1B->next = HE1->next; HE1->next = HE1B;
    HE2B->next = HE2->next; HE2->next = HE2B;

    HE1->vert = HE2->vert = V3;

    //And initialize V3's halfEdge
    V3->edge = HE1;

    //insert them into vector list of mesh
    cubeMesh.halfEdges.push_back(HE1B);
    cubeMesh.halfEdges.push_back(HE2B);

    cubeMesh.vertices.push_back(V3);

    //emit new halfEdge and Vertex to submit into GUI
    emit sig_storeEdge(HE1B);
    emit sig_storeEdge(HE2B);
    emit sig_storeVertex(V3);
}

void MyGL::slot_splitQuadInTriangle(face *f) {
    //CHECK is current face is quad??
    if(f->startEdge->next->next->next->next != f->startEdge)
        return;

    //initialize FACE1, HE
    face *FACE1 = f;
    halfEdge *HE = f->startEdge;

    //step 1: create 2 new HE - HEA and HEB
    halfEdge *HEA = new halfEdge(cubeMesh.halfEdges.size());
    halfEdge *HEB = new halfEdge(cubeMesh.halfEdges.size()+1);
        //initialize their vert and sym
    HEA->vert = HE->vert;
    HEB->vert = HE->next->next->vert;
    HEA->sym = HEB; HEB->sym = HEA;

    //step 2: create new face FACE2 make it's halfEdge to HEA
    face *FACE2 = new face(cubeMesh.faces.size());
    FACE2->color = (FACE1->color)*0.75f;
    FACE2->color[3] = 1.0;

    FACE2->startEdge = HEA;
    //initialize all it's relavent HEs to pointing it including HEA
    HE->next->Face = FACE2;
    HE->next->next->Face = FACE2;
    HEA->Face = FACE2;
    //make HEB pointing FACE1
    HEB->Face = FACE1;

    //step 3: Fix 'next' pointers of all halfEdges
        //HEA to HE-next, HEB to HE-next-next-next
        HEA->next = HE->next;
        HEB->next = HE->next->next->next;
        //HE-next-next(-next) to HEA, HE-next to HEB
        HE->next->next->next = HEA;
        HE->next = HEB;

    //Add new face and 2 new halfedges into list of mesh
    cubeMesh.halfEdges.push_back(HEA);
    cubeMesh.halfEdges.push_back(HEB);

    cubeMesh.faces.push_back(FACE2);

    //Emit 2 halfEdges and 1 face to add to GUI
    emit sig_storeEdge(HEA);
    emit sig_storeEdge(HEB);

    emit sig_storeFace(FACE2);

    //AT END update data on GPU, so it get current no of face and halfedges to draw immidiately
    slot_updateDataOnGPU();
}

///*Warnging*/ deleting more then vertex lead some problem, as development is done considering full connected mesh
void MyGL::slot_deleteVertex(vertex *v) {

    ///Intializing
    //pick a HE pointing to 'v' name it HE0
    halfEdge *HE0 = v->edge;

    //create 2 vectors of HE, 1 for storing start of undeleted HE (begUndelHE) and
    //other for end of undeleted HE (endUndelHE)
    std::vector<halfEdge*> begUndelHE;
    std::vector<halfEdge*> endUndelHE;

    ///Filling begUndelHE and endUndelHE
    //crete variable toDelFaces=0 to count no of faces need to be delete
    int toDelFaces=0;
    //do while loop by initiating current pointer to HE0, untill current again to HE0
    halfEdge *currentHE = HE0;
    do {
        //toDelFaces++
        toDelFaces++;
        std::cout<<"currentFace: "<<toDelFaces<<std::endl;  //
        //push current->next->next to begUndelHE
        begUndelHE.push_back(currentHE->next->next);

        //loop in current face and push back HE, which pointing to currentHE, to endUndelHE
        halfEdge *tempCurrentHE = currentHE;
        do {
            std::cout<<"process before push endUndelHE"<<std::endl; //
            tempCurrentHE=tempCurrentHE->next;
        } while (tempCurrentHE->next != currentHE);
        endUndelHE.push_back(tempCurrentHE);

        //update current to current->next-sym
        currentHE = currentHE->next->sym;
    } while (currentHE!=HE0);//while condition is current!=HE0


    ///Deleting each face, halfEdge and core vertex surrounding given vertex and also from list of CUBE MESH
    //make current to HE0 and loop through toDelFaces
    currentHE = HE0;
    for(int i=0; i<toDelFaces; i++) {
        //delete curret face from CUBE MESH and memory
        /* This is method for deleting in vetor a given value */
        std::vector<face*>::iterator newEndFace = std::remove(cubeMesh.faces.begin(), cubeMesh.faces.end(), currentHE->Face);
        //std::remove is algorithm and it actually not erase element
        cubeMesh.faces.erase(newEndFace, cubeMesh.faces.end());
        delete currentHE->Face;

        //assign temp to current->next
        halfEdge *tempHE = currentHE->next;

        //delete current and remove from MESH and assign it to temp and temp to temp->sym
        std::vector<halfEdge*>::iterator newEndHE = std::remove(cubeMesh.halfEdges.begin(), cubeMesh.halfEdges.end(), currentHE);
        cubeMesh.halfEdges.erase(newEndHE, cubeMesh.halfEdges.end());
        delete currentHE;
        currentHE = tempHE;
        tempHE = tempHE->sym;

        //delete current from CUBE MESH and memory and assign it to temp
        newEndHE = std::remove(cubeMesh.halfEdges.begin(), cubeMesh.halfEdges.end(), currentHE);
        cubeMesh.halfEdges.erase(newEndHE, cubeMesh.halfEdges.end());
        delete currentHE;
        currentHE=tempHE;
    }
    //Finally deleting vertex from CUBE MESH and memory
    std::vector<vertex*>::iterator newEndVertex  = std::remove(cubeMesh.vertices.begin(), cubeMesh.vertices.end(), v);
    cubeMesh.vertices.erase(newEndVertex, cubeMesh.vertices.end());
    delete v;


    ///Updating open corner halfEdge's face and next pointers
    //loop through toDelFaces by i start by 0
    for(int i=0; i<toDelFaces; i++) {
        std::cout<<"i = "<<i<<std::endl;    //
        //endUndelHE[i] = begUndelHE[ (i==0 ? (toDelFaces-1) : i-1 )]
        endUndelHE[i]->next = begUndelHE[ (i==0 ? (toDelFaces-1) : (i-1) )];

        //set currentPointer to begUndelHE[i] and loop till it equal endUndderHE[i]
        begUndelHE[i]->Face = nullptr;
        currentHE = begUndelHE[i];
        while (currentHE != endUndelHE[i]){
            std::cout<<"process of setting face to null"<<std::endl;    //
            currentHE = currentHE->next;
            //currentPointer->face=nullptr
            currentHE->Face = nullptr;
        }
    }

    //CALL UPDATE DATA ON GPU
    slot_updateDataOnGPU();
    sendVertFaceEdge();
}

void MyGL::quadrangulateCatClar(face *f, glm::vec4 centroid, int originalNoVert ) {

    int originalNoHE = cubeMesh.halfEdges.size();   //For filling 'sym' pointers of new half Edges at step 2.
    glm::vec4 faceColor = f->color;

    //create new vertex with centeroid and push it in mesh
    vertex *centroidVert = new vertex(cubeMesh.vertices.size());
    centroidVert->pos = centroid;
    cubeMesh.vertices.push_back(centroidVert);

    //1. Creating all halfEdges, faces and initializing their variables except halfEdges->sym
    //initialize variable countFaces to count no of faces
    int countFaces=0;
    //choose a halfEdge from face which is pointing to one of halfEdges which is pointing original vertex
    halfEdge *firstHE = f->startEdge;
    std::vector<vertex*>::iterator it = std::find(cubeMesh.vertices.begin(), cubeMesh.vertices.end(), firstHE->vert);
    int index = std::distance(cubeMesh.vertices.begin(), it);
    if( index >= originalNoVert ) //if it's not pointing original vertex
        firstHE = firstHE->next;

    //initialize 3 pointers one is current and first pointing firstHE pointer, another is following pointing to current
    halfEdge *currentHE = firstHE, *followingHE;
    halfEdge *tempHE = currentHE->next;
    while(tempHE->next != currentHE) {
        tempHE = tempHE->next;
    }
    followingHE = tempHE;

    //make current face f->startEdge to currentHE and create 2 new HE also push each HE in mesh
    f->startEdge = currentHE;
    halfEdge *HE1 = new halfEdge(cubeMesh.halfEdges.size());
    halfEdge *HE2 = new halfEdge(cubeMesh.halfEdges.size());
    cubeMesh.halfEdges.push_back(HE1);
    cubeMesh.halfEdges.push_back(HE2);

    //store in tempHE current->next->next and update all HE's next of current face
    tempHE = currentHE->next->next; //For keeping first pointer of next face
    HE1->next = HE2;
    HE2->next = currentHE;
    currentHE->next->next = HE1;

    //now initialize their vert as well as faces
    HE1->vert = centroidVert;
    HE2->vert = followingHE->vert;

    HE1->Face = f;
    HE2->Face = f;

    //set centroidVert->edge to HE1(which is pointing this centeroid)
    centroidVert->edge = HE1;

    //update following to current->next and current to temp
    followingHE = currentHE->next;
    currentHE = tempHE;

    //update countFaces
    countFaces++;

    // while loop with condition current!=first
    while ( currentHE != firstHE) {
        //update countFaces
        countFaces++;
        //create new face with it's startEdge equal current and 2 new HE and push them in mesh
        face *tempF = new face(cubeMesh.faces.size());
        tempF->startEdge = currentHE;
        tempF->color = faceColor;
        HE1 = new halfEdge(cubeMesh.halfEdges.size());
        HE2 = new halfEdge(cubeMesh.halfEdges.size());

        cubeMesh.faces.push_back(tempF);
        cubeMesh.halfEdges.push_back(HE1);
        cubeMesh.halfEdges.push_back(HE2);

        //update tempHE then update all HE's next
        tempHE = currentHE->next->next;
        HE1->next = HE2;
        HE2->next = currentHE;
        currentHE->next->next = HE1;

        //initialize their vert as well as face
        HE1->vert = centroidVert;
        HE2->vert = followingHE->vert;

        currentHE->Face = tempF;
        currentHE->next->Face = tempF;
        HE1->Face = tempF;
        HE2->Face = tempF;

        //update current and following pointers
        followingHE = currentHE->next;
        currentHE = tempHE;

    }

    //2. initializing halfEdge->sym
    //loop from 0 to countFace(including) with 2 steps increment
    for(int i=0; i<= countFaces; i+=2) {
        //update sym of cubeMesh->HE[originalNoHE + i] and cubeMesh->HE[originalNoHE + i +3]
        cubeMesh.halfEdges[originalNoHE + i]->sym = cubeMesh.halfEdges[originalNoHE + i +3];
        cubeMesh.halfEdges[originalNoHE + i +3]->sym = cubeMesh.halfEdges[originalNoHE + i];
    }

    //finally update [originalNoHE + 1] and [originalNoHE + 2*countFace - 2]
    cubeMesh.halfEdges[originalNoHE + 1]->sym = cubeMesh.halfEdges[originalNoHE + 2*countFaces - 2];
    cubeMesh.halfEdges[originalNoHE + 2*countFaces - 2]->sym = cubeMesh.halfEdges[originalNoHE + 1];

}


//EXTRA:- We are also considering mesh with open faces, like where some halfEdges can have Null faces
void MyGL::slot_catmullClarkSubdiv(){
    //std::cout<<"Start CatCLARK "<<std::endl;        //
    //get total no of verticies in initial mesh for step III
    int originalVertNos = cubeMesh.vertices.size();
    int originalFaceNo = cubeMesh.faces.size();     // For step IV to split each face into N no of faces, N is no of
                                                    // vertices

    /// I. FIND CENTEROID FOR EACH FACE
    // create separate list of each centroid mapped to index of each face
    std::vector<glm::vec4> centroid;
    std::cout<<"no of faces: "<<cubeMesh.faces.size()<<std::endl;
    for(int i=0; i<cubeMesh.faces.size(); i++) {
        //find all vertex positions surrounding current face add all of them
        glm::vec4 cPos;
        halfEdge *startHE = (cubeMesh.faces[i])->startEdge;
        cPos = startHE->vert->pos;
        halfEdge *tempHE = startHE->next;
        int edgeCount=1;
        while(tempHE!=startHE) {
            edgeCount++;
            cPos += tempHE->vert->pos;
            tempHE = tempHE->next;
        }
        //divide final vertex to no of vertices
        cPos = cPos/(float)edgeCount;
        centroid.push_back(cPos);

    }

    /// II. COMPUTE SMOOTHED MIDPOINT OF EACH EDGE
    //create new vector list of bool type size equal current edges, to check symetric halfEdge don't process twice
    std::vector<bool> checkHE(( cubeMesh.halfEdges.size() )); //create checkHE of size of mesh's no of halfedges
    for(int i=0; i<checkHE.size(); i++){
        checkHE[i] = false;
    }

    //loop in size of checkHE for each mesh's HE, because mesh's HE size is changing
    for(int i=0; i<checkHE.size(); i++) {
        //check is current HE is true in checkHE if it is then "continue"
        if (checkHE[i]==true){
            continue;
        }
        //else make current HE and it's sym' index true in checkHE
        checkHE[i] = true;
        halfEdge *currentHE = cubeMesh.halfEdges[i];
        /*calculating index of sym of current HE */
        std::vector<halfEdge*>::iterator itHE = std::find(cubeMesh.halfEdges.begin(), cubeMesh.halfEdges.end(), currentHE->sym);
        int pos = std::distance(cubeMesh.halfEdges.begin(), itHE);
        checkHE[pos] = true;


        //compute edgePoint e = (v1+v2+f1+f2)/4 or e = (v1+v2+f1)/3
        bool isF2Present;
        bool isF1Present;
        glm::vec4 edgePoint = currentHE->vert->pos; //v1
        edgePoint += currentHE->sym->vert->pos; //v2
        /* calculating index of f1 and f2*/
        //for f1
        if(currentHE->Face == nullptr) {
            isF1Present = false;
        } else {
            isF1Present = true;
            std::vector<face*>::iterator itF = std::find(cubeMesh.faces.begin(), cubeMesh.faces.end(), currentHE->Face);
            pos = std::distance(cubeMesh.faces.begin(), itF);
            edgePoint += centroid[pos];
        }

        //for f2
        if(currentHE->sym->Face == nullptr) {
            isF2Present = false;
        } else {
            isF2Present = true;
            std::vector<face*>::iterator itF = std::find(cubeMesh.faces.begin(), cubeMesh.faces.end(), currentHE->sym->Face);
            int pos = std::distance(cubeMesh.faces.begin(), itF);
            edgePoint += centroid[pos];
        }

        if (isF1Present && isF2Present){
            edgePoint /= 4.0f;
        } else {
            edgePoint /= 3.0f;
        }

        //insert computed vertex in current halfEdge
        slot_addVertex2Edge(currentHE, edgePoint);
    }


    /// III. SMOOTH THE ORIGINAL VERTICIES Compute ( V' = (n-2)V/n + sum(e)/(n*n) + sum(f)/(n*n) )
    //for each original verticies in mesh
    for(int i=0; i<originalVertNos; i++) {
        //initialize variable to store n(no of adjacent mid points), sumE and sumF
        int n=0;
        glm::vec3 sumE(0,0,0);
        glm::vec3 sumF(0,0,0);

        //loop through all adjecent edge of current vertices
        halfEdge *initialHE = cubeMesh.vertices[i]->edge;
        halfEdge *tempHE = initialHE->next->sym;
        //Strategy:- each symHE use to fill sumF and each nextHE use to fill sumE
        if(initialHE->Face != nullptr) {
            /* getting index of what currentHE points*/
            std::vector<face*>::iterator it = std::find(cubeMesh.faces.begin(), cubeMesh.faces.end(), initialHE->Face);
            int pos = std::distance(cubeMesh.faces.begin(), it);
            sumF += glm::vec3(centroid[pos][0], centroid[pos][1], centroid[pos][2] );
        }
        sumE += glm::vec3(initialHE->next->vert->pos[0], initialHE->next->vert->pos[1], initialHE->next->vert->pos[2]);
        n++;

        while(tempHE != initialHE) {
            n++;
            if(tempHE->Face != nullptr) {
            //find current face index and update sumF
                std::vector<face*>::iterator it = std::find(cubeMesh.faces.begin(), cubeMesh.faces.end(), tempHE->Face);
                int pos = std::distance(cubeMesh.faces.begin(), it);
                sumF += glm::vec3(centroid[pos][0], centroid[pos][1], centroid[pos][2]) ;
            }
            //update sumE eqaul tempHE->next->vert
            sumE += glm::vec3(tempHE->next->vert->pos[0], tempHE->next->vert->pos[1], tempHE->next->vert->pos[2]) ;

            //update tempHE equal tempHE->next->sym
            tempHE = tempHE->next->sym;
        }

        //update current position with above formulae ( V' = (n-2)V/n + sum(e)/(n*n) + sum(f)/(n*n) )
        glm::vec3 tempHolder ( cubeMesh.vertices[i]->pos[0], cubeMesh.vertices[i]->pos[1], cubeMesh.vertices[i]->pos[2] );
        tempHolder = (n-2.0f) * tempHolder;
        tempHolder /=  (float)n;
        cubeMesh.vertices[i]->pos = (glm::vec4 (tempHolder,1) + glm::vec4(sumE/((float)n*n) ,0) + glm::vec4(sumF/((float)n*n), 0) );
        cubeMesh.vertices[i]->pos[3] = 1;
    }


    /// IV. SPLIT EACH ORIGINAL FACE INTO N(no of verticies in face) FACES
    //For each face call quadranglulateCatClar function
    //int originalHENo = cubeMesh.halfEdges.size(); // use to pass in quatrangulateCatCalr function in step IV
    for(int i=0; i<originalFaceNo; i++) {
        quadrangulateCatClar(cubeMesh.faces[i], centroid[i], originalVertNos);
    }


    /// V. UPDATE DATA ON GPU
    slot_updateDataOnGPU();
}

