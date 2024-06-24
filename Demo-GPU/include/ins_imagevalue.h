#ifndef INS_IMAGEVALUE_H
#define INS_IMAGEVALUE_H


#include <QDialog>
#include"ins_widget.h"

namespace Ui {
class ins_imagevalue;
}

class ins_imagevalue : public QDialog
{
    Q_OBJECT

public:
    explicit ins_imagevalue(QWidget *parent = nullptr);
    ~ins_imagevalue();

    double normalValue =1;//0904
    double specularValue =1 ;
    double diffuseValue =1 ;
    double shapeValue =1 ;

private:
    Ui::ins_imagevalue *ui;

    //Ins_Widget* imagevalue;


public slots:
    void getGainValue();

signals:
    void valuesUpdated(double normal, double specular, double diffuse, double shape);


};

#endif // INS_IMAGEVALUE_H
