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
 *  @brief  ��ȡSDK�汾��
 *  @param  
 *  @return ���ذ汾�ţ����緵��ֵΪ0x01000001����SDK�汾��ΪV1.0.0.1������ֵΪ0x01000101����SDK�汾��ΪV1.0.1.1
 *  @remarks��ȡSDK�汾�ţ�����ͷ�ļ���Lib�ļ���Dll�ļ���SDK�������ϣ���ֱ�ӵ���
 ************************************************************************/
	extern "C" INSCAMERA_API uint32_t __cdecl InsPMDL_GetSdkVersion();

/********************************************************************//**
 *  @~chinese
 *  @brief  Ѱ������豸����
 *  @param  _num						 [IN]            �豸����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindCameraDevice(int* _num);


/********************************************************************//**
 *  @~chinese
 *  @brief  �����豸���
 *  @param  _ins_handle                 [OUT]           �豸���
 *  @param  _index						[OUT]           �豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks 
 ************************************************************************/
    
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetCameraDeviceInfo(int _num, InsCameraInfo * _insCameraInfo);
	extern "C" INSCAMERA_API void __cdecl InsPMDL_CreateCameraHandle(void** _ins_handle, int _index);

/********************************************************************//***
 *  @~chinese
 *  @brief  ��ȡ��Դ�豸�����Ϣ 
 *  @param  _ins_light                 [IN]              ��Դ�豸���
 *  @param  _insdeviceinfo             [OUT]           �豸��Ϣ�ṹ��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindOtherInfo(void* _ins_light, OtherDeviceInfo * _insdeviceinfo);


/********************************************************************//**
 *  @~chinese
 *  @brief  Ѱ�ҹ�Դ�豸����
 *  @param  _num						 [IN]            �豸����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks 
 ************************************************************************/
extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FindLightSourceDevice(int* _num);

//��ȡ��Դ�豸��Ϣ
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetLightSouceDeviceInfo(int num, Ins2D5LightDeviceAndLocalIP* _deviceInfo);
//�µ�
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_ResetLightSourceIP(const char* _ip, Ins2D5LightDeviceAndLocalIP * _deviceInfo);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ��Դ�豸�ľ��
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _spinsdeviceinfo             [OUT]           �豸��Ϣ�ṹ��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
//��ȡ��Դ���
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetLightSourceHandle(void** _ins_light, Ins2D5LightDeviceAndLocalIP _deviceInfo);

//ƴ��ϵͳ��Ϣ 
extern "C" INSCAMERA_API std::string __cdecl   InsPMDL_GenerateSystemModel(std::string cameraModel, std::string lightSourceModel);

//���������SN�ŵ���Դ
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_SaveSNToLight(void* _ins_light, char _SN[64]);

//��ȡ��Դ����Ե������SN��
extern "C" INSCAMERA_API InsResult __cdecl  InsPMDL_GetSNFromLight(void* _ins_light, const char* _SN);

    //extern "C" INSCAMERA_API InsResult __cdecl ins_find_camera(INSDeviceInfo * insdeviceinfo);
	//extern "C" INSCAMERA_API InsResult __cdecl ins_get_camera_info(INSDeviceInfo * insdeviceinfo);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_grab_once(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  CameraLink�������ȡͼ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _index						 [IN]            ȡͼ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks ��Ҫ��Ӧʹ��ֹͣ�ɼ��ӿڣ�InsPMDL_GrabStop��_index 0-����ȡͼ��1-��������2-������3-IO����
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GrabContinous(void* _ins_handle);//_index 0-����ȡͼ��1-��������2-������3-IO����

