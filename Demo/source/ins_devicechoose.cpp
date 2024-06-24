#include "ins_devicechoose.h"
#include "ui_ins_devicechoose.h"
#include <QMessageBox>
#include "inscp_udp.h"
#include"Ins_Utility_proc.h"

#include <QDebug>
using namespace Ins2D5Camera;

//Q_DECLARE_METATYPE(DEVICE);

ins_devicechoose::ins_devicechoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ins_devicechoose)

//ins_devicechoose::ins_devicechoose(MainWindow* mainWindow) : 
//    mpMainWindow(mainWindow) ,
//    ui(new Ui::ins_devicechoose)
{
    ui->setupUi(this);

    connect(ui->PB_Search,SIGNAL(clicked()),this,SLOT(onSearchDevicesButtonClicked()));

    connect(ui->listW_Devices, &QListWidget::itemClicked, this, &ins_devicechoose::onDeviceClicked);
    //connect(ui->listW_Devices, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onDeviceClicked(QListWidgetItem*)));

    connect(ui->handipBtn, &QPushButton::clicked, this, &ins_devicechoose::onSetIpButtonClicked);//West
}

ins_devicechoose::~ins_devicechoose()
{
    delete ui;
}

void ins_devicechoose::get_mpcamera(void* _mpcamera)
{
    mpcamera = _mpcamera;//浅拷贝
}
void ins_devicechoose::onSearchDevicesButtonClicked()
{
    InsPMDL_InitLocalInfo("192.168.1.2", "255.255.255.0", "192.168.1.1");//0108
    
    //Ins2D5LightDevice deviceInfo;
    //InsPMDL_SearchDevice(mpMainWindow->mpCamera, &deviceInfo);//最前面
    
    InsPMDL_SearchDevice(mpcamera, &deviceInfo);

 

    // 清空现有列表
    ui->listW_Devices->clear();


    QListWidgetItem *item = new QListWidgetItem(ui->listW_Devices);
    item->setIcon(QIcon("./image/logowhite.png"));
    /*
    item->setText(QString("设备名称: %1\nMAC地址: %2\nIP地址: %3\n子网掩码: %4")
                      .arg(DEVICE::get_instance()->device_vision)
                      .arg(DEVICE::get_instance()->mac_addr)
                      .arg(DEVICE::get_instance()->ip_addr) //这里引用utility.h报错
                      .arg(DEVICE::get_instance()->net_mask));
*/
    item->setText(QString("设备名称: %1\nMAC地址: %2\nIP地址: %3\n子网掩码: %4")
        .arg(deviceInfo.device_vision)
        .arg(deviceInfo.mac_addr)
        .arg(deviceInfo.ip_addr) 
        .arg(deviceInfo.net_mask));
    std::cout<<deviceInfo.device_vision<<std::endl;
    std::cout<<deviceInfo.mac_addr<<std::endl;
    std::cout<<deviceInfo.ip_addr<<std::endl;
}


void ins_devicechoose::onDeviceClicked(QListWidgetItem* item)
{
    //Device device = item->data(Qt::UserRole).value<Device>();
     //更新UI
    ui->deviceDescription->setText(deviceInfo.device_vision);//01.10
    ui->deviceIP->setText(deviceInfo.ip_addr);
    ui->deviceMac->setText(deviceInfo.mac_addr);
    ui->deviceSubnet->setText(deviceInfo.net_mask);
    //ui->deviceSubnet->setText(device.ip());


}

void ins_devicechoose::onSetIpButtonClicked()
{
    //INSCP UDip;

    //UDip.search_device();
    QString qnewIp = ui->LineUpdateIp->text();
    std::string newIp = qnewIp.toStdString();

    const char* newIp2 = newIp.c_str();

    int success  = (int)InsPMDL_ResetDeviceIp(mpcamera,newIp2);

    std::cout<<"[here]"<<DEVICE::get_instance()->ip_addr<<std::endl;

    std::cout<<newIp<<std::endl;

    if(!success)
    {
        QMessageBox::information(this,"成功","IP更新成功");
    }

    else if(success == -1)
    {
        QMessageBox::information(this,"失败","IP格式错误，请重新输入");
    }
    else
    {
        QMessageBox::warning(this,"失败","IP更新失败，请重试");
    }

}






