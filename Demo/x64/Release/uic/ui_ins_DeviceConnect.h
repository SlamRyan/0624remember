/********************************************************************************
** Form generated from reading UI file 'ins_DeviceConnect.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_DEVICECONNECT_H
#define UI_INS_DEVICECONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ins_DeviceConnectClass
{
public:
    QGridLayout *gridLayout;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *SystemList;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *CameraList;
    QListWidget *LightList;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_3;
    QPushButton *pushButton5;
    QSpacerItem *verticalSpacer_4;
    QPushButton *pushButton4;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton5_2;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLabel *searchLabel;

    void setupUi(QDialog *ins_DeviceConnectClass)
    {
        if (ins_DeviceConnectClass->objectName().isEmpty())
            ins_DeviceConnectClass->setObjectName(QString::fromUtf8("ins_DeviceConnectClass"));
        ins_DeviceConnectClass->resize(602, 464);
        ins_DeviceConnectClass->setStyleSheet(QString::fromUtf8("background-color: #303539"));
        gridLayout = new QGridLayout(ins_DeviceConnectClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_4 = new QLabel(ins_DeviceConnectClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMaximumSize(QSize(250, 15));
        label_4->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        SystemList = new QListWidget(ins_DeviceConnectClass);
        SystemList->setObjectName(QString::fromUtf8("SystemList"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SystemList->sizePolicy().hasHeightForWidth());
        SystemList->setSizePolicy(sizePolicy);
        SystemList->setMaximumSize(QSize(16777215, 150));
        SystemList->setStyleSheet(QString::fromUtf8(" background-color: #333333; "));

        horizontalLayout_3->addWidget(SystemList);


        gridLayout->addLayout(horizontalLayout_3, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(ins_DeviceConnectClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(250, 15));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(label);

        label_2 = new QLabel(ins_DeviceConnectClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(250, 15));
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(label_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        CameraList = new QListWidget(ins_DeviceConnectClass);
        CameraList->setObjectName(QString::fromUtf8("CameraList"));
        sizePolicy.setHeightForWidth(CameraList->sizePolicy().hasHeightForWidth());
        CameraList->setSizePolicy(sizePolicy);
        CameraList->setStyleSheet(QString::fromUtf8(" background-color: #333333; "));

        horizontalLayout_2->addWidget(CameraList);

        LightList = new QListWidget(ins_DeviceConnectClass);
        LightList->setObjectName(QString::fromUtf8("LightList"));
        sizePolicy.setHeightForWidth(LightList->sizePolicy().hasHeightForWidth());
        LightList->setSizePolicy(sizePolicy);
        LightList->setStyleSheet(QString::fromUtf8(" background-color: #333333; "));

        horizontalLayout_2->addWidget(LightList);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(ins_DeviceConnectClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(70, 40));
        pushButton->setStyleSheet(QString::fromUtf8("#pushButton\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#pushButton::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#pushButton::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(pushButton);

        verticalSpacer_3 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        pushButton5 = new QPushButton(ins_DeviceConnectClass);
        pushButton5->setObjectName(QString::fromUtf8("pushButton5"));
        pushButton5->setMinimumSize(QSize(70, 40));
        pushButton5->setStyleSheet(QString::fromUtf8("#pushButton5\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#pushButton5::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#pushButton5::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(pushButton5);

        verticalSpacer_4 = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_4);

        pushButton4 = new QPushButton(ins_DeviceConnectClass);
        pushButton4->setObjectName(QString::fromUtf8("pushButton4"));
        pushButton4->setMinimumSize(QSize(70, 40));
        pushButton4->setStyleSheet(QString::fromUtf8("#pushButton4\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#pushButton4::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#pushButton4::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(pushButton4);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton5_2 = new QPushButton(ins_DeviceConnectClass);
        pushButton5_2->setObjectName(QString::fromUtf8("pushButton5_2"));
        pushButton5_2->setMinimumSize(QSize(70, 40));
        pushButton5_2->setStyleSheet(QString::fromUtf8("#pushButton5_2\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#pushButton5_2::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#pushButton5_2::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(pushButton5_2);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        pushButton2 = new QPushButton(ins_DeviceConnectClass);
        pushButton2->setObjectName(QString::fromUtf8("pushButton2"));
        pushButton2->setMinimumSize(QSize(70, 40));
        pushButton2->setStyleSheet(QString::fromUtf8("#pushButton2\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#pushButton2::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#pushButton2::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(pushButton2);


        gridLayout->addLayout(verticalLayout, 1, 1, 5, 1);

        horizontalSpacer = new QSpacerItem(74, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(ins_DeviceConnectClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(250, 15));
        label_3->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit = new QLineEdit(ins_DeviceConnectClass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_4->addWidget(lineEdit);


        gridLayout->addLayout(horizontalLayout_4, 5, 0, 1, 1);

        searchLabel = new QLabel(ins_DeviceConnectClass);
        searchLabel->setObjectName(QString::fromUtf8("searchLabel"));
        searchLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(searchLabel, 2, 0, 1, 1);


        retranslateUi(ins_DeviceConnectClass);

        QMetaObject::connectSlotsByName(ins_DeviceConnectClass);
    } // setupUi

    void retranslateUi(QDialog *ins_DeviceConnectClass)
    {
        ins_DeviceConnectClass->setWindowTitle(QApplication::translate("ins_DeviceConnectClass", "ins_DeviceConnect", nullptr));
        label_4->setText(QApplication::translate("ins_DeviceConnectClass", "\345\267\262\345\255\230\345\234\250\347\263\273\347\273\237\357\274\232", nullptr));
        label->setText(QApplication::translate("ins_DeviceConnectClass", "\347\233\270\346\234\272\344\277\241\346\201\257\357\274\232", nullptr));
        label_2->setText(QApplication::translate("ins_DeviceConnectClass", "\345\205\211\346\272\220\344\277\241\346\201\257\357\274\232", nullptr));
        pushButton->setText(QApplication::translate("ins_DeviceConnectClass", "\350\256\276\345\244\207\346\220\234\347\264\242", nullptr));
        pushButton5->setText(QApplication::translate("ins_DeviceConnectClass", "\347\263\273\347\273\237\347\273\204\345\220\210", nullptr));
        pushButton4->setText(QApplication::translate("ins_DeviceConnectClass", "\347\263\273\347\273\237\350\277\236\346\216\245", nullptr));
        pushButton5_2->setText(QApplication::translate("ins_DeviceConnectClass", "\347\263\273\347\273\237\345\244\215\344\275\215", nullptr));
        pushButton2->setText(QApplication::translate("ins_DeviceConnectClass", "\346\233\264\346\224\271IP", nullptr));
        label_3->setText(QApplication::translate("ins_DeviceConnectClass", "\350\276\223\345\205\245\346\233\264\346\224\271\347\232\204IP\357\274\232", nullptr));
        searchLabel->setText(QApplication::translate("ins_DeviceConnectClass", "1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_DeviceConnectClass: public Ui_ins_DeviceConnectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_DEVICECONNECT_H
