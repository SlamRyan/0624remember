#include <cstring>
#include <stdint.h>
#include <thread> 
#include <chrono> // 用于std::chrono命名空间和时间持续类型
#include <ratio>  // 用于处理std::chrono::duration的时间单位比率
#include <string> 
#include <cerrno>

#include "../include/modbus_udp.h"
#include "../include/device.h"
//#include "winsock.h"
//#include <WinSock2.h>


__int32 modbus_udp::tran_num = -1;

/*
void PrintTime() {
    
    auto currentTime = std::chrono::system_clock::now();

    // 将时间戳转换为时间结构
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm* localTime = std::localtime(&currentTime_t);

    // 精确到毫秒的时间
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
    /*
    // 输出时间戳
    std::cout << "当前时间戳: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count()
        << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;

// 输出格式化的时间，精确到毫秒
    std::cout << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;
}
*/
void setnonblocking(int sockfd) {                                                                                                                                                                                                                                                                                                      
    unsigned long on = 1;  //此值为0，会将套接字设置为阻塞(默认的)；为1设置为非阻塞
    if (0 != ioctlsocket(sockfd, FIONBIO, &on))
    {
        std::cout << "设置非阻塞失败";
        /* Handle failure. */
    }
}

double InsPMDL_lightSouce::Calculate16kLinePeriod(double x) //0408
{
	double q = x - static_cast<int>(x);//
	int n = static_cast<int>(q / 0.2);
	double y = static_cast<int>(x) + n * 0.2;
	int p = static_cast<int>((x - y) / 0.047);
	double r = (x - y) - p * 0.047;
	double offSet;
	if (r <= 0.012)
	{
		offSet = 0.012;
	}
	else if (0.012 < r <= 0.024)
	{
		offSet = 0.024;
	}
	else if (0.024< r <= 0.035)
	{
		offSet = 0.035;
	}
	else 
	{
		offSet = 0.047;
	}
	double z = p * 0.047 + y + offSet;

	return z;
}

double InsPMDL_lightSouce::Calculate8kLinePeriod(double x)
{
	int S = static_cast<int>((x - 4.5) / 0.025);
	double  P = (S * 0.025 + 4.5);
	return P;
}

int modbus_udp::initmodbus_udp(const char* host1, const char* local_ip, uint16_t port) {
    std::string host(host1);
   
    //加载套接字
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
        return 1;
    }
    setnonblocking(ClistSock);
    memset(&SrvAddr, 0, sizeof(SrvAddr));
    SrvAddr.sin_family = AF_INET;//选择地址族s
    //SrvAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.11");//服务端的IP地址
    SrvAddr.sin_addr.S_un.S_addr = inet_addr(host.c_str());//服务端的IP地址
    SrvAddr.sin_port = htons(port);//服务端的端口号

    memset(&CliAddr, 0, sizeof(CliAddr));
    CliAddr.sin_family = AF_INET;//选择地址族s
    CliAddr.sin_addr.S_un.S_addr = inet_addr(local_ip);//本地的IP地址
    CliAddr.sin_port = htons(502);//本地的端口号
    errno = 0;
    if (bind(ClistSock, (sockaddr*)&CliAddr, sizeof(CliAddr)) == -1) {
        printf("Bind failed with error code: %d\n", errno);//12.07新增
        perror("bind failed:");
        return 2;
    }
    modbus_udp::_connected = true;
    return 0;
}

int modbus_udp::init_ins_modbus_udp(const char* host1, const char* local_ip, uint16_t port) {
	std::string host(host1);

	//加载套接字
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
		return 1;
	}
	setnonblocking(ClistSock);
	memset(&SrvAddr, 0, sizeof(SrvAddr));
	SrvAddr.sin_family = AF_INET;//选择地址族s
	//SrvAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.11");//服务端的IP地址
	SrvAddr.sin_addr.S_un.S_addr = inet_addr(host.c_str());//服务端的IP地址
	SrvAddr.sin_port = htons(port);//服务端的端口号

	memset(&CliAddr, 0, sizeof(CliAddr));
	CliAddr.sin_family = AF_INET;//选择地址族s
	CliAddr.sin_addr.S_un.S_addr = inet_addr(local_ip);//本地的IP地址
	CliAddr.sin_port = htons(65531);//本地的端口号
	errno = 0;
	if (bind(ClistSock, (sockaddr*)&CliAddr, sizeof(CliAddr)) == -1) {
		printf("Bind failed with error code: %d\n", errno);//12.07新增
		perror("bind failed:");
		return 2;
	}
	modbus_udp::_connected = true;
	return 0;
}




void modbus_udp::modbus_udp_close() const 
{

    closesocket(ClistSock);
    WSACleanup();
    modbus_udp::_connected == false;
}

int modbus_udp::modbus_udp_write_single_register(BoardID id, uint16_t addr, uint16_t* value)
{
    uint8_t send_data[SENDBUFFERNUM] = { 0 };
    //__int16 recv_data;
    tran_num++;
    int reg_num = 1;
    //unsigned char recvBuf[100] = { 0 };//10.10 __int8
    __int16 length = 2 * reg_num + 4;
    send_data[0] = tran_num >> 8;
    send_data[1] = tran_num;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = length >> 8;
    send_data[5] = length;
    send_data[6] = id;
    send_data[7] = 0x06;
    send_data[8] = addr >> 8;
    send_data[9] = addr;
    int i = 0;
    for (i = 0;i < reg_num * 2; i += 2)
    {
        send_data[i + 10] = value[i / 2] >> 8;
        send_data[i + 11] = value[i / 2];
    }
    sendto(ClistSock, (char*)send_data, i + 13, 0, (SOCKADDR*)&SrvAddr, len);
    std::this_thread::sleep_for(std::chrono::microseconds(MODBUS_SLEEP_TIME_US));
    int a = continous_receive(send_data, value, send_data[7]);
    return a;
}

int modbus_udp::modbus_udp_write_registers(BoardID id,uint16_t addr, uint16_t reg_num, const uint16_t* value) 
{
    
    uint8_t send_data[SENDBUFFERNUM] = { 0 };
   // __int16 recv_data;
    uint16_t buffer[2] = {0};
    tran_num++;
    __int16 length = 2 * reg_num + 7;
    send_data[0] = tran_num >> 8;
    send_data[1] = tran_num;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = length >> 8;
    send_data[5] = length;
    send_data[6] = id;
    send_data[7] = 0x10;
    send_data[8] = addr >> 8;
    send_data[9] = addr;
    send_data[10] = reg_num >> 8;
    send_data[11] = reg_num;
    send_data[12] = reg_num * 2;
    int i = 0;
    for (i = 0;i < reg_num * 2; i += 2)
    {
        send_data[i + 13] = value[i / 2] >> 8;
        send_data[i + 14] = value[i / 2];
    }
    //PrintTime("write_registers:");
    sendto(ClistSock, (char*)send_data, i + 13, 0, (SOCKADDR*)&SrvAddr, len);
    std::this_thread::sleep_for(std::chrono::microseconds(MODBUS_SLEEP_TIME_US));
    //PrintTime("start continous_receive:");
    int a=continous_receive(send_data, buffer, 0x10);
    //PrintTime("end continous_receive:");
    return a;
}


//在线升级,端口号65530
int modbus_udp::update_udp_write_registers(BoardID id, uint16_t addr, uint16_t reg_num, const uint16_t* value)
{   
    if (is_connected() != true)
    {
        initmodbus_udp(DEVICE::get_instance()->ip_addr,LOCALDEVICE::get_instance()->ip, 65530);
        modbus_udp::_connected == true;
    }
	uint8_t send_data[SENDBUFFERNUM] = { 0 };
	// __int16 recv_data;
	uint16_t buffer[2] = { 0 };
	tran_num++;
	__int16 length = 2 * reg_num + 7;
	send_data[0] = tran_num >> 8;
	send_data[1] = tran_num;
	send_data[2] = 0x00;
	send_data[3] = 0x00;
	send_data[4] = length >> 8;
	send_data[5] = length;
	send_data[6] = id;
	send_data[7] = 0x10;
	send_data[8] = addr >> 8;
	send_data[9] = addr;
	send_data[10] = reg_num >> 8;
	send_data[11] = reg_num;
	send_data[12] = reg_num * 2 >> 8;
	send_data[13] = reg_num * 2;
	int i = 0;
	for (i = 0; i < reg_num * 2; i += 2)
	{
		send_data[i + 14] = value[i / 2];
		send_data[i + 15] = value[i / 2] >> 8;
	}
	//PrintTime("write_registers:");
	sendto(ClistSock, (char*)send_data, i + 14, 0, (SOCKADDR*)&SrvAddr, len);
	std::this_thread::sleep_for(std::chrono::microseconds(MODBUS_SLEEP_TIME_US));
	//PrintTime("start continous_receive:");
	int a = continous_receive(send_data, buffer, 0x10);
	//PrintTime("end continous_receive:");
	return a;
}

int modbus_udp::set_timeout(int _time)
{
	timeout_ = _time;
	return 0;
}


int modbus_udp::modbus_udp_read_registers(BoardID id, uint16_t addr, uint16_t amount, uint16_t* buffer) {
    uint8_t send_data[SENDBUFFERNUM] = { 0 };
    //uint16_t recv_data;
    tran_num++;
    //10.10 __int8
    send_data[0] = tran_num >> 8;
    send_data[1] = tran_num;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0x06;
    send_data[6] = id;
    send_data[7] = 0x03;
    send_data[8] = addr >> 8;
    send_data[9] = addr;
    send_data[10] = amount >> 8;
    send_data[11] = amount;
    int _senderror = 0;
    _senderror = sendto(ClistSock, (char*)send_data, 12, 0, (SOCKADDR*)&SrvAddr, len);
    if (_senderror <= 0)//12.07
    {
        std::cout<< "_sendtoerror"<< _senderror << std::endl;
    }
    int res = WSAGetLastError();
	if (res != 0)//12.07
	{
		std::cout << "WSAGetLastError" << res << std::endl;
	}
    std::this_thread::sleep_for(std::chrono::microseconds(MODBUS_SLEEP_TIME_MS));
    int a = continous_receive(send_data,buffer, 0x03);
    return a;
}

int modbus_udp::modbuserror_handle(unsigned char* msg, int func)
{
    error_msg = "NO ERR";
    if (msg[7] == func)
    {
        err = false;
        return 0;
    }
    if (msg[7] == func + 0x80)
    {
        err = true;
        //return 1;
        switch (msg[8])
        {
        case EX_ILLEGAL_FUNCTION:
            error_msg = "1 Illegal Function";
            return EX_ILLEGAL_FUNCTION;
            break;
        case EX_ILLEGAL_ADDRESS:
            error_msg = "2 Illegal Address";
            return EX_ILLEGAL_ADDRESS;
            break;
        case EX_ILLEGAL_VALUE:
            error_msg = "3 Illegal Value";
            return EX_ILLEGAL_VALUE;
            break;
        case EX_SERVER_FAILURE:
            error_msg = "4 Server Failure";
            return EX_SERVER_FAILURE;
            break;
        case EX_ACKNOWLEDGE:
            error_msg = "5 Acknowledge";
            return EX_ACKNOWLEDGE;
            break;
        case EX_SERVER_BUSY:
            error_msg = "6 Server Busy";
            return EX_SERVER_BUSY;
            break;
        case EX_NEGATIVE_ACK:
            error_msg = "7 Negative Acknowledge";
            return EX_NEGATIVE_ACK;
            break;
        case EX_MEM_PARITY_PROB:
            error_msg = "8 Memory Parity Problem";
            return EX_MEM_PARITY_PROB;
            break;
        case EX_GATEWAY_PROBLEMP:
            error_msg = "10 Gateway Path Unavailable";
            return EX_GATEWAY_PROBLEMP;
            break;
        case EX_GATEWAY_PROBLEMF:
            error_msg = "11 Gateway Target Device Failed to Respond";
            return EX_GATEWAY_PROBLEMF;
            break;
        default:
            error_msg = "UNKNOWN";
            return MAXINT;
            break;
        }
    }

}

int modbus_udp::continous_receive(uint8_t (&send_data)[SENDBUFFERNUM], uint16_t* buffer, int func)
{
    unsigned char recvBuf[100] = { 0 };
    uint16_t recv_data = 0;
    int res = 0;
    int recv_length = 0;
    int ret = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    while (true)
    {
        recv_length = recvfrom(ClistSock, (char*)(&recvBuf), sizeof(recvBuf), 0, (SOCKADDR*)&SrvAddr, &len);

        ret = WSAGetLastError();
        if (res == SOCKET_ERROR)
        {
            int derr;
            derr = WSAGetLastError();
            if (derr != WSAECONNRESET)
            {
                std::string str;
                printf("recvfrom: error = %d\n", derr);
            }
            else
                return SOCKET_ERROR;
        }

        //if ( recv_length != SOCKET_ERROR &&recv_length != 0xffffffff )
        //{
        if (recv_length != SOCKET_ERROR && recv_length != 0xffffffff && recvBuf[0] == send_data[0] && recvBuf[1] == send_data[1])
        {
            ret = 0;
            ret = modbuserror_handle(recvBuf, func);
            //std::cout <<"ret:" << ret << std::endl;
            if (ret == 0)
            {
                if (func == 0x03 && send_data[11] == 2)
                {
                    //std::cout << func << std::endl;
                    recv_data = (uint16_t)recvBuf[9] << 8 | (uint16_t)recvBuf[10];
                    *buffer = recv_data;
                    recv_data = (uint16_t)recvBuf[11] << 8 | (uint16_t)recvBuf[12];
                    *(buffer + 1) = recv_data;
                    return 0;
                }
                else if (func == 0x03 && send_data[11] == 1)
                {
                    recv_data = (uint16_t)recvBuf[9] << 8 | (uint16_t)recvBuf[10];
                    *buffer = recv_data;
                    return 0;
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                std::cout << "modbuserror_handle:" << ret;
                return ret;
            }
        }
        //else
        //{
       //     std::cout << "收到的并非本指令对应回应";
        //}
    
        auto curTime = std::chrono::high_resolution_clock::now();
        int interval = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime).count();
        if ((interval >= timeout_)&& (timeout_ > 0))//如果TIMEOUT < = 0 ,则永久阻塞，不return
        {
            return 10086;
        }
        
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(MODBUS_SLEEP_TIME_US));
            //recv_length = recvfrom(ClistSock, (char*)(&recvBuf), sizeof(recvBuf), 0, (SOCKADDR*)&SrvAddr, &len);

        }
    }
}



Ins_Modbus_udp* Ins_Modbus_udp::get_instance()
{
    if (mpIns_Modbus_udp == nullptr)
    {
        mpIns_Modbus_udp = new Ins_Modbus_udp();
    }
    return mpIns_Modbus_udp;
}

int Ins_Modbus_udp::ins_modbus_udp_write(BoardID id, uint16_t address, uint16_t* buffer)
{

    if (mpIns_Modbus_udp)
    {
        return mpIns_Modbus_udp->modbus_udp_write_registers(id, address, 2, buffer);
    }
    else
        return 0;
}

int Ins_Modbus_udp::ins_modbus_udp_read(BoardID id, uint16_t address, uint16_t* buffer)
{
    if (mpIns_Modbus_udp) {
        return mpIns_Modbus_udp->modbus_udp_read_registers(id, address, 2, buffer);
    }
    else
        return 0;
}


/*
int Ins_Modbus_udp::update_udp_write(BoardID id, uint16_t address, uint16_t reg_num, uint16_t* buffer)
{

    if (mpIns_Modbus_udp)
    {
        return mpIns_Modbus_udp->update_udp_write_registers(id, address, reg_num, buffer);
    }
    else
        return 0;
}
*/


Update_Modbus_udp* Update_Modbus_udp::get_instance()
{
    if (mpUpdate_Modbus_udp == nullptr)
    {
        mpUpdate_Modbus_udp = new Update_Modbus_udp();
    }

    return mpUpdate_Modbus_udp;
}


int Update_Modbus_udp::update_udp_write(BoardID id, uint16_t address, uint16_t reg_num, uint16_t* buffer)
{

    if (mpUpdate_Modbus_udp)
    {
        return mpUpdate_Modbus_udp->update_udp_write_registers(id, address, reg_num, buffer);
    }
    else
        return 0;
}

/*
bool Ins_Modbus::ins_modbus_init(std::string ip) {
    // create a modbus object
    //initmodbus("192->168->1->11", 502);
    if (mpIns_Modbus) {
        mpIns_Modbus->initmodbus(ip, 502);
        // set slave id
        mpIns_Modbus->modbus_set_slave_id(1);

        return mpIns_Modbus->modbus_connect();
    }

    // connect with the server
    return false;
}

int Ins_Modbus::ins_modbus_write(uint16_t address, uint16_t* buffer)
{

    if (mpIns_Modbus)
    {
        return mpIns_Modbus->modbus_write_registers(address, 2, buffer);
    }
    else
        return 0;
}

int Ins_Modbus::ins_modbus_read(uint16_t address, uint16_t* buffer) {
    if (mpIns_Modbus) {
        return mpIns_Modbus->modbus_read_holding_registers(address, 2, buffer);
    }
    else
        return 0;
}
*/

InsResult InsPMDL_lightSouce::init_communication()
{
    return InsResult(initmodbus_udp(mDevice.lightSouce.ip_addr,mDevice.localIP));
}

InsResult InsPMDL_lightSouce::ins_write(BoardID id, uint16_t address, uint16_t* buffer)
{
    return (InsResult)modbus_udp_write_registers(id, address, 2, buffer);
}

InsResult InsPMDL_lightSouce::ins_write_Laser(BoardID id, uint16_t address, uint16_t* buffer)//0419 激光写一个寄存器
{
	return (InsResult)modbus_udp_write_registers(id, address, 1, buffer);
}

InsResult InsPMDL_lightSouce::ins_read(BoardID id, uint16_t address, uint16_t* buffer)
{
    return (InsResult)modbus_udp_read_registers(id, address, 2, buffer);
}

InsResult InsPMDL_lightSouce::read_register(BoardID _id, uint16_t _address, uint32_t& _buffer)
{
    uint16_t buffer1[2] = { 0 };
    InsResult ret = ins_read(_id, _address, buffer1);
    _buffer = (static_cast<uint32_t>(buffer1[0]) << 16) | (static_cast<uint32_t>(buffer1[1]));
    return ret;
}

InsResult InsPMDL_lightSouce::write_register(BoardID _id, uint16_t _address, uint32_t _buffer)
{
    uint32_t valueToWrite = _buffer;
    uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
    uint16_t lowValue = valueToWrite & 0xFFFF;
    uint16_t buffer1[2] = { highValue,lowValue };
    return ins_write((BoardID)_id, _address, buffer1);
}

InsResult InsPMDL_lightSouce::write_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer)
{
	return (InsResult)modbus_udp_write_registers(_id, _address, reg_num, _buffer);

}

InsResult InsPMDL_lightSouce::read_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer)
{
	return (InsResult)modbus_udp_read_registers(_id, _address, reg_num, _buffer);
}
InsResult InsPMDL_lightSouce::save_SN(uint16_t* _buffer)
{
	write_registers(INTSTM32, 0x14, 32, _buffer);
	uint16_t temp = 1;
	modbus_udp_write_registers(INTSTM32, 0, 1, &temp);
	int num = 3;
	while (temp != 0 && num > 0) {
		Sleep(100);
		num--;
		modbus_udp_read_registers(INTSTM32, 0, 1, &temp);
	}

	if (temp == 0)
	{
		return (InsResult)0;
	}
	return (InsResult)-1;

}

InsResult InsPMDL_lightSouce::read_SN(uint16_t* _buffer)
{
	std::this_thread::sleep_for(std::chrono::microseconds(100));
	read_registers(INTSTM32, 0x14, 1, _buffer);
	read_registers(INTSTM32, 0x14, 1, _buffer);
	read_registers(INTSTM32, 0x14, 1, _buffer);

	for (int i = 0; i < 32; i++)
	{
		read_registers(INTSTM32, 0x14 + i, 1, _buffer + i);
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}
	return (InsResult)0;
}
//-------------------------------工具----------------------
const int Zeroz = 0;
const int Onez = 1;
const int Twoz = 2;
const int Threez = 3;
const int Fourz = 4;
const int Fivez = 5;
const int Sixz = 6;
const int Sevenz = 7;
//和触发有关的只写一块板，和条纹有关的要写三块版
#define CHECKBOARDNUM(x) if( x!= 1 && x!= 3 && x!= 5){return -1;}//to do InsResult


inline int InsPMDL_lightSouce::readFromModbusOne(uint16_t regAdd, uint32_t& value)
{
	uint16_t bufferRead[2] = {};
	int ret = 0;
	ret = (int)ins_read(FIRSTBOARD, regAdd, bufferRead);
	value = (static_cast<uint32_t>(bufferRead[0]) << 16) | bufferRead[1];
	return  ret;
}

inline void InsPMDL_lightSouce::writeToModbusFour(uint16_t regAdd, uint32_t value)//1206写入FiveReg
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
	ins_write(SECONDBOARD, regAdd, buffer);//2
	ins_write(THIRDBOARD, regAdd, buffer);//3
	ins_write(FOURTHBOARD, regAdd, buffer);//4
}

inline void InsPMDL_lightSouce::writeToModbusSix(uint16_t regAdd, uint32_t value)//1206写入FiveReg
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
	ins_write(SECONDBOARD, regAdd, buffer);//2
	ins_write(THIRDBOARD, regAdd, buffer);//3
	ins_write(FOURTHBOARD, regAdd, buffer);//4
	ins_write(FIFTHBOARD, regAdd, buffer);//5
	ins_write(SIXTHBOARD, regAdd, buffer);//6
}
inline void InsPMDL_lightSouce::writeToModbusFive(uint16_t regAdd, uint32_t value)//1206写入FiveReg
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
	ins_write(SECONDBOARD, regAdd, buffer);//2
	ins_write(THIRDBOARD, regAdd, buffer);//3
	ins_write(FOURTHBOARD, regAdd, buffer);//4
	ins_write(FIFTHBOARD, regAdd, buffer);//5
}
inline void InsPMDL_lightSouce::writeToModbusThree(uint16_t regAdd, uint32_t value)//1206写入ThreeReg
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
	ins_write(SECONDBOARD, regAdd, buffer);//2
	ins_write(THIRDBOARD, regAdd, buffer);//3
}
inline void InsPMDL_lightSouce::writeToModbusOne(uint16_t regAdd, uint32_t value)//1206写入OneReg
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
}

inline void InsPMDL_lightSouce::writeToModbusTwo(uint16_t regAdd, uint32_t value)//
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, regAdd, buffer);//1
	ins_write(SECONDBOARD, regAdd, buffer);//2
}

