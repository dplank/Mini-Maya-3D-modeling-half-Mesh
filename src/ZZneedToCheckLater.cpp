//This is file contain rough ideas or what i have to fix
//1. In mygl.cpp, under slot_updateDataOnGPU() (line no. 591), do really need to call slot_getFace((QLIshtWidgetItem*))
// and slot_getVertex() functions.

//2. After triangulating our 4 edge face, when we delete vertex contain triangle face will stop program.

//3. catmul calrk with any vertex deleted.
void slot_resetCube() {
    cubeMesh.destroy();
    cubeMesh.~mesh();
    cubeMesh.faces.clear();
    cubeMesh.halfEdges.clear();
    cubeMesh.faces.clear();

    std::cout<<"face count: "<<cubeMesh.faces.size()<<std::endl;
    std::cout<<"edge count: "<<cubeMesh.halfEdges.size()<<std::endl;
    std::cout<<"vertex count: "<<cubeMesh.vertices.size()<<std::endl;

    //cubeMesh.destroy();
    buildCubeMesh();
    std::cout<<"face count: "<<cubeMesh.faces.size()<<std::endl;
    std::cout<<"edge count: "<<cubeMesh.halfEdges.size()<<std::endl;
    std::cout<<"vertex count: "<<cubeMesh.vertices.size()<<std::endl;

    cubeMesh.create();
    sendVertFaceEdge();
    slot_updateDataOnGPU();
}
