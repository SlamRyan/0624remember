#include <iomanip>
#include <string>
#include <fstream>
#include <map>
#include "Shlobj.h"
#include <tchar.h>
#include "../../third_party/IkapIncludes/IKapBoard.h"
#include "../../third_party/IkapIncludes/IKapC.h"
#include "../../third_party/IkapIncludes/IKapCType.h"
#include "../../third_party/IkapIncludes/IKapCDef.h"
#include "../../third_party/IkapIncludes/IKapCStat.h"
#include"../include/Ins_Camera_proc.h"
#include "..\include\modbus_udp.h"
//和触发有关的只写一块板，和条纹有关的要写三块版
int PicNumCount = 8;//取相数量

//ret 返回1正确，res返回0正确
#pragma comment (lib, "IKapBoard.lib")
#pragma comment( lib, "shlwapi.lib" )
using namespace std;

static DataPocessCallBack mCallback;
static OffLineCallBack removeCallback;
//static void mCallback(unsigned char *);(申明了函数但是不是变量)



/* @brief：打印错误信息后退出程序。
 * @param[in] InsResult：函数返回值。
 *
 * @brief：Print error message and exit the program.
 * @param[in] InsResult：Function return value. */
extern "C" INSCAMERA_API void printErrorAndExit(ITKSTATUS InsResult);

/* @brief：打印 IKapBoard 错误信息后退出程序。*/
extern "C" INSCAMERA_API void printIKapBoardErrorAndExit();

/* @brief：打印 IKapC 错误信息后退出程序。
 * @param[in] InsResult：函数返回值。 */
void printIKapCErrorAndExit(ITKSTATUS InsResult);

/* @brief：用户输入回车键后退出程序。 */
void pressEnterToExit(void);
/*
/* @brief：选择用户配置文件。
 * @param[in] lpConfigFileName：配置文件名。
bool GetOption(char* lpConfigFileName);
bool GetCCFOption(char* lpConfigFileName);
*/
/* @brief：配置相机设备。
 * @param[out] hCamera：相机设备句柄。
 * @param[out] hBoard：采集卡设备句柄。 */
extern "C" INSCAMERA_API InsResult ConfigureCamera(ITKDEVICE * hCamera, HANDLE * hBoard);

/* @brief：配置采集卡设备。
 * @param[in] hCamera：相机设备句柄。
 * @param[in] hBoard：采集卡设备句柄。*/
extern "C" INSCAMERA_API void ConfigureFrameGrabber(ITKDEVICE hCamera, HANDLE hBoard);

/* @brief：清除回调函数。
 *
 * @brief：Unregister callback functions. */
void UnRegisterCallback();

/* @brief：本函数被注册为一个回调函数。当图像采集开始时，函数被调用。
 * @param[in] pContext：输入参数。
 *
 * @brief：This function is registered as a callback function. When starting grabbing images, the function will be called.
 * @param[in] pContext：Input parameter. */
void CALLBACK OnGrabStart(void* pContext);

/* @brief：本函数被注册为一个回调函数。当一帧图像采集完成时，函数被调用。
 * @param[in] pContext：输入参数。
 *
 * @brief：This function is registered as a callback function. When a frame of image grabbing ready, the function will be called.
 * @param[in] pContext：Input parameter. */
void CALLBACK OnFrameReady(void* pContext);

/* @brief：本函数被注册为一个回调函数。当图像采集超时时，函数被调用。
 * @param[in] pContext：输入参数。
 *
 * @brief：This function is registered as a callback function. When grabbing images time out, the function will be called.
 * @param[in] pContext：Input parameter. */
void CALLBACK OnTimeout(void* pContext);

/* @brief：本函数被注册为一个回调函数。当采集丢帧时，函数被调用。
 * @param[in] pContext：输入参数。
 *
 * @brief：This function is registered as a callback function. When grabbing frame lost, the function will be called.
 * @param[in] pContext：Input parameter. */
void CALLBACK OnFrameLost(void* pContext);

/* @brief：本函数被注册为一个回调函数。当图像采集停止时，函数被调用。
 * @param[in] pContext：输入参数。
 *
 * @brief：This function is registered as a callback function. When stopping grabbing images, the function will be called.
 * @param[in] pContext：Input parameter. */

 /* @brief：判断 IKapBoard 函数是否成功调用。
  * @param[in] InsResult：函数返回值。
  */
#define CHECK_IKAPBOARD(InsResult) if (InsResult != 1) printIKapBoardErrorAndExit()
  /* @brief：判断 IKapC 函数是否成功调用。
   * @param[in] InsResult：函数返回值。
   */
#define CHECK_IKAPC(InsResult) if (InsResult != ITKSTATUS_OK) printIKapCErrorAndExit(InsResult)

   /* @brief：判断函数是否成功调用。
	* @param[in] InsResult：函数返回值。
	*
	* @brief：Determine whether the function is called successfully.
	* @param[in] InsResult：Function return value. */
#define CHECK(InsResult) if (ITKSTATUS_OK != InsResult) printErrorAndExit(InsResult)






void CALLBACK OnGrabStop(void* pContext);





int InsCameraIkap::ins_width_ = 8200;//12.11原4096  //0322
int InsCameraIkap::ins_height_ = 40000;//12.11原40000
int InsCameraIkap::ins_type_ = 0;
double InsCameraIkap::ins_exposure_ = 0;
int InsCameraIkap::ins_frame_count_ = 0;
char* InsCameraIkap::ins_PAC_gain_ = (char*)malloc(4);
double InsCameraIkap:: ins_digital_gain_ = 0;
int InsCameraIkap::ins_trigger_mode_ = 0;//IKP_RCV_MORE_DATA_IN_TRIGGER_MODE

int InsCameraIkap::ins_st_width_ = 0;//IKP_SOFTWARE_TRIGGER_WIDTH
int InsCameraIkap::ins_st_period_ = 0;//IKP_SOFTWARE_TRIGGER_PERIOD
int InsCameraIkap::ins_st_count_ = 0;//IKP_SOFTWARE_TRIGGER_COUNT
int InsCameraIkap::ins_st_delay_ = 0;//IKP_SOFTWARE_TRIGGER_DELAY
int InsCameraIkap::ins_st_polarity_ = 0;// IKP_SFOTWARE_TRIGGER_POLARITY
int InsCameraIkap::ins_st_sync_mode_ = 0;//IKP_SOFTWARE_TRIGGER_SYNC_MODE

int InsCameraIkap::ins_io_trigger_mode_ = 0;
double InsCameraIkap::ins_line_period_time_ = 0;
int InsCameraIkap::ins_io_polarity_ = 0;
int InsCameraIkap::ins_io_min_width_ = 0;

int InsCameraIkap::ins_strobe_source_ = 0;
int InsCameraIkap::ins_strobe_method_ = 0;
int InsCameraIkap::ins_strobe_polarity_ = 0;
int InsCameraIkap::ins_strobe_delay_ = 0;
int InsCameraIkap::ins_strobe_width_ = 0;

int InsCameraIkap::ins_et_channel_ = 0;// IKP_SHAFT_ENCODER1_CHANNEL
int InsCameraIkap::ins_et_multi_factor_ = 0;//IKP_SHAFT_ENCODER1_MULTIPlY_FACTOR
int InsCameraIkap::ins_et_division_factor_ = 0;
int InsCameraIkap::ins_et_min_width_ = 0;//IKP_SHAFT_ENCODER1_MIN_WIDTH
int InsCameraIkap::ins_et_valid_direction_ = 0;// IKP_SHAFT_ENCODER1_VALID_DIRECTION
int InsCameraIkap::ins_et_reverse_comp_ = 0;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION
int InsCameraIkap::ins_et_quad_frequency_source_ = 0;//IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE
int InsCameraIkap::ins_et_reverse_compensation_limit_ = 0;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_LIMIT
int InsCameraIkap::ins_et_clock_duty_compensation_type_ = 0;//IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_TYPE
int InsCameraIkap::ins_et_clock_duty_compensation_width_ = 0;// IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_WIDTH

bool InsCameraIkap::cam_update_ = 0;//第一次刷新参数
uint32_t InsCameraIkap::frame_id_ = 0;
int InsCameraIkap::ins_frame_size_ = 0;
int InsCameraIkap::ins_shaft_encoder1_min_width_ = 300;//12.11原300

//0322 

int InsCameraIkap::mi_camera = 0;


const int Zeroz = 0;
const int Onez = 1;
const int Twoz = 2;
const int Threez = 3;
const int Fourz = 4;
const int Fivez = 5;
const int Sixz = 6;
const int Sevenz = 7;

/* @brief：判断 IKapBoard 函数是否成功调用。
 * @param[in] InsResult：函数返回值。
 *
 * @brief：Determine whether the IKapBoard function is called successfully.
 * @param[in] InsResult：Function return value. */
#define CHECK_IKAPBOARD(x) if (x != 1) printIKapBoardErrorAndExit()

 /* @brief：判断 IKapC 函数是否成功调用。
  * @param[in] InsResult：函数返回值。
  *
  * @brief：Determine whether the IKapC function is called successfully.
  * @param[in] InsResult：Function return value. */
#define CHECK_IKAPC(x) if (x != ITKSTATUS_OK) printIKapCErrorAndExit(x)

#define RETURNERRC(x) if (x != ITKSTATUS_OK) return((InsResult)(x & 0xffff))
#define RETURNERRCBOARD(x) if ((x|0x1000) != (0x1000 | 0x00000001)) return((InsResult)(x|0x1000))



INSCAMERA_API void HelloCamera()
{
	std::cout << "Hello, Camera!" << std::endl;
}
vector<string> full_name_;
vector<string> device_class_;
vector<string> SN_;
map<int, int> ins_num_realist_index;//存找到的INS相机序号和对应的实际相机序号,ins的相机序号从1开始
InsResult FindCamera(int& _num)
{
	{
		ITKDEV_INFO di;
		uint32_t num_devices_ = 0;
		_num = 0;
		ITKSTATUS res = ITKSTATUS_OK;
		printf("INS Console Find Camera\n");
		res = ItkManInitialize();
		CHECK(res);
		RETURNERRC(res);

		// 枚举可用设备的数量。在打开设备前，必须调用 ItkManGetDeviceCount() 函数。
		//
		// Enumerate the number of available devices. Before opening the device, ItkManGetDeviceCount() function must be called.
		res = ItkManGetDeviceCount(&num_devices_);
		CHECK(res);
		RETURNERRC(res);

		// 当没有连接的设备时。
		//
		// When there is no connected devices.
		if (num_devices_ == 0)
		{
			fprintf(stderr, "No device.\n");
			printf("No camera.\n");
			return(InsResult(0x0032));
		}

		// 打印连接的相机设备的信息
		uint32_t i = 0;
		for (; i < num_devices_; ++i)
		{
			ItkManGetDeviceInfo(i, &di);
			//if (di == 0)
			//        continue;
			if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
			{
				std::string str;
				int size = strlen(di.FullName);

				//ITKDEV_INFO_ENTRY_MAX_LENGTH;
				str.resize(size);

				memcpy((void*)str.data(), di.FullName, str.size());
				//string a = std::string(di->FullName);
				string pl = "PL";
				size_t found = str.find(pl);
				cout << found;
				// 如果找到 "PL"，则进行替换
				if (found != std::string::npos)
				{
					//cout << "找到了PL";
					str.replace(found, 2, "INS-LC-");
					//cout << "替换了PL";
				}

				size_t ktFound = str.find("KT");
				if (ktFound != std::string::npos)
				{
					str.replace(ktFound, 2, "M");
				}

				//到int InsCamera::ins_shaft_encoder1_min_width_ CXP新增
				size_t PHFound = str.find("PN");
				if (PHFound != std::string::npos)
				{
					str.replace(PHFound, 2, "INS-LC-");
				}
				full_name_.push_back(str);

				std::string str1;
				int size1 = strlen(di.DeviceClass);
				str1.resize(size1);

				memcpy((void*)str1.data(), di.DeviceClass, str1.size());
				device_class_.push_back(str1);
				//保存SN
				size1 = strlen(di.SerialNumber);
				str1.resize(size1);
				memcpy((void*)str1.data(), di.SerialNumber, str1.size());
				SN_.push_back(str1);
				_num ++;
				ins_num_realist_index.insert(make_pair(_num, i));
			}

			if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && (strcmp(di.VendorName, "itek-opto electronics") == 0|| strcmp(di.VendorName, "I-Tek OptoElectronics") == 0))
			{
				std::string str;
				int size = strlen(di.FullName);

				//ITKDEV_INFO_ENTRY_MAX_LENGTH;
				str.resize(size);

				memcpy((void*)str.data(), di.FullName, str.size());
				//string a = std::string(di->FullName);
				string pl = "PL";
				size_t found = str.find(pl);
				cout << found;
				// 如果找到 "PL"，则进行替换
				if (found != std::string::npos)
				{
					//cout << "找到了PL";
					str.replace(found, 2, "INS-LC-");
					//cout << "替换了PL";
				}

				size_t ktFound = str.find("KT");
				if (ktFound != std::string::npos)
				{
					str.replace(ktFound, 2, "M");
				}

				//到int InsCamera::ins_shaft_encoder1_min_width_ CXP新增
				size_t PHFound = str.find("PN");
				if (PHFound != std::string::npos)
				{
					str.replace(PHFound, 15, "INS-LC-8KCXP-220M");
				}


				size_t PAFound = str.find("PA");//0407
				if (PAFound != std::string::npos)
				{
					str.replace(PAFound, 13, "INS-LC-16KCXP-80M");//0409
				}

				//cout << "str" << str << endl;
				full_name_.push_back(str);

				std::string str1;
				int size1 = strlen(di.DeviceClass);
				//ITKDEV_INFO_ENTRY_MAX_LENGTH;
				str1.resize(size1);

				memcpy((void*)str1.data(), di.DeviceClass, str1.size());
				//string a = std::string(di->FullName);
				device_class_.push_back(str1);

				//保存SN
				size1 = strlen(di.SerialNumber);
				str1.resize(size1);
				memcpy((void*)str1.data(), di.SerialNumber, str1.size());
				SN_.push_back(str1);
				_num++;
				ins_num_realist_index.insert(make_pair(_num, i));
			}
		}
		return InsResult::INSCSTATUS_OK;
	}
}
InsResult GetCameraDeviceInfo(int _num, InsCameraInfo& _insCameraInfo)
{
	if ((_num  <= full_name_.size()) && (_num  <= device_class_.size())&& (_num >0))
	{
		_insCameraInfo.device_class_= device_class_[_num - 1].c_str();
		_insCameraInfo.divice_full_name_= full_name_[_num - 1].c_str();
		strcpy(_insCameraInfo.SN_, SN_[_num - 1].c_str());
		return (InsResult)0;
	}
	else
		return InsResult::INSCSTATUS_INVALID_ARG;
}
void InsCameraIkap::enumAllFeatures()
{
	ITKSTATUS res = ITKSTATUS_OK;

	// 设备特征数量。
	//
	// The number of device features.
	uint32_t  nFeatureCount = 0;

	// 特征名称。
	//
	// Feature name.
	char featureName[128];

	// 特征名称长度。
	//
	// The length of feature name.
	uint32_t featureNameLen = 128;

	// 特征类型。
	//
	// The type of feature.
	uint32_t featureType = ITKFEATURE_VAL_TYPE_UNDEFINED;

	// 获取设备特征数量。
	//
	// Get the number of device features.
	res = ItkDevGetFeatureCount(ins_hCamera, &nFeatureCount);
	CHECK(res);

	for (uint32_t i = 0; i < nFeatureCount; i++)
	{
		featureNameLen = 128;

		// 获取特征名称。
		//
		// Get the name of feature.
		res = ItkDevGetFeatureName(ins_hCamera, i, featureName, &featureNameLen);
		CHECK(res);

		// 获取特征类型。
		//
		// Get the type of feature.
		res = ItkDevGetType(ins_hCamera, featureName, &featureType);
		CHECK(res);

		printf("Feature Name : %s    feature type : %d\n", featureName, featureType);
	}
}

