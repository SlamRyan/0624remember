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
//�ʹ����йص�ֻдһ��壬�������йص�Ҫд�����
int PicNumCount = 8;//ȡ������

//ret ����1��ȷ��res����0��ȷ
#pragma comment (lib, "IKapBoard.lib")
#pragma comment( lib, "shlwapi.lib" )
using namespace std;

static DataPocessCallBack mCallback;
static OffLineCallBack removeCallback;
//static void mCallback(unsigned char *);(�����˺������ǲ��Ǳ���)



/* @brief����ӡ������Ϣ���˳�����
 * @param[in] InsResult����������ֵ��
 *
 * @brief��Print error message and exit the program.
 * @param[in] InsResult��Function return value. */
extern "C" INSCAMERA_API void printErrorAndExit(ITKSTATUS InsResult);

/* @brief����ӡ IKapBoard ������Ϣ���˳�����*/
extern "C" INSCAMERA_API void printIKapBoardErrorAndExit();

/* @brief����ӡ IKapC ������Ϣ���˳�����
 * @param[in] InsResult����������ֵ�� */
void printIKapCErrorAndExit(ITKSTATUS InsResult);

/* @brief���û�����س������˳����� */
void pressEnterToExit(void);
/*
/* @brief��ѡ���û������ļ���
 * @param[in] lpConfigFileName�������ļ�����
bool GetOption(char* lpConfigFileName);
bool GetCCFOption(char* lpConfigFileName);
*/
/* @brief����������豸��
 * @param[out] hCamera������豸�����
 * @param[out] hBoard���ɼ����豸����� */
extern "C" INSCAMERA_API InsResult ConfigureCamera(ITKDEVICE * hCamera, HANDLE * hBoard);

/* @brief�����òɼ����豸��
 * @param[in] hCamera������豸�����
 * @param[in] hBoard���ɼ����豸�����*/
extern "C" INSCAMERA_API void ConfigureFrameGrabber(ITKDEVICE hCamera, HANDLE hBoard);

/* @brief������ص�������
 *
 * @brief��Unregister callback functions. */
void UnRegisterCallback();

/* @brief����������ע��Ϊһ���ص���������ͼ��ɼ���ʼʱ�����������á�
 * @param[in] pContext�����������
 *
 * @brief��This function is registered as a callback function. When starting grabbing images, the function will be called.
 * @param[in] pContext��Input parameter. */
void CALLBACK OnGrabStart(void* pContext);

/* @brief����������ע��Ϊһ���ص���������һ֡ͼ��ɼ����ʱ�����������á�
 * @param[in] pContext�����������
 *
 * @brief��This function is registered as a callback function. When a frame of image grabbing ready, the function will be called.
 * @param[in] pContext��Input parameter. */
void CALLBACK OnFrameReady(void* pContext);

/* @brief����������ע��Ϊһ���ص���������ͼ��ɼ���ʱʱ�����������á�
 * @param[in] pContext�����������
 *
 * @brief��This function is registered as a callback function. When grabbing images time out, the function will be called.
 * @param[in] pContext��Input parameter. */
void CALLBACK OnTimeout(void* pContext);

/* @brief����������ע��Ϊһ���ص����������ɼ���֡ʱ�����������á�
 * @param[in] pContext�����������
 *
 * @brief��This function is registered as a callback function. When grabbing frame lost, the function will be called.
 * @param[in] pContext��Input parameter. */
void CALLBACK OnFrameLost(void* pContext);

/* @brief����������ע��Ϊһ���ص���������ͼ��ɼ�ֹͣʱ�����������á�
 * @param[in] pContext�����������
 *
 * @brief��This function is registered as a callback function. When stopping grabbing images, the function will be called.
 * @param[in] pContext��Input parameter. */

 /* @brief���ж� IKapBoard �����Ƿ�ɹ����á�
  * @param[in] InsResult����������ֵ��
  */
#define CHECK_IKAPBOARD(InsResult) if (InsResult != 1) printIKapBoardErrorAndExit()
  /* @brief���ж� IKapC �����Ƿ�ɹ����á�
   * @param[in] InsResult����������ֵ��
   */
#define CHECK_IKAPC(InsResult) if (InsResult != ITKSTATUS_OK) printIKapCErrorAndExit(InsResult)

   /* @brief���жϺ����Ƿ�ɹ����á�
	* @param[in] InsResult����������ֵ��
	*
	* @brief��Determine whether the function is called successfully.
	* @param[in] InsResult��Function return value. */
#define CHECK(InsResult) if (ITKSTATUS_OK != InsResult) printErrorAndExit(InsResult)






void CALLBACK OnGrabStop(void* pContext);





int InsCameraIkap::ins_width_ = 8200;//12.11ԭ4096  //0322
int InsCameraIkap::ins_height_ = 40000;//12.11ԭ40000
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

bool InsCameraIkap::cam_update_ = 0;//��һ��ˢ�²���
uint32_t InsCameraIkap::frame_id_ = 0;
int InsCameraIkap::ins_frame_size_ = 0;
int InsCameraIkap::ins_shaft_encoder1_min_width_ = 300;//12.11ԭ300

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

/* @brief���ж� IKapBoard �����Ƿ�ɹ����á�
 * @param[in] InsResult����������ֵ��
 *
 * @brief��Determine whether the IKapBoard function is called successfully.
 * @param[in] InsResult��Function return value. */
