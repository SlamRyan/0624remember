#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include "insglobalvar.h"
#include <QDebug>
#include <QMessageBox>
#include <typeinfo>
#include <cstring>
#include "ins_camlightpose.h"
#include "ins_spaceadjust.h"
#include "lightedit.h"
#include "ins_titlebar.h"
#include "QFileDialog"
#include <QScrollBar>
#include <qmetatype.h>
#include <QtConcurrent/QtConcurrentRun>
#include <qinputdialog.h>


//#include "inscp_udp.h"//在ins_devicechoose.h中包含
//#include "modbus_udp.h"
//#include "Ins_Utility_proc.h"//这里可行

#include <QDir>

#include <iostream>

//#include <functional>

#include <vector>
#include <fstream>
//#include"ins_encodeverify.h"
using namespace Ins2D5Camera;//0103

#pragma execution_character_set("utf-8")                                //解决中文乱码

//===============================1009===============================
const uint32_t SYSTEM_BASE_ADDRESS = 0x00000000;
const std::vector<uint32_t> SYSTEM_OFFSETS = {0, 4, 8, 0xC, 0x10, 0x14, 0x18, 0x1C ,0x20, 0x24};

uint8_t insupdatafilepBuf[10000*1024];
int PACKET_SIZE = 512;//1222在线升级

int TimeOutCount = 0;//超时次数


//ins_encodeVerify insencode;

//MainWindow* MainWindow::mpMainWindow =nullptr;


bool WriteToRegister(uint32_t address, uint32_t value)
{
    uint16_t highValue = (value >> 16) & 0xFFFF;
    uint16_t lowValue = value & 0xFFFF;

    uint16_t buffer[2] = {highValue, lowValue};

    //写入
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(address, buffer);//West
    return true;
}


bool ReadFromRegister(uint32_t address,uint32_t &valueOut)
{

    uint16_t buffer[2];
    //读取
    //if(!Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(address, buffer))//West
    //{
    //   return false;//读取失败
    //}

    valueOut = ((uint32_t)buffer[0]<<16) | buffer[1];
    return true;

}

void BackupRegisters(const std::string& filename)
{
    std::ofstream file(filename);
    for (auto offset : SYSTEM_OFFSETS)
    {
        uint32_t registerAddress = SYSTEM_BASE_ADDRESS + offset;
        uint32_t value;

        if (!ReadFromRegister(registerAddress,value))
        {
            std::cout << "读取地址 " << std::hex << registerAddress << " 失败!" << std::endl;
            continue;  // 如果读取失败，则跳过此地址
        }
        file << std::hex << registerAddress << " " << value << "\n";
    }
    file.close();
}

void RestoreRegisters(const std::string& filename)
{
    std::ifstream file(filename);
    uint32_t address, value;
    while (file >> std::hex >> address >> value)
    {
        WriteToRegister(address, value);
    }
    file.close();
}

uint32_t readFromModbusOne(uint16_t addr )
{
    //0315 删除
    return 0;
}



void writeToModbusFive(uint16_t regAdd,uint32_t value)//1206写入FiveReg
{
    uint16_t highValue = (value >> 16) & 0xFFFF;
    uint16_t lowValue = value & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,regAdd,buffer);//1
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,regAdd,buffer);//2
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,regAdd,buffer);//3
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD,regAdd,buffer);//4
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD,regAdd,buffer);//5
}

void writeToModbusThree(uint16_t regAdd,uint32_t value)//1206写入ThreeReg
{
    uint16_t highValue = (value >> 16) & 0xFFFF;
    uint16_t lowValue = value & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,regAdd,buffer);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,regAdd,buffer);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,regAdd,buffer);
}


void writeToModbusOne(uint16_t regAdd,uint32_t value)//1206写入OneReg
{
    uint16_t highValue = (value >> 16) & 0xFFFF;
    uint16_t lowValue = value & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,regAdd,buffer);

}





//================================1009end===========================

//0807
QPixmap cvMatToQPixmap(const cv::Mat &mat)//cv::Mat转化为Qpixmap
{
    // Convert the cv::Mat to QImage
    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);

    // Convert the QImage to QPixmap
    QPixmap pixmap = QPixmap::fromImage(img);

    return pixmap;
}

ViewMode currentViewMode = SingleView;



MainWindow* MainWindow::mpMainWindow = nullptr;//wdj

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    imageProcessorThread(new QThread)//0529
{
    ui->setupUi(this);

    StopProcess = false;

    //qRegisterMetaType<unsigned char*>("unsigned char*");
    qRegisterMetaType<std::string>("std::string");
    imageProcessor = new ImageProcessor();

    //-------------------------------0529
    imageProcessor->moveToThread(imageProcessorThread);
    imageProcessorThread->start();
    //connect(this, &MainWindow::startImageProcessing, imageProcessor, &ImageProcessor::processImages);
    connect(imageProcessor, &ImageProcessor::imagesProcessed, this, &MainWindow::updateImages,Qt::QueuedConnection);
    connect(imageProcessorThread, &QThread::finished, imageProcessor, &QObject::deleteLater);

    bool success = connect(this, &MainWindow::startImageProcessing, imageProcessor, &ImageProcessor::processImages, Qt::QueuedConnection);
    if (success)
    {
        qDebug() << "Connection  not failed!";
    }
    else
    {
        qDebug() << "Connection  is failed!";
    }
 
    //0603--------------------
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabIndexChange);//0603
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::onPushButton_11_clicked);

    connect(ui->checkBox_Ori, &QCheckBox::stateChanged, this, &MainWindow::onCheckBox_Ori_changed);
    connect(ui->checkBox_Ori_2, &QCheckBox::stateChanged, this, &MainWindow::onCheckBox_Ori_2_changed);

    //imageProcessorThread->start();
    qDebug() << "Thread is running: " << imageProcessorThread->isRunning();
//==========================sdk test=================================
    mpMainWindow = this;//wdj
    InsPMDL_CreateCameraHandle(&mpCamera,1);//0305

    WindowCount = 1;

    HelloCamera();
    m_upgrad = new UpDateThread(); //新建线程
            //thread->start();

    connect(m_upgrad,SIGNAL(sig_sendState(QString,bool)),this,SLOT(addToStateMessage(QString,bool)));//1212线程

    upTimer = new QTimer();

    connect(upTimer,SIGNAL(timeout()),this,SLOT(slot_sendPercent()));

    InsGlobalVar::m_insUpgrad.baseAddr = 0x00800000; //起始地址bbb
    InsGlobalVar::m_insUpgrad.currentPage = 0;
    InsGlobalVar::m_insUpgrad.totalPage = 0;
    InsGlobalVar::m_insUpgrad.pageSize = 512;
    InsGlobalVar::m_insUpgrad.inUpdate = 0;


    PicNumCount = 8;//默认8
    ui->checkBox_IR->setEnabled(false);

    //图标初始化--------------------------
    QPixmap pixmapRed(":/image/icon/Red20.png");
    ui->label_12->setPixmap(pixmapRed);

    //0304
    ui->connectBtn->setIcon(QIcon(":/image/icon/LEDOFF.png"));
    ui->disConnectBtn->setIcon(QIcon(":/image/icon/LEDON.png"));

    ui->startAcqBtn->setIcon(QIcon(":/image/icon/start.png"));
    ui->stopAcqBtn->setIcon(QIcon(":/image/icon/stop.png"));

    QIcon iconnn(":/image/icon/ins.ico");
    setWindowIcon(iconnn);






    //------------------------------------------
    ui->lineEdit_6->setToolTip("行频 = 1000000/输入值");
    std::cout<<"========upup==0907==upup=========="<<std::endl;

    //DataProcessCallBack func = std::bind(&MainWindow::slot_updateBuffer,this);
    // camcam.set_data_callback(std::bind(&MainWindow::slot_updateBuffer,this,std::placeholders::_1));



//=========================sdk test-end ============================

    /*imageCheckBoxGroup = new QButtonGroup(this); //只能选择一次
    imageCheckBoxGroup->addButton(ui->checkBox_X1);
    imageCheckBoxGroup->addButton(ui->checkBox_X2);
    imageCheckBoxGroup->addButton(ui->checkBox_X3);
    imageCheckBoxGroup->addButton(ui->checkBox_X4);
    imageCheckBoxGroup->addButton(ui->checkBox_Y1);
    imageCheckBoxGroup->addButton(ui->checkBox_Y2);
    imageCheckBoxGroup->addButton(ui->checkBox_Y3);
    imageCheckBoxGroup->addButton(ui->checkBox_Y4);
    imageCheckBoxGroup->addButton(ui->checkBox_Spe);
    imageCheckBoxGroup->addButton(ui->checkBox_Dif);
    imageCheckBoxGroup->addButton(ui->checkBox_SP1);
    imageCheckBoxGroup->addButton(ui->checkBox_SP2);
    //imageCheckBoxGroup->addButton(ui->checkBox_Ori);*/
    //imageCheckBoxGroup->setExclusive(true);



    int type1=qRegisterMetaType<QModbusDevice::State>("QModbusDevice::State");
    int type2=qRegisterMetaType<QModbusDevice::Error>("QModbusDevice::Error");
    //int type3=qRegisterMetaType<QModbusDataUnit::RegisterType>("QModbusDataUnit::RegisterType");
    int type4=qRegisterMetaType<QVector<quint16>>("QVector<quint16>");
    int type5=qRegisterMetaType<unsigned char *>("unsigned char *");
    //int type6=qRegisterMetaType<InsCam>("InsCam");
    m_connect = false;
    InsGlobalVar::insMain = this;
    timerModbusRead=new QTimer(this);
    connect(timerModbusRead,SIGNAL(timeout()),this,SLOT(modbusPeriodRead()),Qt::UniqueConnection);

    m_modbusClient = new ModbusCommit();
    connect(m_modbusClient, SIGNAL(modbus_state_change_signal(QModbusDevice::State)),  this, SLOT(modbus_state_change_slot(QModbusDevice::State)));
    connect(m_modbusClient, SIGNAL(modbus_error_occurred_signal(QModbusDevice::Error)),  this, SLOT(modbus_error_occurred_slot(QModbusDevice::Error)));
    connect(m_modbusClient, SIGNAL(modbus_read_over_signal(QModbusDataUnit::RegisterType,bool,QVector<quint16>)),  this, SLOT(modbus_read_over_slot(QModbusDataUnit::RegisterType,bool,QVector<quint16>)));

    ui->mimage->setMouseTracking(true);
    ui->mimage->installEventFilter(this);

    //ui->mimage_2->setMouseTracking(true);//0815
    //ui->mimage_2->installEventFilter(this);

    m_Image = new Ins_Widget();
    m_Image2 = new Ins_Widget();

    m_RefreshX1 = false;//0828

    ui->zuobiaoX1->setReadOnly(true);
    ui->zuobiaoY1->setReadOnly(true);
    ui->gray1->setReadOnly(true);//只读




    /*//采集初始化
    ui->startAcqBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->startAcqBtn->setIconSize(QSize(16, 16));  // 设置图标为16x16大小
    ui->startAcqBtn->setIcon(QIcon("./icon/start.png"));
    ui->startAcqBtn->setText("开始采集");*/



    //0816 noon start

      for (int i = 0; i < 12; i++)
    {
        m_Images[i] = new Ins_Widget();
        qgraphicsScenes[i] = new QGraphicsScene;
        qgraphicsScenes[i]->addItem(m_Images[i]);
    }
    ui->mimage->setScene(qgraphicsScenes[0]);
    ui->mimage_2->setScene(qgraphicsScenes[0]);
    ui->mimage_3->setScene(qgraphicsScenes[1]);
    ui->mimage_4->setScene(qgraphicsScenes[0]);
    ui->mimage_5->setScene(qgraphicsScenes[1]);
    ui->mimage_6->setScene(qgraphicsScenes[2]);
    ui->mimage_7->setScene(qgraphicsScenes[3]);
    ui->mimage_9->setScene(qgraphicsScenes[0]);
    ui->mimage_10->setScene(qgraphicsScenes[1]);
    ui->mimage_11->setScene(qgraphicsScenes[2]);
    ui->mimage_12->setScene(qgraphicsScenes[3]);
    ui->mimage_13->setScene(qgraphicsScenes[4]);
    ui->mimage_14->setScene(qgraphicsScenes[5]);
    ui->mimage_15->setScene(qgraphicsScenes[6]);
    ui->mimage_16->setScene(qgraphicsScenes[7]);
    ui->mimage_17->setScene(qgraphicsScenes[8]);
    ui->mimage_18->setScene(qgraphicsScenes[9]);
    ui->mimage_19->setScene(qgraphicsScenes[10]);
    ui->mimage_20->setScene(qgraphicsScenes[11]);

    //ui->mimage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//1204在UI里打开
    //ui->mimage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_5->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_6->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_6->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_7->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_7->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_9->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_9->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_10->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_10->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_11->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_11->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_12->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_12->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_13->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_13->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_14->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_14->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_15->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_15->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_16->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_16->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_17->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_17->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_18->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_18->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_19->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_19->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->mimage_20->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mimage_20->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(ui->mimage, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);
    connect(ui->mimage_2, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);
    connect(ui->mimage_3, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);

    connect(ui->mimage_4, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);
    connect(ui->mimage_5, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);
    connect(ui->mimage_6, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);
    connect(ui->mimage_7, &MyGraphicsView::rightClicked, this, &MainWindow::ReturnToCenter);

    //为其QGraphicsView设置场景

    //0816 noon end

    //0807logostart
    //QGraphicsScene *scene = new QGraphicsScene;//QGraphicsScene 对象
    //scene->addItem(m_Image);
    ui->mimage->setScene(qgraphicsScenes[0]);

    qDebug()<<"0824 A";
    qDebug()<<"0828 B";

    QPixmap initialPixmap(":/image/icon/logowhite.png"); 
    if (!initialPixmap.isNull())
    {
        qDebug()<<"0824B";
        m_Images[0]->setImgPixmap(&initialPixmap);//setImgPixmap，用给定的QPixmap对象设置m_pix
    }
    //0807logoend


    //qgraphicsScene = new QGraphicsScene;//要用QGraphicsView就必须要有QGraphicsScene搭配着用
    //qgraphicsScene->addItem(m_Image);//将QGraphicsItem类对象放进QGraphicsScene中
    //ui->mimage->setScene(qgraphicsScene);



    info_timer = new QTimer();
    connect(info_timer,SIGNAL(timeout()),this,SLOT(slot_infoUpdate()));
    img.create(InsGlobalVar::m_Height,InsGlobalVar::m_Width,CV_8UC1);//1109

    connect(this,SIGNAL(sig_updateBuffer(unsigned char*)),this,SLOT(slot_updateBuffer(unsigned char*)));
    connect(this,SIGNAL(sig_InitWindow(InsCam)),this,SLOT(slot_InitWindow(InsCam)));
    //connect(ui->actionCommunication,&QAction::triggered, this, &MainWindow::slot_Comm);//1115
    //connect(ui->actionDeviceInfo,&QAction::triggered, this, &MainWindow::slot_DeviceInfo);
    //connect(ui->actionLightEditor,&QAction::triggered, this, &MainWindow::slot_LightEdit);

    

    ui->insInterface_9->setReadOnly(true);

    connect(ui->action_2, &QAction::triggered, [=]()
    {
        ui->tabWidget->setCurrentIndex(0);
    }
            );
    connect(ui->action_3, &QAction::triggered, [=]()
    {
        ui->tabWidget->setCurrentIndex(1);
    }
            );
    connect(ui->action_4, &QAction::triggered, [=]()
    {
        ui->tabWidget->setCurrentIndex(2);
    }
            );
    connect(ui->action_5, &QAction::triggered, [=]()
    {
        ui->tabWidget->setCurrentIndex(3);
    }
            );
    connect(ui->action_6, &QAction::triggered, [=]()
    {
        ui->tabWidget->setCurrentIndex(4);
    }
            );

    connect(ui->ModbusClientBtn_9, &QPushButton::clicked, this, &MainWindow::processAndDisplayImage);//0807
    //connect(ui->ModbusClientBtn_14, &QPushButton::clicked, this, &MainWindow::showImageX1);//显示X1


    ui->tab_3->setEnabled(false);//默认不可用 1206



    //优化前connect
    /*connect(ui->checkBox_X1, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_X1);
    connect(ui->checkBox_X2, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_X2);
    connect(ui->checkBox_X3, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_X3);
    connect(ui->checkBox_X4, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_X4);
    connect(ui->checkBox_Y1, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Y1);
    connect(ui->checkBox_Y2, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Y2);
    connect(ui->checkBox_Y3, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Y3);
    connect(ui->checkBox_Y4, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Y4);
    connect(ui->checkBox_Spe, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Spe);
    connect(ui->checkBox_Dif, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Dif);
    connect(ui->checkBox_SP1, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_SP1);
    connect(ui->checkBox_SP2, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_SP2);*/
    //connect(ui->checkBox_Ori, &QCheckBox::stateChanged, this, &MainWindow::on_checkBox_Ori);



    connect(ui->LightBtn, SIGNAL(clicked()), this, SLOT(on_LightBtn()));//LightTest
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_btnOffsetMethod()));//多画面


    //优化后connect
    connect(ui->checkBox_X1, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_X2, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_X3, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_X4, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Y1, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Y2, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Y3, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Y4, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Spe, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_SP1, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_SP2, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Dif, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxChanged);
    connect(ui->checkBox_Gro,&QCheckBox::stateChanged,this,&MainWindow::onCheckBoxChanged);

    connect(ui->checkBox_IR,&QCheckBox::stateChanged,this,&MainWindow::onCheckBoxChanged);//1115

    connect(ui->checkBox_ABconverse, &QCheckBox::stateChanged, this, &MainWindow::ABcheckboxConverse);//0321 ABconverse

    connect(ui->LanguageBtn, SIGNAL(clicked()), this, SLOT(LanChange()));//0326 LanTest
    connect(ui->LanguageBtn_2, SIGNAL(clicked()), this, SLOT(LanChange2()));//0326 LanTest




    //
    //connect(ui->LightMode, SIGNAL(currentIndexChanged(const QString &)),this, SLOT(on_comboBox_currentIndexChanged(const QString &)));//lightmode



    connect(ui->LightMode, SIGNAL(currentIndexChanged()), this, SLOT(on_LightMode_currentIndexChanged()));//加int打印两次

    connect(ui->TriggerFre,SIGNAL(currentIndexChanged()),this,SLOT(on_TriggerFre_currentIndexChanged()));

    //connect(ui->LightBtn,SIGNAL(clicked()),this,SLOT(on_applyButton_clicked()));//btn light


    connect(ui->lineEdit_6, &QLineEdit::returnPressed, this, &MainWindow::on_FreLine_returnPressed);
    connect(ui->comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxChanged(int)));

    connect(ui->ClearBtn,SIGNAL(clicked()),this,SLOT(clearAllGraphicsViews()));//btn light


    //connect(ui->PoseBtn,SIGNAL(clicked()),this,SLOT(PoseBtn_clicked()));//0817 pose
    connect(ui->PoseBtn, &QPushButton::clicked, this, &MainWindow::PoseBtn_clicked);


    connect(ui->LightBtn, &QPushButton::clicked, this, &MainWindow::RegionBtn_clicked);//0818 Region

    connect(m_Images[0], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY);
    connect(m_Images[0], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY2);
    connect(m_Images[1], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY2);

    connect(m_Images[0], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY3);
    connect(m_Images[1], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY3);
    connect(m_Images[2], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY3);
    connect(m_Images[3], &Ins_Widget::sig_pixelView, this, &MainWindow::showXY3);



    //connect(this, &MainWindow::imageProcessed, this, &MainWindow::showImage);//0828


    //connect(this,&MainWindow::imageProcessedFinish,this,&MainWindow::onCheckBoxChanged);

    connect(ui->LightBtn2, &QPushButton::clicked, this, &MainWindow::PoseBtn2_clicked);//0829

    connect(ui->ModbusClientBtn_8, &QPushButton::clicked, this, &MainWindow::ImageValue_clicked);//0904

    connect(ui->BtndevIp, &QPushButton::clicked, this, &MainWindow::DevIp_clicked);//0918 IP West//333

    //======1124
    connect(ui->action_7, SIGNAL(triggered()), this, SLOT(saveConfigure()));
    connect(ui->action_8, SIGNAL(triggered()), this, SLOT(loadConfigure()));
    connect(ui->action, SIGNAL(triggered()), this, SLOT(saveConfigureFlash()));
    

    connect(ui->actionChinese, SIGNAL(triggered()), this, SLOT(LanChange2()));
    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(LanChange()));


    //0407
    connect(ui->insInterface_10, SIGNAL(returnPressed()), this, SLOT(SetNormalGain()));
    connect(ui->insInterface_12, SIGNAL(returnPressed()), this, SLOT(SetSpecularGain()));
    connect(ui->insInterface_11, SIGNAL(returnPressed()), this, SLOT(SetDiffuseGain()));
    connect(ui->insInterface_13, SIGNAL(returnPressed()), this, SLOT(SetShapeGain()));


    connect(ui->insName_4, SIGNAL(changeEvent()), this, SLOT(on_insName_4_textChanged()));
   

    
    //insencode = new ins_encodeVerify();//1013  //0407   0412 改到下面去了，上面会报错
    


    //==============0905ceshi==


    timer9 = new QTimer(this);
    timer11 = new QTimer(this);
    connect(timer9, &QTimer::timeout, this, &MainWindow::ReadFpgaTem);//10101 //FPGA温度
    //connect(timer9, &QTimer::timeout, this, &MainWindow::ReadBoardTem);//10101 //主板温度，改到FPGA函数里
    //connect(timer9, &QTimer::timeout, this, &MainWindow::ReadPose);//位姿刷新定时器need改  //相机光源位姿
    connect(timer11, &QTimer::timeout, this, &MainWindow::ABencodePose);//编码器位姿


    //timer9->start(3000);  // 3S


    timer10 = new QTimer(this);
    //connect(timer10, &QTimer::timeout, this, &MainWindow::ABencodePose);//位姿刷新定时器need改

    connect(timer10, &QTimer::timeout, this, &MainWindow::ReadPose2);//1122
    //timer10->start(500);



    //==============================================================================
    //connect(timer9, &QTimer::timeout, this, &MainWindow::updateImages);
    connect(this,SIGNAL(imageProcessedFinish()),this,SLOT(updateImages()));//发信号

    //direction = new ins_DirectionChoose(this);//0531移到下面
    //connect(direction, SIGNAL(chooseDirection1(int)), this, SLOT(DirectionChange(int)));//0314
    InsSystemHandle = nullptr;

    ui->LightMode_12->setCurrentIndex(2);//二级IO

    ui->insName_5->setEnabled(false);//1204设备名称只读
    ui->insName->setEnabled(false);
    ui->insInterface->setEnabled(false);
    ui->insInterface_4->setEnabled(false);
    ui->insInterface_7->setEnabled(false);

   // ui->widget->setEnabled(false);
    ui->tab_5->setEnabled(false);
    ui->tab_2->setEnabled(false);
    //ui->tab_3->setEnabled(false);
    ui->tab_4->setEnabled(false);//0621test注掉
    //ui->tab->setEnabled(false);//1226

    ui->startAcqBtn->setEnabled(false);
    ui->stopAcqBtn->setEnabled(false);




    //===============0905ceshiend===


    for (int i = 0;i<12;++i)
    {
        m_TextItems[i] = nullptr;
    }

    new_titlebar_init();



    InitWindow();

    //on_ModbusClientBtn_clicked();//0823

    SecJug = false;



}

MainWindow::~MainWindow()
{

    imageProcessorThread->quit();
    imageProcessorThread->wait();
    delete ui;
}



//0905shuaxintuxiang========================


//====0905ceshi demo

void MainWindow::simulateUpdateBuffer()
{

    //emit imageProcessedFinish();
}


//===============

