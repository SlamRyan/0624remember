#ifndef INSREGISTER_H
#define INSREGISTER_H

#include "base_camera.h"
#include "../include/inscp_udp.h"
#include "../include/Ins_Utility_proc.h"
#include "../include/modbus_udp.h"
#include "../../third_party/IkapIncludes/IKapBoard.h"
#include "../../third_party/IkapIncludes/IKapC.h"
#include "../../third_party/IkapIncludes/IKapCType.h"
#include "../../third_party/IkapIncludes/IKapCDef.h"
#include "../../third_party/IkapIncludes/IKapCStat.h"
#include "ins_global.h"

extern "C" INSCAMERA_API void HelloCamera();

//图像缓冲区申请的帧数。
#define BOARD_FRMAE_COUNT 1//缓冲区  缓存

//触发源
#define TRIGGER_SOURCE_VAL_GENERAL_INPUT1 0
#define TRIGGER_SOURCE_VAL_GENERAL_INPUT2 1
#define TRIGGER_SOURCE_VAL_SHAFT_ENCODER1 2
#define TRIGGER_SOURCE_VAL_BOARD_SYNC1 3
#define TRIGGER_SOURCE_VAL_BOARD_SYNC2 4
#define TRIGGER_SOURCE_VAL_INNER_TRIGGER 5
#define TRIGGER_SOURCE_VAL_SOFTWARE 6

//工作模式
#define FREE_RUN 0
#define HARDWARE_TRIGGER 1

//图像类型
#define INS_IMAGE_TYPE_VAL_MONOCHROME			0
#define INS_IMAGE_TYPE_VAL_COLORFUL				1
#define INS_IMAGE_TYPE_VAL_RGB                  1
#define INS_IMAGE_TYPE_VAL_RGBC                 2
#define INS_IMAGE_TYPE_VAL_BGR                  3
#define INS_IMAGE_TYPE_VAL_BGRC                 4
#define INS_IMAGE_TYPE_VAL_YUV422               5
#define LOG_FILE "camera_log.txt"

//// 保存图像的文件名。
////
//// File name of image.
//const char* g_saveFileName = "C:\\CImage.tif";

#include <functional>

//GlobalVar
//using DataPocessCallBack = std::function<void(unsigned char*)>;

typedef void (CALLBACK* DataPocessCallBack)(InsPMDL_Buffer*);
typedef void (CALLBACK* OffLineCallBack)(unsigned char*);

using namespace std;


struct INSCameraInfo
{
    const char* divice_full_name_;
    const char* device_class_;
};


void log_message(const std::string& message);//0527


InsResult FindCamera(int& _num);

InsResult GetCameraDeviceInfo(int _num, InsCameraInfo& _insCameraInfo);

class INSCAMERA_API InsCameraIkap: public InsCamera
{
public:
    static int ins_width_;
    static int ins_height_;
    static int ins_type_;
    static double ins_exposure_;
    static double ins_digital_gain_;
    static int ins_frame_count_;
    static char* ins_PAC_gain_;

    static int ins_trigger_mode_;//IKP_RCV_MORE_DATA_IN_TRIGGER_MODE
    static double ins_line_period_time_;
    static int ins_st_width_;//IKP_SOFTWARE_TRIGGER_WIDTH
    static int ins_st_period_;//IKP_SOFTWARE_TRIGGER_PERIOD
    static int ins_st_count_;//IKP_SOFTWARE_TRIGGER_COUNT
    static int ins_st_delay_;//IKP_SOFTWARE_TRIGGER_DELAY
    static int ins_st_polarity_;// IKP_SFOTWARE_TRIGGER_POLARITY
    static int ins_st_sync_mode_;//IKP_SOFTWARE_TRIGGER_SYNC_MODE
    static int ins_io_trigger_mode_;
    static int ins_io_polarity_;
    static int ins_io_min_width_;
    static int ins_strobe_source_;
    static int ins_strobe_method_;
    static int ins_strobe_polarity_;
    static int ins_strobe_delay_;
    static int ins_strobe_width_;
    static int ins_et_channel_;// IKP_SHAFT_ENCODER1_CHANNEL
    static int ins_et_multi_factor_;//IKP_SHAFT_ENCODER1_MULTIPlY_FACTOR
    static int ins_et_division_factor_;
    static int ins_et_min_width_;//IKP_SHAFT_ENCODER1_MIN_WIDTH
    static int ins_et_valid_direction_;// IKP_SHAFT_ENCODER1_VALID_DIRECTION
    static int ins_et_reverse_comp_;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION
    static int ins_et_quad_frequency_source_;//IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE
    static int ins_et_reverse_compensation_limit_;// IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_LIMIT
    static int ins_et_clock_duty_compensation_type_;//IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_TYPE
    static int ins_et_clock_duty_compensation_width_;// IKP_SHAFT_ENCODER1_CLOCK_DUTY_COMPENSATION_WIDTH
    static int ins_shaft_encoder1_min_width_;
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