/********************************************************************//**
 *  @~chinese
 *  @brief  ���ֹͣ�ɼ�
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _index						 [IN]            ȡͼ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks _index 0-����ȡͼ��1-��������2-����,3-IO����
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GrabStop(void* _ins_handle); 

/********************************************************************//**
 *  @~chinese
 *  @brief  ����������ȡͼ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _boardnum                    [IN]            ��Դ���ȳ���10
 *  @param  _picture_num                 [IN]            ��֡��
 *  @param  _index						 [IN]            ȡͼ������0-�أ�1-��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks ��Ҫ����ִ���ļ�Ŀ¼�µ�����ȡͼ������ļ���freerun.ccf�����ý����,�������ȡͼ�����������ã����·����������ļ�������᷵�ش����롣
 *           ��ʼʱ����SDK�ڲ������иߡ��ع�ʱ�䡢��ɨ�����ڡ���Ƶ�������������������ڹر�ʱ���ء�
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FreerunForFocus(void* _ins_handle, InsPMDL_Switch _index);//��λ���������ȡ��






    //extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureBoard(void* _ins_handle);//��Ӧ������250k�Ǹ�

/********************************************************************//**
 *  @~chinese
 *  @brief  250Kϵͳ�Ĳ�������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureBoardWith250K(void* _ins_handle);

	//�����ýӿ�
    extern "C" INSCAMERA_API InsResult __cdecl ins_SaveBoardConfigurationToFile(void* _ins_handle);
	extern "C" INSCAMERA_API InsResult __cdecl SaveCameraConfigurationToFile(void* _ins_handle,char* _filename);
	extern "C" INSCAMERA_API InsResult __cdecl LoadLocalFile(void* _ins_handle);//��ʱ
    extern "C" INSCAMERA_API InsResult __cdecl ins_configure_file_to_camera(void* _ins_handle, char* _filename);//����
    extern "C" INSCAMERA_API InsResult __cdecl ins_save_camera_configuration_to_file(void* _ins_handle, char* _filename);//����

/********************************************************************//**
 *  @~chinese
 *  @brief  ���������ļ���*.icf��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _filename                    [IN]            �������ļ�·����������.icf��׺
 *  @param  _boardnum                    [IN]            ��Դ���ȳ���10
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_LoadConfigureFileToSystem(void* _ins_handle, const char* _filename);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  ���������ļ���*.icf��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _filename                    [IN]            �������ļ�·����������.icf��׺
 *  @param  _boardnum                    [IN]            ��Դ���ȳ���10
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SaveSystemConfigFile(void* _ins_handle, const char* _filename);

/********************************************************************//**
 *  @~chinese
 *  @brief  �ر��豸���ͷ���Դ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_Close(void** _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  �������ú�����ͼ�񻺳���
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks ������ͼ���иߺͿ�֮�����Ҫ���µ��øýӿ�
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureFrameGrabber(void* _ins_handle);//Ŀǰ����

/********************************************************************//**
 *  @~chinese
 *  @brief  ע��ȡͼ�ص�����һ֡ͼ��ɼ��������ûص�����
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _callback                    [IN]            �ص���������
 *  @return 
 *  @remarks ͨ���ýӿڻ��ͼ��,��Ҫ����InsPMDL_UnregisterDataCallback�ӿ���ע���ص�
 ************************************************************************/
    extern "C" INSCAMERA_API void InsPMDL_SetDataCallback(void* _ins_handle, DataPocessCallBack _callback);

