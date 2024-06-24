#include "ins_DeviceConnect.h"
#include "qdebug.h"
#include <iostream>
#include <QThread>
#include <QMessageBox>
#include <QProgressDialog>
#include <qtimer.h>


using namespace std;
using namespace Ins2D5Camera;
#pragma execution_character_set("utf-8")    

void* pCamera;
int ConnectFlag = 0;
ins_DeviceConnect::ins_DeviceConnect(QWidget *parent): 
	QDialog(parent), 
	ui(new Ui::ins_DeviceConnectClass)
{
	ui->setupUi(this);

	QIcon iconnn(":/image/icon/ins.ico");
	setWindowIcon(iconnn);


	qRegisterMetaType<DeviceIndexPair>("DevicePair");
	
    //LightDevice = (Ins2D5LightDeviceAndLocalIP**)malloc(sizeof(Ins2D5LightDevice*) * 128);
	//LightDevice[0] = new Ins2D5LightDeviceAndLocalIP();
	//LightDevice[1] = new Ins2D5LightDeviceAndLocalIP();
	//LightDevice[2] = new Ins2D5LightDeviceAndLocalIP();


	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onDeviceConnectClicked()));
	connect(ui->pushButton2, SIGNAL(clicked()), this, SLOT(onDeviceTestClicked()));
	connect(ui->pushButton5_2, SIGNAL(clicked()), this, SLOT(onDeviceCloseClicked()));

	connect(ui->pushButton4, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));
	connect(ui->pushButton5, SIGNAL(clicked()), this, SLOT(onCombinButtonClicked()));

	connect(ui->SystemList, SIGNAL(itemSelectionChanged()), this, SLOT(onSystemListItemSelected()));


	//ui->LightList->setDragEnabled(true);
	//ui->CameraList->setDragEnabled(true);
	//ui->SystemList->setAcceptDrops(true);
	//ui->SystemList2->setAcceptDrops(true);

	ui->pushButton5->setEnabled(false);
	ui->pushButton5_2->setEnabled(false);
	ui->pushButton4->setEnabled(false);
	ui->pushButton2->setEnabled(false);

	
}

ins_DeviceConnect::~ins_DeviceConnect()
{}


Ins2D5LightDeviceAndLocalIP* device;


