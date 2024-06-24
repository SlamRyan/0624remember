#pragma once
#include "base_camera.h"

#define MV_IP_CFG_STATIC  0x05000000  //�̶�IP��ַģʽ
#define MV_IP_CFG_DHCP  0x06000000  //DHCP�Զ���ȡIPģʽ
#define MV_IP_CFG_LLA  0x04000000  //LLA(Link - local address)����·���ص�ַ


/// \~chinese GigE�豸��Ϣ              \~english GigE device info
struct GIGE_DEVICE_INFO
{
    unsigned int        nIpCfgOption;                               ///< [OUT] \~chinese IP����ѡ��             \~english IP Configuration Options
    unsigned int        nIpCfgCurrent;                              ///< [OUT] \~chinese ��ǰIP����             \~english IP Configuration:bit31-static bit30-dhcp bit29-lla
    unsigned int        nCurrentIp;                                 ///< [OUT] \~chinese ��ǰIP��ַ             \~english Current Ip
    unsigned int        nCurrentSubNetMask;                         ///< [OUT] \~chinese ��ǰ��������           \~english Curtent Subnet Mask
    unsigned int        nDefultGateWay;                             ///< [OUT] \~chinese ��ǰ����               \~english Current Gateway
    unsigned char       chManufacturerName[32];                     ///< [OUT] \~chinese ����������             \~english Manufacturer Name
    unsigned char       chModelName[32];                            ///< [OUT] \~chinese �ͺ�����               \~english Model Name
    unsigned char       chDeviceVersion[32];                        ///< [OUT] \~chinese �豸�汾               \~english Device Version 
    unsigned char       chManufacturerSpecificInfo[48];             ///< [OUT] \~chinese �����̵ľ�����Ϣ       \~english Manufacturer Specific Information
    unsigned char       chSerialNumber[16];                         ///< [OUT] \~chinese ���к�                 \~english Serial Number
    unsigned char       chUserDefinedName[16];                      ///< [OUT] \~chinese �û��Զ�������         \~english User Defined Name 
    unsigned int        nNetExport;                                 ///< [OUT] \~chinese ����IP��ַ             \~english NetWork IP Address

    unsigned int        nReserved[4];                               ///<       \~chinese Ԥ��                   \~english Reserved

};

typedef void (* DataPocessCallBack)(void* handle,unsigned char* pData);

class INSCAMERA_API MvsCamera:public InsCamera
{
public:
    MvsCamera() {};
    ~MvsCamera();

	void* mhandle = NULL;
    std::vector<GIGE_DEVICE_INFO> mMvsCameraInfo;//�豸��Ϣ�б�
    static uint32_t numCameras_;
    static bool cam_update_;//��һ��ˢ�²���
    static uint32_t frame_id_;
    //static bool m_save_Image_;
    //static int lines_;
    static int ins_frame_size_;
    std::vector<std::string> full_name_;//����� �ڵ���findcamera����ֵ
    std::vector<std::string> device_class_;//�����������
   /* char fullName_[128];
    char deviceClass_[128];*/
    uint32_t num_devices_ = 0;//�������
    int mins_frame_size_ = 0;
	int mins_height_ = 0;
	int mins_width_ = 0;
	double ins_exposure_ = 0;
	double ins_digital_gain_ = 0;

    InsResult ConnectCamera(int _index);//������Ҫ�򿪵�������
    InsResult FindCamera();



    /********************************************************************//**
 *  @~chinese
 *  @brief  ǿ��IP
 *  @param  handle                      [IN]            �豸���
 *  @param  nIP                         [IN]            ���õ�IP
 *  @param  nSubNetMask                 [IN]            ��������
 *  @param  nDefaultGateWay             [IN]            Ĭ������
 *  @return �ɹ�,����MV_OK,ʧ��,���ش�����
 *  @remarks ǿ�������豸�������������IP���������롢Ĭ�����أ���ǿ������֮����Ҫ���´����豸�������GigEVision�豸֧�֡�\n
             ����豸ΪDHCP��״̬�����øýӿ�ǿ�������豸�������֮���豸����������

************************************************************************/
    InsResult ForceIpEx( unsigned int _nIP, unsigned int _nSubNetMask, unsigned int _nDefaultGateWay);

    /**
     *  @brief      ����IP��ʽ
     *  @param[in]  nType                                   IP���ͣ���MV_IP_CFG_x
     *  @return     �ɹ�,����MV_OK,ʧ��,���ش�����
     *  @remarks �������������豸��IP��ʽ����DHCP��LLA�ȣ���GigEVision�豸֧�֡�
     */
    InsResult SetIpConfig( unsigned int nType);
   /**
     *  @brief  Thisis a brief description.
	 *  @param[in]  _index == 0_���ô���ģʽΪoff
	                _index == 1_��������ģʽ
	                _index == 2_������·0����ģʽ
	                _index == 3_������·2����ģʽ
	                _index == 4)_���ü�����0����ģʽ
     *  @param[out] outArgName output argument description.
     *  @Return:    InsResult:description of the return value
     */

    InsResult set_trigger_mode(int _index);