    std::string CamFullName;
    uint32_t num_devices_ = 0;//相机数量
    static int mi_camera;//打开的相机是第i台 //0322
    
    // 相机设备句柄。
    ITKDEVICE ins_hCamera;

    // 采集卡设备句柄。
    HANDLE ins_hBoard;


    INSCP ins_cp;
    Ins_Modbus_udp* mpCommunication = nullptr;


    void enumAllFeatures();

    InsCameraIkap();
    ~InsCameraIkap() {};
    InsResult ConnectCamera()override;
    InsResult ConnectIndexCamera(int _index);
    InsResult PrintCamera();
    InsResult FindCamera()override;
    //InsResult FindCLCamera();
    InsResult ConfigureExternalPulseFileToCamera();
    InsResult ConfigureFreeRunFileToCamera();
    InsResult Configure250KFileToCamera();


    //保存和配置文件
    //弃用
    InsResult SaveCameraConfigurationToFile(char* filename_);//11.14改动，保存.ccf
    //InsResult ConfigureFileToCamera(char* filename_);//加载.ccf


    //InsResult ConfigureFileToFrameGrabber();//弹窗选择.vlcf配置文件
    InsResult ConfigureFileToFrameGrabber(const char* filename_);//加载.icf文件  //之后需要重新申请缓冲区Configure file grabber
    InsResult SaveConfigureFileFromeFrameGrabber(const char* filename_, int CameraNum, int LightNum, InsPMDL_SystemInfo SystemInfo);//保存.icf文件，失败会删除文件
    InsResult UserSetSave();
    InsResult set_user_set_selector(const char* str_);//21.28
    InsResult get_user_set_selector(char* val);
    InsResult GrabOnce();
    InsResult GrabContinous();//需要停止采集,_index 0-自由取图，1-编码器，2-软触发
    //InsResult CLGrabContinous(int _index); 0201 Cxp cl 合并到Grabcontinue

    //InsResult CXPGrabContinous();//需要停止采集
   

    InsResult GrabStop();
    InsResult ConfigureBoard();
    InsResult ConfigureCXPBoard(); //CXP相机只兼容Tap数=1，图像宽度只能为16的倍数
    InsResult ConfigureBoardWith250K();
    InsResult SaveBoardConfigurationToFile();

	InsResult LoadLocalFile();//加载这个路径的文件"./200K.vlcf"

    InsResult Close();
    InsResult ConfigureFrameGrabber();
    void set_data_callback(DataPocessCallBack _callback);
    void unregister_data_callback();//注销回调

    void set_device_remove_callback(OffLineCallBack _callback);//注册掉线回调
    void unregister_device_remove_callback();//注销掉线回调

    int get_ins_frame_size();//10.07 错误返回错误码InsResult对应的int值
    InsResult set_ins_trigger_source_activated(int _index);
    InsResult set_ins_work_mode_activated(int _index);
    InsResult set_ins_width(int ins_width_);
    InsResult set_ins_height(int ins_height_);
    int get_ins_width();//11.13 错误返回int值最大值
    int get_ins_height();//11.13 错误返回int值最大值
    InsResult set_ins_frame_count(int ins_frame_count_);
    InsResult set_ins_image_type(int index);
    InsResult set_line_trigger();//设置行触发模式
    InsResult set_frame_trigger();
    InsResult set_ins_exposure(double val);//max=10000000.000000000 min=2.0000000000000000	inc=0.1
    double get_ins_exposure();//10.07
    //CL专用
    InsResult set_ins_line_period_time(double val);//行频，max=10000000.000000000 min=5.0000000000000000	inc=0.1 //返回值为18表示参数超出范围
    double get_ins_line_period_time();//10.07，错误时返回double最大值
    InsResult set_ins_digital_gain(double val);//max=8 min=0.01	inc=0.01
    double get_ins_digital_gain();//10.07，错误时返回double最大值
    InsResult set_ins_PAC_gain(char* ch_);
    InsResult get_ins_PAC_gain(char* val);//10.07,传入(char*)malloc(4);

    InsResult set_ins_analog_gain(char* ch_);//12.11CXP用
    InsResult get_ins_analog_gain(char* val);
    InsResult set_ins_shaft_encoder1_min_width_(int ins_shaft_encoder1_min_width_);//编码器输入信号滤波功能
    int get_ins_shaft_encoder1_min_width_();//错误返回错误码为int最大值
    //static InsResult getinfo();

    string get_cam_full_name();//0506

    int get_ins_frame_id_();
    /*char* alloc_mem(int _size);*/

    //设备初始化
    int fpga_device_initialization(int _board_num);

    int freerunReset(int a);

   
    

    //----------老版本读写单个寄存器------------
    int read_register(BoardID _id, uint16_t _address, uint32_t& _buffer);
    int write_register(BoardID _id, uint16_t _address, uint32_t _buffer);


    void UnRegisterCallback();

    static void CALLBACK OnFrameReady(void* pContext);
};


//InsResult INSCAMERA_API CSharpFindCamera(INSCameraInfo* insdeviceinfo);

#endif 