void MainWindow::updateImages()//刷新// //0919needchange //1018绝对->相对
{
    QString appDir = QCoreApplication::applicationDirPath();//1018添加

    for (QCheckBox *checkBox : checkedCheckBoxes)
    {
        int viewIndex = checkedCheckBoxes.indexOf(checkBox);

        if (checkBox == ui->checkBox_X1)
        {
            showImage(appDir + "/image/X1.bmp", "X1图像", viewIndex);//1018
            //showImage("C:/JFSoft/25d-sdk/1.Ryan5d-code - sdk/image/X1.bmp","X1图像", viewIndex);//1018注
        }
        else if (checkBox == ui->checkBox_X2)
        {
            showImage(appDir + "/image/X2.bmp","X2图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_X3)
        {
            showImage(appDir + "/image/X3.bmp", "X3图像",viewIndex);
        }
        else if (checkBox == ui->checkBox_X4)
        {
            showImage(appDir + "/image/X4.bmp","X4图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_Y1)
        {
            showImage(appDir + "/image/Y1.bmp","Y1图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_Y2)
        {
            showImage(appDir + "/image/Y2.bmp","Y2图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_Y3)
        {
            showImage(appDir + "/image/Y3.bmp","Y3图像", viewIndex);
        }

        else if (checkBox == ui->checkBox_Y4)
        {
            showImage(appDir + "/image/Y4.bmp","Y4图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_Spe)
        {
            showImage(appDir + "/image/Specular.bmp","正反射图像", viewIndex);
        }
        else if (checkBox == ui->checkBox_Dif)
        {
            showImage(appDir + "/image/Diffuse.bmp","漫反射图像", viewIndex);//Diffuse
        }
        else if (checkBox == ui->checkBox_SP1)
        {
            showImage(appDir + "/image/Shape.bmp","形状图像", viewIndex);//Shape
        }
        else if (checkBox == ui->checkBox_SP2)
        {
            showImage(appDir + "/image/Glossratio.bmp", "光泽比图像",viewIndex);//Glossratio
        }
        else if (checkBox == ui->checkBox_Gro)
        {
            showImage(appDir + "/image/Normal.bmp", "标准图像",viewIndex);//Normal
        }
        else if (checkBox == ui->checkBox_IR)
        {
            showImage(appDir + "/image/IR.bmp", "红外图像",viewIndex);//IR
        }

    }
}




//0905shuaxintuxiang==end======================

//================0815copy==================================================


/*Data_25D MainWindow::copyImageData(uint8_t* CamImage, uint32_t l_image_show_width, uint32_t l_image_show_height)
{
    Data_25D t_Img;
    t_Img.data = new uint8_t[l_image_show_width * l_image_show_height];//width * height
    t_Img.length = l_image_show_width;   //设置宽度
    t_Img.height = l_image_show_height;  // 设置高度

    for(uint32_t t_row = 0; t_row < l_image_show_height; t_row++)
    {
        for(uint32_t t_col = 0; t_col < l_image_show_width; t_col++)
        {
            uint32_t index = t_row * l_image_show_width + t_col; // 根据行和列计算索引
            t_Img.data[index] = CamImage[index];
        }
    }

    return t_Img;
}*/



Data_25D MainWindow::copyImageData(uint8_t* CamImage, uint32_t l_image_show_width, uint32_t l_image_show_height)
{
    Data_25D t_Img;

    uint32_t total_size = l_image_show_width * l_image_show_height;  // 总的数据大小
    t_Img.data = new uint8_t[total_size];//记得释放掉0822
    t_Img.length = l_image_show_width;
    t_Img.height = l_image_show_height;

    std::memcpy(t_Img.data, CamImage, total_size);

    return t_Img;
}


//================0815copyend===============================================





//0904imagevalue=====================================================

void MainWindow::updateValues(double normal, double specular, double diffuse, double shape)
{
    normalGain = normal;
    specualrGain = specular;
    diffuseGain = diffuse;
    shapeGain = shape;

}
//------------0407 imagevalue update--------------------------

void MainWindow::SetNormalGain()
{
    normalGain = ui->insInterface_10->text().toDouble();

}

void MainWindow::SetSpecularGain()
{
    specualrGain = ui->insInterface_12->text().toDouble();
}

void MainWindow::SetDiffuseGain()
{
    diffuseGain = ui->insInterface_11->text().toDouble();
}
void MainWindow::SetShapeGain()
{
    shapeGain = ui->insInterface_13->text().toDouble();
}




void MainWindow::ImageValue_clicked()//0904
{

    ins_imagevalue *imagevalueDialog = new ins_imagevalue(this);
    connect(imagevalueDialog, &ins_imagevalue::valuesUpdated, this, &MainWindow::updateValues);
    imagevalueDialog->setWindowTitle("图像个别设定");  // 设置窗口的标题
    connect(imagevalueDialog, &ins_imagevalue::finished, imagevalueDialog, &ins_imagevalue::deleteLater);
    imagevalueDialog->show();

}

void MainWindow::DevIp_clicked()//0918 //333
{
    ins_devicechoose *devIpDialog = new ins_devicechoose(this);
    devIpDialog->get_mpcamera(this->mpCamera);
    devIpDialog->setWindowTitle("扫描/连接光源设备");  // 设置窗口的标题
    connect(devIpDialog, &ins_devicechoose::finished, devIpDialog, &ins_devicechoose::deleteLater);
    devIpDialog->show();
}


//0904imagevalue-end=====================================================

//====================0821坐标灰度显示===========================

void MainWindow::showXY(double x ,double y,int grayvalue)

{


    ui->zuobiaoX1->setText(QString::number(x, 'f', 0));
    ui->zuobiaoY1->setText(QString::number(y, 'f', 0));
    ui->gray1->setText(QString::number(grayvalue, 'f', 0));

}

void MainWindow::showXY2(double x, double y,int grayvalue)
{
    ui->zuobiaoX2->setText(QString::number(x, 'f', 0));
    ui->zuobiaoY2->setText(QString::number(y, 'f', 0));
    ui->gray2->setText(QString::number(grayvalue, 'f', 0));

}

void MainWindow::showXY3(double x, double y,int grayvalue)
{
    ui->zuobiaoX3->setText(QString::number(x, 'f', 0));
    ui->zuobiaoY3->setText(QString::number(y, 'f', 0));
    ui->gray3->setText(QString::number(grayvalue, 'f', 0));

}




//==============================================================



//==================Region0818 Start=============================
void MainWindow::RegionBtn_clicked()//0818 行间距显示窗口
{
    //Ins_CamLightPose pose(this);
    // 从陀螺仪获取数据并更新到dialog的label中
    //double angle = getGyroAngle(); //获取陀螺仪的角度值
    //gyroDialog.setAngleValue(angle);
    //pose.show(); // 使用模态方式显示子窗口

    ins_spaceAdjust *region = new ins_spaceAdjust(this);
    region->setWindowTitle("行间距调整");  // 设置窗口的标题
    connect(region, &ins_spaceAdjust::finished, region, &ins_spaceAdjust::deleteLater);
    region->show();

}

void MainWindow::ReadPose2()
{

    read_internl_pirch(deviceHandles.lightHandle,&LightPosePirch);//1121 CamLightPose

    read_internl_roll(deviceHandles.lightHandle, &LightPoseRoll);//deviceHandles.lightHandle

    read_internl_yaw(deviceHandles.lightHandle, &LightPoseYaw);

    /*
    LightPose2Pirch = LightPosePirch *10;
    LightPose2Roll = LightPoseRoll *10;
    LightPose2Yaw = LightPoseYaw *10;


    LightPosePirch = LightPose2Pirch;
    LightPosePirch /= -10;

    LightPoseRoll = LightPose2Roll;
    LightPoseRoll /= -10;

    LightPoseYaw = LightPose2Yaw;
    LightPoseYaw /= -10;
    */

    //相机 0401 等有了打开
    //read_extern_pirch(deviceHandles.lightHandle, &CamPosePirch);//相机有了打开

    //read_extern_roll(deviceHandles.lightHandle, &CamPoseRoll);

    //read_extern_yaw(deviceHandles.lightHandle, &CamPoseYaw);

/*
    CamPose2Pirch = CamPosePirch *10;
    CamPose2Roll = CamPoseRoll *10;
    CamPose2Yaw = CamPoseYaw *10;


    CamPosePirch = CamPose2Pirch;
    CamPosePirch /= -10;

    CamPoseRoll = CamPose2Roll;
    CamPoseRoll /= -10;

    CamPoseYaw = CamPose2Yaw;
    CamPoseYaw /= -10;

*/

}

//0817posestart============================================================

void MainWindow::PoseBtn_clicked()//0817 //101010
{


    timer10->start(500);

    Ins_CamLightPose *pose = new Ins_CamLightPose(this);
    //pose->setWindowTitle("相机/光源位姿");  // 设置窗口的标题
    pose->setWindowTitle("Camera/Light Pose");  // 设置窗口的标题
    connect(pose, &Ins_CamLightPose::finished, pose, &Ins_CamLightPose::deleteLater);
    pose->show();



}


//===========0817poseEnd




//=================0829自编辑=======================

void MainWindow::PoseBtn2_clicked()//0829
{

    LightEdit *SelfEdit = new LightEdit(this);
    SelfEdit->setWindowTitle("条纹自编辑");  // 设置窗口的标题
    connect(SelfEdit, &LightEdit::finished, SelfEdit, &LightEdit::deleteLater);
    SelfEdit->show();

}
//================自编辑end==========================

//==0812===lightmodestart===================================================

/*void MainWindow::on_comboBox_currentIndexChanged(const QString &selectedMode)
{
    //qDebug()<<"08120";
    qDebug() << "Selected mode text:" << selectedMode;
    //QString selectedMode = ui->LightMode->currentText();
    uint16_t modeValue = getLEDModeValue(selectedMode.toStdString().c_str());
    // 然后根据需要使用modeValue
    qDebug()<<modeValue;
}*/

void MainWindow::on_LightMode_currentIndexChanged(int index)//发光模式
{
    InsPMDL_LineMode lightmode;
    if (index ==0)
    {
        lightmode = Max;
    }
    else if (index == 1)
    {
        lightmode = Big;

    }
    else if (index == 2)
    {
        lightmode = Standard;

    }
    else if (index == 3)
    {
        lightmode = Small;

    }
    else if (index == 4)
    {
        lightmode = Min;

    }
    InsPMDL_SetEmissionMode(InsSystemHandle, lightmode);//0305 0319e
    
}


uint16_t getTriggerFre(int modeIndex)
{
    switch (modeIndex)
    {
    case 0:
        return 12500;//10k //0011 0000 1101 0100
    case 1:
        return 6250;//20k //0001 1000 0110 1010
    case 2:
        return 4166;//30k //0001 0000 0100 0110
    default:
        std::cout << "No matching mode found, defaulting ." << std::endl;
        return 32767;//20k                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
    }
}

void MainWindow::on_TriggerFre_currentIndexChanged(int index)//触发频率

{
    uint32_t TriFre = getTriggerFre(index);
    uint16_t highTriFre = (TriFre >> 16) & 0xFFFF;//high
    uint16_t lowTriFre = TriFre & 0xFFFF;//low
    uint16_t buffer[2] = {highTriFre,lowTriFre};
    qDebug() << "Trigger Fre value:" << TriFre;

    if(index==0)//可以用 高低改之前
    {
        qDebug() << "Trigger Fre value:" << TriFre;
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x2008,buffer);//West
        qDebug()<<"write FreValue to 0x2008";
        qDebug()<<buffer[0];
        qDebug()<<buffer[1];
    }

    //========index=1=========
    else if(index ==1)//2
    {
        qDebug() << "Trigger Fre value:" << TriFre;
        qDebug() << "Trigger Fre value:" << TriFre;
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x2008,buffer);//West
        qDebug()<<"write FreValue to 0x2008";
        qDebug()<<buffer[0];
        qDebug()<<buffer[1];
    }
    
    //===========index=2=================
    else if(index ==2)
    {
        qDebug() << "Trigger Fre value:" << TriFre;
        qDebug() << "Trigger Fre value:" << TriFre;
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x2008,buffer);//West
        qDebug()<<"write FreValue to 0x2008";
        qDebug()<<buffer[0];
        qDebug()<<buffer[1];   
    }
}





/*void MainWindow::on_FreLine_Changed()

{
    uint16_t Fre = ui->lineEdit_6->text();
    if (Fre == NULL)
    {16
        // 如果输入框为空，则显示一个警告消息
        QMessageBox::warning(this, "Warning", "请输入触发频率)");
    }
    else
    {
    customTiggerFre(Fre);
    }
}*/ //判断是否为空 暂时不用


//=========================================old== Fre Line=======================================
/*
void MainWindow::on_FreLine_returnPressed()
{

    uint32_t SWTime;
    bool ok;
    uint16_t Fre = ui->lineEdit_6->text().toDouble(&ok);

    if (ok && Fre >= 4 && Fre <= 200)
    {
        // 输入成功
        //QMessageBox::information(this, tr("Success"), tr("输入成功."));//DDD
        addToStateMessage("行扫描周期-输入成功", false);

        uint32_t MyExp = (Fre*1000)-2000;
        uint32_t MyFre = MyExp/8;//10110
        uint32_t valueToWrite = MyFre;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;
        uint16_t buffer[2] = {highValue,lowValue};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x2008,buffer);//West
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1004,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1004,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1004,buffer);
        //uint16_t customFre = customTriggerFre(Fre);
        std::cout<<"Fre: "<<MyFre <<std::endl;


        uint32_t TriGap = 250 ;//触发间隔
        uint16_t TriGapH = (TriGap >> 16) & 0xFFFF;
        uint16_t TriGapL = TriGap & 0xFFFF;
        uint16_t buffer2[2] = {TriGapH,TriGapL};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x200C,buffer2);//West
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1006,buffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1006,buffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1006,buffer2 );


        if(Fre>=101 && Fre<=200)
        {
            SWTime = 336 ;//开关时间
            uint32_t SWtimeMy = (SWTime/8)-6;
            uint16_t SWTimeMyH = (SWtimeMy >> 16) & 0xFFFF;
            uint16_t SWTimeMyL = SWtimeMy & 0xFFFF;
            uint16_t buffer3[2] = {SWTimeMyH,SWTimeMyL};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer3);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer3);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer3);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer3 );


        }

        else if(Fre>=51 && Fre<=100)
        {
            SWTime = 288 ;
            uint32_t SWtimeMy2 = (SWTime/8)-6;
            uint16_t SWTimeMyH2 = (SWtimeMy2 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL2 = SWtimeMy2 & 0xFFFF;
            uint16_t buffer4[2] = {SWTimeMyH2,SWTimeMyL2};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer4);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer4);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer4);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer4);

        }
        else if(Fre>=21 && Fre<=50)
        {
            SWTime = 312 ;
            uint32_t SWtimeMy3 = (SWTime/8)-6;
            uint16_t SWTimeMyH3 = (SWtimeMy3 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL3 = SWtimeMy3 & 0xFFFF;
            uint16_t buffer5[2] = {SWTimeMyH3,SWTimeMyL3};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer5);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer5);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer5);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer5);

        }
        else if(Fre>=11 && Fre<=20)
        {
            SWTime = 296 ;
            uint32_t SWtimeMy4 = (SWTime/8)-6;
            uint16_t SWTimeMyH4= (SWtimeMy4 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL4 = SWtimeMy4 & 0xFFFF;
            uint16_t buffer6[2] = {SWTimeMyH4,SWTimeMyL4};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer6);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer6);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer6);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer6);

        }

        else if(Fre>=9 && Fre<=10)
        {
            SWTime = 488 ;
            uint32_t SWtimeMy5 = (SWTime/8)-6;
            uint16_t SWTimeMyH5= (SWtimeMy5 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL5 = SWtimeMy5 & 0xFFFF;
            uint16_t buffer7[2] = {SWTimeMyH5,SWTimeMyL5};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer7);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer7);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer7);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer7);

        }

        else if(Fre>=7 && Fre<=8)
        {
            SWTime = 344;
            uint32_t SWtimeMy6 = (SWTime/8)-6;
            uint16_t SWTimeMyH6= (SWtimeMy6 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL6 = SWtimeMy6 & 0xFFFF;
            uint16_t buffer8[2] = {SWTimeMyH6,SWTimeMyL6};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer8);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer8);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer8);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer8);

        }

        else if(Fre>=4 && Fre<=6)
        {
            SWTime = 256;
            uint32_t SWtimeMy7 = (SWTime/8)-6;
            uint16_t SWTimeMyH7= (SWtimeMy7 >> 16) & 0xFFFF;
            uint16_t SWTimeMyL7 = SWtimeMy7 & 0xFFFF;
            uint16_t buffer9[2] = {SWTimeMyH7,SWTimeMyL7};
            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x110C,buffer9);//West
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0886,buffer9);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0886,buffer9);
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0886,buffer9);

        }


        uint32_t SWGap = 2;
        uint16_t SWGapH= (SWGap >> 16) & 0xFFFF;
        uint16_t SWGapL= SWGap & 0xFFFF;
        uint16_t buffer10[2] = {SWGapH,SWGapL};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x1108,buffer10);//West
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0884,buffer10);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0884,buffer10);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0884,buffer10);

        uint32_t LoopGap = 9;
        uint16_t LoopGapH= (LoopGap >> 16) & 0xFFFF;
        uint16_t LoopGapL= LoopGap & 0xFFFF;
        uint16_t buffer11[2] = {LoopGapH,LoopGapL};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x201C,buffer11);//West
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x100E,buffer11);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x100E,buffer11);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x100E,buffer11);



        uint32_t LoopNum= MyExp/((SWTime*12)+(72*12));// 2&9一起改
        uint16_t LoopNumH= (LoopNum >> 16) & 0xFFFF;
        uint16_t LoopNumL = LoopNum & 0xFFFF;
        uint16_t buffer12[2] = {LoopNumH,LoopNumL};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x2020,buffer12);//West
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1010,buffer12);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1010,buffer12);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1010,buffer12);


        if(LoopNum < 1)
        {
            LoopNum = 1;
        }
        qDebug()<<"LoopNum"<<LoopNum<<endl;

        camcam.set_ins_line_period_time(Fre);




    }
    else
    {
        // 输入失败
        //qDebug() << "Input failed. The maximum speed setting is 100.";
       // QMessageBox::warning(this, tr("Failure"), tr("输入失败，自定义范围（4~200）."));
        addToStateMessage("输入失败，输入范围（4~100）", true);

    }


    //10111



    double Fre2  = ui->lineEdit_6->text().toDouble();

    if(Fre > myexposure+2)
    {
        double LineFre= 1000000.0/Fre2;

        cout<<"doubleLineFre<<"<<LineFre<<endl;

        QString str = QString::number(LineFre, 'f', 2);

        ui->insInterface_9->setText(str);
    }
    else
    {
        addToStateMessage("输入失败,行扫描周期应大于曝光时间）", true);
    }


}
*/
//======================================================================================


void MainWindow::on_FreLine_returnPressed()//行频 0319a
{


//1205Z===================================================================行频2
    
    bool ok ;
    double CameraTriggerCycle = ui->lineEdit_6->text().toDouble(&ok);//1205亮度提升
    
    //double dLineFre = 0;
    //double dExp = 0;
    //------------------------------Calculate16k-------------------------------------

    //------------------------------Caluculate8k-------------------------------------



    InsResult ret = InsPMDL_SetScanningPeriod(InsSystemHandle,CameraTriggerCycle, dLineFre, dExp);//0409
    if (ret == InsResult::INSCSTATUS_OK|| ret == InsResult::INSSTATUS_SUCCESS)
    {
        addToStateMessage("输入成功", false);
        addToStateMessage("曝光自动设置成功", false);
    }
    else
    {
        addToStateMessage("输入失败，输入范围（36~800）", true);
    }



    //========================显示============================
    //QString temp = QString::number(dLineFre);
    QString temp = QString::number(dLineFre, 'f', 2);
    if (dLineFre != 0)
    {
        cout << "dLineFre111:" << dLineFre << endl;
        qDebug() << "Temp:1111:" << temp << endl;

        ui->insInterface_9->setText(temp);//行频
        ui->insInterface_5->setText(QString::number(dExp));//曝光
    }
    else
    {

        cout << "dLineFre:"<<dLineFre << endl;
        addToStateMessage("扫描周期显示失败", true);
    }
  
}


//void MainWindow::on_applyButton_clicked()
//{
//    qDebug()<<"here0812btn";
//    QString selectedMode = ui->LightMode->currentText();
//    uint16_t modeValue = getLEDModeValue(selectedMode.toStdString().c_str());
//    qDebug()<<modeValue;
    // 然后根据需要使用modeValue
//}
//0808=====================duochuangkou=====================================




//=========================Image==============================================
//0807processAndDisplay
void MainWindow::processAndDisplayImage()//1018gai //离线仿真
{

   // void* toolPtr;

   // QString appDir = QCoreApplication::applicationDirPath();

   // std::string appDirCv = appDir.toStdString();

   // cv::Mat src= cv::imread(appDirCv + "/image/a.bmp", 0);//

   // //cv::Mat src= cv::imread("./image/a.bmp", 0);



   // // 开始抽帧，图像内存在函数内部申请，需在使用后调用 DestroyImageArray进行内存释放
   // InsImage srcImage[9];
   // ImageExtarction(src.data, src.cols, src.rows,src.cols, 9 ,srcImage);//src改为指针 //9张

   // // 合成，图像内存在函数内部申请，需在使用后调用 DestroyImageArray进行内存释放
   // // 默认四张图顺序：Normal/Specular/Diffuse/Shape

   // CreateLumitraxToolPtr(toolPtr);

   // SetLumitraxToolParam(toolPtr,normalGain,specualrGain,diffuseGain,false,shapeGain,0);

   // InsImage dstImage[5];

   // CreateLumitrxImage(toolPtr,srcImage,dstImage);

   // //CreateLumitrxImage(srcImage, 1.0, 1.0, 1.0, 1.0,1.0,dstImage);

   // // 显示及保存图片
   // cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1 , srcImage[0].data, srcImage[0].stride);
   // cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1 , srcImage[1].data, srcImage[1].stride);
   // cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1 , srcImage[2].data, srcImage[2].stride);
   // cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1 , srcImage[3].data, srcImage[3].stride);
   // cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1 , srcImage[4].data, srcImage[4].stride);
   // cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1 , srcImage[5].data, srcImage[5].stride);
   // cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1 , srcImage[6].data, srcImage[6].stride);
   // cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1 , srcImage[7].data, srcImage[7].stride);
   // cv::Mat IR = cv::Mat(srcImage[8].height, srcImage[8].width, CV_8UC1 , srcImage[8].data, srcImage[8].stride);

   // cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1 , dstImage[0].data, dstImage[0].stride);
   // cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1 , dstImage[1].data, dstImage[1].stride);
   // cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1 , dstImage[2].data, dstImage[2].stride);
   // cv::Mat Glossratio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1 , dstImage[3].data, dstImage[3].stride);
   // cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1 , dstImage[4].data, dstImage[4].stride);
   ///*
   // cv::imwrite("./image/X1.bmp", X1);
   // cv::imwrite("./image/X2.bmp", X2);
   // cv::imwrite("./image/X3.bmp", X3);
   // cv::imwrite("./image/X4.bmp", X4);
   // cv::imwrite("./image/Y1.bmp", Y1);
   // cv::imwrite("./image/Y2.bmp", Y2);
   // cv::imwrite("./image/Y3.bmp", Y3);
   // cv::imwrite("./image/Y4.bmp", Y4);
   // cv::imwrite("./image/Normal.bmp", Normal);
   // cv::imwrite("./image/Specular.bmp", Specular);
   // cv::imwrite("./image/Diffuse.bmp", Diffuse);
   // cv::imwrite("./image/Shape.bmp", Shape);
   // cv::imwrite("./image/Glossratio.bmp", Glossratio);
   // */

   // cv::imwrite(appDirCv + "/image/X1.bmp", X1);//0919 need change//1018changeaabbb
   // cv::imwrite(appDirCv + "/image/X2.bmp", X2);
   // cv::imwrite(appDirCv + "/image/X3.bmp", X3);
   // cv::imwrite(appDirCv + "/image/X4.bmp", X4);
   // cv::imwrite(appDirCv + "/image/Y1.bmp", Y1);
   // cv::imwrite(appDirCv + "/image/Y2.bmp", Y2);
   // cv::imwrite(appDirCv + "/image/Y3.bmp", Y3);
   // cv::imwrite(appDirCv + "/image/Y4.bmp", Y4);
   // cv::imwrite(appDirCv + "/image/Normal.bmp", Normal);
   // cv::imwrite(appDirCv + "/image/Specular.bmp", Specular);
   // cv::imwrite(appDirCv + "/image/Diffuse.bmp", Diffuse);
   // cv::imwrite(appDirCv + "/image/Shape.bmp", Shape);
   // cv::imwrite(appDirCv + "/image/Glossratio.bmp", Glossratio);//1018改路径
   // cv::imwrite(appDirCv + "/image/IR.bmp", IR);//1115


   // // 释放内存
   // DestroyImageArray(srcImage, 9);
   // DestroyImageArray(dstImage, 5);

   // DestroyLumitraxToolPtr(toolPtr);
}

// QImage 转 cv::Mat
cv::Mat QImageToCvMat(const QImage& image)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    case QImage::Format_RGB32:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());
        break;
    default:
        qWarning() << "QImage format not supported for conversion to cv::Mat";
        break;
    }
    return mat.clone();
}

// cv::Mat 转 QImage
QImage CvMatToQImage(const cv::Mat& mat)
{
    QImage::Format format = QImage::Format_Invalid;
    switch (mat.type())
    {
    case CV_8UC4:
        format = QImage::Format_ARGB32;
        break;
    case CV_8UC3:
        format = QImage::Format_RGB888;
        break;
    case CV_8UC1:
        format = QImage::Format_Indexed8;
        break;
    default:
        qWarning() << "cv::Mat type not supported for conversion to QImage";
        break;
    }

    QImage image(mat.data, mat.cols, mat.rows, mat.step, format);
    return image.copy();
}


//----------------------------------------------0531yuan showimage------------------------------------------
void MainWindow::showImage(const QString &filePath, const QString &text,int viewIndex)//show0816//0828//0921
{

    //cout<<"[showImage] called"<<endl;

    if (viewIndex < 0 || viewIndex >= 12)//有效检查
    {
        return;
    }


    
    QImage image(filePath);

    qDebug() << "Image format at input: " << image.format();


    if(ui->comboBox_3->currentIndex() == 0)
    {

        if (PixWindow == 1)
        {
            if (DisplayOriImage == false)
            {
                image = image.scaled(ui->mimage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);//abcd//1113   0531  //0603ui->mimage->size()
                           //image = image.scaled(ui->mimage->size(), Qt::KeepAspectRatio, Qt::FastTransformation);//abcd//1113   0531
            }
          
            
        }

    }
    else if(ui->comboBox_3->currentIndex() == 1)
    {
        if (DisplayOriImage == false)
        {
            image = image.scaled(ui->mimage_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    

    }
    else if(ui->comboBox_3->currentIndex() == 2)
    {
        if (DisplayOriImage == false)
        {
            image = image.scaled(ui->mimage_4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    
    }
    else if(ui->comboBox_3->currentIndex() == 3)
    {
        if (DisplayOriImage == false)
        {
            image = image.scaled(ui->mimage_9->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
    
    }

    //------------------ 平滑滤波处理
    
    if (DisplayOriImage != false)
    {
        if (AntiAliasing == true)
        {
            cv::Mat matImage = QImageToCvMat(image);

            cv::Mat matFiltered;

            cv::Mat smallImage, largeImage;
            cv::resize(matImage, smallImage, cv::Size(), 0.6, 0.6, cv::INTER_LINEAR); // 下采样//INTER_AREA
            cv::Mat filteredSmallImage;
            cv::bilateralFilter(smallImage, filteredSmallImage, 3, 75, 75);
            cv::resize(filteredSmallImage, matFiltered, matImage.size(), 0, 0, cv::INTER_LINEAR); // 放大到原始大小


        //   
        //    
        //    // 选择滤波方法
        //    //cv::GaussianBlur(matImage, matFiltered, cv::Size(5, 5), 0); // 高斯滤波
        //    //cv::bilateralFilter(matImage, matFiltered, 25, 100, 100); // 双边滤波
        //    //cv::medianBlur(matImage, matFiltered, 3); // 中值滤波
        //    //cv::fastNlMeansDenoisingColored(matImage, matFiltered, 10, 10, 7, 21); // 非局部均值滤波

        //    //cv::resize(matImage, matFiltered, matImage.size(), 0, 0, cv::INTER_LANCZOS4);

            image = CvMatToQImage(matFiltered);
        }
    }


    //-----------------


    QPixmap pixmap = QPixmap::fromImage(image);

  
    m_Images[viewIndex]->setImgPixmap(&pixmap);


    QGraphicsScene* scene = m_Images[viewIndex]->scene();//0425 图像居中
    if (scene)
    {
        //QRectF viewRect = scene->views().at(0)->viewport()->rect(); // 获取视图的矩形
        //QPointF centerPos = viewRect.center() - QPointF(pixmap.width() / 2, pixmap.height() / 2);
        m_Images[viewIndex]->setPos(0,0); // 设置图像居中
       
    }

    //=================
    //QGraphicsScene* scene = m_Images[viewIndex]->scene();
    if (scene)
    {
    // 如果已经有一个QGraphicsTextItem，先从场景中移除
    if (m_TextItems[viewIndex])
    {
            scene->removeItem(m_TextItems[viewIndex]);
            delete m_TextItems[viewIndex];
    }

    // 创建新的QGraphicsTextItem并添加到场景中
    m_TextItems[viewIndex] = new QGraphicsTextItem(text);

    // 根据 comboBox 的当前索引设置不同的位置
    if(ui->comboBox_3->currentIndex() == 0)
    {
            m_TextItems[viewIndex]->setPos(-210, -270);//PPose
    }
    else if(ui->comboBox_3->currentIndex() == 1)
    {
            m_TextItems[viewIndex]->setPos(-160, -210);
    }
    else if(ui->comboBox_3->currentIndex() == 2)
    {
            m_TextItems[viewIndex]->setPos(-110, -135);
    }
    else if(ui->comboBox_3->currentIndex() == 3)
    {
            m_TextItems[viewIndex]->setPos(0, 0);
    }

    // 改变文本的字体和大小
    QFont font;
    font.setPointSize(10);  // 设置字体大小为16
    m_TextItems[viewIndex]->setFont(font);

    // 改变文本颜色
    m_TextItems[viewIndex]->setDefaultTextColor(Qt::yellow);
    scene->addItem(m_TextItems[viewIndex]);
    }

    //=================
    //image = image.scaled(ui->mimage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
   //ui->mimage->viewport()->update();
}
//----------------------------------------------0531yuan showimage-end -----------------------------------------

//==========================0531-----------------------------




//==========================0531-----------------------------
// 
// 
// 
//==================== 优化0815end===================================



//======================comboboxReset0816====================================


void MainWindow::onComboBoxChanged(int index)//切换取消勾选
{
    Q_UNUSED(index);  //消除未使用警告

    cout<<"checkbox reset"<<endl;

    // 找到group11里的控件
    QList<QCheckBox*> checkboxes = ui->groupBox_11->findChildren<QCheckBox*>();

    for (QCheckBox* checkbox : checkboxes)
    {
        checkbox->setChecked(false);
    }
}

//=========================================================================
void MainWindow::clearImage()//clear
{
    QPixmap empty;
    m_Image->setImgPixmap(&empty);
    ui->mimage->viewport()->update();
}

/*void MainWindow::clearAllGraphicsViews()//待修改clear
{

    QPixmap empty;

    if (ui->comboBox_3->currentIndex() == 0)
    {
    empty.scaled(ui->mimage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    empty.fill(Qt::transparent);  //透明
    }

    if (ui->comboBox_3->currentIndex() == 1)
    {
    empty.scaled(ui->mimage_2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    empty.fill(Qt::transparent);
    }

    if (ui->comboBox_3->currentIndex() == 2)
    {
    empty.scaled(ui->mimage_4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    empty.fill(Qt::transparent);
    }

    if (ui->comboBox_3->currentIndex() == 3)
    {
    empty.scaled(ui->mimage_9->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    empty.fill(Qt::transparent);
    }
    for(int i=0;i<12;++i)
    {
        m_Images[i]->setImgPixmap(&empty);
    }

}*/


void MainWindow::clearAllGraphicsViews()//0423
{


    QPixmap empty(CamHeightClear/8, CamWidthClear);  // 创建一个500x500的空白图像
    qDebug() << "CamHeightClear is :" << CamHeightClear <<endl;
    qDebug() << "CamWidthClear is :" << CamWidthClear << endl;

    empty.fill(Qt::transparent);  // 将其填充为透明

    for(int i = 0; i < 12; ++i)
    {
        m_Images[i]->setImgPixmap(&empty);
    }
}

//==========================clearend================================



int MainWindow::countCheckedCheckBoxes()//选择了几个checkbox
{
    int count = 0;
    if (ui->checkBox_X1->isChecked()) count++;
    if (ui->checkBox_X2->isChecked()) count++;
    if (ui->checkBox_X3->isChecked()) count++;
    if (ui->checkBox_X4->isChecked()) count++;
    if (ui->checkBox_Y1->isChecked()) count++;
    if (ui->checkBox_Y2->isChecked()) count++;
    if (ui->checkBox_Y3->isChecked()) count++;
    if (ui->checkBox_Y4->isChecked()) count++;
    if (ui->checkBox_Spe->isChecked()) count++;
    if (ui->checkBox_SP1->isChecked()) count++;
    if (ui->checkBox_SP2->isChecked()) count++;
    if (ui->checkBox_Dif->isChecked()) count++;
    if (ui->checkBox_Gro->isChecked()) count++;
    if (ui->checkBox_IR->isChecked()) count++;

    return count;
}
//===================选择优化 0815start==========================================

void MainWindow::onCheckBoxChanged(int state)
{
    QString appDir = QCoreApplication::applicationDirPath();//1018添加
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    qDebug()<<"[onCheckBoxChanged] 1";
    if (state == Qt::Checked)
    {
        checkedCheckBoxes.enqueue(checkBox); // 将选中的checkbox添加到队列的尾部 //入队

        // 判断当前模式允许的最大勾选数量
        int maxChecked = 0;
        switch (currentViewMode)
        {
        case SingleView: maxChecked = 1; break;
        case DoubleView: maxChecked = 2; break;
        case QuadView: maxChecked = 4; break;
        case AllView: maxChecked = 12; break;
        }

        // 如果超过了最大数量，从队列的头部取出一个checkbox并设置为未勾选状态
        while (checkedCheckBoxes.count() > maxChecked)
        {
            QCheckBox* toUncheck = checkedCheckBoxes.dequeue(); //出队
            toUncheck->setChecked(false);
        }

        clearImage();

        // 根据队列中checkbox的索引
        int viewIndex = checkedCheckBoxes.indexOf(checkBox);//决定在哪个窗口显示

        cout<<"viewIndex is :"<<viewIndex<<endl;

        if (checkBox == ui->checkBox_X1)//                 添加或 || 0828 // onCheckBoxChanged更新
        {
            //showImage("C:/JFSoft/25d-sdk/1.Ryan5d-code - sdk/image/X1.bmp","X1图像", viewIndex);//1018注
            showImage(appDir + "/image/X1.bmp", "X1图像", viewIndex);
            std::cout<<"[mainImageValue->normalValue]:"<<normalGain<<std::endl;//0904
            //m_Images[viewIndex]->resetTransform();//66666
        }
        else if (checkBox == ui->checkBox_X2)
            showImage(appDir + "/image/X2.bmp","X2图像", viewIndex);//改地址
        else if (checkBox == ui->checkBox_X3)
            showImage(appDir + "/image/X3.bmp","X3图像", viewIndex);
        else if (checkBox == ui->checkBox_X4)
            showImage(appDir + "/image/X4.bmp","X4图像", viewIndex);
        else if (checkBox == ui->checkBox_Y1)
            showImage(appDir + "/image/Y1.bmp","Y1图像", viewIndex);
        else if (checkBox == ui->checkBox_Y2)
            //showImage("C:/JFSoft/25d-sdk/1.Ryan5d-code - sdk/image/Y2.bmp","Y2图像",viewIndex);
            showImage(appDir + "/image/Y2.bmp","Y2图像", viewIndex);
        else if (checkBox == ui->checkBox_Y3)
            showImage(appDir + "/image/Y3.bmp","Y3图像", viewIndex);
        else if (checkBox == ui->checkBox_Y4)
            showImage(appDir + "/image/Y4.bmp","Y4图像", viewIndex);
        else if (checkBox == ui->checkBox_Spe)
            showImage(appDir + "/image/Specular.bmp","正反射图像",viewIndex);//www
        else if (checkBox == ui->checkBox_Dif)
            showImage(appDir + "/image/Diffuse.bmp","漫反射图像",viewIndex);//Diffuse
        else if (checkBox == ui->checkBox_SP1)
            showImage(appDir + "/image/Shape.bmp","形状图像",viewIndex);//Shape
        else if (checkBox == ui->checkBox_SP2)
            showImage(appDir + "/image/Glossratio.bmp","光泽比图像",viewIndex);//Glossratio
        else if (checkBox == ui->checkBox_Gro)
            showImage(appDir + "/image/Normal.bmp","标准图像",viewIndex);//Normal
        else if (checkBox == ui->checkBox_IR)
            showImage(appDir + "/image/IR.bmp","红外图像",viewIndex);//IR
    } else {
        // 如果一个checkbox被取消勾选，从队列中移除它
        checkedCheckBoxes.removeAll(checkBox);
    }
}


//===================选择优化 0815end==========================================



void MainWindow::slot_btnOffsetMethod()
{
    //cmbEncoderTrigMode下拉选择
    if (ui->comboBox_3->currentIndex() == 0)
    {
        ui->MainWidget->setCurrentIndex(0);  //控件第一页
        clearAllGraphicsViews();
        currentViewMode = SingleView;
        cout<<currentViewMode<<endl;
        WindowCount = 1;
    }
    else if (ui->comboBox_3->currentIndex() == 1)
    {
        ui->MainWidget->setCurrentIndex(2);//clear0816
        clearAllGraphicsViews();
        currentViewMode = DoubleView;
        WindowCount = 2;

    }
    else if (ui->comboBox_3->currentIndex() == 2)
    {
        ui->MainWidget->setCurrentIndex(3);
        clearAllGraphicsViews();
        currentViewMode = QuadView;
        cout<<currentViewMode<<endl;
        WindowCount = 4;
    }
    else if (ui->comboBox_3->currentIndex() == 3)
    {
        if (WindowsJudge == true)
        {
            ui->MainWidget->setCurrentIndex(4);
            clearAllGraphicsViews();
            currentViewMode = AllView;
            cout << currentViewMode << endl;
            WindowCount = 12;
        }
        else
        {
            ui->comboBox_3->setCurrentIndex(2);
            addToStateMessage("停止采集可选择十二画幅", false);
        }
 
    }
    //resetCheckBoxes();
}

//=========================ImageEnd============

//===============Light======================
void MainWindow:: on_LightBtn()
{
    
    qDebug()<<"hellolight2";

}
//=================LightEnd=================
 void MainWindow::InitWindow()
{
    ui->TriggerWidget->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
    ui->MainWidget->setCurrentIndex(0);
 }

void MainWindow::slot_Comm()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::slot_DeviceInfo()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::slot_LightEdit()
{
    ui->MainWidget->setCurrentIndex(1);
}

void MainWindow::slot_MainPage()
{
    ui->tabWidget->setCurrentIndex(2);
    ui->MainWidget->setCurrentIndex(0);
}

/*
void MainWindow::slot_InitWindow(InsCam cam)
{
    ui->insWidth->setValue(cam.m_width);
    ui->insHeight->setValue(cam.m_height);
    //ui->insWorkMode->setCurrentIndex(cam.m_triggerMode);insWorkMode0831
    //ui->insImageType->setCurrentIndex(cam.m_type);insImageType
    ui->TriggerWidget->setCurrentIndex(cam.m_triggerMode);
    ui->insSTwidth->setValue(InsGlobalVar::m_InsCam.m_stWidth);
    ui->insSTperiod->setValue(InsGlobalVar::m_InsCam.m_stPeriod);
    ui->insSTcount->setValue(InsGlobalVar::m_InsCam.m_stCount);
    ui->insSTdelay->setValue(InsGlobalVar::m_InsCam.m_stDelay);
    //ui->insSTpolarity->setCurrentIndex(InsGlobalVar::m_InsCam.m_stPolarity);
    if(InsGlobalVar::m_InsCam.m_stSyncMode)
        ui->insSTsyncmode->setChecked(true);
    else
        ui->insSTsyncmode->setChecked(false);

    ui->insIOtriggermode->setCurrentIndex(InsGlobalVar::m_InsCam.m_ioTriggerMode);
    ui->insIOpolarity->setCurrentIndex(InsGlobalVar::m_InsCam.m_ioPolarity);
    ui->insIOminwidth->setValue(InsGlobalVar::m_InsCam.m_ioMinWidth);

    ui->insStrobeSource->setCurrentIndex(InsGlobalVar::m_InsCam.m_strobeSource);
    ui->insStrobeMethod->setCurrentIndex(InsGlobalVar::m_InsCam.m_strobeMethod);
    ui->insStrobePolarity->setCurrentIndex(InsGlobalVar::m_InsCam.m_strobePolarity);

    /*InsGlobalVar::m_InsCam.m_etChannel = nValue;
    InsGlobalVar::m_InsCam.m_etMultiFactor = nValue;
    InsGlobalVar::m_InsCam.m_etMinWidth = nValue;
    InsGlobalVar::m_InsCam.m_etValidDirection = nValue;
    InsGlobalVar::m_InsCam.m_etReverseComp = nValue;
    InsGlobalVar::m_InsCam.m_etQFSource = nValue;
    InsGlobalVar::m_InsCam.m_etReverseCompLimit = nValue;
    InsGlobalVar::m_InsCam.m_etCDCType = nValue;
    InsGlobalVar::m_InsCam.m_etCDCWidth = nValue;
}

*/


//0921 正在连接按钮 已删 后续可能会用
//void MainWindow::on_ModbusClientBtn_clicked()
//{
    /*
    //QString m_IP = ui->ModbusIP->text(); //0921ModbusIP删掉
    int m_Port = ui->ModbusRobotPort->value();

    if(!m_connect)
    {
        bool flag = m_modbusClient->modbus_connect(m_IP,m_Port);

        if (!flag)
        {
            addToStateMessage(QObject::tr("连接失败！"),true);
            m_connect = false;
        }
        else
        {//处在连接状态进行断开连接的操作
            addToStateMessage(QObject::tr("连接成功！"),false);
            m_connect = true;
        }
    }
    else
    {
        m_modbusClient->modbus_disconnect();
        addToStateMessage(QObject::tr("断开连接！"),true);
        m_connect = !m_connect;
    }*/
//}

void MainWindow::modbus_error_occurred_slot(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError)
    {
        addToStateMessage(QObject::tr("连接出错\n"),true);
        return;
    }
}

void MainWindow::addToStateMessage(QString str, bool Error)
{
    QString AddStr=QDateTime::currentDateTime().toString("hh:mm:ss ")+str;
    QListWidgetItem *AddItem= new QListWidgetItem(QString(AddStr));
    QSize sizeOfItem=QSize(ui->StateMessage->width()-5,ui->StateMessage->height()/2);//5
    AddItem->setSizeHint(sizeOfItem);  //每次改变Item的高度
    if(Error==1)
    {
        AddItem->setTextColor(QColor(255, 0, 0, 255));
    }
    else
    {
        AddItem->setTextColor(QColor(0, 255, 0, 255));
    }
    ui->StateMessage->insertItem(0,AddItem);
    if(ui->StateMessage->count()>100)
    {
        ui->StateMessage->takeItem(100);
        QListWidgetItem *pItem = ui->StateMessage->item(99);
        ui->StateMessage->removeItemWidget(pItem);
        delete pItem;
    }
    ui->StateMessage->scrollToTop();
}

void MainWindow::on_StateSaveBtn_clicked()
{
    QDateTime now=QDateTime::currentDateTime();
    QString snow=now.toString("yyyy_MM_dd_hh_mm_ss");
    snow="log_"+snow+".txt";
    QFile file(snow);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        addToStateMessage("<095>LOG记录文件创建失败!\n",true);
        return;
    }
    QTextStream out(&file);
    for(int i=0;i<ui->StateMessage->count();i++)
    {
        out<<ui->StateMessage->item(i)->text();//第i行写入文本
    }
    addToStateMessage("<611>LOG记录文件保存成功!\n",false);
    return;
}

void MainWindow::on_StateClearBtn_clicked()
{
    ui->StateMessage->clear();
}

void MainWindow::modbus_state_change_slot(QModbusDevice::State state)
{
    qDebug()<<"state="<<state;

    if (state == QModbusDevice::UnconnectedState)
    {
        //ui->ModbusClientBtn->setIcon(QIcon(":/Res/LEDOFF.png"));
        //ui->ModbusClientBtn->setText("开始连接");

    }
    else if(state == QModbusDevice::ConnectingState)
    {
        //ui->ModbusClientBtn->setIcon(QIcon(":/Res/LEDOFF.png"));
        //ui->ModbusClientBtn->setText("正在连接");
    }
    else if (state == QModbusDevice::ConnectedState)
    {
        //ui->ModbusClientBtn->setIcon(QIcon(":/Res/LEDON.png"));
        //ui->ModbusClientBtn->setText("断开连接");
    }
    else if(state == QModbusDevice::ClosingState)
    {
        //ui->ModbusClientBtn->setIcon(QIcon(":/Res/LEDOFF.png"));
        //ui->ModbusClientBtn->setText("开始连接");
    }

    if(state == QModbusDevice::UnconnectedState)
    {
        timerModbusRead->stop();
    }
    else if(state == QModbusDevice::ConnectingState)
    {
        timerModbusRead->stop();
    }
    else if(state == QModbusDevice::ConnectedState)
    {
        //timerModbusRead->start(100);
    }
}

void MainWindow::modbusPeriodRead()
{
    if(!m_modbusClient->modbusClient)
           return;
   QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters,32,8);
   if(auto *reply = m_modbusClient->modbusClient->sendReadRequest(readUnit,1))
   {
       if(!reply->isFinished())
       {
           connect(reply,&QModbusReply::finished,this,&MainWindow::readReady);
       }
       else
           delete reply;
   }
}

void MainWindow::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if(!reply)
        return;
    if(reply->error() == QModbusDevice::NoError)
    {
        const QModbusDataUnit unit = reply->result();
        for(int i = 0;i<unit.valueCount();i++)
        {
            //qDebug()<<"  value1:"<<unit.value(0)<<"  value2:"<<unit.value(1)<<"  value3:"<<unit.value(2)<<"  value4:"<<unit.value(3)<<"  value5:"<<unit.value(4)<<"  value6:"<<unit.value(5)<<"  value7:"<<unit.value(6)<<"  value8:"<<unit.value(7);
            QString msg = QString("addr: %1 , value: %2\r\n").arg(unit.startAddress()+i).arg(unit.value(i));
            ui->textEdit->append(msg);
        }
    }
    reply->deleteLater();
}

void MainWindow::modbus_read_over_slot(QModbusDataUnit::RegisterType type,bool isErr,QVector<quint16> readData)
{
    qDebug()<<"readdata"<<readData.size()<<readData[0];
    if(readData.empty())
        return;
    for(int i = 0;i<readData.size();i++)
    {
        QString msg = QString("addr: %1 , value: %2\r\n").arg(addr+i).arg(readData[i]);
        ui->textEdit->append(msg);
    }
}

void MainWindow::on_toolButton_clicked()
{
    int index = ui->comboBox->currentIndex();

    addr = ui->doubleSpinBox->value();
    num = ui->spinBox->value();
    switch (index) {
    case 0:
        m_modbusClient->modbus_read_coils(1,addr,num);
        break;

    case 1:
        m_modbusClient->modbus_read_input(1,addr,num);
        break;

    case 2:
        m_modbusClient->modbus_read_holding(1,addr,num);
        break;
    default:
        break;

    }
}

void MainWindow::on_toolButton_2_clicked()
{
    int index = ui->comboBox_2->currentIndex();
    quint16 addr = ui->doubleSpinBox_2->value();
    quint16 num = ui->spinBox_2->value();
    if(m_data.empty())
    {
        addToStateMessage("没有数据写入！",true);
        return;
    }
    switch (index) {
    case 0:
        m_modbusClient->modbus_write_coils(1,addr,num,m_data);
        break;
    case 1:
        m_modbusClient->modbus_write_holding(1,addr,num,m_data);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_clicked()
{
    quint16 value = ui->lineEdit->text().toUInt();
    m_data.push_back(value);
    QString message = QString("当前待写入数据长度:%1\r\n").arg(m_data.size());
    addToStateMessage(message,false);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!m_data.empty())
        m_data.pop_back();
     QString message = QString("当前待写入数据长度:%1\r\n").arg(m_data.size());
    addToStateMessage(message,false);
}

void MainWindow::on_toolButton_3_clicked()
{
    if(!m_data.empty())
        m_data.clear();
}

void MainWindow::on_toolButton_4_clicked()
{
    ui->textEdit->clear();
}


void MainWindow::on_connectBtn_clicked()//abc //1215扫描/连接
{
   
    
    //0305 333
    ins_DeviceConnect* devConnect = new ins_DeviceConnect();//1
    connect(devConnect, &ins_DeviceConnect::handleSelected, this, &MainWindow::handleDeviceSelection);//0307
    connect(devConnect, &ins_DeviceConnect::DeviceInfoIndex, this, &MainWindow::DeviceInfoDisplay);//0307
    //devIpiaDlog->get_deviceHandles.cameraHandle(this->deviceHandles.cameraHandle);
    //devConnect->setWindowTitle("连接光源&相机设备");  // 设置窗口的标题//1
    devConnect->setWindowTitle("Device Connect");
    connect(devConnect, &ins_DeviceConnect::finished, devConnect, &ins_DeviceConnect::deleteLater);//1
    devConnect->exec();//1

  
    
    QString appDir = QCoreApplication::applicationDirPath();
    QDir dir(appDir);


    if (!dir.exists("image"))
    {
        dir.mkpath("image"); //创建
    }

    QDateTime now=QDateTime::currentDateTime();//1128
    QString snow=now.toString("yyyy_MM_dd_hh_mm_ss");
    qDebug()<<"start INSCP"<<snow;





    now=QDateTime::currentDateTime();//1128
    snow=now.toString("yyyy_MM_dd_hh_mm_ss");
    qDebug()<<"end INSCP"<<snow;

    //==========0922end


//===================================================//1128a 只连光源


    //加读取本地200k
    //LoadLocalFile(deviceHandles.cameraHandle);//0201
    if (InsSystemHandle == nullptr)
    {
        QMessageBox::warning(this, "提示", "未选择有效设备");
    }
    else
    {
        InsPMDL_ConfigureBoardWith250K(InsSystemHandle);

        //camcam.LoadLocalFile();//1128a
        //ins_SaveBoardConfigurationToFile(deviceHandles.cameraHandle);保存vlcf

        //camcam.ConfigureFrameGrabber();//1128a
        //InsPMDL_ConfigureFrameGrabber(deviceHandles.cameraHandle);保存ccf




        InsPMDL_SetDataCallback(InsSystemHandle, &MainWindow::update_buffer);
        //=========================bind==========================


        ui->insHeight->setValue(InsPMDL_GetHeight(InsSystemHandle));

        ui->insWidth->setValue(InsPMDL_GetWidth(InsSystemHandle));

        //camcam.get_ins_exposure();
        //double exposure = InsPMDL_GetExposure(deviceHandles.cameraHandle);//InsSystemHandle
        double exposure = InsPMDL_GetExposure(InsSystemHandle);
        cout << "0525GetExposure:" << exposure << endl;
        //double exposure = InsCamera::ins_exposure_;


        //camcam.get_ins_digital_gain();
        double gain = InsPMDL_GetDigitalGain(InsSystemHandle);
        //double gain = InsCamera::ins_digital_gain_;



        //ui->insInterface_3->setText(QString::number(exposure,10,2));//1115曝光
        //ui->insInterface_2->setText(QString::number(gain,10,2));

        CamHeightClear = InsPMDL_GetHeight(InsSystemHandle);

        CamWidthClear = InsPMDL_GetWidth(InsSystemHandle);

        cout << "[camcam.ins_height:]" << InsPMDL_GetHeight(InsSystemHandle) << endl;
        cout << "[camcam.ins_width:]" << InsPMDL_GetWidth(InsSystemHandle) << endl;



        //==========10 08==================================== 1008



        //InsPMDL_FPGAInitial(deviceHandles.lightHandle, LightSignInt/10);//0417




        /*
    //=============================FPGA初始化==================================
        uint32_t IniNum1 = 1;
        uint32_t IniNum0 = 0;

        uint16_t highValue = (IniNum1>> 16) & 0xFFFF;
        uint16_t lowValue = IniNum1 & 0xFFFF;
        uint16_t Inibuffer2[2] = {highValue,lowValue};//1

        uint16_t highValue1 = (IniNum0>> 16) & 0xFFFF;
        uint16_t lowValue1 = IniNum0 & 0xFFFF;
        uint16_t Inibuffer1[2] = {highValue1,lowValue1};//0

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1100,Inibuffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B30,Inibuffer1);

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1100,Inibuffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B30,Inibuffer2);

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1100,Inibuffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B30,Inibuffer2);

        //软触发 1221
        uint32_t IniSoftTriPer  =138;//1us -> 1.104us 0105

        if(LightSignInt == 10)
        {

        writeToModbusOne(0xB00,IniSoftTriPer);//周期
        writeToModbusOne(0x0B02,Zeroz);//次数

        }
        else if(LightSignInt == 30)
        {

        writeToModbusThree(0x0B00,IniSoftTriPer);
        writeToModbusThree(0x0B02,Zeroz);
        }

        else if(LightSignInt == 50)
        {

        writeToModbusFive(0x0B00,IniSoftTriPer);
        writeToModbusFive(0x0B02,Zeroz);
        }

    */

    //=======================================================================

    //==1205=============================================================

     //====================1205==============Light Update=========================

        uint16_t REGMOSACK[2] = {};
        uint16_t REGDDSACK[2] = {};
        uint16_t REGPICNUM[2] = {};


        uint16_t REGSWGAP[2] = {};
        uint16_t REGTRIGGAP[2] = {};
        uint16_t REGSWLOOPNUM[2] = {};
        uint16_t REGCAMTRINUM[2] = {};

        /*

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0880,REGMOSACK);//1100
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0882,REGDDSACK);//1104
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x1008,REGPICNUM);//2010


        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x100E,REGSWGAP);//201c
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x1006,REGTRIGGAP);//200c
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x1010,REGSWLOOPNUM);//2020
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0C0A,REGCAMTRINUM);//1814

        MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) <<16) | REGMOSACK[1];
        DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) <<16) | REGDDSACK[1];
        PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) <<16) | REGPICNUM[1];

        SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) <<16) | REGSWGAP[1];
        TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) <<16) | REGTRIGGAP[1];
        SWLoopNumZ = (static_cast<uint32_t>(REGSWLOOPNUM[0]) <<16) | REGSWLOOPNUM[1];
        CamTriNumZ  =(static_cast<uint32_t>(REGCAMTRINUM[0]) <<16) | REGCAMTRINUM[1];
        */



        //====================================================================

        transbool = 1;

        uint16_t bufferRead4[2] = {};
        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0008, bufferRead4);
        //网络状态
        if (!(bufferRead4[0] == 0))
        {
            ui->insName_11->setText("已连接");
        }


        //===============1016 Initial=========================

            //uint16_t buffer[2]; //发光模式

            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x1002,buffer);

        int LightModeInit = 0;
        InsPMDL_GetEmissionMode(InsSystemHandle, LightModeInit);
        cout << "[LightModeInit]" << LightModeInit << endl;

        ui->LightMode->setCurrentIndex(LightModeInit);

        timer9->start(18000);//0321 //0521 //0522



    //====================================初始化1215 Start============================================

    //==========================光源侧==============================================
        //uint32_t TriModeEncode = readFromModbusOne(0x0B2E);//编码器
        //qDebug()<<"TriModeEncode:"<<TriModeEncode;
        //uint32_t TriModeInternal= readFromModbusOne(0x0B04);//软触发

        //if(TriModeEncode == 1 )//判断是否为编码器


        //ui->stackedWidget->setCurrentIndex(0);


        //===================行程方向============================
       // uint32_t Encode2 = readFromModbusOne(0x0B1A);//1634

        //--------------------------------------------//0522注释掉调试
        uint32_t Encode2 = 0;
        InsPMDL_GetTriggerDirection(InsSystemHandle, Encode2);

        if (Encode2 == 0)//单项行程
        {
            ui->ChuFaCombox->setCurrentIndex(0);
            addToStateMessage("当前行程方向为 单向行程", false);
        }
        else if (Encode2 == 1)//回退跟踪
        {
            ui->ChuFaCombox->setCurrentIndex(1);
            addToStateMessage("当前行程方向为 回退跟踪", false);
        }
        else if (Encode2 == 2)//回退忽略
        {
            ui->ChuFaCombox->setCurrentIndex(2);
            addToStateMessage("当前行程方向为 回退忽略", false);
        }
        else if (Encode2 == 3)//双向扫描
        {
            ui->ChuFaCombox->setCurrentIndex(3);
            addToStateMessage("当前行程方向为 双向扫描", false);
        }
        else
        {
            ui->ChuFaCombox->setCurrentIndex(0);
            addToStateMessage("初始化失败，请重新选择行程方向", true);
        }
        //--------------------------------------------


        //================触发模式================================

        //uint32_t Encode1 = readFromModbusOne(0x0B18);
        //-----------------------------------------------------0522
        uint32_t Encode1 = 0;
        InsPMDL_GetEncoderTriggerMode(InsSystemHandle, Encode1);//0105
        if (Encode1 == 0)//A相上升沿 单向计数
        {
            ui->EnCodeTriComBox->setCurrentIndex(0);
            addToStateMessage("当前触发模式为 A相上升沿 单向计数", false);
        }

        else if (Encode1 == 1)//A相上升沿 双向计数
        {
            ui->EnCodeTriComBox->setCurrentIndex(1);
            addToStateMessage("当前触发模式为 A相上升沿 双向计数", false);
        }
        else if (Encode1 == 2)//A相双边沿 双向计数
        {
            ui->EnCodeTriComBox->setCurrentIndex(2);
            addToStateMessage("当前触发模式为 A相双边沿 双向计数", false);
        }
        else if (Encode1 == 3)//A&B 双向
        {
            ui->EnCodeTriComBox->setCurrentIndex(3);
            addToStateMessage("当前触发模式为 A&B双向", false);
        }
        else
        {
            ui->EnCodeTriComBox->setCurrentIndex(0);
            addToStateMessage("初始化失败 请重新选择触发模式", true);
        }
        //-----------------------------------------------------0522

        //================分倍频============================
        //uint32_t FenPinInitial = readFromModbusOne(0x0B1E);
        //uint32_t BeiPinInitial = readFromModbusOne(0x0B26);
        uint32_t FenPinInitial = 0;//0105
        uint32_t BeiPinInitial = 0;
        InsPMDL_GetFrequencyDivision(InsSystemHandle, FenPinInitial);
        InsPMDL_GetFrequencyMultiplier(InsSystemHandle, BeiPinInitial); //0412

        //ui->FenPinSpinBox->setValue((int)FenPinInitial);//0521
        ui->BeiPinSpinBox->setValue(BeiPinInitial + 1);
        ui->FenPinSpinBox->setValue(FenPinInitial);
        /*
        if (BeiPinInitial != 0)
        {
        ui->BeiPinSpinBox->setValue(BeiPinInitial);
        }
        else
        {
        ui->BeiPinSpinBox->setValue(1);
        }*/
        /*
        uint32_t HangSMFre = readFromModbusOne(0x1022);

        uint32_t HangSMFre2 = HangSMFre * CamTriNumZ;

        HangSMFre2 = HangSMFre2*8/1000;//系统扫描周期
        */
        double _line_scanning_period = 0;//0105
        double _line_fre = 0;
        InsPMDL_GetScanningPeriod(InsSystemHandle, _line_scanning_period, _line_fre);
        ui->lineEdit_6->setText(QString::number(_line_scanning_period,'f',2));//0603


        /*
        double Fre2  = static_cast<double>(HangSMFre2);

        double LineFre= 1000000.0/Fre2/1000;*/

        QString str = QString::number(_line_fre, 'f', 2);

        ui->insInterface_9->setText(str);

        //========================发光模式==============================

        int IniLightMode;
        InsPMDL_GetEmissionMode(InsSystemHandle, IniLightMode);//0316句柄
        ui->LightMode->setCurrentIndex(IniLightMode);

        if (IniLightMode == 0)
        {
            addToStateMessage("当前发光模式为 极大", false);
        }

        else if (IniLightMode == 1)
        {
            addToStateMessage("当前发光模式为 大", false);
        }
        else if (IniLightMode == 2)
        {
            addToStateMessage("当前发光模式为 标准", false);
        }
        else if (IniLightMode == 3)
        {
            addToStateMessage("当前发光模式为 小", false);
        }
        else if (IniLightMode == 4)
        {
            addToStateMessage("当前发光模式为 极小", false);
        }
        else
        {
            addToStateMessage("发光模式初始化失败 请重新选择", true);
        }
        //======================================================================

             //X方向角度=========================
        uint32_t AngelX;
        InsPMDL_GetInitialPhaseInXDirection(InsSystemHandle, AngelX);


        if (AngelX == 0)//0
        {
            ui->LightMode_4->setCurrentIndex(0);
            addToStateMessage("X起始相位 0", false);
        }

        else if (AngelX == 72)//90
        {
            ui->LightMode_4->setCurrentIndex(1);
            addToStateMessage("X起始相位 90", false);
        }
        else if (AngelX == 144)//180
        {
            ui->LightMode_4->setCurrentIndex(2);
            addToStateMessage("X起始相位 180", false);
        }
        else if (AngelX == 216)//270
        {
            ui->LightMode_4->setCurrentIndex(3);
            addToStateMessage("X起始相位 270", false);
        }


        //X正反向
        uint32_t DericX;
        InsPMDL_GetXStripeMovementDirection(InsSystemHandle, DericX);
        if (DericX == 0)//0
        {
            ui->LightMode3->setCurrentIndex(0);
            addToStateMessage("X条纹移动方向 正向", false);
        }

        else if (DericX == 1)//90
        {
            ui->LightMode3->setCurrentIndex(1);
            addToStateMessage("X条纹移动方向 反向", false);
        }



        //Y方向角度================================================
        uint32_t AngelY;//角度

        InsPMDL_GetInitialPhaseInYDirection(InsSystemHandle, AngelY);


        if (AngelY == 0)//0
        {
            ui->LightMode_6->setCurrentIndex(0);
            addToStateMessage("Y起始相位 0", false);
        }

        else if (AngelY == 72)//90
        {
            ui->LightMode_6->setCurrentIndex(1);
            addToStateMessage("Y起始相位 90", false);
        }
        else if (AngelY == 144)//180
        {
            ui->LightMode_6->setCurrentIndex(2);
            addToStateMessage("Y起始相位 180", false);
        }
        else if (AngelY == 216)//270
        {
            ui->LightMode_6->setCurrentIndex(3);
            addToStateMessage("Y起始相位 270", false);
        }


        uint32_t DericY;
        InsPMDL_GetYStripeMovementDirection(InsSystemHandle, DericY);//wwddjj
        if (DericY == 0)//0
        {
            ui->LightMode_5->setCurrentIndex(0);
            addToStateMessage("Y条纹移动方向 正向", false);
        }

        else if (DericY == 1)//90
        {
            ui->LightMode_5->setCurrentIndex(1);
            addToStateMessage("Y条纹移动方向 反向", false);
        }
        //=======================扩展触发极性================

        uint32_t IniExternTri;
        InsPMDL_GetExtendedTriggerOutputPolarity(InsSystemHandle, IniExternTri);

        if (IniExternTri == 0)//上
        {
            ui->ChuFaCombox_2->setCurrentIndex(0);
            addToStateMessage("扩展触发极性  上升沿", false);
        }

        else if (IniExternTri == 1)//下
        {
            ui->ChuFaCombox_2->setCurrentIndex(1);
            addToStateMessage("扩展触发极性  下降沿", false);
        }


        //===================相机侧======================================

        if (ui->insName->text().contains("16KCXP")) //0409
        {
            //ui->comboBox_4->clear();
            //ui->comboBox_4->deleteLater();
            //ui->comboBox_4->deleteLater();
            ui->comboBox_4->removeItem(2);
            ui->comboBox_4->removeItem(2);
            ui->comboBox_4->addItem("4");
            ui->comboBox_4->addItem("8");

        }

        if (ui->insName->text().contains("8KCXP")) //0409
        {
            //ui->comboBox_4->clear();
            //ui->comboBox_4->deleteLater();
            //ui->comboBox_4->deleteLater();
            ui->comboBox_4->removeItem(2);
            ui->comboBox_4->removeItem(2);
            ui->comboBox_4->addItem("3");
            ui->comboBox_4->addItem("4");

        }

        if (ui->insName->text().contains("16KCXP"))  //16K cxp
        {
            CamType = 2;
            qDebug() << "CamType：" << CamType;
        }
        else if (ui->insName->text().contains("8KCXP")) //8K cxp
        {
            CamType = 1;
            qDebug() << "CamType：" << CamType;
        }
        else                                                //4K CL
        {
            CamType = 0;
            qDebug() << "CamType：" << CamType;
        }


        TriExposure = InsPMDL_GetExposure(InsSystemHandle);//deviceHandles.cameraHandle
        cout << "0531 GetExposure~~~~~~~~~~~~:" << TriExposure << endl;
        if (TriExposure > 1000)
        {
            QMessageBox::warning(this, "提示", "系统已连接\n请检查是否有已打开的软件");
            QCoreApplication::quit();
        }

        ui->insInterface_8->setText(QString::number(InsPMDL_GetHeight(InsSystemHandle) / 8));//行高  //0418
        ui->insInterface_5->setText(QString::number(TriExposure,'f',2));//曝光
        ui->insInterface_6->setText(QString::number(InsPMDL_GetDigitalGain(InsSystemHandle)));//数字增益

        char* PACGain = (char*)malloc(4);
        InsPMDL_GetPacGain(InsSystemHandle, PACGain);//0412
        if (strcmp(PACGain, "x1") == 0)
        {
            ui->comboBox_4->setCurrentIndex(0);
            // ui->comboBox_4->addItem("1111");
        }
        else if (strcmp(PACGain, "x2") == 0)
        {
            ui->comboBox_4->setCurrentIndex(1);
            // ui->comboBox_4->addItem("2222");
        }
        else if (strcmp(PACGain, "x5") == 0 || strcmp(PACGain, "x4") == 0)
        {
            if (ui->insName->text().contains("8KCXP"))
            {
                ui->comboBox_4->setCurrentIndex(3);
            }
            else if (ui->insName->text().contains("16KCXP"))
            {
                ui->comboBox_4->setCurrentIndex(2);
            }
            else //4K CL
            {
                ui->comboBox_4->setCurrentIndex(2);
            }

        }
        else if (strcmp(PACGain, "x10") == 0 || strcmp(PACGain, "x8") == 0)
        {
            ui->comboBox_4->setCurrentIndex(3);
        }
        else
        {
            ui->comboBox_4->setCurrentIndex(3);
            //addToStateMessage("PACGain False",true);
        }
        uint32_t ReadCamHeight;

        //int ReadError = read_camera_height(deviceHandles.lightHandle,&ReadCamHeight);//&uint32
        int ReadError = InsPMDL_GetHeight(InsSystemHandle) / 8;
        if (ReadError != 0)
        {
            //InsPMDL_SetHeight(deviceHandles.cameraHandle,ReadCamHeight);
            //ui->insInterface_8->setText(QString::number(ReadError));//0523注、逻辑与4K相同

            addToStateMessage("行高加载成功", false);
        }





        qDebug() << "ReadErrror:::::::::" << ReadError << endl;

        if (1)//4K CL
        {
            uint32_t Rh;
            InsPMDL_GetCameraHeight(InsSystemHandle, &Rh);//0418 扫描链接
            ui->insInterface_8->setText(QString::number(Rh));

            if (ui->checkBox_SP2_2->isChecked())
            {
                PicNumCount = 9;
            }
            else
            {
                PicNumCount = 8;
            }
            InsPMDL_SetHeight(InsSystemHandle, Rh * PicNumCount);//0514

            InsPMDL_ConfigureFrameGrabber(InsSystemHandle);
            //设置行高后
        }




        OtherDeviceInfo* OtherInfo = new OtherDeviceInfo;
        InsPMDL_FindOtherInfo(InsSystemHandle, OtherInfo);//高级信息
        cout << "0320Deviceinformation:" << OtherInfo->device_information_;

        ui->insName_2->setText(OtherInfo->device_information_);//设备信息
        ui->insName_3->setText(OtherInfo->product_information_);//产品信息
        ui->insName_10->setText(OtherInfo->fpga_version_generation_date_);//fpga生成日期

        ui->insName_4->setText((OtherInfo->microblaze_vertion));//MB版本

        if (!(OtherInfo->fpga_version_generation_date_ == 0))
        {
            ui->insName_11->setText("已连接");
        }



        //=======================================初始化1215end========================================================


        QString lightModel = mLightInfo.device_model;

        if (lightModel.startsWith("INS"))
        {
            //ui->insName_5->setText(systemModel); // 显示系统型号
            ui->startAcqBtn->setEnabled(true);
            ui->stopAcqBtn->setEnabled(true);
            ui->widget->setEnabled(true);
            ui->tab_5->setEnabled(true);
            ui->tab_2->setEnabled(true);
            //ui->tab_3->setEnabled(false);
            ui->tab_4->setEnabled(true);
            ui->tab->setEnabled(true);
        }
        else
        {
            ui->insName_5->setText("Invalid light source model");
            addToStateMessage("光源连接错误！！！", true);
            ui->startAcqBtn->setEnabled(false);
            ui->stopAcqBtn->setEnabled(false);
            ui->widget->setEnabled(false);
            ui->tab_5->setEnabled(false);
            ui->tab_2->setEnabled(false);
            //ui->tab_3->setEnabled(false);
            ui->tab_4->setEnabled(false);
            ui->tab->setEnabled(false);
        }

        LightSignStr = mLightInfo.length;

        LightSignInt = LightSignStr.toInt();

        qDebug() << "LightSignInt" << LightSignInt << endl;

        now = QDateTime::currentDateTime();//1128
        snow = now.toString("yyyy_MM_dd_hh_mm_ss");
        qDebug() << "finale" << snow;

        ui->connectBtn->setEnabled(false);


        InsPMDL_SetTriggerMode(InsSystemHandle, EncodeTrigger);//0524
        //InsPMDL_SetEmissionMode(InsSystemHandle, Standard);

        InsPMDL_FPGAInitial(InsSystemHandle);//0417

        InsPMDL_InitialLightBrightness(InsSystemHandle);//0513
        //ui->connectBtn->setIcon(QIcon(":/icon/LEDOFF.png"));

        //delete spInsDeviceinfo->stInsCameraInfo;
        //delete spInsDeviceinfo->stIns2D5LightDeviceInfo;
        //delete spInsDeviceinfo->stOtherInfo;
        //delete spInsDeviceinfo;

    }

}






void MainWindow::on_startAcqBtn_clicked()
{
    /*
    uint16_t closeLaser = 0;
    write_extern_laser1(&closeLaser);
    write_extern_laser2(&closeLaser);
*/
    //InsPMDL_OpenOrCloseLasers(InsSystemHandle,0);//0522
    //clearImage();//1219修改行高清除原有图像

    //ui->checkBox_X1->setChecked(true); //ssss

    //ui->comboBox_3->setItemData(2, false, Qt::ItemIsEnabled);
    if (ui->comboBox_3->currentIndex()== 3)
    {
        ui->comboBox_3->setCurrentIndex(2);
    }
    
    DisplayOriImageCheck = false;
    WindowsJudge = false;

    imageProcessor->setStopProcessing(false);
    StopProcess = false;
    int aClear = checkCheckedCheckBoxes(checkedCheckBoxes);
    qDebug() << "CLEAR OR NOT : " << aClear;

    if (aClear == 1)
    {
        clearAllGraphicsViews();
    }
    //clearAllGraphicsViews();
 


    InsGlobalVar::frameID = 0;
  

    //camcam.set_data_callback(std::bind(&MainWindow::slot_updateBuffer,this,std::placeholders::_1));

    //cout<<"[startAcqBtn1]"<<endl;

    //ins_clgrab_continous(mpCamera); //cde

    //addToStateMessage("开始采集!", false);
    //info_timer->start(100);

    //cout<<"[startAcqBtn2]"<<endl;

    timer10->stop();//0316
    timer11->stop();//0316
    /*
//============================fpga通信=======================================
    uint32_t valueToWrite = 1;
    uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
    uint16_t lowValue = valueToWrite & 0xFFFF;



    /*uint32_t valueToWrite2 = 1234;
    uint16_t highValue2 = (valueToWrite2 >> 16) & 0xFFFF;
    uint16_t lowValue2 = valueToWrite2 & 0xFFFF;


 //========================0912  sdk ========================================
    uint16_t buffer[2] = {highValue,lowValue};*/

    //uint16_t buffer2[2] = {highValue2,lowValue2};


    //LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");//only

    //Ins_Modbus_udp::get_instance()->initmodbus_udp(DEVICE::get_instance()->ip_addr,502);//192.168.1.11

    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x165C,buffer);

    InsPMDL_ResetSin(InsSystemHandle);

    int modeIndex = ui->LightMode_2->currentIndex()+1;


    //InsPMDL_SetTriggerMode(InsSystemHandle, SoftTrigger);
    //InsPMDL_SetEmissionMode(InsSystemHandle, Max);//0522 添加 采集前需要设置模式

    if (ui->stackedWidget->currentIndex() == 0)
    {
        InsPMDL_SetTriggerMode(InsSystemHandle, EncodeTrigger);
    }
    else if (ui->stackedWidget->currentIndex() == 1)
    {
        InsPMDL_SetTriggerMode(InsSystemHandle, SoftTrigger);
    }
    else if (ui->stackedWidget->currentIndex() == 2)
    {    
        InsPMDL_SetTriggerMode(InsSystemHandle, IOTrigger);
    }


    InsPMDL_ConfigureFrameGrabber(InsSystemHandle);//申请缓冲区
    InsPMDL_GrabContinous(InsSystemHandle); //cde 0305

    if (SecJug == 1)//二级触发
    {
        int mode_Index = ui->LightMode_12->currentIndex() + 1;
        if (mode_Index == 1)// "Encode模式"
        {
            writeToModbusOne(0x0B6E, 1);//165CW
            cout << "SecJug Here" << endl;
        }
        else if (mode_Index == 2)// "软触发模式"internal
        {
            writeToModbusOne(0x0B44, 1);//1608W
        }
        else if (mode_Index == 3)// "IO触发模式"internal
        {
            writeToModbusOne(0x0B54, 1);//1628W
        }
    }
    
    addToStateMessage("开始采集!", false);
//=====================================================================================

    /*
    QModbusDataUnit writeUnit;

    int modeIndex = ui->LightMode_2->currentIndex();

    switch(modeIndex)
    {
    case 0: // "Encode模式"
        cout<<"Encode模式"<<endl;
        writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x165C, 2);
        break;

    case 1: // "Internal模式"
        cout<<"Internal模式"<<endl;
        writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x1608, 2);
        break;

    default:
        return;
    }

    writeUnit.setValue(0, highValue);
    writeUnit.setValue(1, lowValue);

    if (auto *reply = m_modbusClient->modbusClient->sendWriteRequest(writeUnit, 1))
    {
        if (!reply->isFinished())
            qDebug() << "success";
        else
            delete reply;
    }
    else
    {
        qDebug() << "Write data error:" << m_modbusClient->modbusClient->errorString();
    }
    */


    ui->startAcqBtn->setEnabled(false);

    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_19->setEnabled(false);//8
    ui->lineEdit_20->setEnabled(false);//11
    ui->lineEdit_12->setEnabled(false);
    //ui->FenPinSpinBox->setEnabled(false);
    //ui->BeiPinSpinBox->setEnabled(false);

    ui->LightMode_2->setEnabled(false);
    ui->ChuFaCombox->setEnabled(false);
    ui->EnCodeTriComBox->setEnabled(false);
    ui->LightMode->setEnabled(false);
    ui->LightMode3->setEnabled(false);
    ui->LightMode_4->setEnabled(false);
    ui->LightMode_5->setEnabled(false);
    ui->LightMode_6->setEnabled(false);
    ui->insInterface_5->setEnabled(false);
    ui->insInterface_6->setEnabled(false);
    ui->insInterface_8->setEnabled(false);

    ui->comboBox_4->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);

    ui->CamTriUI->setEnabled(false);
    ui->ExDelayUI->setEnabled(false);
    ui->ExNumUI->setEnabled(false);
    ui->ExWidthUI->setEnabled(false);
    ui->ChuFaCombox_2->setEnabled(false);

    ui->ModbusClientBtn_9->setEnabled(false);

    ui->PoseBtn->setEnabled(false);
    ui->DirectionPoseBtn->setEnabled(false);
    ui->ModbusClientBtn_10->setEnabled(false);
    ui->ClearBtn_2->setEnabled(false);
    ui->ModbusClientBtn_11->setEnabled(false);
    ui->ClearBtn_3->setEnabled(false);
    ui->disConnectBtn->setEnabled(false);


    ui->checkBox_Ori->setEnabled(false);

    //================================================

    






}

void MainWindow::on_stopAcqBtn_clicked()
{
   
    
    InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x165C, 0);
    InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1608, 0);
    InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1628, 0);//IO
    //imageProcessor->setStopProcessing(true);
    int modeIndex = ui->LightMode_2->currentIndex() + 1;
    //InsPMDL_GrabStop(InsSystemHandle);

    //addToStateMessage("停止采集!", false);

    QFuture<void> future = QtConcurrent::run([this]()
        {
            InsPMDL_GrabStop(InsSystemHandle);
        });
    addToStateMessage("停止采集!", false);


    //timer11->stop();

    ui->startAcqBtn->setEnabled(true);
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_19->setEnabled(true);
    ui->lineEdit_20->setEnabled(true);
    ui->lineEdit_12->setEnabled(true);
    ui->FenPinSpinBox->setEnabled(true);
    ui->BeiPinSpinBox->setEnabled(true);

    ui->LightMode_2->setEnabled(true);
    ui->ChuFaCombox->setEnabled(true);
    ui->EnCodeTriComBox->setEnabled(true);
    ui->LightMode->setEnabled(true);
    ui->LightMode3->setEnabled(true);
    ui->LightMode_4->setEnabled(true);
    ui->LightMode_5->setEnabled(true);
    ui->LightMode_6->setEnabled(true);
    ui->insInterface_5->setEnabled(true);
    ui->insInterface_6->setEnabled(true);
    ui->insInterface_8->setEnabled(true);

    ui->comboBox_4->setEnabled(true);
    ui->lineEdit_7->setEnabled(true);

    ui->CamTriUI->setEnabled(true);
    ui->ExDelayUI->setEnabled(true);
    ui->ExNumUI->setEnabled(true);
    ui->ExWidthUI->setEnabled(true);
    ui->ChuFaCombox_2->setEnabled(true);

    ui->ModbusClientBtn_9->setEnabled(true);
    imageProcessor->setStopProcessing(true);
    StopProcess = true;

    ui->PoseBtn->setEnabled(true);
    ui->DirectionPoseBtn->setEnabled(true);
    ui->ModbusClientBtn_10->setEnabled(true);
    ui->ClearBtn_2->setEnabled(true);
    ui->ModbusClientBtn_11->setEnabled(true);
    ui->ClearBtn_3->setEnabled(true);
    ui->disConnectBtn->setEnabled(true);

    DisplayOriImageCheck = true;//0604

    ui->checkBox_Ori->setEnabled(true);

    WindowsJudge = true;

    if (SecJug == 1)//二级触发
    {
        int mode_Index = ui->LightMode_12->currentIndex() + 1;
        if (mode_Index == 1)// "Encode模式"
        {
            writeToModbusOne(0x0B6E, 0);//165CW
            cout << "SecJug Here" << endl;
        }
        else if (mode_Index == 2)// "软触发模式"internal
        {
            writeToModbusOne(0x0B44, 0);//1608W
        }
        else if (mode_Index == 3)// "IO触发模式"internal
        {
            writeToModbusOne(0x0B54, 0);//1628W
        }
    }


   // DestroyLumitraxToolPtr(toolPtr);
}
//========================写0 modbus============================
/*
void MainWindow::writeOModbusRegister(uint16_t address)//0918
{
    uint32_t StopValue = 0;
    uint16_t highValue = (StopValue >> 16) & 0xFFFF;
    uint16_t lowValue = StopValue & 0xFFFF;



    //=============sdk==========================

    uint16_t buffer[2] = {highValue,lowValue};

    //LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");//glass

    //Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11",502);////glassdisConnectBtn


    int modeIndex = ui->LightMode_2->currentIndex();

    switch(modeIndex)
    {
    case 0: // "Encode模式"
        //cout<<"Encode模式"<<endl;
        //writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x165C, 2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B2E,buffer);

        //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x1600,buffer2);

        qDebug()<<"write 0x165c";
        break;

    case 1: // "Internal模式"
        //cout<<"Internal模式"<<endl;
        //writeUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x1608, 2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B04,buffer);
        break;

    default:
        return;
    }


}
*/

