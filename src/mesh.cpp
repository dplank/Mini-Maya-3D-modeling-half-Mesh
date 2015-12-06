#include "mesh.h"
#include <face.h>
#include <halfedge.h>
#include <vertex.h>
#include <iostream>

//Initialize the Index, position, normal and color buffer to give them
//type of buffer object
mesh::mesh()
    : bufIdx(QOpenGLBuffer::IndexBuffer),
      bufPos(QOpenGLBuffer::VertexBuffer),
      bufNor(QOpenGLBuffer::VertexBuffer),
      bufCol(QOpenGLBuffer::VertexBuffer) {
}


//Initialize vector or array, by filling them of position of
//verticies, normal of vericies, buffer and color also create
//openGL buffer and initialize openGL buffer

void initPosNorColIdx(std::vector<glm::vec4> (&mPos), std::vector<glm::vec4> (&mNor), std::vector<glm::vec4> (&mCol), std::vector<GLuint> (&mIdx),
                      std::vector<face*> faces) {

    //for each face in faces
    for(int i=0 ; i<faces.size(); i++) {

    //std::cout<<"face id: "<<faces[i]->id<<std::endl;         //
    //std::cout<<"start edge id: "<<faces[i]->startEdge->id<<std::endl;       //

    ///Initialization of
        /// Position
        //initialize 'firstPtr' and 'currentPtr' as halfEdge points by face
        halfEdge *firstPtr, *currentPtr;
        firstPtr = currentPtr =  faces[i]->startEdge;
        //store 'first' variable as position.length
        GLuint first = mPos.size();
        //initialize 'current' as first+2
        GLuint current = first+2;
        //push vertex of currentPtr = currentPtr, currentPtr->next
        mPos.push_back(currentPtr->vert->pos);
        currentPtr=currentPtr->next;
        mPos.push_back(currentPtr->vert->pos);
        //do currentPtr=currentPtr->next (i.e. currentPtr->next->next)
        currentPtr=currentPtr->next;

        ///Color
        //push 2 times current face's color
        glm::vec4 color = faces[i]->color;
        mCol.push_back(color);
        mCol.push_back(color);

        ///Normal (CAUTION: We not checking for vec1==vec2 case)
        //dir1 = firstPtr->next - firstPtr, dir2=firstPtr->next->next - firstPtr->next
        glm::vec4 dir1 = firstPtr->next->vert->pos  - firstPtr->vert->pos;
        glm::vec4 dir2 = firstPtr->next->next->vert->pos - firstPtr->next->vert->pos;
        //fNormal for this face = cross(dir1,dir2)
        glm::vec4 fNormal = glm::vec4(glm::cross(glm::vec3(dir1[0], dir1[1], dir1[2]),glm::vec3(dir2[0], dir2[1], dir2[2])),0);
        //push this fNormal 2 times into normal
        mNor.push_back(fNormal);
        mNor.push_back(fNormal);

        //std::cout<<"mNormal size: "<<mNor.size()<<std::endl;        //

        //while loop with condition firstPtr!=currentPtr
        while(currentPtr != firstPtr){
            ///with each last vertex of triangle
            //push currentPtr's vertex into 'position'

            //std::cout<<"current edge id: "<<currentPtr->id<<std::endl;       //
            //std::cout<<"it's' vertex id: "<<currentPtr->vert->id<<std::endl;       //

            mPos.push_back(currentPtr->vert->pos);

            //push on index first, current-1, current
            mIdx.push_back(first);
            mIdx.push_back(current-1);
            mIdx.push_back(current);

            //do currentPtr=currentPtr->next
            currentPtr=currentPtr->next;
            //do current++
            current++;
            ///Color  and Normal
            //push face's color
            mCol.push_back(color);
            //push fNormal
            mNor.push_back(fNormal);
        }
    }

}

std::ostream &operator<< (std::ostream& out, glm::vec4 &v){
    out<<"["<<v.x<<", "<<v.y<<", "<<v.z<<" ]";
    return out;
}

void mesh::create() {

    //Initialize vector array for position, normal,color and index
    std::vector<glm::vec4> position;
    std::vector<glm::vec4> normal;
    std::vector<glm::vec4> color;
    std::vector<GLuint> index;

    //call 'initPosNorColIdx' function for initialize above arrays
    initPosNorColIdx(position, normal, color, index, faces);


    std::cout<<"size of normal = "<<normal.size()<<std::endl;       //
    //std::cout<<"size of position = "<<position.size()<<std::endl;       //
    //std::cout<<"size of color = "<<color.size()<<std::endl;     //
    //std::cout<<"size of intex = "<<index.size()<<std::endl;     //

    //assign all data from CPU to GPU by
     //create, bind, setUsagePattern and allocate to buffers
    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(index.data(),index.size()*sizeof(GLuint) );

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(position.data(), position.size() * sizeof(glm::vec4));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(normal.data(), normal.size() * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(color.data(), color.size()*sizeof(glm::vec4));

    //Initialize count Varialbe as index.size
    count = index.size();

}

void mesh::destroy() {
    bufIdx.destroy();
    bufPos.destroy();
    bufNor.destroy();
    bufCol.destroy();
}

GLenum mesh::drawMode() {
    return GL_TRIANGLES;
}

int mesh::elemCount(){
    return count;
}

bool mesh::bindIdx() {
    return bufIdx.bind();
}

bool mesh::bindPos() {
    return bufPos.bind();
}

bool mesh::bindNor() {
    return bufNor.bind();
}

bool mesh::bindCol() {
    return bufCol.bind();
}

//Destructor delete all halfEdges, vertices and faces of mesh
//which was allocated to create mesh
mesh::~mesh()
{
    for(int i=0; i<halfEdges.size(); i++) {
        delete halfEdges[i];
    }
    for(int i=0; i<vertices.size(); i++){
        delete vertices[i];
    }
    for(int i=0; i<faces.size(); i++){
        delete faces[i];
    }
}

//HW5: special
void mesh::update(){
    destroy();
    create();
}