InsResult ConfigureCamera(ITKDEVICE* hCamera, HANDLE* hBoard)
{
	ITKSTATUS res = ITKSTATUS_OK;
	uint32_t numCameras = 0;

	// 枚举可用相机的数量。在打开相机前，必须调用 ItkManGetDeviceCount() 函数。
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// 当没有连接的相机时。
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// 打开CameraLink相机。
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// 获取相机设备信息。
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);
		
		CHECK_IKAPC(res);
		RETURNERRC(res);
		printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);
		// 当设备为 CameraLink 相机且序列号正确时。
		if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
		{
			ITK_CL_DEV_INFO cl_board_info;
			// 打开相机。
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, hCamera);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// 获取 CameraLink 相机设备信息。
			//
			// Get CameraLink camera device information.
			res = ItkManGetCLDeviceInfo(i, &cl_board_info);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// 打开采集卡。
			//
			// Open frame grabber.
			*hBoard = IKapOpen(cl_board_info.HostInterface, cl_board_info.BoardIndex);
			if (*hBoard == INVALID_HANDLE_VALUE) {
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
				
			//CXP新增
			return InsResult::INSCSTATUS_OK;
		}

		if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "itek-opto electronics") == 0)
		{

			ITK_CXP_DEV_INFO cxp_cam_info;
			IKAP_CXP_BOARD_INFO cxp_board_info;

			// 打开相机。
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, hCamera);
			CHECK_IKAPC(res);

			// 获取CoaXPress相机设备信息。
			//
			// Get CoaXPress camera device information.
			res = ItkManGetCXPDeviceInfo(i, &cxp_cam_info);
			CHECK_IKAPC(res);

			// 打开采集卡。
			//
			// Open frame grabber.
			memcpy(&cxp_board_info, &cxp_cam_info, sizeof cxp_cam_info);
			*hBoard = IKapOpenCXP(IKBoardPCIE, cxp_board_info.BoardIndex, cxp_board_info);
			if (*hBoard == INVALID_HANDLE_VALUE)
			{
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
			//enumAllFeatures();
			return InsResult::INSCSTATUS_OK;
		}
	}
	return InsResult::INSCSTATUS_NOT_CAMERALINK_CAMERA_OR_SERIAL_NUMBER_NOT_PROPER;
}

InsCameraIkap::InsCameraIkap()
{    // 相机设备句柄。
	ins_hCamera = NULL;

	// 采集卡设备句柄。
	ins_hBoard = INVALID_HANDLE_VALUE;
}


InsResult InsCameraIkap::PrintCamera()
{
	ITKSTATUS res = ITKSTATUS_OK;
	uint32_t numCameras = 0;

	// 枚举可用相机的数量。在打开相机前，必须调用 ItkManGetDeviceCount() 函数。
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	cout << "numCameras:" << numCameras<<endl;
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// 当没有连接的相机时。
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// 打开CameraLink相机。
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// 获取相机设备信息。
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);
		CHECK_IKAPC(res);
		RETURNERRC(res);
		cout << "第"<< i << "台设备:" <<endl;
		printf("Using camera: serial: %s, name: %s, interface: %s, DeviceVersion: %s, FriendlyName: %s, ModelName: %s, UserDefinedName: %s, VendorName: %s.\n", 
			di.SerialNumber, di.FullName, di.DeviceClass,di.DeviceVersion,di.FriendlyName,di.ModelName,di.UserDefinedName,di.VendorName);
		// 当设备为 CameraLink 相机且序列号正确时。
		}
	return InsResult::INSCSTATUS_OK;
}






// 配置采集卡设备。
//
// Configure frame grabber device.
InsResult InsCameraIkap::ConfigureFrameGrabber()
{
	int ret = IK_RTN_OK;
	 // 设置图像缓冲区帧数。
	 //
	 // Set frame count of buffer.
	ret = IKapSetInfo(ins_hBoard, IKP_FRAME_COUNT, BOARD_FRMAE_COUNT);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 设置超时时间。
	//
	// Set time out time.
	int timeout = -1;
	ret = IKapSetInfo(ins_hBoard, IKP_TIME_OUT, timeout);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 设置采集模式。
	//
	// Set grab mode.
	int grab_mode = IKP_GRAB_NON_BLOCK;
	ret = IKapSetInfo(ins_hBoard, IKP_GRAB_MODE, grab_mode);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);

	// 设置传输模式。
	//
	// Set transfer mode.
	int transfer_mode = IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT;
	ret = IKapSetInfo(ins_hBoard, IKP_FRAME_TRANSFER_MODE, transfer_mode);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 注册回调函数
	//
	// Register callback functions.
	ret = IKapRegisterCallback(ins_hBoard, IKEvent_GrabStart, OnGrabStart, ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	//ret = IKapRegisterCallback(ins_hBoard, IKEvent_FrameReady, OnFrameReady, ins_hBoard);
	//CHECK_IKAPBOARD(ret);
	//RETURNERRCBOARD(ret);
	ret = IKapRegisterCallback(ins_hBoard, IKEvent_FrameLost, OnFrameLost, ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	ret = IKapRegisterCallback(ins_hBoard, IKEvent_TimeOut, OnTimeout, ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	ret = IKapRegisterCallback(ins_hBoard, IKEvent_GrabStop, OnGrabStop, ins_hBoard);
	CHECK_IKAPBOARD(ret);
	return(InsResult::INSSTATUS_SUCCESS);
}

void StartGrabImage(ITKDEVICE hCamera, HANDLE hBoard)
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;

	res = ItkDevExecuteCommand(hCamera, "AcquisitionStop");
	CHECK_IKAPC(res);

	ret = IKapStartGrab(hBoard, 0);
	CHECK_IKAPBOARD(ret);

	res = ItkDevExecuteCommand(hCamera, "AcquisitionStart");
	CHECK_IKAPC(res);
}
// 配置采集卡设备。
//
// Configure frame grabber device.
void ConfigureFrameGrabberIn(ITKDEVICE hCamera, HANDLE hBoard)
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	/*
	char configFilename[MAX_PATH];

	// 导入配置文件。
	//
	// Load configuration file.
	if (GetOption(configFilename) == false)
	{
		printf("Fail to get configuration, using default setting!\n");
	}
	else
	{
		ret = IKapLoadConfigurationFromFile(hBoard, configFilename);
		CHECK_IKAPBOARD(ret);
	}
	*/
	// 设置图像缓冲区帧数。
	//
	// Set frame count of buffer.
	ret = IKapSetInfo(hBoard, IKP_FRAME_COUNT, BOARD_FRMAE_COUNT);
	CHECK_IKAPBOARD(ret);

	// 设置超时时间。
	//
	// Set time out time.
	int timeout = -1;
	ret = IKapSetInfo(hBoard, IKP_TIME_OUT, timeout);
	CHECK_IKAPBOARD(ret);

	// 设置采集模式。
	//
	// Set grab mode.
	int grab_mode = IKP_GRAB_NON_BLOCK;
	ret = IKapSetInfo(hBoard, IKP_GRAB_MODE, grab_mode);
	CHECK_IKAPBOARD(ret);

	// 设置传输模式。
	//
	// Set transfer mode.
	int transfer_mode = IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT;
	ret = IKapSetInfo(hBoard, IKP_FRAME_TRANSFER_MODE, transfer_mode);
	CHECK_IKAPBOARD(ret);

	// 注册回调函数
	//
	// Register callback functions.
	ret = IKapRegisterCallback(hBoard, IKEvent_GrabStart, OnGrabStart, hBoard);
	CHECK_IKAPBOARD(ret);
	ret = IKapRegisterCallback(hBoard, IKEvent_FrameLost, OnFrameLost, hBoard);
	CHECK_IKAPBOARD(ret);
	ret = IKapRegisterCallback(hBoard, IKEvent_TimeOut, OnTimeout, hBoard);
	CHECK_IKAPBOARD(ret);
	ret = IKapRegisterCallback(hBoard, IKEvent_GrabStop, OnGrabStop, hBoard);
	CHECK_IKAPBOARD(ret);
}





// 打印 IKapBoard 错误信息后退出程序。
//
// Print IKapBoard error message and exit the program.
void printIKapBoardErrorAndExit()
{
	// 获取错误码信息。
	//
	// Get error code message.
	IKAPERRORINFO InsResult;
	IKapGetLastError(&InsResult, 1);

	// 打印错误信息。
	//
	// Print error message.
	fprintf(stderr, "INSBoard:Index: %d, error code:%04x\n", InsResult.uBoardIndex, InsResult.uErrorCode);
	std::cout<<stderr<< "INSBoard:Index: %d, error code:%04x\n"<< InsResult.uBoardIndex<< InsResult.uErrorCode;

	//pressEnterToExit();
	//exit(EXIT_FAILURE);
}

// 打印 IKapC 错误信息后退出程序。
//
// Print IKapC error message and exit the program.
void printIKapCErrorAndExit(ITKSTATUS InsResult)
{
	fprintf(stderr, "INSC error code: %08X", InsResult);
	//ItkManTerminate();
	//pressEnterToExit();
	//exit(EXIT_FAILURE);
}

void log_message(const std::string& message) 
{
	std::ofstream log(LOG_FILE, std::ios_base::app);
	std::time_t now = std::time(nullptr);
	log << std::ctime(&now) << ": " << message << std::endl;
}




// 清除回调函数。
//
// Unregister callback functions.
void InsCameraIkap::UnRegisterCallback()
{
	int ret = IK_RTN_OK;

	ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_GrabStart);
	//ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_FrameReady);
	ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_FrameLost);
	ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_TimeOut);
	ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_GrabStop);
}

// 用户输入回车键后退出程序。
//
// Users enter Enter to exit the program.
void pressEnterToExit(void)
{
	fprintf(stderr, "\nPress enter to exit.\n");
	while (getchar() != '\n');
}

// 本函数被注册为一个回调函数。当图像采集开始时，函数被调用。
// 
// This function is registered as a callback function. When starting grabbing images, the function will be called.
void CALLBACK OnGrabStart(void* pContext)
{
	printf("Start grabbing image.\n");
}