void ins_DeviceConnect::onDeviceConnectClicked()//����
{
	ui->searchLabel->setStyleSheet("QLabel { color : orange; }");
	ui->searchLabel->setText("                               �豸������...��ȴ�...");
	QApplication::processEvents();

	ui->CameraList->setEnabled(false);
	ui->LightList->setEnabled(false);
	ui->SystemList->setEnabled(false);


	ui->CameraList->clear();
	ui->LightList->clear();

	int lightNum = 0;
	int camNum = 0;

	ui->LightList->clear();

	
	
	ui->pushButton->setEnabled(false);

	InsPMDL_FindLightSourceDevice(&lightNum);


	InsPMDL_FindCameraDevice(&camNum);

	handles = std::vector<void*>(lightNum, nullptr);
	CamHandles = std::vector<void*>(camNum, nullptr);



	//--------------------------------------
	for (int i = 0; i < lightNum; i++)
	{
		
		device  = new Ins2D5LightDeviceAndLocalIP();//0521 �ĵ�����
		InsPMDL_GetLightSouceDeviceInfo(i,device);
		//QThread::msleep(1000);

		//std::string ip = "192.168.1."+std::to_string(11+i);
		//InsPMDL_ResetLightSourceIP(ip.c_str(), *device);

		devices.push_back(device);
		//QThread::msleep(2000);
	}

	for (int i = 0; i < camNum; i++)
	{
		InsCameraInfo* Camdevice = new InsCameraInfo();
		InsPMDL_GetCameraDeviceInfo(i+1, Camdevice);
		CamDevices.push_back(Camdevice);

	}




	//----------------before--------------------------
	if (lightNum > 0)
	{

		char sn[64] = "1111";

		
		lightSNs.clear();
		 //111111
		for (int i = 0; i < lightNum; i++)
		{
			//InsPMDL_GetLightSourceHandle(&handles[i], *devices[i]);
			//QThread::msleep(1000);
			////memset(sn, '/0', sizeof(sn));
			//InsPMDL_GetSNFromCamera(handles[i], sn);

			///*if (isalnum(sn[0]))
			//{
			//	lightSNs.push_back(sn);
			//}
			//else
			//{
			//	lightSNs.push_back("Null");
			//} */
			//
			//if (isalnum(sn[0]))//isdigit(sn[0]))// || (sn[0] == '-' && isdigit(sn[1]
			//{
			//	
			//	long snValue = std::strtol(sn, nullptr, 10);
			//	
			//	lightSNs.push_back(std::to_string(snValue));
			//	//lightSNs.push_back("88888888");
			//}
			//else
			//{
			//	lightSNs.push_back("00000000");
			//}

			//----------------before--------------------------


			//lightSNs.push_back(std::string(sn)); //����
			//std::cout << "Device " << i << " SN: " << sn << std::endl;


			//--------------0417��
			InsPMDL_GetLightSourceHandle(&handles[i], *devices[i]);
			QThread::msleep(100);
			InsPMDL_GetSNFromLight(handles[i], sn);//0521 Camera->Light//0521debug //SNSNSNS

			// ���sn�ַ����Ƿ�Ϊ�գ�����Ϊ������ӵ�������
			if (sn[0] != '\0')
			{
				lightSNs.push_back(std::string(sn)); // ֱ�Ӵ洢�����ַ���
			}
			else
			{
				lightSNs.push_back("Null"); // ���snΪ�գ������"Null"
			}

			// ��ӡ���к����ڵ���
			std::cout << "Device " << i << " SN: " << sn << std::endl;

	
		}
		

	} 


	if (lightNum > 0)
	{



		int i = 0;
		for (auto& device : devices)
		{
			QListWidgetItem* item = new QListWidgetItem(ui->LightList);
			item->setText(QString("LightName   : %1\nLight IP        : %2\nPaired SN     : %3")
				.arg(device->lightSouce.device_model)
				.arg(device->lightSouce.ip_addr)
				.arg(QString::fromStdString(lightSNs[i])));//QString::fromStdString //QString::fromStdString(lightSNs[i])��Ϊ��ԴSN

			lightItems.append(item);//��ӵ���Դ�б�
			    
			qDebug() << "!!!!!!!!!!!!!!lightSNs[i]"<< QString::fromStdString(lightSNs[i]);


			item->setForeground(QBrush(Qt::white));
			ui->LightList->addItem(item);
			i++;
		}
	}
    else
    {
    	    QListWidgetItem* item = new QListWidgetItem(ui->LightList);
    	    item->setText("None Light Device");
            item->setForeground(QBrush(Qt::white));
    	    //ui->LightList->addItem(item);
    }
	//--------------------------Cam List
	if (camNum > 0)
	{
		for (auto& Camdevice : CamDevices)
		{
			QListWidgetItem* item = new QListWidgetItem(ui->CameraList);
			item->setText(QString("CamName    : %1\nCam Class    : %2\nCam SN       : %3")
				.arg(Camdevice->divice_full_name_)
				.arg(Camdevice->device_class_)
			    .arg(Camdevice->SN_));

			cameraItems.append(item);//��ӵ�����б�

			item->setForeground(QBrush(Qt::white));
			
			
			//ui->LightList->addItem(item); //�жϺ�item����List

		}
	}
	else
	{
		QListWidgetItem* item = new QListWidgetItem(ui->CameraList);
		item->setText("None Camera Device");
		item->setForeground(QBrush(Qt::white));
		ui->CameraList->addItem(item);
	}

	//---------------------------------------------------------------------------------

	//�ж������ԴSN�Ƿ�һ��
	//---------------------------------

	int systemNum = 1;

	for (auto& lightItem : lightItems)
	{
		QString lightSN = lightItem->text().split("\n").last().split(" : ").last();
		for (auto& cameraItem : cameraItems)
		{
			QString cameraSN = cameraItem->text().split("\n").last().split(" : ").last();
			if (lightSN == cameraSN && !lightSN.isEmpty()) 
			{
				// ����
				lightItem->setBackground(QBrush(Qt::gray));
				cameraItem->setBackground(QBrush(Qt::gray));

				// SysemList��ʾ
				QString combinedName = QString("System %1 : %2 +%3")
					.arg(systemNum)
					.arg(devices[lightItems.indexOf(lightItem)]->lightSouce.device_model)
					.arg(CamDevices[cameraItems.indexOf(cameraItem)]->divice_full_name_);
				QListWidgetItem* combinedItem = new QListWidgetItem(ui->SystemList);
				combinedItem->setText(combinedName);
				ui->SystemList->addItem(combinedItem);
				combinedItem->setForeground(QBrush(Qt::white));

				//----
				DeviceIndexPair indexPair = { cameraItems.indexOf(cameraItem), lightItems.indexOf(lightItem) };
				cout << "CamIndex:" <<cameraItems.indexOf(cameraItem) << endl;
				cout << "LightIndex:" << lightItems.indexOf(lightItem) << endl;
				QVariant var;
				var.setValue(indexPair);
				combinedItem->setData(Qt::UserRole, var);

				systemNum++;
				//----
			}
		}
	}



	//---------------------------------


	// ����Ŀ��ӵ��б���
	for (auto& item : cameraItems)
	{
		ui->CameraList->addItem(item);
	}
	for (auto& item : lightItems) 
	{
		ui->LightList->addItem(item);
	}



	

	//if (lightNum > 0)
	//{
	//	SN = std::vector<const char*>(lightNum,nullptr);
	//	for (int i = 0; i < lightNum; i++)
	//	{
	//		InsPMDL_GetLightSourceHandle(&handles[i], *devices[i]);

	//		InsPMDL_GetSNFromCamera(&handles[i], SN[i]);

	//	}

	//}
	/*
	if (camNum > 0)
	{
		for (int i = 0; i < camNum; i++)
		{
			InsPMDL_CreateCameraHandle(&CamHandles[i],i+1);
		}

	}*/
	

	//emit handleSelected(handles, CamHandles);


	/*for (auto& device : devices)
		{
		delete device;
		}*/
	
	//ui->searchLabel->setStyleSheet("QLabel { color : green; }");
	ui->searchLabel->setText(" ");
	ui->pushButton5->setEnabled(true);
	ui->pushButton5_2->setEnabled(true);
	ui->pushButton4->setEnabled(true);
	ui->pushButton2->setEnabled(true);

	ui->CameraList->setEnabled(true);
	ui->LightList->setEnabled(true);
	ui->SystemList->setEnabled(true);
	
	//ui->pushButton5->setEnabled(true); // ������ɺ��������ð�ť
	

}

