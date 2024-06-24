#include "ins_encodeverify.h"
#include "ui_ins_encodeverify.h"
#include<QDebug>
#include"insglobalvar.h"

int decimalValue ;

int decimalValue2 ;

int decimalValue3 ;

ins_encodeVerify::ins_encodeVerify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ins_encodeVerify)
{
    ui->setupUi(this);

    QIcon iconnn(":/image/icon/ins.ico");
    setWindowIcon(iconnn);

    //QTimer *timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &ins_encodeVerify::ReadEncodePose);//1013
    //timer->start(1000);
}

ins_encodeVerify::~ins_encodeVerify()
{
    delete ui;
}

void ins_encodeVerify::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())
    {
        emit ABstatechange() ;
    }

    else
    {
        emit ABstatechange2() ;
    }


    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    qDebug()<<"!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;


}

void ins_encodeVerify::ReadEncodePose()
{
    bool ok;

    QString hexValue = InsGlobalVar::insMain->EncodeRead1;

    int decimalValue = hexValue.toInt(&ok,16);

    ui->insHeight->setValue(decimalValue);

}

void ins_encodeVerify::on_ModbusClientBtn_11_clicked()
{
    //bool ok;

    QString hexValue = InsGlobalVar::insMain->EncodeRead1;

    decimalValue = hexValue.toDouble();

    ui->insHeight->setValue(decimalValue);

    qDebug()<<"!!!!!!!!!!!!!"<<InsGlobalVar::insMain->EncodeRead1<<"!!!!!!!!!!!!!"<<endl;





    QString hexValue2 = InsGlobalVar::insMain->EncodeRead2;

    decimalValue2 = hexValue2.toDouble();

    ui->insWidth->setValue(decimalValue2);




    QString hexValue3 = InsGlobalVar::insMain->EncodeRead3;

    decimalValue3 = hexValue3.toDouble();

    ui->insWidth_2->setValue(decimalValue3);



}

void ins_encodeVerify::switchLanguage(bool english)
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

