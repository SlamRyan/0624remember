#include "ins_spaceadjust.h"
#include "ui_ins_spaceadjust.h"

ins_spaceAdjust::ins_spaceAdjust(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ins_spaceAdjust)
{
    ui->setupUi(this);

    adjustimage = new Ins_Widget();
    qgraphicsScenesAdjust = new QGraphicsScene;
    qgraphicsScenesAdjust->addItem(adjustimage);

    ui->graphicsView->setScene(qgraphicsScenesAdjust);

    QPixmap pict("./image/X1.bmp");

    ShowWidth = 500; // 宽度
    ShowHeight = 500; // 高度

    pict = pict.scaled(ShowWidth, ShowHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!pict.isNull())
    {

        adjustimage->setImgPixmap(&pict);//setImgPixmap，用给定的QPixmap对象设置m_pix

    }

    connect(ui->SetRegionBtn, &QPushButton::clicked, this, &ins_spaceAdjust::startSelection);
    connect(ui->comfirmBtn, &QPushButton::clicked, this, &ins_spaceAdjust::confirmSelection);
    connect(ui->resetBtn, &QPushButton::clicked, this, &ins_spaceAdjust::resetSelection);


}

ins_spaceAdjust::~ins_spaceAdjust()
{
    delete ui;
}

void ins_spaceAdjust::startSelection()
{
    adjustimage->m_isSelecting = true;
}

void ins_spaceAdjust::confirmSelection()
{
    QRectF rect = adjustimage->getSelectionRect();
    qreal ratio = rect.width() / rect.height();
    // 处理或显示这个比率
    qDebug() << "Width/Height ratio:" << ratio;
    ui->WHValue->setText(QString::number(ratio));
}

void ins_spaceAdjust::resetSelection()
{
    adjustimage->m_isSelecting = false;
    adjustimage->m_selectionRect = QRectF();  // 使用默认构造函数创建一个空的矩形
    adjustimage->update();  // 更新界面以清除已绘制的矩形
}



