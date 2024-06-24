#ifndef INS_DEVICECHOOSE_H
#define INS_DEVICECHOOSE_H

#include "ins_camera_c.h"
#include <QDialog>
#include<QListWidgetItem>
//#include"inscp_udp.h"
//这里引用报错

class MainWindow;
namespace Ui
{
class ins_devicechoose;
}

class ins_devicechoose : public QDialog
{
    Q_OBJECT

public:
    explicit ins_devicechoose(QWidget *parent = nullptr);
    //ins_devicechoose(MainWindow* mainWindow);
    ~ins_devicechoose();
    void get_mpcamera(void* _mpcamera);
private:
    Ui::ins_devicechoose *ui;
    //MainWindow* mpMainWindow;
    void* mpcamera;
    Ins2D5LightDevice deviceInfo;
public slots:
    void onSearchDevicesButtonClicked();
    void onDeviceClicked(QListWidgetItem* item);
    void onSetIpButtonClicked();

};

#endif // INS_DEVICECHOOSE_H