//========================

void MainWindow::on_disConnectBtn_clicked()//awm
{


    ui->insInterface_4->clear();
    ui->insName_4->clear();
    QPixmap pixmapRed(":/image/icon/Red20.png");//0327

    ui->label_12->setPixmap(pixmapRed);

    InsPMDL_Close(&InsSystemHandle);//0411
    addToStateMessage("系统断开连接!",false);
    m_camState = false;

    ui->connectBtn->setEnabled(true);
    //ui->insName_4->clear();
    


}

void MainWindow::update_buffer(InsPMDL_Buffer *pbuffer)//wdj
{
    qDebug()<<"[UpDate_pbuffer:]";
    mpMainWindow->slot_updateBuffer(pbuffer->pdata);

    //mpMainWindow->process_buffer(pbuffer->pdata);//newthread
   

}

void ImageExtarction(unsigned char* srcDta, int inWidth, int inHeight, int inStride, int num, InsImage * dstData)
{
    if (num <= 0)
        num = 1;
    int outWidth = inWidth;
    int outHeight = inHeight / num;
    for (int i = 0; i < num; ++i)
    {
        dstData[i].width = outWidth;
        dstData[i].height = outHeight;
        dstData[i].stride = inStride;
        dstData[i].data = new unsigned char[outHeight * outWidth];
    }

    for (int i = 0; i < outHeight; ++i)
    {
        for (int j = 0; j < num; ++j)
        {
            memcpy(dstData[j].data + i * inWidth, srcDta + (i * num + j) * inStride, inWidth);
        }

    }
}
//void* toolPtr;//开始采集