void ins_DeviceConnect::onDeviceTestClicked()//����IP
{


	std::cout << "aaaaaaaaaaaaaaa"<<std::endl;

	QListWidgetItem* IPIndex = ui->LightList->currentItem();

	int ChangeIpIndex = ui->LightList->row(IPIndex);

	//std::string ip = "192.168.1.11";

	std::string ip2 = ui->lineEdit->text().toStdString();

    InsPMDL_ResetLightSourceIP(ip2.c_str(),devices[ChangeIpIndex]);

	std::cout << devices[ChangeIpIndex]->lightSouce.ip_addr;

}

void ins_DeviceConnect::onDeviceCloseClicked()
{
	ui->SystemList->clear();
}

void ins_DeviceConnect::onConnectButtonClicked()//����
{
	if (ConnectFlag == 0)
	{
		//QMessageBox::warning(this, "��ʾ", "�����Ѵ���ϵͳ��ѡ��Ҫ���ӵ�ϵͳ");
		//QMessageBox messageBox(QMessageBox::Warning, "��ʾ", "�����Ѵ���ϵͳ��ѡ��Ҫ���ӵ�ϵͳ", QMessageBox::Ok, this);
		//messageBox.setStyleSheet("QLabel{ color: white; } QPushButton{ color: white; }");
		//messageBox.exec();


		QMessageBox messageBox(QMessageBox::Warning, "��ʾ", "����<�Ѵ���ϵͳ>��ѡ��Ҫ���ӵ�ϵͳ", QMessageBox::Ok, this);
		messageBox.setStyleSheet("QLabel{ color: white; } QPushButton{ color: white; background-color: white; }");
		messageBox.exec();

	

		
	}
	else
	{

		ui->searchLabel->setStyleSheet("QLabel { color : green; }");
		ui->searchLabel->setText("                               �豸������...��ȴ�...");
		QApplication::processEvents();


		selectedLightHandle = nullptr;
		selectedCameraHandle = nullptr;
		systemHandle = nullptr;
		//lightInfo = nullptr;

		cout << "Connect Time :::pair.cameraIndex::" << pair.cameraIndex << endl;


		cout << "Connect Time :::pair.LightIndex::" << pair.lightIndex << endl;
		//------------------------------------



		if (pair.lightIndex >= 0 && pair.lightIndex < devices.size())
		{
			InsPMDL_GetLightSourceHandle(&selectedLightHandle, *devices[pair.lightIndex]);
			qDebug() << "~~~~~~~~~Light Handle Create .";
		}

		if (pair.cameraIndex >= 0 && pair.cameraIndex < CamDevices.size())
		{
			InsPMDL_CreateCameraHandle(&selectedCameraHandle, pair.cameraIndex + 1);//��1��ʼ
			qDebug() << "~~~~~~~~~Camera Handle Create .";
		}

		if (1)//0521
		{
			InsPMDL_SystemInfo SystemInfo;
			SystemInfo.CamInfo.id[0] = pair.cameraIndex + 1;
			SystemInfo.LightInfo.LightDeviceInfo[0] = new Ins2D5LightDeviceAndLocalIP;
			SystemInfo.CamInfo.CamNum = 1;
			SystemInfo.LightInfo.LightNum = 1;
			strcpy(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length, device->lightSouce.length);//0521  //��LightInfo��ȡ
			strcpy(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.ip_addr, device->lightSouce.ip_addr);
			strcpy(SystemInfo.LightInfo.LightDeviceInfo[0]->localIP, device->localIP);
			InsPMDL_GetSystemHandle(&systemHandle, SystemInfo);
		}

		if (selectedLightHandle && selectedCameraHandle)
		{
			char Test[64] = "12345678";
			emit handleSelected(selectedLightHandle, selectedCameraHandle, systemHandle);//0522
			//emit systemHandleSelect(systemHandle);
			//InsPMDL_SaveSNToCamera(selectedLightHandle, Test);//CamDevices[0]->SN_
			InsPMDL_SaveSNToLight(systemHandle, CamDevices[0]->SN_);//CamDevices[0]->SN_ //0521SN
			//InsPMDL_SaveSNToLight(selectedLightHandle, CamDevices[0]->SN_);//CamDevices[0]->SN_ //0521SN
			qDebug() << "!!!!!!!!!!!!!!!!!!CamDevices[0]->SN_" << CamDevices[0]->SN_ << endl;

			ui->searchLabel->setText("");
			//QTimer::singleShot(3000, this, &ins_DeviceConnect::close);
			
		}
		else {

			qDebug() << "!!!!!!!!!!!!Handle Craet Fail";
		}
	}
	

}

