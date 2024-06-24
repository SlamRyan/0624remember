#include "..\include\inscp_udp.h"
#include <thread> 
//#include "winsock.h"
//#include <WinSock2.h>
long ivp4ToInt(char* str) {
	int i, slen = 0;
	int numTemp = 0;
	int dotNum = 0;
	int bitNum = 0;
	long  ipNum = 0;
	if (str == NULL) {            //IP��ַ�ַ���Ϊ��
		printf("str == NULL\n");
		return -1;
	}
	slen = strlen(str);
	if (slen < 7 || slen > 15) {   //�ж�ip��ַ�ĳ����Ƿ���ȷ����С��X.X.X.X��,���XXX.XXX.XXX.XXX��
		printf("strlen(str) < 7 or strlen(str)>15\n");
		return -1;
	}
	if (str[0] == '.' || str[slen - 1] == '.') {  //�ж�ʱ��Ϊ�ԡ�.����ͷ���߽�β
		printf("ip format error\n");
		return -1;
	}
	for (i = 0; i < slen; i++) {
		if (str[i] == '.') {            // �ж��Ƿ�X..X��,����������
			if (str[i - 1] == '.') {
				printf("pStr == . format error\n");
				return -1;
			}
			if (++dotNum > 3) {   // �жϡ�."�ĸ����Ƿ����3������
				printf("dot number over 3\n");
				return -1;
			}
			if (numTemp > 255) {
				printf("numTemp > 255, numTemp :%d\n", numTemp);
				return -1;
			}
			ipNum |= numTemp;
			numTemp = 0;
			ipNum <<= 8;
			bitNum = 0;
			continue;
		}
		if (str[i] < '0' || str[i] > '9') {  //ip�ڵ��ַ��Ƿ�Ϊ������
			printf("ip addr error, str :%c\n", str[i]);
			return -1;
		}
		numTemp = numTemp * 10 + (str[i] - '0');  //���㵥����ַ����ֵ
		if (++bitNum > 3) {   //�жϵ�����ַλ���Ƿ񳬹�3λ��
			printf("ip addr bit number error\n");
			return -1;
		}
	}
	if (dotNum != 3) {    //�жϴ�����ַ�����.���ĸ����Ƿ�Ϊ3��
		printf("dotNum != 3\n");
		return -1;
	}
	ipNum |= numTemp;
	//printf("ipNUm :%lx\n", ipNum);
	return ipNum;
}

int INSCP::InsPMDL_ResetDeviceIp(const char* ip1)// old
{
	std::string ip(ip1);
	char* ch = (char*)malloc((ip.length() + 1) * sizeof(char));
	//ip.copy(ch, ip.length(),0);
	if (ch == nullptr){ return -2; }
	strcpy(ch, ip.c_str());
	if (ivp4ToInt(ch) == -1)
	{
		free(ch);
		ch = nullptr;
		return -1;
	}
	int len = sizeof(SOCKADDR);
	if (!ClistSock_is_init) {
		ins_init_ClistSock();
	}


	if ((htonl(inet_addr(DEVICE::get_instance()->ip_addr)) & htonl(inet_addr(DEVICE::get_instance()->net_mask))) == (htonl(inet_addr(LOCALDEVICE::get_instance()->ip)) & htonl(inet_addr(LOCALDEVICE::get_instance()->net_mask))))
	{
		SrvAddr.sin_addr.S_un.S_addr = inet_addr(DEVICE::get_instance()->ip_addr);//����˵�IP��ַ
	}
	else {
		//str_to_arr(DEVICE::get_instance()->ip, "255.255.255.255", strlen("255.255.255.255"));
		SrvAddr.sin_family = AF_INET;//ѡ���ַ��s
		SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//����˵�IP��ַ //0312 DEVICE::get_instance()->ip_addr ->"255.255.255.255"
		SrvAddr.sin_port = htons(65535);//����˵Ķ˿ں�
	}

	inscp_cmd.header = IP_SET_CMD;
	str_to_arr(inscp_cmd.manufacturer_name, "INS", strlen("INS"));
	str_to_arr(inscp_cmd.model_name, "2D5", strlen("2D5"));
	str_to_arr(inscp_cmd.device_vision, "0.0.0.1", strlen("0.0.0.1"));
	str_to_arr(inscp_cmd.device_model, "INS-CHVS-363-10GH", strlen("INS-CHVS-363-10GH"));
	str_to_arr(inscp_cmd.length, "30", strlen("30"));
	str_to_arr(inscp_cmd.SN, "A0100041000", strlen("A0100041000"));
	str_to_arr(inscp_cmd.ip_addr, ip.c_str(), strlen(ip.c_str()));
	str_to_arr(inscp_cmd.net_mask, LOCALDEVICE::get_instance()->net_mask, strlen(LOCALDEVICE::get_instance()->net_mask));
	str_to_arr(inscp_cmd.gateway, "192.168.1.1", strlen("192.168.1.1"));
	str_to_arr(inscp_cmd.mac_addr, "08:8F:C3:FC:16:9F", strlen("08:8F:C3:FC:16:9F"));
	int request_num = 3;
	while (request_num)
	{
		request_num--;
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//������˷���"hello"��Ϣ
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		receive_length = recvfrom(ClistSock, recvBuf, sizeof(recvBuf), 0, (SOCKADDR*)&SrvAddr, &len);
		if (receive_length != -1)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
	}
	
	cp = (struct INSCP_ACK*)recvBuf;
	str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17����
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17����
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17����

	
	free(ch);
	ch = nullptr;
	return 0;
}