void DestroyImageArray(InsImage * dstData, int num)
{
    for (int i = 0; i < num; ++i)
    {
        if (dstData[i].data != NULL)
            delete[] dstData[i].data;
        dstData[i].data = NULL;
    }
}


void MainWindow::process_buffer(unsigned char* pbuffer)
{

    if (pbuffer == nullptr)
    {
        qDebug() << "pbuffer is nulltptr";
    }
    QString appDir = QCoreApplication::applicationDirPath();
    std::string appDirCv = appDir.toLocal8Bit().constData();
    int cameraHeight = InsPMDL_GetHeight(InsSystemHandle);
    int cameraWidth = InsPMDL_GetWidth(InsSystemHandle);
    bool freeCollect = false; // 
    int picNumCount = 8; //
    qDebug() << "Emitting startImageProcessing";
    emit startImageProcessing(pbuffer, cameraWidth, cameraHeight, appDirCv, freeCollect, picNumCount);
    //ui->insFrameID->setText(QString::number(InsPMDL_GetFrameId(mpCamera)));
    ui->insFrameID->setText(QString::number(imageProcessor->frameId));
}

void MainWindow::slot_updateBuffer(unsigned char *pbuffer)//0919 //实时更新
{
   

    if (StopProcess)//停止处理图像
    {
        return;
    }
    //emit imageProcessedFinish();//0530
    QString appDir = QCoreApplication::applicationDirPath();
    qDebug()<< "appDIR:" << appDir << endl;
    std::string appDirCv = (std::string)(appDir.toLocal8Bit());
    cout << "appDirCv:" << appDirCv << endl;//0529
    int cameraHeight = InsPMDL_GetHeight(InsSystemHandle);//0316   // CL: 输入4w  实际39992
    int cameraWidth = InsPMDL_GetWidth(InsSystemHandle);
    int cameraFrameSize1 = InsPMDL_GetFrameSize(InsSystemHandle);
    cout << "cameraHeight:" << cameraHeight;
    cout << "cameraWidth:" << cameraWidth;
    uint32_t cameraFrameSize = cameraHeight * cameraWidth;
    cout << "cameraFrameSizeFrameSize:"<<cameraFrameSize1;
    cout << "camerFrameSizeHxW:" << cameraFrameSize;

    if (FreeCollect == true)
    {
        cv::Mat img = cv::Mat(cameraHeight, cameraWidth, CV_8UC1, pbuffer);
        cv::imwrite(appDirCv + "/image/Normal.bmp", img);
    }
    else
    {
        if(PicNumCount == 9)
        {
    //    void* toolPtr;
    //    uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize *sizeof (uint8_t));//0925
    //    memcpy(l_img_buffer,pbuffer, cameraFrameSize);//处理l_img_buffer //0925
    //   
    ////===========================================================================0823+
    ////cv::Mat src = l_img_buffer;

    ////cv::Mat src(InsGlobalVar::lines,InsGlobalVar::m_Width, CV_8UC1, l_img_buffer);

    ////===================================图像处理==============================================

    //    InsImage srcImage[9];//122089
    //    ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth,9 , srcImage);//1211
    //    CreateLumitraxToolPtr(toolPtr);

    //    SetLumitraxToolParam(toolPtr,normalGain,specualrGain,diffuseGain,false,shapeGain,0);//1.0绝对值模式

    //    InsImage dstImage[5];

    //    CreateLumitrxImage(toolPtr,srcImage,dstImage);

    //// 显示及保存图片
    // //0925

    //    cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1 , srcImage[0].data, srcImage[0].stride);
    //    cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1 , srcImage[1].data, srcImage[1].stride);
    //    cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1 , srcImage[2].data, srcImage[2].stride);
    //    cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1 , srcImage[3].data, srcImage[3].stride);
    //    cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1 , srcImage[4].data, srcImage[4].stride);
    //    cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1 , srcImage[5].data, srcImage[5].stride);
    //    cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1 , srcImage[6].data, srcImage[6].stride);
    //    cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1 , srcImage[7].data, srcImage[7].stride);
    //    cv::Mat IR = cv::Mat(srcImage[8].height, srcImage[8].width, CV_8UC1 , srcImage[8].data, srcImage[8].stride);

    //    cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1 , dstImage[0].data, dstImage[0].stride);
    //    cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1 , dstImage[1].data, dstImage[1].stride);
    //    cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1 , dstImage[2].data, dstImage[2].stride);
    //    cv::Mat Glossratio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1 , dstImage[3].data, dstImage[3].stride);
    //    cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1 , dstImage[4].data, dstImage[4].stride);//aaa

    //    cv::imwrite(appDirCv + "/image/X1.bmp", X1);//0919 need change//1018changeaabbb
    //    cv::imwrite(appDirCv + "/image/X2.bmp", X2);
    //    cv::imwrite(appDirCv + "/image/X3.bmp", X3);
    //    cv::imwrite(appDirCv + "/image/X4.bmp", X4);
    //    cv::imwrite(appDirCv + "/image/Y1.bmp", Y1);
    //    cv::imwrite(appDirCv + "/image/Y2.bmp", Y2);
    //    cv::imwrite(appDirCv + "/image/Y3.bmp", Y3);
    //    cv::imwrite(appDirCv + "/image/Y4.bmp", Y4);
    //    cv::imwrite(appDirCv + "/image/Normal.bmp", Normal);
    //    cv::imwrite(appDirCv + "/image/Specular.bmp", Specular);
    //    cv::imwrite(appDirCv + "/image/Diffuse.bmp", Diffuse);
    //    cv::imwrite(appDirCv + "/image/Shape.bmp", Shape);
    //    cv::imwrite(appDirCv + "/image/Glossratio.bmp", Glossratio);//1018改路径
    //    cv::imwrite(appDirCv + "/image/IR.bmp", IR);


    ////===========================================================================0823end


    //    free(l_img_buffer);//释放
    //    l_img_buffer = NULL;
    //    InsGlobalVar::pixelView_update_flag = false;
    //    //qDebug()<<"CCC";
    //    DestroyImageArray(srcImage, 9);
    //    DestroyImageArray(dstImage, 5);
    //    DestroyLumitraxToolPtr(toolPtr);

    }

         else if( PicNumCount == 8)
              {

             void* toolPtr;


              qDebug()<<"BBB";

              qDebug()<<"[pbuffer:]"<<pbuffer;
              qDebug()<<*pbuffer<<endl;

              uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize *sizeof (uint8_t));//0925


              memcpy(l_img_buffer,pbuffer, cameraFrameSize);//处理l_img_buffer //0925

              qDebug()<<"[l_img_buffer:]"<<l_img_buffer;
              qDebug()<<*l_img_buffer<<endl;


              InsImage srcImage[8];//122089


             ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 8, srcImage);//l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 8, srcImage

             //CreateLumitraxToolPtr(toolPtr);

             //InsPMDLumitraxToolCreateInstance(toolPtr);//开始采集
             InsPMDLumitraxToolCreateInstance(toolPtr);
             if (toolPtr == nullptr) {
                 qDebug() << "Tool creation failed";
                 return;
             }
             else
             {
                 qDebug() << "Tool creation success";
             }

             cv::Mat image1(srcImage[0].height, srcImage[0].width, CV_8UC1, srcImage[0].data, srcImage[0].stride);
             qDebug() << "image1 data: " << image1.data;
             cv::Mat image2(srcImage[1].height, srcImage[1].width, CV_8UC1, srcImage[1].data, srcImage[1].stride);
             cv::Mat image3(srcImage[2].height, srcImage[2].width, CV_8UC1, srcImage[2].data, srcImage[2].stride);
             cv::Mat image4(srcImage[3].height, srcImage[3].width, CV_8UC1, srcImage[3].data, srcImage[3].stride);
             cv::Mat image5(srcImage[4].height, srcImage[4].width, CV_8UC1, srcImage[4].data, srcImage[4].stride);
             cv::Mat image6(srcImage[5].height, srcImage[5].width, CV_8UC1, srcImage[5].data, srcImage[5].stride);
             cv::Mat image7(srcImage[6].height, srcImage[6].width, CV_8UC1, srcImage[6].data, srcImage[6].stride);
             cv::Mat image8(srcImage[7].height, srcImage[7].width, CV_8UC1, srcImage[7].data, srcImage[7].stride);
             

             InsPMDLumitraxToolClearImage(toolPtr);
             InsPMDLumitraxToolSetImage(toolPtr, image1);
             InsPMDLumitraxToolSetImage(toolPtr, image2);
             InsPMDLumitraxToolSetImage(toolPtr, image3);
             InsPMDLumitraxToolSetImage(toolPtr, image4);
             InsPMDLumitraxToolSetImage(toolPtr, image5);
             InsPMDLumitraxToolSetImage(toolPtr, image6);
             InsPMDLumitraxToolSetImage(toolPtr, image7);
             InsPMDLumitraxToolSetImage(toolPtr, image8);

             InsPMDParams params;
             params.flag = PMD_ALL_ENABLED;
             params.normal_k = normalGain;
             params.normal_b = 0;

             cv::Mat normal, specular, diffuse, glossratio, shape, shapeabs;
             InsPMDLumitrxToolRun(toolPtr, params, normal, specular, diffuse, glossratio, shape, shapeabs);


              //void* toolPtr;


              //qDebug()<<"BBB";

              //qDebug()<<"[pbuffer:]"<<pbuffer;
              //qDebug()<<*pbuffer<<endl;

              //uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize *sizeof (uint8_t));//0925


              //memcpy(l_img_buffer,pbuffer, cameraFrameSize);//处理l_img_buffer //0925

              //qDebug()<<"[l_img_buffer:]"<<l_img_buffer;
              //qDebug()<<*l_img_buffer<<endl;



              ////===================================图像处理==============================================

              //InsImage srcImage[8];//122089

              //ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth,8 , srcImage);//1211

              //CreateLumitraxToolPtr(toolPtr);

              //SetLumitraxToolParam(toolPtr,normalGain,specualrGain,diffuseGain,false,shapeGain,0);//1.0绝对值模式

              //InsImage dstImage[5];

              //CreateLumitrxImage(toolPtr,srcImage,dstImage);

              //qDebug()<<"[ImageExtarction:]";

              //// 显示及保存图片
              // //0925

              cv::Mat X1 = cv::Mat(srcImage[0].height, srcImage[0].width, CV_8UC1 , srcImage[0].data, srcImage[0].stride);
              cv::Mat X2 = cv::Mat(srcImage[1].height, srcImage[1].width, CV_8UC1 , srcImage[1].data, srcImage[1].stride);
              cv::Mat X3 = cv::Mat(srcImage[2].height, srcImage[2].width, CV_8UC1 , srcImage[2].data, srcImage[2].stride);
              cv::Mat X4 = cv::Mat(srcImage[3].height, srcImage[3].width, CV_8UC1 , srcImage[3].data, srcImage[3].stride);
              cv::Mat Y1 = cv::Mat(srcImage[4].height, srcImage[4].width, CV_8UC1 , srcImage[4].data, srcImage[4].stride);
              cv::Mat Y2 = cv::Mat(srcImage[5].height, srcImage[5].width, CV_8UC1 , srcImage[5].data, srcImage[5].stride);
              cv::Mat Y3 = cv::Mat(srcImage[6].height, srcImage[6].width, CV_8UC1 , srcImage[6].data, srcImage[6].stride);
              cv::Mat Y4 = cv::Mat(srcImage[7].height, srcImage[7].width, CV_8UC1 , srcImage[7].data, srcImage[7].stride);
              //cv::Mat IR = cv::Mat(srcImage[8].height, srcImage[8].width, CV_8UC1 , srcImage[8].data, srcImage[8].stride);



              //cv::Mat Normal = cv::Mat(dstImage[0].height, dstImage[0].width, CV_8UC1 , dstImage[0].data, dstImage[0].stride);
              //cv::Mat Specular = cv::Mat(dstImage[1].height, dstImage[1].width, CV_8UC1 , dstImage[1].data, dstImage[1].stride);
              //cv::Mat Diffuse = cv::Mat(dstImage[2].height, dstImage[2].width, CV_8UC1 , dstImage[2].data, dstImage[2].stride);
              //cv::Mat Glossratio = cv::Mat(dstImage[3].height, dstImage[3].width, CV_8UC1 , dstImage[3].data, dstImage[3].stride);
              //cv::Mat Shape = cv::Mat(dstImage[4].height, dstImage[4].width, CV_8UC1 , dstImage[4].data, dstImage[4].stride);//aaa

              cv::imwrite(appDirCv + "/image/X1.bmp", X1);//0919 need change//1018changeaabbb
              cv::imwrite(appDirCv + "/image/X2.bmp", X2);
              cv::imwrite(appDirCv + "/image/X3.bmp", X3);
              cv::imwrite(appDirCv + "/image/X4.bmp", X4);
              cv::imwrite(appDirCv + "/image/Y1.bmp", Y1);
              cv::imwrite(appDirCv + "/image/Y2.bmp", Y2);
              cv::imwrite(appDirCv + "/image/Y3.bmp", Y3);
              cv::imwrite(appDirCv + "/image/Y4.bmp", Y4);
              cv::imwrite(appDirCv + "/image/Normal.bmp", normal);
              cv::imwrite(appDirCv + "/image/Specular.bmp", specular);
              cv::imwrite(appDirCv + "/image/Diffuse.bmp", diffuse);
              cv::imwrite(appDirCv + "/image/Shape.bmp", shape);
              cv::imwrite(appDirCv + "/image/Glossratio.bmp", glossratio);//1018改路径
              cv::imwrite(appDirCv + "/image/ShapeAbs.bmp", shapeabs);//1018改路径
              //cv::imwrite(appDirCv + "/image/IR.bmp", IR);


              ////===========================================================================0823end



              free(l_img_buffer);//释放

              l_img_buffer = NULL;
              InsGlobalVar::pixelView_update_flag = false;
              ////qDebug()<<"CCC";
              DestroyImageArray(srcImage, 8);
              //DestroyImageArray(dstImage, 5);
              InsPMDLumitraxToolDestroyInstance(toolPtr);//

        }
    }
    emit imageProcessedFinish();//0828
    //===========显示帧数====================================
    ui->insFrameID->setText(QString::number(InsPMDL_GetFrameId(mpCamera)));//1016
    //ui->insFrameID->setText(QString::number((InsCamera::frame_id_)+1,10,0));//1016
    //emit imageProcessedFinish();//0828

}