inline void InsPMDL_lightSouce::writeToModbusSecondThird(uint16_t regAdd, uint32_t value)
{
	uint16_t highValue = (value >> 16) & 0xFFFF;
	uint16_t lowValue = value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(SECONDBOARD, regAdd, buffer);
	ins_write(THIRDBOARD, regAdd, buffer);

}
int InsPMDL_lightSouce::fpga_device_initialization(int _board_num)
{
	int LightSignInt = _board_num * 10;
	uint32_t IniNum1 = 1;
	uint32_t IniNum0 = 0;

	uint16_t highValue = (IniNum1 >> 16) & 0xFFFF;
	uint16_t lowValue = IniNum1 & 0xFFFF;
	uint16_t Inibuffer2[2] = { highValue,lowValue };//1

	uint16_t highValue1 = (IniNum0 >> 16) & 0xFFFF;
	uint16_t lowValue1 = IniNum0 & 0xFFFF;
	uint16_t Inibuffer1[2] = { highValue1,lowValue1 };//0

	if (LightSignInt == 10)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);//0
	}

	else if (LightSignInt == 20)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);//0

		ins_write(SECONDBOARD, 0x1100, Inibuffer2);
		ins_write(SECONDBOARD, 0x0B30, Inibuffer2);//1

	}

	else if (LightSignInt == 30)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);//0

		ins_write(SECONDBOARD, 0x1100, Inibuffer2);
		ins_write(SECONDBOARD, 0x0B30, Inibuffer2);//1

		ins_write(THIRDBOARD, 0x1100, Inibuffer2);
		ins_write(THIRDBOARD, 0x0B30, Inibuffer2);//1

		//test0020
		//writeToModbusThree(0x0010, 1234);
	}

	else if (LightSignInt == 40)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);//0

		ins_write(SECONDBOARD, 0x1100, Inibuffer2);
		ins_write(SECONDBOARD, 0x0B30, Inibuffer2);//1

		ins_write(THIRDBOARD, 0x1100, Inibuffer2);
		ins_write(THIRDBOARD, 0x0B30, Inibuffer2);//1

		ins_write(FOURTHBOARD, 0x1100, Inibuffer2);
		ins_write(FOURTHBOARD, 0x0B30, Inibuffer2);//1

	}


	else if (LightSignInt == 50)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);

		ins_write(SECONDBOARD, 0x1100, Inibuffer2);
		ins_write(SECONDBOARD, 0x0B30, Inibuffer2);

		ins_write(THIRDBOARD, 0x1100, Inibuffer2);
		ins_write(THIRDBOARD, 0x0B30, Inibuffer2);

		ins_write(FOURTHBOARD, 0x1100, Inibuffer2);
		ins_write(FOURTHBOARD, 0x0B30, Inibuffer2);

		ins_write(FIFTHBOARD, 0x1100, Inibuffer2);
		ins_write(FIFTHBOARD, 0x0B30, Inibuffer2);
		
	}

	else if (LightSignInt == 60)
	{
		ins_write(FIRSTBOARD, 0x1100, Inibuffer2);
		ins_write(FIRSTBOARD, 0x0B30, Inibuffer1);

		ins_write(SECONDBOARD, 0x1100, Inibuffer2);
		ins_write(SECONDBOARD, 0x0B30, Inibuffer2);

		ins_write(THIRDBOARD, 0x1100, Inibuffer2);
		ins_write(THIRDBOARD, 0x0B30, Inibuffer2);

		ins_write(FOURTHBOARD, 0x1100, Inibuffer2);
		ins_write(FOURTHBOARD, 0x0B30, Inibuffer2);

		ins_write(FIFTHBOARD, 0x1100, Inibuffer2);
		ins_write(FIFTHBOARD, 0x0B30, Inibuffer2);

		ins_write(SIXTHBOARD, 0x1100, Inibuffer2);
		ins_write(SIXTHBOARD, 0x0B30, Inibuffer2);

	}

	//---------------0314
	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1014, 1);//2028
		writeToModbusOne(0x1016, 0);//202C
	}

	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1014, 1);//2028
		writeToModbusTwo(0x1016, 0);//202C
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1014, 1);//2028
		writeToModbusThree(0x1016, 0);//202C
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1014, 1);//2028
		writeToModbusFour(0x1016, 0);//202C
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1014, 1);//2028
		writeToModbusFive(0x1016, 0);//202C
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1014, 1);//2028
		writeToModbusSix(0x1016, 0);//202C
	}

	//--------------

	//软触发 1221
	uint32_t IniSoftTriPer = 125;

	if (LightSignInt == 10)
	{

		writeToModbusOne(0xB00, IniSoftTriPer);//周期
		writeToModbusOne(0x0B02, Zeroz);//次数

	}

	else if (LightSignInt == 20)
	{

		writeToModbusTwo(0x0B00, IniSoftTriPer);
		writeToModbusTwo(0x0B02, Zeroz);
	}

	else if (LightSignInt == 30)
	{

		writeToModbusThree(0x0B00, IniSoftTriPer);
		writeToModbusThree(0x0B02, Zeroz);
	}

	else if (LightSignInt == 40)
	{

		writeToModbusFour(0x0B00, IniSoftTriPer);
		writeToModbusFour(0x0B02, Zeroz);
	}

	else if (LightSignInt == 50)
	{

		writeToModbusFive(0x0B00, IniSoftTriPer);
		writeToModbusFive(0x0B02, Zeroz);
	}

	else if (LightSignInt == 60)
	{

		writeToModbusSix(0x0B00, IniSoftTriPer);
		writeToModbusSix(0x0B02, Zeroz);
	}

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



	ins_read(FIRSTBOARD, 0x0880, REGMOSACK);//1100
	ins_read(FIRSTBOARD, 0x0882, REGDDSACK);//1104
	ins_read(FIRSTBOARD, 0x1008, REGPICNUM);//2010


	ins_read(FIRSTBOARD, 0x100E, REGSWGAP);//201c
	ins_read(FIRSTBOARD, 0x1006, REGTRIGGAP);//200c
	ins_read(FIRSTBOARD, 0x1010, REGSWLOOPNUM);//2020
	ins_read(FIRSTBOARD, 0x0C0A, REGCAMTRINUM);//1814





	MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) << 16) | REGMOSACK[1];
	DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) << 16) | REGDDSACK[1];
	PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) << 16) | REGPICNUM[1];

	SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) << 16) | REGSWGAP[1];
	TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) << 16) | REGTRIGGAP[1];
	SWLoopNumZ = (static_cast<uint32_t>(REGSWLOOPNUM[0]) << 16) | REGSWLOOPNUM[1];
	CamTriNumZ = (static_cast<uint32_t>(REGCAMTRINUM[0]) << 16) | REGCAMTRINUM[1];
	return 0;
}


int InsPMDL_lightSouce::get_other_info(OtherDeviceInfo* _stOtherInfo)
{
	//版本号
	uint16_t bufferRead[2] = {};
	ins_read(FIRSTBOARD, 0x000C, bufferRead);

	uint32_t combinValue = (static_cast<uint32_t>(bufferRead[0]) << 16) | bufferRead[1];

	char buffer[9]; // Assuming 32-bit value, so 8 characters plus null terminator
	sprintf(buffer, "%X", combinValue);
	_stOtherInfo->version_number_ = _strdup(buffer);


	//产品信息
	uint16_t bufferRead2[2] = {};
	ins_read(FIRSTBOARD, 0x0006, bufferRead2);
	uint32_t combinValue2 = (static_cast<uint32_t>(bufferRead2[0]) << 16) | bufferRead2[1];
	// 计算 combinValue2 的十六进制字符串长度（包括终止符）
	int hexStringLength = snprintf(nullptr, 0, "%X", combinValue2) + 1;

	// 分配内存用于存储十六进制字符串
	char* hexString = new char[hexStringLength];

	// 格式化 combinValue2 到十六进制字符串
	snprintf(hexString, hexStringLength, "%X", combinValue2);
	_stOtherInfo->product_information_ = hexString;
	//QString Read2 = QString::number(combinValue2, 16).toUpper().rightJustified(8, '0');//ASCII

	//-----------------------------------
	uint16_t MBbuffer[2] = {};
	ins_read(FIRSTBOARD, 0x4002, MBbuffer);//0320
	uint32_t MBcombin2 = (static_cast<uint32_t>(MBbuffer[0]) << 16) | MBbuffer[1];


	int hexStringLength1 = snprintf(nullptr, 0, "%X", MBcombin2) + 1;

	// 分配内存用于存储十六进制字符串
	char* hexString1 = new char[hexStringLength1];

	// 格式化 combinValue2 到十六进制字符串
	snprintf(hexString1, hexStringLength1, "%X", MBcombin2);
	_stOtherInfo->microblaze_vertion =hexString1; 

	//ui->insName_3->setText(Read2);
    //------------------------------------------

	//设备信息
	uint16_t bufferRead3[2] = {};
	ins_read(FIRSTBOARD, 0x0014, bufferRead3);
	uint32_t combinValue3 = (static_cast<uint32_t>(bufferRead3[0]) << 16) | bufferRead3[1];
	char buffer2[9]; // Assuming 32-bit value, so 8 characters plus null terminator
	sprintf(buffer2, "%X", combinValue);
	_stOtherInfo->device_information_ = _strdup(buffer2);

	//FPGA版本生成日期
	uint16_t bufferRead4[2] = {};
	//ins_read(0x0010, bufferRead4);//West
	ins_read(FIRSTBOARD, 0x0008, bufferRead4);
	uint32_t combinValue4 = (static_cast<uint32_t>(bufferRead4[0]) << 16) | bufferRead4[1];

	uint16_t bufferRead5[2] = {};
	//ins_read(0x0014, bufferRead5);
	ins_read(FIRSTBOARD, 0x000A, bufferRead5);
	uint32_t combinValue5 = (static_cast<uint32_t>(bufferRead5[0]) << 8) | bufferRead5[1];





	// 计算 combinValue4 的十六进制字符串长度（包括终止符）
	int hexStringLength4 = snprintf(nullptr, 0, "%X", combinValue4) + 1;

	// 计算 combinValue5 的十六进制字符串长度（包括终止符）
	int hexStringLength5 = snprintf(nullptr, 0, "%X", combinValue5) + 1;

	// 分配内存用于存储十六进制字符串
	char* hexString4 = new char[hexStringLength4];
	char* hexString5 = new char[hexStringLength5];

	// 格式化 combinValue4 和 combinValue5 到十六进制字符串
	snprintf(hexString4, hexStringLength4, "%X", combinValue4);
	snprintf(hexString5, hexStringLength5, "%X", combinValue5);

	// 计算需要的总长度（包括可能的分隔符和终止符）
	int totalLength = hexStringLength4 + hexStringLength5 + 2;  // 2 是两个空格

	// 分配内存用于存储最终的字符串
	char* combinedText = new char[totalLength];

	// 将两个字符串拼接到 combinedText 中
	snprintf(combinedText, totalLength, "%s %s", hexString4, hexString5);

	//combinedText 包含了 combinValue4 和 combinValue5 的十六进制字符串，中间有一个空格

	// 执行深拷贝
	_stOtherInfo->fpga_version_generation_date_ = _strdup(combinedText);

	// 释放内存
	delete[] hexString4;
	delete[] hexString5;
	delete[] combinedText;
	//delete[] hexString1;
	return 0;
}

enum LEDMode
{
	EXTREME_LARGE = 32766,
	LARGE = 32767,
	STANDARD = 32768,  // 或0x8000
	SMALL = 32769,
	EXTREME_SMALL = 32770
};

uint16_t InsPMDL_lightSouce::getTriggerFre(int modeIndex)
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
		return LEDMode::LARGE;//20k                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
	}
}


//自由取像的全局参数
int myLineHigh = 0;//行高
//bool freeRunFlag2 = false;//标志位
double CameraTriggerCycle = 0;//行扫描周期
double mylineFree = 0;///行频
uint32_t TriNum1 = 0;//触发次数
double maxExposure = 0;///行频
double myExposure = 0;///曝光
InsResult InsPMDL_lightSouce::FreeRunForFocus(int _boardnum, int _picture_num, int _index,int CamType)//上位机里的自由取像
{
	bool freeRunFlag2 = false;//标志位
	if (_index == 1 && freeRunFlag2 == false)
	{
		//myLineHigh = get_ins_height();
		//get_scanning_period(CameraTriggerCycle, mylineFree);
		//get_soft_trigger_num(TriNum1);
		//myExposure = get_ins_exposure();
		//===============固定行高和频率================================
		//int FoucusHigh = 800;

		//set_ins_height(FoucusHigh);
		//ConfigureFrameGrabber();
		uint16_t REGMOSACK[2] = {};
		uint16_t REGDDSACK[2] = {};
		uint16_t REGPICNUM[2] = {};

		uint16_t REGSWGAP[2] = {};
		uint16_t REGTRIGGAP[2] = {};
		uint16_t REGSWLOOPNUM[2] = {};
		uint16_t REGCAMTRINUM[2] = {};

		ins_read(FIRSTBOARD, 0x0880, REGMOSACK);//1100
		ins_read(FIRSTBOARD, 0x0882, REGDDSACK);//1104
		ins_read(FIRSTBOARD, 0x1008, REGPICNUM);//2010

		ins_read(FIRSTBOARD, 0x100E, REGSWGAP);//201c
		ins_read(FIRSTBOARD, 0x1006, REGTRIGGAP);//200c
		ins_read(FIRSTBOARD, 0x1010, REGSWLOOPNUM);//2020
		ins_read(FIRSTBOARD, 0x0C0A, REGCAMTRINUM);//1814

		uint32_t MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) << 16) | REGMOSACK[1];
		uint32_t DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) << 16) | REGDDSACK[1];
		uint32_t PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) << 16) | REGPICNUM[1];

		uint32_t SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) << 16) | REGSWGAP[1];
		uint32_t TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) << 16) | REGTRIGGAP[1];
		uint32_t SWLoopNumZ = (static_cast<uint32_t>(REGSWLOOPNUM[0]) << 16) | REGSWLOOPNUM[1];
		uint32_t CamTriNumZ = (static_cast<uint32_t>(REGCAMTRINUM[0]) << 16) | REGCAMTRINUM[1];

		//固定频率 ----
		double CameraTriggerCycle = 240; //30->100
		uint32_t SwLoopNumVal = CameraTriggerCycle / 4;
		uint32_t MosLat = 3;
		uint32_t DdsLat = 3;
		double SwLat = 2;
		uint32_t IdleLat = 1;
		uint32_t TrigLat = 1;
		uint32_t MosTrue = MOSACKZ + MosLat;
		uint32_t DdsTrue = DDSACKZ + DdsLat;

		double Texpend1 = MosTrue + DdsTrue + IdleLat + MosTrue;
		double Texpend2 = DdsTrue + TrigLat;
		double Tencode = (CameraTriggerCycle * 1000) * PicNumZ / 8; //PicNumZ是取相数量
		double TpicLoop = (Tencode - Texpend1) / PicNumZ;
		uint32_t ExpLoopVal = TpicLoop - TriigerGapZ - Texpend2;
		double SwTrue = (ExpLoopVal / SwLoopNumVal) - 1 - SWGAPZ;
		uint32_t SwAckVal = SwTrue - SwLat;
		uint32_t CameraCycleVal = (CameraTriggerCycle * 1000) / 8; //1207

		uint32_t valueToWrite = ExpLoopVal;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		int LightSignInt = _boardnum * 10;
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1004, valueToWrite);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1004, valueToWrite);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1004, buffer);//2008
			ins_write(SECONDBOARD, 0x1004, buffer);
			ins_write(THIRDBOARD, 0x1004, buffer);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1004, valueToWrite);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1004, valueToWrite);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1004, valueToWrite);
		}

		uint32_t valueToWrite2 = SwAckVal;
		uint16_t highValue2 = (valueToWrite2 >> 16) & 0xFFFF;
		uint16_t lowValue2 = valueToWrite2 & 0xFFFF;
		uint16_t buffer2[2] = { highValue2,lowValue2 };

		if (LightSignInt == 10)
		{
			writeToModbusOne(0x0886, valueToWrite2);
		}

		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x0886, buffer2);//110c
			ins_write(SECONDBOARD, 0x0886, buffer2);
			ins_write(THIRDBOARD, 0x0886, buffer2);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0886, valueToWrite2);
		}

		uint32_t valueToWrite3 = CameraCycleVal;
		uint16_t highValue3 = (valueToWrite3 >> 16) & 0xFFFF;
		uint16_t lowValue3 = valueToWrite3 & 0xFFFF;
		uint16_t buffer3[2] = { highValue3,lowValue3 };
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1022, buffer3);//2044
			ins_write(SECONDBOARD, 0x1022, buffer3);
			ins_write(THIRDBOARD, 0x1022, buffer3);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1022, valueToWrite3);
		}



		uint16_t highValue4 = (SwLoopNumVal >> 16) & 0xFFFF;
		uint16_t lowValue4 = SwLoopNumVal & 0xFFFF;
		uint16_t buffer4[2] = { highValue4,lowValue4 };
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1010, buffer4);//2020
			ins_write(SECONDBOARD, 0x1010, buffer4);
			ins_write(THIRDBOARD, 0x1010, buffer4);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1010, SwLoopNumVal);
		}

		if (LightSignInt == 10)
		{
			
			writeToModbusOne(0x0B00, 12625); //1600
			writeToModbusOne(0x101A, 900);//2034
			writeToModbusOne(0x101C, 900);//2038
			writeToModbusOne(0x1024, 1);//2048
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0B00, 12625);
			writeToModbusTwo(0x101A, 800);//5FF->3E8
			writeToModbusTwo(0x101C, 800);
			writeToModbusTwo(0x1024, 1);

		}
		else if(LightSignInt == 30)
 		{
			writeToModbusThree(0x0B00, 12625);
			writeToModbusThree(0x101A, 800);//5FF->3E8
			writeToModbusThree(0x101C, 800);
			writeToModbusThree(0x1024, 1);

		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0B00, 12625);
			writeToModbusFour(0x101A, 800);//5FF->3E8
			writeToModbusFour(0x101C, 800);
			writeToModbusFour(0x1024, 1);

		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B00, 12625);
			writeToModbusFive(0x101A, 700);
			writeToModbusFive(0x101C, 700);
			writeToModbusFive(0x1024, 1);
			 
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0B00, 12625);
			writeToModbusSix(0x101A, 800);//5FF->3E8
			writeToModbusSix(0x101C, 800);
			writeToModbusSix(0x1024, 1);

		}


		freeRunFlag2 = true;
	}
	else if (_index == 0 && freeRunFlag2 == true)
	{
	    int LightSignInt = _boardnum * 10;
		//GrabStop(0);
		//ConfigureExternalPulseFileToCamera();

		uint32_t valueToWrite = 0;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };

		ins_write(FIRSTBOARD, 0x0B04, buffer);

		if (LightSignInt == 10)
		{

			writeToModbusOne(0x0B00, 138); //1600
			writeToModbusOne(0x101A, 2700);//2034
			writeToModbusOne(0x101C, 2700);//2038
			writeToModbusOne(0x1024, 1);//2048
		}

		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0B00, 138);
			writeToModbusTwo(0x101A, 2700);//5FF->3E8
			writeToModbusTwo(0x101C, 2700);
			writeToModbusTwo(0x1024, 1);

		}
		else if (LightSignInt == 30)
		{
			writeToModbusThree(0x0B00, 138);
			writeToModbusThree(0x101A, 2700);//5FF->3E8
			writeToModbusThree(0x101C, 2700);
			writeToModbusThree(0x1024, 1);

		}

		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0B00, 138);
			writeToModbusFour(0x101A, 2700);//5FF->3E8
			writeToModbusFour(0x101C, 2700);
			writeToModbusFour(0x1024, 1);

		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B00, 138);
			writeToModbusFive(0x101A, 2700);
			writeToModbusFive(0x101C, 2700);
			writeToModbusFive(0x1024, 1);

		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0B00, 138);
			writeToModbusSix(0x101A, 2700);//5FF->3E8
			writeToModbusSix(0x101C, 2700);
			writeToModbusSix(0x1024, 1);

		}


		//================before============================================

		//================改回用户的行高和频率和触发次数和曝光================================

		//set_ins_height(myLineHigh);
		//ConfigureFrameGrabber();
		set_scanning_period(_boardnum , _picture_num,CameraTriggerCycle, mylineFree, maxExposure,CamType);

		
		//set_ins_exposure(myExposure);

		//触发次数 ----
		set_soft_trigger_num(TriNum1);

		freeRunFlag2 = false;
	}
	else
	{
		return (InsResult)-1;
		//addToStateMessage("自由取向关闭失败，行扫描周期输入错误）", true);
	}
}

//---------------------------触发设置----------------------
//设置行扫描周期
int InsPMDL_lightSouce::set_scanning_period(int _board_num, int _picture_num,double _line_scanning_period, double& _line_fre, double& _TriExposure,int CamType)
{
	//bool ok;
	//double CameraTriggerCycle = ui->lineEdit_6->text().toDouble(&ok);//1205亮度提升
	double CameraTriggerCycle = _line_scanning_period;
	int PicNumCount = _picture_num;
	InsResult e;
	uint32_t ReadNum = 0;
	uint16_t REGMOSACK[2] = {};
	uint16_t REGDDSACK[2] = {};
	uint16_t REGPICNUM[2] = {};
	uint16_t REGSWGAP[2] = {};
	uint16_t REGTRIGGAP[2] = {};
	uint32_t MOSACKZ;
	uint32_t DDSACKZ;
	uint32_t PicNumZ;
	uint32_t SWGAPZ;
	uint32_t TriigerGapZ;
	readFromModbusOne(0x0C0A, ReadNum);
	double CameraTriggerCycle2 = CameraTriggerCycle / ReadNum;

	

	//0408
	if (CamType == 0)
	{
		CameraTriggerCycle2 = CameraTriggerCycle2;
	}
	else if (CamType == 1)
	{
		CameraTriggerCycle2 = Calculate8kLinePeriod(CameraTriggerCycle2);//8K cxp

	}
	else if (CamType == 2)
	{
		CameraTriggerCycle2 = Calculate16kLinePeriod(CameraTriggerCycle2); //16K cxp
	}

	ins_read(FIRSTBOARD, 0x0880, REGMOSACK);//1100
	ins_read(FIRSTBOARD, 0x0882, REGDDSACK);//1104
	ins_read(FIRSTBOARD, 0x1008, REGPICNUM);//2010
	ins_read(FIRSTBOARD, 0x100E, REGSWGAP);//201c
	ins_read(FIRSTBOARD, 0x1006, REGTRIGGAP);//200c
	MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) << 16) | REGMOSACK[1];
	DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) << 16) | REGDDSACK[1];
	PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) << 16) | REGPICNUM[1];
	SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) << 16) | REGSWGAP[1];
	TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) << 16) | REGTRIGGAP[1];
	if (CameraTriggerCycle >= 36 && CameraTriggerCycle <= 800)//&ok//CameraTriggerCycle >= 36 && CameraTriggerCycle <= 800 
	{
		uint32_t SwLoopNumVal = CameraTriggerCycle2 / 4;
		uint32_t MosLat = 3;
		uint32_t DdsLat = 3;
		double SwLat = 2;
		uint32_t IdleLat = 1;
		uint32_t TrigLat = 1;
		uint32_t MosTrue = MOSACKZ + MosLat;
		uint32_t DdsTrue = DDSACKZ + DdsLat;

		double Texpend1 = MosTrue + DdsTrue + IdleLat + MosTrue;
		double Texpend2 = DdsTrue + TrigLat;
		double Tencode = (CameraTriggerCycle2 * 1000) * PicNumZ / 8; //PicNumZ是取相数量
		double TpicLoop = (Tencode - Texpend1) / PicNumZ;
		uint32_t ExpLoopVal = TpicLoop - TriigerGapZ - Texpend2;
		double SwTrue = (ExpLoopVal / SwLoopNumVal) - SwLoopNumVal - SWGAPZ;
		uint32_t SwAckVal = SwTrue - SwLat;
		uint32_t CameraCycleVal = ((CameraTriggerCycle2 * 1000) / 8)+1; //1207

		uint32_t valueToWrite = ExpLoopVal;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		int LightSignInt = _board_num * 10;
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1004, valueToWrite);
		}

		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1004, valueToWrite);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1004, buffer);//2008
			ins_write(SECONDBOARD, 0x1004, buffer);
			ins_write(THIRDBOARD, 0x1004, buffer);
		}

		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1004, valueToWrite);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1004, valueToWrite);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1004, valueToWrite);
		}

		uint32_t valueToWrite2 = SwAckVal;
		uint16_t highValue2 = (valueToWrite2 >> 16) & 0xFFFF;
		uint16_t lowValue2 = valueToWrite2 & 0xFFFF;
		uint16_t buffer2[2] = { highValue2,lowValue2 };

		if (LightSignInt == 10)
		{
			writeToModbusOne(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x0886, buffer2);//110c
			ins_write(SECONDBOARD, 0x0886, buffer2);
			ins_write(THIRDBOARD, 0x0886, buffer2);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0886, valueToWrite2);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0886, valueToWrite2);
		}
		uint32_t valueToWrite3 = CameraCycleVal;
		uint16_t highValue3 = (valueToWrite3 >> 16) & 0xFFFF;
		uint16_t lowValue3 = valueToWrite3 & 0xFFFF;
		uint16_t buffer3[2] = { highValue3,lowValue3 };
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1022, buffer3);//2044
			ins_write(SECONDBOARD, 0x1022, buffer3);
			ins_write(THIRDBOARD, 0x1022, buffer3);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1022, valueToWrite3);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1022, valueToWrite3);
		}
		uint16_t highValue4 = (SwLoopNumVal >> 16) & 0xFFFF;
		uint16_t lowValue4 = SwLoopNumVal & 0xFFFF;
		uint16_t buffer4[2] = { highValue4,lowValue4 };
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 30)
		{
			ins_write(FIRSTBOARD, 0x1010, buffer4);//2020
			ins_write(SECONDBOARD, 0x1010, buffer4);
			ins_write(THIRDBOARD, 0x1010, buffer4);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1010, SwLoopNumVal);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1010, SwLoopNumVal);
		}

		//e = set_ins_line_period_time(CameraTriggerCycle2);
		//if ((int)e != 0)
		//return 0;
	}
	else
	{
		return -1;//to do
		//addToStateMessage("输入失败，输入范围（4~100）", true);
	}
	double TriExposure = CameraTriggerCycle;

	TriExposure = TriExposure / PicNumCount;

	TriExposure -= 5; //0329 曝光之前-2  //0408 之前是-4   -3.706

	_TriExposure = TriExposure;
	//addToStateMessage("曝光自动设置成功", false);

	//========================计算行频============================
	_line_fre = 1000000.0 / _line_scanning_period / 1000;

	//std::cout << "inline LineFre:" << _line_fre;
	return 0;
}
int InsPMDL_lightSouce::get_scanning_period(double& _line_scanning_period, double& _line_fre)
{
	int ret = 0;
	uint32_t HangSMFre = 0;
	readFromModbusOne(0x1022, HangSMFre);
	uint16_t REGCAMTRINUM[2] = {};
	ret = (int)ins_read(FIRSTBOARD, 0x0C0A, REGCAMTRINUM);//1814
	if (ret == 0)
	{
		uint32_t CamTriNumZ = (static_cast<uint32_t>(REGCAMTRINUM[0]) << 16) | REGCAMTRINUM[1];
		_line_scanning_period = HangSMFre * CamTriNumZ;
		_line_scanning_period = _line_scanning_period * 8 / 1000;//系统扫描周期
	}
	_line_fre = static_cast<double>(_line_scanning_period);
	_line_fre = 1000000.0 / _line_fre / 1000;



	return 0;
}

//设置触发模式，触发使能开，在相机开始取图后调用使能