#define CHECK_IKAPBOARD(x) if (x != 1) printIKapBoardErrorAndExit()

 /* @brief���ж� IKapC �����Ƿ�ɹ����á�
  * @param[in] InsResult����������ֵ��
  *
  * @brief��Determine whether the IKapC function is called successfully.
  * @param[in] InsResult��Function return value. */
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
map<int, int> ins_num_realist_index;//���ҵ���INS�����źͶ�Ӧ��ʵ��������,ins�������Ŵ�1��ʼ
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

		// ö�ٿ����豸���������ڴ��豸ǰ��������� ItkManGetDeviceCount() ������
		//
		// Enumerate the number of available devices. Before opening the device, ItkManGetDeviceCount() function must be called.
		res = ItkManGetDeviceCount(&num_devices_);
		CHECK(res);
		RETURNERRC(res);

		// ��û�����ӵ��豸ʱ��
		//
		// When there is no connected devices.
		if (num_devices_ == 0)
		{
			fprintf(stderr, "No device.\n");
			printf("No camera.\n");
			return(InsResult(0x0032));
		}

		// ��ӡ���ӵ�����豸����Ϣ
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
				// ����ҵ� "PL"��������滻
				if (found != std::string::npos)
				{
					//cout << "�ҵ���PL";
					str.replace(found, 2, "INS-LC-");
					//cout << "�滻��PL";
				}

				size_t ktFound = str.find("KT");
				if (ktFound != std::string::npos)
				{
					str.replace(ktFound, 2, "M");
				}

				//��int InsCamera::ins_shaft_encoder1_min_width_ CXP����
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
				//����SN
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
				// ����ҵ� "PL"��������滻
				if (found != std::string::npos)
				{
					//cout << "�ҵ���PL";
					str.replace(found, 2, "INS-LC-");
					//cout << "�滻��PL";
				}

				size_t ktFound = str.find("KT");
				if (ktFound != std::string::npos)
				{
					str.replace(ktFound, 2, "M");
				}

				//��int InsCamera::ins_shaft_encoder1_min_width_ CXP����
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

				//����SN
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

	// �豸����������
	//
	// The number of device features.
	uint32_t  nFeatureCount = 0;

	// �������ơ�
	//
	// Feature name.
	char featureName[128];

	// �������Ƴ��ȡ�
	//
	// The length of feature name.
	uint32_t featureNameLen = 128;

	// �������͡�
	//
	// The type of feature.
	uint32_t featureType = ITKFEATURE_VAL_TYPE_UNDEFINED;

	// ��ȡ�豸����������
	//
	// Get the number of device features.
	res = ItkDevGetFeatureCount(ins_hCamera, &nFeatureCount);
	CHECK(res);

	for (uint32_t i = 0; i < nFeatureCount; i++)
	{
		featureNameLen = 128;

		// ��ȡ�������ơ�
		//
		// Get the name of feature.
		res = ItkDevGetFeatureName(ins_hCamera, i, featureName, &featureNameLen);
		CHECK(res);

		// ��ȡ�������͡�
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

	// ö�ٿ���������������ڴ����ǰ��������� ItkManGetDeviceCount() ������
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// ��û�����ӵ����ʱ��
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// ��CameraLink�����
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// ��ȡ����豸��Ϣ��
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);
		
		CHECK_IKAPC(res);
		RETURNERRC(res);
		printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);
		// ���豸Ϊ CameraLink ��������к���ȷʱ��
		if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
		{
			ITK_CL_DEV_INFO cl_board_info;
			// �������
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, hCamera);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// ��ȡ CameraLink ����豸��Ϣ��
			//
			// Get CameraLink camera device information.
			res = ItkManGetCLDeviceInfo(i, &cl_board_info);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// �򿪲ɼ�����
			//
			// Open frame grabber.
			*hBoard = IKapOpen(cl_board_info.HostInterface, cl_board_info.BoardIndex);
			if (*hBoard == INVALID_HANDLE_VALUE) {
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
				
			//CXP����
			return InsResult::INSCSTATUS_OK;
		}

		if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "itek-opto electronics") == 0)
		{

			ITK_CXP_DEV_INFO cxp_cam_info;
			IKAP_CXP_BOARD_INFO cxp_board_info;

			// �������
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, hCamera);
			CHECK_IKAPC(res);

			// ��ȡCoaXPress����豸��Ϣ��
			//
			// Get CoaXPress camera device information.
			res = ItkManGetCXPDeviceInfo(i, &cxp_cam_info);
			CHECK_IKAPC(res);

			// �򿪲ɼ�����
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
{    // ����豸�����
	ins_hCamera = NULL;

	// �ɼ����豸�����
	ins_hBoard = INVALID_HANDLE_VALUE;
}


InsResult InsCameraIkap::PrintCamera()
{
	ITKSTATUS res = ITKSTATUS_OK;
	uint32_t numCameras = 0;

	// ö�ٿ���������������ڴ����ǰ��������� ItkManGetDeviceCount() ������
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	cout << "numCameras:" << numCameras<<endl;
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// ��û�����ӵ����ʱ��
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// ��CameraLink�����
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// ��ȡ����豸��Ϣ��
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);
		CHECK_IKAPC(res);
		RETURNERRC(res);
		cout << "��"<< i << "̨�豸:" <<endl;
		printf("Using camera: serial: %s, name: %s, interface: %s, DeviceVersion: %s, FriendlyName: %s, ModelName: %s, UserDefinedName: %s, VendorName: %s.\n", 
			di.SerialNumber, di.FullName, di.DeviceClass,di.DeviceVersion,di.FriendlyName,di.ModelName,di.UserDefinedName,di.VendorName);
		// ���豸Ϊ CameraLink ��������к���ȷʱ��
		}
	return InsResult::INSCSTATUS_OK;
}






