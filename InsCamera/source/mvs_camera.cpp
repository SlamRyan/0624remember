#include <stdio.h>
#include <Windows.h>
#include <process.h>
#include <conio.h>
#include <string>
#include "..\include\mvs_camera.h"
//#include "D:/1_INS/Includes/MvCameraControl.h"
//#include "D:\1_INS\Includes\MvCameraControl.h"


#define CHECKInsResult(x) if (MV_OK != x) return (InsResult)nRet

uint32_t MvsCamera::numCameras_ = 0;
//global
MV_CC_DEVICE_INFO_LIST stDeviceList;
bool g_bExit = false;
DataPocessCallBack mcallback;


//pData为句柄 pFrameInfo 图像帧信息 pUser传的this指针
void __stdcall ImageCallBackEx(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
	//MV_FRAME_OUT* pstFrame = NULL;
	int ret = 0;
	if (pFrameInfo)
	{
		printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
			pFrameInfo->nWidth, pFrameInfo->nHeight, pFrameInfo->nFrameNum);
	}
	//ret = MV_CC_GetImageBuffer(pData, pstFrame, 10000);
	unsigned char* pDateTemp = (unsigned char*)malloc(pFrameInfo->nWidth* pFrameInfo->nHeight);
	memset(pDateTemp,0, pFrameInfo->nWidth * pFrameInfo->nHeight);
	memcpy(pDateTemp, pData, pFrameInfo->nWidth * pFrameInfo->nHeight);
	std::cout << &pData << std::endl;
	mcallback(pUser, pData);
	free(pDateTemp);
}

bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo)
{
	if (NULL == pstMVDevInfo)
	{
		printf("The Pointer of pstMVDevInfo is NULL!\n");
		return false;
	}
	if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
	{
		int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
		int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
		int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
		int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
		// ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
		printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
		printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
	}
	else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
	{
		printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
		printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
		printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
	}
	else
	{
		printf("Not support.\n");
	}
	return true;
}

 MvsCamera::~MvsCamera()
{
	 //if (mhandle != NULL) {
		// delete mhandle;
		// mhandle = NULL;
	 //}
	 //
}


InsResult MvsCamera::FindCamera()
{
	int nRet = MV_OK;
	GIGE_DEVICE_INFO current_MvsCameraInfo;
	// ch:枚举设备 | en:Enum device
	
	memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
	CHECKInsResult(nRet);
	numCameras_ = stDeviceList.nDeviceNum;
	if (stDeviceList.nDeviceNum > 0)
	{
		for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
		{
			printf("[device %d]:\n", i);
			MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
			if (NULL == pDeviceInfo)
			{
				break;
			}
			PrintDeviceInfo(pDeviceInfo);
			current_MvsCameraInfo.nCurrentIp = pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp;
			strcpy(reinterpret_cast<char*>(current_MvsCameraInfo.chUserDefinedName), reinterpret_cast<const char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName));
			current_MvsCameraInfo.nNetExport = pDeviceInfo->SpecialInfo.stGigEInfo.nNetExport;
			current_MvsCameraInfo.nCurrentSubNetMask = pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentSubNetMask;
			current_MvsCameraInfo.nDefultGateWay = pDeviceInfo->SpecialInfo.stGigEInfo.nDefultGateWay;
			mMvsCameraInfo.push_back(current_MvsCameraInfo);
		}
	}
	else
	{
		printf("Find No Devices!\n");
	}
	return InsResult::INSCSTATUS_OK;
}



