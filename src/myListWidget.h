#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include<QListWidget>

class myListWidget  : public QListWidget
{
    Q_OBJECT

public:
    myListWidget(QWidget *parent);
    ~myListWidget();

private slots:
     void slot_addItem(QListWidgetItem *item);
     void slot_sendSelectedItem(QListWidgetItem *item);

signals:
     void sig_storeFace(QListWidgetItem *item);
     void sig_storeVertex(QListWidgetItem *item);

};

#endif // MYLISTWIDGET_H
