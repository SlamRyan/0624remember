#include "lightedit.h"
#include "ui_lightedit.h"

LightEdit::LightEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LightEdit)
{
    ui->setupUi(this);

    LightEditing = new Ins_Widget();

    //ui->labelX2->setStyleSheet("background-color: gray;");
    //ui->labelX1->setStyleSheet("background-color: gray;");

    for (int i = 1; i <= 24; i++)
    {
        QString labelName = QString("labelX%1").arg(i);  // 构建label名称
        QLabel *label = this->findChild<QLabel *>(labelName);  // 根据名字查找QLabel

        if (label)
        {  // IF FIND
            label->setStyleSheet("background-color: lightgray;");
        }
    }

    for (int i = 1; i <= 24; i++)
    {
        QString labelName = QString("labelY%1").arg(i);  // 构建label名称
        QLabel *label = this->findChild<QLabel *>(labelName);  // 根据名字查找QLabel

        if (label)
        {  // IF FINE
            label->setStyleSheet("background-color: lightgray;");
        }
    }


    for (int i = 1; i <= 24; i++)
    {
        QString checkBoxName = QString("checkBoxX%1").arg(i);
        QCheckBox* checkBox = this->findChild<QCheckBox*>(checkBoxName);

        if (checkBox)
        {
            connect(checkBox, &QCheckBox::stateChanged, this, &LightEdit::onCheckboxStateChanged);
        }
    }

    for (int i = 1; i <= 24; i++)
    {
        QString checkBoxName = QString("checkBoxY%1").arg(i);
        QCheckBox* checkBox = this->findChild<QCheckBox*>(checkBoxName);

        if (checkBox)
        {
            connect(checkBox, &QCheckBox::stateChanged, this, &LightEdit::onCheckboxStateChanged);
        }
    }
}

LightEdit::~LightEdit()
{
    delete ui;
}