InsResult MvsCamera::ConnectCamera(int _index)
{
	//printf("Please Input camera index(0-%d):", numCameras_ - 1);
	//unsigned int nIndex = 0;
	//scanf_s("%d", &nIndex);
	int nRet = MV_OK;
	if (_index >= numCameras_)
	{
		return InsResult::INSSTATUS_INPUT_ERROR;
	}

	// ch:选择设备并创建句柄 | en:Select device and create handle
	nRet = MV_CC_CreateHandle(&mhandle, stDeviceList.pDeviceInfo[_index]);
	if (MV_OK != nRet)
	{
		printf("Create Handle fail! nRet [0x%x]\n", nRet);
	}

	// ch:打开设备 | en:Open device
	nRet = MV_CC_OpenDevice(mhandle);
	CHECKInsResult(nRet);

	// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
	if (stDeviceList.pDeviceInfo[_index]->nTLayerType == MV_GIGE_DEVICE)
	{
		int nPacketSize = MV_CC_GetOptimalPacketSize(mhandle);
		if (nPacketSize > 0)
		{
			nRet = MV_CC_SetIntValue(mhandle, "GevSCPSPacketSize", nPacketSize);
			if (nRet != MV_OK)
			{
				printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
			}
		}
		else
		{
			printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
		}
	}
	return InsResult::INSCSTATUS_OK;

}