// 本函数被注册为一个回调函数。当一帧图像采集完成时，函数被调用。
// 
// This function is registered as a callback function. When a frame of image grabbing ready, the function will be called.
void CALLBACK InsCameraIkap::OnFrameReady(void* pContext)
{
	printf("Grab frame ready.\n");

	HANDLE hDev = (HANDLE)pContext;
	unsigned char* pUserBuffer = NULL;
	int nFrameSize = 0;
	int nFrameCount = 0;
	int nFrameIndex = 0;
	int nDataFormat = 0;
	int nImageType = 0;
	int nChannels = 0;
	int nBitDepth = 0;

	IKAPBUFFERSTATUS status;

	//0202
	//IKapGetInfo(hDev, IKP_FRAME_COUNT, &nFrameCount);
	//InsCameraIkap::ins_frame_count_ = nFrameCount;
	IKapGetInfo(hDev, IKP_CURRENT_BUFFER_INDEX, &nFrameIndex);
	IKapGetBufferStatus(hDev, nFrameIndex, &status);
	
	if (!InsCameraIkap::cam_update_ == true)
	{
		//InsCameraIkap::getinfo();//0202
		//IKapGetInfo(hDev,IKP_SHAFT_ENCODER1_DIVISION_FACTOR, &nValue);
		InsCameraIkap::cam_update_ = true;
	}

	// 当图像缓冲区满时。
	//
	// When the buffer is full.
	if (status.uFull == 1)
	{
		/*//0202
		// 获取一帧图像的大小。
		//
		// Get the size of a frame of image.
		IKapGetInfo(hDev, IKP_FRAME_SIZE, &nFrameSize);
		InsCameraIkap::ins_frame_size_ = nFrameSize;
		cout << "InsCameraIkap::ins_frameSize:" << InsCameraIkap::ins_frame_size_ << endl;*/
		int nwidth = 0;
		int ret = IKapGetInfo(hDev, IKP_IMAGE_WIDTH, &nwidth);

		int nheight = 0;
		int ret2 = IKapGetInfo(hDev, IKP_IMAGE_HEIGHT, &nheight);

		// 获取缓冲区地址。
		//
		// Get the buffer address.
		IKapGetBufferAddress(hDev, nFrameIndex, (void**)&pUserBuffer);
		
		// 获取数据格式
		IKapGetInfo(hDev, IKP_DATA_FORMAT, &nDataFormat);
		
		// 获取图像类型
		IKapGetInfo(hDev, IKP_IMAGE_TYPE, &nImageType);


		InsCameraIkap::frame_id_++;
		InsPMDL_Buffer* insBuffer = new InsPMDL_Buffer;
		insBuffer->pdata = pUserBuffer;											//图像指针
		insBuffer->width = nwidth;											//图像宽度
		insBuffer->height = nheight;										//图像高度
		if (nImageType = 0)
		{
			nChannels = 1;
		}
		if (nImageType = 1)
		{
			nChannels = 3;
		}
		nBitDepth = nChannels * nDataFormat;
		int nBytesPerPixel = nBitDepth / 8;
		if (nBitDepth % 8 != 0)
			nBytesPerPixel++;
		insBuffer->step = nBytesPerPixel;										//图像步长，就是通道数×图像宽度，灰白是一通道，RGB彩色三通道，是位宽（像素深度）向八取整除以8
																				//IKapBoard用户开发手册14页
		insBuffer->imageTxpe = (INS_PMDL_PixelFormat)nImageType;				//图像像素格式,就是IKAP里的图像类型，mono8这种，	
																				
		/*  这些都没放进枚举
			INS_PMDL_IMAGE_TYPE_VAL_COLORFUL = 1,
			INS_PMDL_IMAGE_TYPE_VAL_RGBC = 2,
			INS_PMDL_IMAGE_TYPE_VAL_BGR = 3,
			INS_PMDL_IMAGE_TYPE_VAL_BGRC = 4,
			INS_PMDL_IMAGE_TYPE_VAL_YUV422 = 5,*/
		insBuffer->frameIndex = frame_id_;		  //图像 id
		//-------------------------
		//--------------------------------8k改好注掉
		//ITKDEV_INFO di;
		//ItkManGetDeviceInfo(mi_camera, &di);// to do
		//string namecam = di.FullName;
		//int res = 0;
		
		//if (namecam.find("8KCXP") != string::npos)
		//{
		//	insBuffer->pdata = pUserBuffer+ 2*nwidth; //前2后6
		//	insBuffer->height = nheight - 8;
		//}
		//else if (namecam.find("8KCL") != string::npos) 
		//{
		//	cout << "8KCL" << endl;
		//	insBuffer->pdata = pUserBuffer+ 4*nwidth; //前4后4
		//	insBuffer->height = nheight - 8;
		//}
		//---------------------

		//-----------------------------
		mCallback(insBuffer);
		delete insBuffer;
		insBuffer = nullptr;
	}
}

// 本函数被注册为一个回调函数。当图像采集超时时，函数被调用。
// 
// This function is registered as a callback function. When grabbing images time out, the function will be called.
void CALLBACK OnTimeout(void* pContext)
{
	printf("Grab image timeout.\n");
}

// 本函数被注册为一个回调函数。当采集丢帧时，函数被调用。
// 
// This function is registered as a callback function. When grabbing frame lost, the function will be called.
void CALLBACK OnFrameLost(void* pContext)
{
	printf("Grab frame lost.\n");
}

// 本函数被注册为一个回调函数。当图像采集停止时，函数被调用。
// 
// This function is registered as a callback function. When stopping grabbing images, the function will be called.
void CALLBACK OnGrabStop(void* pContext)
{
	printf("Stop grabbing image.\n");
}
/*
// 选择用户配置文件。
//
// Select configuration file.
bool GetOption(char* lpConfigFileName)
{
	OPENFILENAME iniofn;
	wchar_t	gcMsgBuf[MAX_PATH] = _T("\0");
	wchar_t lpTempFileName[_MAX_PATH] = _T("\0");

	GetModuleFileName(NULL, gcMsgBuf, MAX_PATH);
	PathRemoveFileSpec(gcMsgBuf);

	iniofn.lStructSize = sizeof(OPENFILENAME);
	iniofn.hwndOwner = NULL;
	iniofn.lpstrFilter = _T("Configuration File(*.vlcf)\0*.vlcf\0");
	iniofn.lpstrCustomFilter = NULL;
	iniofn.nFilterIndex = 1;
	iniofn.lpstrFile = lpTempFileName;
	iniofn.nMaxFile = MAX_PATH;
	iniofn.lpstrFileTitle = NULL;
	iniofn.nMaxFileTitle = MAX_PATH;
	iniofn.lpstrInitialDir = gcMsgBuf;
	iniofn.lpstrTitle = _T("The 'Vulcan Configuration File' of the purpose read is chosen.");
	iniofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	iniofn.lpstrDefExt = _T("vlcf");

	if (GetOpenFileName(&iniofn) == FALSE)
		return false;

	WideCharToMultiByte(CP_OEMCP, 0, lpTempFileName, (int)_tcsclen(lpTempFileName) + 1, lpConfigFileName, _MAX_PATH, NULL, NULL);

	return true;
}
*/
/*
// 选择用户配置文件。
//
// Select configuration file.
bool GetCCFOption(char* lpConfigFileName)
{
	OPENFILENAME iniofn;
	wchar_t	gcMsgBuf[MAX_PATH] = _T("\0");
	wchar_t lpTempFileName[_MAX_PATH] = _T("\0");

	GetModuleFileName(NULL, gcMsgBuf, MAX_PATH);
	PathRemoveFileSpec(gcMsgBuf);

	iniofn.lStructSize = sizeof(OPENFILENAME);
	iniofn.hwndOwner = NULL;
	iniofn.lpstrFilter = _T("Configuration File(*.ccf)\0*.ccf\0");
	iniofn.lpstrCustomFilter = NULL;
	iniofn.nFilterIndex = 1;
	iniofn.lpstrFile = lpTempFileName;
	iniofn.nMaxFile = MAX_PATH;
	iniofn.lpstrFileTitle = NULL;
	iniofn.nMaxFileTitle = MAX_PATH;
	iniofn.lpstrInitialDir = gcMsgBuf;
	iniofn.lpstrTitle = _T("The 'Vulcan Configuration File' of the purpose read is chosen.");
	iniofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	iniofn.lpstrDefExt = _T("ccf");

	if (GetOpenFileName(&iniofn) == FALSE)
		return false;

	WideCharToMultiByte(CP_OEMCP, 0, lpTempFileName, (int)_tcsclen(lpTempFileName) + 1, lpConfigFileName, _MAX_PATH, NULL, NULL);

	return true;
}
*/

InsResult InsCameraIkap::ConnectCamera()
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	res = ItkManInitialize();
	CHECK_IKAPC(res);
	if (res != ITKSTATUS_OK)
	{
		RETURNERRC(res);
	}
	uint32_t numCameras = 0;

	// 枚举可用相机的数量。在打开相机前，必须调用 ItkManGetDeviceCount() 函数。
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// 当没有连接的相机时。
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// 打开CameraLink相机。
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// 获取相机设备信息。
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);

		CHECK_IKAPC(res);
		RETURNERRC(res);
		printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);
		// 当设备为 CameraLink 相机且序列号正确时。
		if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
		{
			ITK_CL_DEV_INFO cl_board_info;
			// 打开相机。
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// 获取 CameraLink 相机设备信息。
			//
			// Get CameraLink camera device information.
			res = ItkManGetCLDeviceInfo(i, &cl_board_info);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// 打开采集卡。
			//
			// Open frame grabber.
			ins_hBoard = IKapOpen(cl_board_info.HostInterface, cl_board_info.BoardIndex);
			if (ins_hBoard == INVALID_HANDLE_VALUE) {
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
			mi_camera = i;
			CamFullName = di.FullName;
			//CXP新增
			return InsResult::INSCSTATUS_OK;
		}

		if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && (strcmp(di.VendorName, "itek-opto electronics") == 0|| strcmp(di.VendorName, "I-Tek OptoElectronics") == 0))
		{

			ITK_CXP_DEV_INFO cxp_cam_info;
			IKAP_CXP_BOARD_INFO cxp_board_info;

			// 打开相机。
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
			CHECK_IKAPC(res);

			// 获取CoaXPress相机设备信息。
			//
			// Get CoaXPress camera device information.
			res = ItkManGetCXPDeviceInfo(i, &cxp_cam_info);
			CHECK_IKAPC(res);

			// 打开采集卡。
			//
			// Open frame grabber.
			memcpy(&cxp_board_info, &cxp_cam_info, sizeof cxp_cam_info);
			ins_hBoard = IKapOpenCXP(IKBoardPCIE, cxp_board_info.BoardIndex, cxp_board_info);
			if (ins_hBoard == INVALID_HANDLE_VALUE)
			{
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
			enumAllFeatures();

			mi_camera = i;

			CamFullName = di.FullName;
			return InsResult::INSCSTATUS_OK;
		}


	
	}
	return InsResult::INSCSTATUS_NOT_CAMERALINK_CAMERA_OR_SERIAL_NUMBER_NOT_PROPER;
}



