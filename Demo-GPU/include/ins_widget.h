#ifndef INS_WIDGET_H
#define INS_WIDGET_H
#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>


enum Enum_ZoomState  //放缩状态
{
    NO_STATE,         //无状态
    RESET,            //重置
    ZOOM_IN,          //放大
    ZOOM_OUT          //缩小
};


class Ins_Widget: public QObject,public QGraphicsItem//交互
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    Ins_Widget();
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    wheelEvent(QGraphicsSceneWheelEvent *event); //滚轮事件
    void    ResetItemPos();
    void    SetItemPos();
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    qreal   getScaleValue() const; //获取当前放缩值
    void    setQGraphicsViewWH(int nwidth,int nheight);
    void    setImgPixmap(QPixmap *pixmap);
    //void    setImgPixmap(const QPixmap &pixmap);

    int getGrayValue(double x, double y);

    QRectF getSelectionRect() const//只读函数
    {
        return m_selectionRect;
    }



private:



    qreal       m_scaleValue;
    qreal       m_ImgCenterPos_x;
    qreal       m_ImgCenterPos_y;
    qreal       m_scaleDafault;
    QPixmap     m_pix;   //存储要显示的Qpixmap图像
    int         m_zoomState;
    bool        m_isMove;
    QPointF     m_startPos;
    //QGraphicsView *m_view = nullptr; //0807

public:
    //0821
    bool m_isSelecting = false ;         // 是否正在选择区域//0821
    QRectF m_selectionRect;     // 选择的矩形

signals:
    void sig_pixelView(double x ,double y,int grayValue);//0821

};

#endif // INS_WIDGET_H