//工作模式  IKP_GRAB_MODE
/*void MainWindow::on_insWorkMode_activated(int index)//insWorkMode0831
{
    ITKSTATUS res = ITKSTATUS_OK;
    int ret = IK_RTN_OK;
    if(index==0)//自由模式
    {
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_INNER);
        CHECK_IKAPBOARD(ret);
    }
    else
    {
        // 设置采集卡触发模式。
        // Set frame grabber trigger mode.
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_OUTTER);
        CHECK_IKAPBOARD(ret);
    }
    ui->TriggerWidget->setCurrentIndex(index);
}*/

//触发源  IKP_BOARD_TRIGGER_SOURCE
/*void MainWindow::on_insTriggerSource_activated(int index)//insTriggerSource0831
{
    ITKSTATUS res = ITKSTATUS_OK;
    int ret = IK_RTN_OK;
    switch (index)
    {
        case 0:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
        CHECK_IKAPBOARD(ret);
        break;
        case 1:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT2);
        CHECK_IKAPBOARD(ret);
        break;
        case 2:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1);
        CHECK_IKAPBOARD(ret);
        break;
        case 3:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC1);
        CHECK_IKAPBOARD(ret);
        break;
        case 4:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC2);
        CHECK_IKAPBOARD(ret);
        break;
        case 5:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_INNER_TRIGGER);
        CHECK_IKAPBOARD(ret);
        break;
        case 6:
        ret = IKapSetInfo(g_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_SOFTWARE);
        CHECK_IKAPBOARD(ret);
        break;
        default:
        break;
    }
}*/

//触发帧数  IKP_FRAME_COUNT
/*void MainWindow::on_setFrameCountBtn_clicked()//setFrameCountBtn0831
{
    //InsGlobalVar::nFrameCount = ui->insFrameCount->value();//insFrameCount
    IKapSetInfo(g_hBoard, IKP_FRAME_COUNT, InsGlobalVar::nFrameCount);
}*/


/*void MainWindow::on_insImageType_currentIndexChanged(int index)//insImageType
{
    IKapSetInfo(g_hBoard, IKP_IMAGE_TYPE, index);
}*/

void MainWindow::slot_infoUpdate()
{
   // ui->insFrameID->setText(QString("%1").arg(InsGlobalVar::frameID));
}


void MainWindow::on_checkBox_clicked(bool checked)
{
    InsGlobalVar::m_saveImage = checked;
}

//cv::Mat extractImg(cv::Mat src,cv::Mat &dst,int index)
//{
//    cv::Mat img;
//    return img;
//}

/*void MainWindow::on_pushButton_3_clicked()
{
    cv::Mat img = cv::imread("C:\\Users\\INSNEX\\Desktop\\Ins_25DCAM_Demo\\debug\\2.jpg",-1);
    qDebug()<<"img"<<img.cols;
    cv::imshow("aa",img);
}*/
//删除pushbutton3


void MainWindow::on_doubleSpinBox_3_valueChanged(double arg1)
{
    //InsGlobalVar::lines = arg1;
}


void MainWindow::on_ChuFaCombox_currentIndexChanged(int index)//编码器回退跟踪
{
    InsResult a  = InsPMDL_SetTriggerDirection(InsSystemHandle, index);//0105 //0319b

    if ((a == InsResult::INSCSTATUS_OK)|| (a == InsResult::INSSTATUS_SUCCESS))
    {
        addToStateMessage("触发行程方向成功", false);
    }
    else
    {
        addToStateMessage("触发行程方向失败", true);
    }

    

}


void MainWindow::on_BeiPinSpinBox_valueChanged(int arg1) //一级触发
{
    InsPMDL_SetFrequencyMultiplier(InsSystemHandle, arg1);//0105 //0523deviceHandles.lightHandle
    
    addToStateMessage("倍频—输入成功",false);

}


void MainWindow::on_FenPinSpinBox_valueChanged(int arg1) //一级触发
{
    InsPMDL_SetFrequencyDivision(InsSystemHandle, arg1);//0105

    addToStateMessage("分频—输入成功",false);
}


void MainWindow::on_ModbusClientBtn_10_clicked()
{
    InsPMDL_OpenOrCloseLasers(deviceHandles.lightHandle, 1);

}


void MainWindow::on_ClearBtn_2_clicked()
{
    InsPMDL_OpenOrCloseLasers(deviceHandles.lightHandle, 0);
 
}


void MainWindow::on_EnCodeTriComBox_currentIndexChanged(int index)//0921a  //0319c
{
    InsPMDL_SetEncoderTriggerMode(InsSystemHandle, index);//0105

}


void MainWindow::on_LightMode_2_currentIndexChanged(int index)
{
    if(index == 0)
    {
     ui->stackedWidget->setCurrentIndex(0);
    
     InsPMDL_SetTriggerMode(InsSystemHandle, EncodeTrigger);
    }
    if(index == 1)
    {
       ui->stackedWidget->setCurrentIndex(1);
       InsPMDL_SetTriggerMode(InsSystemHandle, SoftTrigger);
    }
    if(index == 2)
    {
       ui->stackedWidget->setCurrentIndex(2);
       InsPMDL_SetTriggerMode(InsSystemHandle, IOTrigger);
    }


}


void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    for (int i = 0; i < 12; ++i)  // 12个窗口
    {
        QGraphicsScene* scene = m_Images[i]->scene();  // 获取场景
        if (scene && m_TextItems[i])
        {
            scene->removeItem(m_TextItems[i]);  // 从场景中移除
            delete m_TextItems[i];  // 删除对象
            m_TextItems[i] = nullptr;  // 将指针设为 nullptr
        }
    }
}


void MainWindow::on_ModbusClientBtn_11_clicked()//自由开 0530
{
    StopProcess = false;

    
    
    clearAllGraphicsViews();
    //clearAllGraphicsViews();
    //clearAllGraphicsViews();

    int res = 0;
  
    QFuture<void> future = QtConcurrent::run([this]()
        {
            InsResult res = InsPMDL_FreerunForFocus(InsSystemHandle, On);//0305
            
            int err = static_cast<int>(res);
            
            cout << "Res:::" << err << endl;

            if (err == 0)
            {
                ui->checkBox_Gro->setChecked(true);
            }

        });
    addToStateMessage("自由取像开", false);
    //InsPMDL_FreerunForFocus(InsSystemHandle, On);//0305

    ui->comboBox_3->setCurrentIndex(0);
    //clearAllGraphicsViews();
    ui->stopAcqBtn->setEnabled(false);
    //ui->checkBox_Gro->setChecked(true);
    //clearAllGraphicsViews();
    MyLine = ui->lineEdit_6->text().toDouble();
    //timer10->stop();//0316
    //InsPMDL_FreerunForFocus(InsSystemHandle,On);//0305

    PixWindow = 0;

    FreeCollect = true;//

    ui->startAcqBtn->setEnabled(false);

    ui->tab_2->setEnabled(false);


    ui->ModbusClientBtn_11->setEnabled(false);
    ui->ExNumUI->setEnabled(false);
    ui->ExWidthUI->setEnabled(false);
    ui->ChuFaCombox_2->setEnabled(false);
    ui->ModbusClientBtn_10->setEnabled(false);
    ui->ClearBtn_2->setEnabled(false);
    ui->insInterface_8->setEnabled(false);
    ui->checkBox_SP2_2->setEnabled(false);
}

void MainWindow::on_ClearBtn_3_clicked()//自由关
{

    clearAllGraphicsViews();
    PixWindow = 1;
    FreeCollect = false;
    ui->stopAcqBtn->setEnabled(true);
    ui->checkBox_Gro->setChecked(false);
   
    clearAllGraphicsViews();


    QFuture<void> future = QtConcurrent::run([this]()
        {
 
            InsPMDL_FreerunForFocus(InsSystemHandle, Off); 
        });
    //InsPMDL_FreerunForFocus(InsSystemHandle,Off);//0305

    addToStateMessage("自由取像关", false);

    //QThread::sleep(1);
    ui->startAcqBtn->setEnabled(true);
    ui->tab_2->setEnabled(true);

   
    ui->ExNumUI->setEnabled(true);
    ui->ExWidthUI->setEnabled(true);
    ui->ChuFaCombox_2->setEnabled(true);
    ui->ModbusClientBtn_10->setEnabled(true);
    ui->ClearBtn_2->setEnabled(true);
    ui->insInterface_8->setEnabled(true);
    ui->checkBox_SP2_2->setEnabled(true);
    QTimer::singleShot(2500, this, [this]() 
        {
        ui->ModbusClientBtn_11->setEnabled(true);
        });



    //============================1219end==================================
}

   double TriExJug;  //0
void MainWindow::on_insInterface_5_returnPressed()//11155
{
    double TriExposure = ui->lineEdit_6->text().toDouble();//行频值

    TriExposure = TriExposure / PicNumCount;

    myexposure = ui->insInterface_5->text().toDouble();//1115改为全局   //20  21

    //TriExposure = InsPMDL_GetExposure(InsSystemHandle);      //21  20

    InsResult ExJudge;

    
    if (TriExJug < TriExposure)
    {
        TriExJug = TriExposure;                         //21
    }

    if (myexposure <= TriExJug) //0412    //20<21
    {
        ExJudge =InsPMDL_SetExposure(InsSystemHandle, myexposure);

        qDebug() << "Current Exposure is:" << InsCameraIkap::ins_exposure_;
        //qDebug()<<"Current Exposure is:"<<InsCamera::ins_exposure_;

        if (ExJudge == InsResult::INSCSTATUS_OK)
        {
            addToStateMessage("曝光写入成功!", false);

        }
        else
        {
            addToStateMessage("曝光写入失败!", true);
        }
        
    }
    else
    {
        addToStateMessage("曝光输入失败,输入值过大", true);
        ui->insInterface_5->setText(QString::number(TriExposure));
    }
}


void MainWindow::on_insInterface_8_returnPressed()
{


    QString inputText = ui->insInterface_8->text();
    bool isNumeric = true;

    // 检查每个字符是否为数字
    for (QChar c : inputText) 
    {
        if (!c.isDigit()) 
        {
            isNumeric = false;
            break;
        }
    }

    if (!isNumeric) {
        addToStateMessage("行高输入值无效", true);
        return;
    }

    int myLineHigh  = ui->insInterface_8->text().toInt();

    uint32_t H = myLineHigh;

    InsResult HeightJudge;

    if (H<15001)
    {
        InsPMDL_SaveCameraHeight(InsSystemHandle, &H);//0514

        InsResult er = InsPMDL_SetHeight(InsSystemHandle, myLineHigh * PicNumCount);

        //camcam.ConfigureBoard();

        InsPMDL_ConfigureFrameGrabber(InsSystemHandle);

        //int err = static_cast<int>(er);
        //qDebug() << err << endl;
         addToStateMessage("行高输入成功", false);
        
        //uint32_t SaveCamHeight = static_cast<uint32_t>(myLineHigh);

        //save_camera_height(deviceHandles.lightHandle,&SaveCamHeight);
    }
    else
    {
        addToStateMessage("行高输入失败、最高为15000", true);
    }


    


}




void MainWindow::on_insInterface_6_returnPressed()
{
    InsResult DigJudge;
    double myDigGain = ui->insInterface_6->text().toDouble();
    qDebug() << "myDigGain:::" << myDigGain << endl;
    DigJudge = InsPMDL_SetDigitalGain(InsSystemHandle,myDigGain);
    qDebug()<<"Current DigGain is "<< InsPMDL_GetDigitalGain(InsSystemHandle);//deviceHandles.cameraHandle
    //qDebug()<<"Current DigGain is "<<InsCamera::ins_digital_gain_;

    if (DigJudge == InsResult::INSCSTATUS_OK)
    {
        addToStateMessage("数字增益输入成功", false);

    }
    else
    {
        addToStateMessage("数字增益输入失败!", true);
    }
    
}
void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
#if 1
    QList<QString> strList;
    if (ui->insName->text().contains("16KCXP"))
    {
        strList.append(QString("x1"));//QQQ
        strList.append(QString("x2"));
        strList.append(QString("x4"));
        strList.append(QString("x8"));

    }
    else if (ui->insName->text().contains("8KCXP"))
    {
        strList.append(QString("x1"));//QQQ
        strList.append(QString("x2"));
        strList.append(QString("x3"));
        strList.append(QString("x4"));

    }
    else
    {
        strList.append(QString("x1"));
        strList.append(QString("x2"));
        strList.append(QString("x5"));
        strList.append(QString("x10"));

    }
    //strList.append(QString("x1"));
    //strList.append(QString("x2"));
    //strList.append(QString("x5"));
    //strList.append(QString("x10"));
    InsPMDL_SetPacGain(InsSystemHandle, (char*)strList[index].toStdString().c_str());

#else
    const char* consStr = "x1";
    const char* consStr2 = "x2";
    const char* consStr3 = "x5";
    const char* consStr5 = "x10";

    char* NoconsStr = const_cast<char*>(consStr);
    char* NoconsStr2 = const_cast<char*>(consStr2);
    char* NoconsStr3 = const_cast<char*>(consStr3);
    char* NoconsStr5 = const_cast<char*>(consStr5);
    if (index == 0)
    {
        InsPMDL_SetPacGain(mpCamera, NoconsStr);
    }
    if (index == 1)
    {
        InsPMDL_SetPacGain(mpCamera, NoconsStr2);
    }
    if (index == 2)
    {
        InsPMDL_SetPacGain(mpCamera, NoconsStr3);
    }
    if (index == 3)
    {
        InsPMDL_SetPacGain(mpCamera, NoconsStr5);
    }
#endif
}

//=============================1009=================================
void MainWindow::on_pushButton_5_clicked()
{
    //LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");//glass
    //Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);//glass

    // 备份寄存器值
    BackupRegisters("backup.txt");

    // 向寄存器写入测试值
    for (auto offset : SYSTEM_OFFSETS)
    {
        uint32_t registerAddress = SYSTEM_BASE_ADDRESS + offset;
        WriteToRegister(registerAddress, 1);

        uint32_t valueRead;
        if (!ReadFromRegister(registerAddress,valueRead))
        {
            std::cout << "读取地址 " << std::hex << registerAddress << " 失败!" << std::endl;
        }
    }

    // 恢复寄存器值
    RestoreRegisters("backup.txt");

}

//=============================1009====================================


//单次读写
/*
void MainWindow::on_pushButton_6_clicked()
{
    bool ok1, ok2;

    uint32_t address = ui->AddEdit->toPlainText().toUInt(&ok1, 16);  // 从textEdit_2读取地址
    uint32_t valueToWrite = ui->NumEdit->toPlainText().toUInt(&ok2, 16);  // 从textEdit_3读取要写入的值

    if(!ok1 || !ok2)
    {
        QMessageBox::warning(this, "错误", "请输入有效的十六进制地址和值！");
        return;
    }

    uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
    uint16_t lowValue = valueToWrite & 0xFFFF;
    uint16_t buffer[2] = {highValue, lowValue};

    // 初始化并写入值
    LOCALDEVICE::get_instance()->init("192.168.1.2", "255.255.255.0");
    Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);

    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(address, buffer);
}


void MainWindow::on_pushButton_7_clicked()
{
    bool ok;
    uint32_t address = ui->AddEdit->toPlainText().toUInt(&ok, 16);  // 从textEdit_2读取地址

    if(!ok) {
        QMessageBox::warning(this, "错误", "请输入有效的十六进制地址！");
        return;
    }

    uint16_t buffer[2];

    // 初始化并读取值
    LOCALDEVICE::get_instance()->init("192.168.1.2", "255.255.255.0");
    Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(address, buffer);

    // 将读取到的值显示在textEdit_3中
    uint32_t valueRead = (static_cast<uint32_t>(buffer[0]) << 16) | buffer[1];
    ui->NumEdit->setText(QString::number(valueRead, 16).toUpper());
}

*/

//===================================多次 start===============================


void MainWindow::on_pushButton_6_clicked()//write
{
    int index1 = ui->comboBox_5->currentIndex();

    QStringList addressList = ui->AddEdit->toPlainText().split('\n'); // 使用换行符作为分隔符
    QStringList valueList = ui->NumEdit->toPlainText().split('\n');

       if(addressList.size() != valueList.size())
       {
           QMessageBox::warning(this, "错误", "地址和值的数量不匹配！");
           return;
       }

       for(int i = 0; i < addressList.size(); i++)
       {
           bool ok1, ok2;
           uint32_t address = addressList[i].toUInt(&ok1, 16);
           //address = address/2;
           uint32_t valueToWrite = valueList[i].toUInt(&ok2, 16);

           if(!ok1 || !ok2)
           {
               QMessageBox::warning(this, "错误", QString("第%1行的输入无效！").arg(i+1));
               return;
           }

         

           //选板
           switch(index1)//dwg
           {
           case 0:
               //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,address,buffer);

               InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, address, valueToWrite);
               addToStateMessage("写入成功!",false);
               break;
           case 1:
               if(LightSignInt ==10)//sss
               {
                   addToStateMessage("无效的选择",true);

               }
               else
               {
               //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,address,buffer);
               InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 2, address, valueToWrite);
               addToStateMessage("写入成功!",false);
               }
               break;
           case 2:
               if(LightSignInt ==10)//sss
               {
                   addToStateMessage("无效的选择",true);
               }
               else
               {
               //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,address,buffer);
               InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 3, address, valueToWrite);
               addToStateMessage("写入成功!",false);
               }
               break;
           case 3:
               if(LightSignInt ==10 || LightSignInt ==30)//sss
               {
                   addToStateMessage("无效的选择",true);
               }
               else
               {
               //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD,address,buffer);
               InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 4, address, valueToWrite);
               addToStateMessage("写入成功!",false);
               }
               break;
           case 4:
               if(LightSignInt ==10 || LightSignInt ==30)//sss
               {
                   addToStateMessage("无效的选择",true);
               }
               else
               {
               //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD,address,buffer);
               InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 5, address, valueToWrite);
               addToStateMessage("写入成功!",false);
               }
               break;
           case 5:
               if (LightSignInt == 10 || LightSignInt == 30 || LightSignInt == 50)//sss
               {
                   addToStateMessage("无效的选择", true);
               }
               else
               {
                   //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD,address,buffer);
                   InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 6, address, valueToWrite);
                   addToStateMessage("写入成功!", false);
               }
               break;
           default:
               QMessageBox::warning(this,"错误","无效的选择");
               return;

           }
           //单板
           //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(address, buffer);//West
       }
       //QMessageBox::information(this,"提示","写入成功");

}

