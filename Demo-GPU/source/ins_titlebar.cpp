#include "ins_titlebar.h"
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>

INS_TitleBar::INS_TitleBar(QWidget *parent)
    : QWidget{parent}
{
        // 创建标题栏的布局
        layoutV= new QVBoxLayout(this);
        layoutV->setContentsMargins(0, 0, 0, 0);
        layoutV->setSpacing(0);

        //存放title
        widget_title = new QWidget();
        widget_title->setContentsMargins(0,0,0,0);
        layout_title = new QHBoxLayout(widget_title);
        layout_title->setContentsMargins(5, 5, 5, 0);
        //存放menubar
        layout_menubar = new QHBoxLayout;
        layout_menubar->setContentsMargins(0, 0, 0, 0);

        // 添加窗口图标
        iconLabel = new QLabel(this);
        QPixmap iconPixmap(":/image/icon/ins2_b.png"); // 替换成你的图标文件路径
        double Proportion = 0.6; // 缩放比例
        iconLabel->setPixmap(iconPixmap.scaled(iconPixmap.width()  * Proportion, iconPixmap.height() * Proportion)); // 图片尺寸
        layout_title->addWidget(iconLabel);

        // 添加窗口标题
        titleLabel = new QLabel(u8"your title text", this);
        titleLabel->setStyleSheet("color:white; font:13px;");
        layout_title->addWidget(titleLabel);

        // 添加 spacer
        QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        layout_title->addSpacerItem(spacer);

        // 添加最小化按钮
        QSize size = QSize(25,25);
        QPushButton *minimizeButton = new QPushButton(u8"―", this);
        minimizeButton->setFixedSize(size);
        layout_title->addWidget(minimizeButton);
        minimizeButton->setStyleSheet("QPushButton{background:black; "
                                      "border:none;border-radius:3px; "
                                      "color:rgb(241, 241, 241);}"
                                      "QPushButton:hover { "
                                      "background-color: rgb(46, 46, 46); }"); // 启用悬停颜色变化
        connect(minimizeButton, &QPushButton::clicked, this, &INS_TitleBar::minimizeWindow);

        // 添加最大化/还原按钮
        QPushButton *maximizeButton = new QPushButton(u8"□", this);
        maximizeButton->setFixedSize(size);
        layout_title->addWidget(maximizeButton);
        maximizeButton->setStyleSheet("QPushButton{background:black;"
                                      "border:none;border-radius:3px; "
                                      "color:rgb(241, 241, 241);}"
                                      "QPushButton:hover { "
                                      "background-color: rgb(46, 46, 46); }"); // 启用悬停颜色变化
        connect(maximizeButton, &QPushButton::clicked, this, &INS_TitleBar::maximizeOrRestoreWindow);

        // 添加关闭按钮
        QPushButton *closeButton = new QPushButton(u8"×", this);
        closeButton->setFixedSize(size);
        layout_title->addWidget(closeButton);
        closeButton->setStyleSheet("QPushButton{background:black; "
                                   "border:none;border-radius:3px; "
                                   "color:rgb(241, 241, 241);}"
                                   "QPushButton:hover { "
                                   "background-color: rgb(46, 46, 46); }"); // 启用悬停颜色变化
        connect(closeButton, &QPushButton::clicked, this, &INS_TitleBar::closeWindow);



        layoutV->addWidget(widget_title);

        this->setStyleSheet(QString::fromUtf8("\n"
                                              ".QWidget{\n"
                                              "color:rgb(189, 189, 189);\n"
                                              "background-color:rgb(0, 0, 0);\n"
                                              "}\n"
                                                    "QMenuBar{\n"
                                                    "color:rgb(189, 189, 189);\n"
                                                    "background-color:rgb(20, 20, 20);\n"
                                                    "}\n"
                                                    "QMenuBar::item:selected {/*\346\257\217\344\270\252\350\217\234\345\215\225\351\224\256\347\232\204\346\240\267\345\274\217*/\n"
                                                    "background: rgb(46, 46, 46);\n"
                                                    "border-radius:3px;\n"
                                                    "color:rgb(241, 241, 241);\n"
                                                    "padding:1px 1px ;\n"
                                                    "margin:1px 1px ;\n"
                                                    "}\n"
                                                    "\n"
                                                    "/*\345\261\225\345\274\200\347\232\204\346\241\206\346\236\266*/\n"
                                                    "QMenu {\n"
                                                    "background-color:rgb(31, 31, 31);\n"
                                                    "border:1px solid rgb(117, 117, 117);\n"
                                                    "border-radius:3px;\n"
                                                    "color:rgb(180, 180, 180);\n"
                                                    "}\n"
                                                    "\n"
                                                    "QMenu::item:disabled {\n"
                                                    "color:rgb(79, 79, 79);\n"
                                                    "background: rgb(31, 31, 31);\n"
                                                    "border: none;\n"
                                                    "min-height:20px;\n"
                                                    "}\n"
                                                    "\n"
                                                    "/*\351\200\211\344\270\255\346\210\226\350\200\205\350\257\264\351\274\240\346\240\207\346\273\221\350\277\207\347\212\266\346\200\201*/\n"
                                                    "QMenu::item:selected {\n"
                                                    "color:rgb(232, 232, 232);\n"
                                                    "background: rgb(50, 50, 50);\n"
                                                    "border:1px solid rgb(50, 50, 50);\n"
                                                    "border-radius:3px;\n"
                                                    "}\n"
                                                    "QMenu:"
                                                    ":item:disabled:selected {\n"
                                                    "color:rgb(79, 79, 79);\n"
                                                    "background: rgb(31, 31, 31);\n"
                                                    "border: none;\n"
                                                    "}\n"
                                                    "/*\346\221\201\344\270\213\347\212\266\346\200\201*/\n"
                                                    "QMenu::item:pressed {\n"
                                                    "color:rgb(232, 232, 232);\n"
                                                    "background: rgb(50, 50, 50);\n"
                                                    "border:1px solid rgb(50, 50, 50);\n"
                                                    "border-radius:3px;\n"
                                                    "}\n"
                                                    "QMenu::item /*\345\261\225\345\274\200\347\232\204\351\200\211\351\241\271*/\n"
                                                    "{\n"
                                                    "min-width:50px;\n"
                                                    "font-size: 12px;\n"
                                                    "background: rgb(0, 0, 0);\n"
                                                    "border-radius:2px;\n"
                                                    "border:none;\n"
                                                    "padding:1px 1px 10px 1px;\n"
                                                    "margin:1px 1px 10px 1px;\n"
                                                    "font-color:rgb(189, 0, 189);\n"
                                                    "}\n"
                                                    ""));
}