InsResult InsCameraIkap::ConnectIndexCamera(int _index)//采集卡   //0506 打印的是这个
{
	log_message("ConnectIndexCamera index is:" + std::to_string(_index));
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	res = ItkManInitialize();
	CHECK_IKAPC(res);
	if (res != ITKSTATUS_OK)
	{
		log_message("1: " + std::to_string(res));
		RETURNERRC(res);
	}
	//------------------------
	//ITKDEV_INFO di;
	//auto iter = ins_num_realist_index.find(_index);

	//if (iter != ins_num_realist_index.end())
	//{
	//	// 找到了指定键，可以使用 iter->second 访问对应的值
	//	// iter->first 是键，iter->second 是值
	//	_index = iter->second;
	//}
	//else 
	//{
	//	log_message("no map!!!!!!!!!!!!!!! " );
	//}
	//res = ItkManGetDeviceInfo(_index, &di);
	//---------------------------------
	//-------------------
	ITKDEV_INFO di;
	auto iter = ins_num_realist_index.find(_index);
	if (iter != ins_num_realist_index.end()) 
	{
		_index = iter->second;
	}
	log_message("Adjusted index is:" + std::to_string(_index));//32

	uint32_t deviceCount = 0;
	res = ItkManGetDeviceCount(&deviceCount);
	if (res != ITKSTATUS_OK)
	{
		log_message("Failed to get device count, error code: " + std::to_string(res));
		RETURNERRC(res);
	}
	log_message("Total device count is:" + std::to_string(deviceCount));



	// 过滤非相机设备，并记录每个设备的信息
	for (uint32_t i = 0; i < deviceCount; ++i) {
		res = ItkManGetDeviceInfo(i, &di);
		if (res != ITKSTATUS_OK) {
			log_message("Failed to get device info for index: " + std::to_string(i) + ", error code: " + std::to_string(res));
			continue;
		}
		log_message("Device index: " + std::to_string(i) + ", serial: " + std::string(di.SerialNumber) + ", name: " + std::string(di.FullName) + ", interface: " + std::string(di.DeviceClass));
		if ((strcmp(di.DeviceClass, "CameraLink") == 0 || strcmp(di.DeviceClass, "CoaXPress") == 0) && strcmp(di.SerialNumber, "") != 0) {
			log_message("Valid camera found at index: " + std::to_string(i) + ", serial: " + std::string(di.SerialNumber));
			//_index = i; // 使用找到的第一个有效相机索引
			break;
		}
	}
	res = ItkManGetDeviceInfo(_index, &di);
	//----------------
	CHECK_IKAPC(res);
	if (res != ITKSTATUS_OK)
	{
		log_message("2: " + std::to_string(res));
		RETURNERRC(res);
	}
	printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);
	
	// 当设备为 CameraLink 相机且序列号正确时。
	if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
	{
		ITK_CL_DEV_INFO cl_board_info;
		// 打开相机。
		//
		// Open camera.
		res = ItkDevOpen(_index, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("3: " + std::to_string(res));
			RETURNERRC(res);
		}
		// 获取 CameraLink 相机设备信息。
		//
		// Get CameraLink camera device information.
		res = ItkManGetCLDeviceInfo(_index, &cl_board_info);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("4: " + std::to_string(res));
			RETURNERRC(res);
		}
		// 打开采集卡。
		//
		// Open frame grabber.
		ins_hBoard = IKapOpen(cl_board_info.HostInterface, cl_board_info.BoardIndex);
		if (ins_hBoard == INVALID_HANDLE_VALUE) {
			CHECK_IKAPC(IKStatus_OpenBoardFail);
			RETURNERRC(IKStatus_OpenBoardFail);
		}
		mi_camera = _index;

		CamFullName = di.FullName;

		//cout << "sdk CamFullName:" << CamFullName << endl;
		//CXP新增
		return InsResult::INSCSTATUS_OK;
	}

	if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && (strcmp(di.VendorName, "itek-opto electronics") == 0 || strcmp(di.VendorName, "I-Tek OptoElectronics") == 0))
	{

		ITK_CXP_DEV_INFO cxp_cam_info;
		IKAP_CXP_BOARD_INFO cxp_board_info;

		// 打开相机。
		//
		// Open camera.
		res = ItkDevOpen(_index, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("1: " + std::to_string(res));
			RETURNERRC(res);
		}
		// 获取CoaXPress相机设备信息。
		//
		// Get CoaXPress camera device information.
		res = ItkManGetCXPDeviceInfo(_index, &cxp_cam_info);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("2: " + std::to_string(res));
			RETURNERRC(res);
		}
		// 打开采集卡。
		//
		// Open frame grabber.
		memcpy(&cxp_board_info, &cxp_cam_info, sizeof cxp_cam_info);
		ins_hBoard = IKapOpenCXP(IKBoardPCIE, cxp_board_info.BoardIndex, cxp_board_info);
		if (ins_hBoard == INVALID_HANDLE_VALUE)
		{
			CHECK_IKAPC(IKStatus_OpenBoardFail);
			RETURNERRC(IKStatus_OpenBoardFail);
		}

		int ret = 0;
		ret = IKapSetInfo(ins_hBoard, IKP_FRAME_COUNT, 1);//0529hc
		cout << "IKP_FRAME_COUNT_ERROR_CHECK"<< endl;
		CHECK(ret);
		cout << "IKP_FRAME_COUNT_ERROR_CHECK" << endl;
		enumAllFeatures();

		mi_camera = _index;
		CamFullName = di.FullName;//0506
		return InsResult::INSCSTATUS_OK;
	}

	return InsResult();
}




InsResult InsCameraIkap::FindCamera()
{
	ITKDEV_INFO di;

	ITKSTATUS res = ITKSTATUS_OK;
	printf("INS Console Find Camera\n");
	res = ItkManInitialize();
	CHECK(res);
	RETURNERRC(res);

	// 枚举可用设备的数量。在打开设备前，必须调用 ItkManGetDeviceCount() 函数。
	//
	// Enumerate the number of available devices. Before opening the device, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&num_devices_);
	CHECK(res);
	RETURNERRC(res);

	// 当没有连接的设备时。
	//
	// When there is no connected devices.
	if (num_devices_ == 0)
	{
		fprintf(stderr, "No device.\n");
		printf("No camera.\n");
		return(InsResult(0x0032));
	}

	// 打印连接的相机设备的信息
	uint32_t i = 0;
	for (; i < num_devices_; ++i)
	{
		ItkManGetDeviceInfo(i, &di);
		//if (di == 0)
		//        continue;
		if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
		{
			std::string str;
			int size = strlen(di.FullName);

			//ITKDEV_INFO_ENTRY_MAX_LENGTH;
			str.resize(size);

			memcpy((void*)str.data(), di.FullName, str.size());
			//string a = std::string(di->FullName);
			string pl = "PL";
			size_t found = str.find(pl);
			cout << found;
			// 如果找到 "PL"，则进行替换
			if (found != std::string::npos)
			{
				//cout << "找到了PL";
				str.replace(found, 2, "INS-LC-");
				//cout << "替换了PL";
			}

			size_t ktFound = str.find("KT");
			if (ktFound != std::string::npos)
			{
				str.replace(ktFound, 2, "M");
			}

			//到int InsCamera::ins_shaft_encoder1_min_width_ CXP新增
			size_t PHFound = str.find("PN");
			if (PHFound != std::string::npos)
			{
				str.replace(PHFound, 2, "INS-LC-");
			}
			//cout << "str" << str << endl;
			InsCameraIkap::full_name_.push_back(str);

			std::string str1;
			int size1 = strlen(di.DeviceClass);
			//ITKDEV_INFO_ENTRY_MAX_LENGTH;
			str1.resize(size1);

			memcpy((void*)str1.data(), di.DeviceClass, str1.size());
			//string a = std::string(di->FullName);
			InsCameraIkap::device_class_.push_back(str1);
		}

		if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "itek-opto electronics") == 0)
		{
			std::string str;
			int size = strlen(di.FullName);

			//ITKDEV_INFO_ENTRY_MAX_LENGTH;
			str.resize(size);

			memcpy((void*)str.data(), di.FullName, str.size());
			//string a = std::string(di->FullName);
			string pl = "PL";
			size_t found = str.find(pl);
			cout << found;
			// 如果找到 "PL"，则进行替换
			if (found != std::string::npos)
			{
				//cout << "找到了PL";
				str.replace(found, 2, "INS-LC-");
				//cout << "替换了PL";
			}

			size_t ktFound = str.find("KT");
			if (ktFound != std::string::npos)
			{
				str.replace(ktFound, 2, "M");
			}

			//到int InsCamera::ins_shaft_encoder1_min_width_ CXP新增
			size_t PHFound = str.find("PN");
			if (PHFound != std::string::npos)
			{
				str.replace(PHFound, 2, "INS-LC-");
			}
			//cout << "str" << str << endl;
			InsCameraIkap::full_name_.push_back(str);

			std::string str1;
			int size1 = strlen(di.DeviceClass);
			//ITKDEV_INFO_ENTRY_MAX_LENGTH;
			str1.resize(size1);

			memcpy((void*)str1.data(), di.DeviceClass, str1.size());
			//string a = std::string(di->FullName);
			InsCameraIkap::device_class_.push_back(str1);
		}
	}
	return InsResult::INSCSTATUS_OK;
}




InsResult InsCameraIkap::ConfigureFreeRunFileToCamera()
{
	ITKSTATUS res = ItkManInitialize();
	char configFilename[] = ".\\freerun.ccf";
	/*for (int i = 0; i < 7; i++)
	{
		configFilename[i] = s[i];
	}
	// 导入配置文件。
	//
	// Load configuration file.
	if (GetCCFOption(configFilename) == false)
	{
		printf("Fail to get configuration, using default setting!\n");
		return InsResult::INSCSTATUS_NO_File;
	}
	else
	{*/
	res = ItkDevLoadConfigurationFromFile(ins_hCamera, configFilename);
	cout << "here res:" << res << endl;
	CHECK_IKAPC(res);
	return (InsResult)(res & 0xffff);
	//}

}

InsResult InsCameraIkap::Configure250KFileToCamera()
{
	ITKSTATUS res = ItkManInitialize();
	char configFilename[] = "./250K.ccf";
	res = ItkDevLoadConfigurationFromFile(ins_hCamera, configFilename);
	cout << "here res:" << res << endl;
	CHECK_IKAPC(res);
	return (InsResult)(res & 0xffff);
}


InsResult InsCameraIkap::SaveCameraConfigurationToFile(char* _filename)
{
	int res = 0;
	res = ItkDevSaveConfigurationToFile(ins_hCamera, _filename);
	CHECK(res);
	return (InsResult)(res & 0xffff);
}


InsResult InsCameraIkap::ConfigureExternalPulseFileToCamera()
{

	ITKSTATUS res = ItkManInitialize();
	char configFilename[] = "./externalpulse.ccf";
	/*for (int i = 0; i < 7; i++)
	{
		configFilename[i] = s[i];
	}
	// 导入配置文件。
	//
	// Load configuration file.
	if (GetCCFOption(configFilename) == false)
	{
		printf("Fail to get configuration, using default setting!\n");
		return InsResult::INSCSTATUS_NO_File;
	}
	else
	{*/
	res = ItkDevLoadConfigurationFromFile(ins_hCamera, configFilename);
	cout << "here res:" << res << endl;
	CHECK_IKAPC(res);
	return (InsResult)(res & 0xffff);
	//}

}

/*
InsResult InsCameraIkap::ConfigureFileToCamera(char* filename_)
{
	ITKSTATUS res = ItkManInitialize();
	//char configFilename[] = "./externalpulse.ccf";
	res = ItkDevLoadConfigurationFromFile(ins_hCamera, filename_);
	//cout << "here res:" << res << endl;
	CHECK_IKAPC(res);
	return (InsResult)(res & 0xffff);
	//}
}
*/
/*
InsResult InsCameraIkap::ConfigureFileToFrameGrabber()
{
	int ret = IK_RTN_OK;
	// 导入配置文件。
	//
	// Load configuration file.
	char configFilename[128];
	if (GetOption(configFilename) == false)
	{
		printf("Fail to get configuration, using default setting!\n");
		return InsResult::INSCSTATUS_NO_File;
	}
	else
	{
		ret = IKapLoadConfigurationFromFile(ins_hBoard, configFilename);
		CHECK_IKAPBOARD(ret);
		return (InsResult)(ret | 0x1000);
	}
}
*/

static const char* user_u32toStr1(uint32_t aInt)
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


InsResult InsCameraIkap::SaveConfigureFileFromeFrameGrabber(const char* filename_, int CameraNum ,int LightNum, InsPMDL_SystemInfo SystemInfo)//icf
{
	int ret = 0;
	string endname = ".icf";
	string temp(filename_);
	int found = temp.find(endname);
	if (found == string::npos) return InsResult::INSSTATUS_FILENAME_ERROR;//文件地址错误
	ofstream writeFile(filename_, std::ios::out | std::ios::trunc);
	cout << "configure board file" << endl;


	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;


	//----------------------//多光源兼容 遍历
	writeFile << setw(10) << "CamNum :" << setw(20) << std::to_string(CameraNum) << "\n";//相机数量
	writeFile << setw(10) << "LigNum :" << setw(20) << std::to_string(LightNum) << "\n";//光源数量
	writeFile << setw(10) << "LocalIP:" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->localIP) << "\n";//本地IP
	writeFile << setw(10) << "LightIP:" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.ip_addr) << "\n";//光源IP
	writeFile << setw(10) << "Board  :" << setw(20) << std::to_string(SystemInfo.CamInfo.id[0]) << "\n";//板卡数
	writeFile << setw(10) << "Length :" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length) << "\n";//光原板长度


	//----------------------
	if (namecam.find("4K") != string::npos)
	{
		string start_str = "--->INS_2D5_Configure_CL4KCamera_File:";//0126
		writeFile << start_str << endl;
	}

	else if (namecam.find("8KCXP") != string::npos)
	{
		string start_str = "--->INS_2D5_Configure_CXP8KCamera_File:";//0126
		writeFile << start_str << endl;
	}

	else if (namecam.find("16KCXP") != string::npos)
	{
		string start_str = "--->INS_2D5_Configure_CXP16KCamera_File:";//0126
		writeFile << start_str << endl;
	}

	else if (namecam.find("8K") != string::npos)
	{
		string start_str = "--->INS_2D5_Configure_CL8KCamera_File:";//0126
		writeFile << start_str << endl;
	}
	int tmp = 0;
	tmp = get_ins_height();
	if (tmp == INT32_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10)<<"height"<< setw(20) <<std::to_string(tmp) << "\n";

	tmp = get_ins_width();
	if (tmp == INT32_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10) << "width" << setw(20) << std::to_string(tmp) << "\n";

	double tmp_double;
	tmp_double = get_ins_exposure();
	if (tmp_double == DBL_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10) << "exposure" << setw(20) << std::to_string(tmp_double) << "\n";

	tmp_double = get_ins_line_period_time();
	if (tmp_double == DBL_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10) << "lineperiod" << setw(20) << std::to_string(tmp_double) << "\n";

	tmp_double = get_ins_digital_gain();
	if (tmp_double == DBL_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10) << "digitalgan" << setw(20) << std::to_string(tmp_double) << "\n";
	

	
	if (namecam.find("16KCXP") != string::npos)//如果相机是16KCXP   Gain_x1  8
	{
		char ins_PAC_gain[8] = "";
		get_ins_PAC_gain(ins_PAC_gain);
		if (strcmp(ins_PAC_gain, "") == 0)
		{
			writeFile.close();
			std::remove(filename_);
			return InsResult::INS_RTN_ERROR;
		}
		writeFile << setw(10) << "PAC_gain" << setw(20) << ins_PAC_gain << "\n";
	}
	else                    //其他为 x10  4
	{
		char ins_PAC_gain[4] = "";
		get_ins_PAC_gain(ins_PAC_gain);
		if (strcmp(ins_PAC_gain, "") == 0)
		{
			writeFile.close();
			std::remove(filename_);
			return InsResult::INS_RTN_ERROR;
		}
		writeFile << setw(10) << "PAC_gain" << setw(20) << ins_PAC_gain << "\n";
	}
	
	tmp = get_ins_shaft_encoder1_min_width_();
	if (tmp == INT32_MAX)
	{
		writeFile.close();
		std::remove(filename_);
		return InsResult::INS_RTN_ERROR;
	}
	writeFile << setw(10) << "encoderwdt" << setw(20) << std::to_string(tmp) << "\n";


	writeFile.close();
	return InsResult::INSSTATUS_SUCCESS;

}