void ins_DeviceConnect::onCombinButtonClicked()//���
{
	
	int systemNum = 1;
	QListWidgetItem* selectedLightItem = ui->LightList->currentItem();
	QListWidgetItem* selectedCameraItem = ui->CameraList->currentItem();

	//��û�б�ѡ��
	if (!selectedLightItem || !selectedCameraItem)
	{
		qDebug() << "ѡ������͹�Դ.";
		return;
	}

	selectedLightIndex = ui->LightList->row(selectedLightItem);
	selectedCameraIndex = ui->CameraList->row(selectedCameraItem);

	std::cout << "CombineButtonaaaaa::" << selectedLightIndex<<endl;

	// ��ȡ�豸��Ϣ
	if (selectedLightIndex >= 0 && selectedLightIndex < devices.size() && selectedCameraIndex >= 0 && selectedCameraIndex < CamDevices.size())
	{
		Ins2D5LightDeviceAndLocalIP* lightDevice = devices[selectedLightIndex];
		InsCameraInfo* cameraDevice = CamDevices[selectedCameraIndex];

		// ����豸����
			QString combinedName = QString("System %1 : %2 +%3")
			.arg(systemNum)
			.arg(lightDevice->lightSouce.device_model)
			.arg(cameraDevice->divice_full_name_);

		// ��ӵ�SystemList��
		QListWidgetItem* item = new QListWidgetItem(ui->SystemList);
		item->setText(combinedName);
		item->setForeground(QBrush(Qt::white));
		ui->SystemList->addItem(item);
		systemNum++;
	}
	else {
		qDebug() << "ȱ��������Դ��ѡ��.";
	}


}


void ins_DeviceConnect::onSystemListItemSelected() 
{



	QListWidgetItem* selectedItem = ui->SystemList->currentItem();
	if (selectedItem)
	{
		ConnectFlag = 1;
	    pair = selectedItem->data(Qt::UserRole).value<DeviceIndexPair>();
		qDebug() << "Selected camera index in CamDevices: " << pair.cameraIndex;
		qDebug() << "Selected light index in devices: " << pair.lightIndex;
		emit DeviceInfoIndex(pair.cameraIndex, pair.lightIndex);

		//pair.cameraIndex��pair.lightIndex
	}
}

void ins_DeviceConnect::switchLanguage(bool english)
{
	if (english)
	{
		trans.load("Translation_en_en_US.qm");
		qApp->installTranslator(&trans);
		ui->retranslateUi(this);
	}
	else
	{
		ui->retranslateUi(this);
	}

}