/********************************************************************//**
 *  @~chinese
 *  @brief  ע��ȡͼ�ص�
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return 
 *  @remarks ע��ȡͼ�ص�����
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_UnregisterDataCallback(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  ע��ϵͳ���߻ص�����������ߺ�����ûص�����
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _callback                    [IN]            �ص���������
 *  @return 
 *  @remarks ͨ���ýӿڻ�õ�����Ϣ����Ҫ����InsPMDL_SetDeviceRemoveCallback�ӿ�ע���ص�
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_SetDeviceRemoveCallback(void* _ins_handle, DataPocessCallBack _callback);

/********************************************************************//**
 *  @~chinese
 *  @brief  ע��ϵͳ���߻ص�
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return 
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API void InsPMDL_UnregisterDeviceRemoveCallback(void* _ins_handle);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  ��õ�ǰ֡������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ������������������󣬷���INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetFrameSize(void* _ins_handle);



	//�û��ò���
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_trigger_source_activated(void* _ins_handle, int index);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_work_mode_activated(void* _ins_handle, int index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ����ͼ����������
 *  @param  _ins_handle                  [IN]            ����豸��� 
 *  @param  _ins_width                   [IN]            ͼ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetWidth(void* _ins_handle, int _ins_width);

/********************************************************************//**
 *  @~chinese
 *  @brief  ����ͼ��߶�������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ins_height                  [IN]            ͼ��߶�
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetHeight(void* _ins_handle, int _ins_height);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡͼ����������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������ͼ���ȣ����󣬷���INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetWidth(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡͼ��߶�������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������ͼ��߶ȣ����󣬷���INT_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetHeight(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���û�����֡��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ins_frame_count             [IN]            ֡��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks �û�Ӧ���ò�������д���û��ֲ���
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetFrameCount(void* _ins_handle, int _ins_frame_count);

    //�û��ò���
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_image_type(void* _ins_handle, int index);
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_line_trigger(void* _ins_handle);//�����д���ģʽ
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_frame_trigger(void* _ins_handle);

//�������ӿ��Ѿ��ӽ��˵�������ȡͼ�У��û�Ӧ���ò�����ʵ����Ҳ����û�õ�
/********************************************************************//**
 *  @~chinese
 *  @brief  �����ⲿ�����ģ�.ccf���ļ�
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks �����·�����ļ���Ϊ"./externalpulse.ccf"���ļ����ý�ϵͳ������ȡͼ��������Ҫ���ô˽ӿ�
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureExternalPulseFileToCamera(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��������ȡͼ�ģ�.ccf���ļ�
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks �����·�����ļ���Ϊ"./freerun.ccf"���ļ����ý�ϵͳ���ڵ���ʱ��Ҫ����ȡͼ������ȡͼǰ��Ҫ���ô˽ӿ�
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ConfigureFreeRunFileToCamera(void* _ins_handle);
    

/********************************************************************//**
 *  @~chinese
 *  @brief  �����ع�ʱ��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _val                         [IN]            �ع�ʱ��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks �ع�ʱ�䲻�ܴ�����ɨ������ max=10000000.000000000 min=2.0000000000000000	���Ϊinc=0.1
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetExposure(void* _ins_handle, double _val);


/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ�ع�ʱ��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return ����ֵΪ�ع�ʱ�䣬��ȡʧ��ʱ����DBL_MAX
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API double __cdecl InsPMDL_GetExposure(void* _ins_handle);

	//�û��ò���
    //extern "C" INSCAMERA_API InsResult __cdecl ins_set_line_period_time(void* _ins_handle, double _val);//��Ƶ��max=10000000.000000000 min=5.0000000000000000	inc=0.1 //����ֵΪ18��ʾ����������Χ
    //extern "C" INSCAMERA_API double __cdecl ins_get_line_period_time(void* _ins_handle);


/********************************************************************//**
 *  @~chinese
 *  @brief  ������������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _val                         [IN]            ��������
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks max=8 min=0.01	inc=0.01
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetDigitalGain(void* _ins_handle, double _val);//

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ��������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return ����ֵΪ�������棬��ȡʧ��ʱ����DBL_MAX
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API double __cdecl InsPMDL_GetDigitalGain(void* _ins_handle);

/********************************************************************//**
 *  @~chinese
 *  @brief  ����ģ������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ch                          [IN]            ģ������
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks ins_PAC_gainֵ��Ϊ��
			 #define INS_PAC_GAIN_X1 "x1";
			 #define INS_PAC_GAIN_X2 "x2";
			 #define INS_PAC_GAIN_X5 "x5";
			 #define INS_PAC_GAIN_X10 "x10";
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetPacGain(void* _ins_handle, char* _ch);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡģ������
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ch                          [OUT]           ģ������
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks ins_PAC_gainֵ��Ϊ��
			 #define INS_PAC_GAIN_X1 "x1";
			 #define INS_PAC_GAIN_X2 "x2";
			 #define INS_PAC_GAIN_X5 "x5";
			 #define INS_PAC_GAIN_X10 "x10";
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_GetPacGain(void* _ins_handle, char* _ch);
	
/********************************************************************//**
 *  @~chinese
 *  @brief  ���ô�����Сֵ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ins_shaft_encoder1_min_width[IN]            ������Сֵ
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks �����������ź��˲�����
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetShaftEncoder1MinWidth(void* _ins_handle, int _ins_shaft_encoder1_min_width);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ������Сֵ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return �ɹ������ش�����Сֵ�����󣬷���INT_MAX
 *  @remarks �����������ź��˲�����
 ************************************************************************/
    extern "C" INSCAMERA_API int __cdecl InsPMDL_GetShaftEncoder1MinWidth(void* _ins_handle);


/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ��ǰ֡��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return ����֡��
 *  @remarks 
 ************************************************************************/
	extern "C" INSCAMERA_API int __cdecl InsPMDL_GetFrameId(void* _ins_handle);

	//�û��ò���
	//extern "C" INSCAMERA_API InsResult __cdecl ins_user_set_save(void* _ins_handle);//�ϵ籣������������


/********************************************************************//**
 *  @~chinese
 *  @brief  ��ʼ�������豸��ַ
 *  @param  _ip                          [IN]            ��ǰ�ͻ���IP��ַ
 *  @param  _NET_mask                    [IN]            ��ǰ�ͻ�����������
 *  @param  _gateway                     [IN]            ��ǰ�ͻ�������
 *  @remarks 
 ************************************************************************/
    extern "C" INSCAMERA_API void __cdecl InsPMDL_InitLocalInfo(const char* _ip, const char* _NET_mask, const char* _gateway);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���豸����ȡ��ǰ�豸IP����Ϣ
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ins_device                  [OUT]           ��ǰ�豸IP��ַ���������롢���ص���Ϣ
 *  @return �ɹ�����0
 *  @remarks
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SearchDevice(void* _ins_handle, Ins2D5LightDevice * _ins_device);

/********************************************************************//**
 *  @~chinese
 *  @brief  �����豸IP
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _ip                          [OUT]           �û��Զ����豸IP��ַ
 *  @return return -1 means that the ip is wrong,return 0 means success
 *  @remarks ���������豸�ýӿں�ʹ��
 ************************************************************************/
    extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_ResetDeviceIp(void* _ins_handle, const char* _ip);

    //-1 failure�����ص�Device��Ϊ��ǰ�豸��Ϣ
    //extern "C" INSCAMERA_API int factory_configuration(void* _ins_handle, Device * ins_device);//11.18�����������ã���Ҫ�Ȱ��������ý�device ����,�������ʧ�ܣ����ص�device �������ַ���Ϊ��
	