InsResult InsCameraIkap::UserSetSave()
{
	int ret = IK_RTN_OK;
	ret = ItkDevExecuteCommand(ins_hCamera, "UserSetSave");
	CHECK_IKAPC(ret);
	RETURNERRC(ret);
	return (InsResult)0;

}
InsResult InsCameraIkap::set_user_set_selector(const char* str_)//21.28
{
	ITKFEATURE hFeature;
	int res = ItkDevAllocFeature(ins_hCamera, "UserSetSelector", &hFeature);
	CHECK(res);
	RETURNERRC(res);
	// 设置枚举类型特征的值
	res = ItkFeatureFromString(hFeature, str_);
	CHECK(res);
	RETURNERRC(res);
	return InsResult::INSCSTATUS_OK;
}
InsResult InsCameraIkap::get_user_set_selector(char* val)
{
	ITKFEATURE hFeature;
	int res = ItkDevAllocFeature(ins_hCamera, "UserSetSelector", &hFeature);
	CHECK(res);
	RETURNERRC(res);
	// 获取枚举类型特征的值
	uint32_t enumValSize = 0;
	res = ItkFeatureToString(hFeature, NULL, &enumValSize);
	res = ItkFeatureToString(hFeature, val, &enumValSize);
	CHECK(res);
	RETURNERRC(res);
	return InsResult::INSCSTATUS_OK;
}

