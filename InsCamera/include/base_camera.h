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
	unsigned char* pChunkData;                                 ///< [OUT] \~chinese Chunk����              \~english Chunk Data
	unsigned int        nChunkID;                                   ///< [OUT] \~chinese Chunk ID               \~english Chunk ID
	unsigned int        nChunkLen;                                  ///< [OUT] \~chinese Chunk�ĳ���            \~english Chunk Length

	unsigned int        nReserved[8];                               ///<       \~chinese Ԥ��                   \~english Reserved

}CHUNK_DATA_CONTENT;
/// \~chinese ���֡����Ϣ              \~english Output Frame Information
typedef struct _FRAME_OUT_INFO_EX_
{
	unsigned short          nWidth;                                 ///< [OUT] \~chinese ͼ���                 \~english Image Width
	unsigned short          nHeight;                                ///< [OUT] \~chinese ͼ���                 \~english Image Height
	enum PixelType    enPixelType;                            ///< [OUT] \~chinese ���ظ�ʽ               \~english Pixel Type

	unsigned int            nFrameNum;                              ///< [OUT] \~chinese ֡��                   \~english Frame Number
	unsigned int            nDevTimeStampHigh;                      ///< [OUT] \~chinese ʱ�����32λ           \~english Timestamp high 32 bits
	unsigned int            nDevTimeStampLow;                       ///< [OUT] \~chinese ʱ�����32λ           \~english Timestamp low 32 bits
	unsigned int            nReserved0;                             ///< [OUT] \~chinese ������8�ֽڶ���        \~english Reserved, 8-byte aligned
	int64_t                 nHostTimeStamp;                         ///< [OUT] \~chinese �������ɵ�ʱ���       \~english Host-generated timestamp

	unsigned int            nFrameLen;                              ///< [OUT] \~chinese ֡�ĳ���               \~english The Length of Frame

	/// \~chinese �豸ˮӡʱ��      \~english Device frame-specific time scale
	unsigned int            nSecondCount;                           ///< [OUT] \~chinese ����                   \~english The Seconds
	unsigned int            nCycleCount;                            ///< [OUT] \~chinese ������                 \~english The Count of Cycle
	unsigned int            nCycleOffset;                           ///< [OUT] \~chinese ����ƫ����             \~english The Offset of Cycle

	float                   fGain;                                  ///< [OUT] \~chinese ����                   \~english Gain
	float                   fExposureTime;                          ///< [OUT] \~chinese �ع�ʱ��               \~english Exposure Time
	unsigned int            nAverageBrightness;                     ///< [OUT] \~chinese ƽ������               \~english Average brightness

	/// \~chinese ��ƽ�����        \~english White balance
	unsigned int            nRed;                                   ///< [OUT] \~chinese ��ɫ                   \~english Red
	unsigned int            nGreen;                                 ///< [OUT] \~chinese ��ɫ                   \~english Green
	unsigned int            nBlue;                                  ///< [OUT] \~chinese ��ɫ                   \~english Blue

	unsigned int            nFrameCounter;                          ///< [OUT] \~chinese ��֡��                 \~english Frame Counter
	unsigned int            nTriggerIndex;                          ///< [OUT] \~chinese ��������               \~english Trigger Counting

	unsigned int            nInput;                                 ///< [OUT] \~chinese ����                   \~english Input
	unsigned int            nOutput;                                ///< [OUT] \~chinese ���                   \~english Output

	/// \~chinese ROI����           \~english ROI Region
	unsigned short          nOffsetX;                               ///< [OUT] \~chinese ˮƽƫ����             \~english OffsetX
	unsigned short          nOffsetY;                               ///< [OUT] \~chinese ��ֱƫ����             \~english OffsetY
	unsigned short          nChunkWidth;                            ///< [OUT] \~chinese Chunk��                \~english The Width of Chunk
	unsigned short          nChunkHeight;                           ///< [OUT] \~chinese Chunk��                \~english The Height of Chunk

	unsigned int            nLostPacket;                            ///< [OUT] \~chinese ��֡������             \~english Lost Packet Number In This Frame

	unsigned int            nUnparsedChunkNum;                      ///< [OUT] \~chinese δ������Chunkdata����  \~english Unparsed Chunk Number
	union
	{
		CHUNK_DATA_CONTENT* pUnparsedChunkContent;              ///< [OUT] \~chinese δ������Chunk          \~english Unparsed Chunk Content
		int64_t                 nAligning;                          ///< [OUT] \~chinese У׼                   \~english Aligning
	}UnparsedChunkList;

	unsigned int            nReserved[36];                          ///<       \~chinese Ԥ��                   \~english Reserved

}FRAME_OUT_INFO_EX;

/// \~chinese ͼ��ṹ�壬���ͼ���ַ��ͼ����Ϣ    \~english Image Struct, output the pointer of Image and the information of the specific image
typedef struct _FRAME_OUT_
{
	unsigned char* pBufAddr;                               ///< [OUT] \~chinese ͼ��ָ���ַ           \~english  pointer of image
	FRAME_OUT_INFO_EX    stFrameInfo;                            ///< [OUT] \~chinese ͼ����Ϣ               \~english information of the specific image

	unsigned int            nRes[16];                               ///<       \~chinese Ԥ��                   \~english Reserved

}FRAME_OUT;

/// \~chinese ȡ������                  \~english The strategy of Grabbing
typedef enum _GRAB_STRATEGY_
{
	GrabStrategy_OneByOne = 0,                        ///< \~chinese �Ӿɵ���һ֡һ֡�Ļ�ȡͼ��   \~english Grab One By One
	GrabStrategy_LatestImagesOnly = 1,                        ///< \~chinese ��ȡ�б������µ�һ֡ͼ��     \~english Grab The Latest Image
	GrabStrategy_LatestImages = 2,                        ///< \~chinese ��ȡ�б������µ�ͼ��         \~english Grab The Latest Images
	GrabStrategy_UpcomingImage = 3,                        ///< \~chinese �ȴ���һ֡ͼ��               \~english Grab The Upcoming Image

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
	InsResult get_ins_PAC_gain(char* _val);//10.07,����(char*)malloc(4);
private:
	
};



#endif // INS_CAMERA_H