void MainWindow::on_pushButton_7_clicked()//read
{

    int index2 = ui->comboBox_5->currentIndex();
    //cout<<"index2"<<index2<<endl;

    QStringList addressList = ui->AddEdit->toPlainText().split('\n');
    QStringList readValues;

        for(const QString& addrStr : addressList)
        {
            bool ok;
            uint32_t address = addrStr.toUInt(&ok, 16);
            //address = address/2;
            
            cout<<"address:"<<address<<endl;
            if(!ok)
            {
                QMessageBox::warning(this, "错误", QString("地址%1无效！").arg(addrStr));
                return;
            }

            //uint16_t buffer[2];
            uint32_t buffer;

            //选板
            switch(index2)//dwg
            {
            case 0:
               // Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,address,buffer);
                InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 1, address, buffer);
                break;
            case 1:
                if(LightSignInt ==10)//sss
                {
                    addToStateMessage("无效的选择",true);

                }
                else
                {
                InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 2, address, buffer);
                //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD,address,buffer);
                addToStateMessage("读取完毕!",false);
                }
                break;
            case 2:
                if(LightSignInt ==10)//sss
                {
                    addToStateMessage("无效的选择",true);

                }
                else
                {
                InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 3, address, buffer);
               // Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD,address,buffer);
                addToStateMessage("读取完毕!",false);
                }
                break;
            case 3:
                if(LightSignInt ==10 || LightSignInt ==30)//sss
                {
                    addToStateMessage("无效的选择",true);
                }
                else
                {
                InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 4, address, buffer);
                //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FOURTHBOARD,address,buffer);
                addToStateMessage("读取完毕!",false);
                }
                break;
            case 4:
                if(LightSignInt ==10 || LightSignInt ==30)//sss
                {
                    addToStateMessage("无效的选择",true);
                }
                else
                {
                InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 5, address, buffer);
               // Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIFTHBOARD,address,buffer);
                addToStateMessage("读取完毕!",false);
                }
                break;
            case 5:
                if (LightSignInt == 10 || LightSignInt == 30 || LightSignInt == 50)//sss
                {
                    addToStateMessage("无效的选择", true);
                }
                else
                {
                    InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 6, address, buffer);
                    // Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIFTHBOARD,address,buffer);
                    addToStateMessage("读取完毕!", false);
                }
                break;
            default:
                QMessageBox::warning(this,"错误","无效的选择");
                return;

            }

            // 初始化操作只需要执行一次
            //LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");//glass
            //Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);//glass

            //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(address, buffer);//West

            //uint32_t valueRead = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
            readValues.append(QString::number(buffer, 16).toUpper());
            //qDebug()<<"[read buffer[0&1]]"<<buffer[0]<<buffer[1]<<endl;
            //qDebug()<<valueRead;
        }
        //QMessageBox::information(this,"提示","读取完毕");
        //addToStateMessage("读取完毕!",false);

        // 将所有读取到的值显示在textEdit_3中
        ui->NumEdit->setText(readValues.join('\n'));
}

//=========================================================================10101  FPGA温度 10.10


 void MainWindow::ReadFpgaTem()
 {
     uint16_t buffer[2];
     double FpgaTem2 = 0;

     double totalTemperature;
 
     int abb = InsPMDL_GetFpgaTemp(deviceHandles.lightHandle,&FpgaTem2);//Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0012, buffer);//West
     InsPMDL_GetBoardTemp(deviceHandles.lightHandle, &totalTemperature);
     qDebug()<<"abb:"<<abb<<endl;

     if(abb == 10086)//10086 超时错误码
     {
         TimeOutCount++;

         addToStateMessage("TimeOut:"+QString::number(TimeOutCount),false);
         InsPMDL_InitCommunication(InsSystemHandle);
         abb = InsPMDL_GetFpgaTemp(deviceHandles.lightHandle, &FpgaTem2);//Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0012, buffer);//West

         /*
         Ins_Modbus_udp::get_instance()->initmodbus_udp(DEVICE::get_instance()->ip_addr,502);

         Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0012, buffer);

         uint32_t FpgaTem = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);

         double FpgaTem2 = ((FpgaTem*503.975)/4096) -273.15;
*/
         ui->label_76->setText(QString::number(0, 'f', 2));
         ui->label_78->setText(QString::number(0, 'f', 2));
         abb = 0;
         addToStateMessage("通信错误、请检查连接是否正常", true);
     }
     else if(abb == 0)
     {
         //uint32_t FpgaTem = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
         //double FpgaTem2 = ((FpgaTem*503.975)/4096) -273.15;
         ui->label_76->setText(QString::number(FpgaTem2,10,2));
         ui->label_78->setText(QString::number(totalTemperature, 'f', 2));

         if (totalTemperature + FpgaTem2 > 180)//0416
         {
             addToStateMessage("温度过高,自动停止取像", true);

             InsPMDL_GrabStop(InsSystemHandle);
             InsPMDL_GrabStop(InsSystemHandle);
             InsPMDL_GrabStop(InsSystemHandle);

         }

     }

 }

 void MainWindow::ReadBoardTem()  //主板温度 10.10
 {
     double totalTemperature;
     InsPMDL_GetBoardTemp(InsSystemHandle,&totalTemperature);
    /* uint16_t buffer[2];

     Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0900, buffer);//West

     uint32_t BoardTem = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);

     //12位0正1负  5-11位为整数位  1-4小数位  小数位需*0.0625

     BoardTem &= 0x0FFF;

     bool isNegative = BoardTem & (1 << 11);

     int integerPart = (BoardTem >> 4) & 0x7F;

     double decimalPart = (BoardTem & 0x0F) * 0.0625;

     double totalTemperature = integerPart + decimalPart;
     if (isNegative)
     {
         totalTemperature = -totalTemperature;
     }
*/
     ui->label_78->setText(QString::number(totalTemperature, 'f', 2));
 }

 void MainWindow::ReadCamTem()
 {
  /*
     uint16_t buffer[2];
     // 初始化操作只需要执行一次
     LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");
     Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);

     Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x150C, buffer);

     uint32_t FpgaTem = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);

     double FpgaTem2 = ((FpgaTem*503.975)/4096) -273.15;

     ui->label_76->setText(QString::number(FpgaTem2,10,2));
   */
 }
//=========================================Pose1010=========================TEST
 struct RegisterInfoWrite
 {
     uint32_t address;
     uint32_t value;
 };

 void  MainWindow::writeRegisters()
 {
     // 定义要写入的寄存器信息
      RegisterInfoWrite writeCommands[] = {

         {0x1300, 1},
         {0x1308, 107},
         {0x130C, 1},
         {0x1314, 1},

         {0x1300, 1},
         {0x1308, 108},
         {0x130C, 0},
         {0x1314, 1},

         {0x1300, 1},
         {0x1308, 25},
         {0x130C, 9},
         {0x1314, 1},

         {0x1300, 1},
         {0x1308, 26},
         {0x130C, 6},
         {0x1314, 1},

         {0x1300, 1},
         {0x1308, 27},
         {0x130C, 24},
         {0x1314, 1},

         {0x1300, 1},
         {0x1308, 28},
         {0x130C, 24},
         {0x1314, 1}
     };


     //LOCALDEVICE::get_instance()->init("192.168.1.2","255.255.255.0","192.168.1.1");
     //Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11",502);


     for (const RegisterInfoWrite &command : writeCommands)
     {
         uint16_t highValue = (command.value >> 16) & 0xFFFF;
         uint16_t lowValue = command.value & 0xFFFF;
         uint16_t buffer[2] = {highValue, lowValue};
         //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(command.address, buffer);//West
     }
 }

 //==================readpose

 struct RegisterInfoRead
 {
     uint32_t address;
     uint32_t value;
 };

 void MainWindow::ReadPose()
 {
//before read fpga
 }


 //============================ABConverse====================================1012
void MainWindow::on_pushButton_3_clicked()
{
    //ins_encodeVerify *encodeVer = new ins_encodeVerify(this);
    //encodeVer->setWindowTitle("编码器校验");  // 设置窗口的标题
    //connect(encodeVer, &ins_encodeVerify::finished, encodeVer, &ins_encodeVerify::deleteLater);
    //encodeVer->show();
    //insencode
    //insencode = new ins_encodeVerify();

    timer11->start(1000);
    insencode = new ins_encodeVerify();//1013
    //connect(insencode,SIGNAL(ABstatechange()),this,SLOT(ABconverse()));//1012  //0321
    //connect(insencode,SIGNAL(ABstatechange2()),this,SLOT(ABconverse2()));//1012
    insencode->setWindowTitle("编码器校验");  // 设置窗口的标题
    //insencode->setWindowTitle("Encode Verify");//EN2CH
    connect(insencode, &ins_encodeVerify::finished, insencode, &ins_encodeVerify::deleteLater);
    insencode->show();

    /*uint16_t bufferEncode[2] ={};//1013
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1644,bufferEncode);
    uint32_t combinValue2 = (static_cast<uint32_t>(bufferEncode[0]) <<16) | bufferEncode[1];
    EncodeRead1.sprintf("%d",combinValue2);

    uint16_t bufferEncode2[2] ={};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1650,bufferEncode2);
    uint32_t combinValue3 = (static_cast<uint32_t>(bufferEncode2[0]) <<16) | bufferEncode2[1];
    EncodeRead2.sprintf("%d",combinValue3);

    uint16_t bufferEncode3[2] ={};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1654,bufferEncode3);
    uint32_t combinValue4 = (static_cast<uint32_t>(bufferEncode3[0]) <<16) | bufferEncode3[1];
    EncodeRead3.sprintf("%d",combinValue4);*/
}


//===============================1012==============================================

void MainWindow::ABconverse()
{
    InsPMDL_SetABPhaseInversionOnOff(InsSystemHandle,On);//01.11
    /*
    uint32_t AB1 = 1 ;//A&B

    uint16_t AB1H = (AB1 >> 16) & 0xFFFF;
    uint16_t AB1L = AB1 & 0xFFFF;
    uint16_t buffer[2] = {AB1H,AB1L};
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x162C,buffer);//West
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B16,buffer);
*/
}



void MainWindow::ABconverse2()
{
    InsPMDL_SetABPhaseInversionOnOff(InsSystemHandle,Off);
    /*
    uint32_t AB2 = 0 ;//A&B

    uint16_t AB2H = (AB2 >> 16) & 0xFFFF;
    uint16_t AB2L = AB2 & 0xFFFF;
    uint16_t buffer[2] = {AB2H,AB2L};
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(0x162C,buffer);//West
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B16,buffer);
*/
}

void MainWindow::ABencodePose() //0319aaaaa
{
    
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1644,bufferEncode);//West
    uint32_t combinValue2;
    InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1644, combinValue2);
    EncodeRead1.sprintf("%d",combinValue2);
    qDebug()<<"EncodeRead1-----------------:"<<EncodeRead1<<endl;
    qDebug()<<"EncodeRead1-----------------:"<<combinValue2<<endl;

    //uint16_t bufferEncode2[2] ={};
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1650,bufferEncode2);//West
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0B28,bufferEncode2);
    uint32_t combinValue3;
    InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1650, combinValue3);
    EncodeRead2.sprintf("%d",combinValue3);
    qDebug()<<"EncodeRead2-----------------:"<<EncodeRead2<<endl;
    qDebug()<<"EncodeRead2-----------------:"<<combinValue3<<endl;

    //uint16_t bufferEncode3[2] ={};
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(0x1654,bufferEncode3);//West
    //Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD,0x0B2A,bufferEncode3);
    uint32_t combinValue4;
    InsPMDL_ReadRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1654, combinValue4);//
    EncodeRead3.sprintf("%d",combinValue4);
    qDebug()<<"EncodeRead3-----------------:"<<EncodeRead3<<endl;
    qDebug()<<"EncodeRead3-----------------:"<<combinValue4<<endl;
}

void MainWindow::new_titlebar_init()
{
    //禁止用户隐藏工具栏
    //this->setContextMenuPolicy(Qt::PreventContextMenu);

    // 隐藏原有标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 创建自定义标题栏
    INS_TitleBar *myTitleBar = new INS_TitleBar(this);

    myTitleBar->titleLabel->setText(u8"苏映视相偏式2.5D线扫系统_v2.0.4(GPU加速测试版)");//版本
    //myTitleBar->titleLabel->setText(u8"INSNEX_25D_PMDL_v1.0.2");//版本

    QMenuBar *menuBar = this->ui->menubar; // 获取菜单栏
    myTitleBar->layout_menubar->addWidget(menuBar);
    myTitleBar->layoutV->addLayout(myTitleBar->layout_menubar);
    myTitleBar->setGeometry(0, 0, width(), ui->menubar->height() + 50);


    this->setMenuWidget(myTitleBar);
    ui->statusbar->setSizeGripEnabled(true);

    //// 自定义标题栏的信号连接到相应的槽函数
    QObject::connect(myTitleBar, &INS_TitleBar::minimizeClicked, this, &QMainWindow::showMinimized);
    QObject::connect(myTitleBar, &INS_TitleBar::maximizeOrRestoreClicked, [=](){
        if (this->isMaximized()) {
            this->showNormal();
            qDebug() << "showNormal" << this->windowState();
        } else {
            //在Qt 5.14版本之后，QDesktopWidget已经被弃用
            //QRect desktopRect = QApplication::desktop()->availableGeometry(); // 获取屏幕的工作区尺寸
            QScreen *primaryScreen = QGuiApplication::primaryScreen();
            QRect desktopRect = primaryScreen->availableGeometry(); // 获取主屏幕的工作区尺寸

            QRect windowRect = this->geometry(); // 获取mainwindow窗口的几何信息
            if (windowRect.size() == desktopRect.size()) {
                this->setGeometry(desktopRect);
            }
            myTitleBar->setNormalwidth(this->geometry().width());
            this->showMaximized();
            qDebug() << "showMaximized"<< this->windowState();
        }
    });
    QObject::connect(myTitleBar, &INS_TitleBar::closeClicked, this, &QApplication::quit);

}



void MainWindow::on_lineEdit_9_returnPressed()//wbg
{
    uint32_t TriNum1 = ui->lineEdit_9->text().toInt();
    InsPMDL_SetSoftTriggerNum(InsSystemHandle, TriNum1);
}


void MainWindow::on_lineEdit_10_returnPressed()
{
    double Num2 = ui->lineEdit_10->text().toDouble();
    InsPMDL_SetSoftTriggerPeriod(InsSystemHandle, Num2);
/*
    uint32_t Num3 = static_cast<uint32_t>(Num2*1000);

    Num3 /= 8;

    uint16_t highValue = (Num3>> 16) & 0xFFFF;
    uint16_t lowValue = Num3 & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B00,buffer);*/
}
//===============================UpDate IP 1027=========================================


/*
  void MainWindow::on_UpdateIpLine_returnPressed()
{
    INSCP UDipM;

    UDipM.search_device();
    QString qnewIpM = ui->UpdateIpLine->text();

    std::string newIpM = qnewIpM.toStdString();



    const char* newIp2M = newIpM.c_str();

    bool success  = UDipM.ins_reset_device_id(newIp2M);

    std::cout<<"[here]"<<DEVICE::get_instance()->ip<<std::endl;

    std::cout<<newIpM<<std::endl;


    if(!success)
    {
        addToStateMessage("IP更新成功", false);

    }

    else if(success == -1)
    {
        addToStateMessage("IP格式错误，请重新输入", true);

    }
    else
    {
        addToStateMessage("IP更新失败，请重试", true);

    }
}*/

void MainWindow::on_lineEdit_7_returnPressed()
{
    int MinWidth = ui->lineEdit_7->text().toInt();
    InsPMDL_SetShaftEncoder1MinWidth(InsSystemHandle,MinWidth);

    int ErroMin = InsPMDL_GetShaftEncoder1MinWidth(InsSystemHandle);
    if(MinWidth == ErroMin)
    {
        addToStateMessage("触发最小值设置成功",false);
    }
    else
    {
        addToStateMessage("触发最小值设置失败",true);
    }
    qDebug()<<ErroMin;
}


void MainWindow::on_insName_10_textChanged(const QString &arg1)
{
    //QPixmap pixmapGreen(":/icon/Green20.png");
    //ui->label_12->setPixmap(pixmapGreen);
}
//---------===============红外------------============1115

void MainWindow::on_ExNumUI_returnPressed()//触发数量
{
    uint32_t ExNum = ui->ExNumUI->text().toInt();
    InsResult ret = InsPMDL_SetExtendedTriggerOutputQuantity(InsSystemHandle, ExNum);//0108
    if ((int)ret == 0)
    {
        addToStateMessage("触发数量输入成功", false);
    }
}


void MainWindow::on_ChuFaCombox_2_currentIndexChanged(int index)//输出极性
{
    /*if (index == 0)
       {
           uint32_t valueToWrite = 1;//上升沿
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer[2] = {highValue,lowValue};

           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C02,buffer);//combine
           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C02,buffer);//1804
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C02,buffer);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C02,buffer );
           }
           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x0C02,valueToWrite);
           }


       }
       else if(index == 1)
       {
           uint32_t valueToWrite =  0;
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer2[2] = {highValue,lowValue};

           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C02,buffer2);//combine
           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C02,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C02,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C02,buffer2);
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x0C02,valueToWrite);
           }
       }
*/
    InsPMDL_SetExtendedTriggerOutputPolarity(InsSystemHandle, index);//0108
}


void MainWindow::on_ExDelayUI_returnPressed()//触发延迟
{
   double ExGap2 = ui->ExDelayUI->text().toDouble();
   InsPMDL_SetExtendedTriggerOutputDelay(InsSystemHandle, ExGap2);//0108

}


void MainWindow::on_ExWidthUI_returnPressed()
{
    //180C
    double ExWidth2 = ui->ExWidthUI->text().toDouble();
    InsPMDL_SetExtendedTriggerOutputWidth(InsSystemHandle, ExWidth2);//0108
/*  uint32_t ExWidth = ExWidth2;

    ExWidth =ExWidth*1000/8;

    uint16_t ExWidthH = (ExWidth >> 16) & 0xFFFF;
    uint16_t ExWidthL = ExWidth & 0xFFFF;
    uint16_t buffer1[2] = {ExWidthH,ExWidthL};

    if(LightSignInt == 10)
    {
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C06,buffer1);//combine
    }
    else if(LightSignInt == 30)//3
    {

    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C06,buffer1);//180C
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C06,buffer1);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C06,buffer1 );

    }
    else if(LightSignInt == 50)
    {
    writeToModbusFive(0x0C06,ExWidth);
    }*/  
}




void MainWindow::on_CamTriUI_returnPressed()//光源触发数量
{
    uint32_t CamNum = ui->ExNumUI->text().toInt()+8;//1814
    InsPMDL_SetLightSourceTriggerOutputQuantity(InsSystemHandle, CamNum);//0108
    /*
    uint16_t highValue = (CamNum >> 16) & 0xFFFF;
    uint16_t lowValue = CamNum & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    if(LightSignInt == 10)
    {
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C0A,buffer);//combine
    }
    else if(LightSignInt == 30)//3
    {

    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C0A,buffer);//1814
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C0A,buffer);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C0A,buffer );
    }
    else if(LightSignInt == 50)
    {
    writeToModbusFive(0x0C0A,CamNum);
    }
    */
}



void MainWindow::on_lineEdit_8_returnPressed()//1116光源响应时间
{
   /*
    double LightDelay = ui->lineEdit_8->text().toDouble();//17AC
    set_light_source_response_time
    uint32_t  LightDelay2 = LightDelay * 1000;

    LightDelay2 /= 8;

    if(LightDelay2 > 0)
    {
        uint16_t highValue = (LightDelay2 >> 16) & 0xFFFF;
        uint16_t lowValue = LightDelay2 & 0xFFFF;


        uint16_t buffer[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0BD6,buffer);//combine

        addToStateMessage("光源响应时间输入成功",false);
        }
        else if(LightSignInt == 30)
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1020,buffer);//17AC->2040  date:1206
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1020,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1020,buffer );

        addToStateMessage("光源响应时间输入成功",false);
        }
    }
    else
    {
        addToStateMessage("光源响应时间输入须大于0",true);
    }
    */

}


void MainWindow::on_lineEdit_11_returnPressed()
{
     /*double LightWidth2 = ui->lineEdit_11->text().toDouble()*1000;//光源触发宽度
     double LightWidthFre2 = ui->lineEdit_6->text().toDouble()*1000;//行扫描周期 做判断

     uint32_t LightWidth = LightWidth2;
     uint32_t LightWidthFre = LightWidthFre2;


     if(LightWidth>LightWidthFre/2)
     {
         addToStateMessage("输入值过大",true);
     }
     else
     {
         LightWidth /= 8 ;

         uint16_t highValue = (LightWidth >> 16) & 0xFFFF;
         uint16_t lowValue = LightWidth & 0xFFFF;


         uint16_t buffer[2] = {highValue,lowValue};

         if(LightSignInt == 10)
         {
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B8C,buffer);//combine
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0BBC,buffer);//combine

         }
         else if(LightSignInt == 30)
         {

         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B8C,buffer);//1718
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B8C,buffer);
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B8C,buffer);

         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0BBC,buffer);//1778
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0BBC,buffer);
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0BBC,buffer);
         }

         LightWidthFre -= 1000;
         LightWidthFre /=8;

         uint16_t highValue2 = (LightWidthFre >> 16) & 0xFFFF;
         uint16_t lowValue2 = LightWidthFre & 0xFFFF;


         uint16_t buffer2[2] = {highValue2,lowValue2};

         if(LightSignInt == 10)
         {
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B84,buffer2);//combine

         }
         else if(LightSignInt == 30)
         {

         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B84,buffer2);//1708
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B84,buffer2);
         Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B84,buffer2);
         }

         addToStateMessage("光源触发宽度输入成功",false);

     }*/

}
//======================================1117============================================================

void MainWindow::on_LightMode3_currentIndexChanged(int index)//x方向
{
    InsPMDL_SetXStripeMovementDirection(InsSystemHandle, index);//0108
    /*if (index == 0)
       {
           uint32_t valueToWrite = 0;//正向
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1014,buffer);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1014,buffer);//2028
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1014,buffer);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1014,buffer );
           }
           else if(LightSignInt == 50)
           {
           writeToModbusFive(0x1014,valueToWrite);
           }

       }
       else if(index == 1)
       {
           uint32_t valueToWrite =  1;//反向
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer2[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1014,buffer2);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1014,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1014,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1014,buffer2);
           }

           else if(LightSignInt == 50)
           {
           writeToModbusFive(0x1014,valueToWrite);
           }
       }*/
}


void MainWindow::on_LightMode_5_currentIndexChanged(int index)
{
    InsPMDL_SetYStripeMovementDirection(InsSystemHandle, index);//0108
    /*
    if(index == 0)
       {
           uint32_t valueToWrite = 0;//正向
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1016,buffer);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1016,buffer);//202C
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1016,buffer);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1016,buffer );
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1016,valueToWrite);
           }

       }
       else if(index == 1)
       {
           uint32_t valueToWrite =  1;//反向
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer2[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1016,buffer2);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1016,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1016,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1016,buffer2);
           }
           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1016,valueToWrite);
           }
       }
       */
}


void MainWindow::on_LightMode_4_currentIndexChanged(int index)
{
    InsPMDL_SetInitialPhaseInXDirection(InsSystemHandle, index);//0108
    /*if (index == 0)
       {
           uint32_t valueToWrite = 0;
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer);//2000
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1000,buffer);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1000,buffer );
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1000,valueToWrite);
           }

       }
       else if(index == 1)//90
       {
           uint32_t valueToWrite =  72;
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer2[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer2);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer2);//2000
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1000,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1000,buffer2);
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1000,valueToWrite);
           }
       }
    else if(index == 2)//180
    {
        uint32_t valueToWrite =  144;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer3[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer3);//combine

        }
        else if(LightSignInt == 30)//3
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer3);//2000
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1000,buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1000,buffer3);
        }
        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x1000,valueToWrite);
        }
    }
    else if(index == 3)//270
    {
        uint32_t valueToWrite =  216;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer4[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer4);//combine

        }
        else if(LightSignInt == 30)//3
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1000,buffer4);//2000
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1000,buffer4);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1000,buffer4);
        }
        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x1000,valueToWrite);
        }
    }
    */
}


void MainWindow::on_LightMode_6_currentIndexChanged(int index)
{
    InsPMDL_SetInitialPhaseInYDirection(InsSystemHandle, index);//0108
    /*
    if(index == 0)
       {
           uint32_t valueToWrite = 0;
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer);//2070
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1038,buffer);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1038,buffer );
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1038,valueToWrite);
           }

       }
       else if(index == 1)//90
       {
           uint32_t valueToWrite =  72;
           uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
           uint16_t lowValue = valueToWrite & 0xFFFF;


           uint16_t buffer2[2] = {highValue,lowValue};
           if(LightSignInt == 10)
           {
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer2);//combine

           }
           else if(LightSignInt == 30)//3
           {

           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer2);//2070
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1038,buffer2);
           Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1038,buffer2);
           }

           else if(LightSignInt == 50)//3
           {
           writeToModbusFive(0x1038,valueToWrite);
           }
       }
    else if(index == 2)//180
    {
        uint32_t valueToWrite =  144;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer3[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer3);//combine

        }
        else if(LightSignInt == 30)
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer3);//2070
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1038,buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1038,buffer3);
        }

        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x1038,valueToWrite);
        }
    }
    else if(index == 3)//270
    {
        uint32_t valueToWrite =  216;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer4[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer4);//combine

        }
        else if(LightSignInt == 30)
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x1038,buffer4);//2070
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x1038,buffer4);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x1038,buffer4);
        }
        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x1038,valueToWrite);
        }
    }
    */
}





//======================================1124============================

//系统信息
QString MainWindow::generateSystemModel(QString cameraModel, QString lightSourceModel)
{
    //相机
    int start = cameraModel.indexOf("INS-LC-") + 7;
    int end = cameraModel.indexOf('-', start);
    QString cameraKeyPart = cameraModel.mid(start, end - start);

    //光源
    QString lightSourceKeyPart = lightSourceModel.section('-', -1).remove(QRegExp("[^0-9]"));

    //系统
    QString systemModel = "INS-PMDL-25D-" + cameraKeyPart + lightSourceKeyPart+"M";

    return systemModel;
}


//============================配置文件======================================