InsResult InsCameraIkap::ConfigureFileToFrameGrabber(const char* filename_)
{
	if (filename_)
	{
		const char* fileExtension = strrchr(filename_, '.'); // 获取最后一个点的位置
		if (!(fileExtension && strcmp(fileExtension, ".icf") == 0))
		{
			std::cout << "读取失败;" << endl;
			return InsResult::INSSTATUS_FILENAME_ERROR;  // 文件后缀不是 ".icf"
		}
	}
	ifstream readFile(filename_, std::ios::in);
	if (!readFile.is_open())
	{
		std::cout << "读取失败;" << endl;
		return InsResult::INSSTATUS_OPEN_FILE_FAILED; 
	}


	InsResult ret = InsResult::INSSTATUS_SUCCESS;
	string l_readstr;
	string l_tmpstr;
	char* pEndTemp;
	int _height = 0;
	int _width = 0;
	double temp = 0;
	l_readstr.clear();
	std::getline(readFile, l_readstr);
	std::cout <<"name:" << l_readstr << endl;

	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;

	//-----------------
	string line;
	int LineNum = 0;
	while (LineNum <5  && getline(readFile,line))
	{
		LineNum++;
	}

	getline(readFile, line);//读取第六行  //七行 0511
	std::cout << "Newname:" << line << endl;
	//-----------------
	if (namecam.find("4K") != string::npos)//如果相机是4K
	{
		if (line.find("CL4K") == string::npos)//并且icf第一行不是CL4K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}

	else if (namecam.find("8KCXP") != string::npos)//0226
	{
		if (line.find("CXP8K") == string::npos)//并且icf第一行不是CXP8K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}

	else if (namecam.find("16KCXP") != string::npos)//0226
	{
		if (line.find("CXP16K") == string::npos)//并且icf第一行不是CXP8K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}


	else if (namecam.find("8K") != string::npos)//如果相机是8K
	{
		if (line.find("CL8K") == string::npos)//并且icf第一行不是8K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}

	else
	{
		return InsResult::INSCSTATUS_FILE_ERR;
	}


	int line_num = 6;//0
	while (line_num < 14)//8
	{
		line_num++;
		if (readFile.eof()) 
		{ 
			readFile.close();
			break; 
		}
		std::getline(readFile, l_readstr);
		l_tmpstr = l_readstr.substr(0, 10);//从0开始读10个
		if (strcmp(l_tmpstr.c_str(),"    height") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			_height = strtol(&(l_tmpstr[0]), &pEndTemp, 10);//10表示十进制
			ret = set_ins_height(_height);
			if (ret != InsResult::INSSTATUS_SUCCESS) 
			{ 
				readFile.close();
				return ret;
			}
			continue;
		}
		else if (strcmp(l_tmpstr.c_str(), "     width") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			_width = strtol(&(l_tmpstr[0]), &pEndTemp, 10);
			ret = set_ins_width(_width);
			if (ret != InsResult::INSSTATUS_SUCCESS)
			{
				readFile.close();
				return ret;
			}
		}
		else if (strcmp(l_tmpstr.c_str(), "  exposure") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			temp = strtod(&(l_tmpstr[0]), &pEndTemp);
			ret = set_ins_exposure(temp);
			if (ret != InsResult::INSCSTATUS_OK)
			{
				readFile.close();
				return ret;
			}
		}
		else if (strcmp(l_tmpstr.c_str(), "lineperiod") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			temp = strtod(&(l_tmpstr[0]), &pEndTemp);
			ret = set_ins_line_period_time(temp);
			if (ret != InsResult::INSCSTATUS_OK)
			{
				readFile.close();
				return ret;
			}
		}
		else if (strcmp(l_tmpstr.c_str(), "digitalgan") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			temp = strtod(&(l_tmpstr[0]), &pEndTemp);
			ret = set_ins_digital_gain(temp);
			if (ret != InsResult::INSCSTATUS_OK)
			{
				readFile.close();
				return ret;
			}
		}
		else if (strcmp(l_tmpstr.c_str(), "  PAC_gain") == 0)
		{
			
			l_tmpstr = l_readstr.substr(15, 15);
			int pos = l_tmpstr.find("x");
			int length = l_tmpstr.length();
			int char_length = length - pos;
			if (char_length == 2)
			{
				char PAC_gain_temp[3] = "  ";//有5位
				l_tmpstr.copy(PAC_gain_temp, length - pos, pos);
				ret = set_ins_PAC_gain(PAC_gain_temp);
				if (ret != InsResult::INSCSTATUS_OK)
				{
					readFile.close();
					return ret;
				}
			}
			if (char_length == 3)
			{
				char PAC_gain_temp[4] = "   ";//有5位
				l_tmpstr.copy(PAC_gain_temp, length - pos, pos);
				ret = set_ins_PAC_gain(PAC_gain_temp);
				if (ret != InsResult::INSCSTATUS_OK)
				{
					readFile.close();
					return ret;
				}
			}
		}
		else if (strcmp(l_tmpstr.c_str(), "encoderwdt") == 0) //0401
		{
			l_tmpstr = l_readstr.substr(15, 15);
			temp = strtod(&(l_tmpstr[0]), &pEndTemp);
			ret = set_ins_shaft_encoder1_min_width_(temp);
			if (ret != InsResult::INSCSTATUS_OK)
			{
				readFile.close();
				return ret;
			}
		}
	}
	return InsResult::INSSTATUS_SUCCESS;
}

void printErrorAndExit(ITKSTATUS InsResult)
{
	fprintf(stderr, "Error Code:%08x\n", InsResult);
}

InsResult InsCameraIkap::GrabOnce()
{
	InsCameraIkap::frame_id_ = 0;
	int ret = IKapStartGrab(ins_hBoard, 1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 等待图像采集结束。
	//
	// Wait for grabbing images finished.
	ret = IKapWaitGrab(ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 停止图像采集。
	//
	// Stop grabbing images.
	ret = IKapStopGrab(ins_hBoard);
	CHECK_IKAPBOARD(ret);
	InsCameraIkap::frame_id_ ++;
	return InsResult(ret|0x1000);
}

InsResult InsCameraIkap::GrabContinous()
{
	HANDLE hVul = ins_hBoard;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int ret = 1;

	if ((namecam.find("8KCXP") != string::npos )||(namecam.find("16KCXP") != string::npos))//可能不仅是宽度0201 to do
	{
		int res = ItkDevExecuteCommand(ins_hCamera, "AcquisitionStop");
		CHECK(res);
		RETURNERRC(res);

		int ret = IKapStartGrab(ins_hBoard, 0);
		CHECK_IKAPBOARD(ret);
		if (ret != 1)
		{
			return InsResult(ret | 0x1000);
		}
		res = ItkDevExecuteCommand(ins_hCamera, "AcquisitionStart");
		CHECK(res);
		RETURNERRC(res);
	}
	else 
	{ 
		ret = IKapStartGrab(ins_hBoard, 0);
		CHECK_IKAPBOARD(ret);
	}


	return InsResult(ret | 0x1000);
}



InsResult InsCameraIkap::GrabStop()
{
	// 停止图像采集。
	//
	// Stop grabbing images.
	//InsCameraIkap::frame_id_ = 0;
	int ret = IKapStopGrab(ins_hBoard);
	//ITKSTREAM hStream;
	//int res = 0;
	//res = ItkDevAllocStream(hCamera, 0, hBuffer, &hStream);
	//CHECK(res);
	//int ret = ItkStreamStop();
	CHECK_IKAPBOARD(ret);
	InsCameraIkap::frame_id_ = 0;

	return InsResult(ret | 0x1000);
}

void printErrorMessage(const char* msg1,const char* msg2)
{
	IKAPERRORINFO pIKErrInfo;
	memset(&pIKErrInfo, 0, sizeof(IKAPERRORINFO));
	IKapGetLastError(&pIKErrInfo, true);
	printf(" % s(% s)\nType = % d\nIndex = % 08x\nError Code = % 08x\n", msg1, msg2, pIKErrInfo.uBoardType, pIKErrInfo.uBoardIndex, pIKErrInfo.uErrorCode);
}

void freeFrameGrabberHandle(HANDLE hVul)
{
	if (hVul != INVALID_HANDLE_VALUE)
	{
		IKapClose(hVul);
		hVul = INVALID_HANDLE_VALUE;
	}
}

InsResult InsCameraIkap::ConfigureBoard()
{
	HANDLE hVul = ins_hBoard;
	int TimeOut = INFINITE; // 超时时间
	int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // 图像类型
	int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
	int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // 数据格式
	int tapNum = 10; // Tap
	int ret = 0;
	/* 相机 CC1 触发源 */
	int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
	/* 采集卡触发源 */
	int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT1;
	/* 频闪触发源 */
	int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
	/* 采集卡触发方法 */
	int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;

	/* 设置相机扫描类型 */


	//先算（IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)如果设置正确返回值为1，则（1==0）返回0，if（）不执行{}
	if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置超时时间 */
	if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TIME_OUT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像类型 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置数据格式 */
	if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
	{
		printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像宽度 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_WIDTH, ins_width_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_WIDTH");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像高度 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置 Tap 数 */
	if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置相机 CC1 触发源 */
	if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
	{
		printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置采集卡触发源 */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置频闪触发源 */
	if (ret = IKapSetInfo(hVul, IKP_STROBE_TRIGGER_SOURCE, StrobeTriggerSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	if (ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_TRIGGER_MODE, 0) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置采集卡触发方法 */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	//通用输入信号1, 2 触发模式_电平触发
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	//CL 采集卡是否校验 FVAL 信号_是
	ret = IKapSetInfo(hVul, IKP_CHECK_FRAME_VALID_SIGNAL, 0);
	RETURNERRCBOARD(ret);	
	return InsResult(0);
}


InsResult InsCameraIkap::ConfigureCXPBoard()
{
	HANDLE hVul = ins_hBoard;
	int TimeOut = INFINITE; // 超时时间
	int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // 图像类型
	int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
	int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // 数据格式
	int tapNum = 1; // 在CXP版本中只兼容Tap数为1
	int ret = 0;
	/* 相机 CC1 触发源 */
	int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
	/* 采集卡触发源 */
	int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;//gai
	/* 频闪触发源 */
	int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
	/* 采集卡触发方法 */
	int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;

	/* 设置相机扫描类型 */
	if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置超时时间 */
	if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TIME_OUT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像类型 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}


	/* 设置数据格式 */
	if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
	{
		printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像宽度 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_WIDTH, ins_width_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_WIDTH");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置图像高度 */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置 Tap 数 */
	if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置相机 CC1 触发源 */
	if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
	{
		printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置采集卡触发源 */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置频闪触发源 */
	if (ret = IKapSetInfo(hVul, IKP_STROBE_TRIGGER_SOURCE, StrobeTriggerSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	if (ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_TRIGGER_MODE, 0) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* 设置采集卡触发方法 */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	//通用输入信号1, 2 触发模式_电平触发
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	//设置INTEGRATION_PARAM2
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM2, 720);
	RETURNERRCBOARD(ret);
	//设置INTEGRATION_PARAM4
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM4, 720);
	RETURNERRCBOARD(ret);
	//设置GENERAL_INPUT2_MIN_WIDTH
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_MIN_WIDTH, 300);
	RETURNERRCBOARD(ret);
	//设置CXP_TRIGGER_OUTPUT_SELECTOR
	ret = IKapSetInfo(hVul, IKP_CXP_TRIGGER_OUTPUT_SELECTOR, 1);
	RETURNERRCBOARD(ret);
	//设置SHAFT_ENCODER_DEBOUNCE
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_DEBOUNCE, IKP_SHAFT_ENCODER_DEBOUNCE_MIN);
	RETURNERRCBOARD(ret);
	//设置SHAFT_ENCODER_TICK_MODE 
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_TICK_MODE, IKP_CXP_SHAFT_ENCODER_TICK_MODE_ANY_DIRECTION);
	RETURNERRCBOARD(ret);
	//设置SHAFT_ENCODER_TICK_REVERSE_MODE 
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_REVERSE_MODE, IKP_CXP_SHAFT_ENCODER_REVERSE_MODE_ANY_DIRECTION);
	RETURNERRCBOARD(ret);
	//设置IKP_INTEGRATION_TRIGGER_FREQUENCY 
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_FREQUENCY, 199000);//gai
	RETURNERRCBOARD(ret);
	//CL 采集卡是否校验 FVAL 信号_是
	ret = IKapSetInfo(hVul, IKP_CHECK_FRAME_VALID_SIGNAL, 0);
	RETURNERRCBOARD(ret);
	return InsResult(0);
}

InsResult InsCameraIkap::ConfigureBoardWith250K()  //每次更新都要改
{
 	HANDLE hVul = ins_hBoard;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;


	if (namecam.find("8KCXP") != string::npos)//可能不仅是宽度0201 to do
	{
    int ret = 0;
    std::cout << "namecam : " << namecam << endl;
    ret = (int)set_ins_width(8000);

	ITKFEATURE hFeature;
	std::string temStr = "On";
	int res = ItkDevAllocFeature(ins_hCamera, "TriggerMode", &hFeature);
	log_message("DevAllocFeatrue TriggerMode : " + std::to_string(res));
	CHECK(res);
	//RETURNERRC(res);
	// 设置枚举类型特征的值
	res = ItkFeatureFromString(hFeature, temStr.c_str());
	log_message(" FeatureFromString TriggerMode: " + std::to_string(res));
	CHECK(res);
	//RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);
	CHECK(res);

	//----------------0605---------------------------
	std::string temStr2 = "TriggerPulse";
	int res5 = ItkDevAllocFeature(ins_hCamera, "ExposureMode", &hFeature);
	log_message(" res5_ExposureMode: " + std::to_string(res5));
	CHECK(res5);
	RETURNERRC(res5);
	// 设置枚举类型特征的值
	res5 = ItkFeatureFromString(hFeature, temStr2.c_str());
	log_message(" res5_c_str: " + std::to_string(res5));
	CHECK(res5);
	RETURNERRC(res5);
	res5 = ItkDevFreeFeature(hFeature);
	CHECK(res5);


	std::string temStr3 = "CXPin";
	int res2 = ItkDevAllocFeature(ins_hCamera, "LineTriggerSource", &hFeature);
	log_message(" res2_cxpin: " + std::to_string(res2));
	CHECK(res2);
	RETURNERRC(res2);
	// 设置枚举类型特征的值
	res2 = ItkFeatureFromString(hFeature, temStr3.c_str());
	log_message(" res2 : " + std::to_string(res2));
	CHECK(res2);
	RETURNERRC(res2);
	res2 = ItkDevFreeFeature(hFeature);
	CHECK(res2);



	//---------------------------------------0407 针对丢帧问题改进 只有8k有
	bool Enable2 = true;
	int res3 = ItkDevAllocFeature(ins_hCamera, "StrobescopicLightEnable", &hFeature);
	CHECK(res3);
	RETURNERRC(res3);
	// 设置Bool类型特征的值
	res3 = ItkFeatureSetBool(hFeature, Enable2);
	CHECK(res3);
	RETURNERRC(res3);
	res3 = ItkDevFreeFeature(hFeature);
	CHECK(res3);

	ITKSTATUS res7 = ITKSTATUS_OK;
	bool pValue7 = 0;
	// Set feature value
	res7 = ItkDevSetBool(ins_hCamera, "ShaftEncoderUseSingleChannel", pValue7);



	set_user_set_selector("User2");

	res = IK_RTN_OK;
	int pValue = 5;
	// Set info
	res = IKapSetInfo(ins_hBoard, IKP_INTEGRATION_TRIGGER_SOURCE, pValue);

    }





	else if (namecam.find("16KCXP") != string::npos)//可能不仅是宽度0201 to do
	{
		int ret = 0;
		std::cout << "namecam : " << namecam << endl;  //0329
		ret = (int)set_ins_width(16000);//宽度要改0401  set_ins_width 里面包括采集卡和相机 //16384

    //wyn

		ITKFEATURE hFeature;
	    std::string temStr = "On";
		int res = ItkDevAllocFeature(ins_hCamera, "TriggerMode", &hFeature);
		log_message("ItkDevAllocFeatrue TriggerMode : " + std::to_string(res));
	    CHECK(res);
	    RETURNERRC(res);
	    // 设置枚举类型特征的值
	    res = ItkFeatureFromString(hFeature, temStr.c_str());
		log_message("ItkDevAllocFeatrue TriggerMode FromString : " + std::to_string(res));
	    CHECK(res);
	    RETURNERRC(res);
	    res = ItkDevFreeFeature(hFeature);
	    CHECK(res);

		res = IK_RTN_OK;
		int pValue = 5;
		// Set info
		res = IKapSetInfo(ins_hBoard, IKP_INTEGRATION_TRIGGER_SOURCE, pValue);
		CHECK(res);//0529H

		res = IKapSetInfo(ins_hBoard, IKP_FRAME_COUNT, 1);//0529hc
		CHECK(res);

		set_user_set_selector("User2");


	}
	else//4k
	{
		int TimeOut = INFINITE; // 超时时间
		int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // 图像类型
		int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
		int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // 数据格式
		int tapNum = 10; // Tap
		int ret = 0;
		/* 相机 CC1 触发源 */
		int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
		/* 采集卡触发源 */
		int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
		/* 采集卡触发方法 */
		int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;
		/* 频闪触发源 */
		int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
		/* 设置相机扫描类型 */


		///0417 Taps Ten

        ITKFEATURE hFeature;
	    int res3 = ItkDevAllocFeature(ins_hCamera, "SensorTaps", &hFeature);//创建关于相机特征值的指针
	    CHECK(res3);
	    RETURNERRC(res3);
	    // 设置枚举类型特征的值
	    res3 = ItkFeatureFromString(hFeature, "Ten");//利用这个指针去找输入的特征值
	    CHECK(res3);
	    RETURNERRC(res3);
	    res3 = ItkDevFreeFeature(hFeature);//释放指针
	    CHECK(res3);

		int res4 = ItkDevAllocFeature(ins_hCamera, "SynchronizationMode", &hFeature);
		CHECK(res4);
		RETURNERRC(res4);
		// 设置枚举类型特征的值
		res4 = ItkFeatureFromString(hFeature, "ExternalPulse");
		CHECK(res4);
		RETURNERRC(res4);
		res4 = ItkDevFreeFeature(hFeature);
		CHECK(res4);

		int res5 = ItkDevAllocFeature(ins_hCamera, "ExternalTriggerSource", &hFeature);
		CHECK(res5);
		RETURNERRC(res5);
		// 设置枚举类型特征的值
		res5 = ItkFeatureFromString(hFeature, "CC1");
		CHECK(res5);
		RETURNERRC(res5);
		res5 = ItkDevFreeFeature(hFeature);
		CHECK(res5);

		int res6 = ItkDevAllocFeature(ins_hCamera, "InputLineDetectionLevel", &hFeature);
		CHECK(res6);
		RETURNERRC(res6);
		// 设置枚举类型特征的值
		res6 = ItkFeatureFromString(hFeature, "Threshold_for_3V3_to_5V");
		CHECK(res6);
		RETURNERRC(res6);
		res6 = ItkDevFreeFeature(hFeature);
		CHECK(res6);

		//---------0517
        //为断电保存添加、必须设置user2

		set_user_set_selector("User2");
	
	//--------------------------------0417 END-----------------------------
		if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
		{
			printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置超时时间 */
		if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
		{
			printErrorMessage("INSSetInfo", "INS_TIME_OUT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置图像类型 */
		if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
		{
			printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}


		/* 设置数据格式 */
		if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
		{
			printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置图像宽度 */

		int res = 0;
		if (namecam.find("4K") != string::npos)
		{
			ins_width_ = 4100;
		}
		else if (namecam.find("8K") != string::npos)
		{
			ins_width_ = 8200;
		}

		if (ret = IKapSetInfo(hVul, IKP_IMAGE_WIDTH, ins_width_) == false)
		{
			printErrorMessage("INSSetInfo", "INS_IMAGE_WIDTH");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置图像高度 */
		if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
		{
			printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置 Tap 数 */
		if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
		{
			printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置相机 CC1 触发源 */
		if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
		{
			printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置采集卡触发源 */
		if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
		{
			printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置采集卡触发方法 */
		if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_METHOD");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置GENERAL_INPUT1_POLARITY*/
		if (ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_POLARITY, 0) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT1_POLARITY");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* 设置频闪触发源 */
		if (ret = IKapSetInfo(hVul, IKP_STROBE_TRIGGER_SOURCE, StrobeTriggerSource) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_STROBE_TRIGGER_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}

		/* 设置内部触发频率 */
		if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_FREQUENCY, 199000) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_TRIGGER_FREQUENCY");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		ret = IKapSetInfo(hVul, IKP_SHAFT_ENCODER1_MIN_WIDTH, ins_shaft_encoder1_min_width_);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_SHAFT_ENCODER1_MIN_WIDTH");
			cout << "最小触发配置失败" << endl;
		}
		RETURNERRCBOARD(ret);
		cout << "最小触发配置成功" << endl;
		//通用输入信号1, 2 触发模式_电平触发
		ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_TRIGGER_MODE, 0);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT1_TRIGGER_MODE");
		}
		RETURNERRCBOARD(ret);
		ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT2_TRIGGER_MODE");
			cout << "GENERAL_INPUT2_TRIGGER_MODE配置失败" << endl;
		}
		RETURNERRCBOARD(ret);
		cout << "GENERAL_INPUT2_TRIGGER_MODE配置成功" << endl;
		//设置INTEGRATION_PARAM2
		ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM2, 5);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_PARAM2");
		}
		RETURNERRCBOARD(ret);
		//设置INTEGRATION_PARAM4
		ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM4, 1);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_PARAM4");
		}
		RETURNERRCBOARD(ret);
		//设置GENERAL_INPUT2_MIN_WIDTH
		ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_MIN_WIDTH, 1000);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT2_MIN_WIDTH");
		}
		RETURNERRCBOARD(ret);
		//CL 采集卡是否校验 FVAL 信号_是
		ret = IKapSetInfo(hVul, IKP_CHECK_FRAME_VALID_SIGNAL, 0);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_CHECK_FRAME_VALID_SIGNAL");
		}
		RETURNERRCBOARD(ret);
	}
	return InsResult(0);
}

InsResult InsCameraIkap::Close()
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	// 清除回调函数。
//
// Unregister callback functions.
	UnRegisterCallback();

	cout << "UnRegisterCallback" << endl;

	// 关闭采集卡设备。
	//
	// Close frame grabber device.
	ret = IKapClose(ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);


	// 关闭采集设备。
	//
	// Close camera device.
	res = ItkDevClose(ins_hCamera);
	CHECK_IKAPC(res);
	RETURNERRC(res);


	// 释放 IKapC 运行环境。
	//
	// Release IKapC runtime environment.
	ItkManTerminate();
	return InsResult(0);
	//cout << "ItkManTerminate" << endl;

}

void InsCameraIkap::set_data_callback(DataPocessCallBack _callback)
{
	mCallback = _callback;
	int ret = IKapRegisterCallback(ins_hBoard, IKEvent_FrameReady, OnFrameReady, ins_hBoard);
	CHECK_IKAPBOARD(ret);
}
void InsCameraIkap::unregister_data_callback() {
	int ret = 0;
	ret = IKapUnRegisterCallback(ins_hBoard, IKEvent_FrameReady);
	mCallback = NULL;
}
void _stdcall removalCallbackFunction(void* context, ITKEVENTINFO eventInfo)
{
	/* 检索事件类型 */
	uint32_t type = 0;
	uint64_t countstamp = 0;
	ITKDEVICE hDev = (ITKDEVICE)context;
	ITKSTATUS res = ItkEventInfoGetPrm(eventInfo, ITKEVENTINFO_PRM_TYPE, &type);
	removeCallback;
}
void InsCameraIkap::set_device_remove_callback(OffLineCallBack _callback)
{
	removeCallback = _callback;
	void* context = nullptr;
	int res = ItkDevRegisterCallback(ins_hCamera, "DeviceRemove", removalCallbackFunction, context);
	CHECK_IKAPC(res);
}
void InsCameraIkap::unregister_device_remove_callback()
{
	int res = ItkDevUnregisterCallback(ins_hCamera, "DeviceRemove");
	removeCallback = nullptr;
	CHECK_IKAPC(res);
}
int InsCameraIkap::get_ins_frame_size()
{
	int nFrameSize = 0;
	int ret = IKapGetInfo(ins_hBoard, IKP_FRAME_SIZE, &nFrameSize);
	CHECK_IKAPBOARD(ret);
	if ((ret | 0x1000) != (0x1000 | 0x00000001)) { return INT_MAX;}
	ins_frame_size_ = nFrameSize;
	return nFrameSize;
}


//触发源  IKP_BOARD_TRIGGER_SOURCE
InsResult InsCameraIkap::set_ins_trigger_source_activated(int index)
{
	int ret = IK_RTN_OK;
	switch (index)
	{

	case 0:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	case 1:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT2);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	case 2:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1);
		CHECK_IKAPBOARD(ret);
		break;

		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC1);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	case 4:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC2);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	case 5:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_INNER_TRIGGER);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	case 6:
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_SOFTWARE);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret | 0x1000));
		break;
	default:
		return InsResult::INSSTATUS_SET_INFO_ERROR;
		break;
	}
}

