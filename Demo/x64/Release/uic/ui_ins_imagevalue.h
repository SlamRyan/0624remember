/********************************************************************************
** Form generated from reading UI file 'ins_imagevalue.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_IMAGEVALUE_H
#define UI_INS_IMAGEVALUE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ins_imagevalue
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_11;
    QSpacerItem *horizontalSpacer_11;
    QDoubleSpinBox *Normal;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_12;
    QSpacerItem *horizontalSpacer_12;
    QDoubleSpinBox *Specular;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_13;
    QSpacerItem *horizontalSpacer_13;
    QDoubleSpinBox *Diffuse;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QLabel *label_14;
    QSpacerItem *horizontalSpacer_14;
    QDoubleSpinBox *Shape;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_9;
    QCheckBox *checkBox_X1;
    QSpacerItem *horizontalSpacer_15;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_10;

    void setupUi(QDialog *ins_imagevalue)
    {
        if (ins_imagevalue->objectName().isEmpty())
            ins_imagevalue->setObjectName(QString::fromUtf8("ins_imagevalue"));
        ins_imagevalue->resize(457, 350);
        ins_imagevalue->setStyleSheet(QString::fromUtf8("    background-color: #303539"));
        gridLayout_2 = new QGridLayout(ins_imagevalue);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(ins_imagevalue);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QFont font;
        font.setPointSize(10);
        groupBox->setFont(font);
        groupBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout->addWidget(label_11);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_11);

        Normal = new QDoubleSpinBox(groupBox);
        Normal->setObjectName(QString::fromUtf8("Normal"));
        Normal->setMinimumSize(QSize(170, 26));
        Normal->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));
        Normal->setDecimals(0);
        Normal->setMaximum(100000.000000000000000);
        Normal->setValue(0.000000000000000);

        horizontalLayout->addWidget(Normal);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font);
        label_12->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_2->addWidget(label_12);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_12);

        Specular = new QDoubleSpinBox(groupBox);
        Specular->setObjectName(QString::fromUtf8("Specular"));
        Specular->setMinimumSize(QSize(170, 26));
        Specular->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));
        Specular->setDecimals(0);
        Specular->setMaximum(100000.000000000000000);
        Specular->setValue(0.000000000000000);

        horizontalLayout_2->addWidget(Specular);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_8);

        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font);
        label_13->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_3->addWidget(label_13);

        horizontalSpacer_13 = new QSpacerItem(37, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_13);

        Diffuse = new QDoubleSpinBox(groupBox);
        Diffuse->setObjectName(QString::fromUtf8("Diffuse"));
        Diffuse->setMinimumSize(QSize(170, 26));
        Diffuse->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));
        Diffuse->setDecimals(0);
        Diffuse->setMaximum(100000.000000000000000);
        Diffuse->setValue(0.000000000000000);

        horizontalLayout_3->addWidget(Diffuse);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setFont(font);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_4->addWidget(label_14);

        horizontalSpacer_14 = new QSpacerItem(37, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_14);

        Shape = new QDoubleSpinBox(groupBox);
        Shape->setObjectName(QString::fromUtf8("Shape"));
        Shape->setMinimumSize(QSize(170, 26));
        Shape->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 18, 20);\n"
"border:none;"));
        Shape->setDecimals(0);
        Shape->setMaximum(100000.000000000000000);
        Shape->setValue(0.000000000000000);

        horizontalLayout_4->addWidget(Shape);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_4, 3, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_9 = new QSpacerItem(30, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_9);

        checkBox_X1 = new QCheckBox(groupBox);
        checkBox_X1->setObjectName(QString::fromUtf8("checkBox_X1"));
        checkBox_X1->setFont(font);
        checkBox_X1->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));

        horizontalLayout_5->addWidget(checkBox_X1);

        horizontalSpacer_15 = new QSpacerItem(37, 17, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_15);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(0, 25));
        pushButton->setStyleSheet(QString::fromUtf8("#pushButton\n"
"{\n"
"border-radius:8px;\n"
"color: #000000;\n"
" background-color: #FFFFFF; \n"
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

        horizontalLayout_5->addWidget(pushButton);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_10);


        gridLayout->addLayout(horizontalLayout_5, 4, 0, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(ins_imagevalue);

        QMetaObject::connectSlotsByName(ins_imagevalue);
    } // setupUi

    void retranslateUi(QDialog *ins_imagevalue)
    {
        ins_imagevalue->setWindowTitle(QApplication::translate("ins_imagevalue", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("ins_imagevalue", "\345\217\202\346\225\260\350\256\276\345\256\232", nullptr));
        label_11->setText(QApplication::translate("ins_imagevalue", "Normal\345\242\236\347\233\212", nullptr));
        label_12->setText(QApplication::translate("ins_imagevalue", "Specularl\345\242\236\347\233\212", nullptr));
        label_13->setText(QApplication::translate("ins_imagevalue", "Diffuse\345\242\236\347\233\212", nullptr));
        label_14->setText(QApplication::translate("ins_imagevalue", "Shape\345\242\236\347\233\212", nullptr));
        checkBox_X1->setText(QApplication::translate("ins_imagevalue", "\347\273\235\345\257\271\345\200\274\346\250\241\345\274\217", nullptr));
        pushButton->setText(QApplication::translate("ins_imagevalue", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_imagevalue: public Ui_ins_imagevalue {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_IMAGEVALUE_H
