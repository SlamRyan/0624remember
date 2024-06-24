#pragma once
#include "base_camera.h"

#define MV_IP_CFG_STATIC  0x05000000  //固定IP地址模式
#define MV_IP_CFG_DHCP  0x06000000  //DHCP自动获取IP模式
#define MV_IP_CFG_LLA  0x04000000  //LLA(Link - local address)，链路本地地址


/// \~chinese GigE设备信息              \~english GigE device info
struct GIGE_DEVICE_INFO
{
    unsigned int        nIpCfgOption;                               ///< [OUT] \~chinese IP配置选项             \~english IP Configuration Options
    unsigned int        nIpCfgCurrent;                              ///< [OUT] \~chinese 当前IP配置             \~english IP Configuration:bit31-static bit30-dhcp bit29-lla
    unsigned int        nCurrentIp;                                 ///< [OUT] \~chinese 当前IP地址             \~english Current Ip
    unsigned int        nCurrentSubNetMask;                         ///< [OUT] \~chinese 当前子网掩码           \~english Curtent Subnet Mask
    unsigned int        nDefultGateWay;                             ///< [OUT] \~chinese 当前网关               \~english Current Gateway
    unsigned char       chManufacturerName[32];                     ///< [OUT] \~chinese 制造商名称             \~english Manufacturer Name
    unsigned char       chModelName[32];                            ///< [OUT] \~chinese 型号名称               \~english Model Name
    unsigned char       chDeviceVersion[32];                        ///< [OUT] \~chinese 设备版本               \~english Device Version 
    unsigned char       chManufacturerSpecificInfo[48];             ///< [OUT] \~chinese 制造商的具体信息       \~english Manufacturer Specific Information
    unsigned char       chSerialNumber[16];                         ///< [OUT] \~chinese 序列号                 \~english Serial Number
    unsigned char       chUserDefinedName[16];                      ///< [OUT] \~chinese 用户自定义名称         \~english User Defined Name 
    unsigned int        nNetExport;                                 ///< [OUT] \~chinese 网口IP地址             \~english NetWork IP Address

    unsigned int        nReserved[4];                               ///<       \~chinese 预留                   \~english Reserved

};

typedef void (* DataPocessCallBack)(void* handle,unsigned char* pData);

class INSCAMERA_API MvsCamera:public InsCamera
{
public:
    MvsCamera() {};
    ~MvsCamera();

	void* mhandle = NULL;
    std::vector<GIGE_DEVICE_INFO> mMvsCameraInfo;//设备信息列表
    static uint32_t numCameras_;
    static bool cam_update_;//第一次刷新参数
    static uint32_t frame_id_;
    //static bool m_save_Image_;
    //static int lines_;
    static int ins_frame_size_;
    std::vector<std::string> full_name_;//相机名 在调用findcamera后获得值
    std::vector<std::string> device_class_;//相机连接类型
   /* char fullName_[128];
    char deviceClass_[128];*/
    uint32_t num_devices_ = 0;//相机数量
    int mins_frame_size_ = 0;
	int mins_height_ = 0;
	int mins_width_ = 0;
	double ins_exposure_ = 0;
	double ins_digital_gain_ = 0;

    InsResult ConnectCamera(int _index);//输入需要打开的相机序号
    InsResult FindCamera();



    /********************************************************************//**
 *  @~chinese
 *  @brief  强制IP
 *  @param  handle                      [IN]            设备句柄
 *  @param  nIP                         [IN]            设置的IP
 *  @param  nSubNetMask                 [IN]            子网掩码
 *  @param  nDefaultGateWay             [IN]            默认网关
 *  @return 成功,返回MV_OK,失败,返回错误码
 *  @remarks 强制设置设备网络参数（包括IP、子网掩码、默认网关），强制设置之后将需要重新创建设备句柄，仅GigEVision设备支持。\n
             如果设备为DHCP的状态，调用该接口强制设置设备网络参数之后设备将会重启。

************************************************************************/
    InsResult ForceIpEx( unsigned int _nIP, unsigned int _nSubNetMask, unsigned int _nDefaultGateWay);

    /**
     *  @brief      配置IP方式
     *  @param[in]  nType                                   IP类型，见MV_IP_CFG_x
     *  @return     成功,返回MV_OK,失败,返回错误码
     *  @remarks 发送命令设置设备的IP方式，如DHCP、LLA等，仅GigEVision设备支持。
     */
    InsResult SetIpConfig( unsigned int nType);
   /**
     *  @brief  Thisis a brief description.
	 *  @param[in]  _index == 0_设置触发模式为off
	                _index == 1_设置软触发模式
	                _index == 2_设置线路0触发模式
	                _index == 3_设置线路2触发模式
	                _index == 4)_设置计数器0触发模式
     *  @param[out] outArgName output argument description.
     *  @Return:    InsResult:description of the return value
     */

