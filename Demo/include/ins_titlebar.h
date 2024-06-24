#ifndef INS_TITLEBAR_H
#define INS_TITLEBAR_H

#include <QLabel>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QScreen>

class INS_TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit INS_TitleBar(QWidget *parent = nullptr);
    QVBoxLayout *layoutV;
    QHBoxLayout *layout_title;
    QWidget* widget_title;
    QHBoxLayout *layout_menubar;
    QLabel *titleLabel;
    QLabel *iconLabel;

public:
    void setNormalwidth(int w)
    {
        normalwidth = w;
    }

signals:
    void minimizeClicked();
    void maximizeOrRestoreClicked();
    void closeClicked();

private slots:
    void minimizeWindow() {
        emit minimizeClicked();
    }

    void maximizeOrRestoreWindow() {
        emit maximizeOrRestoreClicked();
    }

    void closeWindow() {
        emit closeClicked();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            isMousePressed = true;
            // 记录当前鼠标位置和窗口位置，用于计算移动偏移
            mousePressPos = event->globalPos();
            windowPos = this->window()->pos();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isMousePressed) {

            if(this->window()->windowState() == Qt::WindowState::WindowMaximized) {

                double Proportion = (double)mousePressPos.x()/(double)this->window()->width();

                qDebug() << mousePressPos.x() << this->window()->width() << Proportion;

                this->window()->showNormal();

                windowPos.setX( mousePressPos.x() - normalwidth * Proportion );

                this->window()->move(windowPos);
                qDebug() << mousePressPos << this->window()->normalGeometry().width() << normalwidth;
                return;
            }
            else if(this->window()->windowState() == Qt::WindowState::WindowNoState) {

                // 计算移动偏移
                QPoint moveOffset = event->globalPos() - mousePressPos;
                // 设置窗口新的位置
                this->window()->move(windowPos + moveOffset);

//                if(event->globalPos().y() == 0 ){
//                    this->window()->showMaximized();
//                }
            }
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        Q_UNUSED(event);
        isMousePressed = false;
    }


private:
    bool isMousePressed;
    QPoint mousePressPos;
    QPoint windowPos;

    int normalwidth;
};

#endif // INS_TITLEBAR_H
