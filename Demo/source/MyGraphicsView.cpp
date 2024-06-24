// MyGraphicsView.cpp
#include "MyGraphicsView.h"
#include <QMouseEvent> 
#include <qdebug.h>

void MyGraphicsView::mousePressEvent(QMouseEvent* event) 
{
    //QGraphicsView::mousePressEvent(event);  

    if (event->button() == Qt::RightButton) 
    {
        emit rightClicked(); 
        qDebug() << "rightClick";
    }

    QGraphicsView::mousePressEvent(event);
}

