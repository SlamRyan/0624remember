#ifndef INS_DIRECTIONCHOOSE_H
#define INS_DIRECTIONCHOOSE_H

#include <QDialog>
#include <QTranslator>

namespace Ui {
class ins_DirectionChoose;
}

class ins_DirectionChoose : public QDialog
{
    Q_OBJECT

public:
    explicit ins_DirectionChoose(QWidget *parent = nullptr);
    ~ins_DirectionChoose();

    QTranslator trans;

public slots:
    void switchLanguage(bool english);

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::ins_DirectionChoose *ui;

signals:
    void chooseDirection1(int num);

};

#endif // INS_DIRECTIONCHOOSE_H
