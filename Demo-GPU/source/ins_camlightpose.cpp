#include "ins_camlightpose.h"
#include "ui_ins_camlightpose.h"
#include"insglobalvar.h"
#include <iostream>
using namespace std;





Ins_CamLightPose::Ins_CamLightPose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ins_CamLightPose)
{
    ui->setupUi(this);



     cout<<InsGlobalVar::insMain->resultPosex<<endl;

     QIcon iconnn(":/image/icon/ins.ico");
     setWindowIcon(iconnn);


    item1 =setupGraphicsViewWithImage(ui->CamPose_view, ":/image/icon/cam1.png");
    item2 =setupGraphicsViewWithImage(ui->CamPose_view2, ":/image/icon/camcam.png");
    item3 =setupGraphicsViewWithImage(ui->CamPose_view3, ":/image/icon/cam8.png");
    item4 =setupGraphicsViewWithImage(ui->LightPose_view, ":/image/icon/light1.png");
    item5 =setupGraphicsViewWithImage(ui->LightPose_view2, ":/image/icon/light4.png");
    item6 =setupGraphicsViewWithImage(ui->LightPose_view3, ":/image/icon/light3.png");

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Ins_CamLightPose::updateRotation);
    timer->start(100); // 每秒触发一次

    //ui->CamPose_view->setStyleSheet("background: rgb(125,125,125);border:0px");

}

Ins_CamLightPose::~Ins_CamLightPose()
{
    delete ui;
}

void Ins_CamLightPose::setAngleValue(double angle)
{
    ui->angleLabel->setText(QString::number(angle));
}

/*void Ins_CamLightPose::setupGraphicsViewWithImage(QGraphicsView *view, const QString &imagePath)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    QPixmap pixmap(imagePath);
    int customWidth = 300; // 宽度
    int customHeight = 200; // 高度
    pixmap = pixmap.scaled(customWidth, customHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //pixmap = pixmap.scaled(ui->CamPose_view->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //QGraphicsPixmapItem *item
    scene->addPixmap(pixmap);
    //scene->addPixmap(pixmap);
    //view->setFixedSize(50, 50);
    view->setScene(scene);

    //qreal xScale = view->width() / pixmap.width();
    //qreal yScale = view->height() / pixmap.height();
    //qreal scale = qMin(xScale, yScale);
    //item->setScale(scale);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);


    //double angle = getTLYAngle();
    //item->setTransformOriginPoint(item->boundingRect().center());
    //item->setRotation(angle);
    //return item;
}*/

/*QGraphicsPixmapItem* Ins_CamLightPose::setupGraphicsViewWithImage(QGraphicsView *view, const QString &imagePath)
{
    QGraphicsScene *scene = new QGraphicsScene(this);


    // 添加背景图片
    QPixmap backgroundPixmap("./image/cordin.png");

    int customWidth2 = 500; // 宽度
    int customHeight2 = 230; // 高度
    backgroundPixmap = backgroundPixmap.scaled(customWidth2, customHeight2,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *backgroundItem = scene->addPixmap(backgroundPixmap);
    backgroundItem->setFlag(QGraphicsItem::ItemIsMovable, false);  // 使其不可移动


    QPixmap pixmap(imagePath);
    int customWidth = 150; // 宽度
    int customHeight = 100; // 高度
    pixmap = pixmap.scaled(customWidth, customHeight,  Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = scene->addPixmap(pixmap);
    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);
    return item;
}*/

