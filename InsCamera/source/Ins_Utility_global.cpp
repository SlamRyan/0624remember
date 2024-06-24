#include "..\include\ins_global.h"


void PrintTime(std::string x)
{
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
*/
// 输出格式化的时间，精确到毫秒
std::cout << std::dec<< x << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
<< "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;

}