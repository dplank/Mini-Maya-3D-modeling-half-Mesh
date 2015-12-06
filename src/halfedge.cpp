#include "halfedge.h"

halfEdge::halfEdge(int inid)
{
    id=inid;
    Face=nullptr;
    vert=nullptr;
    next=nullptr;
    sym=nullptr;
    this->setText(QString::number(inid));
}

halfEdge::~halfEdge()
{

}