int INSCP::search_device()
{
	if (ivp4ToInt(LOCALDEVICE::get_instance()->ip) == -1)
	{
		return -1;
	}
	int len = sizeof(SOCKADDR);
	//if (!ClistSock_is_init) {
	//	ins_init_ClistSock();
	//}

	ins_init_ClistSock();//0228
		//-----------0520
	int yes = 1;
	if (setsockopt(ClistSock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes)) == -1)
	{
		perror("setsockopt failed:");
		closesocket(ClistSock); // ����ʱȷ���ر��׽���
		return -1;
	}
	//-----------
	str_to_arr(DEVICE::get_instance()->ip_addr, "255.255.255.255", strlen("255.255.255.255"));
	SrvAddr.sin_family = AF_INET;//ѡ���ַ��s
	SrvAddr.sin_addr.S_un.S_addr = inet_addr(DEVICE::get_instance()->ip_addr);//����˵�IP��ַ
	SrvAddr.sin_port = htons(65535);//����˵Ķ˿ں�

	
	CliAddr.sin_family = AF_INET;//ѡ���ַ��s
	CliAddr.sin_addr.S_un.S_addr = inet_addr(LOCALDEVICE::get_instance()->ip);//���ص�IP��ַ
	CliAddr.sin_port = htons(56541);//���صĶ˿ں�
	if (bind(ClistSock, (sockaddr*)&CliAddr, sizeof(CliAddr)) == -1) {
		perror("bind failed:");
	}

	inscp_cmd.header = SEARCH_DEVICE_CMD;
	str_to_arr(inscp_cmd.manufacturer_name, "INS", strlen("INS"));
	str_to_arr(inscp_cmd.model_name, "2D5", strlen("2D5"));
	str_to_arr(inscp_cmd.device_vision, "0.0.0.1", strlen("0.0.0.1"));
	str_to_arr(inscp_cmd.device_model, "INS-CHVS-363-10GH", strlen("INS-CHVS-363-10GH"));
	str_to_arr(inscp_cmd.length, "30", strlen("30"));
	str_to_arr(inscp_cmd.SN, "A0100041000", strlen("A0100041000"));
	str_to_arr(inscp_cmd.ip_addr, LOCALDEVICE::get_instance()->ip, strlen(LOCALDEVICE::get_instance()->ip));
	str_to_arr(inscp_cmd.net_mask, LOCALDEVICE::get_instance()->net_mask, strlen(LOCALDEVICE::get_instance()->net_mask));
	str_to_arr(inscp_cmd.gateway, LOCALDEVICE::get_instance()->gateway, strlen(LOCALDEVICE::get_instance()->gateway));
	str_to_arr(inscp_cmd.mac_addr, "08:8F:C3:FC:16:9F", strlen("08:8F:C3:FC:16:9F"));
	
	sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//������˷���"hello"��Ϣ
	auto startTime = std::chrono::system_clock::now();

	auto nowTime = std::chrono::system_clock::now();
	while (nowTime - startTime < std::chrono::seconds(5))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		receive_length = recvfrom(ClistSock, recvBuf, sizeof(recvBuf), 0, (SOCKADDR*)&SrvAddr, &len);
		if (receive_length != -1)
		{
			cp = (struct INSCP_ACK*)recvBuf;
			Ins2D5LightDeviceAndLocalIP * _spinsdeviceinfo = new Ins2D5LightDeviceAndLocalIP();
			str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
			str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
			str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
			str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
			str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
			str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
			str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
			str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17����
			str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17����
			str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17����

			strcpy(_spinsdeviceinfo->localIP, LOCALDEVICE::get_instance()->ip);
			strcpy(_spinsdeviceinfo->localNetMask, LOCALDEVICE::get_instance()->net_mask);
			strcpy(_spinsdeviceinfo->localGateWay, LOCALDEVICE::get_instance()->gateway);

			strcpy(_spinsdeviceinfo->lightSouce.ip_addr, cp->ip_addr);
			strcpy(_spinsdeviceinfo->lightSouce.net_mask, cp->net_mask);
			strcpy(_spinsdeviceinfo->lightSouce.gateway, cp->gateway);
			strcpy(_spinsdeviceinfo->lightSouce.mac_addr, cp->mac_addr);
			strcpy(_spinsdeviceinfo->lightSouce.manufacturer_name, cp->manufacturer_name);
			strcpy(_spinsdeviceinfo->lightSouce.device_vision, cp->device_vision);
			strcpy(_spinsdeviceinfo->lightSouce.model_name, cp->model_name);
			strcpy(_spinsdeviceinfo->lightSouce.device_model, cp->device_model);//11.17����
			strcpy(_spinsdeviceinfo->lightSouce.length, cp->length);//11.17����
			strcpy(_spinsdeviceinfo->lightSouce.SN, cp->SN);//11.17����
			mDeviceInfo.push_back(*_spinsdeviceinfo);
			delete _spinsdeviceinfo;
			break;
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		//std::this_thread::sleep_for(std::chrono::seconds(2));//11.28
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		nowTime = std::chrono::system_clock::now();

	}
	return -1;
}

