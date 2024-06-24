/********************************************************************************
** Form generated from reading UI file 'ins_camlightpose.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_CAMLIGHTPOSE_H
#define UI_INS_CAMLIGHTPOSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_Ins_CamLightPose
{
public:
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *LightPose_view;
    QGraphicsView *LightPose_view2;
    QGraphicsView *LightPose_view3;
    QLabel *angleLabel;
    QLineEdit *lineEdit;
    QLabel *angleLabel_3;
    QLineEdit *lineEdit2;
    QLabel *angleLabel_4;
    QLineEdit *lineEdit3;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *CamPose_view;
    QGraphicsView *CamPose_view2;
    QGraphicsView *CamPose_view3;
    QLineEdit *lineEdit5;
    QLineEdit *lineEdit6;
    QLabel *angleLabel_5;
    QLineEdit *lineEdit4;
    QLabel *angleLabel_6;
    QLabel *angleLabel_2;

    void setupUi(QDialog *Ins_CamLightPose)
    {
        if (Ins_CamLightPose->objectName().isEmpty())
            Ins_CamLightPose->setObjectName(QString::fromUtf8("Ins_CamLightPose"));
        Ins_CamLightPose->resize(843, 680);
        Ins_CamLightPose->setStyleSheet(QString::fromUtf8("    background-color: #303539"));
        gridLayout_3 = new QGridLayout(Ins_CamLightPose);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(Ins_CamLightPose);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font;
        font.setPointSize(10);
        groupBox->setFont(font);
        groupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        LightPose_view = new QGraphicsView(groupBox);
        LightPose_view->setObjectName(QString::fromUtf8("LightPose_view"));
        QFont font1;
        font1.setPointSize(11);
        LightPose_view->setFont(font1);
        LightPose_view->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout->addWidget(LightPose_view);

        LightPose_view2 = new QGraphicsView(groupBox);
        LightPose_view2->setObjectName(QString::fromUtf8("LightPose_view2"));
        LightPose_view2->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout->addWidget(LightPose_view2);

        LightPose_view3 = new QGraphicsView(groupBox);
        LightPose_view3->setObjectName(QString::fromUtf8("LightPose_view3"));
        LightPose_view3->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout->addWidget(LightPose_view3);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 6);

        angleLabel = new QLabel(groupBox);
        angleLabel->setObjectName(QString::fromUtf8("angleLabel"));

        gridLayout->addWidget(angleLabel, 1, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 30));
        QFont font2;
        font2.setPointSize(15);
        lineEdit->setFont(font2);
        lineEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout->addWidget(lineEdit, 1, 1, 1, 1);

        angleLabel_3 = new QLabel(groupBox);
        angleLabel_3->setObjectName(QString::fromUtf8("angleLabel_3"));

        gridLayout->addWidget(angleLabel_3, 1, 2, 1, 1);

        lineEdit2 = new QLineEdit(groupBox);
        lineEdit2->setObjectName(QString::fromUtf8("lineEdit2"));
        lineEdit2->setMinimumSize(QSize(0, 30));
        lineEdit2->setFont(font2);
        lineEdit2->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout->addWidget(lineEdit2, 1, 3, 1, 1);

        angleLabel_4 = new QLabel(groupBox);
        angleLabel_4->setObjectName(QString::fromUtf8("angleLabel_4"));

        gridLayout->addWidget(angleLabel_4, 1, 4, 1, 1);

        lineEdit3 = new QLineEdit(groupBox);
        lineEdit3->setObjectName(QString::fromUtf8("lineEdit3"));
        lineEdit3->setMinimumSize(QSize(0, 30));
        lineEdit3->setFont(font2);
        lineEdit3->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout->addWidget(lineEdit3, 1, 5, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(Ins_CamLightPose);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setFont(font);
        groupBox_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        CamPose_view = new QGraphicsView(groupBox_2);
        CamPose_view->setObjectName(QString::fromUtf8("CamPose_view"));
        CamPose_view->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout_2->addWidget(CamPose_view);

        CamPose_view2 = new QGraphicsView(groupBox_2);
        CamPose_view2->setObjectName(QString::fromUtf8("CamPose_view2"));
        CamPose_view2->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout_2->addWidget(CamPose_view2);

        CamPose_view3 = new QGraphicsView(groupBox_2);
        CamPose_view3->setObjectName(QString::fromUtf8("CamPose_view3"));
        CamPose_view3->setStyleSheet(QString::fromUtf8("background-color: #202328;\n"
"background-image: url(:/image/icon/cordinFinal2.png);"));

        horizontalLayout_2->addWidget(CamPose_view3);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 6);

        lineEdit5 = new QLineEdit(groupBox_2);
        lineEdit5->setObjectName(QString::fromUtf8("lineEdit5"));
        lineEdit5->setMinimumSize(QSize(0, 30));
        lineEdit5->setFont(font2);
        lineEdit5->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout_2->addWidget(lineEdit5, 1, 3, 1, 1);

        lineEdit6 = new QLineEdit(groupBox_2);
        lineEdit6->setObjectName(QString::fromUtf8("lineEdit6"));
        lineEdit6->setMinimumSize(QSize(0, 30));
        lineEdit6->setFont(font2);
        lineEdit6->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout_2->addWidget(lineEdit6, 1, 5, 1, 1);

        angleLabel_5 = new QLabel(groupBox_2);
        angleLabel_5->setObjectName(QString::fromUtf8("angleLabel_5"));

        gridLayout_2->addWidget(angleLabel_5, 1, 2, 1, 1);

        lineEdit4 = new QLineEdit(groupBox_2);
        lineEdit4->setObjectName(QString::fromUtf8("lineEdit4"));
        lineEdit4->setMinimumSize(QSize(0, 30));
        lineEdit4->setFont(font2);
        lineEdit4->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));

        gridLayout_2->addWidget(lineEdit4, 1, 1, 1, 1);

        angleLabel_6 = new QLabel(groupBox_2);
        angleLabel_6->setObjectName(QString::fromUtf8("angleLabel_6"));

        gridLayout_2->addWidget(angleLabel_6, 1, 4, 1, 1);

        angleLabel_2 = new QLabel(groupBox_2);
        angleLabel_2->setObjectName(QString::fromUtf8("angleLabel_2"));

        gridLayout_2->addWidget(angleLabel_2, 1, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 2, 0, 1, 1);


        retranslateUi(Ins_CamLightPose);

        QMetaObject::connectSlotsByName(Ins_CamLightPose);
    } // setupUi

    void retranslateUi(QDialog *Ins_CamLightPose)
    {
        Ins_CamLightPose->setWindowTitle(QApplication::translate("Ins_CamLightPose", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("Ins_CamLightPose", "\345\205\211\346\272\220", nullptr));
        angleLabel->setText(QApplication::translate("Ins_CamLightPose", "\350\203\214\351\235\242", nullptr));
        angleLabel_3->setText(QApplication::translate("Ins_CamLightPose", "\344\276\247\351\235\242", nullptr));
        angleLabel_4->setText(QApplication::translate("Ins_CamLightPose", "\344\270\212\346\226\271", nullptr));
        groupBox_2->setTitle(QApplication::translate("Ins_CamLightPose", "\347\233\270\346\234\272", nullptr));
        angleLabel_5->setText(QApplication::translate("Ins_CamLightPose", "\344\276\247\351\235\242", nullptr));
        angleLabel_6->setText(QApplication::translate("Ins_CamLightPose", "\344\270\212\346\226\271", nullptr));
        angleLabel_2->setText(QApplication::translate("Ins_CamLightPose", "\350\203\214\351\235\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Ins_CamLightPose: public Ui_Ins_CamLightPose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_CAMLIGHTPOSE_H
