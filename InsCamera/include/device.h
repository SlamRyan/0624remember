#pragma once
#include <stdint.h>
#include "ins_global.h"
/*
//设备信息
class INSCAMERA_API DEVICE {
public:
	char ip[16] = "192.168.1.11";                     //192.168.1.11
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
	uint16_t device_id;				//65535   SN号
	char device_vision[16];			//0.0.0.1  设备型号
	char net_mask[16]= "255.255.255.0";				//255.255.255.0
	char gateway[16]="192.168.1.1";				//192.168.1.1
	char mac_addr[18];				//0c:12:2b:56:0b:45
	static DEVICE* get_instance();
private:
	DEVICE()=default;
	static DEVICE* device;
};
*/

class INSCAMERA_API DEVICE {
public:
	char manufacturer_name[8];            //公司："INS"
public:
	char model_name[8];                   //项目名："2D5"
public:
	char device_model[16];                //设备型号" INS-CHVS-363-10GH"
public:
	char length[8];                       //光源板长度："30"
public:
	char SN[16];                          //设备条形码号：" A0100041000"
public:
	char device_vision[16];               //软件版本号:"2"
public:
	char ip_addr[16];                     //ip地址:"192.168.1.11"
public:
	char net_mask[16];                    //子网掩码："255.255.255.0"
public:
	char gateway[16];                     //网关"192.168.1.1"
public:
	char mac_addr[18];					  //mac地址："0c:12:2b:56:0b:45"
	static DEVICE* get_instance();
private:
	DEVICE() = default;
	static DEVICE* device;
};


//本地ip和子网掩码
class INSCAMERA_API LOCALDEVICE {
public:
	void init(const char* mip, const char* mnet_mask, const char* mgateway);
	static LOCALDEVICE* get_instance();	
	char ip[16];
	char net_mask[16];
	char gateway[16];
private:

	LOCALDEVICE()=default;
	static LOCALDEVICE* localdevice;
};


#define MaxCamera       (16)
#define MaxLight        (16)



struct Ins_PMDL_Userlight  //第三层
{
	void* ins_light;
	INS_PMDL_TriggerModeSelect TriggerMode;
	INS_PMDL_BoardNum BoardNum;
};


struct Ins_PMDL_UserCamera  //第三层
{
	void* ins_camera;
};

struct Ins_PMDL_SystemLight  //第二层
{
	uint32_t LightCount; //几个有效
	Ins_PMDL_Userlight light[MaxLight];
};

struct Ins_PMDL_SystemCamera  //第二层
{
	uint32_t CameraCount;
	Ins_PMDL_UserCamera Camera[MaxCamera];
};

struct InsPMDL_System             //最外层
{
	Ins_PMDL_SystemCamera SystemCamera;

	Ins_PMDL_SystemLight SystemLight;

	double normalGain ;
	double specualrGain;
	double diffuseGain ;
	double shapeGain;
};