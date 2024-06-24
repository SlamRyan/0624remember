#pragma once
#include <functional>
#include "ins_global.h"


#ifdef INSCAMERA_EXPORTS
#define INSCAMERA_API __declspec(dllexport)

#else
#define INSCAMERA_API __declspec(dllimport)
//#define INSCAMERA_API 
#endif



//GlobalVar
//using DataPocessCallBack = std::function<void(unsigned char*)>;
//typedef void (CALLBACK* DataPocessCallBack)(unsigned char*);

typedef void (__stdcall* DataPocessCallBack)(InsPMDL_Buffer*);
typedef void (__stdcall* OffLineCallBack)(unsigned char*);




namespace Ins2D5Camera
{
/********************************************************************//**
 *  @~chinese
 *  @brief  获取SDK版本号
 *  @param  
 *  @return 返回版本号，比如返回值为0x01000001，即SDK版本号为V1.0.0.1；返回值为0x01000101，即SDK版本号为V1.0.1.1
 *  @remarks获取SDK版本号，引入头文件、Lib文件、Dll文件后，SDK环境搭建完毕，可直接调用
 ************************************************************************/
	extern "C" INSCAMERA_API uint32_t __cdecl InsPMDL_GetSdkVersion();

/********************************************************************//**
 *  @~chinese
 *  @brief  寻找相机设备数量
 *  @param  _num						 [IN]            设备数量
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindCameraDevice(int* _num);


/********************************************************************//**
 *  @~chinese
 *  @brief  创建设备句柄
 *  @param  _ins_handle                 [OUT]           设备句柄
 *  @param  _index						[OUT]           设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 
 ************************************************************************/
    
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetCameraDeviceInfo(int _num, InsCameraInfo * _insCameraInfo);
	extern "C" INSCAMERA_API void __cdecl InsPMDL_CreateCameraHandle(void** _ins_handle, int _index);

/********************************************************************//***
 *  @~chinese
 *  @brief  获取光源设备相关信息 
 *  @param  _ins_light                 [IN]              光源设备句柄
 *  @param  _insdeviceinfo             [OUT]           设备信息结构体
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindOtherInfo(void* _ins_light, OtherDeviceInfo * _insdeviceinfo);


/********************************************************************//**
 *  @~chinese
 *  @brief  寻找光源设备数量
 *  @param  _num						 [IN]            设备数量
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 
 ************************************************************************/
extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindLightSourceDevice(int* _num);

//获取光源设备信息
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetLightSouceDeviceInfo(int num, Ins2D5LightDeviceAndLocalIP* _deviceInfo);
//新的
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_ResetLightSourceIP(const char* _ip, Ins2D5LightDeviceAndLocalIP * _deviceInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取光源设备的句柄
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _spinsdeviceinfo             [OUT]           设备信息结构体
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
//获取光源句柄
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetLightSourceHandle(void** _ins_light, Ins2D5LightDeviceAndLocalIP _deviceInfo);

//拼接系统信息 
extern "C" INSCAMERA_API std::string __cdecl   InsPMDL_GenerateSystemModel(std::string cameraModel, std::string lightSourceModel);

//保存相机的SN号到光源
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_SaveSNToLight(void* _ins_light, char _SN[64]);

//读取光源中配对的相机的SN号
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetSNFromLight(void* _ins_light, const char* _SN);

    //extern "C" INSCAMERA_API InsResult __cdecl ins_find_camera(INSDeviceInfo * insdeviceinfo);
	//extern "C" INSCAMERA_API InsResult __cdecl ins_get_camera_info(INSDeviceInfo * insdeviceinfo);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_grab_once(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  CameraLink相机连续取图
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _index						 [IN]            取图参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 需要对应使用停止采集接口：InsPMDL_GrabStop，_index 0-自由取图，1-编码器，2-软触发，3-IO触发
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GrabContinous(void* _ins_handle);//_index 0-自由取图，1-编码器，2-软触发，3-IO触发

/********************************************************************//**
 *  @~chinese
 *  @brief  相机停止采集
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _index						 [IN]            取图参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks _index 0-自由取图，1-编码器，2-软触发,3-IO触发
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GrabStop(void* _ins_handle); 

/********************************************************************//**
 *  @~chinese
 *  @brief  调焦用自由取图
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _boardnum                    [IN]            光源长度除以10
 *  @param  _picture_num                 [IN]            抽帧数
 *  @param  _index						 [IN]            取图参数，0-关，1-开
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 需要将可执行文件目录下的自由取图的相机文件（freerun.ccf）配置进相机,完成自由取图基础参数配置，如果路径错误或者文件名错误会返回错误码。
 *           开始时会在SDK内部保存行高、曝光时间、行扫描周期、行频、软触发次数参数，并在关闭时加载。
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FreerunForFocus(void* _ins_handle, InsPMDL_Switch _index);//上位机里的自由取像






    //extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureBoard(void* _ins_handle);//对应的是配250k那个

/********************************************************************//**
 *  @~chinese
 *  @brief  250K系统的参数配置
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureBoardWith250K(void* _ins_handle);

	//调试用接口
    extern "C" INSCAMERA_API InsResult __cdecl ins_SaveBoardConfigurationToFile(void* _ins_handle);
	extern "C" INSCAMERA_API InsResult __cdecl SaveCameraConfigurationToFile(void* _ins_handle,char* _filename);
	extern "C" INSCAMERA_API InsResult __cdecl LoadLocalFile(void* _ins_handle);//临时
    extern "C" INSCAMERA_API InsResult __cdecl ins_configure_file_to_camera(void* _ins_handle, char* _filename);//弃用
    extern "C" INSCAMERA_API InsResult __cdecl ins_save_camera_configuration_to_file(void* _ins_handle, char* _filename);//弃用

/********************************************************************//**
 *  @~chinese
 *  @brief  加载配置文件（*.icf）
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _filename                    [IN]            完整的文件路径，包含到.icf后缀
 *  @param  _boardnum                    [IN]            光源长度除以10
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_LoadConfigureFileToSystem(void* _ins_handle, const char* _filename);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  保存配置文件（*.icf）
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _filename                    [IN]            完整的文件路径，包含到.icf后缀
 *  @param  _boardnum                    [IN]            光源长度除以10
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SaveSystemConfigFile(void* _ins_handle, const char* _filename);

/********************************************************************//**
 *  @~chinese
 *  @brief  关闭设备，释放资源
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_Close(void** _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  参数配置和申请图像缓冲区
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 在设置图像行高和宽之后务必要重新调用该接口
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureFrameGrabber(void* _ins_handle);//目前在用

/********************************************************************//**
 *  @~chinese
 *  @brief  注册取图回调，当一帧图像采集完后会进入该回调函数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _callback                    [IN]            回调函数类型
 *  @return 
 *  @remarks 通过该接口获得图像,需要调用InsPMDL_UnregisterDataCallback接口来注销回调
 ************************************************************************/
    extern "C" INSCAMERA_API void InsPMDL_SetDataCallback(void* _ins_handle, DataPocessCallBack _callback);

/********************************************************************//**
 *  @~chinese
 *  @brief  注销取图回调
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 
 *  @remarks 注销取图回调函数
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_UnregisterDataCallback(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  注册系统掉线回调，当相机掉线后会进入该回调函数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _callback                    [IN]            回调函数类型
 *  @return 
 *  @remarks 通过该接口获得掉线信息，需要调用InsPMDL_SetDeviceRemoveCallback接口注销回调
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_SetDeviceRemoveCallback(void* _ins_handle, DataPocessCallBack _callback);

/********************************************************************//**
 *  @~chinese
 *  @brief  注销系统掉线回调
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_UnregisterDeviceRemoveCallback(void* _ins_handle);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  获得当前帧像素数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回像素数；错误，返回INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetFrameSize(void* _ins_handle);



	//用户用不到
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_trigger_source_activated(void* _ins_handle, int index);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_work_mode_activated(void* _ins_handle, int index);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置图像宽度像素数
 *  @param  _ins_handle                  [IN]            相机设备句柄 
 *  @param  _ins_width                   [IN]            图像宽度
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetWidth(void* _ins_handle, int _ins_width);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置图像高度像素数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ins_height                  [IN]            图像高度
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetHeight(void* _ins_handle, int _ins_height);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取图像宽度像素数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回图像宽度；错误，返回INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetWidth(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取图像高度像素数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回图像高度；错误，返回INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetHeight(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置缓冲区帧数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ins_frame_count             [IN]            帧数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 用户应该用不到，不写进用户手册了
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetFrameCount(void* _ins_handle, int _ins_frame_count);

    //用户用不到
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_image_type(void* _ins_handle, int index);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_line_trigger(void* _ins_handle);//设置行触发模式
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_frame_trigger(void* _ins_handle);

//这两个接口已经加进了调焦自由取图中，用户应该用不到，实际中也基本没用到
/********************************************************************//**
 *  @~chinese
 *  @brief  配置外部触发的（.ccf）文件
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 将相对路径和文件名为"./externalpulse.ccf"的文件配置进系统，自由取图结束后需要调用此接口
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureExternalPulseFileToCamera(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  配置自由取图的（.ccf）文件
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 将相对路径和文件名为"./freerun.ccf"的文件配置进系统，在调焦时需要自由取图，自由取图前需要调用此接口
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureFreeRunFileToCamera(void* _ins_handle);
    

/********************************************************************//**
 *  @~chinese
 *  @brief  设置曝光时间
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _val                         [IN]            曝光时间
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 曝光时间不能大于行扫描速率 max=10000000.000000000 min=2.0000000000000000	间隔为inc=0.1
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetExposure(void* _ins_handle, double _val);


/********************************************************************//**
 *  @~chinese
 *  @brief  获取曝光时间
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 返回值为曝光时间，获取失败时返回DBL_MAX
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API double __cdecl InsPMDL_GetExposure(void* _ins_handle);

	//用户用不到
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_line_period_time(void* _ins_handle, double _val);//行频，max=10000000.000000000 min=5.0000000000000000	inc=0.1 //返回值为18表示参数超出范围
    //extern "C" INSCAMERA_API double __cdecl ins_get_line_period_time(void* _ins_handle);


/********************************************************************//**
 *  @~chinese
 *  @brief  设置数字增益
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _val                         [IN]            数字增益
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks max=8 min=0.01	inc=0.01
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetDigitalGain(void* _ins_handle, double _val);//

/********************************************************************//**
 *  @~chinese
 *  @brief  获取数字增益
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 返回值为数字增益，获取失败时返回DBL_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API double __cdecl InsPMDL_GetDigitalGain(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置模拟增益
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ch                          [IN]            模拟增益
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks ins_PAC_gain值可为：
			 #define INS_PAC_GAIN_X1 "x1";
			 #define INS_PAC_GAIN_X2 "x2";
			 #define INS_PAC_GAIN_X5 "x5";
			 #define INS_PAC_GAIN_X10 "x10";
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetPacGain(void* _ins_handle, char* _ch);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取模拟增益
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ch                          [OUT]           模拟增益
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks ins_PAC_gain值可为：
			 #define INS_PAC_GAIN_X1 "x1";
			 #define INS_PAC_GAIN_X2 "x2";
			 #define INS_PAC_GAIN_X5 "x5";
			 #define INS_PAC_GAIN_X10 "x10";
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetPacGain(void* _ins_handle, char* _ch);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  设置触发最小值
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ins_shaft_encoder1_min_width[IN]            触发最小值
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks 编码器输入信号滤波功能
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetShaftEncoder1MinWidth(void* _ins_handle, int _ins_shaft_encoder1_min_width);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取触发最小值
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 成功，返回触发最小值；错误，返回INT_MAX
 *  @remarks 编码器输入信号滤波功能
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetShaftEncoder1MinWidth(void* _ins_handle);


/********************************************************************//**
 *  @~chinese
 *  @brief  获取当前帧数
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return 返回帧数
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API int __cdecl InsPMDL_GetFrameId(void* _ins_handle);

	//用户用不到
	//extern "C" INSCAMERA_API InsResult __cdecl ins_user_set_save(void* _ins_handle);//断电保存相机相关配置


/********************************************************************//**
 *  @~chinese
 *  @brief  初始化本机设备地址
 *  @param  _ip                          [IN]            当前客户端IP地址
 *  @param  _NET_mask                    [IN]            当前客户端子网掩码
 *  @param  _gateway                     [IN]            当前客户端网关
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API void __cdecl InsPMDL_InitLocalInfo(const char* _ip, const char* _NET_mask, const char* _gateway);

/********************************************************************//**
 *  @~chinese
 *  @brief  找设备，获取当前设备IP等信息
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ins_device                  [OUT]           当前设备IP地址、子网掩码、网关等信息
 *  @return 成功返回0
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SearchDevice(void* _ins_handle, Ins2D5LightDevice * _ins_device);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置设备IP
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _ip                          [OUT]           用户自定义设备IP地址
 *  @return return -1 means that the ip is wrong,return 0 means success
 *  @remarks 必须在找设备该接口后使用
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ResetDeviceIp(void* _ins_handle, const char* _ip);

    //-1 failure，返回的Device内为当前设备信息
    //extern "C" INSCAMERA_API int factory_configuration(void* _ins_handle, Device * ins_device);//11.18新增出厂配置，需要先把配置设置进device 类型,如果设置失败，返回的device 类型里字符串为空
	
/********************************************************************//**
 *  @~chinese
 *  @brief  通讯连接初始化
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @return return -1 means that the ip is wrong,return 0 means success
 *  @remarks 可以不使用该接口，当出现连接错误，可重新进行初始化
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_InitCommunication(void* _ins_light);
	
	
/********************************************************************//**
 *  @~chinese
 *  @brief  断电保存
 *  @param  _ins_handle                  [IN]            相机设备句柄
 *  @param  _board_num                   [IN]            光源长度除以10
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_PowerOffSave(void* _ins_handle);

//-----------------------------------------Add New ------------------------

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FpgaRegInitial(void* _ins_handle);


	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FPGAInitial(void* _ins_light);

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SelectDirection(void* _ins_light, int index);//四种位姿 0 、1 、2、3

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetTimeOut(void* _ins_light, int time);

	extern "C" INSCAMERA_API int __cdecl ins_save_camera_height(void* ins_light, uint32_t * buffer);

	extern "C" INSCAMERA_API int __cdecl ins_read_camera_height(void* ins_light, uint32_t * buffer);//32

	extern "C" INSCAMERA_API int __cdecl InsPMDL_ResetSin(void* ins_light);

	extern "C" INSCAMERA_API int __cdecl InsPMDL_InitialLightBrightness(void* ins_light);//初始化光源接口 AFF->7FF

	extern "C" INSCAMERA_API int __cdecl ins_read_camera_heightTemp(void* ins_light, uint32_t * buffer); //0514

	extern "C" INSCAMERA_API int __cdecl ins_save_camera_heightTemp(void* ins_light, uint32_t * buffer);

	extern "C" INSCAMERA_API int __cdecl InsPMDL_SaveCameraHeight(void* ins_light, uint32_t * buffer);

	extern "C" INSCAMERA_API int __cdecl InsPMDL_GetCameraHeight(void* ins_light, uint32_t * buffer);





	//0429 
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetSystemHandle(void** ins_handle, InsPMDL_SystemInfo SystemInfo);

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetImage(void* ins_handle, InsPMDL_Buffer InputBuffer, InsPMDL_FusionBuffer* Output, INS_PMDL_ImageType OutPutIndex);

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetTriggerMode(void* ins_handle ,INS_PMDL_TriggerModeSelect TriMode);


	extern "C" INSCAMERA_API void __cdecl InsPMDL_LoadSystemInfoFromFile(const char* filename, InsPMDL_SystemInfo & systemInfo);

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_Initial25DSystem(void** _ins_handle, const char* filename);
	









//-----------------------------------------

	//————————————————触发设置（功能）————————————————————
/********************************************************************//**
 *  @~chinese
 *  @brief  设置系统扫描周期
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _board_num                   [IN]            光源长度除以10
 *  @param  _line_scanning_period        [IN]            扫描周期(单位：微秒)
 *  @param  _line_fre                    [OUT]           行频(单位：KHz)
 *  @param  _exposure                    [OUT]           曝光时间(单位：微秒)
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks设置系统当前扫描周期_line_scanning_period(单位：微秒),和获取对应的行频_line_fre(单位：KHz),行扫描周期应大于曝光时间+2,输入范围（4~100）
 *			自动计算相机曝光给出，并设置相机曝光
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetScanningPeriod(void* _ins_handle,  double _line_scanning_period, double& _line_fre, double& _exposure);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取系统扫描周期
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _board_num                   [IN]            光源长度除以10
 *  @param  _line_scanning_period        [OUT]           扫描周期(单位：微秒)
 *  @param  _line_fre                    [OUT]           行频(单位：KHz)
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks获得当前系统扫描周期_line_scanning_period(单位：微秒)和行频_line_fre(单位：KHz)
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetScanningPeriod(void* _ins_light, double& _line_scanning_period, double& _line_fre);


	//——————————————编码器触发设置————————————————————
/********************************************************************//**
 *  @~chinese
 *  @brief  编码器校验，A-B相取反
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [IN]            索引参数，1-开，0-关
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks编码器校验，A-B相取反1开、0关
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetABPhaseInversionOnOff(void* _ins_light, InsPMDL_Switch _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置触发行程方向
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [IN]            索引参数，0-单向行程，1-回退跟踪(反向不计)，2-回退忽略，3-双向扫描
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks设置触发行程方向,0-单向行程，1-回退跟踪(反向不计)，2-回退忽略，3-双向扫描
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetTriggerDirection(void* _ins_light, int _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置编码器触发模式
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [IN]            索引参数，0-A相上升沿，单向计数，1-A相上升沿，双向计数，2-A相双边沿，双向计数，3-A&B相双边沿，双向计数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置分频
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _frequency_division			 [IN]            分频参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetFrequencyDivision(void* _ins_light, uint32_t _frequency_division);

/********************************************************************//**
 *  @~chinese
 *  @brief  设置倍频
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _frequency_multiplier		 [IN]            倍频参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks_frequency_multiplier值必须>1
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetFrequencyMultiplier(void* _ins_light, uint32_t _frequency_multiplier);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取触发行程方向
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [OUT]           索引参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks获取触发方向,0-单向行程，1-回退跟踪，2-回退忽略，3-双向扫描
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetTriggerDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取编码器触发模式
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [OUT]           索引参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks获取编码器触发模式，0-A相上升沿，单向计数，1-A相上升沿，双向计数，2-A相双边沿，双向计数，3-A&B相双边沿，双向
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEncoderTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取分频
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [OUT]           分频参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetFrequencyDivision(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  获取倍频
 *  @param  _ins_light                   [IN]            光源设备句柄
 *  @param  _index						 [OUT]           分频参数
 *  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetFrequencyMultiplier(void* _ins_light, uint32_t& _index);


	//————————————————————IO触发设置————————————————————
/********************************************************************//**
*  @~chinese
*  @brief  设置io触发模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index				  	    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置io触发模式,只有三个板子的版本,to do
*           0-模式1：IO“0”开始，IO“1”结束,
*	        1-模式2：IO“1”开始，IO“0”结束,
*	        2-模式3：IO“0”开始，输出固定触发脉冲,
*	        3-模式4：IO“1”开始，输出固定触发脉冲,
*	        4-模式5：IO“0”有效，上升沿有效，下降沿结束,
*	        5-模式6：IO“1”有效，上升沿有效，下降沿结束,
*	        6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
*	        7-模式8：多个IO"1"作为开始，输出固定触发脉冲
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置io触发数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _trinum					    [IN]            触发数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置io触发数量,只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerNum(void* _ins_light,  uint32_t _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  设置io脉冲数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _count					    [IN]            脉冲数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置io触发数量,只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOPulseCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  设置io触发脉冲宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _width					    [IN]            脉冲宽度
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置io触发脉冲宽度,只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerPulseWidth(void* _ins_light,  uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  设置io触发延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _delay					    [IN]            触发延迟
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置io触发延迟,只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerDelay(void* _ins_light, uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  设置io触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _period					    [IN]            触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks置io触发周期,只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerPeriod(void* _ins_light,  uint32_t _period);

/********************************************************************//**
*  @~chinese
*  @brief  设置io触发输入极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks置io触发输入极性,0-保持，1-反转，只有三个板子的版本,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerInputPolarity(void* _ins_light,uint32_t _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index				  	    [OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks获取io触发模式,只有三个板子的版本,to do
*           0-模式1：IO“0”开始，IO“1”结束,
*	        1-模式2：IO“1”开始，IO“0”结束,
*	        2-模式3：IO“0”开始，输出固定触发脉冲,
*	        3-模式4：IO“1”开始，输出固定触发脉冲,
*	        4-模式5：IO“0”有效，上升沿有效，下降沿结束,
*	        5-模式6：IO“1”有效，上升沿有效，下降沿结束,
*	        6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
*	        7-模式8：多个IO"1"作为开始，输出固定触发脉冲
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _trinum					    [OUT]           触发数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerNum(void* _ins_light, uint32_t& _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  获取io脉冲数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _count					    [OUT]            脉冲数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOPulseCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发脉冲宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _width					    [OUT]            脉冲宽度
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerPulseWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _delay					    [OUT]           触发延迟
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _period					    [OUT]           触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerPeriod(void* _ins_light, uint32_t& _period);

/********************************************************************//**
*  @~chinese
*  @brief  获取io触发输入极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index					    [OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks获取io触发输入极性，0-保持，1-反转
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerInputPolarity(void* _ins_light, uint32_t& _index);


	//————————————————光源条纹设置————————————————————
/********************************************************************//**
*  @~chinese
*  @brief  设置光源模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	设置光源模式，0-极大模式，1-大模式，2-标准模式，3-小模式，4-极小模式
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEmissionMode(void* _ins_handle, InsPMDL_LineMode LineMode);

/********************************************************************//**
*  @~chinese
*  @brief  获取光源发光模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index					    [OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	获取光源发光模式，0-极大模式，1-大模式，2-标准模式，3-小模式，4-极小模式
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEmissionMode(void* _ins_light, int& _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置X条纹移动方向
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	设置X条纹移动方向，0-正向，1-反向
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetXStripeMovementDirection(void* _ins_light,  int _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置Y条纹移动方向
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	设置Y条纹移动方向，0-正向，1-反向
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetYStripeMovementDirection(void* _ins_light,  int _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置X方向起始相位
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	设置X方向起始相位，0-0°，1-90°，2-180°，3-270°
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetInitialPhaseInXDirection(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置Y方向起始相位
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index					    [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	设置Y方向起始相位，0-0°，1-90°，2-180°，3-270°
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetInitialPhaseInYDirection(void* _ins_light,int _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取X条纹移动方向
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index					    [OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	获取X条纹移动方向，0-正向，1-反向
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetXStripeMovementDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取Y条纹移动方向
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index					    [OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	获取Y条纹移动方向，0-正向，1-反向
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetYStripeMovementDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取X方向起始相位
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _initial_phase				[OUT]           起始相位 
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	_initial_phase值可为0°，90°，180°，270°
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetInitialPhaseInXDirection(void* _ins_light, uint32_t& _initial_phase);

/********************************************************************//**
*  @~chinese
*  @brief  获取Y方向起始相位
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _initial_phase				[OUT]           起始相位
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	_initial_phase值可为0°，90°，180°，270°
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetInitialPhaseInYDirection(void* _ins_light, uint32_t& _initial_phase);

	//——————————————————光源条纹设置——————————————————

	//————————————————二级触发光源触发模式———————————————
/********************************************************************//**
*  @~chinese
*  @brief  二级触发使能和停止使能
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index			        	[IN]            索引系数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	_index:0-disable,1-enable
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SecondaryTriggerEnableAndDisable(void* _ins_light,double _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置编码器二级触发脉宽
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _encodewidth			        [IN]            编码器二级触发脉宽(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks	
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderSecondaryTriggerPulseWidth(void* _ins_light,  double _encodewidth);

/********************************************************************//**
*  @~chinese
*  @brief  设置编码器二级触发方向
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _boardnum                    [IN]            光源长度除以10
*  @param  _index			            [IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks触发方向,0-单向行程，1-回退跟踪(反向不计)，2-回退忽略，3-双向扫描,只写了第一块板子的寄存器
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderSecondaryTriggerTravelDirection(void* _ins_light,  int _index);
	
/********************************************************************//**
*  @~chinese
*  @brief  设置编码器二级触发分频
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _frequency_division			[IN]            分频参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks //to do  没找到编码器触发模式
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSecondaryFrequencyDivision(void* _ins_light, uint32_t _frequency_division);

/********************************************************************//**
*  @~chinese
*  @brief  设置编码器二级触发倍频
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _frequency_multiplier		[IN]            倍频参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks //设置二级触发倍频
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSecondaryFrequencyMultiplier(void* _ins_light, uint32_t _frequency_multiplier);

	//——————————————————软触发————————————————————
/********************************************************************//**
*  @~chinese
*  @brief  设置软触发触发次数
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _count						[IN]            触发次数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks 
* ***********************************************************************/
	//设置软触发触发次数
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftwareTriggerTriggerCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  设置触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _period						[IN]            触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	//设置触发周期
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftwareTriggerTriggerPeriod(void* _ins_light, double _period);

/********************************************************************//**
*  @~chinese
*  @brief  获得软触发触发次数
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _count						[OUT]           触发次数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	//获得软触发触发次数
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftwareTriggerTriggerCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  获得软触发触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _period						[OUT]           触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	//获得触发周期
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftwareTriggerTriggerPeriod(void* _ins_light, double& _period);

	//——————————————————IO触发——————————————————————
/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io触发模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index						[IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks_index取值：
		   0-模式1：IO“0”开始，IO“1”结束,
	       1-模式2：IO“1”开始，IO“0”结束,
	       2-模式3：IO“0”开始，输出固定触发脉冲,
	       3-模式4：IO“1”开始，输出固定触发脉冲,
	       4-模式5：IO“0”有效，上升沿有效，下降沿结束,
	       5-模式6：IO“1”有效，上升沿有效，下降沿结束,
	       6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
	       7-模式8：多个IO"1"作为开始，输出固定触发脉冲
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io触发数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _trinum						[IN]            触发数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerNum(void* _ins_light,  uint32_t _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io脉冲数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _count						[IN]            触发数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryPulseCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io脉冲宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _width						[IN]            触发宽度
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerPulseWidth(void* _ins_light, uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io触发延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _delay						[IN]            触发延迟
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerDelay(void* _ins_light,  uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _period						[IN]            触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerPeriod(void* _ins_light, uint32_t _period);

/********************************************************************//**
*  @~chinese
*  @brief  设置二级触发io输入极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index						[IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置二级触发io触发输入极性,0-保持，1-反转
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerInputPolarity(void* _ins_light,  uint32_t _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io触发模式
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index						[OUT]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks 获取二级触发io触发模式,_index：
			0-模式1：IO“0”开始，IO“1”结束,
			1-模式2：IO“1”开始，IO“0”结束,
			2-模式3：IO“0”开始，输出固定触发脉冲,
			3-模式4：IO“1”开始，输出固定触发脉冲,
			4-模式5：IO“0”有效，上升沿有效，下降沿结束,
			5-模式6：IO“1”有效，上升沿有效，下降沿结束,
			6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
			7-模式8：多个IO"1"作为开始，输出固定触发脉冲
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io触发数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _count						[OUT]           触发数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerNum(void* _ins_light, uint32_t& _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io脉冲数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _count						[OUT]           脉冲数量
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryPulseCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io脉冲宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _width						[OUT]           脉冲宽度
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerPulseWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io触发延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _delay						[OUT]           触发延迟
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index						[OUT]			索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerPeriod(void* _ins_light, uint32_t& _period);

/********************************************************************//**
*  @~chinese
*  @brief  获取二级触发io触发输入极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _period						[OUT]           触发周期
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks获取二级触发io触发输入极性，0-保持，1-反转
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerInputPolarity(void* _ins_light, uint32_t& _index);
	//———————————二级触发光源触发模式————————————





	//——————————————红外模式————————————
	
/********************************************************************//**
*  @~chinese
*  @brief  红外使能和停止使能
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index						[IN]			使能索引参数
*  @param  _quantity					[IN]            扩展触发输出数量,一般为1（单位：次）
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks 红外使能和停止使能,_index:0-disable,1-enable，停止使能时，参数_quantity可以为任意数
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_ExtendedTriggerEnableAndDisable(void* _ins_light,  double _index, int _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  设置扩展触发输出数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _quantity					[IN]            扩展触发输出数量,一般为1（单位：次）
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks 
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputQuantity(void* _ins_light, uint32_t _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  设置扩展触发输出延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _delay						[IN]            扩展触发输出延迟(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputDelay(void* _ins_light, uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  设置扩展触发输出宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _width						[IN]            输出宽度(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputWidth(void* _ins_light,  uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  设置扩展触发输出极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _index						[IN]            索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置扩展触发输出极性,0-上升沿，1-下降沿
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputPolarity(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  获取扩展触发输出数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _quantity					[OUT]           扩展触发输出数量,一般为1（单位：次）
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputQuantity(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  获取扩展触发输出延迟
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _delay						[OUT]           扩展触发输出延迟(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  获取扩展触发输出宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _width						[OUT]           扩展触发输出宽度(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  获取扩展触发输出极性
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _index						[OUT]           索引参数
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks获取扩展触发输出极性,0-上升沿，1-下降沿
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputPolarity(void* _ins_light, uint32_t& _index);
	//——————————————红外模式————————————

#
	//——————————————高级设置——————————————
/********************************************************************//**
*  @~chinese
*  @brief  设置光源响应时间
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _delay						[IN]            光源响应时间(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置光源响应时间(单位：微秒)值必须＞03
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceResponseTime(void* _ins_light,  double _delay);

/********************************************************************//**
*  @~chinese
*  @brief  设置光源触发宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _width						[IN]            光源触发宽度(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置光源触发宽度(单位：微秒)值必须<=系统行扫描周期的一半
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceTriggerOutputWidth(void* _ins_light,double _width);

/********************************************************************//**
*  @~chinese
*  @brief  设置光源亮度调节
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _percentage					[IN]            光源亮度调节(单位：%)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks设置光源亮度调节(单位：%),亮度输入应为整数 , 范围：1~100
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceBrightnessAdjustment(void* _ins_light, uint32_t _percentage);

/********************************************************************//**
*  @~chinese
*  @brief  设置编码器触发脉宽
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _encode_width				[IN]            编码器触发脉宽(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderTriggerPulseWidth(void* _ins_light, double _encode_width);

/********************************************************************//**
*  @~chinese
*  @brief  设置光源触发输出数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _quantity					[IN]            光源触发输出数量(单位：次)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceTriggerOutputQuantity(void* _ins_light,  uint32_t _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  获取光源响应时间
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _delay						[OUT]           光源响应时间(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceResponseTime(void* _ins_light, double& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  获取光源触发宽度
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _width						[OUT]           光源触发宽度(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks //后续可能改，to do 
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceTriggerOutputWidth(void* _ins_light, double& _width);

/********************************************************************//**
*  @~chinese
*  @brief  获取光源亮度调节
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _percentage					[OUT]           光源亮度调节(单位：%)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks光源亮度调节,取值范围：1~100
* ***********************************************************************/
	//获取光源亮度调节(单位：%), 范围：1~100
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceBrightnessAdjustment(void* _ins_light, uint32_t& _percentage);

/********************************************************************//**
*  @~chinese
*  @brief  获取编码器触发脉宽
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _board_num                   [IN]            光源长度除以10
*  @param  _encode_width				[OUT]           编码器触发脉宽(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEncoderTriggerPulseWidth(void* _ins_light,double& _encode_width);

/********************************************************************//**
*  @~chinese
*  @brief  获取光源触发输出数量
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _quantity					[OUT]           光源触发输出数量(单位：次)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceTriggerOutputQuantity(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  设置光源软触发次数
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _quantity					[IN]            光源软触发次数(单位：次)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks软触发次数(单位：次)值必须＞0
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftTriggerNum(void* _ins_light, uint32_t _num);

/********************************************************************//**
*  @~chinese
*  @brief  设置软触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _quantity					[IN]            软触发周期(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftTriggerPeriod(void* _ins_light, double _period);

/********************************************************************//**
*  @~chinese
*  @brief  获取软触发次数
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _quantity					[OUT]           软触发次数(单位：次)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftTriggerNum(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  获取软触发周期
*  @param  _ins_light                   [IN]            光源设备句柄
*  @param  _period						[OUT]           软触发周期(单位：微秒)
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftTriggerPeriod(void* _ins_light, double& _period);


	//-------------------------------------------------------------



	//——————————————高级设置——————————————

	//激光器开关，置1开，置0关
	extern "C" INSCAMERA_API InsResult InsPMDL_OpenOrCloseLasers(void* _ins_handle, uint16_t buffer);

	//读相机位姿，pitch为前向俯仰
	extern "C" INSCAMERA_API int InsPMDL_GetCameraPitch(void* ins_light,double* buffer);
	//读相机位姿，roll为左右俯仰
	extern "C" INSCAMERA_API int InsPMDL_GetCameraRoll(void* ins_light, double* buffer);
	//读相机位姿，yaw为水平旋转角
	extern "C" INSCAMERA_API int InsPMDL_GetCameraYaw(void* ins_light, double* buffer);
	//读光源位姿，pirch为前向俯仰
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourcePitch(void* ins_light, double* buffer);
	//读光源位姿，roll为左右俯仰
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourceRoll(void* ins_light, double* buffer);
	//读光源位姿，yaw为水平旋转角
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourceYaw(void* ins_light, double* buffer);
	//保存相机行高
	//extern "C" INSCAMERA_API int save_camera_height(uint32_t * buffer);
	//extern "C" INSCAMERA_API int read_camera_height(uint32_t * buffer);
	//获取FPGA温度
	extern "C" INSCAMERA_API int InsPMDL_GetFpgaTemp(void* _ins_handle, double* buffer);
	//获取主板温度
	extern "C" INSCAMERA_API int InsPMDL_GetBoardTemp(void* _ins_handle, double* buffer);

    //已知设备IP的情况下，不通过找设备来获取设备IP，来进行通讯初始化
	//extern "C" INSCAMERA_API InsResult InsPMDL_InitCommunication(void* _ins_handle, const char* _ip);

/********************************************************************//**
*  @~chinese
*  @brief  高级权限读寄存器
*  @param  _ins_handle                  [IN]            相机设备句柄
*  @param  _input						[IN]            特定字符串
*  @param  _id							[IN]            片选灯板号
*  @param  _address						[IN]            寄存器地址
*  @param  _buffer						[OUT]           读出的数据
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarks		_id:片选灯板号,可设置值为如下：
				EXTSTM32 = 101,
				INTSTM32 = 100,
				FIRSTBOARD = 1,
				SECONDBOARD = 2,
				THIRDBOARD = 3,
				FOURTHBOARD = 4,
				FIFTHBOARD = 5,
				寄存器地址不需要偏移，内部已经/2
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_ReadRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t & _buffer);

/********************************************************************//**
*  @~chinese
*  @brief  高级权限写寄存器
*  @param  _ins_handle                  [IN]            相机设备句柄
*  @param  _input						[IN]            特定字符串
*  @param  _id							[IN]            片选灯板号
*  @param  _address						[IN]            寄存器地址
*  @param  _buffer						[IN]            写入的数据
*  @return 成功，返回InsResult::INSCSTATUS_OK或者InsResult::INSSTATUS_SUCCESS；错误，返回错误码
*  @remarksid	_id:片选灯板号,可设置值为如下：
				EXTSTM32 = 101,
				INTSTM32 = 100,
				FIRSTBOARD = 1,
				SECONDBOARD = 2,
				THIRDBOARD = 3,
				FOURTHBOARD = 4,
				FIFTHBOARD = 5,
				寄存器地址不需要偏移，内部已经/2
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_WriteRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t _buffer);


	//================0304==========面阵对外接口===============================
	extern "C" INSCAMERA_API InsResult write_fpga_reg(void* _ins_light, const char* _input, int32_t index, uint32_t _addr, uint32_t _value);


	//初始化
	extern "C" INSCAMERA_API InsResult InsPMDLight_Initial(void* _ins_light, int BoardNum);

	// 设置面阵光源IO触发周期
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerCycle(void* _ins_light, uint32_t _index, double _time_ms);

	// 设置面阵光源IO触发数量
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerNum(void* _ins_light, uint32_t _index, int _num);

	// 设置面阵光源IO触发使能
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerEn(void* _ins_light, uint32_t _index, bool _enable);

	//光源点亮时间(单位：ms)
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIlluminationTime(void* _ins_light, uint32_t _index, double _time);

	//面阵发光模式
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetLightMode(void* _ins_light, uint32_t _index, int index);

	//自由取像开：
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFreeRunOn(void* _ins_light, uint32_t _index);

	//自由取像关：
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFreeRunOff(void* _ins_light, uint32_t _index);

	//触发（8张）开：
	extern "C" INSCAMERA_API InsResult InsPMDLight_Trigger_8_ImagesOn(void* _ins_light, uint32_t index, uint32_t _time, uint32_t _delaytime);

	//触发（8张）关：
	extern "C" INSCAMERA_API InsResult InsPMDLight_Trigger_8_ImagesOff(void* _ins_light, int32_t index);

	//点亮时间10ms下对应参数计算及配置
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIlluminationTime10ms(void* _ins_light, uint32_t index, double _time);

	//设置面阵模式使能
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetArrayEn(void* _ins_light,uint32_t _index, bool _enable);

	//设置面阵属性——仅第一张
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFirstLightProperty(void* _ins_light, uint32_t _index);

	//设置面阵属性——全八张
	extern "C" INSCAMERA_API InsResult InsPMDLight_Set8LightProperty(void* _ins_light, uint32_t _index, uint32_t _illuminate_mode);


	//0311

	extern "C" INSCAMERA_API InsResult InsPMDLight_SetDirection(void* _ins_light, int _BoardNum, int _Direction);

	extern "C" INSCAMERA_API InsResult InsPMDLight_SetPhaseOffset(void* _ins_light, uint32_t _index, int _Direction, int LightMode);


}