//工作模式  IKP_GRAB_MODE
InsResult InsCameraIkap::set_ins_work_mode_activated(int index)
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	if (index == 0)//自由模式
	{
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_INNER);
		CHECK_IKAPBOARD(ret);
	}
	else
	{
		// 设置采集卡触发模式。
		// Set frame grabber trigger mode.
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_OUTTER);
		CHECK_IKAPBOARD(ret);
	}
	return InsResult(ret | 0x1000);
}
InsResult InsCameraIkap::set_ins_width(int index)//0201 todo
{
	int ret = IK_RTN_OK;
	HANDLE hVul = ins_hBoard;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;


	if (namecam.find("8KCXP") != string::npos)//可能不仅是宽度0201 to do
	{

		
		std::cout << "namecam : " << namecam << endl;
		InsResult camret = InsResult::INSCSTATUS_OK;

		ITKFEATURE hFeature;
		int	res = ItkDevAllocFeature(ins_hCamera, "Width", &hFeature);
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}

		// 获取Double特征值
		res = ItkFeatureSetInt64(hFeature, index);
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}

		res = ItkDevFreeFeature(hFeature);
		ins_width_ = index;
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}
	}

	else if (namecam.find("16KCXP") != string::npos)//可能不仅是宽度0201 to do
	{
		std::cout << "namecam : " << namecam << endl;
		InsResult camret = InsResult::INSCSTATUS_OK;

		ITKFEATURE hFeature;
		int	res = ItkDevAllocFeature(ins_hCamera, "Width", &hFeature);
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}

		// 获取Double特征值
		res = ItkFeatureSetInt64(hFeature, index);
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}

		res = ItkDevFreeFeature(hFeature);
		ins_width_ = index;
		CHECK(res);
		if (res != ITKSTATUS_OK)
		{
			return(InsResult)res;
		}
	}
		// 设置采集卡触发模式。
		// Set frame grabber trigger mode.
		ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_WIDTH, index);
		cout<<"Width::::"<<get_ins_width();
		CHECK_IKAPBOARD(ret);
		InsCameraIkap::ins_width_ = index;
		return InsResult(ret | 0x1000);
}
InsResult InsCameraIkap::set_ins_height(int index)
{
	int ret = IK_RTN_OK;
	// 设置采集卡触发模式。
	// Set frame grabber trigger mode.
	//ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index + 8);//0322

	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("8KCXP") != string::npos)//如果相机是4K  //0325    只有CXP+8行 CL+8行会出错
	{
		//ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index + 8);//0322  //抽行处理
		ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index);
	}

	else 
	{
		ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index);//0322 //0403
	}

	CHECK_IKAPBOARD(ret);
	InsCameraIkap::ins_height_ = index;
	return InsResult(ret | 0x1000);


}
string  InsCameraIkap::get_cam_full_name()
{
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do

	cout << "full name :" << di.FullName;

	return di.FullName;
}
int InsCameraIkap::get_ins_width()
{
	int nwidth = 0;
	int ret = IKapGetInfo(ins_hBoard, IKP_IMAGE_WIDTH, &nwidth);
	CHECK_IKAPBOARD(ret);
	if ((ret | 0x1000) != (0x1000 | 0x00000001)) { return INT32_MAX; }
	ins_width_ = nwidth;
	return nwidth;

	
}

int InsCameraIkap::get_ins_height()
{
	int nheight = 0;
	int ret = IKapGetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, &nheight);// IKP_IMAGE_HEIGHT

	CHECK_IKAPBOARD(ret);
	if ((ret | 0x1000) != (0x1000 | 0x00000001)) 
	{
		return INT32_MAX;
	}
	
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("8K") != string::npos)//如果相机是4K
	{
		ins_height_ = nheight;//-8
		return nheight;//0322  //-8 0403
	}

	else
	{
		ins_height_ = nheight;
		return nheight;//0322
	}

}


//缓冲区帧数  IKP_FRAME_COUNT
InsResult InsCameraIkap::set_ins_frame_count(int nins_frame_count_)
{
	ins_frame_count_ = nins_frame_count_;
	int ret = IKapSetInfo(ins_hBoard, IKP_FRAME_COUNT, 1);
	return InsResult(ret | 0x1000);
}


InsResult InsCameraIkap::set_ins_image_type(int index)
{
	ins_type_ = index;
	int ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_TYPE, index);
	return InsResult(ret | 0x1000);
}

InsResult InsCameraIkap::set_line_trigger()
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;

	// 设置CC1信号源。
	// Set CC1 signal source.
	ret = IKapSetInfo(ins_hBoard, IKP_CC1_SOURCE, IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 设置积分控制方法触发信号源。
	// Set integration control method trigger source.
	ret = IKapSetInfo(ins_hBoard, IKP_INTEGRATION_TRIGGER_SOURCE, IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);

	// 设置同步模式。
	// Set synchronization mode.
	const char* syncName = "SynchronizationMode";
	const char* syncParameter = "ExternalPulse";
	res = ItkDevFromString(ins_hCamera, syncName, syncParameter);
	CHECK_IKAPC(res);
	return InsResult(res & 0xffff);
}

InsResult InsCameraIkap::set_frame_trigger()
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;

	// 设置采集卡触发模式。
	// Set frame grabber trigger mode.
	ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_OUTTER);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 设置采集卡触发源。
	// Set frame grabber trigger source.
	ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// 设置同步模式。
	// Set synchronization mode.
	const char* syncName = "SynchronizationMode";
	const char* syncParameter = "InternalFreeRun";
	res = ItkDevFromString(ins_hCamera, syncName, syncParameter);
	CHECK_IKAPC(res);
	return(InsResult(res & 0xffff));
}
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}

InsResult InsCameraIkap::SaveBoardConfigurationToFile()
{
	ITKSTATUS res = ItkManInitialize();
	int ret = IK_RTN_OK;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return InsResult::INSCSTATUS_NO_FILE;
	}
	SHGetPathFromIDList(idl, szBuffer);
	char configFilename[128];
	TcharToChar(szBuffer, configFilename);

	//char configFilename[sizeof(szBuffer) + sizeof(".vlcf")]; // 包含.szBuffer和".vlcf"的空间
	//
	//// 复制szBuffer到configFilename
	//for (int i = 0; i < sizeof(szBuffer); i++) {
	//	configFilename[i] = szBuffer[i];
	//}
	//// 添加.vlcf后缀
	//strcat_s(configFilename, sizeof(configFilename), ".vlcf");
	

	// 添加 .vlcf 后缀
	//strcat_s(configFilename, sizeof(configFilename), ".vlcf");
	///添加.vlcf后缀
	//std::string path = "D:/1/1.vlcf";
	strcat_s(configFilename, ".vlcf");
	//// 导入配置文件。
	////
	//// Load configuration file.
	//if (GetOption(configFilename) == false)
	//{
	//	printf("Fail to get configuration, using default setting!\n");
	//}
	//else
	//{
		//ret = IKapLoadConfigurationFromFile(hBoard, configFilename);
		//CHECK_IKAPBOARD(ret);

		//ret = IKapSaveConfigurationToFile(ins_hBoard, (char*)path.c_str());
		ret = IKapSaveConfigurationToFile(ins_hBoard, configFilename);
		///ret = ItkDevSaveConfigurationToFile(ins_hBoard, configFilename);
		CHECK_IKAPBOARD(ret);
		return(InsResult(ret|0x1000));
	//}
}

InsResult InsCameraIkap::LoadLocalFile()
{
	int ret = IK_RTN_OK;
	//char configFilename[MAX_PATH];
	//char configFilename[128] = "C:/Users/INSNEX/Desktop/Ins2.5D_camera2023.8.7/InsCameraIkap/InsCameraIkap/OK.vlcf";
	char configFilename[128] = "./200K.vlcf";
	ret = IKapLoadConfigurationFromFile(ins_hBoard, configFilename);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	return (InsResult)0;
}


/* 手动选相机配置文件
InsResult InsCameraIkap::SaveCameraConfigurationToFile()
{
	ITKSTATUS res = ItkManInitialize();
	int ret = IK_RTN_OK;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("从下面选文件夹目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return InsResult::INSCSTATUS_NO_File;
	}
	SHGetPathFromIDList(idl, szBuffer);
	char configFilename[128];
	TcharToChar(szBuffer, configFilename);

	res = ItkDevSaveConfigurationToFile(ins_hCamera, configFilename);
	CHECK(res);
	return (InsResult)(res & 0xffff);
	//}
}
*/




double InsCameraIkap::get_ins_exposure()//cankao
{
	ITKFEATURE hFeature;
	log_message("Feature is Exposure Time");
	int	res = ItkDevAllocFeature(ins_hCamera, "ExposureTime", &hFeature);//ExposureTime
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		log_message("Failed to allocate feature ExposureTime , error code:" + std::to_string(res));
		return(1001);
	}
	
	double val = 0;
	// 获取Double特征值
	res = ItkFeatureGetDouble(hFeature, &val);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(1002);
	}
	
	res = ItkDevFreeFeature(hFeature);
	ins_exposure_ = val;
	
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(1003);
	}
	return val;

	//ITKSTATUS res = ITKSTATUS_OK;
	//double pValue = 0;
	//// Get feature value
	//res = ItkDevGetDouble(ins_hCamera, "ExposureTime", &pValue);

	//return pValue;
}

InsResult InsCameraIkap::set_ins_exposure(double val)
{
	ITKFEATURE hFeature;
	int	res = ItkDevAllocFeature(ins_hCamera, "ExposureTime", &hFeature);
	CHECK(res);
	RETURNERRC(res);
	//	// 获取整型特征值
	//	res = ItkFeatureGetDouble(hFeature, &val);
	//	CHECK(res);
	//	// 设置整型特征值
	//	val = 20.0;
	res = ItkFeatureSetDouble(hFeature, val);
	CHECK(res);
	RETURNERRC(res);
	ins_exposure_ = val;
	res = ItkDevFreeFeature(hFeature);
	return InsResult::INSCSTATUS_OK;
}


double InsCameraIkap::get_ins_line_period_time()
{
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("8KCXP") != string::npos)
	{

		res = ItkDevAllocFeature(ins_hCamera, "AcquisitionLineRate", &hFeature);

	}
	else if (namecam.find("16KCXP") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "AcquisitionLinePeriod", &hFeature);
	}
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "LinePeriodTime", &hFeature);
	}

	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}
	double val = 0;
	// 获取浮点型特征值
	res = ItkFeatureGetDouble(hFeature, &val);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}
	ins_line_period_time_ = val;
	res = ItkDevFreeFeature(hFeature);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}

	if (namecam.find("CXP") != string::npos)
	{

		return 1000000/val;

	}
	else
	{
		return val;

	}
	
}