void LightEdit::onCheckboxStateChanged()
{
    valueX = 0;  // 每次开始前都重置这个值
    if (ui->checkBoxX1->isChecked()) valueX |= (1 << 0);
    if (ui->checkBoxX2->isChecked()) valueX |= (1 << 1);
    if (ui->checkBoxX3->isChecked()) valueX |= (1 << 2);
    if (ui->checkBoxX4->isChecked()) valueX |= (1 << 3);
    if (ui->checkBoxX5->isChecked()) valueX |= (1 << 4);
    if (ui->checkBoxX6->isChecked()) valueX |= (1 << 5);
    if (ui->checkBoxX7->isChecked()) valueX |= (1 << 6);
    if (ui->checkBoxX8->isChecked()) valueX |= (1 << 7);
    if (ui->checkBoxX9->isChecked()) valueX |= (1 << 8);
    if (ui->checkBoxX10->isChecked()) valueX |= (1 << 9);
    if (ui->checkBoxX11->isChecked()) valueX |= (1 << 10);
    if (ui->checkBoxX12->isChecked()) valueX |= (1 << 11);
    if (ui->checkBoxX13->isChecked()) valueX |= (1 << 12);
    if (ui->checkBoxX14->isChecked()) valueX |= (1 << 13);
    if (ui->checkBoxX15->isChecked()) valueX |= (1 << 14);
    if (ui->checkBoxX16->isChecked()) valueX |= (1 << 15);
    if (ui->checkBoxX17->isChecked()) valueX |= (1 << 16);
    if (ui->checkBoxX18->isChecked()) valueX |= (1 << 17);
    if (ui->checkBoxX19->isChecked()) valueX |= (1 << 18);
    if (ui->checkBoxX20->isChecked()) valueX |= (1 << 19);
    if (ui->checkBoxX21->isChecked()) valueX |= (1 << 20);
    if (ui->checkBoxX22->isChecked()) valueX |= (1 << 21);
    if (ui->checkBoxX23->isChecked()) valueX |= (1 << 22);
    if (ui->checkBoxX24->isChecked()) valueX |= (1 << 23);


    valueY = 0;  // 每次开始前都重置这个值
    if (ui->checkBoxY1->isChecked()) valueY |= (1 << 0);
    if (ui->checkBoxY2->isChecked()) valueY |= (1 << 1);
    if (ui->checkBoxY3->isChecked()) valueY |= (1 << 2);
    if (ui->checkBoxY4->isChecked()) valueY |= (1 << 3);
    if (ui->checkBoxY5->isChecked()) valueY |= (1 << 4);
    if (ui->checkBoxY6->isChecked()) valueY |= (1 << 5);
    if (ui->checkBoxY7->isChecked()) valueY |= (1 << 6);
    if (ui->checkBoxY8->isChecked()) valueY |= (1 << 7);
    if (ui->checkBoxY9->isChecked()) valueY |= (1 << 8);
    if (ui->checkBoxY10->isChecked()) valueY |= (1 << 9);
    if (ui->checkBoxY11->isChecked()) valueY |= (1 << 10);
    if (ui->checkBoxY12->isChecked()) valueY |= (1 << 11);
    if (ui->checkBoxY13->isChecked()) valueY |= (1 << 12);
    if (ui->checkBoxY14->isChecked()) valueY |= (1 << 13);
    if (ui->checkBoxY15->isChecked()) valueY |= (1 << 14);
    if (ui->checkBoxY16->isChecked()) valueY |= (1 << 15);
    if (ui->checkBoxY17->isChecked()) valueY |= (1 << 16);
    if (ui->checkBoxY18->isChecked()) valueY |= (1 << 17);
    if (ui->checkBoxY19->isChecked()) valueY |= (1 << 18);
    if (ui->checkBoxY20->isChecked()) valueY |= (1 << 19);
    if (ui->checkBoxY21->isChecked()) valueY |= (1 << 20);
    if (ui->checkBoxY22->isChecked()) valueY |= (1 << 21);
    if (ui->checkBoxY23->isChecked()) valueY |= (1 << 22);
    if (ui->checkBoxY24->isChecked()) valueY |= (1 << 23);


//===============================X===================================
    if (ui->checkBoxX1->isChecked())
    {
        ui->labelX1->setStyleSheet("background-color: lightblue;");
        ui->labelX1->raise();
    } else
    {
        ui->labelX1->setStyleSheet("background-color: lightgray;");
        //ui->labelX1->lower();
    }

    if (ui->checkBoxX2->isChecked())
    {
        ui->labelX2->setStyleSheet("background-color: lightblue;");
        ui->labelX2->raise();
    } else
    {
        ui->labelX2->setStyleSheet("background-color: lightgray;");
        //ui->labelX2->lower();
    }

    if (ui->checkBoxX3->isChecked())
    {
        ui->labelX3->setStyleSheet("background-color: lightblue;");
        ui->labelX3->raise();
    } else
    {
        ui->labelX3->setStyleSheet("background-color: lightgray;");
       // ui->labelY1->lower();
    }

    if (ui->checkBoxX4->isChecked())
    {
        ui->labelX4->setStyleSheet("background-color: lightblue;");
        ui->labelX4->raise();
    } else
    {
        ui->labelX4->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }

    if (ui->checkBoxX5->isChecked())
    {
        ui->labelX5->setStyleSheet("background-color: lightblue;");
        ui->labelX5->raise();
    } else
    {
        ui->labelX5->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX6->isChecked())
    {
        ui->labelX6->setStyleSheet("background-color: lightblue;");
        ui->labelX6->raise();
    } else
    {
        ui->labelX6->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX7->isChecked())
    {
        ui->labelX7->setStyleSheet("background-color: lightblue;");
        ui->labelX7->raise();
    } else
    {
        ui->labelX7->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX8->isChecked())
    {
        ui->labelX8->setStyleSheet("background-color: lightblue;");
        ui->labelX8->raise();
    } else
    {
        ui->labelX8->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX9->isChecked())
    {
        ui->labelX9->setStyleSheet("background-color: lightblue;");
        ui->labelX9->raise();
    } else
    {
        ui->labelX9->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX10->isChecked())
    {
        ui->labelX10->setStyleSheet("background-color: lightblue;");
        ui->labelX10->raise();
    } else
    {
        ui->labelX10->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX11->isChecked())
    {
        ui->labelX11->setStyleSheet("background-color: lightblue;");
        ui->labelX11->raise();
    } else
    {
        ui->labelX11->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX12->isChecked())
    {
        ui->labelX12->setStyleSheet("background-color: lightblue;");
        ui->labelX12->raise();
    } else
    {
        ui->labelX12->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX13->isChecked())
    {
        ui->labelX13->setStyleSheet("background-color: lightblue;");
        ui->labelX13->raise();
    } else
    {
        ui->labelX13->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX14->isChecked())
    {
        ui->labelX14->setStyleSheet("background-color: lightblue;");
        ui->labelX14->raise();
    } else
    {
        ui->labelX14->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX15->isChecked())
    {
        ui->labelX15->setStyleSheet("background-color: lightblue;");
        ui->labelX15->raise();
    } else
    {
        ui->labelX15->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX16->isChecked())
    {
        ui->labelX16->setStyleSheet("background-color: lightblue;");
        ui->labelX16->raise();
    } else
    {
        ui->labelX16->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX17->isChecked())
    {
        ui->labelX17->setStyleSheet("background-color: lightblue;");
        ui->labelX17->raise();
    } else
    {
        ui->labelX17->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX18->isChecked())
    {
        ui->labelX18->setStyleSheet("background-color: lightblue;");
        ui->labelX18->raise();
    } else
    {
        ui->labelX18->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX19->isChecked())
    {
        ui->labelX19->setStyleSheet("background-color: lightblue;");
        ui->labelX19->raise();
    } else
    {
        ui->labelX19->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX20->isChecked())
    {
        ui->labelX20->setStyleSheet("background-color: lightblue;");
        ui->labelX20->raise();
    } else
    {
        ui->labelX20->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX21->isChecked())
    {
        ui->labelX21->setStyleSheet("background-color: lightblue;");
        ui->labelX21->raise();
    } else
    {
        ui->labelX21->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX22->isChecked())
    {
        ui->labelX22->setStyleSheet("background-color: lightblue;");
        ui->labelX22->raise();
    } else
    {
        ui->labelX22->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX23->isChecked())
    {
        ui->labelX23->setStyleSheet("background-color: lightblue;");
        ui->labelX23->raise();
    } else
    {
        ui->labelX23->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxX24->isChecked())
    {
        ui->labelX24->setStyleSheet("background-color: lightblue;");
        ui->labelX24->raise();
    } else
    {
        ui->labelX24->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
//============================Y===========================================

    if (ui->checkBoxY1->isChecked())
    {
        ui->labelY1->setStyleSheet("background-color: lightblue;");
        ui->labelY1->raise();
    } else
    {
        ui->labelY1->setStyleSheet("background-color: lightgray;");
        //ui->labelX1->lower();
    }

    if (ui->checkBoxY2->isChecked())
    {
        ui->labelY2->setStyleSheet("background-color: lightblue;");
        ui->labelY2->raise();
    } else
    {
        ui->labelY2->setStyleSheet("background-color: lightgray;");
        //ui->labelX2->lower();
    }

    if (ui->checkBoxY3->isChecked())
    {
        ui->labelY3->setStyleSheet("background-color: lightblue;");
        ui->labelY3->raise();
    } else
    {
        ui->labelY3->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }

    if (ui->checkBoxY4->isChecked())
    {
        ui->labelY4->setStyleSheet("background-color: lightblue;");
        ui->labelY4->raise();
    } else
    {
        ui->labelY4->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }

    if (ui->checkBoxY5->isChecked())
    {
        ui->labelY5->setStyleSheet("background-color: lightblue;");
        ui->labelY5->raise();
    } else
    {
        ui->labelY5->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY6->isChecked())
    {
        ui->labelY6->setStyleSheet("background-color: lightblue;");
        ui->labelY6->raise();
    } else
    {
        ui->labelY6->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY7->isChecked())
    {
        ui->labelY7->setStyleSheet("background-color: lightblue;");
        ui->labelY7->raise();
    } else
    {
        ui->labelY7->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY8->isChecked())
    {
        ui->labelY8->setStyleSheet("background-color: lightblue;");
        ui->labelY8->raise();
    } else
    {
        ui->labelY8->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY9->isChecked())
    {
        ui->labelY9->setStyleSheet("background-color: lightblue;");
        ui->labelY9->raise();
    } else
    {
        ui->labelY9->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY10->isChecked())
    {
        ui->labelY10->setStyleSheet("background-color: lightblue;");
        ui->labelY10->raise();
    } else
    {
        ui->labelY10->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY11->isChecked())
    {
        ui->labelY11->setStyleSheet("background-color: lightblue;");
        ui->labelY11->raise();
    } else
    {
        ui->labelY11->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY12->isChecked())
    {
        ui->labelY12->setStyleSheet("background-color: lightblue;");
        ui->labelY12->raise();
    } else
    {
        ui->labelY12->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY13->isChecked())
    {
        ui->labelY13->setStyleSheet("background-color: lightblue;");
        ui->labelY13->raise();
    } else
    {
        ui->labelY13->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY14->isChecked())
    {
        ui->labelY14->setStyleSheet("background-color: lightblue;");
        ui->labelY14->raise();
    } else
    {
        ui->labelY14->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY15->isChecked())
    {
        ui->labelY15->setStyleSheet("background-color: lightblue;");
        ui->labelY15->raise();
    } else
    {
        ui->labelY15->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY16->isChecked())
    {
        ui->labelY16->setStyleSheet("background-color: lightblue;");
        ui->labelY16->raise();
    } else
    {
        ui->labelY16->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY17->isChecked())
    {
        ui->labelY17->setStyleSheet("background-color: lightblue;");
        ui->labelY17->raise();
    } else
    {
        ui->labelY17->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY18->isChecked())
    {
        ui->labelY18->setStyleSheet("background-color: lightblue;");
        ui->labelY18->raise();
    } else
    {
        ui->labelY18->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY19->isChecked())
    {
        ui->labelY19->setStyleSheet("background-color: lightblue;");
        ui->labelY19->raise();
    } else
    {
        ui->labelY19->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY20->isChecked())
    {
        ui->labelY20->setStyleSheet("background-color: lightblue;");
        ui->labelY20->raise();
    } else
    {
        ui->labelY20->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY21->isChecked())
    {
        ui->labelY21->setStyleSheet("background-color: lightblue;");
        ui->labelY21->raise();
    } else
    {
        ui->labelY21->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY22->isChecked())
    {
        ui->labelY22->setStyleSheet("background-color: lightblue;");
        ui->labelY22->raise();
    } else
    {
        ui->labelY22->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY23->isChecked())
    {
        ui->labelY23->setStyleSheet("background-color: lightblue;");
        ui->labelY23->raise();
    } else
    {
        ui->labelY23->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }
    if (ui->checkBoxY24->isChecked())
    {
        ui->labelY24->setStyleSheet("background-color: lightblue;");
        ui->labelY24->raise();
    } else
    {
        ui->labelY24->setStyleSheet("background-color: lightgray;");
        // ui->labelY1->lower();
    }



    qDebug() << "VALUE X:"<<valueX; //10
    //qDebug() << "VALUE X:" << QString::number(valueX, 16);//16
    qDebug() << "VALUE Y:"<<valueY;
}
