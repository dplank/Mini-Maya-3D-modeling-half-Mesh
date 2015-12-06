#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();

    //HW5
    currentVert = nullptr;
    currentFace = nullptr;
    currentHE = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

//HW5 below all

void MainWindow::slot_getFace(QListWidgetItem *item){
    currentFace = (face*)item;
    //update selected face's colors
    emit sig_r((int)(currentFace->color[0]*100));
    emit sig_g((int)(currentFace->color[1]*100));
    emit sig_b((int)(currentFace->color[2]*100));
}

void MainWindow::slot_getVertex(QListWidgetItem *item){
    currentVert = (vertex*)item;
    //update selected vertex's position
    emit sig_vX(currentVert->pos[0]);
    emit sig_vY(currentVert->pos[1]);
    emit sig_vZ(currentVert->pos[2]);
}


void MainWindow::slot_getHE(QListWidgetItem *item){
    currentHE = (halfEdge*)item;
}

void MainWindow::slot_processAddVert(){
    //std::cout<<"CALLLED"<<std::endl;    //
    if(currentHE!=nullptr)
       emit sig_addVertex2Edge(currentHE);
}

void MainWindow::slot_processSplitQuad(){
    if (currentFace != nullptr)
        emit sig_splitQuadInTriangle(currentFace);
}

void MainWindow::slot_processDeleteVertex(){
    if(currentVert != nullptr) {
        emit sig_deleteVertex(currentVert);
        currentVert=nullptr;
    }
}

void MainWindow::slot_getVX(double x){
    if(currentVert != nullptr){
        currentVert->pos[0] = x;
        emit sig_updateDataOnGPU();
    }
}

void MainWindow::slot_getVY(double y){
    if(currentVert != nullptr){
        currentVert->pos[1] = y;
        emit sig_updateDataOnGPU();
    }
}

void MainWindow::slot_getVZ(double z){
    if(currentVert != nullptr){
        currentVert->pos[2] = z;
        emit sig_updateDataOnGPU();
    }
}


void MainWindow::slot_getR(int r){
    if(currentFace != nullptr){
        currentFace->color[0] = (float)r/100.0f;
        emit sig_updateDataOnGPU();
    }
}

void MainWindow::slot_getG(int g){
    if(currentFace != nullptr){
        currentFace->color[1] = (float)g/100.0f;
        emit sig_updateDataOnGPU();
    }
}

void MainWindow::slot_getB(int b){
    if(currentFace != nullptr){
        currentFace->color[2] = (float)b/100.0f;
        emit sig_updateDataOnGPU();
    }
}