// ���òɼ����豸��
//
// Configure frame grabber device.
InsResult InsCameraIkap::ConfigureFrameGrabber()
{
	int ret = IK_RTN_OK;
	 // ����ͼ�񻺳���֡����
	 //
	 // Set frame count of buffer.
	ret = IKapSetInfo(ins_hBoard, IKP_FRAME_COUNT, BOARD_FRMAE_COUNT);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ���ó�ʱʱ�䡣
	//
	// Set time out time.
	int timeout = -1;
	ret = IKapSetInfo(ins_hBoard, IKP_TIME_OUT, timeout);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ���òɼ�ģʽ��
	//
	// Set grab mode.
	int grab_mode = IKP_GRAB_NON_BLOCK;
	ret = IKapSetInfo(ins_hBoard, IKP_GRAB_MODE, grab_mode);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);

	// ���ô���ģʽ��
	//
	// Set transfer mode.
	int transfer_mode = IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT;
	ret = IKapSetInfo(ins_hBoard, IKP_FRAME_TRANSFER_MODE, transfer_mode);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ע��ص�����
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
// ���òɼ����豸��
//
// Configure frame grabber device.
void ConfigureFrameGrabberIn(ITKDEVICE hCamera, HANDLE hBoard)
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	/*
	char configFilename[MAX_PATH];

	// ���������ļ���
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
	// ����ͼ�񻺳���֡����
	//
	// Set frame count of buffer.
	ret = IKapSetInfo(hBoard, IKP_FRAME_COUNT, BOARD_FRMAE_COUNT);
	CHECK_IKAPBOARD(ret);

	// ���ó�ʱʱ�䡣
	//
	// Set time out time.
	int timeout = -1;
	ret = IKapSetInfo(hBoard, IKP_TIME_OUT, timeout);
	CHECK_IKAPBOARD(ret);

	// ���òɼ�ģʽ��
	//
	// Set grab mode.
	int grab_mode = IKP_GRAB_NON_BLOCK;
	ret = IKapSetInfo(hBoard, IKP_GRAB_MODE, grab_mode);
	CHECK_IKAPBOARD(ret);

	// ���ô���ģʽ��
	//
	// Set transfer mode.
	int transfer_mode = IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT;
	ret = IKapSetInfo(hBoard, IKP_FRAME_TRANSFER_MODE, transfer_mode);
	CHECK_IKAPBOARD(ret);

	// ע��ص�����
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





// ��ӡ IKapBoard ������Ϣ���˳�����
//
// Print IKapBoard error message and exit the program.
void printIKapBoardErrorAndExit()
{
	// ��ȡ��������Ϣ��
	//
	// Get error code message.
	IKAPERRORINFO InsResult;
	IKapGetLastError(&InsResult, 1);

	// ��ӡ������Ϣ��
	//
	// Print error message.
	fprintf(stderr, "INSBoard:Index: %d, error code:%04x\n", InsResult.uBoardIndex, InsResult.uErrorCode);
	std::cout<<stderr<< "INSBoard:Index: %d, error code:%04x\n"<< InsResult.uBoardIndex<< InsResult.uErrorCode;

	//pressEnterToExit();
	//exit(EXIT_FAILURE);
}

// ��ӡ IKapC ������Ϣ���˳�����
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




// ����ص�������
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

// �û�����س������˳�����
//
// Users enter Enter to exit the program.
void pressEnterToExit(void)
{
	fprintf(stderr, "\nPress enter to exit.\n");
	while (getchar() != '\n');
}

// ��������ע��Ϊһ���ص���������ͼ��ɼ���ʼʱ�����������á�
// 
// This function is registered as a callback function. When starting grabbing images, the function will be called.
void CALLBACK OnGrabStart(void* pContext)
{
	printf("Start grabbing image.\n");
}

// ��������ע��Ϊһ���ص���������һ֡ͼ��ɼ����ʱ�����������á�
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

	// ��ͼ�񻺳�����ʱ��
	//
	// When the buffer is full.
	if (status.uFull == 1)
	{
		/*//0202
		// ��ȡһ֡ͼ��Ĵ�С��
		//
		// Get the size of a frame of image.
		IKapGetInfo(hDev, IKP_FRAME_SIZE, &nFrameSize);
		InsCameraIkap::ins_frame_size_ = nFrameSize;
		cout << "InsCameraIkap::ins_frameSize:" << InsCameraIkap::ins_frame_size_ << endl;*/
		int nwidth = 0;
		int ret = IKapGetInfo(hDev, IKP_IMAGE_WIDTH, &nwidth);

		int nheight = 0;
		int ret2 = IKapGetInfo(hDev, IKP_IMAGE_HEIGHT, &nheight);

		// ��ȡ��������ַ��
		//
		// Get the buffer address.
		IKapGetBufferAddress(hDev, nFrameIndex, (void**)&pUserBuffer);
		
		// ��ȡ���ݸ�ʽ
		IKapGetInfo(hDev, IKP_DATA_FORMAT, &nDataFormat);
		
		// ��ȡͼ������
		IKapGetInfo(hDev, IKP_IMAGE_TYPE, &nImageType);


		InsCameraIkap::frame_id_++;
		InsPMDL_Buffer* insBuffer = new InsPMDL_Buffer;
		insBuffer->pdata = pUserBuffer;											//ͼ��ָ��
		insBuffer->width = nwidth;											//ͼ����
		insBuffer->height = nheight;										//ͼ��߶�
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
		insBuffer->step = nBytesPerPixel;										//ͼ�񲽳�������ͨ������ͼ���ȣ��Ұ���һͨ����RGB��ɫ��ͨ������λ��������ȣ����ȡ������8
																				//IKapBoard�û������ֲ�14ҳ
		insBuffer->imageTxpe = (INS_PMDL_PixelFormat)nImageType;				//ͼ�����ظ�ʽ,����IKAP���ͼ�����ͣ�mono8���֣�	
																				
		/*  ��Щ��û�Ž�ö��
			INS_PMDL_IMAGE_TYPE_VAL_COLORFUL = 1,
			INS_PMDL_IMAGE_TYPE_VAL_RGBC = 2,
			INS_PMDL_IMAGE_TYPE_VAL_BGR = 3,
			INS_PMDL_IMAGE_TYPE_VAL_BGRC = 4,
			INS_PMDL_IMAGE_TYPE_VAL_YUV422 = 5,*/
		insBuffer->frameIndex = frame_id_;		  //ͼ�� id
		//-------------------------
		//--------------------------------8k�ĺ�ע��
		//ITKDEV_INFO di;
		//ItkManGetDeviceInfo(mi_camera, &di);// to do
		//string namecam = di.FullName;
		//int res = 0;
		
		//if (namecam.find("8KCXP") != string::npos)
		//{
		//	insBuffer->pdata = pUserBuffer+ 2*nwidth; //ǰ2��6
		//	insBuffer->height = nheight - 8;
		//}
		//else if (namecam.find("8KCL") != string::npos) 
		//{
		//	cout << "8KCL" << endl;
		//	insBuffer->pdata = pUserBuffer+ 4*nwidth; //ǰ4��4
		//	insBuffer->height = nheight - 8;
		//}
		//---------------------

		//-----------------------------
		mCallback(insBuffer);
		delete insBuffer;
		insBuffer = nullptr;
	}
}

// ��������ע��Ϊһ���ص���������ͼ��ɼ���ʱʱ�����������á�
// 
// This function is registered as a callback function. When grabbing images time out, the function will be called.
void CALLBACK OnTimeout(void* pContext)
{
	printf("Grab image timeout.\n");
}