inline int InsPMDL_lightSouce::set_trigger_mode_on(int _index)
{




	uint32_t valueToWrite = 1;
	uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
	uint16_t lowValue = valueToWrite & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };

	if (_index == 1)// "Encode模式"
	{
		ins_write(FIRSTBOARD, 0x0B2E, buffer);//165CW
	}
	else if (_index == 2)// "软触发模式"internal
	{
		ins_write(FIRSTBOARD, 0x0B04, buffer);//1608W
	}
	else if (_index == 3)// "IO触发模式"internal
	{
		ins_write(FIRSTBOARD, 0x0B14, buffer);//1628W
	}
	return 0;
}
int InsPMDL_lightSouce::set_trigger_mode_off()
{
	uint32_t valueToWrite = 0;
	uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
	uint16_t lowValue = valueToWrite & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };

    ins_write(FIRSTBOARD, 0x0B2E, buffer);//165CW
    ins_write(FIRSTBOARD, 0x0B04, buffer);//1608W
    ins_write(FIRSTBOARD, 0x0B14, buffer);//1608W

	return 0;
}
//----------编码器触发设置------------
//编码器校验，A-B相取反1开、0关
int InsPMDL_lightSouce::set_a_b_phase_inversion_on_off(int _index)
{
	if (_index == 1)
	{
		uint32_t AB1 = 1;//A&B
		uint16_t AB1H = (AB1 >> 16) & 0xFFFF;
		uint16_t AB1L = AB1 & 0xFFFF;
		uint16_t buffer[2] = { AB1H,AB1L };
		//ins_write(0x162C,buffer);//West
		ins_write(FIRSTBOARD, 0x0B16, buffer);
	}
	if (_index == 0)
	{
		uint32_t AB2 = 0;//A&B
		uint16_t AB2H = (AB2 >> 16) & 0xFFFF;
		uint16_t AB2L = AB2 & 0xFFFF;
		uint16_t buffer[2] = { AB2H,AB2L };
		//ins_write(0x162C,buffer);//West
		ins_write(FIRSTBOARD, 0x0B16, buffer);
	}
	return 0;
}


int InsPMDL_lightSouce::set_trigger_direction(int _index)
{
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

	uint16_t buffer[2] = { highValue,lowValue };
	uint16_t buffer1[2] = { highValue1,lowValue1 };
	uint16_t buffer2[2] = { highValue2,lowValue2 };
	uint16_t buffer3[2] = { highValue3,lowValue3 };
	int ret = 0;
	ret = (int)ins_write(FIRSTBOARD, 0x0B2E, buffer);
	if (ret != 0)
		return ret;
	if (_index == 0)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B1A, buffer);
	}
	else if (_index == 1)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B1A, buffer1);
	}

	else if (_index == 2)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B1A, buffer2);
	}

	else if (_index == 3)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B1A, buffer3);
	}
	return ret;
}
int InsPMDL_lightSouce::set_encoder_trigger_mode(int _index)
{
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


	uint16_t buffer[2] = { highValue,lowValue };
	uint16_t buffer1[2] = { highValue1,lowValue1 };
	uint16_t buffer2[2] = { highValue2,lowValue2 };
	uint16_t buffer3[2] = { highValue3,lowValue3 };
	int ret = 0;

	if (_index == 0)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B18, buffer);
	}
	else if (_index == 1)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B18, buffer1);
	}

	else if (_index == 2)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B18, buffer2);
	}

	else if (_index == 3)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x0B18, buffer3);
	}
	return ret;
}
int InsPMDL_lightSouce::set_frequency_division(uint32_t _frequency_division)
{
	uint32_t valueToWrite = _frequency_division;
	uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
	uint16_t lowValue = valueToWrite & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B1E, buffer);
	//addToStateMessage("分频—输入成功", false);
	return 0;
}
int InsPMDL_lightSouce::set_frequency_multiplier(uint32_t _frequency_multiplier)
{
	uint32_t arg1 = _frequency_multiplier;
	uint32_t valueToWrite = arg1 - 1;

	if (arg1 >= 1)
	{
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint32_t valueToWrite1 = 1;
		uint16_t highValue1 = (valueToWrite1 >> 16) & 0xFFFF;
		uint16_t lowValue1 = valueToWrite1 & 0xFFFF;


		uint16_t buffer[2] = { highValue,lowValue };
		uint16_t buffer1[2] = { highValue1,lowValue1 };

		ins_write(FIRSTBOARD, 0x0B26, buffer);
		ins_write(FIRSTBOARD, 0x0B24, buffer1);
		//qDebug() << "write BeiPin to 0x1648";

		//addToStateMessage("倍频—输入成功", false);
		return 0;
	}
	else
	{
		//addToStateMessage("输入失败，须大于等于1", true);
		return 0x00001030;
	}
}
//获取触发方向,0-单向行程，1-回退跟踪，2-回退忽略，3-双向扫描
int InsPMDL_lightSouce::get_trigger_direction(uint32_t& _index)
{
	return readFromModbusOne(0x0B1A, _index);//1634

}
//获取编码器触发模式，0-A相上升沿，单向计数，1-A相上升沿，双向计数，1-A相双边沿，双向计数，A&B相双边沿，双向
int InsPMDL_lightSouce::get_encoder_trigger_mode(uint32_t& _index)
{
	return readFromModbusOne(0x0B18, _index);
}

int InsPMDL_lightSouce::get_frequency_division(uint32_t& _index)
{

	return readFromModbusOne(0x0B1E, _index);
	

}
int InsPMDL_lightSouce::get_frequency_multiplier(uint32_t& _index)
{
	int ret = readFromModbusOne(0x0B26, _index);
	if (ret != 0)
	{
		return ret;
	}
	if (_index == 0)
	{
		_index = 0;
	}
	return 0;
}
//----------IO触发设置------------
int InsPMDL_lightSouce::set_io_trigger_mode(int _board_num, int _index)
{
	//1614 0-7
	int index = _index;
	if (index == 0)
	{
		writeToModbusOne(0x0B0A, Zeroz);
		//qDebug() << "write 0 to 0B0A" << endl;
	}
	else if (index == 1)
	{
		writeToModbusOne(0x0B0A, Onez);
		//qDebug() << "write 1 to 0B0A" << endl;
	}
	else if (index == 2)
	{
		writeToModbusOne(0x0B0A, Twoz);
		//qDebug() << "write 2 to 0B0A" << endl;
	}
	else if (index == 3)
	{
		writeToModbusOne(0x0B0A, Threez);
		//qDebug() << "write 3 to 0B0A" << endl;
	}
	else if (index == 4)
	{
		writeToModbusOne(0x0B0A, Fourz);
		//qDebug() << "write 4 to 0B0A" << endl;
	}
	else if (index == 5)
	{
		writeToModbusOne(0x0B0A, Fivez);
		//qDebug() << "write 5 to 0B0A" << endl;
	}
	else if (index == 6)
	{
		writeToModbusOne(0x0B0A, Sixz);
		//qDebug() << "write 6 to 0B0A" << endl;
	}
	else if (index == 7)
	{
		writeToModbusOne(0x0B0A, Sevenz);
		//qDebug() << "write 7 to 0B0A" << endl;
	}
	return 0;
}
int InsPMDL_lightSouce::set_io_trigger_num(int _board_num, uint32_t _trinum)
{
	writeToModbusOne(0x0B08, _trinum);
	return 0;
}
int InsPMDL_lightSouce::set_io_pulse_count(int _board_num, uint32_t _count)
{
	//1620
	writeToModbusOne(0x0B10, _count);
	return 0;
}
int InsPMDL_lightSouce::set_io_trigger_pulse_width(int _board_num, uint32_t _width)
{
	//1618  输入*1000/8
	uint32_t TriWidth = _width;
	uint32_t TriWidthToReg = TriWidth * 1000 / 8;
	writeToModbusOne(0x0B0C, TriWidthToReg);
	return 0;
}
int InsPMDL_lightSouce::set_io_trigger_delay(int _board_num, uint32_t _delay)
{
	//161C
	uint32_t TriDelay = _delay;
	uint32_t TriDelayToReg = TriDelay * 1000 / 8;
	writeToModbusOne(0x0B0E, TriDelayToReg);
	return 0;
}
int InsPMDL_lightSouce::set_io_trigger_period(int _board_num, uint32_t _period)
{
	//200C  输入*1000/8
	uint32_t TriPeriod = _period;
	uint32_t TriPeriodToReg = TriPeriod * 1000 / 8;

	if (_board_num == 1)
	{
		writeToModbusOne(0xB06, TriPeriodToReg);
	}
	else if (_board_num == 2)
	{
		writeToModbusTwo(0xB06, TriPeriodToReg);
	}
	else if (_board_num == 3)
	{
		writeToModbusThree(0xB06, TriPeriodToReg);
	}
	else if (_board_num == 4)
	{
		writeToModbusFour(0xB06, TriPeriodToReg);
	}
	else if (_board_num == 5)
	{
		writeToModbusFive(0xB06, TriPeriodToReg);
	}
	else if (_board_num == 6)
	{
		writeToModbusSix(0xB06, TriPeriodToReg);
	}
	
	//writeToModbusThree(0x1006, TriPeriodToReg);
	return 0;
}
int InsPMDL_lightSouce::set_io_trigger_input_polarity(int _board_num, uint32_t _index)
{
	//1624 0保持 1反转
	if (_index == 0)
	{
		if (_board_num == 1)
		{
			writeToModbusOne(0x0B12, Zeroz);
		}
		else if(_board_num == 2)
		{
			writeToModbusTwo(0x0B12, Zeroz);
		}
		else if (_board_num == 3)
		{
			writeToModbusThree(0x0B12, Zeroz);
		}
		else if (_board_num == 4)
		{
			writeToModbusFour(0x0B12, Zeroz);
		}
		else if (_board_num == 5)
		{
			writeToModbusFive(0x0B12, Zeroz);
		}
		else if (_board_num == 6)
		{
			writeToModbusSix(0x0B12, Zeroz);
		}

	}
	else if (_index == 1)
	{
	
		if (_board_num == 1)
		{
			writeToModbusOne(0x0B12, Onez);
		}
		else if (_board_num == 2)
		{
			writeToModbusTwo(0x0B12, Onez);
		}
		else if (_board_num == 3)
		{
			writeToModbusThree(0x0B12, Onez);
		}
		else if (_board_num == 4)
		{
			writeToModbusFour(0x0B12, Onez);
		}
		else if (_board_num == 5)
		{
			writeToModbusFive(0x0B12, Onez);
		}
		else if (_board_num == 6)
		{
			writeToModbusSix(0x0B12, Onez);
		}
	}
	return 0;
}
int InsPMDL_lightSouce::get_io_trigger_mode(uint32_t& _index)
{
	return readFromModbusOne(0x0B0A, _index);
}
int InsPMDL_lightSouce::get_io_trigger_num(uint32_t& _trinum)
{
	return readFromModbusOne(0x0B08, _trinum);
}
int InsPMDL_lightSouce::get_io_pulse_count(uint32_t& _count)
{
	return readFromModbusOne(0x0B10, _count);
}

int InsPMDL_lightSouce::get_io_trigger_pulse_width(uint32_t& _width)
{
	int ret = readFromModbusOne(0x0B0C, _width);
	_width = _width * 8 / 1000;
	return ret;
}
int InsPMDL_lightSouce::get_io_trigger_delay(uint32_t& _delay)
{
	//161C
	int ret = readFromModbusOne(0x0B0E, _delay);
	_delay = _delay * 8 / 1000;
	return ret;
}
int InsPMDL_lightSouce::get_io_trigger_period(uint32_t& _period)
{
	//161C
	int ret = readFromModbusOne(0x1006, _period);
	_period = _period * 8 / 1000;
	return ret;
}
int InsPMDL_lightSouce::get_io_trigger_input_polarity(uint32_t& _index)
{
	//161C
	int ret = readFromModbusOne(0x0B12, _index);
	return ret;
}

//---------------------------光源条纹设置----------------------
int InsPMDL_lightSouce::set_emission_mode(int _index, int _board_len)
{
	_board_len *= 10;
	int ret = 0;
	uint32_t DDSPhaseOffSet = 144;
	uint16_t DDShighValue = (DDSPhaseOffSet >> 16) & 0xFFFF;
	uint16_t DDSlowValue = DDSPhaseOffSet & 0xFFFF;
	uint16_t DDSbuffer[2] = { DDShighValue,DDSlowValue };//144

	uint32_t DDSPhaseOffSet0 = 0;
	uint16_t DDShighValue0 = (DDSPhaseOffSet0 >> 16) & 0xFFFF;
	uint16_t DDSlowValue0 = DDSPhaseOffSet0 & 0xFFFF;
	uint16_t DDSbuffer0[2] = { DDShighValue0,DDSlowValue0 };

	uint32_t DDSPhaseOffSet72 = 72;
	uint16_t DDShighValue72 = (DDSPhaseOffSet72 >> 16) & 0xFFFF;
	uint16_t DDSlowValue72 = DDSPhaseOffSet72 & 0xFFFF;
	uint16_t DDSbuffer72[2] = { DDShighValue72,DDSlowValue72 };

	uint32_t DDSPhaseOffSet216 = 216;
	uint16_t DDShighValue216 = (DDSPhaseOffSet216 >> 16) & 0xFFFF;
	uint16_t DDSlowValue216 = DDSPhaseOffSet216 & 0xFFFF;
	uint16_t DDSbuffer216[2] = { DDShighValue216,DDSlowValue216 };

	uint32_t modeValues[] = { 3, 6, 12, 18, 24 }; //发光模式值

	//获取当前index对应的uint32_t值
	uint32_t currentValue = modeValues[_index];

	//高低位
	uint16_t highValue = (currentValue >> 16) & 0xFFFF;
	uint16_t lowValue = currentValue & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };

	if (_index == 0)//极大        //0516Start //ff
	{
		if (_board_len == 10)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		}
		else if (_board_len == 20)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer72);

		}
		else if (_board_len == 30)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer72);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer);
		}
		else if (_board_len == 40)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer72);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer216);

		}
		else if (_board_len == 50)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer72);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer216);

		}
		else if (_board_len == 60)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer72);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer216);
			ret = (int)ins_write(SIXTHBOARD, 0x1012, DDSbuffer72);

		}
	}
	else if (_index == 1)//大
	{
		if (_board_len == 10)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		}

		else if (_board_len == 20)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);

		}
		else if (_board_len == 30)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		}

		else if (_board_len == 40)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);

		}
		else if (_board_len == 50)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);

		}

		else if (_board_len == 60)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1012, DDSbuffer);

		}
	}
	else if (_index == 2)//标准模式
	{
		if (_board_len == 10)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		}
		else if (_board_len == 30)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		}
		else if (_board_len == 20)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
		}
		else if (_board_len == 40)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);

		}
		else if (_board_len == 50)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);

		}
		else if (_board_len == 60)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(SIXTHBOARD, 0x1012, DDSbuffer0);

		}
	}
	else if (_index == 3)//小模式
	{
		if (_board_len == 10)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		}
		else if (_board_len == 20)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
	
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			
		}
		else if (_board_len == 30)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		}
		else if (_board_len == 40)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);


			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);

		}
		else if (_board_len == 50)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);

		}
		else if (_board_len == 60)
		{
			ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
			ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
			ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
			ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1002, buffer);

			//反转
			ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
			ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer);
			ret = (int)ins_write(SIXTHBOARD, 0x1012, DDSbuffer);

		}
	}
	else if (_index == 4) //极小
	{
	if (_board_len == 10)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
	}

	else if (_board_len == 20)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);

		//反转
		ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);

	}
	else if (_board_len == 30)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
		ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
		//反转
		ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
	}

	else if (_board_len == 40)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
		ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
		ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);


		//反转
		ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);

	}
	else if (_board_len == 50)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
		ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
		ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
		ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);

		//反转
		ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);

	}
	else if (_board_len == 60)
	{
		ret = (int)ins_write(FIRSTBOARD, 0x1002, buffer);
		ret = (int)ins_write(SECONDBOARD, 0x1002, buffer);
		ret = (int)ins_write(THIRDBOARD, 0x1002, buffer);
		ret = (int)ins_write(FOURTHBOARD, 0x1002, buffer);
		ret = (int)ins_write(FIFTHBOARD, 0x1002, buffer);
		ret = (int)ins_write(SIXTHBOARD, 0x1002, buffer);

		//反转
		ret = (int)ins_write(SECONDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(THIRDBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(FIFTHBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(FOURTHBOARD, 0x1012, DDSbuffer0);
		ret = (int)ins_write(SIXTHBOARD, 0x1012, DDSbuffer0);

	}
	}
	return 0;
}
//0-极大，1-大，2-标准，3-小，4-极小
int InsPMDL_lightSouce::get_emission_mode(int& _index)
{
	uint32_t IniLightMode = 0;
	readFromModbusOne(0x1002, IniLightMode);

	if (IniLightMode == 3)
	{
		_index = 0;
		//addToStateMessage("当前发光模式为 极大", false);
	}

	else if (IniLightMode == 6)
	{
		_index = 1;
		//addToStateMessage("当前发光模式为 大", false);
	}
	else if (IniLightMode == 12)
	{
		_index = 2;
		//addToStateMessage("当前发光模式为 标准", false);
	}
	else if (IniLightMode == 18)
	{
		_index = 3;
		//addToStateMessage("当前发光模式为 小", false);
	}
	else if (IniLightMode == 24)
	{
		_index = 4;
		//addToStateMessage("当前发光模式为 极小", false);
	}
	else
	{
		_index = 0;
		return -1;//to do
		//addToStateMessage("发光模式初始化失败 请重新选择", true);
	}
	return 0;
}
//设置X条纹移动方向,0-正向，1-反向
int InsPMDL_lightSouce::set_x_stripe_movement_direction(int _boardnum, int _index)
{
	if (_index == 0)
	{
		uint32_t valueToWrite = 0;//正向
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;

		uint16_t buffer[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1014, buffer);//combine

		}
		else if (_boardnum == 2)
		{
			writeToModbusTwo(0x1014, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{
			ins_write(FIRSTBOARD, 0x1014, buffer);//2028
			ins_write(SECONDBOARD, 0x1014, buffer);
			ins_write(THIRDBOARD, 0x1014, buffer);
		}
		else if (_boardnum == 4)
		{
			writeToModbusFour(0x1014, valueToWrite);
		}
		else if (_boardnum == 5)
		{
			writeToModbusFive(0x1014, valueToWrite);
		}
		else if (_boardnum == 6)
		{
			writeToModbusSix(0x1014, valueToWrite);
		}
	}
	else if (_index == 1)
	{
		uint32_t valueToWrite = 1;//反向
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer2[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1014, buffer2);//combine
		}
		else if (_boardnum == 2)
		{
			writeToModbusTwo(0x1014, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{
			ins_write(FIRSTBOARD, 0x1014, buffer2);
			ins_write(SECONDBOARD, 0x1014, buffer2);
			ins_write(THIRDBOARD, 0x1014, buffer2);
		}
		else if (_boardnum == 4)
		{
			writeToModbusFour(0x1014, valueToWrite);
		}
		else if (_boardnum == 5)
		{
			writeToModbusFive(0x1014, valueToWrite);
		}
		else if (_boardnum == 6)
		{
			writeToModbusSix(0x1014, valueToWrite);
		}
	}
	return 0;
}
//设置Y条纹移动方向,0-正向，1-反向
int InsPMDL_lightSouce::set_y_stripe_movement_direction(int _boardnum, int _index)
{
	if (_index == 0)
	{
		uint32_t valueToWrite = 0;//正向
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1016, buffer);//combine

		}
		else if (_boardnum == 2)
		{
			writeToModbusTwo(0x1016, valueToWrite);
		}

		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1016, buffer);//202C
			ins_write(SECONDBOARD, 0x1016, buffer);
			ins_write(THIRDBOARD, 0x1016, buffer);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1016, valueToWrite);
		}

		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1016, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1016, valueToWrite);
		}

	}
	else if (_index == 1)
	{
		uint32_t valueToWrite = 1;//反向
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer2[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1016, buffer2);//combine

		}
		else if (_boardnum == 2)
		{
			writeToModbusTwo(0x1016, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1016, buffer2);
			ins_write(SECONDBOARD, 0x1016, buffer2);
			ins_write(THIRDBOARD, 0x1016, buffer2);
		}
		else if (_boardnum == 4)
		{
			writeToModbusFour(0x1016, valueToWrite);
		}
		else if (_boardnum == 5)
		{
			writeToModbusFive(0x1016, valueToWrite);
		}
		else if (_boardnum == 6)
		{
			writeToModbusSix(0x1016, valueToWrite);
		}
	}
	return 0;
}
//设置X方向起始相位
int InsPMDL_lightSouce::set_initial_phase_in_x_direction(int _boardnum, int _index)
{
	if (_index == 0)
	{
		uint32_t valueToWrite = 0;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1000, buffer);//combine

		}

		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1000, valueToWrite);
		}

		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1000, buffer);//2000
			ins_write(SECONDBOARD, 0x1000, buffer);
			ins_write(THIRDBOARD, 0x1000, buffer);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1000, valueToWrite);
		}

		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1000, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1000, valueToWrite);
		}

	}
	else if (_index == 1)//90
	{
		uint32_t valueToWrite = 72;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer2[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1000, buffer2);//combine

		}

		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1000, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1000, buffer2);//2000
			ins_write(SECONDBOARD, 0x1000, buffer2);
			ins_write(THIRDBOARD, 0x1000, buffer2);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1000, valueToWrite);
		}

		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1000, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1000, valueToWrite);
		}
	}
	else if (_index == 2)//180
	{
		uint32_t valueToWrite = 144;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer3[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1000, buffer3);//combine

		}

		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1000, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1000, buffer3);//2000
			ins_write(SECONDBOARD, 0x1000, buffer3);
			ins_write(THIRDBOARD, 0x1000, buffer3);
		}

		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1000, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1000, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1000, valueToWrite);
		}
	}
	else if (_index == 3)//270
	{
		uint32_t valueToWrite = 216;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer4[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1000, buffer4);//combine

		}
		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1000, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{

			ins_write(FIRSTBOARD, 0x1000, buffer4);//2000
			ins_write(SECONDBOARD, 0x1000, buffer4);
			ins_write(THIRDBOARD, 0x1000, buffer4);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1000, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1000, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1000, valueToWrite);
		}
	}
	return 0;
}
//设置Y方向起始相位
int InsPMDL_lightSouce::set_initial_phase_in_y_direction(int _boardnum, int _index)
{
	if (_index == 0)
	{
		uint32_t valueToWrite = 0;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;

		uint16_t buffer[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer);//combine
		}
		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1038, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{
			ins_write(FIRSTBOARD, 0x1038, buffer);//2070
			ins_write(SECONDBOARD, 0x1038, buffer);
			ins_write(THIRDBOARD, 0x1038, buffer);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1038, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1038, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1038, valueToWrite);
		}
	}
	else if (_index == 1)//90
	{
		uint32_t valueToWrite = 72;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;

		uint16_t buffer2[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer2);//combine
		}
		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1038, valueToWrite);
		}
		else if (_boardnum == 3)//3
		{
			ins_write(FIRSTBOARD, 0x1038, buffer2);//2070
			ins_write(SECONDBOARD, 0x1038, buffer2);
			ins_write(THIRDBOARD, 0x1038, buffer2);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1038, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1038, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1038, valueToWrite);
		}
	}
	else if (_index == 2)//180
	{
		uint32_t valueToWrite = 144;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;

		uint16_t buffer3[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer3);//combine
		}
		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1038, valueToWrite);
		}
		else if (_boardnum == 3)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer3);//2070
			ins_write(SECONDBOARD, 0x1038, buffer3);
			ins_write(THIRDBOARD, 0x1038, buffer3);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1038, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1038, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1038, valueToWrite);
		}
	}
	else if (_index == 3)//270
	{
		uint32_t valueToWrite = 216;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer4[2] = { highValue,lowValue };
		if (_boardnum == 1)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer4);//combine
		}

		else if (_boardnum == 2)//3
		{
			writeToModbusTwo(0x1038, valueToWrite);
		}
		else if (_boardnum == 3)
		{
			ins_write(FIRSTBOARD, 0x1038, buffer4);//2070
			ins_write(SECONDBOARD, 0x1038, buffer4);
			ins_write(THIRDBOARD, 0x1038, buffer4);
		}
		else if (_boardnum == 4)//3
		{
			writeToModbusFour(0x1038, valueToWrite);
		}
		else if (_boardnum == 5)//3
		{
			writeToModbusFive(0x1038, valueToWrite);
		}
		else if (_boardnum == 6)//3
		{
			writeToModbusSix(0x1038, valueToWrite);
		}
	}
	return 0;
}
//获取X条纹移动方向,0-正向，1-反向
int InsPMDL_lightSouce::get_x_stripe_movement_direction(uint32_t& _index)
{
	int ret = readFromModbusOne(0x1014, _index);
	return ret;
}
int InsPMDL_lightSouce::get_y_stripe_movement_direction(uint32_t& _index)
{
	int ret = readFromModbusOne(0x1016, _index);
	return ret;
}
int InsPMDL_lightSouce::get_initial_phase_in_x_direction(uint32_t& _initial_phase)
{
	int ret = readFromModbusOne(0x1000, _initial_phase);
	uint32_t AngelX = _initial_phase;
	if (AngelX == 0)//0
	{
		_initial_phase = 0;
	}
	else if (AngelX == 72)//90
	{
		_initial_phase = 90;
	}
	else if (AngelX == 144)//180
	{
		_initial_phase = 180;
	}
	else if (AngelX == 216)//270
	{
		_initial_phase = 270;
	}
	return ret;
}
int InsPMDL_lightSouce::get_initial_phase_in_y_direction(uint32_t& _initial_phase)
{
	int ret = readFromModbusOne(0x1038, _initial_phase);//角度
	uint32_t AngelY = _initial_phase;
	if (AngelY == 0)//0
	{
		_initial_phase = 0;
	}

	else if (AngelY == 72)//90
	{
		_initial_phase = 90;
	}
	else if (AngelY == 144)//180
	{
		_initial_phase = 180;
	}
	else if (AngelY == 216)//270
	{
		_initial_phase = 270;
	}
	return ret;
}