QGraphicsPixmapItem* Ins_CamLightPose::setupGraphicsViewWithImage(QGraphicsView *view, const QString &imagePath)
{
    QGraphicsScene *scene = new QGraphicsScene(this);

    // 添加背景图片
    QPixmap backgroundPixmap("./image/cordin.png");
    int customWidth2 = 500; // 宽度
    int customHeight2 = 230; // 高度
    backgroundPixmap = backgroundPixmap.scaled(customWidth2, customHeight2, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *backgroundItem = scene->addPixmap(backgroundPixmap);
    backgroundItem->setFlag(QGraphicsItem::ItemIsMovable, false);  // 使其不可移动

    QPixmap pixmap(imagePath);
    int customWidth = 150; // 宽度
    int customHeight = 120; // 高度
    pixmap = pixmap.scaled(customWidth, customHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *item = scene->addPixmap(pixmap);

    // 计算旋转图片的左上角坐标，使其居中于背景图片
    qreal xPos = (backgroundItem->boundingRect().width() - item->boundingRect().width()) / 2.0;
    qreal yPos = (backgroundItem->boundingRect().height() - item->boundingRect().height()) / 2.0;
    item->setPos(xPos, yPos);  // 设置图片的位置

    view->setScene(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);

    return item;
}

void Ins_CamLightPose::updateRotation()
{
    //double angle1 = qrand() % 91; // 生成0到90的随机数
    //double angle2 = qrand() % 91;

    //double angle1 = 0;
    //double angle2 = -179;

    //double angle1 = gyro.getAngle1(); //获取真实陀螺仪数据
    //double angle2 = gyro.getAngle2();

   /* ui->lineEdit->setText(QString::number(angle1, 'f', 2) + "°");//保留两位小鼠
    ui->lineEdit2->setText(QString::number(angle1, 'f', 2) + "°");//QString::number(angle1)+"°"
    ui->lineEdit3->setText(QString::number(angle1, 'f', 2) + "°");
    ui->lineEdit4->setText(QString::number(angle2, 'f', 2) + "°");
    ui->lineEdit5->setText(QString::number(angle2, 'f', 2) + "°");
    ui->lineEdit6->setText(QString::number(angle2, 'f', 2) + "°");*/

    item1->setTransformOriginPoint(item1->boundingRect().center());
    item2->setTransformOriginPoint(item2->boundingRect().center());
    item3->setTransformOriginPoint(item3->boundingRect().center());
    item4->setTransformOriginPoint(item4->boundingRect().center());
    item5->setTransformOriginPoint(item5->boundingRect().center());
    item6->setTransformOriginPoint(item6->boundingRect().center());
    item1->setRotation(0);
    item2->setRotation(0);
    item3->setRotation(0);
    item4->setRotation(0);
    item5->setRotation(0);
    item6->setRotation(0);




    item1->setTransformOriginPoint(item1->boundingRect().center());//相机
    item1->setRotation(InsGlobalVar::insMain->CamPosePirch);

    item2->setTransformOriginPoint(item2->boundingRect().center());
    item2->setRotation(InsGlobalVar::insMain->CamPoseRoll-180);

    item3->setTransformOriginPoint(item3->boundingRect().center());
    item3->setRotation(InsGlobalVar::insMain->CamPoseYaw);


    //=================================================================
    item4->setTransformOriginPoint(item4->boundingRect().center());
    item4->setRotation(InsGlobalVar::insMain->LightPosePirch);

    item5->setTransformOriginPoint(item5->boundingRect().center());
    item5->setRotation(-(InsGlobalVar::insMain->LightPoseRoll -90));

    item6->setTransformOriginPoint(item6->boundingRect().center());
    item6->setRotation(InsGlobalVar::insMain->LightPoseYaw);

    ui->lineEdit->setText(QString::number(InsGlobalVar::insMain->LightPosePirch,'f', 2) + "°");//保留两位小数//1122
    ui->lineEdit2->setText(QString::number(InsGlobalVar::insMain->LightPoseRoll, 'f', 2) + "°");//保留两位小数//1122
    ui->lineEdit3->setText(QString::number(InsGlobalVar::insMain->LightPoseYaw, 'f', 2) + "°");//保留两位小数//1122

    ui->lineEdit4->setText(QString::number(InsGlobalVar::insMain->CamPosePirch, 'f', 2) + "°");//保留两位小数//1122
    ui->lineEdit5->setText(QString::number(InsGlobalVar::insMain->CamPoseRoll, 'f', 2) + "°");//保留两位小数//1122
    ui->lineEdit6->setText(QString::number(InsGlobalVar::insMain->CamPoseYaw, 'f', 2) + "°");//保留两位小数//1122

    //ui->lineEdit2->setText(QString::number(angle2, 'f', 2) + "°");//QString::number(angle1)+"°"
    //ui->lineEdit3->setText(QString::number(angle2, 'f', 2) + "°");
    //ui->lineEdit4->setText(QString::number(angle1, 'f', 2) + "°");
    //ui->lineEdit5->setText(QString::number(angle1, 'f', 2) + "°");
    //ui->lineEdit6->setText(QString::number(angle1, 'f', 2) + "°");

    qDebug()<<"1111111111111111111111111111111111111"<<endl;
    qDebug()<<"LightPosePirch:::"<<InsGlobalVar::insMain->LightPosePirch<<endl;
    qDebug()<<"LightPosePirch:::"<<InsGlobalVar::insMain->LightPoseRoll<<endl;
    qDebug()<<"LightPosePirch:::"<<InsGlobalVar::insMain->LightPoseYaw<<endl;
    qDebug()<<"CamPosePirch:::"<<InsGlobalVar::insMain->CamPosePirch<<endl;
    qDebug()<<"CamPosePirch:::"<<InsGlobalVar::insMain->CamPoseRoll<<endl;
    qDebug()<<"CamPosePirch:::"<<InsGlobalVar::insMain->CamPoseYaw<<endl;

}