void MainWindow::saveConfigure()//保存
  {
      QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                      "/home/INS_ConfigureFile.icf", // 默认路径和文件名
                                                      tr("Text Files (*.icf);;All Files (*)"));
      if (!filePath.isEmpty())
      {

          InsResult camJudge;

          int  LightJudge;
          std::string filePath2= filePath.toStdString();

          const char* c_filepath2 = filePath2.c_str();

          qDebug()<<"save filePath :"<<c_filepath2<<endl;


          QString LightLength = DEVICE::get_instance()->length;

          int Length = LightLength.toInt();

          char* filepath2 = const_cast<char*>(c_filepath2);

          camJudge = InsPMDL_SaveSystemConfigFile(InsSystemHandle,c_filepath2 );
          /*
          camJudge = camcam.SaveConfigureFileFromeFrameGrabber(filepath2);

          if(camJudge == Errc::INSSTATUS_SUCCESS)

          {

              addToStateMessage("相机配置保存成功",false);
          }
          else
          {
              addToStateMessage("相机配置保存失败",true);
          }


          LightJudge = save_config_file(Length/10,c_filepath2);//Length/10
*/
          QString tempRet = QString::number((int)camJudge);
          //if(LightJudge == 0)
          //{
          //    addToStateMessage("配置保存成功",false);
          //}
          //else
          //{
          //    addToStateMessage("配置保存失败"+ tempRet,true);
          //}
      }
}

  void MainWindow::loadConfigure()//加载
  {
      QString fileName = QFileDialog::getOpenFileName(this, tr("Open Config File"), "", tr("Config Files (*.icf)"));
      if (!fileName.isEmpty())
      {
      // 将QString转换为const char*
      std::string filepath = fileName.toStdString();

      const char* c_filepath = filepath.c_str();

      qDebug()<<c_filepath;

      InsResult camJudge;

      int  LightJudge;

      QString LightLength = DEVICE::get_instance()->length;

      int Length = LightLength.toInt();


      const char* filepath2 = const_cast<char*>(c_filepath);

      camJudge = InsPMDL_LoadConfigureFileToSystem(InsSystemHandle,filepath2);

      QApplication::processEvents();
      

      if (camJudge == InsResult::INSCSTATUS_OK)
      {
          addToStateMessage("配置加载成功", false);
      }
      else
      {
          addToStateMessage("配置加载失败,请重新加载！", true);
      }

      /*InsPMDL_ConfigureFrameGrabber(mpCamera);

      LightJudge = load_config_file(Length/10, c_filepath);//Length/10

      if(camJudge == InsResult::INSSTATUS_SUCCESS)
      {
          addToStateMessage("相机配置加载成功",false);
      }
      else
      {
          addToStateMessage("相机配置加载失败",true);
      }

      if(LightJudge == 0)
      {

          addToStateMessage("光源配置加载成功",false);
      }
      else
      {
          addToStateMessage("光源配置加载失败",true);
      }
*/
      }

      ui->insInterface_8->setText(QString::number(InsPMDL_GetHeight(InsSystemHandle)/8));//行高
      ui->insInterface_5->setText(QString::number(InsPMDL_GetExposure(InsSystemHandle), 'f', 2));//曝光
      ui->insInterface_6->setText(QString::number(InsPMDL_GetDigitalGain(InsSystemHandle)));//数字增益

     //0412模拟增益

      char* PACGain = (char*)malloc(4);
      InsPMDL_GetPacGain(InsSystemHandle, PACGain);//0412
      if (strcmp(PACGain, "x1") == 0)
      {
          ui->comboBox_4->setCurrentIndex(0);
          // ui->comboBox_4->addItem("1111");
      }
      else if (strcmp(PACGain, "x2") == 0)
      {
          ui->comboBox_4->setCurrentIndex(1);
          // ui->comboBox_4->addItem("2222");
      }
      else if (strcmp(PACGain, "x5") == 0 || strcmp(PACGain, "x4") == 0)//0418配置文件
      {
          if (ui->insName->text().contains("8KCXP"))
          {
              ui->comboBox_4->setCurrentIndex(3);
          }
          else if (ui->insName->text().contains("16KCXP"))
          {
              ui->comboBox_4->setCurrentIndex(2);
          }
          else //4K CL
          {
              ui->comboBox_4->setCurrentIndex(2);
          }

      }
      else if (strcmp(PACGain, "x10") == 0 || strcmp(PACGain, "x8") == 0)
      {
          ui->comboBox_4->setCurrentIndex(3);
      }
      else
      {
          ui->comboBox_4->setCurrentIndex(2);
          //addToStateMessage("PACGain False",true);
      }
      //wwww

      //===================行程方向============================
      uint32_t Encode2 = 0;//1634
      InsPMDL_GetTriggerDirection(InsSystemHandle, Encode2);//0105
      if(Encode2 == 0)//单项行程
          {
          ui->ChuFaCombox->setCurrentIndex(0);
          addToStateMessage("当前行程方向为 单向行程",false);
          }
      else if(Encode2 == 1)//回退跟踪
          {
          ui->ChuFaCombox->setCurrentIndex(1);
          addToStateMessage("当前行程方向为 回退跟踪",false);
          }
      else if(Encode2 == 2)//回退忽略
          {
          ui->ChuFaCombox->setCurrentIndex(2);
          addToStateMessage("当前行程方向为 回退忽略",false);
          }
      else if(Encode2 == 3)//双向扫描
          {
          ui->ChuFaCombox->setCurrentIndex(3);
          addToStateMessage("当前行程方向为 双向扫描",false);
          }
      else
          {
          ui->ChuFaCombox->setCurrentIndex(0);
          addToStateMessage("初始化失败，请重新选择行程方向",true);
          }



      //================触发模式================================

      //uint32_t Encode1 = readFromModbusOne(0x0B18);
      uint32_t Encode1 = 0;//0105
      InsPMDL_GetEncoderTriggerMode(InsSystemHandle, Encode1);

      if(Encode1 == 0)//A相上升沿 单向计数
          {
          ui->EnCodeTriComBox->setCurrentIndex(0);
          addToStateMessage("当前触发模式为 A相上升沿 单向计数",false);
          }

      else if(Encode1 == 1)//A相上升沿 双向计数
          {
          ui->EnCodeTriComBox->setCurrentIndex(1);
          addToStateMessage("当前触发模式为 A相上升沿 双向计数",false);
          }
      else if(Encode1 == 2)//A相双边沿 双向计数
          {
          ui->EnCodeTriComBox->setCurrentIndex(2);
          addToStateMessage("当前触发模式为 A相双边沿 双向计数",false);
          }
      else if(Encode1 == 3)//A&B 双向
          {
          ui->EnCodeTriComBox->setCurrentIndex(3);
          addToStateMessage("当前触发模式为 A&B双向",false);
          }
      else
          {
          ui->EnCodeTriComBox->setCurrentIndex(0);
          addToStateMessage("初始化失败 请重新选择触发模式",true);
          }

      QApplication::processEvents();
      //================分倍频============================
      /*uint32_t FenPinInitial = readFromModbusOne(0x0B1E);
      uint32_t BeiPinInitial = readFromModbusOne(0x0B26);

      ui->FenPinSpinBox->setValue(FenPinInitial);

      if (BeiPinInitial != 0)
      {
      ui->BeiPinSpinBox->setValue(BeiPinInitial);
      }
      else
      {
      ui->BeiPinSpinBox->setValue(1);
      }*/
      uint32_t FenPinInitial = 0;//0105
      uint32_t BeiPinInitial = 0;
      InsPMDL_GetFrequencyDivision(InsSystemHandle, FenPinInitial);
      InsPMDL_GetFrequencyMultiplier(InsSystemHandle, BeiPinInitial); //0412

      ui->FenPinSpinBox->setValue(FenPinInitial);
      ui->BeiPinSpinBox->setValue(BeiPinInitial+1);
      /*
      uint32_t HangSMFre = readFromModbusOne(0x1022);

      uint32_t HangSMFre2 = HangSMFre * CamTriNumZ;

      HangSMFre2 = HangSMFre2*8/1000;//系统扫描周期

      ui->lineEdit_6->setText(QString::number(HangSMFre2));
*/
      double _line_scanning_period = 0;//0105
      double _line_fre = 0;
      InsPMDL_GetScanningPeriod(InsSystemHandle, _line_scanning_period, _line_fre);
      ui->lineEdit_6->setText(QString::number(_line_scanning_period,'f',2));


      /*
      double Fre2  = static_cast<double>(HangSMFre2);

      double LineFre= 1000000.0/Fre2/1000;*/

      QString str = QString::number(_line_fre, 'f', 2);

      ui->insInterface_9->setText(str);

      //bbbb
      //========================发光模式==============================


      int IniLightMode;
      InsPMDL_GetEmissionMode(InsSystemHandle, IniLightMode);//0316句柄


       if(IniLightMode == 0)
           {
           ui->LightMode->setCurrentIndex(0);
           addToStateMessage("当前发光模式为 极大",false);
           }

       else if(IniLightMode == 1)
           {
           ui->LightMode->setCurrentIndex(1);
           addToStateMessage("当前发光模式为 大",false);
           }
       else if(IniLightMode == 2)
           {
           ui->LightMode->setCurrentIndex(2);
           addToStateMessage("当前发光模式为 标准",false);
           }
       else if(IniLightMode == 3)
           {
           ui->LightMode->setCurrentIndex(3);
           addToStateMessage("当前发光模式为 小",false);
           }
       else if(IniLightMode == 4)
           {
           ui->LightMode->setCurrentIndex(4);
           addToStateMessage("当前发光模式为 极小",false);
           }
       else
           {
           ui->LightMode->setCurrentIndex(0);
           addToStateMessage("发光模式初始化失败 请重新选择",true);
           }
       QApplication::processEvents();
       //0401
  //     uint32_t AngelX = readFromModbusOne(0x1000);//角度

  //     if(AngelX == 0)//0
  //         {
  //         ui->LightMode_4->setCurrentIndex(0);
  //         addToStateMessage("X起始相位 0",false);
  //         }
  //     else if(AngelX == 72)//90
  //         {
  //         ui->LightMode_4->setCurrentIndex(1);
  //         addToStateMessage("X起始相位 90",false);
  //         }
  //     else if(AngelX == 144)//180
  //         {
  //         ui->LightMode_4->setCurrentIndex(2);
  //         addToStateMessage("X起始相位 180",false);
  //         }
  //     else if(AngelX == 216)//270
  //         {
  //         ui->LightMode_4->setCurrentIndex(3);
  //         addToStateMessage("X起始相位 270",false);
  //         }
  //     QApplication::processEvents();

  //     //X正反向
  //     uint32_t DericX = readFromModbusOne(0x1014);
  //     if(DericX == 0)//0
  //         {
  //         ui->LightMode3->setCurrentIndex(0);
  //         addToStateMessage("X条纹移动方向 正向",false);
  //         }

  //     else if(DericX == 1)//90
  //         {
  //         ui->LightMode3->setCurrentIndex(1);
  //         addToStateMessage("X条纹移动方向 反向",false);
  //         }


  //     QApplication::processEvents();
  ////Y方向角度================================================
  //     uint32_t AngelY = readFromModbusOne(0x1038);//角度


  //     if(AngelY == 0)//0
  //         {
  //         ui->LightMode_6->setCurrentIndex(0);
  //         addToStateMessage("Y起始相位 0",false);
  //         }

  //     else if(AngelY == 72)//90
  //         {
  //         ui->LightMode_6->setCurrentIndex(1);
  //         addToStateMessage("Y起始相位 90",false);
  //         }
  //     else if(AngelY == 144)//180
  //         {
  //         ui->LightMode_6->setCurrentIndex(2);
  //         addToStateMessage("Y起始相位 180",false);
  //         }
  //     else if(AngelY == 216)//270
  //         {
  //         ui->LightMode_6->setCurrentIndex(3);
  //         addToStateMessage("Y起始相位 270",false);
  //         }

  //     QApplication::processEvents();
  //     uint32_t DericY = readFromModbusOne(0x1016);
  //     if(DericY == 0)//0
  //         {
  //         ui->LightMode_5->setCurrentIndex(0);
  //         addToStateMessage("Y条纹移动方向 正向",false);
  //         }

  //     else if(DericY == 1)//90
  //         {
  //         ui->LightMode_5->setCurrentIndex(1);
  //         addToStateMessage("Y条纹移动方向 反向",false);
  //         }
  //     QApplication::processEvents();
       //================扩展触发输出极性==============================
       uint32_t IniExternTri;
       InsPMDL_GetExtendedTriggerOutputPolarity(InsSystemHandle, IniExternTri);//wwddjj2

       if(IniExternTri == 0)//上
           {
           ui->ChuFaCombox_2->setCurrentIndex(0);
           addToStateMessage("扩展触发极性  上升沿",false);
           }

       else if(IniExternTri == 1)//下
           {
           ui->ChuFaCombox_2->setCurrentIndex(1);
           addToStateMessage("扩展触发极性  下降沿",false);
           }
       QApplication::processEvents();

       //============扩展触发延迟=====================================//cccc
       //uint32_t IniExternDelay = readFromModbusOne(0x0C04);//延迟
       //IniExternDelay = IniExternDelay*8/1000;
       uint32_t IniExternDelay = 0;
       InsPMDL_GetExtendedTriggerOutputDelay(InsSystemHandle, IniExternDelay);//0108
       ui->ExDelayUI->setText(QString::number(IniExternDelay));
       
       uint32_t IniExternNum = 0;
       InsPMDL_GetExtendedTriggerOutputQuantity(InsSystemHandle, IniExternNum);//0108
       //uint32_t IniExternNum = readFromModbusOne(0x0C00);//数量
       ui->ExNumUI->setText(QString::number(IniExternNum));
       
       uint32_t IniExternWid = 0;
       InsPMDL_GetExtendedTriggerOutputWidth(InsSystemHandle, IniExternWid);//0108
       //uint32_t IniExternWid = readFromModbusOne(0x0C06);//宽度
       //IniExternWid = IniExternWid*8/1000;
       ui->ExWidthUI->setText(QString::number(IniExternWid));
       QApplication::processEvents();
       uint32_t IniCamTri = 0;
       InsPMDL_GetLightSourceTriggerOutputQuantity(InsSystemHandle, IniCamTri);//0108
       //uint32_t IniCamTri = readFromModbusOne(0x0C0A);//光源触发数量
       ui->CamTriUI->setText(QString::number(IniCamTri));
       QApplication::processEvents();
      
 }

 //0327 断电保存

  void MainWindow::saveConfigureFlash()
  {
      //0418
      uint32_t H = ui->insInterface_8->text().toInt();

      ins_save_camera_height(InsSystemHandle, &H);

  
      InsResult e = InsPMDL_PowerOffSave(InsSystemHandle);//0109
      int f = static_cast<int>(e);
      if ((e == InsResult::INSCSTATUS_OK) || (e == InsResult::INSSTATUS_SUCCESS))
      {
          addToStateMessage("配置保存成功", false);
      }
      else
      {
          //addToStateMessage("配置保存失败" + QString::number(f), true);
      }

  }





void MainWindow::on_checkBox_SP2_2_stateChanged(int arg1)//1124
{
    if (ui->checkBox_SP2_2->isChecked())
    {
        ui->checkBox_IR->setEnabled(true);
        PicNumCount = 9;//122089
        uint32_t Num = ui->ExNumUI->text().toInt();
        InsPMDL_ExtendedTriggerEnableAndDisable(InsSystemHandle,1, Num);
        /*
        uint32_t NumNine = 8 + ui->ExNumUI->text().toInt();

        uint32_t valueToWrite =  1;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer2[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C08,buffer2);//combine
        writeToModbusOne(0xC0A,NumNine);

        }
        else if(LightSignInt == 30)//3
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C08,buffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C08,buffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C08,buffer2);
        writeToModbusThree(0xC0A,NumNine);
        }

        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x0C08,valueToWrite);
        }

        qDebug()<<"Write 1 to 0C08"<<endl;*/
    }
    else
    {
        ui->checkBox_IR->setEnabled(false);
        PicNumCount = 8;//122089
        InsPMDL_ExtendedTriggerEnableAndDisable(InsSystemHandle, 0, 0);

        /*uint32_t NumEight = 8;

        uint32_t valueToWrite = 0;
        uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
        uint16_t lowValue = valueToWrite & 0xFFFF;


        uint16_t buffer[2] = {highValue,lowValue};
        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C08,buffer);//combine
        writeToModbusOne(0xC0A,NumEight);

        }
        else if(LightSignInt == 30)//3
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0C08,buffer);//1810
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0C08,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0C08,buffer );
        writeToModbusThree(0xC0A,NumEight);
        }

        else if(LightSignInt == 50)//3
        {
        writeToModbusFive(0x0C08,valueToWrite);
        }

        qDebug()<<"Write 0 to 0C08"<<endl;
*/
    }
}


void MainWindow::on_insInterface_4_textChanged(const QString &arg1)
{
    //QPixmap pixmapGreen(":/image/icon/Green20.png");//":/image/icon/Red20.png"
    //ui->label_12->setPixmap(pixmapGreen);//0327
}


void MainWindow::on_insName_4_textChanged()
{
    QPixmap pixmapGreen(":/image/icon/Green20.png");//":/image/icon/Red20.png"
    ui->label_12->setPixmap(pixmapGreen);//0327

}

void MainWindow::on_lineEdit_12_returnPressed()
{
    //1640
     double EncodeWidth = ui->lineEdit_12->text().toDouble();//1640
     InsResult ret = InsPMDL_SetEncoderTriggerPulseWidth(InsSystemHandle, EncodeWidth);//0108
     if  ((int)ret ==0 )
     { 
        addToStateMessage("编码器触发脉宽输入成功", false);
     }                                                                             /*
     uint32_t EncodeWidth2 = EncodeWidth * 1000 /8;

     uint16_t highValue = (EncodeWidth2 >> 16) & 0xFFFF;
     uint16_t lowValue = EncodeWidth2 & 0xFFFF;


     uint16_t buffer[2] = {highValue,lowValue};
     if(LightSignInt == 10)
     {
     Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B20,buffer);//combine

     addToStateMessage("编码器触发脉宽输入成功",false);
     }
     else if(LightSignInt == 30)
     {
     Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B20,buffer);//1640
     Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B20,buffer);
     Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B20,buffer );

     addToStateMessage("编码器触发脉宽输入成功",false);
     }

     else if(LightSignInt == 50)//3
     {
     writeToModbusFive(0x0B20,EncodeWidth2);
     addToStateMessage("编码器触发脉宽输入成功",false);
     }
*/
}

void MainWindow::on_checkBox_SP2_3_stateChanged(int arg1)//1206
{
    if(ui->checkBox_SP2_3->isChecked())
    {
        uint32_t SecondOne = 1;
        ui->tab_3->setEnabled(true);
        InsPMDL_SecondaryTriggerEnableAndDisable(InsSystemHandle, SecondOne);
        SecJug = true;
    }
    else
    {
        uint32_t SecondZero= 0;
        ui->tab_3->setEnabled(false);
        InsPMDL_SecondaryTriggerEnableAndDisable(InsSystemHandle,SecondZero);//01.10
        SecJug = false;
    }
}

//弃用
void MainWindow::on_lineEdit_18_returnPressed()//亮度 1206
{
    /*uint32_t BrightNess = ui->lineEdit_18->text().toUInt();

    if (BrightNess >=1 && BrightNess <=100)
    {
        uint32_t BrightNessToReg = 256+79*BrightNess-1;

        uint32_t BrightNessOne = 1;

        writeToModbusThree(0x101A,BrightNessToReg);//2034
        writeToModbusThree(0x101C,BrightNessToReg);//2038
        writeToModbusThree(0x1024,BrightNessOne);//2048

        addToStateMessage("亮度输入成功",false);
    }
    else
    {
        addToStateMessage("亮度输入应为整数 , 范围：1~100",true);
    }*/
}

//----------IO触发设置------------
void MainWindow::on_LightMode_3_currentIndexChanged(int index)//0105
{
    InsPMDL_SetIOTriggerMode(InsSystemHandle, index);
}


void MainWindow::on_lineEdit_13_returnPressed()//1触发数量
{
    //1610
    uint32_t TriNum = ui->lineEdit_13->text().toUInt();
    InsPMDL_SetIOTriggerNum(InsSystemHandle, TriNum);//0105
}


void MainWindow::on_lineEdit_16_returnPressed()//1脉冲数量
{
    //1620
    uint32_t PulseNum = ui->lineEdit_16->text().toUInt();
    InsPMDL_SetIOPulseCount(InsSystemHandle, PulseNum);//0105
}


void MainWindow::on_lineEdit_15_returnPressed()//1触发脉宽
{
    //1618  输入*1000/8
    uint32_t TriWidth = ui->lineEdit_15->text().toUInt();
    InsPMDL_SetIOTriggerPulseWidth(InsSystemHandle,TriWidth);//0105

    }

void MainWindow::on_lineEdit_17_returnPressed()//1触发延迟
{
    //161C
    uint32_t TriDelay = ui->lineEdit_17->text().toUInt();
    InsPMDL_SetIOTriggerDelay(InsSystemHandle,TriDelay);//0105
}


void MainWindow::on_lineEdit_14_returnPressed()//1触发周期
{
    //200C  输入*1000/8
    uint32_t TriPeriod = ui->lineEdit_14->text().toUInt();
    InsPMDL_SetIOTriggerPeriod(InsSystemHandle, TriPeriod);//0105
}



void MainWindow::on_LightMode_7_currentIndexChanged(int index)
{
    InsPMDL_SetIOTriggerInputPolarity(InsSystemHandle, index);//0105
}


//=======================================二级触发==========================================1206

void MainWindow::on_lineEdit_23_returnPressed()
{
    double EncodeWidth = ui->lineEdit_23->text().toDouble();//1640
    InsPMDL_SetEncoderSecondaryTriggerPulseWidth(InsSystemHandle, EncodeWidth);//0105
    /*
    uint32_t EncodeWidth2 = EncodeWidth * 1000 /8;


    uint16_t highValue = (EncodeWidth2 >> 16) & 0xFFFF;
    uint16_t lowValue = EncodeWidth2 & 0xFFFF;


    uint16_t buffer[2] = {highValue,lowValue};
    if(LightSignInt == 10)
    {
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B90,buffer);//combine

    addToStateMessage("编码器触发脉宽输入成功",false);
    }
    else if(LightSignInt == 30)
    {

    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B90,buffer);//1640 ->1720
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B90,buffer);
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B90,buffer );

    addToStateMessage("编码器触发脉宽输入成功",false);
    }

    else if(LightSignInt == 50)//3
    {
    writeToModbusFive(0x0B90,EncodeWidth2);
    addToStateMessage("编码器触发脉宽输入成功",false);
    }*/
}


void MainWindow::on_ChuFaCombox_3_currentIndexChanged(int index)
{
    InsPMDL_SetEncoderSecondaryTriggerTravelDirection(InsSystemHandle, index);//0105
    /*
    //============================fpga通信=======================================
    uint32_t valueToWrite = 0;
    uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
    uint16_t lowValue = valueToWrite & 0xFFFF;

    uint32_t valueToWrite1 = 1;
    uint16_t highValue1 = (valueToWrite1 >> 16) & 0xFFFF;
    uint16_t lowValue1 = valueToWrite1 & 0xFFFF;

    uint32_t valueToWrite2 = 2;
    uint16_t highValue2 = (valueToWrite2 >> 16) & 0xFFFF;
    uint16_t lowValue2 = valueToWrite2 & 0xFFFF;

    uint32_t valueToWrite3 = 3;
    uint16_t highValue3 = (valueToWrite3 >> 16) & 0xFFFF;
    uint16_t lowValue3 = valueToWrite3 & 0xFFFF;


 //========================0912  sdk ========================================
    uint16_t buffer[2] = {highValue,lowValue};
    uint16_t buffer1[2] = {highValue1,lowValue1};
    uint16_t buffer2[2] = {highValue2,lowValue2};
    uint16_t buffer3[2] = {highValue3,lowValue3};



    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B6E,buffer);//->16DC

    if (index == 0)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B5A,buffer);//1634->16B4
        qDebug()<<"write 0 to 0x1634";
    }
        else if (index == 1)
        {
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B5A,buffer1);
            qDebug()<<"write 2 to  0x1634";
        }

        else if (index == 2)
        {
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B5A,buffer2);
            qDebug()<<"write 1 to 0x1634";
        }

        else if (index == 3)
        {
            Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B5A,buffer3);
            qDebug()<<"write 3 to 0x1634";
        }
        */
}






void MainWindow::on_FenPinSpinBox_2_valueChanged(int arg1) //0319d //二级触发
{
    InsPMDL_SetSecondaryFrequencyDivision(InsSystemHandle, arg1);//0105

    addToStateMessage("分频—输入成功",false);
}


void MainWindow::on_BeiPinSpinBox_2_valueChanged(int arg1) //二级触发
{
    InsPMDL_SetSecondaryFrequencyMultiplier(InsSystemHandle, arg1);//0105

    addToStateMessage("倍频—输入成功",false);
}


void MainWindow::on_lineEdit_24_returnPressed()
{
    uint32_t TriNum1 = ui->lineEdit_24->text().toInt();
    /*
    uint16_t highValue = (TriNum1>> 16) & 0xFFFF;
    uint16_t lowValue = TriNum1 & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B42,buffer);
    */
    InsPMDL_SetSoftwareTriggerTriggerCount(InsSystemHandle, TriNum1);//0105
}


void MainWindow::on_lineEdit_25_returnPressed()
{
    double Num2 = ui->lineEdit_25->text().toDouble();
    InsPMDL_SetSoftwareTriggerTriggerPeriod(InsSystemHandle, Num2);//0105
    /*
    uint32_t Num3 = static_cast<uint32_t>(Num2*1000);

    uint16_t highValue = (Num3>> 16) & 0xFFFF;
    uint16_t lowValue = Num3 & 0xFFFF;
    uint16_t buffer[2] = {highValue,lowValue};
    Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B40,buffer);
    */
}


void MainWindow::on_LightMode_8_currentIndexChanged(int index)
{
    InsPMDL_SetIoSecondaryTriggerMode(InsSystemHandle, index);//0105
 
}


void MainWindow::on_lineEdit_26_returnPressed()//2触发数量
{
    uint32_t TriNum = ui->lineEdit_26->text().toUInt();
    InsPMDL_SetIoSecondaryTriggerNum(InsSystemHandle, TriNum);//0105
}


void MainWindow::on_lineEdit_27_returnPressed()//2脉冲数量
{
    uint32_t PulseNum = ui->lineEdit_27->text().toUInt();
    InsPMDL_SetIoSecondaryPulseCount(InsSystemHandle, PulseNum);//0105
}


void MainWindow::on_lineEdit_28_returnPressed()//2脉宽
{
    uint32_t TriWidth = ui->lineEdit_28->text().toUInt();
    InsPMDL_SetIoSecondaryTriggerPulseWidth(InsSystemHandle, TriWidth);//0105
    /*
    uint32_t TriWidthToReg = TriWidth*1000/8;
    writeToModbusThree(0x0B4C,TriWidthToReg);*/
}


void MainWindow::on_lineEdit_29_returnPressed()//2延迟
{
    uint32_t TriDelay = ui->lineEdit_29->text().toUInt();
    InsPMDL_SetIoSecondaryTriggerDelay(InsSystemHandle, TriDelay);//0105

    //uint32_t TriDelayToReg = TriDelay*1000/8;
    //writeToModbusThree(0x0B4E,TriDelayToReg);
}


void MainWindow::on_lineEdit_30_returnPressed()//2周期
{
    uint32_t TriPeriod = ui->lineEdit_14->text().toUInt(); 
    InsPMDL_SetIoSecondaryTriggerPeriod(InsSystemHandle, TriPeriod);
    //0105
    //uint32_t TriPeriodToReg = TriPeriod*1000/8;
    //writeToModbusThree(0x1046,TriPeriodToReg);

}


void MainWindow::on_LightMode_14_currentIndexChanged(int index)//反转
{
    InsPMDL_SetIoSecondaryTriggerInputPolarity(InsSystemHandle, index);//0105
    /*if (index == 0)
    {
       writeToModbusThree(0x0B52,Zeroz);
       qDebug()<<"保持"<<endl;

    }
    else if(index == 1)
    {
       writeToModbusThree(0x0B52,Onez);
       qDebug()<<"反转"<<endl;
    }*/
}


void MainWindow::on_LightMode_12_currentIndexChanged(int index)
{
    if(index == 0)
    {
     ui->stackedWidget_2->setCurrentIndex(0);//二级触发===
    }
    if(index == 1)
    {
       ui->stackedWidget_2->setCurrentIndex(1);
    }
    if(index == 2)
    {
       ui->stackedWidget_2->setCurrentIndex(2);
    }
}



//=========================================================12-14===============================================
void MainWindow::on_pushButton_4_clicked()//保存当前配置
{
    InsResult e = InsPMDL_PowerOffSave(InsSystemHandle);//0109
    int f = static_cast<int>(e);
    if ((e == InsResult::INSCSTATUS_OK) || (e == InsResult::INSSTATUS_SUCCESS))
    {
        addToStateMessage("配置保存成功", false);
    }
    else
    {
        addToStateMessage("配置保存失败" + QString::number(f), true);
    }

}


void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
    if(index == 0)
    {
    ui->stackedWidget_3->setCurrentIndex(4);//关
    }
    else if(index == 1)
    {
    ui->stackedWidget_3->setCurrentIndex(5);//光源
    }
    else if(index == 2)
    {
    ui->stackedWidget_3->setCurrentIndex(6);//软触发
    }
    else if(index == 3)
    {
    ui->stackedWidget_3->setCurrentIndex(7);//软触发
    }
}


void MainWindow::on_lineEdit_19_returnPressed()
{
    double LightDelay = ui->lineEdit_19->text().toDouble();//17AC

    if (LightDelay <=0)//0108
    {
        addToStateMessage("光源响应时间输入须大于0", false);
    }
    else{
        InsResult ret = InsPMDL_SetLightSourceResponseTime(InsSystemHandle, LightDelay);//0105
    
        if ((int)ret == 0)
        {
          addToStateMessage("光源响应时间输入成功",false);
        }
    }
}