	InsResult send_soft_trigger_sig();
    /**
      *  @brief  ����SDK�ڲ�ͼ�񻺴�ڵ���������ڵ���1����ץͼǰ����
      *  @param[in]  �ڲ��������ڵ�����������Ĭ��Ϊ1
      *  @param[out] outArgName output argument description.
      *  @Return:    InsResult:description of the return value
      */
    InsResult SetImageNodeNum(unsigned int nNum);

    /**
      *  @brief  ע��ץͼ�ص�
      *  @param[in]  inArgName input argument description.type name:purpose(Range)
      *  @param[out] outArgName output argument description.
      *  @Return:    :description of the return value
      */
	InsResult RegisterImageCallBack(DataPocessCallBack _callback);
	InsResult AntiRegisterImageCallBack();
	InsResult GrabContinous();
    /**
    *  @brief  ʹ���ڲ������ȡһ֡ͼƬ
    *  @param[in]  
    *  @param[out] pstFrame                           ͼ�����ݺ�ͼ����Ϣ
    *  @param[in]  nMsec                              �ȴ���ʱʱ�䣬����0xFFFFFFFFʱ��ʾ���޵ȴ���ֱ���յ�һ֡���ݻ���ֹͣȡ��
     *  @return �ɹ�������MV_OK�����󣬷��ش�����
     *  @remarks ���øýӿڻ�ȡͼ������֮֡ǰ��Ҫ�ȵ���GrabContinous����ͼ��ɼ����ýӿ�Ϊ����ʽ��ȡ֡���ݣ��ϲ�Ӧ�ó�����Ҫ����֡�ʣ����ƺõ��øýӿڵ�Ƶ�ʡ��ýӿ�֧�����ó�ʱʱ�䣬SDK�ڲ��ȴ�ֱ��������ʱ���أ���������ȡ��ƽ���ԣ��ʺ����ڶ�ƽ����Ҫ��ϸߵĳ��ϡ� \n
             �ýӿ���FreeImageBuffer����ʹ�ã���������ȡ�������ݺ���Ҫ��FreeImageBuffer�ӿڽ�pFrame�ڵ�����ָ��Ȩ�޽����ͷš� \n
             �ýӿڶ���U3V��GIGE�豸����֧�֡� \n
             �ýӿڲ�֧��CameraLink�豸��
     */
    InsResult GetImageBuffer(FRAME_OUT* pstFrame,unsigned int nMsec);
    
    InsResult FreeImageBuffer(FRAME_OUT* pstFrame);

	InsResult GrabStop();

    

    //InsResult FindCLCamera();
    InsResult ConfigureExternalPulseFileToCamera();
    InsResult ConfigureFreeRunFileToCamera();
    InsResult Configure250KFileToCamera();


    //����������ļ�.ini
    InsResult SaveCameraConfigurationToFile(const char* _filename);//����
    InsResult ConfigureFileToCamera(const char* _filename);//����
    InsResult XML_GetGenICamXML(IN OUT unsigned char* pData, IN unsigned int nDataSize, OUT unsigned int* pnDataLen);
    
    //InsResult ConfigureFileToFrameGrabber();//����ѡ��.vlcf�����ļ�
    InsResult ConfigureFileToFrameGrabber(const char* _filename);//����.icf�ļ�  //֮����Ҫ�������뻺����Configure file grabber
    InsResult SaveConfigureFileFromeFrameGrabber(const char* _filename);//����.icf�ļ���ʧ�ܻ�ɾ���ļ�


    InsResult GrabOnce();

  
    InsResult ConfigureBoard();
    InsResult ConfigureCXPBoard(); //CXP���ֻ����Tap��=1��ͼ����ֻ��Ϊ16�ı���
    InsResult ConfigureBoardWith250K();
    InsResult SaveBoardConfigurationToFile();

    InsResult LoadLocalFile();


    InsResult Close();
    InsResult ConfigureFrameGrabber();
    //void set_data_callback(DataPocessCallBack _callback);
    int get_ins_frame_size();//10.07 ���󷵻ش�����InsResult��Ӧ��intֵ
    InsResult set_ins_trigger_source_activated(int index);
    InsResult set_ins_work_mode_activated(int index);
    InsResult set_ins_width(int ins_width_);
    InsResult set_ins_height(int ins_height_);
    int get_ins_width();//11.13 ���󷵻�intֵ���ֵ
    int get_ins_height();//11.13 ���󷵻�intֵ���ֵ
    InsResult set_ins_frame_count(int ins_frame_count_);
    InsResult set_ins_image_type(int index);

    int get_auto_exposure_time_lower();
    InsResult set_ins_exposure(double _val);                                                                                                                             
    double get_ins_exposure();

    InsResult set_ins_digital_gain(double _val);
    double get_ins_digital_gain();
    
    
    
    
    
	InsResult set_pixel_format(unsigned int _pixel_type);
    unsigned int get_pixel_format();
    
    InsResult set_ins_PAC_gain(char* ch_);
    InsResult get_ins_PAC_gain(char* val);//10.07,����(char*)malloc(4);
    InsResult set_ins_shaft_encoder1_min_width_(int ins_shaft_encoder1_min_width_);//�����������ź��˲�����
    int get_ins_shaft_encoder1_min_width_();//���󷵻ش�����Ϊint���ֵ
    static InsResult getinfo();
    /*char* alloc_mem(int _size);*/
  
};
