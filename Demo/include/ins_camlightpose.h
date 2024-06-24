#ifndef INS_CAMLIGHTPOSE_H
#define INS_CAMLIGHTPOSE_H

#include <QDialog>
#include"ins_widget.h"


namespace Ui
{
class Ins_CamLightPose;
}

class Ins_CamLightPose : public QDialog
{
    Q_OBJECT

public:
    explicit Ins_CamLightPose(QWidget *parent = nullptr);
    ~Ins_CamLightPose();


    Ins_Widget* c_Image1;
    Ins_Widget* c_Image2;
    Ins_Widget* c_Image3;

    Ins_Widget* l_Image1;
    Ins_Widget* l_Image2;
    Ins_Widget* l_Image3;

    QGraphicsScene* qgraphicsScenes1;
    QGraphicsScene* qgraphicsScenes2;
    QGraphicsScene* qgraphicsScenes3;
    QGraphicsScene* qgraphicsScenes4;
    QGraphicsScene* qgraphicsScenes5;
    QGraphicsScene* qgraphicsScenes6;



private:
    Ui::Ins_CamLightPose *ui;

    QTimer *timer;
    QGraphicsPixmapItem *item1;
    QGraphicsPixmapItem *item2;
    QGraphicsPixmapItem *item3;
    QGraphicsPixmapItem *item4;
    QGraphicsPixmapItem *item5;
    QGraphicsPixmapItem *item6;

    //GyroInterface gyro;//陀螺仪



public slots:
    void setAngleValue(double angle);
    //void setupGraphicsViewWithImage(QGraphicsView *view, const QString &imagePath);

    QGraphicsPixmapItem* setupGraphicsViewWithImage(QGraphicsView *view, const QString &imagePath);
    //double getTLYAngle();//陀螺仪返回值

    void updateRotation();


};

#endif // INS_CAMLIGHTPOSE_H