void MainWindow::on_lineEdit_20_returnPressed()
{
    double LightWidth2 = ui->lineEdit_20->text().toDouble()*1000;//光源触发宽度
    InsResult ret = InsPMDL_SetLightSourceTriggerOutputWidth(InsSystemHandle, LightWidth2);//0108
    if ((int)ret == 0)
    {
        addToStateMessage("光源触发宽度输入成功", false);
    }
    else
        addToStateMessage("光源触发宽度输入失败", true);   
    /*
    double LightWidthFre2 = ui->lineEdit_6->text().toDouble() * 1000;//行扫描周期 做判断

    uint32_t LightWidth = LightWidth2;
    uint32_t LightWidthFre = LightWidthFre2;


    if(LightWidth>LightWidthFre/2)
    {
        addToStateMessage("输入值过大",true);
    }
    else
    {
        LightWidth /= 8 ;

        uint16_t highValue = (LightWidth >> 16) & 0xFFFF;
        uint16_t lowValue = LightWidth & 0xFFFF;


        uint16_t buffer[2] = {highValue,lowValue};

        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B8C,buffer);//combine
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0BBC,buffer);//combine

        }
        else if(LightSignInt == 30)//3
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B8C,buffer);//1718
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B8C,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B8C,buffer);

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0BBC,buffer);//1778
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0BBC,buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0BBC,buffer);
        }
        else if(LightSignInt == 50)
        {
        writeToModbusFive(0x0B8C,LightWidth);
        writeToModbusFive(0x0BBC,LightWidth);
        }

        LightWidthFre -= 1000;
        LightWidthFre /=8;

        uint16_t highValue2 = (LightWidthFre >> 16) & 0xFFFF;
        uint16_t lowValue2 = LightWidthFre & 0xFFFF;


        uint16_t buffer2[2] = {highValue2,lowValue2};

        if(LightSignInt == 10)
        {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B84,buffer2);//combine

        }
        else if(LightSignInt == 30)//3
        {

        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD,0x0B84,buffer2);//1708
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD,0x0B84,buffer2);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD,0x0B84,buffer2);
        }
        else if(LightSignInt == 50)
        {
        writeToModbusFive(0x0B84,LightWidthFre);
        }

        addToStateMessage("光源触发宽度输入成功",false);

    }*/
}


void MainWindow::on_lineEdit_21_returnPressed()
{
    uint32_t BrightNess = ui->lineEdit_21->text().toUInt();
    InsResult ret = InsPMDL_SetLightSourceBrightnessAdjustment(InsSystemHandle, BrightNess);//0108
    if ((int)ret == 0)
    {
        addToStateMessage("光源触发宽度输入成功", false);
    }
    else
        addToStateMessage("光源触发宽度输入失败", true);
    /*
    if (BrightNess >=1 && BrightNess <=100)
    {
        uint32_t BrightNessToReg = 256+79*BrightNess-1;

        uint32_t BrightNessOne = 1;

        writeToModbusThree(0x101A,BrightNessToReg);//2034
        writeToModbusThree(0x101C,BrightNessToReg);//2038
        writeToModbusThree(0x1024,BrightNessOne);//2048

        addToStateMessage("亮度输入成功",false);
    }
    else
    {
        addToStateMessage("亮度输入应为整数 , 范围：1~100",true);
    }*/
}





void MainWindow::on_pushButton_8_clicked()
{
    QString path = QCoreApplication::applicationDirPath() + "/image";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}


//=================================1222在线升级====================================
//================================================================================

void MainWindow::on_pushButton_9_clicked()//选择文件
{
    QString filename = QFileDialog::getOpenFileName(this,tr("upgrading bin file"),QDir::currentPath(),tr("Bin files (*.bin);;All files(*.*)"));//弹出对话框
           if(filename == "")
           {
           addToStateMessage("Upgrading file loads failed......",true);
           //QMessageBox::information(this, tr("提示消息"), tr("请先选择正确的Bin文件！"), QMessageBox::Ok);
           return;
           }

           ui->insName_13->setText(filename);
           QFile updatafile;//创建QFile对象
           updatafile.setFileName(filename);
           QFileInfo fileInfo(filename);
           //数据信息计算
           InsGlobalVar::fileLen = fileInfo.size();//待发送文件大小->储存在全局变量fileLen中
           InsGlobalVar::sendCnt = (int)ceil(InsGlobalVar::fileLen / (PACKET_SIZE * 1.0));//总发送次数
           InsGlobalVar::lastLen = InsGlobalVar::fileLen % PACKET_SIZE;//最后一次发送的大小

           qDebug()<<"fileLen"<<InsGlobalVar::fileLen;
           qDebug()<<"sendCnt"<<InsGlobalVar::sendCnt;
           qDebug()<<"LastLen"<<InsGlobalVar::lastLen;

           if (0 == InsGlobalVar::lastLen)
           {//恰巧是PACKET_SIZE的整数倍
           InsGlobalVar::lastLen = PACKET_SIZE;
           }

           qDebug()<< "fileLen,lastLen,sendCnt:" << InsGlobalVar::fileLen << InsGlobalVar::lastLen << InsGlobalVar::sendCnt;

           if (updatafile.open(QIODevice::ReadOnly))
           {
           QDataStream dataStream(&updatafile);
           dataStream.readRawData((char *)insupdatafilepBuf, InsGlobalVar::fileLen);//将读取的内容放入缓冲取
           }//从文件流 dataStream中读取Ins_Globalvar::fileLen字节的数据，并将这些数据存放到insupdatafilepBuf指向的内存区域中。
           updatafile.close();
           addToStateMessage("Upgrading file loads successfully......",false);
           //for(int i= 0;i<100;i++)
           //{
           //    qDebug()<<"i="<<i<<"  "<<(uint8_t)insupdatafilepBuf[i];
           //}

}

InsUpgrad MainWindow:: ReadUpgradInfo()//读取升级信息
{
        //Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11", 502);
        qDebug()<<"ReadUpgradInfo here1";
        InsUpgrad info;

        info.inUpdate = 0;
        info.totalPage = 0;
        info.currentPage = 0;
        info.pageSize = 0;
        info.upgradStatus = 0;
        if(!true)//判断是否连接
        {
        return info;
        }
        uint16_t buffer[2];


        int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4082, buffer);//8104 in_update
        info.inUpdate = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
        if(ret != 0)
        {
        addToStateMessage("read in_update failed!",true);
        }
        //}
        /* else if(LightSignInt == 30)
        {
            int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4082, buffer);//8104 in_update
            int ret2 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4082, buffer2);//8104 in_update
            int ret3 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4082, buffer3);//8104 in_update
            info.inUpdate = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
            if(ret != 0|| ret2!=0|| ret3!=0)
            {
            addToStateMessage("read in_update failed!",true);
            }

        }
        else if(LightSignInt == 50)
        {
        int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4082, buffer);//8104 in_update
        int ret2 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4082, buffer2);//8104 in_update
        int ret3 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4082, buffer3);//8104 in_update
        int ret4 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FOURTHBOARD, 0x4082, buffer4);//8104 in_update
        int ret5 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIFTHBOARD, 0x4082, buffer5);//8104 in_update
        info.inUpdate = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
            if(ret != 0|| ret2!=0|| ret3!=0||ret4 !=0||ret5!= 0)
            {
            addToStateMessage("read in_update failed!",true);
            }

        }*/


//--------------------------------------------------------------------------------------------------------------------
        uint16_t buffer3[2];
        //if(LightSignInt == 10)
        //{
        int ret3 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4086, buffer3);//8108//totalpage
        info.totalPage = (static_cast<uint32_t>((buffer3[0]) << 16) | buffer3[1]);
        if(ret3 != 0)
        {
        addToStateMessage("read totalpage failed!",true);
        }
        //}

       /* else if(LightSignInt == 30)
        {
            int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4086, buffer);//8104 in_update
            int ret2 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4086, buffer2);//8104 in_update
            int ret3 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4086, buffer3);//8104 in_update
            info.inUpdate = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
            if(ret != 0|| ret2!=0|| ret3!=0)
            {
            addToStateMessage("read totalpage failed!",true);
            }

        }
        else if(LightSignInt == 50)
        {
        int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4086, buffer);//8104 in_update
        int ret2 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4086, buffer2);//8104 in_update
        int ret3 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4086, buffer3);//8104 in_update
        int ret4 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FOURTHBOARD, 0x4086, buffer4);//8104 in_update
        int ret5 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIFTHBOARD, 0x4086, buffer5);//8104 in_update
        info.inUpdate = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
            if(ret != 0|| ret2!=0|| ret3!=0||ret4 !=0||ret5!= 0)
            {
            addToStateMessage("read totalpage failed!",true);
            }

        }*/
//--------------------------------------------------------------------------------------------------------------------
        uint16_t buffer4[2];
        int ret5 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4088, buffer4);// //CurrentPage
        info.currentPage = (static_cast<uint32_t>((buffer4[0]) << 16) | buffer4[1]);
        if(ret5 != 0)
        {
        addToStateMessage("read currentpage failed!",true);
        }

        uint16_t buffer5[2];
        int ret6 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x408A, buffer5);//pagesize
        info.pageSize = (static_cast<uint32_t>((buffer5[0]) << 16) | buffer5[1]);
        if(ret6 != 0)
        {
        addToStateMessage("read pagesize failed!",true);
        }

        uint16_t buffer6[2];
        int ret7 = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x408C, buffer6);//flag
        info.upgradStatus = (static_cast<uint32_t>((buffer6[0]) << 16) | buffer6[1]);
        if(ret7 != 0)
        {
        addToStateMessage("read upgradstatus failed!",true);
        }

        qDebug()<<"ReadUpgradInfo here2:"<<info.upgradStatus;
        return info;


}

void MainWindow::WriteUpgradInfo(InsUpgrad info)//写入升级信息  //0611+204060
{
    if (!true)//改为标志位
    {
        return;
    }
    qDebug() << "Write 1";


    Ins_Modbus_udp::get_instance()->initmodbus_udp("192.168.1.11","192.168.1.2", 502);//modbus//0111   //0422在线升级与QT加了192.168.1.2
    Update_Modbus_udp::get_instance()->init_ins_modbus_udp("192.168.1.11","192.168.1.2", 65530);//0111

    qDebug() << "Write 2";

    uint16_t highValue = (info.inUpdate >> 16) & 0xFFFF;
    uint16_t lowValue = info.inUpdate & 0xFFFF;
    uint16_t buffer[2] = { highValue,lowValue };
    qDebug() << "Write 3"; //ok
    //qDebug() << "inUpdate" << info.inUpdate;

    //
    if (UpdateTenLength == 10)//bbbq
    {
        //int ret =
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);//8104  in_update
            //if(ret != 0)
            //{
            //addToStateMessage("Write in_update failed!",true);
            //}
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4082, buffer);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4082, buffer);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4082, buffer);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4082, buffer);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4082, buffer);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4082, buffer);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x4082, buffer);//0110

    }
    QThread::msleep(10);


    qDebug() << "Write 4";
    uint16_t highValue3 = (info.baseAddr >> 16) & 0xFFFF;
    uint16_t lowValue3 = info.baseAddr & 0xFFFF;
    uint16_t buffer3[2] = { highValue3,lowValue3 };
    if (UpdateTenLength == 10)//bbbq
    {
        //int ret3 =
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);//8108//BaseAddr
        //if(ret3 != 0)
        //{
        //addToStateMessage("Write baseAddr failed!",true);
        //}
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4084, buffer3);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4084, buffer3);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4084, buffer3);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4084, buffer3);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4084, buffer3);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4084, buffer3);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x4084, buffer3);//0110
    }
    QThread::msleep(10);



    uint16_t highValue4 = (info.totalPage >> 16) & 0xFFFF;
    uint16_t lowValue4 = info.totalPage & 0xFFFF;
    uint16_t buffer4[2] = { highValue4,lowValue4 };
    if (UpdateTenLength == 10)
    {
        //int ret4 =
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        //if(ret4 != 0)
        //{
        //addToStateMessage("Write totalpage failed!",true);
        //}
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4086, buffer4);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4086, buffer4);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4086, buffer4);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4086, buffer4);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4086, buffer4);//810C //TotalPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4086, buffer4);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x4086, buffer4);//0110
    }
    QThread::msleep(10);


    uint16_t highValue5 = (info.currentPage >> 16) & 0xFFFF;
    uint16_t lowValue5 = info.currentPage & 0xFFFF;
    uint16_t buffer5[2] = { highValue5,lowValue5 };
    if (UpdateTenLength == 10)
    {
        //int ret5 =
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        //if(ret5 != 0)
        //{
        //addToStateMessage("Write currentpage failed!",true);
        //}
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4088, buffer5);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4088, buffer5);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4088, buffer5);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4088, buffer5);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4088, buffer5);//8110 //CurrentPage
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4088, buffer5);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x4088, buffer5);//0110
    }
    QThread::msleep(10);


    uint16_t highValue6 = (info.pageSize >> 16) & 0xFFFF;
    uint16_t lowValue6 = info.pageSize & 0xFFFF;
    uint16_t buffer6[2] = { highValue6,lowValue6 };

    if (UpdateTenLength == 10)
    {
        //int ret6 =
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);//8114 //PageSize
        //if(ret6 != 0)
        //{
        //addToStateMessage("Write pagesize failed!",true);
        //}
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408A, buffer6);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408A, buffer6);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408A, buffer6);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x408A, buffer6);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408A, buffer6);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x408A, buffer6);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x408A, buffer6);//0110
    }
    QThread::msleep(10);


    qDebug() << "Write 5";
    uint16_t highValue7 = (info.upgradStatus >> 16) & 0xFFFF;
    uint16_t lowValue7 = info.upgradStatus & 0xFFFF;
    uint16_t buffer7[2] = { highValue7,lowValue7 };//读取状态
    //int ret7 =
    if (UpdateTenLength == 10)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);//8118 //Flag
    }
    else if (UpdateTenLength == 20)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408C, buffer7);//0110
    }
    else if (UpdateTenLength == 30)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408C, buffer7);//0110
    }
    else if (UpdateTenLength == 40)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408C, buffer7);//0110
    }
    else if (UpdateTenLength == 50)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x408C, buffer7);//0110
    }
    else if (UpdateTenLength == 60)
    {
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x408C, buffer7);
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x408C, buffer7);//0110
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SIXTHBOARD, 0x408C, buffer7);//0110
    }
    QThread::msleep(10);
    //if(ret7 != 0)
    //{
    //addToStateMessage("Write upgradFlag failed!",true);
    //}

    //qDebug()<<" Write info.upgradStatus"<<info.upgradStatus;

    qDebug() << "Write Finish";


}

int MainWindow::FormatStrData(string * aStr, uint32_t aStrLen)
{
        aStr->erase(remove(aStr->begin(), aStr->end(), ' '), aStr->end()); //remove all space in the string
        uint8_t lStrLen = (uint8_t)aStr->size();
        if(lStrLen>=aStrLen)
        * aStr = aStr->substr(0, aStrLen);
        else
        {
        for(uint8_t i=0; i<(aStrLen-lStrLen) ;i++)
        {
            aStr->insert(0,"0");
        }
        }
        for(uint8_t i=0; i<aStrLen;i++)
        {
        uint8_t lTempChar = aStr->at(i);
        if(!(((lTempChar>='0')&&(lTempChar<='9'))||((lTempChar>='A')&&(lTempChar<='F'))||((lTempChar>='a')&&(lTempChar<='f'))))
            return -1;
        }
        transform(aStr->begin(),aStr->end(),aStr->begin(),::tolower);
        return 0;
}


uint32_t MainWindow::ArrayToVariable32(uint8_t* array,uint8_t length)
{
        uint32_t value;
        if(length==2)
        {
        value = (((uint16_t)array[0]<<8)+((uint16_t)array[1]));
        }
        else if(length == 4)
        {
        value = (((uint32_t)array[0]<<24)+((uint32_t)array[1]<<16)+((uint32_t)array[2]<<8)+((uint32_t)array[3]));
        }
        return value;
}


void MainWindow::on_pushButton_10_clicked()//开始烧录
{

        timer9->stop();
        upTimer->stop();
        qDebug()<<"Update click 1";
        if(!true)//标志位
        {
        addToStateMessage("Device doesn't be opened......",true);
        return;
        }

        int binType = 0;
        uint32_t baseAddr = 0x01000000;//
        //uint32_t baseAddr = 0x04000000;//
        string lValGet = ui->insName_12->text().toStdString();
        FormatStrData(&lValGet,8);//标准化 确保为8位长度

        //qDebug()<<"lValGet:"<<QString::fromStdString(lValGet);

        uint8_t* lTXBufferAddr = new uint8_t[4];//转换为字节数组

        for(int8_t i=0;i<4;i++)
        {
        char * pEndTemp;
        string lstr = lValGet.substr(2*i,2);//每次提取两个字符 00
        qDebug()<<"lstr:"<<QString::fromStdString(lstr);
        qDebug()<<"Update click 2";
        lTXBufferAddr[i]=(uint8_t)strtoul(&(lstr[0]),&pEndTemp,16);//转换为数字 16进制
        qDebug()<<"pEndTemp:"<<pEndTemp<<"  "<<lTXBufferAddr[i];
        }
        qDebug()<<"Update click 3";
        baseAddr = ArrayToVariable32(lTXBufferAddr,4); // 4byte 转成Uint32
        //binType = ui->BinType->currentIndex();

        m_upgrad->setData((uint8_t *)insupdatafilepBuf,(int64_t)InsGlobalVar::fileLen,InsGlobalVar::sendCnt,InsGlobalVar::lastLen,binType,baseAddr);
        //初始化升级所需数据 //传递给升级线程

        addToStateMessage("Bootloader starting......",false);
        qDebug()<<"1225....."<<endl;

        m_upgrad->start();
        qDebug()<<"Update click 4";
        upTimer->start(1000);//run()


}

void MainWindow::slot_sendPercent()
{

    ui->progressBar->setValue(InsGlobalVar::percent);
    if(InsGlobalVar::percent == 100)
    {
        upTimer->stop();
        QMessageBox::information(this,"BootLoader","Finish",QMessageBox::Ok|QMessageBox::Cancel);
       // timer9->start();
    }

}

//=================================1222在线升级end==================================
//================================================================================



//============================一拖多  0307=================================
void MainWindow::handleDeviceSelection(void* lightHandle, void* cameraHandle,void* systemHandle)
{
    
    

    deviceHandles.lightHandle = lightHandle;
    deviceHandles.cameraHandle = cameraHandle;
    InsSystemHandle = systemHandle;

    if (!(deviceHandles.lightHandle == nullptr))
    {
        InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1608, 1);

        QThread::sleep(2);

        InsPMDL_WriteRegister(deviceHandles.lightHandle, "INSNEX", 1, 0x1608, 0);



        cout << "today is 0520" << endl;

        //InsPMDL_SetTriggerMode(InsSystemHandle, SoftTrigger);
        //InsPMDL_SetEmissionMode(InsSystemHandle, Max);
        //InsPMDL_GrabContinous(InsSystemHandle);//ok

        //QThread::sleep(2);

        //InsPMDL_GrabStop(InsSystemHandle);//ok
        cout << "today is 0521" << endl;

        //double TestExposure  = InsPMDL_GetExposure(deviceHandles.cameraHandle); //曝光输入问题

        //std::cout << "TestExposure0313::::" << TestExposure << std::endl;
    }


}


void MainWindow:: DeviceInfoDisplay(int CameraInfo, int LightInfo)
{
    deviceHandles.DeviceCamInfo = CameraInfo;//0521 CameraInfo
    deviceHandles.DeviceLigInfo = LightInfo;

    Ins2D5LightDeviceAndLocalIP* device = new Ins2D5LightDeviceAndLocalIP();
    InsPMDL_GetLightSouceDeviceInfo(deviceHandles.DeviceLigInfo, device);
    mLightInfo = device->lightSouce;
    ui->deviceDescription->setText(mLightInfo.device_model);
    ui->deviceIP->setText(mLightInfo.ip_addr);
    ui->insInterface_4->setText((QString)mLightInfo.length + "CM");
    ui->deviceMac->setText(mLightInfo.mac_addr);
    ui->deviceSubnet->setText(mLightInfo.net_mask);
    ui->stm32version->setText(mLightInfo.device_vision);
    ui->insInterface_7->setText(mLightInfo.SN);

    cout << "net MASK" << mLightInfo.net_mask << endl;

    InsCameraInfo* Camdevice = new InsCameraInfo();
    InsPMDL_GetCameraDeviceInfo(deviceHandles.DeviceCamInfo + 1, Camdevice);
    ui->insName->setText(Camdevice->divice_full_name_);
    ui->insInterface->setText(Camdevice->device_class_);


    std::string SystemName  = InsPMDL_GenerateSystemModel(Camdevice->divice_full_name_, mLightInfo.device_model);

    ui->insName_5 ->setText(QString::fromStdString(SystemName));
      

}



void MainWindow::ABcheckboxConverse()
{
    if (ui->checkBox_ABconverse->isChecked())
    {
        InsPMDL_SetABPhaseInversionOnOff(InsSystemHandle, On);
    }
    else 
    {
        InsPMDL_SetABPhaseInversionOnOff(InsSystemHandle, Off);
    }
}


void MainWindow::LanChange()
{
    QTranslator  LangEn;



    //0407
    bool ret = LangEn.load("Translation_en_en_US.qm");
    if (!ret)
    {
        cout << "读取错误 error error";
    }

    //insencode = new ins_encodeVerify();
    //insencode->switchLanguage(true);//子窗口显示英文界面
    //insencode->setWindowTitle("Encode Verify");

    //ins_DeviceConnect* devConnect = new ins_DeviceConnect();
    //devConnect->switchLanguage(true);
    //devConnect->setWindowTitle("Device Connect");



    //direction = new ins_DirectionChoose(this);//0531lan
    //direction->setWindowTitle("Select Pose");
    //direction->switchLanguage(true);

    qApp->installTranslator(&LangEn);
    ui->retranslateUi(this);

    if (transbool == 1)
    {
        ui->deviceDescription->setText(mLightInfo.device_model);
        ui->deviceIP->setText(mLightInfo.ip_addr);
        ui->deviceMac->setText(mLightInfo.mac_addr);
        ui->deviceSubnet->setText(mLightInfo.net_mask);
    }



}

void MainWindow::LanChange2()
{

    //insencode->switchLanguage(false); //0407
    ui->retranslateUi(this);

    if (transbool == 1)
    {
        ui->deviceDescription->setText(mLightInfo.device_model);
        ui->deviceIP->setText(mLightInfo.ip_addr);
        ui->deviceMac->setText(mLightInfo.mac_addr);
        ui->deviceSubnet->setText(mLightInfo.net_mask);
    }

}


//0327
void MainWindow::on_DirectionPoseBtn_clicked()
{
    direction = new ins_DirectionChoose(this);
    connect(direction, SIGNAL(chooseDirection1(int)), this, SLOT(DirectionChange(int)));//0314
    //direction->setWindowTitle("位姿选择");  // 设置窗口的标题
    direction->setWindowTitle("Select Pose");
    connect(direction, &ins_DirectionChoose::finished, direction, &ins_DirectionChoose::deleteLater);
    direction->show();
}

void MainWindow::DirectionChange(int index)

{
    cout << "DirectionChange" << endl;
    InsPMDL_SelectDirection(InsSystemHandle,index);
}




void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{
    if (index == 0)
    {
        UpdateTenLength = 10;
        qDebug() << "UpdateTenLength:" << UpdateTenLength;
    }
    else if (index == 1)
    {
        UpdateTenLength = 20;
        qDebug() << "UpdateTwoLength:" << UpdateTenLength;
    }
    else if (index == 2)
    {
        UpdateTenLength = 30;
        qDebug() << "UpdateThiLength:" << UpdateTenLength;
    }
    else if (index == 3)
    {
        UpdateTenLength = 40;
        qDebug() << "UpdateFouLength:" << UpdateTenLength;
    }
    else if (index == 4)
    {
        UpdateTenLength = 50;
        qDebug() << "UpdateFivLength:" << UpdateTenLength;
    }
    else if (index == 5)
    {
        UpdateTenLength = 60;
        qDebug() << "UpdateSixLength:" << UpdateTenLength;
    }

}


int MainWindow::checkCheckedCheckBoxes(const QQueue<QCheckBox*>& checkedCheckBoxes)
{
    for (QCheckBox* checkBox : checkedCheckBoxes) 
    {
        if (checkBox->isChecked()) 
        {
            return 1;  // 如果找到被选中的复选框，返回1
        }
    }
    return 0;  // 如果没有找到被选中的复选框，返回0
}

void MainWindow::ReturnToCenter()
{
    if (WindowCount == 1)
    {
        m_Images[0]->setPos(0, 0);  //0428pm
        int fixedX = -500;  
        int fixedY = -290;  
        ui->mimage->horizontalScrollBar()->setValue(fixedX);
        ui->mimage->verticalScrollBar()->setValue(fixedY);
    }
    else if (WindowCount == 2)
    {
        m_Images[0]->setPos(0, 0);  //0428pm
        m_Images[1]->setPos(0, 0);  //0428pm
        int fixedX = -225;
        int fixedY = -270;
        ui->mimage_2->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_2->verticalScrollBar()->setValue(fixedY-5);

        ui->mimage_3->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_3->verticalScrollBar()->setValue(fixedY-5);
    }
    else if (WindowCount == 4)
    {

        int fixedX = -210;
        int fixedY = -150;
        ui->mimage_4->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_4->verticalScrollBar()->setValue(fixedY);

        ui->mimage_5->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_5->verticalScrollBar()->setValue(fixedY);

        ui->mimage_6->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_6->verticalScrollBar()->setValue(fixedY);

        ui->mimage_7->horizontalScrollBar()->setValue(fixedX);
        ui->mimage_7->verticalScrollBar()->setValue(fixedY);

        m_Images[0]->setPos(0, 0);  //0428pm
        m_Images[1]->setPos(0, 0);  //0428pm
        m_Images[2]->setPos(0, 0);  //0428pm
        m_Images[3]->setPos(0, 0);  //0428pm
    }

  

    qDebug() << "----------------042555555555555-------------------" << endl;
}
void MainWindow::lineEdit_8returnPress()
{
    bool ok;
    double CameraTriggerCycle = ui->lineEdit_8->text().toDouble(&ok);//1205亮度提升

    InsResult ret = InsPMDL_SetScanningPeriod(InsSystemHandle, CameraTriggerCycle, dLineFre, dExp);//0409
    if (ret == InsResult::INSCSTATUS_OK || ret == InsResult::INSSTATUS_SUCCESS)
    {
        addToStateMessage("输入成功", false);
        addToStateMessage("曝光自动设置成功", false);
    }
    else
    {
        addToStateMessage("输入失败，输入范围（36~800）", true);
    }
}

//---------------------0603-------------------------

void MainWindow::onTabIndexChange(int index)
{
    if (index == 5&& !isPassed) 
    {  
        bool ok;


        QString password = QInputDialog::getText(this, "Verification",
            "Enter Password:", QLineEdit::Password,
            "", &ok);


        if (ok && password == "zpfljfinsnex" )
        {
            isPassed = true;
            QMessageBox::information(this, "Access Granted", "Verification passed.");
            ui->pushButton_11->setText("退出登录");
        }
        else 
        {
            QMessageBox::critical(this, "Access Denied", "Wrong password.");
            ui->tabWidget->setCurrentIndex(0);  
        }
    }
}

void MainWindow::onPushButton_11_clicked()
{
    isPassed = false;
    ui->pushButton_11->setText("INSNEX");
    ui->tabWidget->setCurrentIndex(0);
}


void MainWindow::onCheckBox_Ori_changed()
{
    if (ui->checkBox_Ori->isChecked())
    {
        ui->checkBox_Ori->setStyleSheet("QCheckBox { color : orange; }");
      
        QMessageBox::warning(this, "提示", "1.开启<显示原图>会增加内存的使用量\n\n2.建议在使用2K及以上分辨率显示器时选择此项\n");
        //QApplication::processEvents();
        DisplayOriImage = true;
        cout << "ori"<<endl;
        //ui->checkBox_Ori_2->setEnabled(true);
        

    }
    else 
    {
        ui->checkBox_Ori->setStyleSheet("QCheckBox { color : #a1d6e2; }");
        DisplayOriImage = false;
        //ui->checkBox_Ori_2->setEnabled(false);
        cout << " dis ori"<<endl;
    }
}

void MainWindow::onCheckBox_Ori_2_changed()
{
    if (ui->checkBox_Ori_2->isChecked())
    {
        if (DisplayOriImage == true)
        {
            AntiAliasing = true;
            QMessageBox::warning(this, "提示", "开启 <抗锯齿> 可以显著减少图像的边缘锯齿和噪声\n开启此项会增加系统计算负担,如果设备性能较高建议开启此项");
        }
        else
        {
            QMessageBox::warning(this, "提示", "开启 <显示原图> 后可选择此项");
            AntiAliasing = false;
            ui->checkBox_Ori_2->setCheckState(Qt::Unchecked);
        }



    }
    else
    {
        AntiAliasing = false;
    }
}