//--------------二级触发光源触发模式--------------
int InsPMDL_lightSouce::secondary_trigger_enable_and_disable(int _boardnum, double _index)
{
	if (_boardnum == 1) {
		writeToModbusOne(0x0B7E, _index);
	}
	if (_boardnum == 3) {
		writeToModbusThree(0x0B7E, _index);
	}
	if (_boardnum == 5) {
		writeToModbusFive(0x0B7E, _index);
	}
	return 0;
}
int InsPMDL_lightSouce::set_encoder_secondary_trigger_pulse_width(int _boardnum, double _encodewidth)
{
	double EncodeWidth = _encodewidth;//1640
	uint32_t EncodeWidth2 = EncodeWidth * 1000 / 8;
	uint16_t highValue = (EncodeWidth2 >> 16) & 0xFFFF;
	uint16_t lowValue = EncodeWidth2 & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	int LightSignInt = _boardnum * 10;
	if (LightSignInt == 10)
	{
		ins_write(FIRSTBOARD, 0x0B90, buffer);//combine
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 20)//3
	{
		writeToModbusTwo(0x0B90, EncodeWidth2);
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 30)
	{
		ins_write(FIRSTBOARD, 0x0B90, buffer);//1640 ->1720
		ins_write(SECONDBOARD, 0x0B90, buffer);
		ins_write(THIRDBOARD, 0x0B90, buffer);
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 40)//3
	{
		writeToModbusFour(0x0B90, EncodeWidth2);
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 50)//3
	{
		writeToModbusFive(0x0B90, EncodeWidth2);
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 60)//3
	{
		writeToModbusSix(0x0B90, EncodeWidth2);
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	return 0;
}
int InsPMDL_lightSouce::set_encoder_secondary_trigger_travel_direction(int _boardnum, int _index)
{
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
	uint16_t buffer[2] = { highValue,lowValue };
	uint16_t buffer1[2] = { highValue1,lowValue1 };
	uint16_t buffer2[2] = { highValue2,lowValue2 };
	uint16_t buffer3[2] = { highValue3,lowValue3 };

	ins_write(FIRSTBOARD, 0x0B6E, buffer);//->16DC
	if (_index == 0)
	{
		ins_write(FIRSTBOARD, 0x0B5A, buffer);//1634->16B4
	}
	else if (_index == 1)
	{
		ins_write(FIRSTBOARD, 0x0B5A, buffer1);
	}

	else if (_index == 2)
	{
		ins_write(FIRSTBOARD, 0x0B5A, buffer2);
	}

	else if (_index == 3)
	{
		ins_write(FIRSTBOARD, 0x0B5A, buffer3);
	}
	return 0;
}

int InsPMDL_lightSouce::set_secondary_frequency_division(uint32_t _frequency_division)
{
	uint32_t valueToWrite = _frequency_division;
	uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
	uint16_t lowValue = valueToWrite & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B5E, buffer);
	//addToStateMessage("分频—输入成功", false);
	return 0;
}
int InsPMDL_lightSouce::set_secondary_frequency_multiplier(uint32_t _frequency_multiplier)
{
	uint32_t valueToWrite = _frequency_multiplier -1;
	if (_frequency_multiplier>= 1)
	{
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint32_t valueToWrite1 = 1;
		uint16_t highValue1 = (valueToWrite1 >> 16) & 0xFFFF;
		uint16_t lowValue1 = valueToWrite1 & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		uint16_t buffer1[2] = { highValue1,lowValue1 };
		ins_write(FIRSTBOARD, 0x0B66, buffer);
		ins_write(FIRSTBOARD, 0x0B64, buffer1);

	}
	else
	{
		return 0X00001000;
	}
	
	//addToStateMessage("倍频—输入成功", false);
	return 0;
}

//-------软触发-----
int InsPMDL_lightSouce::set_software_trigger_trigger_count(uint32_t _count)
{
	uint32_t TriNum1 = _count;
	uint16_t highValue = (TriNum1 >> 16) & 0xFFFF;
	uint16_t lowValue = TriNum1 & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B42, buffer);
	return 0;
}
int InsPMDL_lightSouce::set_software_trigger_trigger_period(double _period)
{
	double Num2 = _period;
	uint32_t Num3 = static_cast<uint32_t>(Num2 * 1000);
	uint16_t highValue = (Num3 >> 16) & 0xFFFF;
	uint16_t lowValue = Num3 & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B40, buffer);
	return 0;
}
int InsPMDL_lightSouce::get_software_trigger_trigger_count(uint32_t& _count)
{
	readFromModbusOne(0x0B42, _count);
	return 0;
}
int InsPMDL_lightSouce::get_software_trigger_trigger_period(double& _period)
{
	uint32_t utemp = 0;
	readFromModbusOne(0x0B40, utemp);
	_period = static_cast<double>(utemp) / 1000;
	return 0;
}
//-------IO触发-----
int InsPMDL_lightSouce::set_io_secondary_trigger_mode(int _board_num, int _index)
{
	int index = _index;
	if (index == 0)
	{
		writeToModbusOne(0x0B4A, Zeroz);
	}
	else if (index == 1)
	{
		writeToModbusOne(0x0B4A, Onez);
	}
	else if (index == 2)
	{
		writeToModbusOne(0x0B4A, Twoz);
	}
	else if (index == 3)
	{
		writeToModbusOne(0x0B4A, Threez);
	}
	else if (index == 4)
	{
		writeToModbusOne(0x0B4A, Fourz);
	}
	else if (index == 5)
	{
		writeToModbusOne(0x0B4A, Fivez);
	}
	else if (index == 6)
	{
		writeToModbusOne(0x0B4A, Sixz);
	}
	else if (index == 7)
	{
		writeToModbusOne(0x0B4A, Sevenz);
	}
	return 0;
}
//设置io触发数量
int InsPMDL_lightSouce::set_io_secondary_trigger_num(int _board_num, uint32_t _trinum)
{
	writeToModbusOne(0x0B48, _trinum);
	return 0;
}
//设置io脉冲数量
int InsPMDL_lightSouce::set_io_secondary_pulse_count(int _board_num, uint32_t _count)
{
	writeToModbusOne(0x0B50, _count);
	return 0;
}
//设置io触发脉冲宽度
int InsPMDL_lightSouce::set_io_secondary_trigger_pulse_width(int _board_num, uint32_t _width)
{
	uint32_t TriWidth = _width;
	uint32_t TriWidthToReg = TriWidth * 1000 / 8;
	writeToModbusOne(0x0B4C, TriWidthToReg);
	return 0;
}
//设置io触发延迟
int InsPMDL_lightSouce::set_io_secondary_trigger_delay(int _board_num, uint32_t _delay)
{
	uint32_t TriDelay = _delay;
	uint32_t TriDelayToReg = TriDelay * 1000 / 8;
	writeToModbusOne(0x0B4E, TriDelayToReg);
	return 0;
}
//设置io触发周期
int InsPMDL_lightSouce::set_io_secondary_trigger_period(int _board_num, uint32_t _period)
{
	uint32_t TriPeriod = _period;
	uint32_t TriPeriodToReg = TriPeriod * 1000 / 8;
	writeToModbusOne(0x0B46, TriPeriodToReg);
	return 0;
}
//设置io触发输入极性,0-保持，1-反转
int InsPMDL_lightSouce::set_io_secondary_trigger_input_polarity(int _board_num, uint32_t _index)
{
	if (_index == 0)
	{
		writeToModbusOne(0x0B52, Zeroz);//56
		//qDebug() << "保持" << endl;
	}
	else if (_index == 1)
	{
		writeToModbusOne(0x0B52, Onez);
		//qDebug() << "反转" << endl;
	}
	return 0;
}

/*获取io触发模式,
0-模式1：IO“0”开始，IO“1”结束,
1-模式2：IO“1”开始，IO“0”结束,
2-模式3：IO“0”开始，输出固定触发脉冲,
3-模式4：IO“1”开始，输出固定触发脉冲,
4-模式5：IO“0”有效，上升沿有效，下降沿结束,
5-模式6：IO“1”有效，上升沿有效，下降沿结束,
6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
7-模式8：多个IO"1"作为开始，输出固定触发脉冲
*/
int InsPMDL_lightSouce::get_io_secondary_trigger_mode(uint32_t& _index)
{
	return readFromModbusOne(0x0B4A, _index);
}
//获取io触发数量
int InsPMDL_lightSouce::get_io_secondary_trigger_num(uint32_t& _trinum)
{
	return readFromModbusOne(0x0B48, _trinum);
}
//获取io脉冲数量
int InsPMDL_lightSouce::get_io_secondary_pulse_count(uint32_t& _count)
{
	return readFromModbusOne(0x0B50, _count);
}
//获取io触发脉冲宽度
int InsPMDL_lightSouce::get_io_secondary_trigger_pulse_width(uint32_t& _width)
{
	int ret = readFromModbusOne(0x0B4C, _width);
	_width = _width * 8 / 1000;
	return ret;
}
//获取io触发延迟
int InsPMDL_lightSouce::get_io_secondary_trigger_delay(uint32_t& _delay)
{
	int ret = readFromModbusOne(0x0B4E, _delay);
	_delay = _delay * 8 / 1000;
	return ret;
}
//获取io触发周期
int InsPMDL_lightSouce::get_io_secondary_trigger_period(uint32_t& _period)
{
	int ret = readFromModbusOne(0x1046, _period);
	_period = _period * 8 / 1000;
	return ret;
}
//获取io触发输入极性，0-保持，1-反转
int InsPMDL_lightSouce::get_io_secondary_trigger_input_polarity(uint32_t& _index)
{
	return  readFromModbusOne(0x0B52, _index);
}

//--------------------红外模式-------------------------
int InsPMDL_lightSouce::extended_trigger_enable_and_disable(int _boardnum, double _index, int _quantity)
{
	int LightSignInt = _boardnum * 10;
	if (_index == 1)
	{
		//PicNumCount = 9;//122089
		uint32_t NumNine = 8 + _quantity;//ui->ExNumUI->text().toInt();
		int PicNumCount = NumNine;
		uint32_t valueToWrite = 1;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;


		uint16_t buffer2[2] = { highValue,lowValue };
		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0C08, buffer2);//combine
			writeToModbusOne(0xC0A, NumNine);
		}
		else if (LightSignInt == 20)//3
		{
		
			writeToModbusTwo(0x0C08, valueToWrite);
			writeToModbusTwo(0xC0A, NumNine);
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0C08, buffer2);
			ins_write(SECONDBOARD, 0x0C08, buffer2);
			ins_write(THIRDBOARD, 0x0C08, buffer2);
			writeToModbusThree(0xC0A, NumNine);
		}
		else if (LightSignInt == 40)//3
		{
			writeToModbusFour(0x0C08, valueToWrite);
			writeToModbusFour(0xC0A, NumNine);
		}
		else if (LightSignInt == 50)//3
		{
			writeToModbusFive(0x0C08, valueToWrite);
			writeToModbusFive(0xC0A, NumNine);
		}
		else if (LightSignInt == 60)//3
		{
			writeToModbusSix(0x0C08, valueToWrite);
			writeToModbusSix(0xC0A, NumNine);
		}
		//qDebug() << "Write 1 to 0C08" << endl;
	}
	else
	{
		//ui->checkBox_IR->setEnabled(false);
		int PicNumCount = 8;//122089
		uint32_t NumEight = 8;
		uint32_t valueToWrite = 0;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0C08, buffer);//combine
			writeToModbusOne(0xC0A, NumEight);
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0C08, buffer);//1810
			ins_write(SECONDBOARD, 0x0C08, buffer);
			ins_write(THIRDBOARD, 0x0C08, buffer);
			writeToModbusThree(0xC0A, NumEight);
		}
		else if (LightSignInt == 50)//3
		{
			writeToModbusFive(0x0C08, valueToWrite);
			writeToModbusFive(0xC0A, NumEight);
		}
		//qDebug() << "Write 0 to 0C08" << endl;
	}
	return 0;
}


int InsPMDL_lightSouce::set_extended_trigger_output_quantity(int _board_num, uint32_t _quantity)
{
	uint16_t ExNumH = (_quantity >> 16) & 0xFFFF;
	uint16_t ExNumyL = _quantity & 0xFFFF;
	uint16_t buffer1[2] = { ExNumH,ExNumyL };
	if (_board_num == 1)
	{
		ins_write(FIRSTBOARD, 0x0C00, buffer1);//combine
	}
	else if (_board_num == 2)
	{
		writeToModbusTwo(0x0C00, _quantity);
	}
	else if (_board_num == 3)//3
	{
		ins_write(FIRSTBOARD, 0x0C00, buffer1);
		ins_write(SECONDBOARD, 0x0C00, buffer1);
		ins_write(THIRDBOARD, 0x0C00, buffer1);
	}
	else if (_board_num == 4)
	{
		writeToModbusFour(0x0C00, _quantity);
	}
	else if (_board_num == 5)
	{
		ins_write(FIRSTBOARD, 0x0C00, buffer1);
		ins_write(SECONDBOARD, 0x0C00, buffer1);
		ins_write(THIRDBOARD, 0x0C00, buffer1);
		ins_write(FOURTHBOARD, 0x0C00, buffer1);
		ins_write(FIFTHBOARD, 0x0C00, buffer1);
	}
	else if (_board_num == 6)
	{
		writeToModbusSix(0x0C00, _quantity);
	}
	return 0;
}
int InsPMDL_lightSouce::set_extended_trigger_output_delay(int _board_num, uint32_t _delay)
{
	uint32_t ExGap = _delay;

	ExGap = ExGap * 1000 / 8;

	uint16_t ExGapH = (ExGap >> 16) & 0xFFFF;
	uint16_t ExGapL = ExGap & 0xFFFF;
	uint16_t buffer1[2] = { ExGapH,ExGapL };

	if (_board_num == 1)
	{
		ins_write(FIRSTBOARD, 0x0C04, buffer1);//combine
	}

	else if (_board_num == 2)//3
	{
		writeToModbusTwo(0x0C04, ExGap);
	}
	else if (_board_num == 3)//3
	{
		ins_write(FIRSTBOARD, 0x0C04, buffer1);//1808
		ins_write(SECONDBOARD, 0x0C04, buffer1);
		ins_write(THIRDBOARD, 0x0C04, buffer1);
	}
	else if (_board_num == 4)//3
	{
		writeToModbusFour(0x0C04, ExGap);
	}
	else if (_board_num == 5)//3
	{
		writeToModbusFive(0x0C04, ExGap);
	}
	else if (_board_num == 6)//3
	{
		writeToModbusSix(0x0C04, ExGap);
	}
	return 0;
}
int InsPMDL_lightSouce::set_extended_trigger_output_width(int _board_num, uint32_t _width)
{
	//180C
	uint32_t ExWidth = _width;

	ExWidth = ExWidth * 1000 / 8;

	uint16_t ExWidthH = (ExWidth >> 16) & 0xFFFF;
	uint16_t ExWidthL = ExWidth & 0xFFFF;
	uint16_t buffer1[2] = { ExWidthH,ExWidthL };

	if (_board_num == 1)
	{
		ins_write(FIRSTBOARD, 0x0C06, buffer1);//combine
	}
	else if (_board_num == 2)
	{
		writeToModbusTwo(0x0C06, ExWidth);
	}
	else if (_board_num == 3)//3
	{
		ins_write(FIRSTBOARD, 0x0C06, buffer1);//180C
		ins_write(SECONDBOARD, 0x0C06, buffer1);
		ins_write(THIRDBOARD, 0x0C06, buffer1);
	}
	else if (_board_num == 4)
	{
		writeToModbusFour(0x0C06, ExWidth);
	}
	else if (_board_num == 5)
	{
		writeToModbusFive(0x0C06, ExWidth);
	}
	else if (_board_num == 6)
	{
		writeToModbusSix(0x0C06, ExWidth);
	}
	return 0;
}
int InsPMDL_lightSouce::set_extended_trigger_output_polarity(int _board_num, int _index)
{
	if (_index == 0)
	{
		uint32_t valueToWrite = 1;//上升沿
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;

		uint16_t buffer[2] = { highValue,lowValue };

		if (_board_num == 1)
		{
			ins_write(FIRSTBOARD, 0x0C02, buffer);//combine
		}
		else if (_board_num == 2)//5
		{
			writeToModbusTwo(0x0C02, valueToWrite);
		}
		else if (_board_num == 3)//3
		{
			ins_write(FIRSTBOARD, 0x0C02, buffer);//1804
			ins_write(SECONDBOARD, 0x0C02, buffer);
			ins_write(THIRDBOARD, 0x0C02, buffer);
		}
		else if (_board_num == 4)//5
		{
			writeToModbusFour(0x0C02, valueToWrite);
		}
		else if (_board_num == 5)//5
		{
			writeToModbusFive(0x0C02, valueToWrite);
		}
		else if (_board_num == 6)//5
		{
			writeToModbusSix(0x0C02, valueToWrite);
		}
	}
	else if (_index == 1)
	{
		uint32_t valueToWrite = 0;
		uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
		uint16_t lowValue = valueToWrite & 0xFFFF;
		uint16_t buffer2[2] = { highValue,lowValue };
		if (_board_num == 1)
		{
			ins_write(FIRSTBOARD, 0x0C02, buffer2);//combine
		}
		else if (_board_num == 2)//3
		{
			writeToModbusTwo(0x0C02, valueToWrite);
		}
		else if (_board_num == 3)//3
		{

			ins_write(FIRSTBOARD, 0x0C02, buffer2);
			ins_write(SECONDBOARD, 0x0C02, buffer2);
			ins_write(THIRDBOARD, 0x0C02, buffer2);
		}
		else if (_board_num == 4)//3
		{
			writeToModbusFour(0x0C02, valueToWrite);
		}
		else if (_board_num == 5)//3
		{
			writeToModbusFive(0x0C02, valueToWrite);
		}
		else if (_board_num == 6)//3
		{
			writeToModbusSix(0x0C02, valueToWrite);
		}
	}
	return 0;
}

//获取扩展触发输出数量(单位：次)
int InsPMDL_lightSouce::get_extended_trigger_output_quantity(uint32_t& _quantity)
{
	int ret = readFromModbusOne(0x0C00, _quantity);//数量
	return ret;
}
//获取扩展触发输出延迟(单位：微秒)
int InsPMDL_lightSouce::get_extended_trigger_output_delay(uint32_t& _delay)
{
	int ret = readFromModbusOne(0x0C04, _delay);//延迟
	_delay = _delay * 8 / 1000;
	return ret;
}
//获取扩展触发输出宽度(单位：微秒)
int InsPMDL_lightSouce::get_extended_trigger_output_width(uint32_t& _width)
{
	int ret = readFromModbusOne(0x0C06, _width);//宽度
	_width = _width * 8 / 1000;
	return ret;
}
//获取扩展触发输出极性,0-上升沿，1-下降沿
int InsPMDL_lightSouce::get_extended_trigger_output_polarity(uint32_t& _index)
{
	int ret = readFromModbusOne(0x0C02, _index);
	return ret;
}


//——————————高级设置——————————————————
//设置光源响应时间(单位)值必须＞0
int InsPMDL_lightSouce::set_light_source_response_time(int _board_num, double _delay)
{
	CHECKBOARDNUM(_board_num)
		uint32_t  LightDelay2 = _delay * 1000;
	LightDelay2 /= 8;
	if (LightDelay2 > 0)
	{
		uint16_t highValue = (LightDelay2 >> 16) & 0xFFFF;
		uint16_t lowValue = LightDelay2 & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		if (_board_num == 1)
		{
			ins_write(FIRSTBOARD, 0x1020, buffer);//combine
			return 0;
		}
		else if (_board_num == 2)
		{
			writeToModbusTwo(0x1020, LightDelay2);
			return 0;
		}
		else if (_board_num == 3)//3
		{
			ins_write(FIRSTBOARD, 0x1020, buffer);//17AC->2040  date:1206
			ins_write(SECONDBOARD, 0x1020, buffer);
			ins_write(THIRDBOARD, 0x1020, buffer);
			return 0;
		}
		else if (_board_num == 4)
		{
			writeToModbusFour(0x1020, LightDelay2);
			return 0;
		}
		else if (_board_num == 5)
		{
			writeToModbusFive(0x1020, LightDelay2);
			return 0;
		}
		else if (_board_num == 6)
		{
			writeToModbusSix(0x1020, LightDelay2);
			return 0;
		}
	}
	else
	{
		return -1;//to do
		//addToStateMessage("光源响应时间输入须大于0", true);
	}
}

//设置光源触发宽度
int InsPMDL_lightSouce::set_light_source_trigger_output_width(int _board_num, double _width)
{
	CHECKBOARDNUM(_board_num)
		int	LightSignInt = _board_num * 10;
	int PicNumCount = 8; //to do
	double LightWidth2 = _width;//光源触发宽度
	double LightWidthFre2 = 0;
	double line_fre = 0;
	get_scanning_period(LightWidthFre2, line_fre);//行扫描周期 做判断
	LightWidthFre2 *= 1000;
	uint32_t LightWidth = LightWidth2;
	uint32_t LightWidthFre = LightWidthFre2 / PicNumCount;
	if (LightWidth > LightWidthFre)
	{
		//addToStateMessage("输入值过大", true);
	}
	else
	{
		LightWidth /= 8;
		uint16_t highValue = (LightWidth >> 16) & 0xFFFF;
		uint16_t lowValue = LightWidth & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };

		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0B8C, buffer);//combine
			ins_write(FIRSTBOARD, 0x0BBC, buffer);//combine
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0B8C, LightWidth);
			writeToModbusTwo(0x0BBC, LightWidth);
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0B8C, buffer);//1718
			ins_write(SECONDBOARD, 0x0B8C, buffer);
			ins_write(THIRDBOARD, 0x0B8C, buffer);

			ins_write(FIRSTBOARD, 0x0BBC, buffer);//1778
			ins_write(SECONDBOARD, 0x0BBC, buffer);
			ins_write(THIRDBOARD, 0x0BBC, buffer);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0B8C, LightWidth);
			writeToModbusFour(0x0BBC, LightWidth);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B8C, LightWidth);
			writeToModbusFive(0x0BBC, LightWidth);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0B8C, LightWidth);
			writeToModbusSix(0x0BBC, LightWidth);
		}

		uint32_t LightWidth3 = LightWidth2;
		LightWidthFre = LightWidth3 * 2;
		LightWidthFre /= 8;

		uint16_t highValue2 = (LightWidthFre >> 16) & 0xFFFF;
		uint16_t lowValue2 = LightWidthFre & 0xFFFF;
		uint16_t buffer2[2] = { highValue2,lowValue2 };

		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0B84, buffer2);//combine
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x0B84, LightWidthFre);
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0B84, buffer2);//1708
			ins_write(SECONDBOARD, 0x0B84, buffer2);
			ins_write(THIRDBOARD, 0x0B84, buffer2);
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x0B84, LightWidthFre);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B84, LightWidthFre);
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x0B84, LightWidthFre);
		}
		//addToStateMessage("光源触发宽度输入成功", false);
	}
	/*
	if (LightWidth > LightWidthFre / 2)
	{
		return -1;//to do
		//addToStateMessage("输入值过大", true);
	}
	else
	{
		LightWidth /= 8;
		uint16_t highValue = (LightWidth >> 16) & 0xFFFF;
		uint16_t lowValue = LightWidth & 0xFFFF;
		uint16_t buffer[2] = { highValue,lowValue };
		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0B8C, buffer);//combine
			ins_write(FIRSTBOARD, 0x0BBC, buffer);//combine
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0B8C, buffer);//1718
			ins_write(SECONDBOARD, 0x0B8C, buffer);
			ins_write(THIRDBOARD, 0x0B8C, buffer);
			ins_write(FIRSTBOARD, 0x0BBC, buffer);//1778
			ins_write(SECONDBOARD, 0x0BBC, buffer);
			ins_write(THIRDBOARD, 0x0BBC, buffer);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B8C, LightWidth);
			writeToModbusFive(0x0BBC, LightWidth);
		}
		LightWidthFre -= 1000;
		LightWidthFre /= 8;
		uint16_t highValue2 = (LightWidthFre >> 16) & 0xFFFF;
		uint16_t lowValue2 = LightWidthFre & 0xFFFF;
		uint16_t buffer2[2] = { highValue2,lowValue2 };

		if (LightSignInt == 10)
		{
			ins_write(FIRSTBOARD, 0x0B84, buffer2);//combine
		}
		else if (LightSignInt == 30)//3
		{
			ins_write(FIRSTBOARD, 0x0B84, buffer2);//1708
			ins_write(SECONDBOARD, 0x0B84, buffer2);
			ins_write(THIRDBOARD, 0x0B84, buffer2);
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x0B84, LightWidthFre);
		}
		return 0;
		//addToStateMessage("光源触发宽度输入成功", false);
	}*/
}
//设置光源亮度调节
int InsPMDL_lightSouce::set_light_source_brightness_adjustment(int _board_num, uint32_t _percentage)
{
	CHECKBOARDNUM(_board_num)
		uint32_t BrightNess = _percentage;
	if (BrightNess >= 1 && BrightNess <= 100)
	{
		uint32_t BrightNessToReg = 256 + 79 * BrightNess - 1;
		uint32_t BrightNessOne = 1;
		if (_board_num == 1)
		{
			writeToModbusOne(0x101A, BrightNessToReg);//2034
			writeToModbusOne(0x101C, BrightNessToReg);//2038
			writeToModbusOne(0x1024, BrightNessOne);//2048
		}
		else if (_board_num == 2)
		{
			writeToModbusTwo(0x101A, BrightNessToReg);//2034
			writeToModbusTwo(0x101C, BrightNessToReg);//2038
			writeToModbusTwo(0x1024, BrightNessOne);//2048
		}
		else if (_board_num == 3)
		{
			writeToModbusThree(0x101A, BrightNessToReg);//2034
			writeToModbusThree(0x101C, BrightNessToReg);//2038
			writeToModbusThree(0x1024, BrightNessOne);//2048
		}
		else if (_board_num == 4)
		{
			writeToModbusFour(0x101A, BrightNessToReg);//2034
			writeToModbusFour(0x101C, BrightNessToReg);//2038
			writeToModbusFour(0x1024, BrightNessOne);//2048
		}
		else if (_board_num == 5)
		{
			writeToModbusFive(0x101A, BrightNessToReg);//2034
			writeToModbusFive(0x101C, BrightNessToReg);//2038
			writeToModbusFive(0x1024, BrightNessOne);//2048
		}
		else if (_board_num == 6)
		{
			writeToModbusSix(0x101A, BrightNessToReg);//2034
			writeToModbusSix(0x101C, BrightNessToReg);//2038
			writeToModbusSix(0x1024, BrightNessOne);//2048
		}
		return 0;
		//addToStateMessage("亮度输入成功", false);
	}
	else
	{
		return -1;//to do 
		//addToStateMessage("亮度输入应为整数 , 范围：1~100", true);
	}
}
//设置编码器触发脉宽
int InsPMDL_lightSouce::set_encoder_trigger_pulse_width(int _board_num, double _encode_width)
{
	CHECKBOARDNUM(_board_num)
		int	LightSignInt = _board_num * 10;
	//1640
	double EncodeWidth = _encode_width;//1640
	uint32_t EncodeWidth2 = EncodeWidth * 1000 / 8;
	uint16_t highValue = (EncodeWidth2 >> 16) & 0xFFFF;
	uint16_t lowValue = EncodeWidth2 & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	if (LightSignInt == 10)
	{
		ins_write(FIRSTBOARD, 0x0B20, buffer);//combine
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 20)//3
	{
		writeToModbusTwo(0x0B20, EncodeWidth2);
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 30)
	{
		ins_write(FIRSTBOARD, 0x0B20, buffer);//1640
		ins_write(SECONDBOARD, 0x0B20, buffer);
		ins_write(THIRDBOARD, 0x0B20, buffer);
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 40)//3
	{
		writeToModbusFour(0x0B20, EncodeWidth2);
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 50)//3
	{
		writeToModbusFive(0x0B20, EncodeWidth2);
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
	else if (LightSignInt == 60)//3
	{
		writeToModbusSix(0x0B20, EncodeWidth2);
		return 0;
		//addToStateMessage("编码器触发脉宽输入成功", false);
	}
}
//设置光源触发输出数量
int InsPMDL_lightSouce::set_light_source_trigger_output_quantity(int _board_num, uint32_t _quantity)
{
	CHECKBOARDNUM(_board_num)
		int	LightSignInt = _board_num * 10;
	//uint32_t CamNum = ui->ExNumUI->text().toInt() + 8;//1814
	uint32_t CamNum = 0;
	get_extended_trigger_output_quantity(CamNum);//扩展触发输出数量
	uint16_t highValue = (CamNum >> 16) & 0xFFFF;
	uint16_t lowValue = CamNum & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	if (LightSignInt == 10)
	{
		ins_write(FIRSTBOARD, 0x0C0A, buffer);//combine
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x0C0A, CamNum);
	}
	else if (LightSignInt == 30)//3
	{
		ins_write(FIRSTBOARD, 0x0C0A, buffer);//1814
		ins_write(SECONDBOARD, 0x0C0A, buffer);
		ins_write(THIRDBOARD, 0x0C0A, buffer);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x0C0A, CamNum);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x0C0A, CamNum);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x0C0A, CamNum);
	}
	return 0;
}
//获取光源响应时间(单位：微秒)
int InsPMDL_lightSouce::get_light_source_response_time(double& _delay)
{
	uint32_t delay;
	int ret = readFromModbusOne(0x1020, delay);
	_delay = ((double)(delay * 8)) / 1000;
	return ret;
}
//获取光源触发宽度(单位：微秒)
int InsPMDL_lightSouce::get_light_source_trigger_output_width(int _board_num, double& _width)
{
	return 0;
	//to do
}
//获取光源亮度调节(单位：%),亮度输入应为整数 , 范围：1~100
int InsPMDL_lightSouce::get_light_source_brightness_adjustment(int _board_num, uint32_t& _percentage)
{
	uint32_t BrightNessToReg;
	int ret = readFromModbusOne(0x101A, BrightNessToReg);//2034
	_percentage = (BrightNessToReg + 1 - 256) / 79;
	return ret;
}
//获取编码器触发脉宽(单位：微秒)
int InsPMDL_lightSouce::get_encoder_trigger_pulse_width(int _board_num, double& _encode_width)
{
	int ret = 0;

	return ret;
	//to do
}