int INSCP::search_all_devices(int& _num)
{
	_num = 0;
	mDeviceInfo.clear();
	std::vector<std::string> netMask;
	std::vector<std::string> gateWay;
	std::vector<std::string> ipFamily = get_adapters(netMask,gateWay);//��ȡ����IP
	int i = 0;
	while (i< ipFamily.size())
	{
		if(ipFamily[i].find("0.0.0.0") == std::string::npos)//��IP����
		{ 
			LOCALDEVICE::get_instance()->init(ipFamily[i].c_str(), netMask[i].c_str(), gateWay[i].c_str());
			search_device();
		}
		i++;
	}
	_num = mDeviceInfo.size();
	return 0;
}
int INSCP::get_device_info(int _num, Ins2D5LightDeviceAndLocalIP& _deviceInfo)
{
	if ((_num) < mDeviceInfo.size()) //num��ֵ
	{
		strcpy(_deviceInfo.localIP, mDeviceInfo[_num].localIP);
		strcpy(_deviceInfo.localGateWay, mDeviceInfo[_num].localGateWay);
		strcpy(_deviceInfo.lightSouce.ip_addr, mDeviceInfo[_num].lightSouce.ip_addr);
		strcpy(_deviceInfo.lightSouce.net_mask, mDeviceInfo[_num].lightSouce.net_mask);
		strcpy(_deviceInfo.lightSouce.gateway, mDeviceInfo[_num].lightSouce.gateway);
		strcpy(_deviceInfo.lightSouce.mac_addr, mDeviceInfo[_num].lightSouce.mac_addr);
		strcpy(_deviceInfo.lightSouce.manufacturer_name, mDeviceInfo[_num].lightSouce.manufacturer_name);
		strcpy(_deviceInfo.lightSouce.device_vision, mDeviceInfo[_num].lightSouce.device_vision);
		strcpy(_deviceInfo.lightSouce.model_name, mDeviceInfo[_num].lightSouce.model_name);
		strcpy(_deviceInfo.lightSouce.device_model, mDeviceInfo[_num].lightSouce.device_model);//11.17����
		strcpy(_deviceInfo.lightSouce.length, mDeviceInfo[_num].lightSouce.length);//11.17����
		strcpy(_deviceInfo.lightSouce.SN, mDeviceInfo[_num].lightSouce.SN);//11.17����
		return 0;
	}
	else
	{
		return -1;
	}
}