InsResult InsCameraIkap::set_ins_line_period_time(double val)//0226PERIOD
{
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("8KCXP") != string::npos)
	{

		val = (1000000 / val);
		res = ItkDevAllocFeature(ins_hCamera, "AcquisitionLineRate", &hFeature);
		
	}

	else if (namecam.find("16KCXP") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "AcquisitionLinePeriod", &hFeature);
	}

	//======================
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "LinePeriodTime", &hFeature); //AcquisitionLinePeriod
	
    }
	CHECK(res);
	RETURNERRC(res);

	double max = 0, min = 0, inc = 0;
	/*
	res = ItkFeatureGetDoubleMax(hFeature, &max);
	CHECK(res);
	cout << "max" << max << endl;
	res = ItkFeatureGetDoubleMin(hFeature, &min);
	cout << "min" << min << endl;

	CHECK(res);
	res = ItkFeatureGetDoubleInc(hFeature, &inc);
	cout << "inc" << inc << endl;
	*/
	CHECK(res);
	//	// 获取整型特征值
	//	res = ItkFeatureGetDouble(hFeature, &val);
	//	CHECK(res);
	//	// 设置整型特征值
	//	val = 20.0;
	res = ItkFeatureSetDouble(hFeature, val);
	CHECK(res);
	RETURNERRC(res);
	ins_line_period_time_ = val;
	res = ItkDevFreeFeature(hFeature);
	return InsResult::INSCSTATUS_OK;
}


InsResult InsCameraIkap::set_ins_digital_gain(double val)
{
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("4K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "DigitalGain", &hFeature);
	}
	else if (namecam.find("CXP") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "DigitalGain", &hFeature);
	}
	else if (namecam.find("8K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "Gain", &hFeature);
	}
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "Gain", &hFeature);
	}
	CHECK(res);
	RETURNERRC(res);
	res = ItkFeatureSetDouble(hFeature, val);
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);
	RETURNERRC(res);
	ins_digital_gain_ = val;
	return InsResult::INSCSTATUS_OK;
}

double InsCameraIkap::get_ins_digital_gain()
{
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("4K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "DigitalGain", &hFeature);
	}
	else if (namecam.find("CXP") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "DigitalGain", &hFeature);
	}
	else if (namecam.find("8K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "Gain", &hFeature);
	}
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "Gain", &hFeature);
	}
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}
	double val = 0.0;
	// 获取整型特征值
	res = ItkFeatureGetDouble(hFeature, &val);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}
	ins_digital_gain_ = val;
	res = ItkDevFreeFeature(hFeature);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return(DBL_MAX);
	}
	return val;
}

InsResult InsCameraIkap::set_ins_PAC_gain(char* str_)
{

	char Cxp16Gain[16];
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("4K")!= string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "PGAGain", &hFeature);
	}
	else if (namecam.find("8KCXP") != string::npos)//cxp相机名字里也有8k
	{
		if (*str_ == 'x')
		{
			*str_ = 'X';
		}
		 return set_ins_analog_gain(str_);
	}
	else if (namecam.find("16KCXP") != string::npos)

	{
		std::string temStr = "Gain_";
		temStr = temStr + str_;
		
		strncpy(Cxp16Gain, temStr.c_str(), temStr.size() + 1);

		res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	}

	else if (namecam.find("8K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	}
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	}
	CHECK(res);
	RETURNERRC(res);
	// 设置枚举类型特征的值
	if (namecam.find("16KCXP") != string::npos)
	{
		res = ItkFeatureFromString(hFeature, Cxp16Gain);

	}
	else
	{
		res = ItkFeatureFromString(hFeature, str_);
	}
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return (InsResult)res;
	}

	return InsResult::INSCSTATUS_OK;
}

InsResult InsCameraIkap::get_ins_PAC_gain(char* val)
{
	ITKFEATURE hFeature;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("4K") != string::npos)
	{
		res = ItkDevAllocFeature(ins_hCamera, "PGAGain", &hFeature);
	}
	else if (namecam.find("CXP") != string::npos)//cxp相机名字里也有8k
	{

		InsResult retcam = get_ins_analog_gain(val);
		if (*val == 'X')
		{
			*val = 'x';
		}
		else if (*val =='G')
		{
			*val = *(val + 5);
			*(val+1) = *(val + 6);
			*(val + 2) = '\0'; //Gain_x1 -> x1
		}
		return retcam;
	}
	else if (namecam.find("8K") != string::npos)//8k CameraLink
	{
		res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	}
	else
	{
		res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	}
	CHECK(res);
	RETURNERRC(res);
	// 获取枚举类型特征的值
	uint32_t enumValSize = 0;
	res = ItkFeatureToString(hFeature, NULL, &enumValSize);
	res = ItkFeatureToString(hFeature, ins_PAC_gain_, &enumValSize);
	res = ItkFeatureToString(hFeature, val, &enumValSize);
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//只要使用了必须释放
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return (InsResult)res;
	}
	return InsResult::INSCSTATUS_OK;
}

InsResult InsCameraIkap::set_ins_analog_gain(char* val)
{
	ITKFEATURE hFeature;
	int res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);//创建关于相机特征值的指针
	CHECK(res);
	RETURNERRC(res);
	// 设置枚举类型特征的值
	res = ItkFeatureFromString(hFeature, val);//利用这个指针去找输入的特征值
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//释放指针
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return (InsResult)res;
	}
	return InsResult::INSCSTATUS_OK;
}

InsResult InsCameraIkap::get_ins_analog_gain(char* val)
{
	ITKFEATURE hFeature;
	int res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);
	CHECK(res);
	RETURNERRC(res);
	// 获取枚举类型特征的值
	uint32_t enumValSize = 0;
	res = ItkFeatureToString(hFeature, NULL, &enumValSize);
	res = ItkFeatureToString(hFeature, ins_PAC_gain_, &enumValSize);
	res = ItkFeatureToString(hFeature, val, &enumValSize);
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//后加的，必须释放
	CHECK(res);
	if (res != ITKSTATUS_OK)
	{
		return (InsResult)res;
	}
	return InsResult::INSCSTATUS_OK;
}

InsResult InsCameraIkap::set_ins_shaft_encoder1_min_width_(int ins_shaft_encoder1_min_width_)
{
	int ret = 0;
	ret = IKapSetInfo(ins_hBoard, IKP_SHAFT_ENCODER1_MIN_WIDTH, ins_shaft_encoder1_min_width_);
	if (ret != IK_RTN_OK)
	{
		//printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(ins_hBoard);
		return InsResult(ret | 0x1000);
	}
	InsCameraIkap::ins_shaft_encoder1_min_width_ = ins_shaft_encoder1_min_width_;
	return InsResult(ret | 0x1000);
}

int InsCameraIkap::get_ins_shaft_encoder1_min_width_()
{
	int ret = 0;
	int ins_shaft_encoder1_min_width_ = 0;
	
	ret = IKapGetInfo(ins_hBoard, IKP_SHAFT_ENCODER1_MIN_WIDTH, &ins_shaft_encoder1_min_width_);
	if ( ret != IK_RTN_OK)
	{
		//printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(ins_hBoard);
		return INT_MAX;
	}
	InsCameraIkap::ins_shaft_encoder1_min_width_ = ins_shaft_encoder1_min_width_;
	return ins_shaft_encoder1_min_width_;
}
/*
InsResult InsCameraIkap::getinfo()
{
	HANDLE hDev = (HANDLE)ins_hBoard;
	unsigned char* pUserBuffer = NULL;
	int nFrameSize = 0;
	int nFrameCount = 0;
	int nFrameIndex = 0;
	int nImageType = 0;
	int nImageWidth = 0;
	int nImageHeight = 0;
	int nValue = 0;

	IKAPBUFFERSTATUS status;
	IKapGetInfo(hDev, IKP_FRAME_SIZE, &nFrameSize);
	InsCameraIkap::ins_frame_size_ = nFrameSize;
	IKapGetInfo(hDev, IKP_IMAGE_TYPE, &nImageType);
	InsCameraIkap::ins_type_ = nImageType;
	IKapGetInfo(hDev, IKP_IMAGE_WIDTH, &nImageWidth);
	InsCameraIkap::ins_width_ = nImageWidth;
	IKapGetInfo(hDev, IKP_IMAGE_HEIGHT, &nImageHeight);
	InsCameraIkap::ins_height_ = nImageHeight;
	IKapGetInfo(hDev, IKP_BOARD_TRIGGER_MODE, &nValue);
	InsCameraIkap::ins_trigger_mode_ = nValue;
	IKapGetInfo(hDev, IKP_SOFTWARE_TRIGGER_WIDTH, &nValue);
	InsCameraIkap::ins_st_width_ = nValue;
	IKapGetInfo(hDev, IKP_SOFTWARE_TRIGGER_PERIOD, &nValue);
	InsCameraIkap::ins_st_period_ = nValue;
	IKapGetInfo(hDev, IKP_SOFTWARE_TRIGGER_COUNT, &nValue);
	InsCameraIkap::ins_st_count_ = nValue;
	IKapGetInfo(hDev, IKP_SOFTWARE_TRIGGER_DELAY, &nValue);
	InsCameraIkap::ins_st_delay_ = nValue;
	IKapGetInfo(hDev, IKP_SFOTWARE_TRIGGER_POLARITY, &nValue);
	InsCameraIkap::ins_st_polarity_ = nValue;
	IKapGetInfo(hDev, IKP_SOFTWARE_TRIGGER_SYNC_MODE, &nValue);
	InsCameraIkap::ins_st_sync_mode_ = nValue;
	IKapGetInfo(hDev, IKP_GENERAL_INPUT1_TRIGGER_MODE, &nValue);
	InsCameraIkap::ins_io_trigger_mode_ = nValue;
	IKapGetInfo(hDev, IKP_GENERAL_INPUT1_POLARITY, &nValue);
	InsCameraIkap::ins_io_polarity_ = nValue;
	IKapGetInfo(hDev, IKP_GENERAL_INPUT1_MIN_WIDTH, &nValue);
	InsCameraIkap::ins_io_min_width_ = nValue;
	IKapGetInfo(hDev, IKP_STROBE_TRIGGER_SOURCE, &nValue);
	InsCameraIkap::ins_strobe_source_ = nValue;
	IKapGetInfo(hDev, IKP_STROBE_METHOD, &nValue);
	InsCameraIkap::ins_strobe_method_ = nValue;
	IKapGetInfo(hDev, IKP_STROBE_POLARITY, &nValue);
	InsCameraIkap::ins_strobe_polarity_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_CHANNEL, &nValue);
	InsCameraIkap::ins_et_channel_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_MULTIPlY_FACTOR, &nValue);
	InsCameraIkap::ins_et_multi_factor_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_MIN_WIDTH, &nValue);
	InsCameraIkap::ins_et_min_width_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_VALID_DIRECTION, &nValue);
	InsCameraIkap::ins_et_valid_direction_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION, &nValue);
	InsCameraIkap::ins_et_reverse_comp_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE, &nValue);
	InsCameraIkap::ins_et_quad_frequency_source_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_LIMIT, &nValue);
	InsCameraIkap::ins_et_reverse_compensation_limit_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_TYPE, &nValue);
	InsCameraIkap::ins_et_clock_duty_compensation_type_ = nValue;
	IKapGetInfo(hDev, IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_WIDTH, &nValue);
	InsCameraIkap::ins_et_clock_duty_compensation_width_ = nValue;
	
	return InsResult::INSCSTATUS_OK;

}
*/
int InsCameraIkap::get_ins_frame_id_()
{
	return frame_id_;
}

int InsCameraIkap::freerunReset(int a)
{
	HANDLE hVul = ins_hBoard;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;


	if (a == 0)
	{

		if (namecam.find("16KCXP") != string::npos)//可能不仅是宽度0201 to do
		{
			int ret = 0;
			std::cout << "namecam : " << namecam << endl;
			ret = (int)set_ins_width(8000);

		}
	}
	else if ( a == 1)
	{

		if (namecam.find("16KCXP") != string::npos)//可能不仅是宽度0201 to do
		{
			int ret = 0;
			std::cout << "namecam : " << namecam << endl;
			ret = (int)set_ins_width(16000);

		}
	}


	return 0;
}


std::string generateSystemModel(const std::string& cameraModel, const std::string& lightSourceModel)
{
	// 相机
	size_t start = cameraModel.find("INS-LC-") + 7;
	size_t end = cameraModel.find('-', start);
	std::string cameraKeyPart = cameraModel.substr(start, end - start);

	// 光源
	std::string lightSourceKeyPart;
	size_t lightSourceStart = lightSourceModel.size() - 1;
	while (lightSourceStart >= 0 && !std::isdigit(lightSourceModel[lightSourceStart])) {
		--lightSourceStart;
	}
	lightSourceKeyPart = lightSourceModel.substr(lightSourceStart, 1);

	// 系统
	std::string systemModel = "INS-PMDL-25D-" + cameraKeyPart + lightSourceKeyPart + "M";

	return systemModel;
}





//----------读写单个寄存器------------
int InsCameraIkap::read_register(BoardID _id, uint16_t _address, uint32_t& _buffer)
{
	uint16_t buffer1[2] = { 0 };
	int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(_id, _address, buffer1);
	_buffer = (static_cast<uint32_t>(buffer1[0]) << 16) | (static_cast<uint32_t>(buffer1[1]));
	return ret;
}

int InsCameraIkap::write_register(BoardID _id, uint16_t _address, uint32_t _buffer)
{
	uint32_t valueToWrite = _buffer;
	uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
	uint16_t lowValue = valueToWrite & 0xFFFF;
	uint16_t buffer1[2] = { highValue,lowValue };
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)_id, _address, buffer1);
}