int InsPMDL_lightSouce::get_light_source_trigger_output_quantity(uint32_t& _quantity)
{
	int ret = readFromModbusOne(0x0C0A, _quantity);//光源触发数量
	return ret;
}

int InsPMDL_lightSouce::set_soft_trigger_num(uint32_t _num)
{
	uint16_t highValue = (_num >> 16) & 0xFFFF;
	uint16_t lowValue = _num & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B02, buffer);
	return 0;
}
int InsPMDL_lightSouce::set_soft_trigger_period(double _period)
{
	uint32_t Num3 = static_cast<uint32_t>(_period * 1000);
	Num3 /= 8;
	uint16_t highValue = (Num3 >> 16) & 0xFFFF;
	uint16_t lowValue = Num3 & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };
	ins_write(FIRSTBOARD, 0x0B00, buffer);
	return 0;
}

//获取软触发次数(单位：次)
int InsPMDL_lightSouce::get_soft_trigger_num(uint32_t& _num)
{
	int ret = readFromModbusOne(0x0B02, _num);//软触发次数
	return ret;
}
//获取软触发周期(单位：微秒)
int InsPMDL_lightSouce::get_soft_trigger_period(double& _period)
{
	uint32_t u_period = 0;
	int ret = readFromModbusOne(0x0B00, u_period);//软触发次数
	_period = (static_cast<double>(u_period * 8)) / 1000;
	return ret;
}





#include <iostream>
#include <fstream>
#include <chrono>
//#include <ctime>
#include <iomanip>


#include "..\include\Ins_Utility_proc.h"
#include "..\include\modbus_udp.h"

#define BUF_SIZE 128
using namespace std;


#define OFFSET

#ifdef OFFSET
#define ADDR_OFFSET_HALVE
#ifdef ADDR_OFFSET_HALVE
#define SYSTEM_ADDR 0x00000000
//#define DEVICE_PHY_ADDR 0x00001000
#define DEVICE_DAC_ADDR 0x00000880
#define DEVICE_temp_ADDR 0x00000900
//#define DEVICE_MPU0_ADDR 0x00001300
#define DEVICE_laser_ADDR 0x00000A00
//#define DEVICE_MPU1_ADDR 0x00001500
#define DEVICE_trig_cfg_ADDR 0x00000B00
#define DEVICE_trig_out_ADDR 0x00000B80
#define CONTROLLER_loop_mode_ADDR 0x00001000
#define CONTROLLER_test_mode_ADDR 0x00001080
#define CONTROLLER_core_ctrl_ADDR 0x00001100

#define CONTROLLER_step_forward_ADDR 0x00001180
#define CONTROLLER_step_phase_ADDR 0x00001200
#define CONTROLLER_step_base_ADDR 0x00001280

#define WAVE_wave_list_ADDR 0x00001800
#define WAVE_wave_adjust_ADDR 0x00001880
#define CHECK_num_check_ADDR 0x00003800
#define CHECK_miss_check_ADDR 0x00003880


struct  SYSTEM
{
	uint16_t COMPANY;
	uint16_t DEVICE;
	uint16_t DEVICE_SUB;
	uint16_t INTERFACETYPE;
	//uint4_t PIXELTYPE;
	//uint4_t DPITYPE;
	//uint4_t SPEEDTYPE;
	uint16_t YEAR;
	//uint4_t MONTH;
	//uint4_t DATE;
	uint8_t HOUR;
	uint8_t MINUTE;
	uint16_t REV;
	uint16_t reg_soft_reset;
	uint16_t reg_test;
	uint16_t reg_device_temp;
};


struct DEVICE_DAC
{
	uint16_t reg_mos_ack_time;
	uint16_t reg_dds_ack_time;
	uint16_t reg_dac_ack_time;
	uint16_t reg_sw_ack_time;
};



struct DEVICE_trig_cfg
{
	uint16_t reg_soft_trigger_cycle;
	uint16_t reg_soft_trigger_num;
	uint16_t reg_soft_trigger_en;
	uint16_t reg_trigger_cycle;
	uint16_t reg_trigger_num;
	uint16_t reg_trigger_mode;
	uint16_t reg_trigger_width;
	uint16_t reg_trigger_delay;
	uint16_t reg_trigger_pulse;
	uint16_t reg_trigger_polar;
	uint16_t reg_trigger_en;
	uint16_t reg_encoder_phase;
	uint16_t reg_encoder_cnt_mode;
	uint16_t reg_encoder_dis_mode;
	uint16_t reg_encoder_ignore;
	uint16_t reg_encoder_div;
	uint16_t reg_encoder_width;
	uint16_t reg_encoder_location;
	uint16_t reg_encoder_multi_en;
	uint16_t reg_encoder_multi_coe;
	uint16_t reg_encoder_a_cnt;
	uint16_t reg_encoder_b_cnt;
	uint16_t reg_encoder_clr;
	uint16_t reg_encoder_en;
	uint16_t reg_slave_device;
	uint16_t reg_status_cnt_clr;
	uint16_t reg_NULL0;//空寄存器
	uint16_t reg_NULL1;//空寄存器
	uint16_t reg_NULL2;//空寄存器
	uint16_t reg_NULL3;//空寄存器
	uint16_t reg_NULL4;//空寄存器
	uint16_t reg_NULL5;//空寄存器
	uint16_t reg_l1_soft_trigger_cycle;
	uint16_t reg_l1_soft_trigger_num;
	uint16_t reg_l1_soft_trigger_en;
	uint16_t reg_l1_trigger_cycle;
	uint16_t reg_l1_trigger_num;
	uint16_t reg_l1_trigger_mode;
	uint16_t reg_l1_trigger_width;
	uint16_t reg_l1_trigger_delay;
	uint16_t reg_l1_trigger_pulse;
	uint16_t reg_l1_trigger_polar;
	uint16_t reg_l1_trigger_en;
	uint16_t reg_l1_encoder_phase;
	uint16_t reg_l1_encoder_cnt_mode;
	uint16_t reg_l1_encoder_dis_mode;
	uint16_t reg_l1_encoder_ignore;
	uint16_t reg_l1_encoder_div;
	uint16_t reg_l1_encoder_width;
	uint16_t reg_l1_encoder_location;
	uint16_t reg_l1_encoder_multi_en;
	uint16_t reg_l1_encoder_multi_coe;
	uint16_t reg_l1_encoder_a_cnt;
	uint16_t reg_l1_encoder_b_cnt;
	uint16_t reg_l1_encoder_clr;
	uint16_t reg_l1_encoder_en;
	uint16_t reg_l1_status_cnt_clr;

};


struct DEVICE_trig_out
{
	uint16_t reg_exp_chan;
	uint16_t reg_exposure_time;
	uint16_t reg_led_cnt_max;
	uint16_t reg_trigger_multi_en;
	uint16_t reg_led_polar;
	uint16_t reg_led_pwm_start_0;
	uint16_t reg_led_pwm_end_0;
	uint16_t reg_led_pwm_start_1;
	uint16_t reg_led_pwm_end_1;
	uint16_t reg_led_pwm_start_2;
	uint16_t reg_led_pwm_end_2;
	uint16_t reg_led_pwm_start_3;
	uint16_t reg_led_pwm_end_3;
	uint16_t reg_led_pwm_start_4;
	uint16_t reg_led_pwm_end_4;
	uint16_t reg_led_pwm_start_5;
	uint16_t reg_led_pwm_end_5;
	uint16_t reg_led_pwm_start_6;
	uint16_t reg_led_pwm_end_6;
	uint16_t reg_led_pwm_start_7;
	uint16_t reg_led_pwm_end_7;
	uint16_t reg_led_pwm_start_8;
	uint16_t reg_led_pwm_end_8;
	uint16_t reg_led_pwm_start_9;
	uint16_t reg_led_pwm_end_9;
	uint16_t reg_led_pwm_start_10;
	uint16_t reg_led_pwm_end_10;
	uint16_t reg_led_pwm_start_11;
	uint16_t reg_led_pwm_end_11;
	uint16_t reg_trig_start_0;
	uint16_t reg_trig_end_0;
	uint16_t reg_trig_start_1;
	uint16_t reg_trig_end_1;
	uint16_t reg_trig_start_2;
	uint16_t reg_trig_end_2;
	uint16_t reg_trig_start_3;
	uint16_t reg_trig_end_3;
	uint16_t reg_trig_start_4;
	uint16_t reg_trig_end_4;
	uint16_t reg_trig_start_5;
	uint16_t reg_trig_end_5;
	uint16_t reg_trig_out_polar;

};


struct CONTROLLER_loop_mode
{
	uint16_t reg_dds_phase;
	uint16_t reg_dds_inc;
	uint16_t reg_exp_cycle;
	uint16_t reg_trigger_gap;
	uint16_t reg_pic_num;
	uint16_t reg_sw_status;
	uint16_t reg_sw_shift;
	uint16_t reg_sw_loop_gap;
	uint16_t reg_sw_loop_num;
	uint16_t reg_dds_phase_offset;
	uint16_t reg_dds_direction_x;
	uint16_t reg_dds_direction_y;
	uint16_t reg_current_offset;
	uint16_t reg_dac_value_forward;
	uint16_t reg_dac_value_backward;
	uint16_t reg_sw_wait;
	uint16_t reg_camera_delay;
	uint16_t reg_camera_cycle;
	uint16_t reg_dac_req;

};


struct CONTROLLER_test_mode
{
	uint16_t reg_dac_value_x0;
	uint16_t reg_sw_value_x0;
	uint16_t reg_mos_value_x0;
	uint16_t reg_trigger_req_x0;
	uint16_t reg_dac_value_x1;
	uint16_t reg_sw_value_x1;
	uint16_t reg_mos_value_x1;
	uint16_t reg_trigger_req_x1;
	uint16_t reg_dac_value_x2;
	uint16_t reg_sw_value_x2;
	uint16_t reg_mos_value_x2;
	uint16_t reg_trigger_req_x2;
	uint16_t reg_dac_value_x3;
	uint16_t reg_sw_value_x3;
	uint16_t reg_mos_value_x3;
	uint16_t reg_trigger_req_x3;
	uint16_t reg_dac_value_y0;
	uint16_t reg_sw_value_y0;
	uint16_t reg_mos_value_y0;
	uint16_t reg_trigger_req_y0;
	uint16_t reg_dac_value_y1;
	uint16_t reg_sw_value_y1;
	uint16_t reg_mos_value_y1;
	uint16_t reg_trigger_req_y1;
	uint16_t reg_dac_value_y2;
	uint16_t reg_sw_value_y2;
	uint16_t reg_mos_value_y2;
	uint16_t reg_trigger_req_y2;
	uint16_t reg_dac_value_y3;
	uint16_t reg_sw_value_y3;
	uint16_t reg_mos_value_y3;
	uint16_t reg_trigger_req_y3;
};


struct CONTROLLER_core_ctrl
{
	uint16_t reg_core_en;
	uint16_t reg_core_mode;
};

struct CONTROLLER_step_forward
{
	uint16_t reg_step_en;
	uint16_t reg_step_pic_num;
	uint16_t reg_step_x_seq;
	uint16_t reg_step_y_seq;
};
//11.24
struct CONTROLLER_step_phase
{
	uint16_t reg_step_phase_0;
	uint16_t reg_step_phase_1;
	uint16_t reg_step_phase_2;
	uint16_t reg_step_phase_3;
	uint16_t reg_step_phase_4;
	uint16_t reg_step_phase_5;
	uint16_t reg_step_phase_6;
	uint16_t reg_step_phase_7;
	uint16_t reg_step_phase_8;
	uint16_t reg_step_phase_9;
	uint16_t reg_step_phase_10;
	uint16_t reg_step_phase_11;
	uint16_t reg_step_phase_12;
	uint16_t reg_step_phase_13;
	uint16_t reg_step_phase_14;
	uint16_t reg_step_phase_15;
	uint16_t reg_step_phase_16;
	uint16_t reg_step_phase_17;
	uint16_t reg_step_phase_18;
	uint16_t reg_step_phase_19;
	uint16_t reg_step_phase_20;
	uint16_t reg_step_phase_21;
	uint16_t reg_step_phase_22;
	uint16_t reg_step_phase_23;
	uint16_t reg_step_phase_24;
	uint16_t reg_step_phase_25;
	uint16_t reg_step_phase_26;
	uint16_t reg_step_phase_27;
	uint16_t reg_step_phase_28;
	uint16_t reg_step_phase_29;
	uint16_t reg_step_phase_30;
	uint16_t reg_step_phase_31;
	uint16_t reg_step_inc_0;
	uint16_t reg_step_inc_1;
	uint16_t reg_step_inc_2;
	uint16_t reg_step_inc_3;
	uint16_t reg_step_inc_4;
	uint16_t reg_step_inc_5;
	uint16_t reg_step_inc_6;
	uint16_t reg_step_inc_7;
	uint16_t reg_step_inc_8;
	uint16_t reg_step_inc_9;
	uint16_t reg_step_inc_10;
	uint16_t reg_step_inc_11;
	uint16_t reg_step_inc_12;
	uint16_t reg_step_inc_13;
	uint16_t reg_step_inc_14;
	uint16_t reg_step_inc_15;
	uint16_t reg_step_inc_16;
	uint16_t reg_step_inc_17;
	uint16_t reg_step_inc_18;
	uint16_t reg_step_inc_19;
	uint16_t reg_step_inc_20;
	uint16_t reg_step_inc_21;
	uint16_t reg_step_inc_22;
	uint16_t reg_step_inc_23;
	uint16_t reg_step_inc_24;
	uint16_t reg_step_inc_25;
	uint16_t reg_step_inc_26;
	uint16_t reg_step_inc_27;
	uint16_t reg_step_inc_28;
	uint16_t reg_step_inc_29;
	uint16_t reg_step_inc_30;
	uint16_t reg_step_inc_31;
};

//11.24
struct CONTROLLER_step_base
{
	uint16_t reg_step_base_0;
	uint16_t reg_step_base_1;
	uint16_t reg_step_base_2;
	uint16_t reg_step_base_3;
	uint16_t reg_step_base_4;
	uint16_t reg_step_base_5;
	uint16_t reg_step_base_6;
	uint16_t reg_step_base_7;
	uint16_t reg_step_base_8;
	uint16_t reg_step_base_9;
	uint16_t reg_step_base_10;
	uint16_t reg_step_base_11;
	uint16_t reg_step_base_12;
	uint16_t reg_step_base_13;
	uint16_t reg_step_base_14;
	uint16_t reg_step_base_15;
	uint16_t reg_step_base_16;
	uint16_t reg_step_base_17;
	uint16_t reg_step_base_18;
	uint16_t reg_step_base_19;
	uint16_t reg_step_base_20;
	uint16_t reg_step_base_21;
	uint16_t reg_step_base_22;
	uint16_t reg_step_base_23;
	uint16_t reg_step_base_24;
	uint16_t reg_step_base_25;
	uint16_t reg_step_base_26;
	uint16_t reg_step_base_27;
	uint16_t reg_step_base_28;
	uint16_t reg_step_base_29;
	uint16_t reg_step_base_30;
	uint16_t reg_step_base_31;
};

struct WAVE_wave_list
{
	uint16_t reg_ram_cfg_en;
	uint16_t reg_ram_whrl;
	uint16_t reg_ram_addr;
	uint16_t reg_ram_wdata;
	uint16_t reg_ram_rdata;
	uint16_t reg_ram_req;
	uint16_t reg_ram_done;
	uint16_t reg_wave_start_addr;
	uint16_t reg_wave_end_addr;
};

//弃用
struct WAVE_wave_adjust
{
	uint16_t reg_adjust_en;
	uint16_t reg_adjust_gain;
	uint16_t reg_adjust_offset;
};


struct CHECK_num_check
{
	uint16_t reg_num_check_clr;
	uint16_t reg_io_in_0_num;
	uint16_t reg_io_in_1_num;
	uint16_t reg_encoder_a_num;
	uint16_t reg_encoder_b_num;
	uint16_t reg_io_out_num;
	uint16_t reg_mos_req_num;
	uint16_t reg_mos_ack_num;
	uint16_t reg_dds_req_num;
	uint16_t reg_dds_ack_num;
	uint16_t reg_dac_req_num;
	uint16_t reg_dac_ack_num;
	uint16_t reg_sw_req_num;
	uint16_t reg_sw_ack_num;
	uint16_t reg_reg_req_num;
	uint16_t reg_reg_ack_num;
	uint16_t reg_sum_err_num;

};


struct CHECK_miss_check
{
	uint8_t reg_core_status;
};



#else

#define SYSTEM_ADDR 0x00000000
//#define DEVICE_PHY_ADDR 0x00001000
#define DEVICE_DAC_ADDR 0x00001100
#define DEVICE_temp_ADDR 0x00001200
//#define DEVICE_MPU0_ADDR 0x00001300
#define DEVICE_laser_ADDR 0x00001400
//#define DEVICE_MPU1_ADDR 0x00001500
#define DEVICE_trig_cfg_ADDR 0x00001600
#define DEVICE_trig_out_ADDR 0x00001700
#define CONTROLLER_loop_mode_ADDR 0x00002000
#define CONTROLLER_test_mode_ADDR 0x00002100
#define CONTROLLER_core_ctrl_ADDR 0x00002200
#define CONTROLLER_step_forward_ADDR 0x00002300
#define CONTROLLER_step_phase_ADDR 0x00002400
#define CONTROLLER_step_base_ADDR 0x00002500
#define WAVE_wave_list_ADDR 0x00003000
#define WAVE_wave_adjust_ADDR 0x00003100
#define CHECK_num_check_ADDR 0x00007000
#define CHECK_miss_check_ADDR 0x00007100

struct  SYSTEM
{
	uint32_t COMPANY;
	uint32_t DEVICE;
	uint32_t DEVICE_SUB;
	uint8_t INTERFACETYPE;
	uint8_t PIXELTYPE;
	uint8_t DPITYPE;
	uint8_t SPEEDTYPE;
	uint16_t YEAR;
	uint8_t MONTH;
	uint8_t DATE;
	uint16_t HOUR;
	uint16_t MINUTE;
	uint32_t REV;
	uint32_t reg_soft_reset;
	uint32_t reg_test;
	uint32_t reg_device_temp;
};
typedef SYSTEM* SYSTEM_PTR;
SYSTEM* SYSTEM_MODULE = (SYSTEM_PTR)(SYSTEM_ADDR);

struct DEVICE_DAC
{
	uint32_t reg_mos_ack_time;
	uint32_t reg_dds_ack_time;
	uint32_t reg_dac_ack_time;
	uint32_t reg_sw_ack_time;
};
typedef DEVICE_DAC* DEVICE_DAC_PTR;
DEVICE_DAC* DEVCIE_DAC_MODULE = (DEVICE_DAC*)(DEVICE_DAC_ADDR);

/*struct DEVICE_MPU0
{
	uint32_t reg_iic_whrl;
	uint32_t reg_iic_dev_addr;
	uint32_t reg_iic_addr;
	uint32_t reg_iic_wdata;
	uint32_t reg_iic_rdata;
	uint32_t reg_iic_req;
	uint32_t reg_iic_done;
};
typedef DEVICE_MPU0* DEVICE_MPU0_PTR;
DEVICE_MPU0_PTR DEVICE_MPU0_MODULE = DEVICE_MPU0_PTR(DEVICE_MPU0_ADDR);

struct DEVICE_MPU1
{
	uint32_t reg_mcu1_angle_x;
	uint32_t reg_mcu1_angle_y;
	uint32_t reg_mcu1_angle_z;
	uint32_t reg_mcu1_temp;
	uint32_t reg_mcu1_cfg_addr;
	uint32_t reg_mcu1_cfg_value;
	uint32_t reg_mcu1_cfg_req;
	uint32_t reg_mcu1_cfg_done;
	uint32_t reg_mcu1_bps;
};
typedef DEVICE_MPU1* DEVICE_MPU1_PTR;
DEVICE_MPU1_PTR DEVICE_MPU1_MODULE = DEVICE_MPU1_PTR(DEVICE_MPU1_ADDR);
*/
struct DEVICE_trig_cfg
{
	uint32_t reg_soft_trigger_cycle;
	uint32_t reg_soft_trigger_num;
	uint32_t reg_soft_trigger_en;
	uint32_t reg_trigger_cycle;
	uint32_t reg_trigger_num;
	uint32_t reg_trigger_mode;
	uint32_t reg_trigger_width;
	uint32_t reg_trigger_delay;
	uint32_t reg_trigger_pulse;
	uint32_t reg_trigger_polar;
	uint32_t reg_trigger_en;
	uint32_t reg_encoder_phase;
	uint32_t reg_encoder_cnt_mode;
	uint32_t reg_encoder_dis_mode;
	uint32_t reg_encoder_ignore;
	uint32_t reg_encoder_div;
	uint32_t reg_encoder_width;
	uint32_t reg_encoder_location;
	uint32_t reg_encoder_multi_en;
	uint32_t reg_encoder_multi_coe;
	uint32_t reg_encoder_a_cnt;
	uint32_t reg_encoder_b_cnt;
	uint32_t reg_encoder_clr;
	uint32_t reg_encoder_en;
	uint32_t reg_slave_device;
	uint32_t reg_status_cnt_clr;
};
typedef DEVICE_trig_cfg* DEVICE_trig_cfg_PTR;
DEVICE_trig_cfg_PTR DEVICE_trig_cfg_MODULE = DEVICE_trig_cfg_PTR(DEVICE_trig_cfg_ADDR);