// ��������ע��Ϊһ���ص����������ɼ���֡ʱ�����������á�
// 
// This function is registered as a callback function. When grabbing frame lost, the function will be called.
void CALLBACK OnFrameLost(void* pContext)
{
	printf("Grab frame lost.\n");
}

// ��������ע��Ϊһ���ص���������ͼ��ɼ�ֹͣʱ�����������á�
// 
// This function is registered as a callback function. When stopping grabbing images, the function will be called.
void CALLBACK OnGrabStop(void* pContext)
{
	printf("Stop grabbing image.\n");
}
/*
// ѡ���û������ļ���
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
// ѡ���û������ļ���
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

	// ö�ٿ���������������ڴ����ǰ��������� ItkManGetDeviceCount() ������
	// Enumerate the number of available cameras. Before opening the camera, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&numCameras);
	CHECK_IKAPC(res);
	RETURNERRC(res);

	// ��û�����ӵ����ʱ��
	//
	// When there is no connected cameras.
	if (numCameras == 0)
	{
		printf("No camera.\n");
		return(InsResult::INSCSTATUS_NO_CAMERA);
	}
	// ��CameraLink�����
	// Open CameraLink camera.
	for (uint32_t i = 0; i < numCameras; i++)
	{
		ITKDEV_INFO di;

		// ��ȡ����豸��Ϣ��
		// Get camera device information.
		res = ItkManGetDeviceInfo(i, &di);

		CHECK_IKAPC(res);
		RETURNERRC(res);
		printf("Using camera: serial: %s, name: %s, interface: %s.\n", di.SerialNumber, di.FullName, di.DeviceClass);
		// ���豸Ϊ CameraLink ��������к���ȷʱ��
		if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
		{
			ITK_CL_DEV_INFO cl_board_info;
			// �������
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// ��ȡ CameraLink ����豸��Ϣ��
			//
			// Get CameraLink camera device information.
			res = ItkManGetCLDeviceInfo(i, &cl_board_info);
			CHECK_IKAPC(res);
			RETURNERRC(res);
			// �򿪲ɼ�����
			//
			// Open frame grabber.
			ins_hBoard = IKapOpen(cl_board_info.HostInterface, cl_board_info.BoardIndex);
			if (ins_hBoard == INVALID_HANDLE_VALUE) {
				CHECK_IKAPC(IKStatus_OpenBoardFail);
				RETURNERRC(IKStatus_OpenBoardFail);
			}
			mi_camera = i;
			CamFullName = di.FullName;
			//CXP����
			return InsResult::INSCSTATUS_OK;
		}

		if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && (strcmp(di.VendorName, "itek-opto electronics") == 0|| strcmp(di.VendorName, "I-Tek OptoElectronics") == 0))
		{

			ITK_CXP_DEV_INFO cxp_cam_info;
			IKAP_CXP_BOARD_INFO cxp_board_info;

			// �������
			//
			// Open camera.
			res = ItkDevOpen(i, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
			CHECK_IKAPC(res);

			// ��ȡCoaXPress����豸��Ϣ��
			//
			// Get CoaXPress camera device information.
			res = ItkManGetCXPDeviceInfo(i, &cxp_cam_info);
			CHECK_IKAPC(res);

			// �򿪲ɼ�����
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



InsResult InsCameraIkap::ConnectIndexCamera(int _index)//�ɼ���   //0506 ��ӡ�������
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
	//	// �ҵ���ָ����������ʹ�� iter->second ���ʶ�Ӧ��ֵ
	//	// iter->first �Ǽ���iter->second ��ֵ
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



	// ���˷�����豸������¼ÿ���豸����Ϣ
	for (uint32_t i = 0; i < deviceCount; ++i) {
		res = ItkManGetDeviceInfo(i, &di);
		if (res != ITKSTATUS_OK) {
			log_message("Failed to get device info for index: " + std::to_string(i) + ", error code: " + std::to_string(res));
			continue;
		}
		log_message("Device index: " + std::to_string(i) + ", serial: " + std::string(di.SerialNumber) + ", name: " + std::string(di.FullName) + ", interface: " + std::string(di.DeviceClass));
		if ((strcmp(di.DeviceClass, "CameraLink") == 0 || strcmp(di.DeviceClass, "CoaXPress") == 0) && strcmp(di.SerialNumber, "") != 0) {
			log_message("Valid camera found at index: " + std::to_string(i) + ", serial: " + std::string(di.SerialNumber));
			//_index = i; // ʹ���ҵ��ĵ�һ����Ч�������
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
	
	// ���豸Ϊ CameraLink ��������к���ȷʱ��
	if (strcmp(di.DeviceClass, "CameraLink") == 0 && strcmp(di.SerialNumber, "") != 0 && strcmp(di.VendorName, "Itek") == 0)
	{
		ITK_CL_DEV_INFO cl_board_info;
		// �������
		//
		// Open camera.
		res = ItkDevOpen(_index, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("3: " + std::to_string(res));
			RETURNERRC(res);
		}
		// ��ȡ CameraLink ����豸��Ϣ��
		//
		// Get CameraLink camera device information.
		res = ItkManGetCLDeviceInfo(_index, &cl_board_info);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("4: " + std::to_string(res));
			RETURNERRC(res);
		}
		// �򿪲ɼ�����
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
		//CXP����
		return InsResult::INSCSTATUS_OK;
	}

	if (strcmp(di.DeviceClass, "CoaXPress") == 0 && strcmp(di.SerialNumber, "") != 0 && (strcmp(di.VendorName, "itek-opto electronics") == 0 || strcmp(di.VendorName, "I-Tek OptoElectronics") == 0))
	{

		ITK_CXP_DEV_INFO cxp_cam_info;
		IKAP_CXP_BOARD_INFO cxp_board_info;

		// �������
		//
		// Open camera.
		res = ItkDevOpen(_index, ITKDEV_VAL_ACCESS_MODE_EXCLUSIVE, &ins_hCamera);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("1: " + std::to_string(res));
			RETURNERRC(res);
		}
		// ��ȡCoaXPress����豸��Ϣ��
		//
		// Get CoaXPress camera device information.
		res = ItkManGetCXPDeviceInfo(_index, &cxp_cam_info);
		CHECK_IKAPC(res);
		if (res != ITKSTATUS_OK)
		{
			log_message("2: " + std::to_string(res));
			RETURNERRC(res);
		}
		// �򿪲ɼ�����
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

	// ö�ٿ����豸���������ڴ��豸ǰ��������� ItkManGetDeviceCount() ������
	//
	// Enumerate the number of available devices. Before opening the device, ItkManGetDeviceCount() function must be called.
	res = ItkManGetDeviceCount(&num_devices_);
	CHECK(res);
	RETURNERRC(res);

	// ��û�����ӵ��豸ʱ��
	//
	// When there is no connected devices.
	if (num_devices_ == 0)
	{
		fprintf(stderr, "No device.\n");
		printf("No camera.\n");
		return(InsResult(0x0032));
	}

	// ��ӡ���ӵ�����豸����Ϣ
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
			// ����ҵ� "PL"��������滻
			if (found != std::string::npos)
			{
				//cout << "�ҵ���PL";
				str.replace(found, 2, "INS-LC-");
				//cout << "�滻��PL";
			}

			size_t ktFound = str.find("KT");
			if (ktFound != std::string::npos)
			{
				str.replace(ktFound, 2, "M");
			}

			//��int InsCamera::ins_shaft_encoder1_min_width_ CXP����
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
			// ����ҵ� "PL"��������滻
			if (found != std::string::npos)
			{
				//cout << "�ҵ���PL";
				str.replace(found, 2, "INS-LC-");
				//cout << "�滻��PL";
			}

			size_t ktFound = str.find("KT");
			if (ktFound != std::string::npos)
			{
				str.replace(ktFound, 2, "M");
			}

			//��int InsCamera::ins_shaft_encoder1_min_width_ CXP����
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
	// ���������ļ���
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
	// ���������ļ���
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
	// ���������ļ���
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
	if (found == string::npos) return InsResult::INSSTATUS_FILENAME_ERROR;//�ļ���ַ����
	ofstream writeFile(filename_, std::ios::out | std::ios::trunc);
	cout << "configure board file" << endl;


	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;


	//----------------------//���Դ���� ����
	writeFile << setw(10) << "CamNum :" << setw(20) << std::to_string(CameraNum) << "\n";//�������
	writeFile << setw(10) << "LigNum :" << setw(20) << std::to_string(LightNum) << "\n";//��Դ����
	writeFile << setw(10) << "LocalIP:" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->localIP) << "\n";//����IP
	writeFile << setw(10) << "LightIP:" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.ip_addr) << "\n";//��ԴIP
	writeFile << setw(10) << "Board  :" << setw(20) << std::to_string(SystemInfo.CamInfo.id[0]) << "\n";//�忨��
	writeFile << setw(10) << "Length :" << setw(20) <<(SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length) << "\n";//��ԭ�峤��


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
	

	
	if (namecam.find("16KCXP") != string::npos)//��������16KCXP   Gain_x1  8
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
	else                    //����Ϊ x10  4
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
	// ����ö������������ֵ
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
	// ��ȡö������������ֵ
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
		const char* fileExtension = strrchr(filename_, '.'); // ��ȡ���һ�����λ��
		if (!(fileExtension && strcmp(fileExtension, ".icf") == 0))
		{
			std::cout << "��ȡʧ��;" << endl;
			return InsResult::INSSTATUS_FILENAME_ERROR;  // �ļ���׺���� ".icf"
		}
	}
	ifstream readFile(filename_, std::ios::in);
	if (!readFile.is_open())
	{
		std::cout << "��ȡʧ��;" << endl;
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

	getline(readFile, line);//��ȡ������  //���� 0511
	std::cout << "Newname:" << line << endl;
	//-----------------
	if (namecam.find("4K") != string::npos)//��������4K
	{
		if (line.find("CL4K") == string::npos)//����icf��һ�в���CL4K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}

	else if (namecam.find("8KCXP") != string::npos)//0226
	{
		if (line.find("CXP8K") == string::npos)//����icf��һ�в���CXP8K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}

	else if (namecam.find("16KCXP") != string::npos)//0226
	{
		if (line.find("CXP16K") == string::npos)//����icf��һ�в���CXP8K
		{
			return InsResult::INSCSTATUS_FILE_ERR;
		}
	}


	else if (namecam.find("8K") != string::npos)//��������8K
	{
		if (line.find("CL8K") == string::npos)//����icf��һ�в���8K
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
		l_tmpstr = l_readstr.substr(0, 10);//��0��ʼ��10��
		if (strcmp(l_tmpstr.c_str(),"    height") == 0)
		{
			l_tmpstr = l_readstr.substr(15, 15);
			_height = strtol(&(l_tmpstr[0]), &pEndTemp, 10);//10��ʾʮ����
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
				char PAC_gain_temp[3] = "  ";//��5λ
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
				char PAC_gain_temp[4] = "   ";//��5λ
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
	// �ȴ�ͼ��ɼ�������
	//
	// Wait for grabbing images finished.
	ret = IKapWaitGrab(ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ֹͣͼ��ɼ���
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

	if ((namecam.find("8KCXP") != string::npos )||(namecam.find("16KCXP") != string::npos))//���ܲ����ǿ��0201 to do
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
	// ֹͣͼ��ɼ���
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
	int TimeOut = INFINITE; // ��ʱʱ��
	int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // ͼ������
	int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
	int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // ���ݸ�ʽ
	int tapNum = 10; // Tap
	int ret = 0;
	/* ��� CC1 ����Դ */
	int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
	/* �ɼ�������Դ */
	int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT1;
	/* Ƶ������Դ */
	int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
	/* �ɼ����������� */
	int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;

	/* �������ɨ������ */


	//���㣨IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)���������ȷ����ֵΪ1����1==0������0��if������ִ��{}
	if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���ó�ʱʱ�� */
	if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TIME_OUT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ������ */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* �������ݸ�ʽ */
	if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
	{
		printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ���� */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_WIDTH, ins_width_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_WIDTH");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ��߶� */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���� Tap �� */
	if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ������� CC1 ����Դ */
	if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
	{
		printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���òɼ�������Դ */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����Ƶ������Դ */
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
	/* ���òɼ����������� */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	//ͨ�������ź�1, 2 ����ģʽ_��ƽ����
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	//CL �ɼ����Ƿ�У�� FVAL �ź�_��
	ret = IKapSetInfo(hVul, IKP_CHECK_FRAME_VALID_SIGNAL, 0);
	RETURNERRCBOARD(ret);	
	return InsResult(0);
}


InsResult InsCameraIkap::ConfigureCXPBoard()
{
	HANDLE hVul = ins_hBoard;
	int TimeOut = INFINITE; // ��ʱʱ��
	int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // ͼ������
	int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
	int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // ���ݸ�ʽ
	int tapNum = 1; // ��CXP�汾��ֻ����Tap��Ϊ1
	int ret = 0;
	/* ��� CC1 ����Դ */
	int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
	/* �ɼ�������Դ */
	int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;//gai
	/* Ƶ������Դ */
	int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
	/* �ɼ����������� */
	int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;

	/* �������ɨ������ */
	if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���ó�ʱʱ�� */
	if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TIME_OUT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ������ */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}


	/* �������ݸ�ʽ */
	if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
	{
		printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ���� */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_WIDTH, ins_width_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_WIDTH");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����ͼ��߶� */
	if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
	{
		printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���� Tap �� */
	if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
	{
		printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ������� CC1 ����Դ */
	if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
	{
		printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ���òɼ�������Դ */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	/* ����Ƶ������Դ */
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
	/* ���òɼ����������� */
	if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
	{
		printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
		//freeFrameGrabberHandle(hVul);
		return InsResult(ret | 0x1000);
	}
	//ͨ�������ź�1, 2 ����ģʽ_��ƽ����
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_TRIGGER_MODE, IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL);
	RETURNERRCBOARD(ret);
	//����INTEGRATION_PARAM2
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM2, 720);
	RETURNERRCBOARD(ret);
	//����INTEGRATION_PARAM4
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM4, 720);
	RETURNERRCBOARD(ret);
	//����GENERAL_INPUT2_MIN_WIDTH
	ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_MIN_WIDTH, 300);
	RETURNERRCBOARD(ret);
	//����CXP_TRIGGER_OUTPUT_SELECTOR
	ret = IKapSetInfo(hVul, IKP_CXP_TRIGGER_OUTPUT_SELECTOR, 1);
	RETURNERRCBOARD(ret);
	//����SHAFT_ENCODER_DEBOUNCE
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_DEBOUNCE, IKP_SHAFT_ENCODER_DEBOUNCE_MIN);
	RETURNERRCBOARD(ret);
	//����SHAFT_ENCODER_TICK_MODE 
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_TICK_MODE, IKP_CXP_SHAFT_ENCODER_TICK_MODE_ANY_DIRECTION);
	RETURNERRCBOARD(ret);
	//����SHAFT_ENCODER_TICK_REVERSE_MODE 
	ret = IKapSetInfo(hVul, IKP_CXP_SHAFT_ENCODER_REVERSE_MODE, IKP_CXP_SHAFT_ENCODER_REVERSE_MODE_ANY_DIRECTION);
	RETURNERRCBOARD(ret);
	//����IKP_INTEGRATION_TRIGGER_FREQUENCY 
	ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_FREQUENCY, 199000);//gai
	RETURNERRCBOARD(ret);
	//CL �ɼ����Ƿ�У�� FVAL �ź�_��
	ret = IKapSetInfo(hVul, IKP_CHECK_FRAME_VALID_SIGNAL, 0);
	RETURNERRCBOARD(ret);
	return InsResult(0);
}

