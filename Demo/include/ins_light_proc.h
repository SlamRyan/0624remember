#pragma once
#include <iostream>
#include <math.h>
#include <stdint.h>
#include <String>
#include <cstring>

#define MIN_ALLOWED_A   1     //������С��ֵ
#define MAX_ALLOWED_A  10     //��������ֵ

#define MIN_ALLOWED_T  3.14 // ������С���������
#define MAX_ALLOWED_T  100*3.14 // ����������������

#define MIN_LIGHT 0    // �������
#define MAX_LIGHT 255  // �������

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
    STANDARD = 32768,  // ��0x8000
    SMALL = 32769,
    EXTREME_SMALL = 32770
};


//===================1.�Ա༭����===============================

// ���û������ϵ�LED��״̬����Ϊ32λ����
//uint32_t encodeLEDRowStatus(const bool rowStatus[24]);//����λ�������bool����ת��Ϊuint32_t��ֵ
//
//// ��32λ��������Ϊ�û������ϵ�LED��״̬
//void decodeLEDRowStatus(uint32_t encoded, bool rowStatus[24]);//��FPGA�����ݽ����bool������ʾ��UI������
//
//
//uint32_t encodeLEDColStatus(const bool colStatus[24]);//����
//
//
//void decodeLEDColStatus(uint32_t encoded, bool colStatus[24]);//����FPGA
//===========================================================


//=========================2.��ֵ��Ƶ��=========================
//uint16_t setA(double A);
//
//
//uint16_t setT(double T);


//===========================================================

//=========================3.����=============================



//==============3.Modbus�շ�===================================
// 
// Modbus Send Uint32 To FPGA
//void sendRowViaModbus(uint32_t encoded);
//void sendColViaModbus(uint32_t encoded);
//
//// Modbus ��FPGA����Uint32
//uint32_t receiveRowViaModbus();//��FPGA��������
//uint32_t receiveColViaModbus();
//===========================================================