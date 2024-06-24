#pragma once
#include <QDialog>
#include "ui_ins_DeviceConnect.h"
#include<QListWidgetItem>
#include"ins_camera_c.h"
#include <QTranslator>



class ins_DeviceConnect : public QDialog
{
	Q_OBJECT

public:
	ins_DeviceConnect(QWidget *parent = nullptr);
	~ins_DeviceConnect();

	//Ins2D5LightDeviceAndLocalIP **LightInfo;

	Ins2D5LightDeviceAndLocalIP** LightDevice;


	std::vector<Ins2D5LightDeviceAndLocalIP*> devices;
	std::vector<InsCameraInfo*> CamDevices;

	std::vector<void*> handles;

	std::vector<void*> CamHandles;

	//std::vector<char*> SN[64];

	std::vector<std::string> lightSNs;

	//std::vector<char*> lightSNs;

	//bool isMatched = false;

	QList<QListWidgetItem*> cameraItems;
	QList<QListWidgetItem*> lightItems;


	void switchLanguage(bool english);//0426
	QTranslator trans;


	
	 

	void* _ins_light1;
	void* _ins_light2;

	void* selectedLightHandle = nullptr;
	void* selectedCameraHandle = nullptr;
	void* systemHandle = nullptr;//0520
	//Ins2D5LightDeviceAndLocalIP* lightInfo = nullptr;//0315



	int selectedLightIndex;
	int selectedCameraIndex;

	//----
	struct DeviceIndexPair 
	{
		int cameraIndex;
		int lightIndex;

		

	};

	

	DeviceIndexPair pair;
	
	//----

private:
	Ui::ins_DeviceConnectClass *ui;


signals:
	void handleSelected(void* lightHandle, void* cameraHandle, void* systemHandle);

	void systemHandleSelect(void* systemHandle);
	//0315
	void DeviceInfoIndex(int CamIndex, int Lightindex);


public slots:
	void onDeviceConnectClicked();
	void onDeviceTestClicked();
	void onDeviceCloseClicked();

	void onConnectButtonClicked();

	void onCombinButtonClicked();

	void onSystemListItemSelected();
};


Q_DECLARE_METATYPE(ins_DeviceConnect::DeviceIndexPair);