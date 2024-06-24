#include "ins_widget.h"
#include "insglobalvar.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>
#include<qgraphicsview.h>//0426

Ins_Widget::Ins_Widget()
{
    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptDrops(true);//If enabled is true, this item will accept hover events; otherwise, it will ignore them. By default, items do not accept hover events.
    m_scaleValue = 1;
    m_ImgCenterPos_x = 1;
    m_ImgCenterPos_y = 1;
    m_scaleDafault = 1;
    m_isMove = false;
}

void Ins_Widget::setImgPixmap(QPixmap *pixmap)
{
    m_pix = *pixmap;
    update();//0816jia
}



QRectF Ins_Widget::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2, m_pix.width(), m_pix.height());
}


void Ins_Widget::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)//绘制方法
{
    painter->drawPixmap(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
    if (!m_selectionRect.isNull())
    {
        painter->setPen(QPen(Qt::red, 2));
        painter->drawRect(m_selectionRect);
    }
}

void Ins_Widget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        m_isMove = true;//标记鼠标左键被按下
    }
    else if(event->button() == Qt::RightButton)
    {
        ResetItemPos();//右击鼠标重置大小
    }
    //emit printPixel(m_startPos);
    QGraphicsItem::mousePressEvent(event);*/

    if (m_isSelecting)
    {
        m_startPos = event->pos();
        m_selectionRect.setTopLeft(m_startPos);
        m_selectionRect.setBottomRight(m_startPos);
    }
    else
    {
        if(event->button()== Qt::LeftButton)
        {
            m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
            m_isMove = true;//标记鼠标左键被按下

            qDebug()<<"STARTPOS"<<m_startPos<<endl;
        }
        else if(event->button() == Qt::RightButton)
        {
            ResetItemPos();//右击鼠标重置大小
        }
        //emit printPixel(m_startPos);
        QGraphicsItem::mousePressEvent(event);
    }
}

void Ins_Widget::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

    if (m_isSelecting)
    {
        m_selectionRect.setBottomRight(event->pos());
        update();
    }
    else
    {
        QPointF point = (event->pos() - m_startPos)*m_scaleValue;
        //qDebug()<<event->pos().x()<<"  "<<event->pos().y();
        if(m_isMove)
        {
    
            m_ImgCenterPos_x += point.x();//0426
            m_ImgCenterPos_y += point.y() ;//0426
            if (1)//(m_ImgCenterPos_x > -800 && m_ImgCenterPos_x < 800)&&(m_ImgCenterPos_y > -700 && m_ImgCenterPos_y < 700)
            {
              
                qDebug() << "m_imagecenterX:" << m_ImgCenterPos_x << endl;
                qDebug() << "m_imagecentery:" << m_ImgCenterPos_y << endl;
                setPos(m_ImgCenterPos_x, m_ImgCenterPos_y);//0426
               
       
            }
            else
            {
                qDebug() << "XXXXXXXXXX" << endl;
                //setPos(0, 0);//0426
                ResetItemPos();//右击鼠标重置大小

            }
           
        }
        QGraphicsItem::mouseMoveEvent(event);
    }

}




void Ins_Widget::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    //m_isMove = false;//标记鼠标左键已经抬起

    if (m_isSelecting)
    {
        m_selectionRect = m_selectionRect.normalized();
        m_isSelecting = false;
    }
    else
    {
        m_isMove = false;//标记鼠标左键已经抬起
    }
}

/*void Ins_Widget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)//改在下
{
    qDebug()<<event->pos().x()<<"  "<<event->pos().y();
    emit sig_pixelView(event->pos().x(), event->pos().y());//发射新的信号 x ,y
    if(InsGlobalVar::pixelView_update_flag == FALSE)
    {
        InsGlobalVar::pixelView_update_flag = TRUE;
        emit sig_pixelView(event->pos().x(),event->pos().y());
    }
    QGraphicsItem::hoverMoveEvent(event);
}*/

void Ins_Widget::hoverMoveEvent(QGraphicsSceneHoverEvent *event)//悬停移动事件（不需要按下）
{
    double x = static_cast<int>(event->pos().x());
    double y = static_cast<int>(event->pos().y());
    int grayValue = getGrayValue(x, y);

    //qDebug() << x << "  " << y << " Gray: " << grayValue;
    emit sig_pixelView(x, y, grayValue);

    //if (InsGlobalVar::pixelView_update_flag == FALSE)//注释掉这里否则不刷新图片0825
    //{
    //    InsGlobalVar::pixelView_update_flag = TRUE;
    //    emit sig_pixelView(x, y, grayValue);
    //}
    QGraphicsItem::hoverMoveEvent(event);
}


