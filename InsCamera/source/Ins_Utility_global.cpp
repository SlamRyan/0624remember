#include "..\include\ins_global.h"


void PrintTime(std::string x)
{
auto currentTime = std::chrono::system_clock::now();

// ��ʱ���ת��Ϊʱ��ṹ
std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
std::tm* localTime = std::localtime(&currentTime_t);

// ��ȷ�������ʱ��
auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
/*
// ���ʱ���
std::cout << "��ǰʱ���: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count()
    << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;
*/
// �����ʽ����ʱ�䣬��ȷ������
std::cout << std::dec<< x << "��ǰʱ��: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
<< "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;

}