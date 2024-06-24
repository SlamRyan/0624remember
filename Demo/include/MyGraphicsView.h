// MyGraphicsView.h
#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>

class MyGraphicsView : public QGraphicsView 
{
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView;  // �̳й��캯��

    void mousePressEvent(QMouseEvent* event);

signals:
    void rightClicked();
};

#endif // MYGRAPHICSVIEW_H


