#pragma once

#ifdef _MSC_VER
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��
#pragma comment(lib,"ws2_32.lib")
#else
#include <pcap.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include "device.h"


enum acknowledge_header {

	SEARCH_DEVICE_ACK,
	IP_SET_ACK,
	DEVICE_CONFIG_SET_ACK
};

enum command_header {

	SEARCH_DEVICE_CMD,
	IP_SET_CMD,
	DEVICE_CONFIG_SET_CMD
};
/*
struct  INSCP_ACK {
	acknowledge_header header;		//search device;ip_set
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
	uint16_t device_id;				//65535
	char device_vision[16];			//0.0.0.1
	char ip_addr[16];				//192.168.1.11
	char net_mask[16];				//255.255.255.0
	char gateway[16];				//192.168.1.1
	char mac_addr[18];				//0c:12:2b:56:0b:45
};
*/
struct  INSCP_ACK {
	acknowledge_header header;      //search device;ip_set;device_config_set
	char manufacturer_name[8];      //��˾��"INS"
	char model_name[8];             //��Ŀ����"2D5"
	char device_model[16];          //�豸�ͺ�" INS-CHVS-363-10GH"  ����
	char length[8];                 //��Դ�峤�ȣ�"30"  ����
	char SN[16];                    //�豸������ţ�" A0100041000"  ����
	char device_vision[16];         //����汾��:"2"
	char ip_addr[16];               //ip��ַ:"192.168.1.11"
	char net_mask[16];              //�������룺"255.255.255.0"
	char gateway[16];               //����"192.168.1.1"
	char mac_addr[18];              //mac��ַ��"0c:12:2b:56:0b:45"
};


struct factory {
	command_header header;			//search device;ip_set
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
};
/*
struct  INSCP_CMD {
	command_header header;			//search device;ip_set
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
	uint16_t device_id;				//65535
	char device_vision[16];			//0.0.0.1
	char ip_addr[16];				//192.168.1.11
	char net_mask[16];				//255.255.255.0
	char gateway[16];				//192.168.1.1
	char mac_addr[18];				//0c:12:2b:56:0b:45
};
*/
struct  INSCP_CMD {
	command_header header;                //search device;ip_set;device_config_set
	char manufacturer_name[8];            //��˾��"INS"
	char model_name[8];                   //��Ŀ����"2D5"
	char device_model[16];                //�豸�ͺ�" INS-CHVS-363-10GH"  ����
	char length[8];                       //��Դ�峤�ȣ�"30"  ����
	char SN[16];                          //�豸������ţ�" A0100041000"  ����
	char device_vision[16];               //����汾��:"2"
	char ip_addr[16];                     //ip��ַ:"192.168.1.11"
	char net_mask[16];                    //�������룺"255.255.255.0"
	char gateway[16];                     //����"192.168.1.1"
	char mac_addr[18];                    //mac��ַ��"0c:12:2b:56:0b:45"
};


class INSCAMERA_API  INSCP {
public:
	int InsPMDL_ResetDeviceIp(const char* ip);//�����豸ip
	int reset_device_ip(const char* ip, Ins2D5LightDeviceAndLocalIP& _deviceInfo);//�����豸ip
    int search_device(); //�ҵ����豸
	int search_all_devices(int& _num);//��ȫ���豸
	int get_device_info(int num, Ins2D5LightDeviceAndLocalIP& _deviceInfo);
	int factory_configuration();//11.17�����������ã���Ҫ�Ȱ��������ý�device ����
    int ins_init_ClistSock();
    int compare_NET_seg();
	int close_soket();
private:
    SOCKET ClistSock = {};
    SOCKADDR_IN SrvAddr;
    SOCKADDR_IN CliAddr;
    INSCP_CMD inscp_cmd;
	INSCP_ACK inscp_ack;
    char recvBuf[600] = { -1,0 };    //��
    char sendBuf[100] = { 0 };    //��
    char tempBuf[100] = { 0 };    //�洢�м���Ϣ����
    bool ClistSock_is_init = false;
    INSCP_ACK* cp;
	int receive_length;
	std::vector<Ins2D5LightDeviceAndLocalIP> mDeviceInfo;
};

void INSCAMERA_API str_to_arr(char* arr, const char* str, uint16_t len);
void INSCAMERA_API str_to_arr(char* arr, char* str, uint16_t len);

//���÷�����
void setnonblocking(int sockfd);

//��ȡ����ip
INSCAMERA_API std::vector<std::string> get_adapters(std::vector<std::string>& _net_mask, std::vector<std::string>& _gate_way);