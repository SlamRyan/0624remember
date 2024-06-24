/********************************************************************************
** Form generated from reading UI file 'ins_encodeverify.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_ENCODEVERIFY_H
#define UI_INS_ENCODEVERIFY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ins_encodeVerify
{
public:
    QGridLayout *gridLayout;
    QLabel *label_70;
    QDoubleSpinBox *insHeight;
    QLabel *label_71;
    QDoubleSpinBox *insWidth;
    QLabel *label_72;
    QDoubleSpinBox *insWidth_2;
    QCheckBox *checkBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *ModbusClientBtn_11;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *ModbusClientBtn_12;
    QSpacerItem *horizontalSpacer_3;

    void setupUi(QDialog *ins_encodeVerify)
    {
        if (ins_encodeVerify->objectName().isEmpty())
            ins_encodeVerify->setObjectName(QString::fromUtf8("ins_encodeVerify"));
        ins_encodeVerify->resize(368, 198);
        ins_encodeVerify->setStyleSheet(QString::fromUtf8("background-color: #303539"));
        gridLayout = new QGridLayout(ins_encodeVerify);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_70 = new QLabel(ins_encodeVerify);
        label_70->setObjectName(QString::fromUtf8("label_70"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_70->sizePolicy().hasHeightForWidth());
        label_70->setSizePolicy(sizePolicy);
        label_70->setMinimumSize(QSize(58, 0));
        QFont font;
        font.setPointSize(10);
        label_70->setFont(font);
        label_70->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(label_70, 0, 0, 1, 1);

        insHeight = new QDoubleSpinBox(ins_encodeVerify);
        insHeight->setObjectName(QString::fromUtf8("insHeight"));
        insHeight->setMinimumSize(QSize(170, 26));
        insHeight->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-color: rgb(17, 18, 20);\n"
"border:none;"));
        insHeight->setDecimals(0);
        insHeight->setMinimum(-99999999999.000000000000000);
        insHeight->setMaximum(99999999999.000000000000000);
        insHeight->setValue(0.000000000000000);

        gridLayout->addWidget(insHeight, 0, 1, 1, 1);

        label_71 = new QLabel(ins_encodeVerify);
        label_71->setObjectName(QString::fromUtf8("label_71"));
        sizePolicy.setHeightForWidth(label_71->sizePolicy().hasHeightForWidth());
        label_71->setSizePolicy(sizePolicy);
        label_71->setMinimumSize(QSize(58, 0));
        label_71->setFont(font);
        label_71->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(label_71, 1, 0, 1, 1);

        insWidth = new QDoubleSpinBox(ins_encodeVerify);
        insWidth->setObjectName(QString::fromUtf8("insWidth"));
        insWidth->setMinimumSize(QSize(170, 26));
        insWidth->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;\n"
"color: rgb(255, 255, 255);"));
        insWidth->setDecimals(0);
        insWidth->setMinimum(-9999999999.000000000000000);
        insWidth->setMaximum(999999999999999.000000000000000);
        insWidth->setValue(0.000000000000000);

        gridLayout->addWidget(insWidth, 1, 1, 1, 1);

        label_72 = new QLabel(ins_encodeVerify);
        label_72->setObjectName(QString::fromUtf8("label_72"));
        sizePolicy.setHeightForWidth(label_72->sizePolicy().hasHeightForWidth());
        label_72->setSizePolicy(sizePolicy);
        label_72->setMinimumSize(QSize(58, 0));
        label_72->setFont(font);
        label_72->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(label_72, 2, 0, 1, 1);

        insWidth_2 = new QDoubleSpinBox(ins_encodeVerify);
        insWidth_2->setObjectName(QString::fromUtf8("insWidth_2"));
        insWidth_2->setMinimumSize(QSize(170, 26));
        insWidth_2->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;\n"
"color: rgb(255, 255, 255);"));
        insWidth_2->setDecimals(0);
        insWidth_2->setMinimum(-9999999999.000000000000000);
        insWidth_2->setMaximum(999999999999.000000000000000);
        insWidth_2->setValue(0.000000000000000);

        gridLayout->addWidget(insWidth_2, 2, 1, 1, 1);

        checkBox = new QCheckBox(ins_encodeVerify);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        gridLayout->addWidget(checkBox, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ModbusClientBtn_11 = new QPushButton(ins_encodeVerify);
        ModbusClientBtn_11->setObjectName(QString::fromUtf8("ModbusClientBtn_11"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ModbusClientBtn_11->sizePolicy().hasHeightForWidth());
        ModbusClientBtn_11->setSizePolicy(sizePolicy1);
        ModbusClientBtn_11->setMinimumSize(QSize(120, 46));
        ModbusClientBtn_11->setMaximumSize(QSize(120, 46));
        ModbusClientBtn_11->setFont(font);
        ModbusClientBtn_11->setStyleSheet(QString::fromUtf8("#ModbusClientBtn_11\n"
"{\n"
"border-radius:8px;\n"
"color: #FFFFFF;\n"
" background-color: #333333; \n"
"\n"
"}\n"
"\n"
"#ModbusClientBtn_11::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#ModbusClientBtn_11::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Res/LEDOFF.png"), QSize(), QIcon::Normal, QIcon::Off);
        ModbusClientBtn_11->setIcon(icon);

        horizontalLayout->addWidget(ModbusClientBtn_11);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        ModbusClientBtn_12 = new QPushButton(ins_encodeVerify);
        ModbusClientBtn_12->setObjectName(QString::fromUtf8("ModbusClientBtn_12"));
        sizePolicy1.setHeightForWidth(ModbusClientBtn_12->sizePolicy().hasHeightForWidth());
        ModbusClientBtn_12->setSizePolicy(sizePolicy1);
        ModbusClientBtn_12->setMinimumSize(QSize(120, 46));
        ModbusClientBtn_12->setMaximumSize(QSize(120, 46));
        ModbusClientBtn_12->setFont(font);
        ModbusClientBtn_12->setStyleSheet(QString::fromUtf8("#ModbusClientBtn_12\n"
"{\n"
"border-radius:8px;\n"
"color: #FFFFFF;\n"
" background-color: #333333; \n"
"\n"
"}\n"
"\n"
"#ModbusClientBtn_12::hover\n"
"{	\n"
"color: #FFFFFF;\n"
"\n"
"background-color: #1781b5;\n"
"\n"
"}\n"
"\n"
"#ModbusClientBtn_12::pressed,QPushButton::checked\n"
"{\n"
"    color: #FFFFFF;\n"
"   background-color: #718093;\n"
"}"));
        ModbusClientBtn_12->setIcon(icon);

        horizontalLayout->addWidget(ModbusClientBtn_12);

        horizontalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 2);


        retranslateUi(ins_encodeVerify);

        QMetaObject::connectSlotsByName(ins_encodeVerify);
    } // setupUi

    void retranslateUi(QDialog *ins_encodeVerify)
    {
        ins_encodeVerify->setWindowTitle(QApplication::translate("ins_encodeVerify", "Dialog", nullptr));
        label_70->setText(QApplication::translate("ins_encodeVerify", "\347\274\226\347\240\201\345\231\250\344\275\215\347\275\256", nullptr));
        label_71->setText(QApplication::translate("ins_encodeVerify", "A\347\233\270\350\256\241\346\225\260\345\200\274", nullptr));
        label_72->setText(QApplication::translate("ins_encodeVerify", "B\347\233\270\350\256\241\346\225\260\345\200\274", nullptr));
        checkBox->setText(QApplication::translate("ins_encodeVerify", "A_B\347\233\270\345\217\226\345\217\215", nullptr));
        ModbusClientBtn_11->setText(QApplication::translate("ins_encodeVerify", "\345\210\267\346\226\260\344\275\215\347\275\256", nullptr));
        ModbusClientBtn_12->setText(QApplication::translate("ins_encodeVerify", "\346\270\205\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_encodeVerify: public Ui_ins_encodeVerify {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_ENCODEVERIFY_H