struct DEVICE_trig_out
{
	uint32_t reg_exp_chan;
	uint32_t reg_exposure_time;
	uint32_t reg_led_cnt_max;
	uint32_t reg_trigger_multi_en;
	uint32_t reg_led_polar;
	uint32_t reg_led_pwm_start_0;
	uint32_t reg_led_pwm_end_0;
	uint32_t reg_led_pwm_start_1;
	uint32_t reg_led_pwm_end_1;
	uint32_t reg_led_pwm_start_2;
	uint32_t reg_led_pwm_end_2;
	uint32_t reg_led_pwm_start_3;
	uint32_t reg_led_pwm_end_3;
	uint32_t reg_led_pwm_start_4;
	uint32_t reg_led_pwm_end_4;
	uint32_t reg_led_pwm_start_5;
	uint32_t reg_led_pwm_end_5;
	uint32_t reg_led_pwm_start_6;
	uint32_t reg_led_pwm_end_6;
	uint32_t reg_led_pwm_start_7;
	uint32_t reg_led_pwm_end_7;
	uint32_t reg_led_pwm_start_8;
	uint32_t reg_led_pwm_end_8;
	uint32_t reg_led_pwm_start_9;
	uint32_t reg_led_pwm_end_9;
	uint32_t reg_led_pwm_start_10;
	uint32_t reg_led_pwm_end_10;
	uint32_t reg_led_pwm_start_11;
	uint32_t reg_led_pwm_end_11;
	uint32_t reg_trig_start_0;
	uint32_t reg_trig_end_0;
	uint32_t reg_trig_start_1;
	uint32_t reg_trig_end_1;
	uint32_t reg_trig_start_2;
	uint32_t reg_trig_end_2;
	uint32_t reg_trig_start_3;
	uint32_t reg_trig_end_3;
	uint32_t reg_trig_start_4;
	uint32_t reg_trig_end_4;
	uint32_t reg_trig_start_5;
	uint32_t reg_trig_end_5;
	uint32_t reg_trig_out_polar;
	uint32_t reg_camera_cycle;
	uint32_t reg_camera_delay;
};

typedef DEVICE_trig_out* DEVICE_trig_out_PTR;
DEVICE_trig_out_PTR DEVICE_trig_out_MODULE = DEVICE_trig_out_PTR(DEVICE_trig_out_ADDR);

struct CONTROLLER_loop_mode
{
	uint32_t reg_dds_phase;
	uint32_t reg_dds_inc;
	uint32_t reg_exp_cycle;
	uint32_t reg_trigger_gap;
	uint32_t reg_pic_num;
	uint32_t reg_sw_status;
	uint32_t reg_sw_shift;
	uint32_t reg_sw_loop_gap;
	uint32_t reg_sw_loop_num;
	uint32_t reg_dds_phase_offset;
	uint32_t reg_dds_direction_x;
	uint32_t reg_dds_direction_y;
};
typedef CONTROLLER_loop_mode* CONTROLLER_loop_mode_PTR;
CONTROLLER_loop_mode_PTR CONTROLLER_loop_mode_MODULE = CONTROLLER_loop_mode_PTR(CONTROLLER_loop_mode_ADDR);

struct CONTROLLER_test_mode
{
	uint32_t reg_dac_value_x0;
	uint32_t reg_sw_value_x0;
	uint32_t reg_mos_value_x0;
	uint32_t reg_trigger_req_x0;
	uint32_t reg_dac_value_x1;
	uint32_t reg_sw_value_x1;
	uint32_t reg_mos_value_x1;
	uint32_t reg_trigger_req_x1;
	uint32_t reg_dac_value_x2;
	uint32_t reg_sw_value_x2;
	uint32_t reg_mos_value_x2;
	uint32_t reg_trigger_req_x2;
	uint32_t reg_dac_value_x3;
	uint32_t reg_sw_value_x3;
	uint32_t reg_mos_value_x3;
	uint32_t reg_trigger_req_x3;
	uint32_t reg_dac_value_y0;
	uint32_t reg_sw_value_y0;
	uint32_t reg_mos_value_y0;
	uint32_t reg_trigger_req_y0;
	uint32_t reg_dac_value_y1;
	uint32_t reg_sw_value_y1;
	uint32_t reg_mos_value_y1;
	uint32_t reg_trigger_req_y1;
	uint32_t reg_dac_value_y2;
	uint32_t reg_sw_value_y2;
	uint32_t reg_mos_value_y2;
	uint32_t reg_trigger_req_y2;
	uint32_t reg_dac_value_y3;
	uint32_t reg_sw_value_y3;
	uint32_t reg_mos_value_y3;
	uint32_t reg_trigger_req_y3;
};
typedef CONTROLLER_test_mode* CONTROLLER_test_mode_PTR;
CONTROLLER_test_mode_PTR CONTROLLER_test_mode_MODULE = CONTROLLER_test_mode_PTR(CONTROLLER_test_mode_ADDR);

struct CONTROLLER_core_ctrl
{
	uint32_t reg_core_en;
	uint32_t reg_core_mode;
};
typedef CONTROLLER_core_ctrl* CONTROLLER_core_ctrl_PTR;
CONTROLLER_core_ctrl_PTR CONTROLLER_core_ctrl_MODULE = CONTROLLER_core_ctrl_PTR(CONTROLLER_core_ctrl_ADDR);

struct WAVE_wave_list
{
	uint32_t reg_ram_cfg_en;
	uint32_t reg_ram_whrl;
	uint32_t reg_ram_addr;
	uint32_t reg_ram_wdata;
	uint32_t reg_ram_rdata;
	uint32_t reg_ram_req;
	uint32_t reg_ram_done;
	uint32_t reg_wave_start_addr;
	uint32_t reg_wave_end_addr;
};
typedef WAVE_wave_list* WAVE_wave_list_PTR;
WAVE_wave_list_PTR WAVE_wave_list_MODULE = WAVE_wave_list_PTR(WAVE_wave_list_ADDR);

struct WAVE_wave_adjust
{
	uint32_t reg_adjust_en;
	uint32_t reg_adjust_gain;
	uint32_t reg_adjust_offset;
};
typedef WAVE_wave_adjust* WAVE_wave_adjust_PTR;
WAVE_wave_adjust_PTR WAVE_wave_adjust_MODULE = WAVE_wave_adjust_PTR(WAVE_wave_adjust_ADDR);

struct CHECK_num_check
{
	uint32_t reg_num_check_clr;
	uint32_t reg_io_in_0_num;
	uint32_t reg_io_in_1_num;
	uint32_t reg_encoder_a_num;
	uint32_t reg_encoder_b_num;
	uint32_t reg_io_out_num;
	uint32_t reg_mos_req_num;
	uint32_t reg_mos_ack_num;
	uint32_t reg_dds_req_num;
	uint32_t reg_dds_ack_num;
	uint32_t reg_dac_req_num;
	uint32_t reg_dac_ack_num;
	uint32_t reg_sw_req_num;
	uint32_t reg_sw_ack_num;
	uint32_t reg_reg_req_num;
	uint32_t reg_reg_ack_num;
	uint32_t reg_sum_err_num;

};
typedef CHECK_num_check* CHECK_num_check_PTR;
CHECK_num_check_PTR CHECK_num_check_MODULE = CHECK_num_check_PTR(CHECK_num_check_ADDR);

struct CHECK_miss_check
{
	uint16_t reg_core_status;
};
typedef CHECK_miss_check* CHECK_miss_check_PTR;
CHECK_miss_check_PTR CHECK_miss_check_MODULE = CHECK_miss_check_PTR(CHECK_miss_check_ADDR);


#endif
#endif


typedef SYSTEM* SYSTEM_PTR;
SYSTEM* SYSTEM_MODULE = (SYSTEM_PTR)(SYSTEM_ADDR);
typedef DEVICE_DAC* DEVICE_DAC_PTR;
DEVICE_DAC* DEVCIE_DAC_MODULE = (DEVICE_DAC*)(DEVICE_DAC_ADDR);
typedef DEVICE_trig_cfg* DEVICE_trig_cfg_PTR;
DEVICE_trig_cfg_PTR DEVICE_trig_cfg_MODULE = DEVICE_trig_cfg_PTR(DEVICE_trig_cfg_ADDR);
typedef DEVICE_trig_out* DEVICE_trig_out_PTR;
DEVICE_trig_out_PTR DEVICE_trig_out_MODULE = DEVICE_trig_out_PTR(DEVICE_trig_out_ADDR);
typedef CONTROLLER_loop_mode* CONTROLLER_loop_mode_PTR;
CONTROLLER_loop_mode_PTR CONTROLLER_loop_mode_MODULE = CONTROLLER_loop_mode_PTR(CONTROLLER_loop_mode_ADDR);
typedef CONTROLLER_test_mode* CONTROLLER_test_mode_PTR;
CONTROLLER_test_mode_PTR CONTROLLER_test_mode_MODULE = CONTROLLER_test_mode_PTR(CONTROLLER_test_mode_ADDR);
typedef CONTROLLER_core_ctrl* CONTROLLER_core_ctrl_PTR;
CONTROLLER_core_ctrl_PTR CONTROLLER_core_ctrl_MODULE = CONTROLLER_core_ctrl_PTR(CONTROLLER_core_ctrl_ADDR);



typedef CONTROLLER_step_forward* CONTROLLER_step_forward_PTR;
CONTROLLER_step_forward_PTR CONTROLLER_step_forward_MODULE = CONTROLLER_step_forward_PTR(CONTROLLER_step_forward_ADDR);

typedef CONTROLLER_step_phase* CONTROLLER_step_phase_PTR;
CONTROLLER_step_phase_PTR CONTROLLER_step_phase_MODULE = CONTROLLER_step_phase_PTR(CONTROLLER_step_phase_ADDR);

typedef CONTROLLER_step_base* CONTROLLER_step_base_PTR;
CONTROLLER_step_base_PTR CONTROLLER_step_base_MODULE = CONTROLLER_step_base_PTR(CONTROLLER_step_base_ADDR);




typedef WAVE_wave_list* WAVE_wave_list_PTR;
WAVE_wave_list_PTR WAVE_wave_list_MODULE = WAVE_wave_list_PTR(WAVE_wave_list_ADDR);
typedef WAVE_wave_adjust* WAVE_wave_adjust_PTR;
WAVE_wave_adjust_PTR WAVE_wave_adjust_MODULE = WAVE_wave_adjust_PTR(WAVE_wave_adjust_ADDR);
typedef CHECK_num_check* CHECK_num_check_PTR;
CHECK_num_check_PTR CHECK_num_check_MODULE = CHECK_num_check_PTR(CHECK_num_check_ADDR);
typedef CHECK_miss_check* CHECK_miss_check_PTR;
CHECK_miss_check_PTR CHECK_miss_check_MODULE = CHECK_miss_check_PTR(CHECK_miss_check_ADDR);


/*
void PrintTime() {
   /*
	auto currentTime = std::chrono::system_clock::now();

	// 将时间戳转换为时间结构
	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
	std::tm* localTime = std::localtime(&currentTime_t);

	// 输出时间戳
	std::cout << "当前时间戳: " << currentTime_t << std::endl;

	// 输出格式化的时间
	std::cout << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << std::endl;

	auto currentTime = std::chrono::system_clock::now();

	// 将时间戳转换为时间结构
	std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
	std::tm* localTime = std::localtime(&currentTime_t);

	// 精确到毫秒的时间
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
	/*
	// 输出时间戳
	std::cout << "当前时间戳: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count()
		<< "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;

	// 输出格式化的时间，精确到毫秒
	std::cout << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
		<< "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;
}
*/

int InsPMDL_lightSouce::get_microblaze_version(uint32_t* buffer)
{
	return (int)read_register(FIRSTBOARD, 0x4001, *buffer);
}

int InsPMDL_lightSouce::save_config_in_device(int board_num)//0321
{
	int ret = 0;
	uint16_t buffer[2] = { 0,1 };
	if (board_num == 1)
	{
		ins_write(FIRSTBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
	}
	else if (board_num == 3)
	{
		ins_write(FIRSTBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(SECONDBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(THIRDBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
	}
	else if (board_num == 5)
	{
		int ret = 0;
		ins_write(FIRSTBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(SECONDBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(THIRDBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(FOURTHBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
		ins_write(FIFTHBOARD, 0x4000, buffer);
		if (ret != 0) { return ret; }
	}
	Sleep(1000);
	return if_config_in_device_ok(board_num);
}

int InsPMDL_lightSouce::if_config_in_device_ok(int board_num)//返回-1没读成功，0保存成功，1没保存完，-2板数不对
{   //写完1后，开始保存，当寄存器值恢复为0则保存成功
	uint16_t buffer[2] = { 0 };
	InsResult ret = (InsResult)0;
	if (board_num == 1)
	{
		ret = ins_read(FIRSTBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) {
			return -1;
		}
		else
			return buffer[1];
	}
	else if (board_num == 3)
	{
		ret = ins_read(FIRSTBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(SECONDBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(THIRDBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		return buffer[1];
	}
	else if (board_num == 5)
	{
		ret = ins_read(FIRSTBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(SECONDBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(THIRDBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(FOURTHBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		if (buffer != 0) { return buffer[1]; }
		ret = ins_read(FIFTHBOARD, 0x4000, buffer);
		if (ret != (InsResult)0) { return -1; }
		return buffer[1];
	}
	else { return -2; }//输入的板块数不是1，3，5
}



int InsPMDL_lightSouce::read_extern_laser1(uint16_t* buffer)
{
	return modbus_udp_read_registers(EXTSTM32, 10, 1, buffer);
}

int InsPMDL_lightSouce::read_extern_laser2(uint16_t* buffer)
{
	return modbus_udp_read_registers(EXTSTM32, 11, 1, buffer);
}

int InsPMDL_lightSouce::write_extern_laser1(uint16_t* buffer)
{
	return modbus_udp_write_single_register(EXTSTM32, 10, buffer);
}

int InsPMDL_lightSouce::write_extern_laser2(uint16_t* buffer)
{
	return modbus_udp_write_single_register(EXTSTM32, 11, buffer);
}

int InsPMDL_lightSouce::read_extern_pirch(double* buffer)
{
	//读到的角度需要除以十
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(EXTSTM32, 12, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::read_extern_roll(double* buffer)
{
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(EXTSTM32, 13, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::read_extern_yaw(double* buffer)
{
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(EXTSTM32, 14, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::read_internl_pirch(double* buffer)
{
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(INTSTM32, 10, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::read_internl_roll(double* buffer)
{
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(INTSTM32, 11, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::read_internl_yaw(double* buffer)
{
	int ret = 0;
	uint16_t temp = 0;
	short temp_short = 0;
	modbus_udp_read_registers(INTSTM32, 12, 1, &temp);
	if (ret == 0)
	{
		*buffer = (static_cast<short>(temp)) / -10.0;
	}
	return ret;
}
int InsPMDL_lightSouce::save_camera_height(uint32_t* buffer)
{
	int ret = 0;
	int num = 2;
	uint16_t temp = 1;
	uint16_t buffer_temp[2] = {};
	buffer_temp[0] = static_cast<uint16_t>(*buffer >> 16);
	buffer_temp[1] = static_cast<uint16_t>(*buffer & 0xFFFF);
	modbus_udp_write_registers(INTSTM32, 1, 2, buffer_temp);
	modbus_udp_write_registers(INTSTM32, 0, 1, &temp);
	while (temp != 0 && num > 0) {
		Sleep(100);
		num--;
		modbus_udp_read_registers(INTSTM32, 0, 1, &temp);
	}

	if (temp == 0)
	{
		return 0;
	}
	return -1;
}
int InsPMDL_lightSouce::read_camera_height(uint32_t* buffer)
{
	uint16_t buffer_temp[2] = {};
	int ret = 0;
	modbus_udp_read_registers(INTSTM32, 1, 2, buffer_temp);
	*buffer = (static_cast<uint32_t>(buffer_temp[0]) << 16) | buffer_temp[1];
	return ret;
}

//0523----------------------
int InsPMDL_lightSouce::save_camera_heightTemp(uint32_t* buffer)
{
	int ret = 0;
	int num = 2;
	uint16_t temp = 1;
	uint16_t buffer_temp[2] = {};
	buffer_temp[0] = static_cast<uint16_t>(*buffer >> 16);
	buffer_temp[1] = static_cast<uint16_t>(*buffer & 0xFFFF);
	modbus_udp_write_registers(INTSTM32, 3, 2, buffer_temp); // 1->3
	modbus_udp_write_registers(INTSTM32, 0, 1, &temp);
	while (temp != 0 && num > 0) {
		Sleep(100);
		num--;
		modbus_udp_read_registers(INTSTM32, 0, 1, &temp);
	}

	if (temp == 0)
	{
		return 0;
	}
	return -1;
}
int InsPMDL_lightSouce::read_camera_heightTemp(uint32_t* buffer)
{
	uint16_t buffer_temp[2] = {};
	int ret = 0;
	modbus_udp_read_registers(INTSTM32, 3, 2, buffer_temp);
	*buffer = (static_cast<uint32_t>(buffer_temp[0]) << 16) | buffer_temp[1];
	return ret;
}

/*测试用的
int ins_write_reg_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, 1, *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, 1, *buffer);
}
*/

#if 1

//system
InsResult InsPMDL_lightSouce::ins_write_reg_soft_reset(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (SYSTEM_MODULE->reg_soft_reset), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_test(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (SYSTEM_MODULE->reg_test), *buffer);
}


//DAC
InsResult InsPMDL_lightSouce::ins_read_reg_mos_ack_time(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_ack_time(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_read_reg_dds_ack_time(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dds_ack_time(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_read_reg_dac_ack_time(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_ack_time(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_ack_time(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_ack_time(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), *buffer);
}

//temp
/*
int ins_write_reg_mainboard_temp(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(DEVICE_temp_ADDR), *buffer);
}
*/

//num_check
InsResult InsPMDL_lightSouce::ins_write_reg_num_check_clr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_num_check_clr), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_io_in_0_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_0_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_io_in_1_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_1_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_a_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_a_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_encoder_b_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_b_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_io_out_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_out_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_mos_req_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_req_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_mos_ack_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_ack_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_dds_req_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_dds_ack_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_ack_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_dac_req_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_req_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_dac_ack_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_sw_req_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_req_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_sw_ack_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_reg_req_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_req_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_reg_ack_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_ack_num), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_sum_err_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sum_err_num), *buffer);
}

//miss_check
InsResult InsPMDL_lightSouce::ins_write_reg_core_status(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(CHECK_miss_check_MODULE->reg_core_status), *buffer);
}








//READ
// 
//system

InsResult InsPMDL_lightSouce::ins_read_COMPANY(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->COMPANY), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_DEVICE(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->DEVICE), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_DEVICE_SUB(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->DEVICE_SUB), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_INTERFACETYPE(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->INTERFACETYPE), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_YEAR(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->YEAR), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_HOUR(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->HOUR), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_REV(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->REV), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_soft_reset(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_soft_reset), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_test(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_test), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_device_temp(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_device_temp), *buffer);
}


// phy
//int ins_write_reg_phy_rst(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t)(DEVICE_PHY_ADDR), *buffer);
//}


/*
//MPU0
int ins_write_reg_iic_whrl(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), *buffer);
}
int ins_write_reg_iic_dev_addr(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), *buffer);
}
int ins_write_reg_iic_addr(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), *buffer);
}
int ins_write_reg_iic_wdata(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), *buffer);
}
//int ins_write_reg_iic_rdata(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), *buffer);
//}
int ins_write_reg_iic_req(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), *buffer);
}
//int ins_write_reg_iic_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), *buffer);
//}
*/
//laser
InsResult InsPMDL_lightSouce::ins_write_reg_laser_status(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t)(DEVICE_laser_ADDR), *buffer);
}


////mpu1
//int ins_write_reg_mcu1_angle_x(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), *buffer);
//}
//int ins_write_reg_mcu1_angle_y(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), *buffer);
//}
//int ins_write_reg_mcu1_angle_z(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), *buffer);
//}
//int ins_write_reg_mcu1_temp(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), *buffer);
//}

/*
int ins_write_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), *buffer);
}
int ins_write_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), *buffer);
}
int ins_write_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), *buffer);
}
//int ins_write_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), *buffer);
//}
int ins_write_reg_mcu1_bps(BoardID id,uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), *buffer);
}
*/
//trig_cfg
InsResult InsPMDL_lightSouce::ins_write_reg_soft_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_soft_trigger_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_soft_trigger_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_width(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_delay(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_pulse(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_polar(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_phase(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_cnt_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_dis_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_ignore(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_div(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_width(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), *buffer);
}
//int ins_write_reg_encoder_location(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), *buffer);
//}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_multi_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_encoder_multi_coe(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), *buffer);
}
//int ins_write_reg_encoder_a_cnt(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), *buffer);
//}
//int ins_write_reg_encoder_b_cnt(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), *buffer);
//}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_clr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_encoder_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_slave_device(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_status_cnt_clr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), *buffer);
}
//1123新增


InsResult InsPMDL_lightSouce::ins_write_reg_l1_soft_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_soft_trigger_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_soft_trigger_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_width(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_width), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_delay(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_delay), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_pulse(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_pulse), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_polar(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_polar), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_trigger_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_phase(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_phase), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_cnt_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_cnt_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_dis_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_dis_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_ignore(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_ignore), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_div(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_div), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_width(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_width), *buffer);
}
/*
InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_location(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_location), *buffer);
}
*/
InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_multi_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_multi_coe(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_coe), *buffer);
}
/*
InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_a_cnt), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_b_cnt), *buffer);
}
*/
InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_clr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_clr), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_encoder_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_l1_status_cnt_clr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_status_cnt_clr), *buffer);
}

//写死
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_level(BoardID id, uint32_t* buffer)
{
	return write_register(id, 2942, *buffer);
}



////trig_out
InsResult InsPMDL_lightSouce::ins_write_reg_exp_chan(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_exposure_time(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_cnt_max(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_multi_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_polar(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_start_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_led_pwm_end_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_start_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_end_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trig_out_polar(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), *buffer);
}
//loopmode
InsResult InsPMDL_lightSouce::ins_write_reg_dds_phase(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dds_inc(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_exp_cycle(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_gap(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_pic_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_status(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_shift(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_loop_gap(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_loop_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dds_phase_offset(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), *buffer);
}

//11.23
InsResult InsPMDL_lightSouce::ins_write_reg_dds_direction_x(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_dds_direction_y(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_current_offset(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_current_offset), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_forward(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_forward), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_backward(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_backward), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_sw_wait(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_wait), *buffer);
}
	
InsResult InsPMDL_lightSouce::ins_write_reg_camera_delay(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_camera_cycle(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_dac_req(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_req), *buffer);
}
//写死地址
InsResult InsPMDL_lightSouce::ins_write_reg_dds_phase_y(BoardID id, uint32_t * buffer)
{
	return write_register(id, 4152, *buffer);
}


//test
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_x0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_x0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_x0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_x0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_x1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_x1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_x1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_x1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_x2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_x2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_x2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_x2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_x3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_x3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_x3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_x3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_y0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_y0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_y0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_y0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_y1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_y1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_y1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_y1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_y2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_y2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_y2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_y2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_dac_value_y3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_sw_value_y3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_mos_value_y3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_trigger_req_y3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), *buffer);
}
//core
InsResult InsPMDL_lightSouce::ins_write_reg_core_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_core_mode(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), *buffer);
}


//step forward
InsResult InsPMDL_lightSouce::ins_write_reg_step_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_pic_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_pic_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_x_seq(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_x_seq), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_y_seq(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_y_seq), *buffer);
}
//step phase write
InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_phase_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_31), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_inc_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_31), *buffer);
}


//step base read
InsResult InsPMDL_lightSouce::ins_read_reg_step_base_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_25(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_base_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_31), *buffer);
}

//step base write
InsResult InsPMDL_lightSouce::ins_write_reg_step_base_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_write_reg_step_base_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_31), *buffer);
}


//wave_list
InsResult InsPMDL_lightSouce::ins_write_reg_ram_cfg_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_ram_whrl(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_ram_addr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), *buffer);
}
//wave
InsResult InsPMDL_lightSouce::ins_write_reg_ram_wdata(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), *buffer);
}
//InsResult InsPMDL_lightSouce::ins_write_reg_ram_rdata(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), *buffer);
//}
InsResult InsPMDL_lightSouce::ins_write_reg_ram_req(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), *buffer);
}
//InsResult InsPMDL_lightSouce::ins_write_reg_ram_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), *buffer);
//}
InsResult InsPMDL_lightSouce::ins_write_reg_wave_start_addr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_wave_end_addr(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), *buffer);
}
//adjust
InsResult InsPMDL_lightSouce::ins_write_reg_adjust_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_adjust_gain(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), *buffer);
}
InsResult InsPMDL_lightSouce::ins_write_reg_adjust_offset(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), *buffer);
}




//READ
// phy
//InsResult InsPMDL_lightSouce::ins_read_reg_phy_rst(BoardID id,uint32_t* buffer)
//{
//    return read_register(id, (uint16_t)(DEVICE_PHY_ADDR), *buffer);
//}

//temp
InsResult InsPMDL_lightSouce::ins_read_reg_mainboard_temp(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(DEVICE_temp_ADDR), *buffer);
}
/*
//MPU0
InsResult InsPMDL_lightSouce::ins_read_reg_iic_whrl(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_dev_addr(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_addr(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_wdata(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_rdata(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_req(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_iic_done(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), *buffer);
}
*/
InsResult InsPMDL_lightSouce::ins_read_reg_laser_status(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(DEVICE_laser_ADDR), *buffer);
}
/*
////mpu1
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_angle_x(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_angle_y(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_angle_z(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_temp(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mcu1_bps(BoardID id,uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), *buffer);
}

*/

