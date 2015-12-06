#include "myListWidget.h"
#include <iostream>

myListWidget::myListWidget(QWidget *parent) : QListWidget(parent)
{


}

myListWidget::~myListWidget()
{

}

void myListWidget::slot_addItem(QListWidgetItem *item){
    QListWidget::addItem( item );
}

void myListWidget::slot_sendSelectedItem(QListWidgetItem *item) {
    //std::cout<<"itemClicked"<<std::endl;
    //std::cout<<"item text: "<<item->text().toUtf8().constData()<<std::endl;
    emit sig_storeFace(item);
    emit sig_storeVertex(item);
}
