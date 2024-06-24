#include "ins_directionchoose.h"
#include "ui_ins_directionchoose.h"

ins_DirectionChoose::ins_DirectionChoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ins_DirectionChoose)
{
    ui->setupUi(this);



    QIcon iconnn(":/image/icon/ins.ico");
    setWindowIcon(iconnn);

   QPixmap pixmapleftFan(":/image/icon/leftfan10.png");

   ui->label->setPixmap(pixmapleftFan);


   QPixmap pixmapleftZheng(":/image/icon/leftzheng10.png");
   ui->label2->setPixmap(pixmapleftZheng);

   QPixmap pixmaprightFan(":/image/icon/rightfan10.png");
   ui->label3->setPixmap(pixmaprightFan);



   QPixmap pixmaprightZheng(":/image/icon/rightzheng10.png");
   ui->label4->setPixmap(pixmaprightZheng);


}

ins_DirectionChoose::~ins_DirectionChoose()
{
    delete ui;
}

void ins_DirectionChoose::on_comboBox_currentIndexChanged(int index)
{

    if(index ==0)
    {
        ui->label->setFrameShape (QFrame::Box);
        ui->label->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label2->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label3->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label4->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
    }

    if(index ==1)
    {
        ui->label2->setFrameShape (QFrame::Box);
        ui->label2->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label3->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label4->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
    }

    if(index ==2)
    {
        ui->label3->setFrameShape (QFrame::Box);
        ui->label3->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label2->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label4->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
    }

    if(index ==3)
    {
        ui->label4->setFrameShape (QFrame::Box);
        ui->label4->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label3->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
        ui->label2->setStyleSheet("border-width: 0px;border-style: solid;border-color: rgb(255, 170, 0);");
    }

    emit chooseDirection1(index);
}

void ins_DirectionChoose::switchLanguage(bool english)
{
    if (english)
    {
        trans.load("Translation_en_en_US.qm");
        qApp->installTranslator(&trans);
        ui->retranslateUi(this);
    }
    else
    {
        ui->retranslateUi(this);
    }

}


