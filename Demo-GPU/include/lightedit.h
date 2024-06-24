#ifndef LIGHTEDIT_H
#define LIGHTEDIT_H

#include <QDialog>
#include "ins_widget.h"

namespace Ui {
class LightEdit;
}

class LightEdit : public QDialog
{
    Q_OBJECT

public:
    explicit LightEdit(QWidget *parent = nullptr);
    ~LightEdit();


    Ins_Widget* LightEditing;


private slots:
    void onCheckboxStateChanged();



private:
    Ui::LightEdit *ui;
    uint32_t valueX = 0;
    uint32_t valueY = 0;
};

#endif // LIGHTEDIT_H