void Ins_Widget::wheelEvent(QGraphicsSceneWheelEvent *event)//鼠标滚轮事件
{
    if((event->delta() > 0)&&(m_scaleValue >= 75))//最大放大到原始图像的75倍（实际81.4048）
    {
        std::cout<<"here1"<<std::endl;
        return;

    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDafault*0.01))//图像缩小到自适应大小*0.5之后就不继续缩小//0531
    {
        std::cout<<"here2"<<std::endl;
        //ResetItemPos();//重置图片大小和位置，使之自适应控件窗口大小
    }
    else
    {

        //std::cout<<"here3"<<std::endl;//0817
        qreal qrealOriginScale = m_scaleValue;//orisize
        if(event->delta() > 0)//鼠标滚轮向前滚动
        {
            m_scaleValue*=1.1;//每次放大10%
        }
        else
        {
            m_scaleValue*=0.9;//每次缩小10%
        }
        setScale(m_scaleValue);

//        if(event->delta() > 0)
//        {
//            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);//使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的
//        }
//        else
//        {
//            moveBy(event->pos().x()*qrealOriginScale*0.1 , event->pos().y()*qrealOriginScale*0.1);
//        }

        if(event->delta() > 0)
        {
            m_ImgCenterPos_x -= event->pos().x()*qrealOriginScale*0.1;
            m_ImgCenterPos_y -= event->pos().y()*qrealOriginScale*0.1 ;
        }
        else
        {
            m_ImgCenterPos_x += event->pos().x()*qrealOriginScale*0.1;
            m_ImgCenterPos_y += event->pos().y()*qrealOriginScale*0.1 ;
        }
        setPos(m_ImgCenterPos_x,m_ImgCenterPos_y);
    }
    // if (m_view)
    //{
    //    m_view->ensureVisible(this);
    //}

}



void Ins_Widget::setQGraphicsViewWH(int nwidth, int nheight)//将主界面的控件QGraphicsView的width和height传进本类中，并根据图像的长宽和控件的长宽的比例来使图片缩放到适合控件的大小
{
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;
    //qDebug()<<"nImgWidth"<<nImgWidth<<"nImgHeight"<<nImgHeight<<"temp1"<<temp1<<"temp2"<<temp2;
    if(temp1>temp2)
    {
        m_scaleDafault = temp2;
    }
    else
    {
        m_scaleDafault = temp1;
    }
    //setScale(m_scaleDafault);

    if(m_scaleValue== 0) m_scaleValue = m_scaleDafault;
    setScale(m_scaleValue);
    setPos(m_ImgCenterPos_x,m_ImgCenterPos_y);

    //qDebug()<<"m_scaleValue"<<m_scaleValue<<"       m_ImgCenterPos_x"<<m_ImgCenterPos_x<<"     m_ImgCenterPos_y"<<m_ImgCenterPos_y;
}

void Ins_Widget::ResetItemPos()//重置图片位置
{
    m_scaleValue = m_scaleDafault;//缩放比例回到一开始的自适应比例 //1129PM
    setScale(m_scaleDafault);//缩放到一开始的自适应大小//  //1019//注释掉->解开 //解开后右键自适应大小
    m_ImgCenterPos_x =  0;
    m_ImgCenterPos_y =  0;
    setPos(0,0);
}

void Ins_Widget::SetItemPos()//重置图片位置
{
    setScale(m_scaleValue);
    setPos(m_ImgCenterPos_x,m_ImgCenterPos_y);
}

qreal Ins_Widget::getScaleValue() const//GetScaleValue
{
    return m_scaleValue;
}



/*int Ins_Widget::getGrayValue(double x, double y)


{
    if (m_pix.isNull() || x < 0 || x >= m_pix.width() || y < 0 || y >= m_pix.height())
    {
        return -1;  // 或其他默认值
    }

    QImage image = m_pix.toImage();
    QColor color(image.pixel(x, y));
    return qGray(color.rgb());//togray
}*/

int Ins_Widget::getGrayValue(double x, double y)
{
    double mappedX = x + m_pix.width() / 2;
    double mappedY = y + m_pix.height() / 2;

    if (m_pix.isNull() || mappedX < 0 || mappedX >= m_pix.width() || mappedY < 0 || mappedY >= m_pix.height())
    {
        return -1;
    }

    QImage image = m_pix.toImage();
    QColor color(image.pixel(mappedX, mappedY));
    return qGray(color.rgb());
}



