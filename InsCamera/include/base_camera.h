#ifndef INS_CAMERA_H
#define INS_CAMERA_H


#include<Windows.h>
#include<iostream>
#include<shellapi.h>
#include <commdlg.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <shlwapi.h>
#include <tchar.h>
#include <vector>

#include "ins_global.h"

enum PixelType
{
	// Undefined pixel type
	PixelType_Undefined = 0xFFFFFFFF
};

typedef struct _CHUNK_DATA_CONTENT_
{
	unsigned char* pChunkData;                                 ///< [OUT] \~chinese Chunk数据              \~english Chunk Data
	unsigned int        nChunkID;                                   ///< [OUT] \~chinese Chunk ID               \~english Chunk ID
	unsigned int        nChunkLen;                                  ///< [OUT] \~chinese Chunk的长度            \~english Chunk Length

	unsigned int        nReserved[8];                               ///<       \~chinese 预留                   \~english Reserved

}CHUNK_DATA_CONTENT;
/// \~chinese 输出帧的信息              \~english Output Frame Information
typedef struct _FRAME_OUT_INFO_EX_
{
	unsigned short          nWidth;                                 ///< [OUT] \~chinese 图像宽                 \~english Image Width
	unsigned short          nHeight;                                ///< [OUT] \~chinese 图像高                 \~english Image Height
	enum PixelType    enPixelType;                            ///< [OUT] \~chinese 像素格式               \~english Pixel Type

	unsigned int            nFrameNum;                              ///< [OUT] \~chinese 帧号                   \~english Frame Number
	unsigned int            nDevTimeStampHigh;                      ///< [OUT] \~chinese 时间戳高32位           \~english Timestamp high 32 bits
	unsigned int            nDevTimeStampLow;                       ///< [OUT] \~chinese 时间戳低32位           \~english Timestamp low 32 bits
	unsigned int            nReserved0;                             ///< [OUT] \~chinese 保留，8字节对齐        \~english Reserved, 8-byte aligned
	int64_t                 nHostTimeStamp;                         ///< [OUT] \~chinese 主机生成的时间戳       \~english Host-generated timestamp

	unsigned int            nFrameLen;                              ///< [OUT] \~chinese 帧的长度               \~english The Length of Frame

	/// \~chinese 设备水印时标      \~english Device frame-specific time scale
	unsigned int            nSecondCount;                           ///< [OUT] \~chinese 秒数                   \~english The Seconds
	unsigned int            nCycleCount;                            ///< [OUT] \~chinese 周期数                 \~english The Count of Cycle
	unsigned int            nCycleOffset;                           ///< [OUT] \~chinese 周期偏移量             \~english The Offset of Cycle

	float                   fGain;                                  ///< [OUT] \~chinese 增益                   \~english Gain
	float                   fExposureTime;                          ///< [OUT] \~chinese 曝光时间               \~english Exposure Time
	unsigned int            nAverageBrightness;                     ///< [OUT] \~chinese 平均亮度               \~english Average brightness

	/// \~chinese 白平衡相关        \~english White balance
	unsigned int            nRed;                                   ///< [OUT] \~chinese 红色                   \~english Red
	unsigned int            nGreen;                                 ///< [OUT] \~chinese 绿色                   \~english Green
	unsigned int            nBlue;                                  ///< [OUT] \~chinese 蓝色                   \~english Blue

	unsigned int            nFrameCounter;                          ///< [OUT] \~chinese 总帧数                 \~english Frame Counter
	unsigned int            nTriggerIndex;                          ///< [OUT] \~chinese 触发计数               \~english Trigger Counting

	unsigned int            nInput;                                 ///< [OUT] \~chinese 输入                   \~english Input
	unsigned int            nOutput;                                ///< [OUT] \~chinese 输出                   \~english Output

	/// \~chinese ROI区域           \~english ROI Region
	unsigned short          nOffsetX;                               ///< [OUT] \~chinese 水平偏移量             \~english OffsetX
	unsigned short          nOffsetY;                               ///< [OUT] \~chinese 垂直偏移量             \~english OffsetY
	unsigned short          nChunkWidth;                            ///< [OUT] \~chinese Chunk宽                \~english The Width of Chunk
	unsigned short          nChunkHeight;                           ///< [OUT] \~chinese Chunk高                \~english The Height of Chunk

	unsigned int            nLostPacket;                            ///< [OUT] \~chinese 本帧丢包数             \~english Lost Packet Number In This Frame

	unsigned int            nUnparsedChunkNum;                      ///< [OUT] \~chinese 未解析的Chunkdata个数  \~english Unparsed Chunk Number
	union
	{
		CHUNK_DATA_CONTENT* pUnparsedChunkContent;              ///< [OUT] \~chinese 未解析的Chunk          \~english Unparsed Chunk Content
		int64_t                 nAligning;                          ///< [OUT] \~chinese 校准                   \~english Aligning
	}UnparsedChunkList;

	unsigned int            nReserved[36];                          ///<       \~chinese 预留                   \~english Reserved

}FRAME_OUT_INFO_EX;

/// \~chinese 图像结构体，输出图像地址及图像信息    \~english Image Struct, output the pointer of Image and the information of the specific image
typedef struct _FRAME_OUT_
{
	unsigned char* pBufAddr;                               ///< [OUT] \~chinese 图像指针地址           \~english  pointer of image
	FRAME_OUT_INFO_EX    stFrameInfo;                            ///< [OUT] \~chinese 图像信息               \~english information of the specific image

	unsigned int            nRes[16];                               ///<       \~chinese 预留                   \~english Reserved

}FRAME_OUT;

/// \~chinese 取流策略                  \~english The strategy of Grabbing
typedef enum _GRAB_STRATEGY_
{
	GrabStrategy_OneByOne = 0,                        ///< \~chinese 从旧到新一帧一帧的获取图像   \~english Grab One By One
	GrabStrategy_LatestImagesOnly = 1,                        ///< \~chinese 获取列表中最新的一帧图像     \~english Grab The Latest Image
	GrabStrategy_LatestImages = 2,                        ///< \~chinese 获取列表中最新的图像         \~english Grab The Latest Images
	GrabStrategy_UpcomingImage = 3,                        ///< \~chinese 等待下一帧图像               \~english Grab The Upcoming Image

}GRAB_STRATEGY;



class INSCAMERA_API InsCamera
{
public:
	InsCamera();
	virtual ~InsCamera();
	virtual InsResult ConnectCamera();
	virtual InsResult FindCamera();
	virtual InsResult GrabContinous();
	virtual InsResult GrabStop();
	virtual InsResult set_ins_width(int _ins_width);
	virtual InsResult set_ins_height(int _ins_height);
	virtual int get_ins_width();
	virtual int get_ins_height();

	virtual InsResult set_ins_exposure(double _val);
	virtual double get_ins_exposure();
	
	InsResult set_ins_digital_gain(double _val);
	double get_ins_digital_gain();
	InsResult set_ins_PAC_gain(char* _ch);
	InsResult get_ins_PAC_gain(char* _val);//10.07,传入(char*)malloc(4);
private:
	
};



#endif // INS_CAMERA_H
