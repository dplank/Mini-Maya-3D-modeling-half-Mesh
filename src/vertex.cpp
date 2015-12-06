
#include "vertex.h"

vertex::vertex(int inId)
{
    id=inId;
    pos=glm::vec4(0,0,0,1);
    edge=nullptr;
    this->setText(QString::number(inId));
}

vertex::vertex(int inId, float x, float y, float z){

    id=inId;
    //initializing position
    pos[0]=x;
    pos[1]=y;
    pos[2]=z;
    edge=nullptr;
}

vertex::~vertex()
{

}

