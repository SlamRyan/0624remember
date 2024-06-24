#ifndef INS_ENCODEVERIFY_H
#define INS_ENCODEVERIFY_H

#include <QDialog>
#include <QTranslator>


namespace Ui {
class ins_encodeVerify;
}

class ins_encodeVerify : public QDialog
{
    Q_OBJECT

public:
    explicit ins_encodeVerify(QWidget *parent = nullptr);
    ~ins_encodeVerify();

    QTranslator trans;

public slots:
    void on_checkBox_stateChanged(int arg1);
    void ReadEncodePose();

    void switchLanguage(bool english);


private:
    Ui::ins_encodeVerify *ui;


signals:
    void ABstatechange();

    void ABstatechange2() ;
private slots:
    void on_ModbusClientBtn_11_clicked();
};

#endif // INS_ENCODEVERIFY_H