InsResult InsCameraIkap::ConfigureBoardWith250K()  //ÿ�θ��¶�Ҫ��
{
 	HANDLE hVul = ins_hBoard;
	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;


	if (namecam.find("8KCXP") != string::npos)//���ܲ����ǿ��0201 to do
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
	// ����ö������������ֵ
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
	// ����ö������������ֵ
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
	// ����ö������������ֵ
	res2 = ItkFeatureFromString(hFeature, temStr3.c_str());
	log_message(" res2 : " + std::to_string(res2));
	CHECK(res2);
	RETURNERRC(res2);
	res2 = ItkDevFreeFeature(hFeature);
	CHECK(res2);



	//---------------------------------------0407 ��Զ�֡����Ľ� ֻ��8k��
	bool Enable2 = true;
	int res3 = ItkDevAllocFeature(ins_hCamera, "StrobescopicLightEnable", &hFeature);
	CHECK(res3);
	RETURNERRC(res3);
	// ����Bool����������ֵ
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





	else if (namecam.find("16KCXP") != string::npos)//���ܲ����ǿ��0201 to do
	{
		int ret = 0;
		std::cout << "namecam : " << namecam << endl;  //0329
		ret = (int)set_ins_width(16000);//���Ҫ��0401  set_ins_width ��������ɼ�������� //16384

    //wyn

		ITKFEATURE hFeature;
	    std::string temStr = "On";
		int res = ItkDevAllocFeature(ins_hCamera, "TriggerMode", &hFeature);
		log_message("ItkDevAllocFeatrue TriggerMode : " + std::to_string(res));
	    CHECK(res);
	    RETURNERRC(res);
	    // ����ö������������ֵ
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
		int TimeOut = INFINITE; // ��ʱʱ��
		int ImageType = IKP_IMAGE_TYPE_VAL_MONOCHROME; // ͼ������
		int ScanType = IKP_SCAN_TYPE_VAL_LINEAR;
		int dataFormat = IKP_DATA_FORMAT_VAL_8Bit; // ���ݸ�ʽ
		int tapNum = 10; // Tap
		int ret = 0;
		/* ��� CC1 ����Դ */
		int CC1Source = IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1;
		/* �ɼ�������Դ */
		int integrationSource = IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
		/* �ɼ����������� */
		int integrationMethod = IKP_INTEGRATION_METHOD_VAL_1;
		/* Ƶ������Դ */
		int StrobeTriggerSource = IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1;
		/* �������ɨ������ */


		///0417 Taps Ten

        ITKFEATURE hFeature;
	    int res3 = ItkDevAllocFeature(ins_hCamera, "SensorTaps", &hFeature);//���������������ֵ��ָ��
	    CHECK(res3);
	    RETURNERRC(res3);
	    // ����ö������������ֵ
	    res3 = ItkFeatureFromString(hFeature, "Ten");//�������ָ��ȥ�����������ֵ
	    CHECK(res3);
	    RETURNERRC(res3);
	    res3 = ItkDevFreeFeature(hFeature);//�ͷ�ָ��
	    CHECK(res3);

		int res4 = ItkDevAllocFeature(ins_hCamera, "SynchronizationMode", &hFeature);
		CHECK(res4);
		RETURNERRC(res4);
		// ����ö������������ֵ
		res4 = ItkFeatureFromString(hFeature, "ExternalPulse");
		CHECK(res4);
		RETURNERRC(res4);
		res4 = ItkDevFreeFeature(hFeature);
		CHECK(res4);

		int res5 = ItkDevAllocFeature(ins_hCamera, "ExternalTriggerSource", &hFeature);
		CHECK(res5);
		RETURNERRC(res5);
		// ����ö������������ֵ
		res5 = ItkFeatureFromString(hFeature, "CC1");
		CHECK(res5);
		RETURNERRC(res5);
		res5 = ItkDevFreeFeature(hFeature);
		CHECK(res5);

		int res6 = ItkDevAllocFeature(ins_hCamera, "InputLineDetectionLevel", &hFeature);
		CHECK(res6);
		RETURNERRC(res6);
		// ����ö������������ֵ
		res6 = ItkFeatureFromString(hFeature, "Threshold_for_3V3_to_5V");
		CHECK(res6);
		RETURNERRC(res6);
		res6 = ItkDevFreeFeature(hFeature);
		CHECK(res6);

		//---------0517
        //Ϊ�ϵ籣����ӡ���������user2

		set_user_set_selector("User2");
	
	//--------------------------------0417 END-----------------------------
		if (ret = IKapSetInfo(hVul, IKP_SCAN_TYPE, ScanType) == false)
		{
			printErrorMessage("INSSetInfo", "INS_SCAN_TYPE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ���ó�ʱʱ�� */
		if (ret = IKapSetInfo(hVul, IKP_TIME_OUT, TimeOut) == false)
		{
			printErrorMessage("INSSetInfo", "INS_TIME_OUT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ����ͼ������ */
		if (ret = IKapSetInfo(hVul, IKP_IMAGE_TYPE, ImageType) == false)
		{
			printErrorMessage("INSSetInfo", "INS_IMAGE_TYPE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}


		/* �������ݸ�ʽ */
		if (ret = IKapSetInfo(hVul, IKP_DATA_FORMAT, dataFormat) == false)
		{
			printErrorMessage("INSSetInfo", "INS_DATA_FORMAT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ����ͼ���� */

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
		/* ����ͼ��߶� */
		if (ret = IKapSetInfo(hVul, IKP_IMAGE_HEIGHT, ins_height_) == false)
		{
			printErrorMessage("INSSetInfo", "INS_IMAGE_HEIGHT");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ���� Tap �� */
		if (ret = IKapSetInfo(hVul, IKP_TAP_NUMBER, tapNum) == false)
		{
			printErrorMessage("INSSetInfo", "INS_TAP_NUMBER");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ������� CC1 ����Դ */
		if (ret = IKapSetInfo(hVul, IKP_CC1_SOURCE, CC1Source) == false)
		{
			printErrorMessage("INSSetInfo", "INS_CC1_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ���òɼ�������Դ */
		if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_TRIGGER_SOURCE, integrationSource) == false)
		{
			printErrorMessage("INSSetInfo", "INS_INTEGRATION_TRIGGER_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ���òɼ����������� */
		if (ret = IKapSetInfo(hVul, IKP_INTEGRATION_METHOD, integrationMethod) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_METHOD");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ����GENERAL_INPUT1_POLARITY*/
		if (ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT1_POLARITY, 0) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT1_POLARITY");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}
		/* ����Ƶ������Դ */
		if (ret = IKapSetInfo(hVul, IKP_STROBE_TRIGGER_SOURCE, StrobeTriggerSource) == false)
		{
			printErrorMessage("INSSetInfo", "IKP_STROBE_TRIGGER_SOURCE");
			freeFrameGrabberHandle(hVul);
			return InsResult(ret | 0x1000);
		}

		/* �����ڲ�����Ƶ�� */
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
			cout << "��С��������ʧ��" << endl;
		}
		RETURNERRCBOARD(ret);
		cout << "��С�������óɹ�" << endl;
		//ͨ�������ź�1, 2 ����ģʽ_��ƽ����
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
			cout << "GENERAL_INPUT2_TRIGGER_MODE����ʧ��" << endl;
		}
		RETURNERRCBOARD(ret);
		cout << "GENERAL_INPUT2_TRIGGER_MODE���óɹ�" << endl;
		//����INTEGRATION_PARAM2
		ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM2, 5);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_PARAM2");
		}
		RETURNERRCBOARD(ret);
		//����INTEGRATION_PARAM4
		ret = IKapSetInfo(hVul, IKP_INTEGRATION_PARAM4, 1);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_INTEGRATION_PARAM4");
		}
		RETURNERRCBOARD(ret);
		//����GENERAL_INPUT2_MIN_WIDTH
		ret = IKapSetInfo(hVul, IKP_GENERAL_INPUT2_MIN_WIDTH, 1000);
		if (ret != 1)
		{
			printErrorMessage("INSSetInfo", "IKP_GENERAL_INPUT2_MIN_WIDTH");
		}
		RETURNERRCBOARD(ret);
		//CL �ɼ����Ƿ�У�� FVAL �ź�_��
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
	// ����ص�������
//
// Unregister callback functions.
	UnRegisterCallback();

	cout << "UnRegisterCallback" << endl;

	// �رղɼ����豸��
	//
	// Close frame grabber device.
	ret = IKapClose(ins_hBoard);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);


	// �رղɼ��豸��
	//
	// Close camera device.
	res = ItkDevClose(ins_hCamera);
	CHECK_IKAPC(res);
	RETURNERRC(res);


	// �ͷ� IKapC ���л�����
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
	/* �����¼����� */
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


//����Դ  IKP_BOARD_TRIGGER_SOURCE
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

//����ģʽ  IKP_GRAB_MODE
InsResult InsCameraIkap::set_ins_work_mode_activated(int index)
{
	ITKSTATUS res = ITKSTATUS_OK;
	int ret = IK_RTN_OK;
	if (index == 0)//����ģʽ
	{
		ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_INNER);
		CHECK_IKAPBOARD(ret);
	}
	else
	{
		// ���òɼ�������ģʽ��
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


	if (namecam.find("8KCXP") != string::npos)//���ܲ����ǿ��0201 to do
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

		// ��ȡDouble����ֵ
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

	else if (namecam.find("16KCXP") != string::npos)//���ܲ����ǿ��0201 to do
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

		// ��ȡDouble����ֵ
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
		// ���òɼ�������ģʽ��
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
	// ���òɼ�������ģʽ��
	// Set frame grabber trigger mode.
	//ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index + 8);//0322

	ITKDEV_INFO di;
	ItkManGetDeviceInfo(mi_camera, &di);// to do
	string namecam = di.FullName;
	int res = 0;
	if (namecam.find("8KCXP") != string::npos)//��������4K  //0325    ֻ��CXP+8�� CL+8�л����
	{
		//ret = IKapSetInfo(ins_hBoard, IKP_IMAGE_HEIGHT, index + 8);//0322  //���д���
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
	if (namecam.find("8K") != string::npos)//��������4K
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


//������֡��  IKP_FRAME_COUNT
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

	// ����CC1�ź�Դ��
	// Set CC1 signal source.
	ret = IKapSetInfo(ins_hBoard, IKP_CC1_SOURCE, IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ���û��ֿ��Ʒ��������ź�Դ��
	// Set integration control method trigger source.
	ret = IKapSetInfo(ins_hBoard, IKP_INTEGRATION_TRIGGER_SOURCE, IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);

	// ����ͬ��ģʽ��
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

	// ���òɼ�������ģʽ��
	// Set frame grabber trigger mode.
	ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_MODE, IKP_BOARD_TRIGGER_MODE_VAL_OUTTER);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ���òɼ�������Դ��
	// Set frame grabber trigger source.
	ret = IKapSetInfo(ins_hBoard, IKP_BOARD_TRIGGER_SOURCE, IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT1);
	CHECK_IKAPBOARD(ret);
	RETURNERRCBOARD(ret);
	// ����ͬ��ģʽ��
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
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//��tcharֵ����_char    
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
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return InsResult::INSCSTATUS_NO_FILE;
	}
	SHGetPathFromIDList(idl, szBuffer);
	char configFilename[128];
	TcharToChar(szBuffer, configFilename);

	//char configFilename[sizeof(szBuffer) + sizeof(".vlcf")]; // ����.szBuffer��".vlcf"�Ŀռ�
	//
	//// ����szBuffer��configFilename
	//for (int i = 0; i < sizeof(szBuffer); i++) {
	//	configFilename[i] = szBuffer[i];
	//}
	//// ���.vlcf��׺
	//strcat_s(configFilename, sizeof(configFilename), ".vlcf");
	

	// ��� .vlcf ��׺
	//strcat_s(configFilename, sizeof(configFilename), ".vlcf");
	///���.vlcf��׺
	//std::string path = "D:/1/1.vlcf";
	strcat_s(configFilename, ".vlcf");
	//// ���������ļ���
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


/* �ֶ�ѡ��������ļ�
InsResult InsCameraIkap::SaveCameraConfigurationToFile()
{
	ITKSTATUS res = ItkManInitialize();
	int ret = IK_RTN_OK;
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("������ѡ�ļ���Ŀ¼:");
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
	// ��ȡDouble����ֵ
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
	//	// ��ȡ��������ֵ
	//	res = ItkFeatureGetDouble(hFeature, &val);
	//	CHECK(res);
	//	// ������������ֵ
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
	// ��ȡ����������ֵ
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
	//	// ��ȡ��������ֵ
	//	res = ItkFeatureGetDouble(hFeature, &val);
	//	CHECK(res);
	//	// ������������ֵ
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
	// ��ȡ��������ֵ
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
	else if (namecam.find("8KCXP") != string::npos)//cxp���������Ҳ��8k
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
	// ����ö������������ֵ
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
	else if (namecam.find("CXP") != string::npos)//cxp���������Ҳ��8k
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
	// ��ȡö������������ֵ
	uint32_t enumValSize = 0;
	res = ItkFeatureToString(hFeature, NULL, &enumValSize);
	res = ItkFeatureToString(hFeature, ins_PAC_gain_, &enumValSize);
	res = ItkFeatureToString(hFeature, val, &enumValSize);
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//ֻҪʹ���˱����ͷ�
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
	int res = ItkDevAllocFeature(ins_hCamera, "AnalogGain", &hFeature);//���������������ֵ��ָ��
	CHECK(res);
	RETURNERRC(res);
	// ����ö������������ֵ
	res = ItkFeatureFromString(hFeature, val);//�������ָ��ȥ�����������ֵ
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//�ͷ�ָ��
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
	// ��ȡö������������ֵ
	uint32_t enumValSize = 0;
	res = ItkFeatureToString(hFeature, NULL, &enumValSize);
	res = ItkFeatureToString(hFeature, ins_PAC_gain_, &enumValSize);
	res = ItkFeatureToString(hFeature, val, &enumValSize);
	CHECK(res);
	RETURNERRC(res);
	res = ItkDevFreeFeature(hFeature);//��ӵģ������ͷ�
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

		if (namecam.find("16KCXP") != string::npos)//���ܲ����ǿ��0201 to do
		{
			int ret = 0;
			std::cout << "namecam : " << namecam << endl;
			ret = (int)set_ins_width(8000);

		}
	}
	else if ( a == 1)
	{

		if (namecam.find("16KCXP") != string::npos)//���ܲ����ǿ��0201 to do
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
	// ���
	size_t start = cameraModel.find("INS-LC-") + 7;
	size_t end = cameraModel.find('-', start);
	std::string cameraKeyPart = cameraModel.substr(start, end - start);

	// ��Դ
	std::string lightSourceKeyPart;
	size_t lightSourceStart = lightSourceModel.size() - 1;
	while (lightSourceStart >= 0 && !std::isdigit(lightSourceModel[lightSourceStart])) {
		--lightSourceStart;
	}
	lightSourceKeyPart = lightSourceModel.substr(lightSourceStart, 1);

	// ϵͳ
	std::string systemModel = "INS-PMDL-25D-" + cameraKeyPart + lightSourceKeyPart + "M";

	return systemModel;
}





//----------��д�����Ĵ���------------
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
