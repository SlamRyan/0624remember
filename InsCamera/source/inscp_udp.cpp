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
	if (str == NULL) {            //IP地址字符串为空
		printf("str == NULL\n");
		return -1;
	}
	slen = strlen(str);
	if (slen < 7 || slen > 15) {   //判断ip地址的长度是否正确，最小“X.X.X.X”,最大“XXX.XXX.XXX.XXX”
		printf("strlen(str) < 7 or strlen(str)>15\n");
		return -1;
	}
	if (str[0] == '.' || str[slen - 1] == '.') {  //判断时否为以“.”开头或者结尾
		printf("ip format error\n");
		return -1;
	}
	for (i = 0; i < slen; i++) {
		if (str[i] == '.') {            // 判断是否“X..X”,两个点相连
			if (str[i - 1] == '.') {
				printf("pStr == . format error\n");
				return -1;
			}
			if (++dotNum > 3) {   // 判断“."的个数是否大于3个以上
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
		if (str[i] < '0' || str[i] > '9') {  //ip内的字符是否为纯数字
			printf("ip addr error, str :%c\n", str[i]);
			return -1;
		}
		numTemp = numTemp * 10 + (str[i] - '0');  //计算单个地址的数值
		if (++bitNum > 3) {   //判断单个地址位数是否超过3位数
			printf("ip addr bit number error\n");
			return -1;
		}
	}
	if (dotNum != 3) {    //判断传入的字符串“.”的个数是否为3个
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
		SrvAddr.sin_addr.S_un.S_addr = inet_addr(DEVICE::get_instance()->ip_addr);//服务端的IP地址
	}
	else {
		//str_to_arr(DEVICE::get_instance()->ip, "255.255.255.255", strlen("255.255.255.255"));
		SrvAddr.sin_family = AF_INET;//选择地址族s
		SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//服务端的IP地址 //0312 DEVICE::get_instance()->ip_addr ->"255.255.255.255"
		SrvAddr.sin_port = htons(65535);//服务端的端口号
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
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//往服务端发送"hello"消息
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
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17新增
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17新增
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17新增

	
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
		closesocket(ClistSock); // 出错时确保关闭套接字
		return -1;
	}
	//-----------
	str_to_arr(DEVICE::get_instance()->ip_addr, "255.255.255.255", strlen("255.255.255.255"));
	SrvAddr.sin_family = AF_INET;//选择地址族s
	SrvAddr.sin_addr.S_un.S_addr = inet_addr(DEVICE::get_instance()->ip_addr);//服务端的IP地址
	SrvAddr.sin_port = htons(65535);//服务端的端口号

	
	CliAddr.sin_family = AF_INET;//选择地址族s
	CliAddr.sin_addr.S_un.S_addr = inet_addr(LOCALDEVICE::get_instance()->ip);//本地的IP地址
	CliAddr.sin_port = htons(56541);//本地的端口号
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
	
	sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//往服务端发送"hello"消息
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
			str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17新增
			str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17新增
			str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17新增

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
			strcpy(_spinsdeviceinfo->lightSouce.device_model, cp->device_model);//11.17新增
			strcpy(_spinsdeviceinfo->lightSouce.length, cp->length);//11.17新增
			strcpy(_spinsdeviceinfo->lightSouce.SN, cp->SN);//11.17新增
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
	std::vector<std::string> ipFamily = get_adapters(netMask,gateWay);//获取本机IP
	int i = 0;
	while (i< ipFamily.size())
	{
		if(ipFamily[i].find("0.0.0.0") == std::string::npos)//当IP存在
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
	if ((_num) < mDeviceInfo.size()) //num初值
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
		strcpy(_deviceInfo.lightSouce.device_model, mDeviceInfo[_num].lightSouce.device_model);//11.17新增
		strcpy(_deviceInfo.lightSouce.length, mDeviceInfo[_num].lightSouce.length);//11.17新增
		strcpy(_deviceInfo.lightSouce.SN, mDeviceInfo[_num].lightSouce.SN);//11.17新增
		return 0;
	}
	else
	{
		return -1;
	}
}

int INSCP::reset_device_ip(const char* ip1, Ins2D5LightDeviceAndLocalIP& _deviceInfo)//设置设备ip
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
		SrvAddr.sin_addr.S_un.S_addr = inet_addr(_deviceInfo.lightSouce.ip_addr);//服务端的IP地址
	}
	else {
		//str_to_arr(DEVICE::get_instance()->ip, "255.255.255.255", strlen("255.255.255.255"));
		SrvAddr.sin_family = AF_INET;//选择地址族s
		SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//服务端的IP地址
		SrvAddr.sin_port = htons(65535);//服务端的端口号
	}

	inscp_cmd.header = IP_SET_CMD;
	str_to_arr(inscp_cmd.manufacturer_name, "INS", strlen("INS"));
	str_to_arr(inscp_cmd.model_name, "2D5", strlen("2D5"));
	str_to_arr(inscp_cmd.device_vision, "0.0.0.1", strlen("0.0.0.1"));
	str_to_arr(inscp_cmd.device_model, "INS-CHVS-363-10GH", strlen("INS-CHVS-363-10GH"));
	str_to_arr(inscp_cmd.length, "30", strlen("30"));
	str_to_arr(inscp_cmd.SN, "A0100041000", strlen("A0100041000"));
	str_to_arr(inscp_cmd.ip_addr, ip.c_str(), strlen(ip.c_str()));                                                       //ip
	str_to_arr(inscp_cmd.net_mask, "255.255.255.0", strlen("255.255.255.0"));//子网掩码 255.255.255.0 //LOCALDEVICE::get_instance()->net_mask->"255,255,255.0"
	str_to_arr(inscp_cmd.gateway, "192.168.1.1", strlen("192.168.1.1"));                                                 //网关未连公网、网关DC(只要设备和本机IP在同一网段即可)
	str_to_arr(inscp_cmd.mac_addr, "08:8F:C3:FC:16:9F", strlen("08:8F:C3:FC:16:9F"));
	int request_num = 3;
	while (request_num)
	{
		request_num--;
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//往服务端发送"hello"消息
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
	str_to_arr(_deviceInfo.lightSouce.device_model, cp->device_model, strlen(cp->device_model));//11.17新增
	str_to_arr(_deviceInfo.lightSouce.length, cp->length, strlen(cp->length));//11.17新增
	str_to_arr(_deviceInfo.lightSouce.SN, cp->SN, strlen(cp->SN));//11.17新增

	{
	str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17新增
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17新增
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17新增
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

	//置零
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
	SrvAddr.sin_family = AF_INET;//选择地址族s
	SrvAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");//服务端的IP地址
	SrvAddr.sin_port = htons(65535);//服务端的端口号

	SOCKADDR_IN CliAddr;
	CliAddr.sin_family = AF_INET;//选择地址族s
	CliAddr.sin_addr.S_un.S_addr = inet_addr(LOCALDEVICE::get_instance()->ip);//本地的IP地址
	CliAddr.sin_port = htons(65534);//本地的端口号
	if (bind(ClistSock, (sockaddr*)&CliAddr, sizeof(CliAddr)) == -1) {
		perror("bind failed:");
	}

	int len = sizeof(SOCKADDR);

	int request_num = 3;
	while (request_num)
	{
		request_num--;
		sendto(ClistSock, (char*)(&inscp_cmd), sizeof(inscp_cmd), 0, (SOCKADDR*)&SrvAddr, len);//往服务端发送"hello"消息
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
	//等待数据
	std::this_thread::sleep_for(std::chrono::milliseconds(126));
	recv_length = recvfrom(ClistSock, (char*)&inscp_ack, sizeof(inscp_ack), 0, (SOCKADDR*)&SrvAddr, &len);
    */
	std::cout << "recv_length = " << receive_length << std::endl;//正常为142
	cp = &inscp_ack;
	str_to_arr(DEVICE::get_instance()->ip_addr, cp->ip_addr, strlen(cp->ip_addr));
	str_to_arr(DEVICE::get_instance()->net_mask, cp->net_mask, strlen(cp->net_mask));
	str_to_arr(DEVICE::get_instance()->gateway, cp->gateway, strlen(cp->gateway));
	str_to_arr(DEVICE::get_instance()->mac_addr, cp->mac_addr, strlen(cp->mac_addr));
	str_to_arr(DEVICE::get_instance()->manufacturer_name, cp->manufacturer_name, strlen(cp->manufacturer_name));
	str_to_arr(DEVICE::get_instance()->device_vision, cp->device_vision, strlen(cp->device_vision));
	str_to_arr(DEVICE::get_instance()->model_name, cp->model_name, strlen(cp->model_name));
	str_to_arr(DEVICE::get_instance()->device_model, cp->device_model, strlen(cp->device_model));//11.17新增
	str_to_arr(DEVICE::get_instance()->length, cp->length, strlen(cp->length));//11.17新增
	str_to_arr(DEVICE::get_instance()->SN, cp->SN, strlen(cp->SN));//11.17新增
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
	unsigned long on = 1;  //此值为0，会将套接字设置为阻塞(默认的)；为1设置为非阻塞
	if (0 != ioctlsocket(sockfd, FIONBIO, &on))
	{
		/* Handle failure. */
	}
}


int INSCP::ins_init_ClistSock()
{
	WORD wVersionRequested;//套接字库版本号
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);//定义套接字的版本号

	err = WSAStartup(wVersionRequested, &wsaData);//创建套接字
	if (err != 0) {
		return -1;
	}
	///创建套接字失败处理
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	ClistSock = socket(AF_INET, SOCK_DGRAM, 0);//创建套接字类型

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
		//std::cout << "在同一网段" << std::endl;
	}
	else {
		return 0;
		//std::cout << "不在同一网段" << std::endl;
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
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	//记录网卡数量
	int netCardNum = 0;
	//记录每张网卡上的IP地址数量
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	}
	if (ERROR_SUCCESS == nRel)
	{
		//输出网卡信息
		//可能有多网卡,因此通过循环去判断
		while (pIpAdapterInfo)
		{

			std::cout << "网卡名称：" << pIpAdapterInfo->AdapterName << std::endl;
			std::cout << "网卡描述：" << pIpAdapterInfo->Description << std::endl;
			IP_ADDR_STRING* pIpAddrString = &(pIpAdapterInfo->IpAddressList);
			int IPnumPerNetCard = 0;
			do
			{
				ips.push_back(std::string(pIpAddrString->IpAddress.String));// + std::string(" IpMask:") + std::string(pIpAddrString->IpMask.String));
				_gate_way.push_back(std::string(pIpAdapterInfo->GatewayList.IpAddress.String));
				_net_mask.push_back(std::string(pIpAddrString->IpMask.String));

				std::cout << "该网卡上的IP数量：" << ++IPnumPerNetCard << std::endl;
				std::cout << "IP 地址：" << pIpAddrString->IpAddress.String << std::endl;
				std::cout << "子网地址：" << pIpAddrString->IpMask.String << std::endl;
				std::cout << "网关地址：" << pIpAdapterInfo->GatewayList.IpAddress.String << std::endl;
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete[] pIpAdapterInfo;
	}
	return ips;
}