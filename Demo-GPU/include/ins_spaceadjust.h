#ifndef INS_SPACEADJUST_H
#define INS_SPACEADJUST_H

#include"ins_widget.h"
#include <QDialog>

namespace Ui
{
class ins_spaceAdjust;
}

class ins_spaceAdjust : public QDialog
{
    Q_OBJECT

public:
    explicit ins_spaceAdjust(QWidget *parent = nullptr);
    ~ins_spaceAdjust();

    Ins_Widget* adjustimage;
    QGraphicsScene* qgraphicsScenesAdjust;
    QGraphicsPixmapItem *itemAdjust;
    int ShowWidth;
    int ShowHeight;

public slots:

    void startSelection();


    void confirmSelection();

    void resetSelection();


private:
    Ui::ins_spaceAdjust *ui;
};

#endif // INS_SPACEADJUST_H


