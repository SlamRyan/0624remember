#include "ins_imagevalue.h"
#include "ui_ins_imagevalue.h"
#include <iostream>

ins_imagevalue::ins_imagevalue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ins_imagevalue)
{
    ui->setupUi(this);

    //imagevalue = new Ins_Widget;

    connect(ui->pushButton, &QPushButton::clicked, this, &ins_imagevalue::getGainValue);

    ui->Shape->setValue(1);
    ui->Diffuse->setValue(1);
    ui->Normal->setValue(1);
    ui->Specular->setValue(1);

}

ins_imagevalue::~ins_imagevalue()
{
    delete ui;
}


void ins_imagevalue::getGainValue()
{
    normalValue = ui->Normal->text().toDouble();
    std::cout<<normalValue<<std::endl;
    specularValue = ui->Specular->text().toDouble();
    diffuseValue = ui->Diffuse->text().toDouble();
    shapeValue = ui->Shape->text().toDouble();

    std::cout << "Values updated: " << normalValue << ", " << specularValue << ", " << diffuseValue << ", " << shapeValue << std::endl;

    emit valuesUpdated(normalValue, specularValue, diffuseValue, shapeValue);


}