//trig_cfg
InsResult InsPMDL_lightSouce::ins_read_reg_soft_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_soft_trigger_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_soft_trigger_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_width(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_delay(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_pulse(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_polar(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_phase(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_cnt_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_dis_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_ignore(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_div(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_width(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_location(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_multi_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_multi_coe(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_a_cnt(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_b_cnt(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_clr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_slave_device(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_status_cnt_clr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), *buffer);
}
//1123新增

InsResult InsPMDL_lightSouce::ins_read_reg_l1_soft_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_soft_trigger_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_soft_trigger_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_cycle(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_width(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_width), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_delay(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_delay), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_pulse(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_pulse), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_polar(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_polar), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_trigger_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_phase(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_phase), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_cnt_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_cnt_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_dis_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_dis_mode), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_ignore(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_ignore), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_div(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_div), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_width(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_width), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_location(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_location), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_multi_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_multi_coe(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_coe), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_a_cnt(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_a_cnt), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_b_cnt(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_b_cnt), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_clr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_clr), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_encoder_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_l1_status_cnt_clr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_status_cnt_clr), *buffer);
}

//写死
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_level(BoardID id, uint32_t* buffer)
{
	return read_register(id, 2942, *buffer);
}



////trig_out
InsResult InsPMDL_lightSouce::ins_read_reg_exp_chan(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_exposure_time(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_cnt_max(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_multi_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_polar(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_start_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_led_pwm_end_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_start_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_end_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trig_out_polar(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), *buffer);
}



//loopmode
InsResult InsPMDL_lightSouce::ins_read_reg_dds_phase(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dds_inc(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_exp_cycle(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_gap(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_pic_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_status(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_shift(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_loop_gap(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_loop_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dds_phase_offset(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), *buffer);
}

//11.23
InsResult InsPMDL_lightSouce::ins_read_reg_dds_direction_x(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dds_direction_y(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_current_offset(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_current_offset), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_forward(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_forward), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_backward(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_backward), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_sw_wait(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_wait), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_camera_delay(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_camera_cycle(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dac_req(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_req), *buffer);
}
//写死地址
InsResult InsPMDL_lightSouce::ins_read_reg_dds_phase_y(BoardID id, uint32_t * buffer)
{
	return read_register(id, 4152, *buffer);
}



//test
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_x0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_x0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_x0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_x0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_x1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_x1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_x1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_x1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_x2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_x2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_x2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_x2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_x3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_x3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_x3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_x3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_y0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_y0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_y0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_y0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_y1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_y1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_y1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_y1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_y2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_y2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_y2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_y2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_dac_value_y3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_sw_value_y3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_mos_value_y3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_trigger_req_y3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), *buffer);
}
//core
InsResult InsPMDL_lightSouce::ins_read_reg_core_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_core_mode(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), *buffer);
}
//step forward
InsResult InsPMDL_lightSouce::ins_read_reg_step_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_en), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_pic_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_pic_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_x_seq(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_x_seq), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_y_seq(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_y_seq), *buffer);
}
//step phase
InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_25(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_phase_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_31), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_0), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_1), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_2), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_3), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_4), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_5), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_6), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_7), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_8), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_9), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_10), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_11), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_12), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_13), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_14), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_15), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_16), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_17), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_18), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_19), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_20), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_21), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_22), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_23), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_24), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_25(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_25), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_26), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_27), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_28), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_29), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_30), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_step_inc_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_31), *buffer);
}





//wave list
InsResult InsPMDL_lightSouce::ins_read_reg_ram_cfg_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_ram_whrl(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_ram_addr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), *buffer);
}
//wave
InsResult InsPMDL_lightSouce::ins_read_reg_ram_wdata(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_ram_rdata(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_ram_req(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_ram_done(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_wave_start_addr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_wave_end_addr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), *buffer);
}
//adjust
InsResult InsPMDL_lightSouce::ins_read_reg_adjust_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_adjust_gain(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_adjust_offset(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), *buffer);
}


//num_check
InsResult InsPMDL_lightSouce::ins_read_reg_num_check_clr(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_num_check_clr), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_io_in_0_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_0_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_io_in_1_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_1_num), *buffer);
}
InsResult InsPMDL_lightSouce::ins_read_reg_encoder_a_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_a_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_encoder_b_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_b_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_io_out_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_out_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_mos_req_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_mos_ack_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dds_req_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dds_ack_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dac_req_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_dac_ack_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_sw_req_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_sw_ack_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_reg_req_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_req_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_reg_ack_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_ack_num), *buffer);
}

InsResult InsPMDL_lightSouce::ins_read_reg_sum_err_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sum_err_num), *buffer);
}

//miss_check
InsResult InsPMDL_lightSouce::ins_read_reg_core_status(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t)(CHECK_miss_check_MODULE->reg_core_status), *buffer);
}

#endif


static const char* user_u32toStr(uint32_t aInt)
{
	static char HEXSEQ[] = "0123456789abcdef";
	static string lRegAddrStr;
	lRegAddrStr.clear();
	for (int8_t i = 7; i >= 0; i--)
	{
		int8_t tChar = (aInt >> (4 * i)) & 0xf;
		lRegAddrStr.push_back(HEXSEQ[tChar]);
	}
	return lRegAddrStr.c_str();
}



//ch传001,boardcount板子id, writeFile要写入的文件，buffer存放读出来的数据，addr为寄存器地址,读不上来就再读三次
int InsPMDL_lightSouce::write_file(const char* ch, int boardcount, std::ofstream* writeFile, uint16_t* buffer, uint16_t addr)
{
	int y = 0;
	int flag = 0;
	int ret = 0;
	uint32_t buffer_u32 = 0;
	for (int id = 1; id < boardcount + 1; id++) {
		for (; y < 3; y++)
		{
			ret = (int)ins_read((BoardID)id, addr, buffer);
			if (ret == 0)
			{
				break;
			}
			if ((ret != 0) && (y != 2))
				continue;
			else if (y == 2)
			{
				flag++;
				buffer[0] = 0xffff;
				buffer[1] = 0xffff;
			}
		}
		buffer_u32 = ((uint32_t)buffer[0] << 16) | (uint32_t)buffer[1];
		//cout << "buffer[0]" << buffer[0]<<endl;
		//cout << "buffer[1]" << buffer[1] << endl;
		//cout << "buffer_u32" << buffer_u32 << endl;
		*writeFile << "_S_" << ch << "_" << id << "_" << user_u32toStr(buffer_u32) << "_E_;\n";

	}
	return flag;
}

