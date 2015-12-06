#include "face.h"

face::face(int inId)
{
    id=inId;
    startEdge=nullptr;
    color=glm::vec4(0,0,0,1);

    this->setText(QString::number(inId));

}

face::~face()
{

}