/********************************************************************//**
 *  @~chinese
 *  @brief  ͨѶ���ӳ�ʼ��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @return return -1 means that the ip is wrong,return 0 means success
 *  @remarks ���Բ�ʹ�øýӿڣ����������Ӵ��󣬿����½��г�ʼ��
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_InitCommunication(void* _ins_light);
	
	
/********************************************************************//**
 *  @~chinese
 *  @brief  �ϵ籣��
 *  @param  _ins_handle                  [IN]            ����豸���
 *  @param  _board_num                   [IN]            ��Դ���ȳ���10
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_PowerOffSave(void* _ins_handle);

//-----------------------------------------Add New ------------------------

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FpgaRegInitial(void* _ins_handle);


	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_FPGAInitial(void* _ins_light);

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SelectDirection(void* _ins_light, int index);//����λ�� 0 ��1 ��2��3

	extern "C" INSCAMERA_API InsResult __cdecl InsPMDL_SetTimeOut(void* _ins_light, int time);

	extern "C" INSCAMERA_API int __cdecl ins_save_camera_height(void* ins_light, uint32_t * buffer);

	extern "C" INSCAMERA_API int __cdecl ins_read_camera_height(void* ins_light, uint32_t * buffer);//32

	extern "C" INSCAMERA_API int __cdecl InsPMDL_ResetSin(void* ins_light);

	extern "C" INSCAMERA_API int __cdecl InsPMDL_InitialLightBrightness(void* ins_light);//��ʼ����Դ�ӿ� AFF->7FF

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

	//���������������������������������������ã����ܣ�����������������������������������������
/********************************************************************//**
 *  @~chinese
 *  @brief  ����ϵͳɨ������
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _board_num                   [IN]            ��Դ���ȳ���10
 *  @param  _line_scanning_period        [IN]            ɨ������(��λ��΢��)
 *  @param  _line_fre                    [OUT]           ��Ƶ(��λ��KHz)
 *  @param  _exposure                    [OUT]           �ع�ʱ��(��λ��΢��)
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks����ϵͳ��ǰɨ������_line_scanning_period(��λ��΢��),�ͻ�ȡ��Ӧ����Ƶ_line_fre(��λ��KHz),��ɨ������Ӧ�����ع�ʱ��+2,���뷶Χ��4~100��
 *			�Զ���������ع����������������ع�
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetScanningPeriod(void* _ins_handle,  double _line_scanning_period, double& _line_fre, double& _exposure);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡϵͳɨ������
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _board_num                   [IN]            ��Դ���ȳ���10
 *  @param  _line_scanning_period        [OUT]           ɨ������(��λ��΢��)
 *  @param  _line_fre                    [OUT]           ��Ƶ(��λ��KHz)
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks��õ�ǰϵͳɨ������_line_scanning_period(��λ��΢��)����Ƶ_line_fre(��λ��KHz)
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetScanningPeriod(void* _ins_light, double& _line_scanning_period, double& _line_fre);


	//�����������������������������������������á���������������������������������������
/********************************************************************//**
 *  @~chinese
 *  @brief  ������У�飬A-B��ȡ��
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [IN]            ����������1-����0-��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks������У�飬A-B��ȡ��1����0��
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetABPhaseInversionOnOff(void* _ins_light, InsPMDL_Switch _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���ô����г̷���
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [IN]            ����������0-�����г̣�1-���˸���(���򲻼�)��2-���˺��ԣ�3-˫��ɨ��
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks���ô����г̷���,0-�����г̣�1-���˸���(���򲻼�)��2-���˺��ԣ�3-˫��ɨ��
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetTriggerDirection(void* _ins_light, int _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���ñ���������ģʽ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [IN]            ����������0-A�������أ����������1-A�������أ�˫�������2-A��˫���أ�˫�������3-A&B��˫���أ�˫�����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���÷�Ƶ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _frequency_division			 [IN]            ��Ƶ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetFrequencyDivision(void* _ins_light, uint32_t _frequency_division);

/********************************************************************//**
 *  @~chinese
 *  @brief  ���ñ�Ƶ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _frequency_multiplier		 [IN]            ��Ƶ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks_frequency_multiplierֵ����>1
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetFrequencyMultiplier(void* _ins_light, uint32_t _frequency_multiplier);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ�����г̷���
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [OUT]           ��������
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks��ȡ��������,0-�����г̣�1-���˸��٣�2-���˺��ԣ�3-˫��ɨ��
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetTriggerDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ����������ģʽ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [OUT]           ��������
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks��ȡ����������ģʽ��0-A�������أ����������1-A�������أ�˫�������2-A��˫���أ�˫�������3-A&B��˫���أ�˫��
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEncoderTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ��Ƶ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [OUT]           ��Ƶ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetFrequencyDivision(void* _ins_light, uint32_t& _index);

/********************************************************************//**
 *  @~chinese
 *  @brief  ��ȡ��Ƶ
 *  @param  _ins_light                   [IN]            ��Դ�豸���
 *  @param  _index						 [OUT]           ��Ƶ����
 *  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
 *  @remarks
 ************************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetFrequencyMultiplier(void* _ins_light, uint32_t& _index);


	//����������������������������������������IO�������á���������������������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ����io����ģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index				  	    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks����io����ģʽ,ֻ���������ӵİ汾,to do
*           0-ģʽ1��IO��0����ʼ��IO��1������,
*	        1-ģʽ2��IO��1����ʼ��IO��0������,
*	        2-ģʽ3��IO��0����ʼ������̶���������,
*	        3-ģʽ4��IO��1����ʼ������̶���������,
*	        4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
*	        5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
*	        6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
*	        7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ����io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _trinum					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks����io��������,ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerNum(void* _ins_light,  uint32_t _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  ����io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _count					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks����io��������,ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOPulseCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  ����io����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _width					    [IN]            ������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks����io����������,ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerPulseWidth(void* _ins_light,  uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  ����io�����ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _delay					    [IN]            �����ӳ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks����io�����ӳ�,ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerDelay(void* _ins_light, uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ����io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _period					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��io��������,ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerPeriod(void* _ins_light,  uint32_t _period);

/********************************************************************//**
*  @~chinese
*  @brief  ����io�������뼫��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��io�������뼫��,0-���֣�1-��ת��ֻ���������ӵİ汾,to do
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIOTriggerInputPolarity(void* _ins_light,uint32_t _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio����ģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index				  	    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��ȡio����ģʽ,ֻ���������ӵİ汾,to do
*           0-ģʽ1��IO��0����ʼ��IO��1������,
*	        1-ģʽ2��IO��1����ʼ��IO��0������,
*	        2-ģʽ3��IO��0����ʼ������̶���������,
*	        3-ģʽ4��IO��1����ʼ������̶���������,
*	        4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
*	        5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
*	        6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
*	        7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _trinum					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerNum(void* _ins_light, uint32_t& _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _count					    [OUT]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOPulseCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _width					    [OUT]            ������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerPulseWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio�����ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _delay					    [OUT]           �����ӳ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _period					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerPeriod(void* _ins_light, uint32_t& _period);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡio�������뼫��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��ȡio�������뼫�ԣ�0-���֣�1-��ת
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIOTriggerInputPolarity(void* _ins_light, uint32_t& _index);


	//����������������������������������Դ�������á���������������������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	���ù�Դģʽ��0-����ģʽ��1-��ģʽ��2-��׼ģʽ��3-Сģʽ��4-��Сģʽ
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEmissionMode(void* _ins_handle, InsPMDL_LineMode LineMode);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��Դ����ģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	��ȡ��Դ����ģʽ��0-����ģʽ��1-��ģʽ��2-��׼ģʽ��3-Сģʽ��4-��Сģʽ
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEmissionMode(void* _ins_light, int& _index);

/********************************************************************//**
*  @~chinese
*  @brief  ����X�����ƶ�����
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	����X�����ƶ�����0-����1-����
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetXStripeMovementDirection(void* _ins_light,  int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ����Y�����ƶ�����
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	����Y�����ƶ�����0-����1-����
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetYStripeMovementDirection(void* _ins_light,  int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ����X������ʼ��λ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	����X������ʼ��λ��0-0�㣬1-90�㣬2-180�㣬3-270��
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetInitialPhaseInXDirection(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ����Y������ʼ��λ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index					    [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	����Y������ʼ��λ��0-0�㣬1-90�㣬2-180�㣬3-270��
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetInitialPhaseInYDirection(void* _ins_light,int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡX�����ƶ�����
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	��ȡX�����ƶ�����0-����1-����
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetXStripeMovementDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡY�����ƶ�����
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index					    [OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	��ȡY�����ƶ�����0-����1-����
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetYStripeMovementDirection(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡX������ʼ��λ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _initial_phase				[OUT]           ��ʼ��λ 
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	_initial_phaseֵ��Ϊ0�㣬90�㣬180�㣬270��
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetInitialPhaseInXDirection(void* _ins_light, uint32_t& _initial_phase);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡY������ʼ��λ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _initial_phase				[OUT]           ��ʼ��λ
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	_initial_phaseֵ��Ϊ0�㣬90�㣬180�㣬270��
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetInitialPhaseInYDirection(void* _ins_light, uint32_t& _initial_phase);

	//��������������������������������������Դ�������á�����������������������������������

	//������������������������������������������Դ����ģʽ������������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ��������ʹ�ܺ�ֹͣʹ��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index			        	[IN]            ����ϵ��
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	_index:0-disable,1-enable
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SecondaryTriggerEnableAndDisable(void* _ins_light,double _index);

/********************************************************************//**
*  @~chinese
*  @brief  ���ñ�����������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _encodewidth			        [IN]            ������������������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks	
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderSecondaryTriggerPulseWidth(void* _ins_light,  double _encodewidth);

/********************************************************************//**
*  @~chinese
*  @brief  ���ñ�����������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _boardnum                    [IN]            ��Դ���ȳ���10
*  @param  _index			            [IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��������,0-�����г̣�1-���˸���(���򲻼�)��2-���˺��ԣ�3-˫��ɨ��,ֻд�˵�һ����ӵļĴ���
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderSecondaryTriggerTravelDirection(void* _ins_light,  int _index);
	
/********************************************************************//**
*  @~chinese
*  @brief  ���ñ���������������Ƶ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _frequency_division			[IN]            ��Ƶ����
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks //to do  û�ҵ�����������ģʽ
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSecondaryFrequencyDivision(void* _ins_light, uint32_t _frequency_division);

/********************************************************************//**
*  @~chinese
*  @brief  ���ñ���������������Ƶ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _frequency_multiplier		[IN]            ��Ƶ����
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks //���ö���������Ƶ
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSecondaryFrequencyMultiplier(void* _ins_light, uint32_t _frequency_multiplier);

	//��������������������������������������������������������������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ����������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _count						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks 
* ***********************************************************************/
	//����������������
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftwareTriggerTriggerCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  ���ô�������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _period						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	//���ô�������
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftwareTriggerTriggerPeriod(void* _ins_light, double _period);