//写寄存器值，格式为寄存器名称、地址、板选号、写入值
int InsPMDL_lightSouce::save_config_file(int boardcount, const char* _config_filename)
{
	if (!((boardcount == 1) || (boardcount == 3) || (boardcount == 5)))
		return -1;
	uint16_t buffer[2] = { 0 };

	string endname = ".icf";
	string temp(_config_filename);
	int found = temp.find(endname);
	if (found == string::npos) return -1;//文件地址错误
	//以写模式打开文件
	//ofstream outfile;
	//out表示只写，trunc表示截断，当不存在文件将新建文件，当存在则清空重新写
	//ofstream writeFile(_config_filename, std::ios::out | std::ios::trunc);
	//ofstream writeFile(_config_filename, std::ios::out | std::ios::app);

	const int startingLine = 15;//FPGA寄存器开始写入行数

	// 读取前八行
	std::ifstream inFile(_config_filename);
	if (!inFile.is_open()) {
		std::cerr << "无法打开文件进行读取：" << _config_filename << std::endl;
		return -2;
	}

	std::vector<std::string> lines;
	std::string line;
	for (int i = 0; i < startingLine - 1 && std::getline(inFile, line); ++i) {
		lines.push_back(line);
	}

	inFile.close();

	// 打开文件并设置为在文件末尾追加
	std::ofstream writeFile(_config_filename, std::ios::out | std::ios::trunc);

	if (!writeFile.is_open()) {
		std::cerr << "无法打开文件进行写入：" << _config_filename << std::endl;
		return -2;
	}

	// 写入前八行
	for (const auto& savedLine : lines) {
		writeFile << savedLine << '\n';
	}

	cout << "start save registers configure file" << endl;
	string start_str = "--->INS_2D5_Registers_Configure_File:";
	//向文件写入用户输入的数据
	writeFile << start_str << endl;
	int flag = 0;
	{
		int ret = 0;
		if (0 != write_file("001", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("002", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("003", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("004", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		//用slave测试C#
		//writeFile.close();
		//return 0;
		if (0 != write_file("005", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("006", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("007", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("008", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("009", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("010", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("011", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("012", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("013", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("015", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("014", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("016", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("017", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("018", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("019", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("020", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("021", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("022", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("023", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("024", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("025", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("026", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("027", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("028", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("029", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("030", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("031", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("032", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("033", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("034", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("035", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("036", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("037", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("038", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("039", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("040", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("041", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("042", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("043", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("044", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("045", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("046", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("047", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("048", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("049", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("050", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("051", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("052", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("053", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("054", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("055", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("056", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("057", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("058", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("059", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("060", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("061", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("062", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("063", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("064", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("065", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("066", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("067", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("068", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("069", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("070", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("071", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("072", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("073", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("074", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("075", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("076", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("077", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("078", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("079", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("080", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("081", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
		if (0 != write_file("082", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset))) {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}

	}
	//关闭打开的文件
	writeFile.close();
	return 0;

}




int InsPMDL_lightSouce::LoadRegisters(BoardID id, uint16_t addr, uint16_t* buffer)
{
	int ret = 0;
	int i = 0;
	while (i++ < 3)
	{
		ret = (int)ins_write(id, addr, buffer);
		if (ret == 0) { break; }
	}
	return ret;
}

int InsPMDL_lightSouce::load_config_file(int _board_num, const char* _config_filename)
{

	PRINTTIME("start load");
	int flag = 0;
	if (!((_board_num == 1) || (_board_num == 3) || (_board_num == 5)))
		return -3;

	int ret = 0;//返回值
	if (_config_filename)
	{
		const char* fileExtension = strrchr(_config_filename, '.'); // 获取最后一个点的位置
		if (!(fileExtension && strcmp(fileExtension, ".icf") == 0)) 
		{
			std::cout << "读取失败;" << endl;
			return -1;  // 文件后缀bu是 ".icf"
		}
	}
	ifstream readFile(_config_filename, std::ios::in);
	if (!readFile.is_open())
	{
		std::cout << "打开失败;" << endl;
		return -1;
	}
	string l_readstr;
	string l_tmpstr;
	uint32_t l_writeNum;
	uint32_t l_writeBoard;

	char* pEndTemp;
	uint32_t l_writeBuffer;
	l_readstr.clear();

	int start_line_num = 0;//出现"--->INS_2D5_Registers_Configure_File:"的行数
	while (true)
	{
		start_line_num++;
		std::getline(readFile, l_readstr);
		if (strcmp(l_readstr.c_str(), "--->INS_2D5_Registers_Configure_File:") == 0)
		{
			std::cout << l_readstr << endl;
			break;
		}
	}
	// 除了"--->INS_2D5_Registers_Configure_File:"这一行，获取后面文件行数
	int lineCount = 0;
	std::string line;
	while (std::getline(readFile, line)) {
		++lineCount;
	}
	if (lineCount != _board_num * 82) {  //寄存器数量变化要改此处
		return -2;
	}
	// 将文件指针返回到开头
	readFile.clear();  // 清除文件状态
	readFile.seekg(0, std::ios::beg);
	while (true)
	{
		std::getline(readFile, l_readstr);
		if (strcmp(l_readstr.c_str(), "--->INS_2D5_Registers_Configure_File:") == 0)
		{
			std::cout << l_readstr << endl;
			break;
		}
	}

	int cycle_index = 0;//循环次数

	while (true)
	{
		//PRINTTIME("循环开始");
		cycle_index++;
		//std::cout <<dec<< cycle_index << std::endl;
		if (readFile.eof()) {
			// Code to handle reaching the end of the file
			break;
		}
		std::getline(readFile, l_readstr);
		if (l_readstr == "")
		{
			if (cycle_index == lineCount + 1) { return 0; }
			return -1;//lRtVal = INS_Result::INS_FILE_OPEN_ERR;
		}
		l_tmpstr = l_readstr.substr(0, 3);//从0开始读3个
		if (l_tmpstr != "_S_")
		{
			continue;
		}
		l_tmpstr = l_readstr.substr(3, 3);
		l_writeNum = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 10);
		l_tmpstr = l_readstr.substr(7, 1);
		l_writeBoard = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 10);
		if (!((l_writeBoard == 1) || (l_writeBoard == 2) || (l_writeBoard == 3) || (l_writeBoard == 4) || (l_writeBoard == 5)))
			return -1;
		l_tmpstr = l_readstr.substr(9, 8);
		l_writeBuffer = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 16);
		l_writeBuffer = (l_writeBuffer << 16) | (l_writeBuffer >> 16);
		//PrintTime("开始写寄存器");
		switch (l_writeNum) {
		case 1:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 2:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 3:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 4:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 5:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 6:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 7:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 8:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 9:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 10:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 11:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 12:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 13:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 14:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 15:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 16:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 17:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 18:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 19:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 20:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 21:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 22:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 23:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 24:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 25:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 26:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 27:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 28:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 29:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 30:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 31:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 32:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 33:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 34:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 35:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 36:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 37:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 38:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 39:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 40:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 41:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 42:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 43:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 44:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 45:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 46:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 47:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 48:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 49:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 50:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 51:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 52:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 53:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 54:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 55:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 56:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 57:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 58:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 59:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 60:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 61:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 62:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 63:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 64:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 65:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 66:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 67:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 68:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 69:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 70:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 71:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 72:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 73:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 74:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 75:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 76:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 77:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 78:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 79:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 80:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 81:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		case 82:
			//std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), (uint16_t*)(&l_writeBuffer)) != 0) {
				readFile.close();
				cout << ++flag << endl;//return -1;
			}
			break;
		default:
			cout << "无对应序号";
		}
	}
	l_writeBuffer = 1;
	for (l_writeBoard = 0; l_writeBoard < _board_num; l_writeBoard++)
	{
		if (LoadRegisters((BoardID)l_writeBoard, 0x1024, (uint16_t*)(&l_writeBuffer)) != 0)
		{
			readFile.close();
			cout << ++flag << endl;//return -1;
		}
	}
	readFile.close();
	PRINTTIME("end");
	if (flag != 0)
	{
		return flag;
	}
	return 0;
}


















//----------面阵需求------------ //0304函数实现
int InsPMDL_lightSouce::Ins_SetIlluminationTime(uint32_t _index, double _time)
{
	uint32_t index = _index;

	double LightTime = _time;

	uint32_t ReadNum = 0;
	readFromModbusOne(0x0C0A, ReadNum);

	double CameraTriggerCycle2 = ((LightTime * 1000) - 1) / ReadNum;
	uint32_t SwLoopNumVal = CameraTriggerCycle2 / 4;//4us

	uint32_t MosLat = 3;
	uint32_t DdsLat = 3;
	double SwLat = 2;
	uint32_t IdleLat = 1;
	uint32_t TrigLat = 1;

	uint16_t REGMOSACK[2] = {};
	uint16_t REGDDSACK[2] = {};
	uint16_t REGPICNUM[2] = {};

	uint16_t REGSWGAP[2] = {};
	uint16_t REGTRIGGAP[2] = {};
	uint16_t REGSWLOOPNUM[2] = {};
	uint16_t REGCAMTRINUM[2] = {};

	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0880, REGMOSACK);//1100
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0882, REGDDSACK);//1104
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1008, REGPICNUM);//2010

	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x100E, REGSWGAP);//201c
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1006, REGTRIGGAP);//200c
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1010, REGSWLOOPNUM);//2020
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0C0A, REGCAMTRINUM);//1814

	uint32_t MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) << 16) | REGMOSACK[1];
	uint32_t DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) << 16) | REGDDSACK[1];
	uint32_t PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) << 16) | REGPICNUM[1];

	uint32_t SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) << 16) | REGSWGAP[1];
	uint32_t TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) << 16) | REGTRIGGAP[1];
	uint32_t SWLoopNumZ = (static_cast<uint32_t>(REGSWLOOPNUM[0]) << 16) | REGSWLOOPNUM[1];
	uint32_t CamTriNumZ = (static_cast<uint32_t>(REGCAMTRINUM[0]) << 16) | REGCAMTRINUM[1];

	uint32_t MosTrue = MOSACKZ + MosLat;
	uint32_t DdsTrue = DDSACKZ + DdsLat;

	double Texpend1 = MosTrue + DdsTrue + IdleLat + MosTrue;
	double Texpend2 = DdsTrue + TrigLat;

	double Tencode = ((CameraTriggerCycle2)*PicNumZ) * (1000 / 8); //PicNumZ是取相数量
	double TpicLoop = (Tencode - Texpend1) / PicNumZ;

	uint32_t ExpLoopVal = TpicLoop - TriigerGapZ - Texpend2;
	double SwTrue = (ExpLoopVal / SwLoopNumVal) - PicNumZ - SWGAPZ;
	uint32_t SwAckVal = SwTrue - SwLat;
	uint32_t CameraCycleVal = (CameraTriggerCycle2) * (1000 / 8); //1207


	uint32_t valueToWrite = ExpLoopVal;
	write_fpga_reg(index, 0x2008, valueToWrite);
	//writeToModbusOne(0x1004, valueToWrite);

	uint32_t valueToWrite2 = SwAckVal;
	//writeToModbusOne(0x0886, valueToWrite2);
	write_fpga_reg(index, 0x110C, valueToWrite2);

	uint32_t valueToWrite3 = CameraCycleVal;
	//writeToModbusOne(0x1022, valueToWrite3);
	write_fpga_reg(index, 0x2044, valueToWrite3);

	//writeToModbusOne(0x1010, SwLoopNumVal);
	write_fpga_reg(index, 0x2020, SwLoopNumVal);

	write_fpga_reg(index, 0x1600, LightTime * 1000000 / 8);
	//writeToModbusOne(0x0B00, valueToWrite);

	return 0;
}

//0极大,1大，2标准，3小，4极小
int InsPMDL_lightSouce::Ins_SetLightMode(uint32_t _index, int index)
{
	uint32_t modeValues[] = { 3, 6, 12, 18, 24 }; //发光模式值

	uint32_t currentValue;

	currentValue = modeValues[index];

	uint16_t highValue = (currentValue >> 16) & 0xFFFF;
	uint16_t lowValue = currentValue & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };

	ins_write((BoardID)_index, 0x1240, buffer);//2480
	ins_write((BoardID)_index, 0x1242, buffer);//+4
	ins_write((BoardID)_index, 0x1244, buffer);//+4
	ins_write((BoardID)_index, 0x1246, buffer);//+4
	ins_write((BoardID)_index, 0x1248, buffer);//+4
	ins_write((BoardID)_index, 0x124A, buffer);//+4
	ins_write((BoardID)_index, 0x124C, buffer);//+4
	ins_write((BoardID)_index, 0x124E, buffer);//+4
	return 0;
}
//自由取像开：  
int InsPMDL_lightSouce::Ins_SetFreeRunOn(uint32_t index)
{

	double CameraTriggerCycle = 30;
	uint32_t SwLoopNumVal = CameraTriggerCycle / 4;
	uint32_t MosLat = 3;
	uint32_t DdsLat = 3;
	double SwLat = 2;
	uint32_t IdleLat = 1;
	uint32_t TrigLat = 1;
	uint32_t MosTrue = 2 + MosLat;
	uint32_t DdsTrue = 2 + DdsLat;

	double Texpend1 = MosTrue + DdsTrue + IdleLat + MosTrue;
	double Texpend2 = DdsTrue + TrigLat;
	double Tencode = (CameraTriggerCycle * 1000) * 8 / 8; //PicNumZ是取相数量
	double TpicLoop = (Tencode - Texpend1) / 8;
	uint32_t ExpLoopVal = TpicLoop - 250 - Texpend2;
	double SwTrue = (ExpLoopVal / SwLoopNumVal) - 1 - 9;
	uint32_t SwAckVal = SwTrue - SwLat;
	uint32_t CameraCycleVal = (CameraTriggerCycle * 1000) / 8; //1207


	write_fpga_reg(index, 0x2008, ExpLoopVal);

	write_fpga_reg(index, 0x110C, SwAckVal);

	write_fpga_reg(index, 0x2044, CameraCycleVal);

	write_fpga_reg(index, 0x2020, SwLoopNumVal);

	write_fpga_reg(index, 0x1600, 138);


	//-------------------------------

	//writeToModbusOne(0x0B04, 0);//1608->0//开始时先关一次
	//writeToModbusOne(0x1180, 0);//2300->0//开始时先关一次
	write_fpga_reg(index, 0x1608, 0);
	write_fpga_reg(index, 0x2300, 0);

	//writeToModbusOne(0x0B02, 0);//1604->0 一直拍
	//writeToModbusOne(0x0888, 255);
	write_fpga_reg(index, 0x1604, 0);
	write_fpga_reg(index, 0x1110, 255);

	uint32_t SoftOn = 1;
	//writeToModbusOne(0x1182, 1);//2304->1
	//writeToModbusOne(0x1280, 512);//2500->512
	//writeToModbusOne(0x1200, 144);//2400->144
	//writeToModbusOne(0x1180, SoftOn);//2300->1
	//writeToModbusOne(0x0B04, SoftOn);//1608->1

	write_fpga_reg(index, 0x2480, 0);
	write_fpga_reg(index, 0x2304, 1);
	write_fpga_reg(index, 0x2500, 512);
	write_fpga_reg(index, 0x2400, 144);
	write_fpga_reg(index, 0x2300, SoftOn);
	write_fpga_reg(index, 0x1608, SoftOn);

	return 0;
}
//自由取像关：  
int InsPMDL_lightSouce::Ins_SetFreeRunOff(uint32_t _index)
{
	uint32_t SoftOff = 0;
	write_fpga_reg(_index, 0x1608, SoftOff);
	write_fpga_reg(_index, 0x2300, SoftOff);

	return 0;
}
//触发（8张）开：

int InsPMDL_lightSouce::Ins_Trigger_8_ImagesOn(int BoardNum, uint32_t _time, uint32_t delaytime)
{
	for (int index = 1; index <= BoardNum; index++)//111
	{

		write_fpga_reg(index, 0x110C, 487);//110C -> 0x 1e7  //sssss
		write_fpga_reg(index, 0x2008, 1249728);//2008 -> 0x 1311c0
		write_fpga_reg(index, 0x2020, 2500);//2020 -> 9c4

		write_fpga_reg(index, 0x1608, 0);
		write_fpga_reg(index, 0x2300, 0);

		write_fpga_reg(index, 0x1604, 8);

		//write_fpga_reg(index, 0x2500, 0);
		//write_fpga_reg(index, 0x2400, 0);
		write_fpga_reg(index, 0x1110, 255);
		write_fpga_reg(index, 0x2304, 8);
		write_fpga_reg(index, 0x2300, 1);
		//------------------------------------
		write_fpga_reg(index, 0x2040, delaytime * 1000000 / 8);//
	}
	write_fpga_reg(1, 0x2044, _time * 1000000 / 8);//2044 -> 0x 1312d0 //10ms
	write_fpga_reg(1, 0x1600, _time * 1000000 / 8);//1600 -> 0x 1312d0
	write_fpga_reg(1, 0x1608, 1);//开

	return 0;
}
//触发（8张）关：
int InsPMDL_lightSouce::Ins_Trigger_8_ImagesOff(int BoardNum)
{
	write_fpga_reg(FIRSTBOARD, 0x1608, 0);
	for (int index = 1; index <= BoardNum; index++)
	{
		write_fpga_reg(index, 0x2300, 0);
	}
	return 0;
}

//===0304

int InsPMDL_lightSouce::write_fpga_reg(int32_t index, uint32_t _addr, uint32_t _value)
{


	// Modbus协议需要将待写入地址除2
	uint32_t HalfAddr = _addr / 2;

	// 格式隐式转换 保持写入格式一致
	uint16_t Addr = HalfAddr;

	uint16_t highValue = (_value >> 16) & 0xFFFF;
	uint16_t lowValue = _value & 0xFFFF;
	uint16_t buffer[2] = { highValue,lowValue };

	ins_write((BoardID)index, Addr, buffer);
	return 0;


}

// 设置面阵光源IO触发周期
int InsPMDL_lightSouce::Ins_SetIOTriggerCycle(uint32_t _index, double _time_ms)
{
	double IOTriggerCycle = _time_ms;

	// 毫秒转FPGA拍数
	double IOTriggerCycleBeat = (IOTriggerCycle * 1000 * 1000) / 8;

	// 格式隐式转换 保持写入格式一致
	uint32_t IOTrigCycle = IOTriggerCycleBeat;

	// 写入
	// writeToModbusOne(0x806, IOTrigCycle); //160C
	write_fpga_reg(_index, 0x160C, IOTrigCycle);

	return 0;
}

// 设置面阵光源IO触发数量
int InsPMDL_lightSouce::Ins_SetIOTriggerNum(uint32_t _index, int _num)
{
	int IOTriggerNum = _num;

	// 格式隐式转换 保持写入格式一致
	uint32_t IOTrigNum = IOTriggerNum;

	// 写入
	// writeToModbusOne(REGAddrIOTriggerNum, IOTrigCycle); //1610
	write_fpga_reg(_index, 0x1610, IOTrigNum); //1610

	return 0;

}

// 设置面阵光源IO触发使能
int InsPMDL_lightSouce::Ins_SetIOTriggerEn(uint32_t _index, bool _enable)
{

	bool IOTriggerEn = _enable;

	// 格式隐式转换 保持写入格式一致
	uint32_t IOTrigEn = IOTriggerEn;

	// 写入
	// writeToModbusOne(REGAddrIOTriggerEn, IOTrigEn); //1628
	write_fpga_reg(_index, 0x1628, IOTrigEn); //1628

	return 0;

}

int InsPMDL_lightSouce::Ins_SetBrightness(uint32_t _index, uint32_t _percentage)
{
	uint32_t BrightNess = _percentage;
	if (BrightNess >= 1 && BrightNess <= 100)
	{
		uint32_t BrightNessToReg = 256 + 79 * BrightNess - 1;
		uint32_t BrightNessOne = 1;

		write_fpga_reg(_index, 0x2034, BrightNessToReg);
		write_fpga_reg(_index, 0x2038, BrightNessToReg);
		write_fpga_reg(_index, 0x2048, BrightNessOne);

		return 0;
		//addToStateMessage("亮度输入成功", false);
	}
	else
	{
		return -1;//to do 
		//addToStateMessage("亮度输入应为整数 , 范围：1~100", true);
	}
}

int InsPMDL_lightSouce::Ins_Initial(int BoardNum)
{
	if (BoardNum == 1)
	{
		writeToModbusOne(0x0886, 487);//110C -> 0x 1e7  //sssss
		writeToModbusOne(0x1004, 1249728);//2008 -> 0x 1311c0
		writeToModbusOne(0x1022, 1250000);//2044 -> 0x 1312d0
		writeToModbusOne(0x0B00, 1250000);//1600 -> 0x 1312d0
		writeToModbusOne(0x0C00, 0);//1800 -> 0
		writeToModbusOne(0x0C0A, 1);//1814 -> 1
		writeToModbusOne(0x1008, 1);//2010 -> 1

		writeToModbusOne(0x1010, 2500);//2020 -> 9c4

		writeToModbusOne(0x1708 / 2, 0x99999);//1708->0x99999
		writeToModbusOne(0x1718 / 2, 0x88888);//1718->0x88888
		writeToModbusOne(0x1778 / 2, 0x555555);//1718->0x555555
	}
	else if (BoardNum == 2)
	{
		write_fpga_reg(2, 0x0B30, 1);//10*20第二块版

		writeToModbusTwo(0x0886, 487);//110C -> 0x 1e7  //sssss
		writeToModbusTwo(0x1004, 1249728);//2008 -> 0x 1311c0
		writeToModbusTwo(0x1022, 1250000);//2044 -> 0x 1312d0
		writeToModbusTwo(0x0B00, 1250000);//1600 -> 0x 1312d0
		writeToModbusTwo(0x0C00, 0);//1800 -> 0
		writeToModbusTwo(0x0C0A, 1);//1814 -> 1
		writeToModbusTwo(0x1008, 1);//2010 -> 1

		writeToModbusTwo(0x1010, 2500);//2020 -> 9c4

		writeToModbusTwo(0x1708 / 2, 0x99999);//1708->0x99999
		writeToModbusTwo(0x1718 / 2, 0x88888);//1718->0x88888
		writeToModbusTwo(0x1778 / 2, 0x555555);//1718->0x555555

	}
	else if (BoardNum == 3)
	{
		writeToModbusSecondThird(0x0B30, 1);//10*30第二第三块板
//        ui->LightModel->setText(str);

		writeToModbusThree(0x0886, 487);//110C -> 0x 1e7
		writeToModbusThree(0x1004, 1249728);//2008 -> 0x 1311c0
		writeToModbusThree(0x1022, 1250000);//2044 -> 0x 1312d0
		writeToModbusThree(0x0B00, 1250000);//1600 -> 0x 1312d0
		writeToModbusThree(0x0C00, 0);//1800 -> 0
		writeToModbusThree(0x0C0A, 1);//1814 -> 1
		writeToModbusThree(0x1008, 1);//2010 -> 1

		writeToModbusThree(0x1010, 2500);//2020 -> 9c4

		//初始化触发输出脉宽
		writeToModbusThree(0x1708 / 2, 0x99999);//1708->0x99999
		writeToModbusThree(0x1718 / 2, 0x88888);//1718->0x88888
		writeToModbusThree(0x1778 / 2, 0x555555);//1718->0x555555
	}

	return 0;

	

}

int InsPMDL_lightSouce::Ins_SetIlluminationTime10ms(uint32_t index, double _time)
{
	double LightTime = _time;

	uint32_t ReadNum = 0;
	// 读取相机触发数量
	readFromModbusOne(0x0C0A, ReadNum);

	// 毫秒值转化为微秒值，为确保安全(计算误差)，保留1us余量
	// 光源内部循环以4us为一个循环周期，计算出内部循环次数
	double CameraTriggerCycle2 = ((LightTime * 1000) - 1) / ReadNum;
	uint32_t SwLoopNumVal = CameraTriggerCycle2 / 4;//4us

	// 光源内部各个状态切换时间消耗
	uint32_t MosLat = 3;
	uint32_t DdsLat = 3;
	double   SwLat = 2;
	uint32_t IdleLat = 1;
	uint32_t TrigLat = 1;

	uint16_t REGMOSACK[2] = {};
	uint16_t REGDDSACK[2] = {};
	uint16_t REGPICNUM[2] = {};

	uint16_t REGSWGAP[2] = {};
	uint16_t REGTRIGGAP[2] = {};
	uint16_t REGSWLOOPNUM[2] = {};
	uint16_t REGCAMTRINUM[2] = {};

	// 获取光源内部各个状态响应时间 --一般为定值
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0880, REGMOSACK);//1100
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0882, REGDDSACK);//1104
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1008, REGPICNUM);//2010
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x100E, REGSWGAP);//201c
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1006, REGTRIGGAP);//200c
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x1010, REGSWLOOPNUM);//2020
	Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x0C0A, REGCAMTRINUM);//1814

	// 16bit→32bit位c拼接,将上文获取到的信息拼接为完整的寄存器值
	uint32_t MOSACKZ = (static_cast<uint32_t>(REGMOSACK[0]) << 16) | REGMOSACK[1];
	uint32_t DDSACKZ = (static_cast<uint32_t>(REGDDSACK[0]) << 16) | REGDDSACK[1];
	uint32_t PicNumZ = (static_cast<uint32_t>(REGPICNUM[0]) << 16) | REGPICNUM[1];
	uint32_t SWGAPZ = (static_cast<uint32_t>(REGSWGAP[0]) << 16) | REGSWGAP[1];
	uint32_t TriigerGapZ = (static_cast<uint32_t>(REGTRIGGAP[0]) << 16) | REGTRIGGAP[1];
	uint32_t SWLoopNumZ = (static_cast<uint32_t>(REGSWLOOPNUM[0]) << 16) | REGSWLOOPNUM[1];
	uint32_t CamTriNumZ = (static_cast<uint32_t>(REGCAMTRINUM[0]) << 16) | REGCAMTRINUM[1];

	// 状态切换时间+状态响应时间为实际状态所需要时间
	uint32_t MosTrue = MOSACKZ + MosLat;
	uint32_t DdsTrue = DDSACKZ + DdsLat;

	// 计算灯控循环的小循环和小小循环消耗的额外时间
	double Texpend1 = MosTrue + DdsTrue + IdleLat + MosTrue;
	double Texpend2 = DdsTrue + TrigLat;

	// 计算单次光源点亮周期并转换为FPGA内部拍数
	double Tencode = ((CameraTriggerCycle2)*PicNumZ) * (1000 / 8); //PicNumZ是取相数量
	// 计算单次光源循环内消耗时间
	double TpicLoop = (Tencode - Texpend1) / PicNumZ;

	// 计算单次光源循环光源实际点亮消耗时间(包含开关本身消耗及单次光源循环时隙)
	uint32_t ExpLoopVal = TpicLoop - TriigerGapZ - Texpend2;
	// 计算单次光源循环光源实际点亮消耗时间(去除开关本身消耗及单次光源循环时隙)
	double SwTrue = (ExpLoopVal / SwLoopNumVal) - PicNumZ - SWGAPZ;

	// 计算待写入参数并写入参数
	uint32_t SwAckVal = SwTrue - SwLat;
	uint32_t CameraCycleVal = (CameraTriggerCycle2) * (1000 / 8); //1207

	uint32_t valueToWrite = ExpLoopVal;
	write_fpga_reg(index, 0x2008, valueToWrite);
	//writeToModbusOne(0x1004, valueToWrite);

	uint32_t valueToWrite2 = SwAckVal;
	//writeToModbusOne(0x0886, valueToWrite2);
	write_fpga_reg(index, 0x110C, valueToWrite2);

	uint32_t valueToWrite3 = CameraCycleVal;
	//writeToModbusOne(0x1022, valueToWrite3);
	write_fpga_reg(index, 0x2044, valueToWrite3);

	//writeToModbusOne(0x1010, SwLoopNumVal);
	write_fpga_reg(index, 0x2020, SwLoopNumVal);

	// 软触发周期不需要在这里设置
	// writeToModbusOne(0x0B00, LightTime * 1000000 / 8);

	// 2044寄存器已经写过了，重复写没有意义
	// writeToModbusOne(0x1022, LightTime * 1000000 / 8);
	return 0;
}


int InsPMDL_lightSouce::Ins_SetArrayEn(uint32_t _index, bool _enable)
{
	write_fpga_reg(_index, 0x2300, (uint32_t)_enable);//2300->0//开始时先关一次
	return 0;
}


int InsPMDL_lightSouce::Ins_SetFirstLightProperty(uint32_t _index)
{
	write_fpga_reg(_index, 0x2300, 0);
	write_fpga_reg(_index, 0x2400, 0);
	write_fpga_reg(_index, 0x2500, 0);

	return 0;
}


int InsPMDL_lightSouce::Ins_Set8LightProperty(uint32_t _index, uint32_t _illuminate_mode)
{
	write_fpga_reg(_index, 0x2300, 0);
	if ((_illuminate_mode == 1) || (_illuminate_mode == 3))
	{
		if (_index % 2) //若index为奇数
		{
			write_fpga_reg(_index, 0x2400, 0);
			write_fpga_reg(_index, 0x2404, 72);
			write_fpga_reg(_index, 0x2408, 144);
			write_fpga_reg(_index, 0x240C, 216);
			write_fpga_reg(_index, 0x2410, 0);
			write_fpga_reg(_index, 0x2414, 216);
			write_fpga_reg(_index, 0x2418, 144);
			write_fpga_reg(_index, 0x241C, 72);

			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
		}
		else //index为偶数
		{
			write_fpga_reg(_index, 0x2400, (144 + 0) % 288); //144  波表周期288
			write_fpga_reg(_index, 0x2404, (144 + 72) % 288);//216
			write_fpga_reg(_index, 0x2408, (144 + 144) % 288);//0
			write_fpga_reg(_index, 0x240C, (144 + 216) % 288);//72
			write_fpga_reg(_index, 0x2410, (144 + 0) % 288);//144
			write_fpga_reg(_index, 0x2414, (144 + 216) % 288);//72
			write_fpga_reg(_index, 0x2418, (144 + 144) % 288);//0
			write_fpga_reg(_index, 0x241C, (144 + 72) % 288);//216

			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
			write_fpga_reg(_index, 0x2500, 0);
		}
	}
	else if (_illuminate_mode == 0)
	{
		write_fpga_reg(_index, 0x2400, ((_index - 1) * 72 + 0) % 288);
		write_fpga_reg(_index, 0x2404, ((_index - 1) * 72 + 72) % 288);
		write_fpga_reg(_index, 0x2408, ((_index - 1) * 72 + 144) % 288);
		write_fpga_reg(_index, 0x240C, ((_index - 1) * 72 + 216) % 288);
		write_fpga_reg(_index, 0x2410, ((_index - 1) * 72 + 0) % 288);
		write_fpga_reg(_index, 0x2414, ((_index - 1) * 72 + 216) % 288);
		write_fpga_reg(_index, 0x2418, ((_index - 1) * 72 + 144) % 288);
		write_fpga_reg(_index, 0x241C, ((_index - 1) * 72 + 72) % 288);

		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
	}
	else
	{
		write_fpga_reg(_index, 0x2400, 0);
		write_fpga_reg(_index, 0x2404, 72);
		write_fpga_reg(_index, 0x2408, 144);
		write_fpga_reg(_index, 0x240C, 216);
		write_fpga_reg(_index, 0x2410, 0);
		write_fpga_reg(_index, 0x2414, 216);
		write_fpga_reg(_index, 0x2418, 144);
		write_fpga_reg(_index, 0x241C, 72);

		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
		write_fpga_reg(_index, 0x2500, 0);
	}
	return 0;
}


int InsPMDL_lightSouce::Ins_SetDirection(int _BoardNum, int _Direction)
{
	if (_Direction == 0)
	{
		for (int index = 1; index <= _BoardNum; index++)
		{
			write_fpga_reg(index, 0x2308, 0x0F0F0F0F);
			write_fpga_reg(index, 0x230c, 0xF0F0F0F0);
			write_fpga_reg(index, 0x2400, 0);
			write_fpga_reg(index, 0x2404, 72);
			write_fpga_reg(index, 0x2408, 144);
			write_fpga_reg(index, 0x240c, 216);
			write_fpga_reg(index, 0x2410, 0);
			write_fpga_reg(index, 0x2414, 216);
			write_fpga_reg(index, 0x2418, 144);
			write_fpga_reg(index, 0x241c, 72);

		}
	}
	if (_Direction == 1)
	{
		for (int index = 1; index <= _BoardNum; index++)
		{
			write_fpga_reg(index, 0x2308, 0xF0F0F0F0);
			write_fpga_reg(index, 0x230c, 0x0F0F0F0F);
			write_fpga_reg(index, 0x2400, 0);
			write_fpga_reg(index, 0x2404, 72);
			write_fpga_reg(index, 0x2408, 144);
			write_fpga_reg(index, 0x240c, 216);
			write_fpga_reg(index, 0x2410, 0);
			write_fpga_reg(index, 0x2414, 72);
			write_fpga_reg(index, 0x2418, 144);
			write_fpga_reg(index, 0x241c, 216);

		}
	}

	if (_Direction == 2)
	{
		for (int index = 1; index <= _BoardNum; index++)
		{
			write_fpga_reg(index, 0x2308, 0x0F0F0F0F);
			write_fpga_reg(index, 0x230c, 0xF0F0F0F0);
			write_fpga_reg(index, 0x2400, 0);
			write_fpga_reg(index, 0x2404, 216);
			write_fpga_reg(index, 0x2408, 144);
			write_fpga_reg(index, 0x240c, 72);
			write_fpga_reg(index, 0x2410, 0);
			write_fpga_reg(index, 0x2414, 72);
			write_fpga_reg(index, 0x2418, 144);
			write_fpga_reg(index, 0x241c, 216);
			write_fpga_reg(index, 0x2414, 72);
			write_fpga_reg(index, 0x241c, 216);

		}
	}

	if (_Direction == 3)
	{
		for (int index = 1; index <= _BoardNum; index++)
		{
			write_fpga_reg(index, 0x2308, 0xF0F0F0F0);
			write_fpga_reg(index, 0x230c, 0x0F0F0F0F);
			write_fpga_reg(index, 0x2400, 0);
			write_fpga_reg(index, 0x2404, 216);
			write_fpga_reg(index, 0x2408, 144);
			write_fpga_reg(index, 0x240c, 72);
			write_fpga_reg(index, 0x2410, 0);
			write_fpga_reg(index, 0x2414, 216);
			write_fpga_reg(index, 0x2418, 144);
			write_fpga_reg(index, 0x241c, 72);

		}
	}
	return 0;
}


int InsPMDL_lightSouce::Ins_SetPhaseOffset(uint32_t _index, int _Direction, int LightMode)
{


	if (_index == 1)
	{
		write_fpga_reg(1, 0x2500, 0);
		write_fpga_reg(1, 0x2504, 0);
		write_fpga_reg(1, 0x2508, 0);
		write_fpga_reg(1, 0x250c, 0);
		write_fpga_reg(1, 0x2510, 0);
		write_fpga_reg(1, 0x2514, 0);
		write_fpga_reg(1, 0x2518, 0);
		write_fpga_reg(1, 0x251c, 0);
	}
	else if (_index == 2)
	{
		if (_Direction == 0 || _Direction == 2)// 角度1 //角度3
		{
			if (LightMode == 0)//极大
			{
				write_fpga_reg(2, 0x2500, 72);
				write_fpga_reg(2, 0x2504, 72);
				write_fpga_reg(2, 0x2508, 72);
				write_fpga_reg(2, 0x250c, 72);
				write_fpga_reg(2, 0x2510, 0);
				write_fpga_reg(2, 0x2514, 0);
				write_fpga_reg(2, 0x2518, 0);
				write_fpga_reg(2, 0x251c, 0);
			}
			else if (LightMode == 1 || LightMode == 3)//大 小
			{
				write_fpga_reg(2, 0x2500, 144);
				write_fpga_reg(2, 0x2504, 144);
				write_fpga_reg(2, 0x2508, 144);
				write_fpga_reg(2, 0x250c, 144);
				write_fpga_reg(2, 0x2510, 0);
				write_fpga_reg(2, 0x2514, 0);
				write_fpga_reg(2, 0x2518, 0);
				write_fpga_reg(2, 0x251c, 0);

			}

			else if (LightMode == 2 || LightMode == 4)//标准 极小
			{
				write_fpga_reg(2, 0x2500, 0);
				write_fpga_reg(2, 0x2504, 0);
				write_fpga_reg(2, 0x2508, 0);
				write_fpga_reg(2, 0x250c, 0);
				write_fpga_reg(2, 0x2510, 0);
				write_fpga_reg(2, 0x2514, 0);
				write_fpga_reg(2, 0x2518, 0);
				write_fpga_reg(2, 0x251c, 0);
			}
		}

		if (_Direction == 1 || _Direction == 3)//角度2 角度4
		{
			if (LightMode == 0)//极大
			{
				write_fpga_reg(2, 0x2500, 0);
				write_fpga_reg(2, 0x2504, 0);
				write_fpga_reg(2, 0x2508, 0);
				write_fpga_reg(2, 0x250c, 0);
				write_fpga_reg(2, 0x2510, 72);
				write_fpga_reg(2, 0x2514, 72);
				write_fpga_reg(2, 0x2518, 72);
				write_fpga_reg(2, 0x251c, 72);
			}

			else if (LightMode == 1 || LightMode == 3)//大  小
			{
				write_fpga_reg(2, 0x2500, 0);
				write_fpga_reg(2, 0x2504, 0);
				write_fpga_reg(2, 0x2508, 0);
				write_fpga_reg(2, 0x250c, 0);
				write_fpga_reg(2, 0x2510, 144);
				write_fpga_reg(2, 0x2514, 144);
				write_fpga_reg(2, 0x2518, 144);
				write_fpga_reg(2, 0x251c, 144);
			}

			else if (LightMode == 2 || LightMode == 4)// 标准
			{
				write_fpga_reg(2, 0x2500, 0);
				write_fpga_reg(2, 0x2504, 0);
				write_fpga_reg(2, 0x2508, 0);
				write_fpga_reg(2, 0x250c, 0);
				write_fpga_reg(2, 0x2510, 0);
				write_fpga_reg(2, 0x2514, 0);
				write_fpga_reg(2, 0x2518, 0);
				write_fpga_reg(2, 0x251c, 0);
			}

		}

	}

	else if (_index == 3) //第三块板
	{

		if (_Direction == 0 || _Direction == 2)
		{
			if (LightMode == 0)
			{
				write_fpga_reg(3, 0x2500, 144);
				write_fpga_reg(3, 0x2504, 144);
				write_fpga_reg(3, 0x2508, 144);
				write_fpga_reg(3, 0x250c, 144);
				write_fpga_reg(3, 0x2510, 0);
				write_fpga_reg(3, 0x2514, 0);
				write_fpga_reg(3, 0x2518, 0);
				write_fpga_reg(3, 0x251c, 0);
			}
			else
			{
				write_fpga_reg(3, 0x2500, 0);
				write_fpga_reg(3, 0x2504, 0);
				write_fpga_reg(3, 0x2508, 0);
				write_fpga_reg(3, 0x250c, 0);
				write_fpga_reg(3, 0x2510, 0);
				write_fpga_reg(3, 0x2514, 0);
				write_fpga_reg(3, 0x2518, 0);
				write_fpga_reg(3, 0x251c, 0);

			}
		}
		else if (_Direction == 1 || _Direction == 3)
		{
			if (LightMode == 0)
			{
				write_fpga_reg(3, 0x2500, 0);
				write_fpga_reg(3, 0x2504, 0);
				write_fpga_reg(3, 0x2508, 0);
				write_fpga_reg(3, 0x250c, 0);
				write_fpga_reg(3, 0x2510, 144);
				write_fpga_reg(3, 0x2514, 144);
				write_fpga_reg(3, 0x2518, 144);
				write_fpga_reg(3, 0x251c, 144);
			}
			else
			{
				write_fpga_reg(3, 0x2500, 0);
				write_fpga_reg(3, 0x2504, 0);
				write_fpga_reg(3, 0x2508, 0);
				write_fpga_reg(3, 0x250c, 0);
				write_fpga_reg(3, 0x2510, 0);
				write_fpga_reg(3, 0x2514, 0);
				write_fpga_reg(3, 0x2518, 0);
				write_fpga_reg(3, 0x251c, 0);

			}
		}

	}


	return 0;
}

//---------------------0318---------------------------------
int InsPMDL_lightSouce::FreeRunSinToContinue(int LightSignInt)
{

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x0C0A, 1);//1814
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x0C0A, 1);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x0C0A, 1);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x0C0A, 1);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x0C0A, 1);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x0C0A, 1);
	}

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1182, 1);//2304
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1182, 1);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1182, 1);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1182, 1);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1182, 1);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1182, 1);
	}

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1200, 144);//2400

	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1200, 144);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1200, 144);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1200, 144);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1200, 144);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1200, 144);
	}

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1240, 0);//2480
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1240, 0);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1240, 0);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1240, 0);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1240, 0);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1240, 0);
	}

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1280, 512);//2500
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1240, 0);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1280, 512);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1240, 0);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1280, 512);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1240, 0);
	}

	if (LightSignInt == 10)
	{
		writeToModbusOne(0x1180, 1);//2300
	}
	else if (LightSignInt == 20)
	{
		writeToModbusTwo(0x1180, 1);
	}
	else if (LightSignInt == 30)
	{
		writeToModbusThree(0x1180, 1);
	}
	else if (LightSignInt == 40)
	{
		writeToModbusFour(0x1180, 1);
	}
	else if (LightSignInt == 50)
	{
		writeToModbusFive(0x1180, 1);
	}
	else if (LightSignInt == 60)
	{
		writeToModbusSix(0x1180, 1);
	}

	return 0;
}

int InsPMDL_lightSouce::select_direction(int index,int _board_num)
{

	int LightSignInt = _board_num * 10;
	if (index == 0)
	{

		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1014, 1);//2028
			writeToModbusOne(0x1016, 0);//202C
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1014, 1);//2028
			writeToModbusTwo(0x1016, 0);//202C
		}
		else if (LightSignInt == 30)
		{
			writeToModbusThree(0x1014, 1);//2028
			writeToModbusThree(0x1016, 0);//202C
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1014, 1);//2028
			writeToModbusFour(0x1016, 0);//202C
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1014, 1);//2028
			writeToModbusFive(0x1016, 0);//202C
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1014, 1);//2028
			writeToModbusSix(0x1016, 0);//202C
		}
		cout << "111" << endl;


	}
	else if (index == 1)
	{

		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1014, 0);//2028
			writeToModbusOne(0x1016, 1);//202C
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1014, 0);//2028
			writeToModbusTwo(0x1016, 1);//202C
		}
		else if (LightSignInt == 30)
		{
			writeToModbusThree(0x1014, 0);//2028
			writeToModbusThree(0x1016, 1);//202C
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1014, 0);//2028
			writeToModbusFour(0x1016, 1);//202C
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1014, 0);//2028
			writeToModbusFive(0x1016, 1);//202C
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1014, 0);//2028
			writeToModbusSix(0x1016, 1);//202C
		}
		cout << "222" << endl;


	}
	else if (index == 2)
	{
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1014, 1);//2028
			writeToModbusOne(0x1016, 1);//202C
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1014, 1);//2028
			writeToModbusTwo(0x1016, 1);//202C
		}
		else if (LightSignInt == 30)
		{
			writeToModbusThree(0x1014, 1);//2028
			writeToModbusThree(0x1016, 1);//202C
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1014, 1);//2028
			writeToModbusFour(0x1016, 1);//202C
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1014, 1);//2028
			writeToModbusFive(0x1016, 1);//202C
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1014, 1);//2028
			writeToModbusSix(0x1016, 1);//202C
		}

		cout << "333" << endl;

	}
	else if (index == 3)
	{
		if (LightSignInt == 10)
		{
			writeToModbusOne(0x1014, 0);//2028
			writeToModbusOne(0x1016, 0);//202C
		}
		else if (LightSignInt == 20)
		{
			writeToModbusTwo(0x1014, 0);//2028
			writeToModbusTwo(0x1016, 0);//202C
		}
		else if (LightSignInt == 30)
		{
			writeToModbusThree(0x1014, 0);//2028
			writeToModbusThree(0x1016, 0);//202C
		}
		else if (LightSignInt == 40)
		{
			writeToModbusFour(0x1014, 0);//2028
			writeToModbusFour(0x1016, 0);//202C
		}
		else if (LightSignInt == 50)
		{
			writeToModbusFive(0x1014, 0);//2028
			writeToModbusFive(0x1016, 0);//202C
		}
		else if (LightSignInt == 60)
		{
			writeToModbusSix(0x1014, 0);//2028
			writeToModbusSix(0x1016, 0);//202C
		}

	}

	return 0;
}