InsResult MvsCamera::ForceIpEx(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay)
{
	int nRet = MV_OK;
	nRet = MV_GIGE_ForceIpEx(mhandle, nIP, nSubNetMask, nDefaultGateWay);
	CHECKInsResult(nRet);

	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::SetIpConfig(unsigned int _nType)
{
	int nRet = MV_OK;

	MV_GIGE_SetIpConfig(mhandle, _nType);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::set_trigger_mode(int _index)
{
	int nRet = MV_OK;
	if (_index == 0)
	{
		// ch:设置触发模式为off | en:Set trigger mode as off
		nRet = MV_CC_SetEnumValue(mhandle, "TriggerMode", 0);
		CHECKInsResult(nRet);
	} 
	else if (_index == 1)
	{
		// ch:设置软触发模式 | en:Set Trigger Mode and Set Trigger Source
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerMode", "On");
		CHECKInsResult(nRet);
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerSource", "Software");
		CHECKInsResult(nRet);
	}
	else if(_index == 2)
	{
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerMode", "On");
		CHECKInsResult(nRet);
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerSource", "Line0");
		CHECKInsResult(nRet);
	}
	else if (_index == 3)
	{
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerMode", "On");
		CHECKInsResult(nRet);
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerSource", "Line2");
		CHECKInsResult(nRet);
	}
	else if (_index == 4)
	{
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerMode", "On");
		CHECKInsResult(nRet);
		nRet = MV_CC_SetEnumValueByString(mhandle, "TriggerSource", "Counter0");
		CHECKInsResult(nRet);
	}

	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::send_soft_trigger_sig()
{
	int nRet = MV_OK;
	nRet = MV_CC_SetCommandValue(mhandle, "TriggerSoftware");
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::SetImageNodeNum(unsigned int nNum)
{
	int nRet = MV_OK;
	nRet = MV_CC_SetImageNodeNum(mhandle, nNum);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}


//static  unsigned int __stdcall WorkThread(void* pUser)
//{
//	int nRet = MV_OK;
//	MV_FRAME_OUT stOutFrame = { 0 };
//
//	while (true)
//	{
//		nRet = MV_CC_GetImageBuffer(pUser, &stOutFrame, 1000);
//		if (nRet == MV_OK)
//		{
//			printf("Get Image Buffer: Width[%d], Height[%d], FrameNum[%d]\n",
//				stOutFrame.stFrameInfo.nWidth, stOutFrame.stFrameInfo.nHeight, stOutFrame.stFrameInfo.nFrameNum);
//
//			nRet = MV_CC_FreeImageBuffer(pUser, &stOutFrame);
//			if (nRet != MV_OK)
//			{
//				printf("Free Image Buffer fail! nRet [0x%x]\n", nRet);
//			}
//		}
//		else
//		{
//			printf("Get Image fail! nRet [0x%x]\n", nRet);
//		}
//		if (g_bExit)
//		{
//			break;
//		}
//	}
//
//	return 0;
//}


InsResult MvsCamera::GrabContinous()
{
	int nRet = MV_OK;
	// ch:开始取流 | en:Start grab image
	nRet = MV_CC_StartGrabbing(mhandle);
	CHECKInsResult(nRet);

	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::GetImageBuffer(FRAME_OUT* pstFrame, unsigned int nMsec)
{
	int nRet = MV_OK;
	nRet = MV_CC_GetImageBuffer(mhandle, (MV_FRAME_OUT *) pstFrame, nMsec);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}
InsResult MvsCamera::FreeImageBuffer(FRAME_OUT* pstFrame)
{
	int nRet = MV_OK;
	nRet = MV_CC_FreeImageBuffer(mhandle, (MV_FRAME_OUT*)pstFrame);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}


InsResult MvsCamera::GrabStop()
{
	int nRet = MV_OK;
	nRet = MV_CC_StopGrabbing(mhandle);
	CHECKInsResult(nRet);

	return InsResult::INSCSTATUS_OK;
}
InsResult MvsCamera::Close()
{
	// ch:关闭设备 | Close device
	int nRet = MV_OK;

	nRet = MV_CC_CloseDevice(mhandle);
	if (MV_OK != nRet)
		CHECKInsResult(nRet);

	// ch:销毁句柄 | Destroy handle
	nRet = MV_CC_DestroyHandle(mhandle);
	if (nRet != MV_OK)
	{
		if (mhandle != NULL)
		{
			MV_CC_DestroyHandle(mhandle);
			mhandle = NULL;
		}
	}
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}




InsResult MvsCamera::RegisterImageCallBack(DataPocessCallBack _callback)
{
	int nRet = MV_OK;
	// ch:注册抓图回调 | en:Register image callback
	nRet = MV_CC_RegisterImageCallBackEx(mhandle, ImageCallBackEx, this);
	CHECKInsResult(nRet);
	mcallback = _callback;
	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::AntiRegisterImageCallBack()
{
	// ch:注销抓图回调 | en:Unregister image callback
	int nRet = MV_OK;
	nRet = MV_CC_RegisterImageCallBackEx(mhandle, NULL, NULL);
	CHECKInsResult(nRet);
	mcallback = NULL;
	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::set_ins_width(int _ins_width)
{
	int nRet = MV_OK;
	// Get value of Integer nodes. Such as, 'width' etc.
	MVCC_INTVALUE stIntVal;
	memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_SetWidth(mhandle, _ins_width);
	if (MV_OK != nRet)
	{
		return (InsResult)nRet;
	}
	return InsResult::INSCSTATUS_OK;
}
InsResult MvsCamera::set_ins_height(int _ins_height)
{
	int nRet = MV_OK;
	// Get value of Integer nodes. Such as, 'width' etc.
	MVCC_INTVALUE stIntVal;
	memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_SetHeight(mhandle, _ins_height);
	if (MV_OK != nRet)
	{
		return (InsResult)nRet;
	}
	return InsResult::INSCSTATUS_OK;
}
int MvsCamera::get_ins_width()//11.13 错误返回int值最大值
{
	int nRet = MV_OK;
	// Get value of Integer nodes. Such as, 'width' etc.
	MVCC_INTVALUE stIntVal;
	memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetWidth(mhandle, &stIntVal);
	if (MV_OK != nRet)
	{
		return INT_MAX;
	}
	return stIntVal.nCurValue;
}
int MvsCamera::get_ins_height()//11.13 错误返回int值最大值
{
	int nRet = MV_OK;
	MVCC_INTVALUE stIntVal;
	memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetHeight(mhandle, &stIntVal);
	if (MV_OK != nRet)
	{
		return INT_MAX;
	}
	return stIntVal.nCurValue;
}

int MvsCamera::get_auto_exposure_time_lower()
{
	int nRet = MV_OK;
	MVCC_INTVALUE stIntVal;
	memset(&stIntVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetAutoExposureTimeLower(mhandle, &stIntVal);
	if (MV_OK != nRet)
	{
		return INT_MAX;
	}
	return stIntVal.nCurValue;
}

InsResult MvsCamera::set_ins_exposure(double _val)
{
	int nRet = MV_OK;
	nRet = MV_CC_SetExposureTime(mhandle, (float)_val);
	CHECKInsResult(nRet);
	ins_exposure_ = _val;
	return InsResult::INSCSTATUS_OK;

}
double MvsCamera::get_ins_exposure()
{
	/*
	int nRet = MV_OK;
	MVCC_FLOATVALUE stflVal;
	memset(&stflVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetExposureTime(mhandle, &stflVal);
	if (MV_OK != nRet)
	{
		return DBL_MAX;
	}
	ins_exposure_ = (double)stflVal.fCurValue;
	return (double)stflVal.fCurValue;
	*/
	return 0;
}

InsResult MvsCamera::set_ins_digital_gain(double _val)
{
	int nRet = MV_OK;
	nRet = MV_CC_SetGain(mhandle, (float)_val);
	CHECKInsResult(nRet);
	ins_digital_gain_ = _val;
	return InsResult::INSCSTATUS_OK;
}
double MvsCamera::get_ins_digital_gain()
{
	/*
	int nRet = MV_OK;
	MVCC_FLOATVALUE stflVal;
	memset(&stflVal, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetGain(mhandle, &stflVal);
	if (MV_OK != nRet)
	{
		return DBL_MAX;
	}
	ins_digital_gain_ = (double)stflVal.fCurValue;
	return (double)stflVal.fCurValue;
	*/
	return 0;
}

unsigned int MvsCamera::get_pixel_format()
{
	int nRet = MV_OK;
	MVCC_ENUMVALUE* pstEnumValue = NULL;
	nRet = MV_CC_GetEnumValue(mhandle, "PixelFormat", pstEnumValue);
	if (MV_OK != nRet)
		return PixelType::PixelType_Undefined;
	return pstEnumValue->nCurValue;
}
InsResult MvsCamera::set_pixel_format(unsigned int _pixel_type)
{
	int nRet = MV_OK;
	MVCC_ENUMVALUE* pstEnumValue;
	nRet = MV_CC_SetEnumValue(mhandle, "PixelFormat", _pixel_type);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}


InsResult MvsCamera::SaveCameraConfigurationToFile(const char* _filename)//保存
{
	std::string temp(_filename);
	size_t pos = temp.find(".ini");
	if (pos!= std::string::npos && pos == temp.length() - 4)
	{
		const char* ch = temp.c_str();

		// Read mode
		//int nRet = MV_CC_FeatureSave(mhandle, ch);
		int nRet = MV_CC_FeatureSave(mhandle, ch);
		CHECKInsResult(nRet);
	}
	else
	{
		return InsResult::INSSTATUS_INPUT_ERROR;
	}
	return InsResult::INSCSTATUS_OK;
}


InsResult MvsCamera::ConfigureFileToCamera(const char* _filename)//加载
{
	std::string temp(_filename);
	size_t pos = temp.find(".ini");
	if (pos != std::string::npos && pos == temp.length() - 4)
	{
		const char* ch = temp.c_str();

		// Read mode
		int nRet = MV_CC_FeatureLoad(mhandle, ch);
		CHECKInsResult(nRet);
	}
	else
	{
		return InsResult::INSSTATUS_INPUT_ERROR;
	}
	return InsResult::INSCSTATUS_OK;
}

InsResult MvsCamera::XML_GetGenICamXML( IN OUT unsigned char* pData, IN unsigned int nDataSize, OUT unsigned int* pnDataLen)
{
	int nRet = MV_XML_GetGenICamXML(mhandle, pData,  nDataSize , pnDataLen);
	CHECKInsResult(nRet);
	return InsResult::INSCSTATUS_OK;
}

