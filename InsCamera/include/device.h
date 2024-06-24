#pragma once
#include <stdint.h>
#include "ins_global.h"
/*
//�豸��Ϣ
class INSCAMERA_API DEVICE {
public:
	char ip[16] = "192.168.1.11";                     //192.168.1.11
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
	uint16_t device_id;				//65535   SN��
	char device_vision[16];			//0.0.0.1  �豸�ͺ�
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
	char manufacturer_name[8];            //��˾��"INS"
public:
	char model_name[8];                   //��Ŀ����"2D5"
public:
	char device_model[16];                //�豸�ͺ�" INS-CHVS-363-10GH"
public:
	char length[8];                       //��Դ�峤�ȣ�"30"
public:
	char SN[16];                          //�豸������ţ�" A0100041000"
public:
	char device_vision[16];               //����汾��:"2"
public:
	char ip_addr[16];                     //ip��ַ:"192.168.1.11"
public:
	char net_mask[16];                    //�������룺"255.255.255.0"
public:
	char gateway[16];                     //����"192.168.1.1"
public:
	char mac_addr[18];					  //mac��ַ��"0c:12:2b:56:0b:45"
	static DEVICE* get_instance();
private:
	DEVICE() = default;
	static DEVICE* device;
};


//����ip����������
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



struct Ins_PMDL_Userlight  //������
{
	void* ins_light;
	INS_PMDL_TriggerModeSelect TriggerMode;
	INS_PMDL_BoardNum BoardNum;
};


struct Ins_PMDL_UserCamera  //������
{
	void* ins_camera;
};

struct Ins_PMDL_SystemLight  //�ڶ���
{
	uint32_t LightCount; //������Ч
	Ins_PMDL_Userlight light[MaxLight];
};

struct Ins_PMDL_SystemCamera  //�ڶ���
{
	uint32_t CameraCount;
	Ins_PMDL_UserCamera Camera[MaxCamera];
};

struct InsPMDL_System             //�����
{
	Ins_PMDL_SystemCamera SystemCamera;

	Ins_PMDL_SystemLight SystemLight;

	double normalGain ;
	double specualrGain;
	double diffuseGain ;
	double shapeGain;
};