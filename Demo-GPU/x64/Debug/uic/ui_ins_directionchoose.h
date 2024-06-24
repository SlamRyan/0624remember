/********************************************************************************
** Form generated from reading UI file 'ins_directionchoose.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_DIRECTIONCHOOSE_H
#define UI_INS_DIRECTIONCHOOSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ins_DirectionChoose
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_7;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label3;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label4;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QLabel *label_6;
    QVBoxLayout *verticalLayout_4;
    QLabel *label2;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ins_DirectionChoose)
    {
        if (ins_DirectionChoose->objectName().isEmpty())
            ins_DirectionChoose->setObjectName(QString::fromUtf8("ins_DirectionChoose"));
        ins_DirectionChoose->resize(551, 521);
        ins_DirectionChoose->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(ins_DirectionChoose);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_7 = new QLabel(ins_DirectionChoose);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(11);
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(label_7);

        comboBox = new QComboBox(ins_DirectionChoose);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(comboBox);


        gridLayout->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label3 = new QLabel(ins_DirectionChoose);
        label3->setObjectName(QString::fromUtf8("label3"));
        label3->setPixmap(QPixmap(QString::fromUtf8(":/icon/rightfan20.png")));

        verticalLayout->addWidget(label3);

        label_4 = new QLabel(ins_DirectionChoose);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8(""));

        verticalLayout->addWidget(label_4);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        label4 = new QLabel(ins_DirectionChoose);
        label4->setObjectName(QString::fromUtf8("label4"));
        label4->setStyleSheet(QString::fromUtf8("background-image: url(:/icon/rightfan.png);"));
        label4->setPixmap(QPixmap(QString::fromUtf8(":/icon/rightzheng20.png")));

        verticalLayout_2->addWidget(label4);

        label_5 = new QLabel(ins_DirectionChoose);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(label_5);


        horizontalLayout_2->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label = new QLabel(ins_DirectionChoose);
        label->setObjectName(QString::fromUtf8("label"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/icon/leftfan20.png")));

        verticalLayout_3->addWidget(label);

        label_6 = new QLabel(ins_DirectionChoose);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_3->addWidget(label_6);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label2 = new QLabel(ins_DirectionChoose);
        label2->setObjectName(QString::fromUtf8("label2"));
        label2->setStyleSheet(QString::fromUtf8(""));
        label2->setPixmap(QPixmap(QString::fromUtf8(":/icon/leftfzheng20.png")));

        verticalLayout_4->addWidget(label2);

        label_3 = new QLabel(ins_DirectionChoose);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_4->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout_4);


        gridLayout->addLayout(horizontalLayout, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer_2, 1, 0, 1, 1);


        retranslateUi(ins_DirectionChoose);

        QMetaObject::connectSlotsByName(ins_DirectionChoose);
    } // setupUi

    void retranslateUi(QDialog *ins_DirectionChoose)
    {
        ins_DirectionChoose->setWindowTitle(QApplication::translate("ins_DirectionChoose", "Dialog", nullptr));
        label_7->setText(QApplication::translate("ins_DirectionChoose", "         \351\200\211\346\213\251\344\275\215\345\247\277\357\274\232", nullptr));
        comboBox->setItemText(0, QApplication::translate("ins_DirectionChoose", "\344\275\215\345\247\2771", nullptr));
        comboBox->setItemText(1, QApplication::translate("ins_DirectionChoose", "\344\275\215\345\247\2772", nullptr));
        comboBox->setItemText(2, QApplication::translate("ins_DirectionChoose", "\344\275\215\345\247\2773", nullptr));
        comboBox->setItemText(3, QApplication::translate("ins_DirectionChoose", "\344\275\215\345\247\2774", nullptr));

        label3->setText(QString());
        label_4->setText(QApplication::translate("ins_DirectionChoose", "                   \344\275\215\345\247\2773", nullptr));
        label4->setText(QString());
        label_5->setText(QApplication::translate("ins_DirectionChoose", "                  \344\275\215\345\247\2774", nullptr));
        label->setText(QString());
        label_6->setText(QApplication::translate("ins_DirectionChoose", "                  \344\275\215\345\247\2771", nullptr));
        label2->setText(QString());
        label_3->setText(QApplication::translate("ins_DirectionChoose", "                  \344\275\215\345\247\2772", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_DirectionChoose: public Ui_ins_DirectionChoose {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_DIRECTIONCHOOSE_H
