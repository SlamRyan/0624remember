#pragma once
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <String>
#include <cstring>

#define MIN_ALLOWED_A   1     //定义最小幅值
#define MAX_ALLOWED_A  10     //定义最大幅值

#define MIN_ALLOWED_T  3.14 // 定义最小允许的周期
#define MAX_ALLOWED_T  100*3.14 // 定义最大允许的周期

#define MIN_LIGHT 0    // 最低亮度
#define MAX_LIGHT 255  // 最高亮度

#ifdef INSLIGHT_EXPORTS

#define INSLIGHT_API __declspec(dllexport)
#else
#define INSLIGHT_API __declspec(dllimport)
#endif

extern "C" INSLIGHT_API void HelloLight();
extern "C" INSLIGHT_API uint16_t getLEDModeValue(int modeIndex);
extern "C" INSLIGHT_API uint16_t setLEDLight(uint16_t light);
extern "C" INSLIGHT_API uint16_t getTriggerFre(int modeIndex);
extern "C" INSLIGHT_API uint16_t customTriggerFre(uint16_t Fre);

 

enum LEDMode 
{
    EXTREME_LARGE = 32766,
    LARGE = 32767,
    STANDARD = 32768,  // 或0x8000
    SMALL = 32769,
    EXTREME_SMALL = 32770
};


//===================1.自编辑条纹===============================

// 从用户界面上的LED行状态编码为32位整数
//uint32_t encodeLEDRowStatus(const bool rowStatus[24]);//将上位机输入的bool数组转换为uint32_t的值
//
//// 从32位整数解码为用户界面上的LED行状态
//void decodeLEDRowStatus(uint32_t encoded, bool rowStatus[24]);//将FPGA的数据解码乘bool类型显示在UI界面上
//
//
//uint32_t encodeLEDColStatus(const bool colStatus[24]);//输入
//
//
//void decodeLEDColStatus(uint32_t encoded, bool colStatus[24]);//解码FPGA
//===========================================================


//=========================2.幅值、频率=========================
//uint16_t setA(double A);
//
//
//uint16_t setT(double T);


//===========================================================

//=========================3.亮度=============================



//==============3.Modbus收发===================================
// 
// Modbus Send Uint32 To FPGA
//void sendRowViaModbus(uint32_t encoded);
//void sendColViaModbus(uint32_t encoded);
//
//// Modbus 从FPGA接收Uint32
//uint32_t receiveRowViaModbus();//从FPGA接受数据
//uint32_t receiveColViaModbus();
//===========================================================