/********************************************************************//**
*  @~chinese
*  @brief  ���������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _count						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	//���������������
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftwareTriggerTriggerCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  ���������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _period						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	//��ô�������
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftwareTriggerTriggerPeriod(void* _ins_light, double& _period);

	//������������������������������������IO������������������������������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io����ģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks_indexȡֵ��
		   0-ģʽ1��IO��0����ʼ��IO��1������,
	       1-ģʽ2��IO��1����ʼ��IO��0������,
	       2-ģʽ3��IO��0����ʼ������̶���������,
	       3-ģʽ4��IO��1����ʼ������̶���������,
	       4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
	       5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
	       6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
	       7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerMode(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _trinum						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerNum(void* _ins_light,  uint32_t _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _count						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryPulseCount(void* _ins_light, uint32_t _count);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _width						[IN]            �������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerPulseWidth(void* _ins_light, uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io�����ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _delay						[IN]            �����ӳ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerDelay(void* _ins_light,  uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _period						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerPeriod(void* _ins_light, uint32_t _period);

/********************************************************************//**
*  @~chinese
*  @brief  ���ö�������io���뼫��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks���ö�������io�������뼫��,0-���֣�1-��ת
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetIoSecondaryTriggerInputPolarity(void* _ins_light,  uint32_t _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io����ģʽ
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index						[OUT]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks ��ȡ��������io����ģʽ,_index��
			0-ģʽ1��IO��0����ʼ��IO��1������,
			1-ģʽ2��IO��1����ʼ��IO��0������,
			2-ģʽ3��IO��0����ʼ������̶���������,
			3-ģʽ4��IO��1����ʼ������̶���������,
			4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
			5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
			6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
			7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerMode(void* _ins_light, uint32_t& _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _count						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerNum(void* _ins_light, uint32_t& _trinum);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _count						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryPulseCount(void* _ins_light, uint32_t& _count);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _width						[OUT]           ������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerPulseWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io�����ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _delay						[OUT]           �����ӳ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index						[OUT]			��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerPeriod(void* _ins_light, uint32_t& _period);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������io�������뼫��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _period						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��ȡ��������io�������뼫�ԣ�0-���֣�1-��ת
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetIoSecondaryTriggerInputPolarity(void* _ins_light, uint32_t& _index);
	//��������������������������������Դ����ģʽ������������������������





	//��������������������������������ģʽ������������������������
	
/********************************************************************//**
*  @~chinese
*  @brief  ����ʹ�ܺ�ֹͣʹ��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index						[IN]			ʹ����������
*  @param  _quantity					[IN]            ��չ�����������,һ��Ϊ1����λ���Σ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks ����ʹ�ܺ�ֹͣʹ��,_index:0-disable,1-enable��ֹͣʹ��ʱ������_quantity����Ϊ������
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_ExtendedTriggerEnableAndDisable(void* _ins_light,  double _index, int _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ������չ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _quantity					[IN]            ��չ�����������,һ��Ϊ1����λ���Σ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks 
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputQuantity(void* _ins_light, uint32_t _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ������չ��������ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _delay						[IN]            ��չ��������ӳ�(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputDelay(void* _ins_light, uint32_t _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ������չ����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _width						[IN]            ������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputWidth(void* _ins_light,  uint32_t _width);

/********************************************************************//**
*  @~chinese
*  @brief  ������չ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _index						[IN]            ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks������չ�����������,0-�����أ�1-�½���
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetExtendedTriggerOutputPolarity(void* _ins_light, int _index);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��չ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _quantity					[OUT]           ��չ�����������,һ��Ϊ1����λ���Σ�
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputQuantity(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��չ��������ӳ�
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _delay						[OUT]           ��չ��������ӳ�(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputDelay(void* _ins_light, uint32_t& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��չ����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _width						[OUT]           ��չ����������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputWidth(void* _ins_light, uint32_t& _width);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��չ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _index						[OUT]           ��������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��ȡ��չ�����������,0-�����أ�1-�½���
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetExtendedTriggerOutputPolarity(void* _ins_light, uint32_t& _index);
	//��������������������������������ģʽ������������������������

#
	//�����������������������������߼����á���������������������������
/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դ��Ӧʱ��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _delay						[IN]            ��Դ��Ӧʱ��(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks���ù�Դ��Ӧʱ��(��λ��΢��)ֵ���룾03
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceResponseTime(void* _ins_light,  double _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դ�������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _width						[IN]            ��Դ�������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks���ù�Դ�������(��λ��΢��)ֵ����<=ϵͳ��ɨ�����ڵ�һ��
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceTriggerOutputWidth(void* _ins_light,double _width);

/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դ���ȵ���
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _percentage					[IN]            ��Դ���ȵ���(��λ��%)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks���ù�Դ���ȵ���(��λ��%),��������ӦΪ���� , ��Χ��1~100
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceBrightnessAdjustment(void* _ins_light, uint32_t _percentage);

/********************************************************************//**
*  @~chinese
*  @brief  ���ñ�������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _encode_width				[IN]            ��������������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetEncoderTriggerPulseWidth(void* _ins_light, double _encode_width);

/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _quantity					[IN]            ��Դ�����������(��λ����)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetLightSourceTriggerOutputQuantity(void* _ins_light,  uint32_t _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��Դ��Ӧʱ��
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _delay						[OUT]           ��Դ��Ӧʱ��(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceResponseTime(void* _ins_light, double& _delay);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��Դ�������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _width						[OUT]           ��Դ�������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks //�������ܸģ�to do 
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceTriggerOutputWidth(void* _ins_light, double& _width);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��Դ���ȵ���
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _percentage					[OUT]           ��Դ���ȵ���(��λ��%)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��Դ���ȵ���,ȡֵ��Χ��1~100
* ***********************************************************************/
	//��ȡ��Դ���ȵ���(��λ��%), ��Χ��1~100
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceBrightnessAdjustment(void* _ins_light, uint32_t& _percentage);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _board_num                   [IN]            ��Դ���ȳ���10
*  @param  _encode_width				[OUT]           ��������������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetEncoderTriggerPulseWidth(void* _ins_light,double& _encode_width);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��Դ�����������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _quantity					[OUT]           ��Դ�����������(��λ����)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetLightSourceTriggerOutputQuantity(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ���ù�Դ��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _quantity					[IN]            ��Դ��������(��λ����)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks��������(��λ����)ֵ���룾0
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftTriggerNum(void* _ins_light, uint32_t _num);

/********************************************************************//**
*  @~chinese
*  @brief  ������������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _quantity					[IN]            ��������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_SetSoftTriggerPeriod(void* _ins_light, double _period);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _quantity					[OUT]           ��������(��λ����)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftTriggerNum(void* _ins_light, uint32_t& _quantity);

/********************************************************************//**
*  @~chinese
*  @brief  ��ȡ��������
*  @param  _ins_light                   [IN]            ��Դ�豸���
*  @param  _period						[OUT]           ��������(��λ��΢��)
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_GetSoftTriggerPeriod(void* _ins_light, double& _period);


	//-------------------------------------------------------------



	//�����������������������������߼����á���������������������������

	//���������أ���1������0��
	extern "C" INSCAMERA_API InsResult InsPMDL_OpenOrCloseLasers(void* _ins_handle, uint16_t buffer);

	//�����λ�ˣ�pitchΪǰ����
	extern "C" INSCAMERA_API int InsPMDL_GetCameraPitch(void* ins_light,double* buffer);
	//�����λ�ˣ�rollΪ���Ҹ���
	extern "C" INSCAMERA_API int InsPMDL_GetCameraRoll(void* ins_light, double* buffer);
	//�����λ�ˣ�yawΪˮƽ��ת��
	extern "C" INSCAMERA_API int InsPMDL_GetCameraYaw(void* ins_light, double* buffer);
	//����Դλ�ˣ�pirchΪǰ����
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourcePitch(void* ins_light, double* buffer);
	//����Դλ�ˣ�rollΪ���Ҹ���
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourceRoll(void* ins_light, double* buffer);
	//����Դλ�ˣ�yawΪˮƽ��ת��
	extern "C" INSCAMERA_API int InsPMDL_GetLightSourceYaw(void* ins_light, double* buffer);
	//��������и�
	//extern "C" INSCAMERA_API int save_camera_height(uint32_t * buffer);
	//extern "C" INSCAMERA_API int read_camera_height(uint32_t * buffer);
	//��ȡFPGA�¶�
	extern "C" INSCAMERA_API int InsPMDL_GetFpgaTemp(void* _ins_handle, double* buffer);
	//��ȡ�����¶�
	extern "C" INSCAMERA_API int InsPMDL_GetBoardTemp(void* _ins_handle, double* buffer);

    //��֪�豸IP������£���ͨ�����豸����ȡ�豸IP��������ͨѶ��ʼ��
	//extern "C" INSCAMERA_API InsResult InsPMDL_InitCommunication(void* _ins_handle, const char* _ip);

/********************************************************************//**
*  @~chinese
*  @brief  �߼�Ȩ�޶��Ĵ���
*  @param  _ins_handle                  [IN]            ����豸���
*  @param  _input						[IN]            �ض��ַ���
*  @param  _id							[IN]            Ƭѡ�ư��
*  @param  _address						[IN]            �Ĵ�����ַ
*  @param  _buffer						[OUT]           ����������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarks		_id:Ƭѡ�ư��,������ֵΪ���£�
				EXTSTM32 = 101,
				INTSTM32 = 100,
				FIRSTBOARD = 1,
				SECONDBOARD = 2,
				THIRDBOARD = 3,
				FOURTHBOARD = 4,
				FIFTHBOARD = 5,
				�Ĵ�����ַ����Ҫƫ�ƣ��ڲ��Ѿ�/2
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_ReadRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t & _buffer);

/********************************************************************//**
*  @~chinese
*  @brief  �߼�Ȩ��д�Ĵ���
*  @param  _ins_handle                  [IN]            ����豸���
*  @param  _input						[IN]            �ض��ַ���
*  @param  _id							[IN]            Ƭѡ�ư��
*  @param  _address						[IN]            �Ĵ�����ַ
*  @param  _buffer						[IN]            д�������
*  @return �ɹ�������InsResult::INSCSTATUS_OK����InsResult::INSSTATUS_SUCCESS�����󣬷��ش�����
*  @remarksid	_id:Ƭѡ�ư��,������ֵΪ���£�
				EXTSTM32 = 101,
				INTSTM32 = 100,
				FIRSTBOARD = 1,
				SECONDBOARD = 2,
				THIRDBOARD = 3,
				FOURTHBOARD = 4,
				FIFTHBOARD = 5,
				�Ĵ�����ַ����Ҫƫ�ƣ��ڲ��Ѿ�/2
* ***********************************************************************/
	extern "C" INSCAMERA_API InsResult InsPMDL_WriteRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t _buffer);


	//================0304==========�������ӿ�===============================
	extern "C" INSCAMERA_API InsResult write_fpga_reg(void* _ins_light, const char* _input, int32_t index, uint32_t _addr, uint32_t _value);


	//��ʼ��
	extern "C" INSCAMERA_API InsResult InsPMDLight_Initial(void* _ins_light, int BoardNum);

	// ���������ԴIO��������
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerCycle(void* _ins_light, uint32_t _index, double _time_ms);

	// ���������ԴIO��������
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerNum(void* _ins_light, uint32_t _index, int _num);

	// ���������ԴIO����ʹ��
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIOTriggerEn(void* _ins_light, uint32_t _index, bool _enable);

	//��Դ����ʱ��(��λ��ms)
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIlluminationTime(void* _ins_light, uint32_t _index, double _time);

	//���󷢹�ģʽ
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetLightMode(void* _ins_light, uint32_t _index, int index);

	//����ȡ�񿪣�
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFreeRunOn(void* _ins_light, uint32_t _index);

	//����ȡ��أ�
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFreeRunOff(void* _ins_light, uint32_t _index);

	//������8�ţ�����
	extern "C" INSCAMERA_API InsResult InsPMDLight_Trigger_8_ImagesOn(void* _ins_light, uint32_t index, uint32_t _time, uint32_t _delaytime);

	//������8�ţ��أ�
	extern "C" INSCAMERA_API InsResult InsPMDLight_Trigger_8_ImagesOff(void* _ins_light, int32_t index);

	//����ʱ��10ms�¶�Ӧ�������㼰����
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetIlluminationTime10ms(void* _ins_light, uint32_t index, double _time);

	//��������ģʽʹ��
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetArrayEn(void* _ins_light,uint32_t _index, bool _enable);

	//�����������ԡ�������һ��
	extern "C" INSCAMERA_API InsResult InsPMDLight_SetFirstLightProperty(void* _ins_light, uint32_t _index);

	//�����������ԡ���ȫ����
	extern "C" INSCAMERA_API InsResult InsPMDLight_Set8LightProperty(void* _ins_light, uint32_t _index, uint32_t _illuminate_mode);


	//0311

	extern "C" INSCAMERA_API InsResult InsPMDLight_SetDirection(void* _ins_light, int _BoardNum, int _Direction);

	extern "C" INSCAMERA_API InsResult InsPMDLight_SetPhaseOffset(void* _ins_light, uint32_t _index, int _Direction, int LightMode);


}


