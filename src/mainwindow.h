#pragma once

#include <QMainWindow>

//HW5
#include <QListWidgetItem>
#include <vertex.h>
#include <face.h>
#include <halfedge.h>

namespace Ui
{
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

    //HW5
private:
    vertex *currentVert;
    face *currentFace;
    halfEdge *currentHE; // for HW5, 3.4

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionQuit_triggered();

    //HW5
    void slot_getFace(QListWidgetItem *item);
    void slot_getVertex(QListWidgetItem *item);
    void slot_getHE(QListWidgetItem *item); // for HW5, 3.4
    void slot_processAddVert(); // for HW5, 3.4
    void slot_processSplitQuad(); //for HW5, 3.5
    void slot_processDeleteVertex(); //for HW5, 3.6

    //HW5 getting values from spin box and setting it to current vertex
    void slot_getVX(double x);
    void slot_getVY(double y);
    void slot_getVZ(double z);

    //HW5 getting values from slider and setting it to current face color
    void slot_getR(int r);
    void slot_getG(int g);
    void slot_getB(int b);

private:
    Ui::MainWindow *ui;

//HW5
signals:
    void sig_vX(double x);  //Emits current vertex position to show
    void sig_vY(double y);  //Emits when vertex selected
    void sig_vZ(double z);

    void sig_r(int r);   //Emits current RGB value of face
    void sig_g(int g);   //Emits when face selected
    void sig_b(int b);

    void sig_updateDataOnGPU();

    void sig_addVertex2Edge(halfEdge *he);  // for HW5, 3.4
    void sig_splitQuadInTriangle(face *f); //for HW5, 3.5
    void sig_deleteVertex(vertex *v); //for HW5, 3.6

};