int INSCP::reset_device_ip(const char* ip1, Ins2D5LightDeviceAndLocalIP& _deviceInfo)//�����豸ip
{
	LOCALDEVICE::get_instance()->init(_deviceInfo.localIP, _deviceInfo.localNetMask, _deviceInfo.localGateWay);

	std::string ip(ip1);
	char* ch = (char*)malloc((ip.length() + 1) * sizeof(char));
	//ip.copy(ch, ip.length(),0);
	if (ch == nullptr) { return -2; }
	strcpy(ch, ip.c_str());
	if (ivp4ToInt(ch) == -1)
	{
		free(ch);
		ch = nullptr;
		return -1;
	}
	int len = sizeof(SOCKADDR);
	if (!ClistSock_is_init) {
		ins_init_ClistSock();
	}
	//ins_init_ClistSock();

	if ((htonl(inet_addr(_deviceInfo.lightSouce.ip_addr)) & htonl(inet_addr(_deviceInfo.lightSouce.net_mask))) == (htonl(inet_addr(_deviceInfo.localIP) & htonl(inet_addr(_deviceInfo.localNetMask)))))
	{
		SrvAddr.sin_addr.S_un.S_addr = inet_addr(_deviceInfo.lightSouce.ip_addr);//����˵�IP��ַ
	}
	else {
		//str_to_arr(DEVICE::get_instance()->ip, "255.255.255.255", strlen("255.255.255.255"));
		SrvAddr.sin_family = AF_INET;//ѡ���ַ��s
		SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//����˵�IP��ַ
		SrvAddr.sin_port = htons(65535);//����˵Ķ˿ں�
	}

	inscp_cmd.header = IP_SET_CMD;
	str_to_arr(inscp_cmd.manufacturer_name, "INS", strlen("INS"));
	str_to_arr(inscp_cmd.model_name, "2D5", strlen("2D5"));
	str_to_arr(inscp_cmd.device_vision, "0.0.0.1", strlen("0.0.0.1"));
	str_to_arr(inscp_cmd.device_model, "INS-CHVS-363-10GH", strlen("INS-CHVS-363-10GH"));
	str_to_arr(inscp_cmd.length, "30", strlen("30"));
	str_to_arr(inscp_cmd.SN, "A0100041000", strlen("A0100041000"));
	str_to_arr(inscp_cmd.ip_addr, ip.c_str(), strlen(ip.c_str()));                                                       //ip
	str_to_arr(inscp_cmd.net_mask, "255.255.255.0", strlen("255.255.255.0"));//�������� 255.255.255.0 //LOCALDEVICE::get_instance()->net_mask->"255,255,255.0"
	str_to_arr(inscp_cmd.gateway, "192.168.1.1", strlen("192.168.1.1"));                                                 //����δ������������DC(ֻҪ�豸�ͱ���IP��ͬһ���μ���)
	str_to_arr(inscp_cmd.mac_addr, "08:8F:C3:FC:16:9F", strlen("08:8F:C3:FC:16:9F"));
	int request_num = 3;
	while (request_num)
	{
		request_num--;
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//������˷���"hello"��Ϣ
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		receive_length = recvfrom(ClistSock, recvBuf, sizeof(recvBuf), 0, (SOCKADDR*)&SrvAddr, &len);
		if (receive_length != -1)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
	}

	cp = (struct INSCP_ACK*)recvBuf;
	
	str_to_arr(_deviceInfo.lightSouce.ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(_deviceInfo.lightSouce.net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(_deviceInfo.lightSouce.gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(_deviceInfo.lightSouce.mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(_deviceInfo.lightSouce.manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(_deviceInfo.lightSouce.device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(_deviceInfo.lightSouce.model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(_deviceInfo.lightSouce.device_model, cp->device_model, strlen(cp->device_model));//11.17����
	str_to_arr(_deviceInfo.lightSouce.length, cp->length, strlen(cp->length));//11.17����
	str_to_arr(_deviceInfo.lightSouce.SN, cp->SN, strlen(cp->SN));//11.17����

	{
	str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17����
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17����
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17����
	}
	free(ch);
	ch = nullptr;
	return 0;
}





int INSCP::factory_configuration()
{
	if (!ClistSock_is_init) {
		ins_init_ClistSock();
	}
	inscp_cmd.header = DEVICE_CONFIG_SET_CMD;
	str_to_arr(inscp_cmd.manufacturer_name, DEVICE::get_instance()->manufacturer_name, strlen(DEVICE::get_instance()->manufacturer_name));
	str_to_arr(inscp_cmd.model_name, DEVICE::get_instance()->model_name, strlen(DEVICE::get_instance()->model_name));
	str_to_arr(inscp_cmd.device_model, DEVICE::get_instance()->device_model, strlen(DEVICE::get_instance()->device_model));
	str_to_arr(inscp_cmd.length, DEVICE::get_instance()->length, strlen(DEVICE::get_instance()->length));
	str_to_arr(inscp_cmd.SN, DEVICE::get_instance()->SN, strlen(DEVICE::get_instance()->SN));
	str_to_arr(inscp_cmd.device_vision, DEVICE::get_instance()->device_vision, strlen(DEVICE::get_instance()->device_vision));
	str_to_arr(inscp_cmd.ip_addr, DEVICE::get_instance()->ip_addr, strlen(DEVICE::get_instance()->ip_addr));
	str_to_arr(inscp_cmd.net_mask, DEVICE::get_instance()->net_mask, strlen(DEVICE::get_instance()->net_mask));
	str_to_arr(inscp_cmd.gateway, DEVICE::get_instance()->gateway, strlen(DEVICE::get_instance()->gateway));
	str_to_arr(inscp_cmd.mac_addr, DEVICE::get_instance()->mac_addr, strlen(DEVICE::get_instance()->mac_addr));
	char temp_manufacturer_name[16] = {0};
	str_to_arr(temp_manufacturer_name, DEVICE::get_instance()->manufacturer_name, strlen(DEVICE::get_instance()->manufacturer_name));

	//����
	memset(DEVICE::get_instance()->manufacturer_name, 0, sizeof(DEVICE::get_instance()->manufacturer_name));
	memset(DEVICE::get_instance()->model_name, 0, sizeof(DEVICE::get_instance()->model_name));
	memset(DEVICE::get_instance()->device_model, 0, sizeof(DEVICE::get_instance()->device_model));
	memset(DEVICE::get_instance()->length, 0, sizeof(DEVICE::get_instance()->length));
	memset(DEVICE::get_instance()->SN, 0, sizeof(DEVICE::get_instance()->SN));
	memset(DEVICE::get_instance()->device_vision, 0, sizeof(DEVICE::get_instance()->device_vision));
	memset(DEVICE::get_instance()->ip_addr, 0, sizeof(DEVICE::get_instance()->ip_addr));
	memset(DEVICE::get_instance()->net_mask, 0, sizeof(DEVICE::get_instance()->net_mask));
	memset(DEVICE::get_instance()->gateway, 0, sizeof(DEVICE::get_instance()->gateway));
	memset(DEVICE::get_instance()->mac_addr, 0, sizeof(DEVICE::get_instance()->mac_addr));
	
	
	SOCKADDR_IN SrvAddr;
	SrvAddr.sin_family = AF_INET;//ѡ���ַ��s
	SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//����˵�IP��ַ
	SrvAddr.sin_port = htons(65535);//����˵Ķ˿ں�

	SOCKADDR_IN CliAddr;
	CliAddr.sin_family = AF_INET;//ѡ���ַ��s
	CliAddr.sin_addr.S_un.S_addr = inet_addr(LOCALDEVICE::get_instance()->ip);//���ص�IP��ַ
	CliAddr.sin_port = htons(65534);//���صĶ˿ں�
	if (bind(ClistSock, (sockaddr*)&CliAddr, sizeof(CliAddr)) == -1) {
		perror("bind failed:");
	}

	int len = sizeof(SOCKADDR);

	int request_num = 3;
	while (request_num)
	{
		request_num--;
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//������˷���"hello"��Ϣ
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
		receive_length = recvfrom(ClistSock, (char*)&inscp_ack, sizeof(inscp_ack), 0, (SOCKADDR*)&SrvAddr, &len);
		if (receive_length != -1)
		{
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
	}

	/*
	std::this_thread::sleep_for(std::chrono::milliseconds(SEARCH_DEVICE_TIME));
	sendto(ClistSock, (char*)&inscp_cmd, sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);
	//�ȴ�����
	std::this_thread::sleep_for(std::chrono::milliseconds(126));
	recv_length = recvfrom(ClistSock, (char*)&inscp_ack, sizeof(inscp_ack), 0, (SOCKADDR*)&SrvAddr, &len);
    */
	std::cout << "recv_length = " << receive_length << std::endl;//����Ϊ142
	cp = &inscp_ack;
	str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17����
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17����
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17����
	if(strcmp(DEVICE::get_instance()->manufacturer_name, temp_manufacturer_name) == 0)
	return 0;
	else
	{
		return -1;
	}
}




void str_to_arr(char* arr, const char* str, uint16_t len) {
	uint16_t i;
	for (i = 0; i < len; i++) {

			arr[i] = str[i];	

	}
	arr[i] = '\0';
}

void str_to_arr(char* arr, char* str, uint16_t len) {
	uint16_t i;
	for (i = 0; i < len; i++) {

		arr[i] = str[i];

	}
	arr[i] = '\0';
}

void setnonblocking1(int sockfd) {
	unsigned long on = 1;  //��ֵΪ0���Ὣ�׽�������Ϊ����(Ĭ�ϵ�)��Ϊ1����Ϊ������
	if (0 != ioctlsocket(sockfd, FIONBIO, &on))
	{
		/* Handle failure. */
	}
}


int INSCP::ins_init_ClistSock()
{
	WORD wVersionRequested;//�׽��ֿ�汾��
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);//�����׽��ֵİ汾��

	err = WSAStartup(wVersionRequested, &wsaData);//�����׽���
	if (err != 0) {
		return -1;
	}
	///�����׽���ʧ�ܴ���
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	ClistSock = socket(AF_INET, SOCK_DGRAM, 0);//�����׽�������

	const char* enableBroadcast = "1";
	if (setsockopt(ClistSock, SOL_SOCKET, SO_BROADCAST, enableBroadcast, sizeof(enableBroadcast)) == -1) {
		return -1;
	}
	setnonblocking1(ClistSock);
	ClistSock_is_init = true;
	return 0;
}


int INSCP::compare_NET_seg()
{
	if ((htonl(inet_addr(DEVICE::get_instance()->ip_addr)) & htonl(inet_addr(DEVICE::get_instance()->net_mask))) == (htonl(inet_addr(LOCALDEVICE::get_instance()->ip)) & htonl(inet_addr(LOCALDEVICE::get_instance()->net_mask))))
	{
		return 1;
		//std::cout << "��ͬһ����" << std::endl;
	}
	else {
		return 0;
		//std::cout << "����ͬһ����" << std::endl;
	}
}

int INSCP::close_soket()
{
	closesocket(ClistSock);
	WSACleanup();
	return 0;
}



//GetLocalIP
std::vector<std::string> get_adapters(std::vector<std::string>& _net_mask,std::vector<std::string>& _gate_way)
{
	std::vector<std::string> ips;
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{

			std::cout << "�������ƣ�" << pIpAdapterInfo->AdapterName << std::endl;
			std::cout << "����������" << pIpAdapterInfo->Description << std::endl;
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			int IPnumPerNetCard = 0;
			do
			{
				ips.push_back(std::string(pIpAddrString->IpAddress.String));// + std::string(" IpMask:") + std::string(pIpAddrString->IpMask.String));
				_gate_way.push_back(std::string(pIpAdapterInfo->GatewayList.IpAddress.String));
				_net_mask.push_back(std::string(pIpAddrString->IpMask.String));

				std::cout << "�������ϵ�IP������" << ++IPnumPerNetCard << std::endl;
				std::cout << "IP ��ַ��" << pIpAddrString->IpAddress.String << std::endl;
				std::cout << "������ַ��" << pIpAddrString->IpMask.String << std::endl;
				std::cout << "���ص�ַ��" << pIpAdapterInfo->GatewayList.IpAddress.String << std::endl;
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete[] pIpAdapterInfo;
	}
	return ips;
}