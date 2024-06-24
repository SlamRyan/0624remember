/********************************************************************************
** Form generated from reading UI file 'ins_devicechoose.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_DEVICECHOOSE_H
#define UI_INS_DEVICECHOOSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ins_devicechoose
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QListWidget *listW_Devices;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_11;
    QLabel *deviceDescription;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_8;
    QLabel *deviceMac;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_9;
    QLabel *deviceIP;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QLabel *deviceSubnet;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *PB_Search;
    QPushButton *PB_Select;
    QPushButton *PB_Close;
    QPushButton *handipBtn;
    QLineEdit *LineUpdateIp;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ins_devicechoose)
    {
        if (ins_devicechoose->objectName().isEmpty())
            ins_devicechoose->setObjectName(QString::fromUtf8("ins_devicechoose"));
        ins_devicechoose->resize(564, 417);
        ins_devicechoose->setStyleSheet(QString::fromUtf8("background-color: #303539"));
        gridLayout = new QGridLayout(ins_devicechoose);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(ins_devicechoose);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        verticalLayout_3->addWidget(label);

        listW_Devices = new QListWidget(ins_devicechoose);
        listW_Devices->setObjectName(QString::fromUtf8("listW_Devices"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listW_Devices->sizePolicy().hasHeightForWidth());
        listW_Devices->setSizePolicy(sizePolicy);
        listW_Devices->setStyleSheet(QString::fromUtf8(" background-color: #333333; "));

        verticalLayout_3->addWidget(listW_Devices);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 2);

        groupBox_2 = new QGroupBox(ins_devicechoose);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        horizontalLayout_7->addWidget(label_11);

        deviceDescription = new QLabel(groupBox_2);
        deviceDescription->setObjectName(QString::fromUtf8("deviceDescription"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(deviceDescription->sizePolicy().hasHeightForWidth());
        deviceDescription->setSizePolicy(sizePolicy2);

        horizontalLayout_7->addWidget(deviceDescription);

        horizontalLayout_7->setStretch(0, 3);
        horizontalLayout_7->setStretch(1, 8);

        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        horizontalLayout_8->addWidget(label_8);

        deviceMac = new QLabel(groupBox_2);
        deviceMac->setObjectName(QString::fromUtf8("deviceMac"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(deviceMac->sizePolicy().hasHeightForWidth());
        deviceMac->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(deviceMac);

        horizontalLayout_8->setStretch(0, 3);
        horizontalLayout_8->setStretch(1, 8);

        verticalLayout_2->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_9->addWidget(label_9);

        deviceIP = new QLabel(groupBox_2);
        deviceIP->setObjectName(QString::fromUtf8("deviceIP"));
        sizePolicy3.setHeightForWidth(deviceIP->sizePolicy().hasHeightForWidth());
        deviceIP->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(deviceIP);

        horizontalLayout_9->setStretch(0, 3);
        horizontalLayout_9->setStretch(1, 8);

        verticalLayout_2->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        horizontalLayout_10->addWidget(label_10);

        deviceSubnet = new QLabel(groupBox_2);
        deviceSubnet->setObjectName(QString::fromUtf8("deviceSubnet"));
        sizePolicy3.setHeightForWidth(deviceSubnet->sizePolicy().hasHeightForWidth());
        deviceSubnet->setSizePolicy(sizePolicy3);

        horizontalLayout_10->addWidget(deviceSubnet);

        horizontalLayout_10->setStretch(0, 3);
        horizontalLayout_10->setStretch(1, 8);

        verticalLayout_2->addLayout(horizontalLayout_10);


        gridLayout->addWidget(groupBox_2, 1, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        PB_Search = new QPushButton(ins_devicechoose);
        PB_Search->setObjectName(QString::fromUtf8("PB_Search"));
        sizePolicy3.setHeightForWidth(PB_Search->sizePolicy().hasHeightForWidth());
        PB_Search->setSizePolicy(sizePolicy3);
        PB_Search->setStyleSheet(QString::fromUtf8("#PB_Search\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
"background-color: #ffffff;\n"
" \n"
"\n"
"}\n"
"\n"
"#PB_Search::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#PB_Search::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(PB_Search);

        PB_Select = new QPushButton(ins_devicechoose);
        PB_Select->setObjectName(QString::fromUtf8("PB_Select"));
        sizePolicy3.setHeightForWidth(PB_Select->sizePolicy().hasHeightForWidth());
        PB_Select->setSizePolicy(sizePolicy3);
        PB_Select->setStyleSheet(QString::fromUtf8("#PB_Select\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
" background-color: #ffffff; \n"
"\n"
"}\n"
"\n"
"#PB_Select::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#PB_Select::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(PB_Select);

        PB_Close = new QPushButton(ins_devicechoose);
        PB_Close->setObjectName(QString::fromUtf8("PB_Close"));
        sizePolicy3.setHeightForWidth(PB_Close->sizePolicy().hasHeightForWidth());
        PB_Close->setSizePolicy(sizePolicy3);
        PB_Close->setStyleSheet(QString::fromUtf8("#PB_Close\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
" background-color: #ffffff; \n"
"\n"
"}\n"
"\n"
"#PB_Close::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#PB_Close::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(PB_Close);

        handipBtn = new QPushButton(ins_devicechoose);
        handipBtn->setObjectName(QString::fromUtf8("handipBtn"));
        sizePolicy3.setHeightForWidth(handipBtn->sizePolicy().hasHeightForWidth());
        handipBtn->setSizePolicy(sizePolicy3);
        handipBtn->setStyleSheet(QString::fromUtf8("#handipBtn\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
" background-color: #ffffff; \n"
"\n"
"}\n"
"\n"
"#handipBtn::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#handipBtn::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));

        verticalLayout->addWidget(handipBtn);

        LineUpdateIp = new QLineEdit(ins_devicechoose);
        LineUpdateIp->setObjectName(QString::fromUtf8("LineUpdateIp"));
        LineUpdateIp->setStyleSheet(QString::fromUtf8("color:#ffffff;"));

        verticalLayout->addWidget(LineUpdateIp);

        verticalSpacer_2 = new QSpacerItem(20, 13, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);


        gridLayout->addLayout(verticalLayout, 1, 1, 1, 1);


        retranslateUi(ins_devicechoose);

        QMetaObject::connectSlotsByName(ins_devicechoose);
    } // setupUi

    void retranslateUi(QDialog *ins_devicechoose)
    {
        ins_devicechoose->setWindowTitle(QApplication::translate("ins_devicechoose", "Dialog", nullptr));
        label->setText(QApplication::translate("ins_devicechoose", "\350\257\267\351\200\211\346\213\251\344\270\200\344\270\252\345\217\257\347\224\250\347\232\204\345\205\211\346\272\220\350\256\276\345\244\207\357\274\232", nullptr));
        groupBox_2->setTitle(QApplication::translate("ins_devicechoose", "\345\205\211\346\272\220\350\256\276\345\244\207\344\277\241\346\201\257", nullptr));
        label_11->setText(QApplication::translate("ins_devicechoose", "\350\256\276\345\244\207\347\240\201:", nullptr));
        deviceDescription->setText(QString());
        label_8->setText(QApplication::translate("ins_devicechoose", "MAC\345\234\260\345\235\200:", nullptr));
        deviceMac->setText(QString());
        label_9->setText(QApplication::translate("ins_devicechoose", "IP\345\234\260\345\235\200:", nullptr));
        deviceIP->setText(QString());
        label_10->setText(QApplication::translate("ins_devicechoose", "\345\255\220\347\275\221\346\216\251\347\240\201:", nullptr));
        deviceSubnet->setText(QString());
        PB_Search->setText(QApplication::translate("ins_devicechoose", "\346\220\234\347\264\242\350\256\276\345\244\207", nullptr));
        PB_Select->setText(QApplication::translate("ins_devicechoose", "\351\200\211\346\213\251\350\256\276\345\244\207", nullptr));
        PB_Close->setText(QApplication::translate("ins_devicechoose", "\345\205\263\351\227\255", nullptr));
        handipBtn->setText(QApplication::translate("ins_devicechoose", "\346\233\264\346\226\260 IP", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_devicechoose: public Ui_ins_devicechoose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_DEVICECHOOSE_H