    InsResult set_trigger_mode(int _index);

	InsResult send_soft_trigger_sig();
    /**
      *  @brief  设置SDK内部图像缓存节点个数，大于等于1，在抓图前调用
      *  @param[in]  内部缓冲区节点数，不设置默认为1
      *  @param[out] outArgName output argument description.
      *  @Return:    InsResult:description of the return value
      */
    InsResult SetImageNodeNum(unsigned int nNum);

    /**
      *  @brief  注册抓图回调
      *  @param[in]  inArgName input argument description.type name:purpose(Range)
      *  @param[out] outArgName output argument description.
      *  @Return:    :description of the return value
      */
	InsResult RegisterImageCallBack(DataPocessCallBack _callback);
	InsResult AntiRegisterImageCallBack();
	InsResult GrabContinous();
    /**
    *  @brief  使用内部缓存获取一帧图片
    *  @param[in]  
    *  @param[out] pstFrame                           图像数据和图像信息
    *  @param[in]  nMsec                              等待超时时间，输入0xFFFFFFFF时表示无限等待，直到收到一帧数据或者停止取流
     *  @return 成功，返回MV_OK；错误，返回错误码
     *  @remarks 调用该接口获取图像数据帧之前需要先调用GrabContinous启动图像采集。该接口为主动式获取帧数据，上层应用程序需要根据帧率，控制好调用该接口的频率。该接口支持设置超时时间，SDK内部等待直到有数据时返回，可以增加取流平稳性，适合用于对平稳性要求较高的场合。 \n
             该接口与FreeImageBuffer配套使用，当处理完取到的数据后，需要用FreeImageBuffer接口将pFrame内的数据指针权限进行释放。 \n
             该接口对于U3V、GIGE设备均可支持。 \n
             该接口不支持CameraLink设备。
     */
    InsResult GetImageBuffer(FRAME_OUT* pstFrame,unsigned int nMsec);
    
    InsResult FreeImageBuffer(FRAME_OUT* pstFrame);

	InsResult GrabStop();

    

    //InsResult FindCLCamera();
    InsResult ConfigureExternalPulseFileToCamera();
    InsResult ConfigureFreeRunFileToCamera();
    InsResult Configure250KFileToCamera();


    //保存和配置文件.ini
    InsResult SaveCameraConfigurationToFile(const char* _filename);//保存
    InsResult ConfigureFileToCamera(const char* _filename);//加载
    InsResult XML_GetGenICamXML(IN OUT unsigned char* pData, IN unsigned int nDataSize, OUT unsigned int* pnDataLen);
    
    //InsResult ConfigureFileToFrameGrabber();//弹窗选择.vlcf配置文件
    InsResult ConfigureFileToFrameGrabber(const char* _filename);//加载.icf文件  //之后需要重新申请缓冲区Configure file grabber
    InsResult SaveConfigureFileFromeFrameGrabber(const char* _filename);//保存.icf文件，失败会删除文件


    InsResult GrabOnce();

  
    InsResult ConfigureBoard();
    InsResult ConfigureCXPBoard(); //CXP相机只兼容Tap数=1，图像宽度只能为16的倍数
    InsResult ConfigureBoardWith250K();
    InsResult SaveBoardConfigurationToFile();

    InsResult LoadLocalFile();


    InsResult Close();
    InsResult ConfigureFrameGrabber();
    //void set_data_callback(DataPocessCallBack _callback);
    int get_ins_frame_size();//10.07 错误返回错误码InsResult对应的int值
    InsResult set_ins_trigger_source_activated(int index);
    InsResult set_ins_work_mode_activated(int index);
    InsResult set_ins_width(int ins_width_);
    InsResult set_ins_height(int ins_height_);
    int get_ins_width();//11.13 错误返回int值最大值
    int get_ins_height();//11.13 错误返回int值最大值
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
    InsResult get_ins_PAC_gain(char* val);//10.07,传入(char*)malloc(4);
    InsResult set_ins_shaft_encoder1_min_width_(int ins_shaft_encoder1_min_width_);//编码器输入信号滤波功能
    int get_ins_shaft_encoder1_min_width_();//错误返回错误码为int最大值
    static InsResult getinfo();
    /*char* alloc_mem(int _size);*/
  
};
