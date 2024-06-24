/********************************************************************************
** Form generated from reading UI file 'ins_spaceadjust.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INS_SPACEADJUST_H
#define UI_INS_SPACEADJUST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ins_spaceAdjust
{
public:
    QGridLayout *gridLayout;
    QGraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout;
    QPushButton *SetRegionBtn;
    QPushButton *comfirmBtn;
    QPushButton *resetBtn;
    QLabel *label;
    QLineEdit *WHValue;

    void setupUi(QDialog *ins_spaceAdjust)
    {
        if (ins_spaceAdjust->objectName().isEmpty())
            ins_spaceAdjust->setObjectName(QString::fromUtf8("ins_spaceAdjust"));
        ins_spaceAdjust->resize(931, 656);
        gridLayout = new QGridLayout(ins_spaceAdjust);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        graphicsView = new QGraphicsView(ins_spaceAdjust);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setStyleSheet(QString::fromUtf8("background-color: #202328;"));

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        SetRegionBtn = new QPushButton(ins_spaceAdjust);
        SetRegionBtn->setObjectName(QString::fromUtf8("SetRegionBtn"));

        horizontalLayout->addWidget(SetRegionBtn);

        comfirmBtn = new QPushButton(ins_spaceAdjust);
        comfirmBtn->setObjectName(QString::fromUtf8("comfirmBtn"));

        horizontalLayout->addWidget(comfirmBtn);

        resetBtn = new QPushButton(ins_spaceAdjust);
        resetBtn->setObjectName(QString::fromUtf8("resetBtn"));

        horizontalLayout->addWidget(resetBtn);

        label = new QLabel(ins_spaceAdjust);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(140, 16777215));

        horizontalLayout->addWidget(label);

        WHValue = new QLineEdit(ins_spaceAdjust);
        WHValue->setObjectName(QString::fromUtf8("WHValue"));
        WHValue->setMaximumSize(QSize(100, 20));

        horizontalLayout->addWidget(WHValue);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);


        retranslateUi(ins_spaceAdjust);

        QMetaObject::connectSlotsByName(ins_spaceAdjust);
    } // setupUi

    void retranslateUi(QDialog *ins_spaceAdjust)
    {
        ins_spaceAdjust->setWindowTitle(QApplication::translate("ins_spaceAdjust", "Dialog", nullptr));
        SetRegionBtn->setText(QApplication::translate("ins_spaceAdjust", "\350\214\203\345\233\264\351\200\211\345\217\226", nullptr));
        comfirmBtn->setText(QApplication::translate("ins_spaceAdjust", "\347\241\256\345\256\232", nullptr));
        resetBtn->setText(QApplication::translate("ins_spaceAdjust", "\351\207\215\347\275\256", nullptr));
        label->setText(QApplication::translate("ins_spaceAdjust", "  \345\256\275 / \351\253\230 :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ins_spaceAdjust: public Ui_ins_spaceAdjust {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INS_SPACEADJUST_H
