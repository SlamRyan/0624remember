#pragma once

#include <iomanip>
#include <chrono>
#include <iostream>

#ifdef INSCAMERA_EXPORTS
#define INSCAMERA_API __declspec(dllexport)
#else
#define INSCAMERA_API __declspec(dllimport)

#endif


#define MODBUS_SLEEP_TIME_10MS 10
#define MODBUS_SLEEP_TIME_MS 1
#define MODBUS_SLEEP_TIME_US 100

#define SYSTEM_DEVICE_MAXNUM 4



#define SEARCH_DEVICE_TIME 126
INSCAMERA_API void PrintTime(std::string x);


#ifdef _DEBUG
#define PRINTTIME(x) { PrintTime(x);}
#else
#define PRINTTIME(x)
#endif

//在线升级数据传输宏
#define SENDBUFFERNUM 4096*2
///Exception Codes
//通讯异常
#define EX_ILLEGAL_FUNCTION 0x01 // Function Code not Supported
#define EX_ILLEGAL_ADDRESS 0x02  // Output Address not exists
#define EX_ILLEGAL_VALUE 0x03    // Output Value not in Range
#define EX_SERVER_FAILURE 0x04   // Slave Deive Fails to process request
#define EX_ACKNOWLEDGE 0x05      // Service Need Long Time to Execute
#define EX_SERVER_BUSY 0x06      // Server Was Unable to Accept MB Request PDU
#define EX_NEGATIVE_ACK 0x07
#define EX_MEM_PARITY_PROB 0x08
#define EX_GATEWAY_PROBLEMP 0x0A // Gateway Path not Available
#define EX_GATEWAY_PROBLEMF 0x0B // Target Device Failed to Response
#define EX_BAD_DATA 0XFF         // Bad Data lenght or Address
#define BAD_CON -1

//模拟增益参数
#define INS_PAC_GAIN_X1		"x1";
#define INS_PAC_GAIN_X2		"x2";
#define INS_PAC_GAIN_X5		"x5";
#define INS_PAC_GAIN_X10	"x10";

/************************************************************************/
/*Data Format
/************************************************************************/

#define IKP_DATA_FORMAT_VAL_8Bit				(8)
#define IKP_DATA_FORMAT_VAL_10Bit				(10)
#define IKP_DATA_FORMAT_VAL_12Bit				(12)
#define IKP_DATA_FORMAT_VAL_14Bit				(14)
#define IKP_DATA_FORMAT_VAL_16Bit				(16)


/************************************************************************/
/*Image Type
/************************************************************************/
enum INSCAMERA_API INS_PMDL_PixelFormat
{
	INS_PMDL_IMAGE_TYPE_VAL_MONOCHROME			 = 0,
	INS_PMDL_IMAGE_TYPE_VAL_RGB                  = 1,
};

enum INSCAMERA_API INS_PMDL_ImageType

{
	Noraml = 1,
	Specular = 2,
	Diffuse = 4,
	Glossratio = 8,
	Shape = 16,
	X1 = 32,
	X2 = 64,
	X3 = 128,
	X4 = 256,
	Y1 = 512,
	Y2 = 1024,
	Y3 = 2048,
	Y4 = 4096,
	IR = 8192,
	AllImageWithOutIR = 8191,
    AllImageIncludeIR = 16383,
};


enum INSCAMERA_API INS_PMDL_TriggerModeSelect
{
	EncodeTrigger = 1,
	SoftTrigger = 2,
	IOTrigger = 3,
};

enum INSCAMERA_API INS_PMDL_BoardNum
{
	OneBoard = 1,
	TwoBoard = 2,
	ThreeBoard = 3,
	FourBoard = 4,
	FiveBoard = 5,
	SixBoard = 6,
};

enum INSCAMERA_API BoardID
{
    EXTSTM32	 =	101,
    INTSTM32	 =	100,
    FIRSTBOARD   =	1,
    SECONDBOARD  =	2,
    THIRDBOARD   =	3,
    FOURTHBOARD  =	4,
    FIFTHBOARD   =	5,
	SIXTHBOARD   =  6,
};

enum class InsResult : uint32_t {
	//names for generic error codes
	//camera
	INSCSTATUS_OK = 0x0000, //true
	INSCSTATUS_INVALID_HANDLE = 0x0001,
	INSCSTATUS_INSCUFFICIENT_RESOURCES = 0x0002,
	INSCSTATUS_BUFFER_TOO_SMALL = 0x0003,
	INSCSTATUS_MISSING_RESOURCE = 0x0004,
	INSCSTATUS_UNINITIALIZE = 0x0005,
	INSCSTATUS_DEVICE_ID_OUTOF_RANGE = 0x0006,
	INSCSTATUS_SERAIL_PORT_NOT_AVAILABLE = 0x0007,
	INSCSTATUS_XML_NOT_FOUND = 0x0008,
	INSCSTATUS_DEVICE_NOT_ACCESSABLE = 0x0009,
	INSCSTATUS_DEVICE_PERMISSION_DENY = 0x000A,//权限不足，或设备已被占用
	INSCSTATUS_REGISTRY_NOT_FOUND = 0x000B,
	INSCSTATUS_XML_PARSE_ERROR = 0x000C,
	INSCSTATUS_INVALID_ARG = 0x000D,
	INSCSTATUS_INVALID_NAME = 0x000E,//选择的文件不存在
	INSCSTATUS_INCOMPATIBLE_FEATURE_TYPE = 0x000F,
	INSCSTATUS_TIME_OUT = 0x0010,
	INSCSTATUS_COMMAND_CRASH = 0x0011,
	INSCSTATUS_COMMAND_PARAM_OUT_OF_RANGE = 0x0012,
	INSCSTATUS_COMMAND_NOT_ALLOW = 0x0013,
	INSCSTATUS_COMMAND_NOT_PRASE = 0x0014,
	INSCSTATUS_COMMAND_PENDING = 0x0015,
	INSCSTATUS_ARG_OUT_OF_RANGE = 0x0016,
	INSCSTATUS_NOT_IMPLEMENT = 0x0017,
	INSCSTATUS_NO_MEMORY = 0x0018,
	INSCSTATUS_INCOMPATIBLE_ARG_TYPE = 0x0019,
	INSCSTATUS_STREAM_IN_PROCESS = 0X001A,
	INSCSTATUS_PRM_READ_ONLY = 0X001B,
	INSCSTATUS_STREAM_IS_OPENED = 0X001C,
	INSCSTATUS_SYSTEM_ERROR = 0X001D,
	INSCSTATUS_INVALID_ADDRESS = 0X001E,
	INSCSTATUS_BAD_ALIGNMENT = 0X001F,
	INSCSTATUS_DEVICE_BUSY = 0X0020,
	INSCSTATUS_DEVICE_IS_REMOVED = 0X0021,
	INSCSTATUS_DEVICE_NOT_FOUND = 0X0022,
	INSCSATTUS_BOARD_IS_OPENED = 0X0023,
	INSCSTATUS_BOARD_NO_OPENED = 0X0024,
	INSCSTATUS_PRM_WRITE_ONLY = 0X0025,
	INSCSTATUS_BOARD_CONNECTION_FAIL = 0X0026,
	INSCSTATUS_RUNTIME_ERROR = 0X0027,
	INSCSTATUS_IO_ERROR = 0X0028,
	INSCSTATUS_BUFFER_OVERFLOW = 0X0029,
	INSCSTATUS_COMMUNICATION_ERROR = 0X0030,
	INSCSTATUS_CXP_CONTROL_CRC_ERROR = 0X0031,
	INSCSTATUS_NO_CAMERA = 0X0032,
	INSCSTATUS_NOT_CAMERALINK_CAMERA_OR_SERIAL_NUMBER_NOT_PROPER = 0X0033,
	INSCSTATUS_NO_FILE = 0X0034,
	INSCSTATUS_FILE_ERR = 0X0034,//配置文件和设备型号不匹配


	//BOARD
	INS_RTN_ERROR = 0X00001000,//4096
	INSSTATUS_SUCCESS = 0X00001001,//4097 //true
	INSSTATUS_BOARDNOTFOUND = 0X00001002,
	INSSTATUS_ALLOCMEMORYFAIL = 0X00001003,
	INSSTATUS_INVALIDPARAMETER = 0X00001004,
	INSSTATUS_OPENBOARDFAIL = 0X00001005,
	INSSTATUS_TIMEOUT = 0X00001006,
	INSSTATUS_WINERROR = 0X00001007,
	INSSTATUS_BOARDNOTOPEN = 0X00001008,
	INSSTATUS_CONFIGFILEPATHINVALID = 0X00001009,
	INSSTATUS_CONFIGPARAMETERINVALID = 0X0000100A,
	INSSTATUS_ZLP = 0X0000100B,
	INSSTATUS_THREADUNSETUP = 0X0000100C,
	INSSTATUS_THREADEXIST = 0X0000100D,
	INSSTATUS_CAMERAUNSUPPORT = 0X0000100E,
	INSSTATUS_XMLFILELOADFAIL = 0X0000100F,
	INSSTATUS_NODENOTEXIST = 0X00001010,
	INSSTATUS_WRITESERIALFAIL = 0X00001011,
	INSSTATUS_COMMANDNOTEXIST = 0X00001012,
	INSSTATUS_COMMANDCOLLISION = 0X00001013,
	INSSTATUS_COMMANDMISSREQUIREMENT = 0X00001014,
	INSSTATUS_COMMANDNOTALLOW = 0X00001015,
	INSSTATUS_COMMANDSYNTAXERROR = 0X00001016,
	INSSTATUS_NODETYPEDISMATCH = 0X00001017,
	INSSTATUS_FEATUREUNSUPPORT = 0X00001018,
	INSSTATUS_COMMANDRESULTNOEXIT = 0X00001019,
	INSSTATUS_CLREGPATHNOTFOUND = 0X0000101A,
	INSSTATUS_CLDLLNOTFOUND = 0X0000101B,
	INSSTATUS_CAMERANOTFOUND = 0X0000101C,
	INSSTATUS_BUFFERTOOSMALL = 0X0000101D,
	INSSTATUS_BAUDRATENOTSUPPORT = 0X0000101E,
	INSSTATUS_CAMERAINUSE = 0X0000101F,
	INSSTATUS_FPGA_ERASEFLASHFAIL = 0X00001020,
	INSSTATUS_FPGA_CHECKFAIL = 0X00001021,
	INSSTATUS_BOARDNOTBINDINGCOM = 0X00001022,
	INSSTATUS_READREGFAIL = 0X00001023,
	INSSTATUS_INVALID_MUTEX = 0X00001024,
	INSSTATUS_MUTEX_LOCKED = 0X00001025,
	INSSTATUS_INVALID_HANDLE = 0X00001026,
	INSSTATUS_SET_INFO_ERROR = 0X00001027,
	INSSTATUS_GRAB_PENDING = 0X00001028,
	INSSTATUS_INSUFFICIENT_RESOURCE = 0X00001029,
	INSSTATUS_GRAB_ABORT = 0X0000102A,
	INSSTATUS_NEED_REBOOT = 0X0000102B,
	INSSTATUS_NEED_RESTART = 0X0000102C,
	INSSTATUS_NOT_IMPLEMENT = 0X0000102D,
	INSSTATUS_NOT_IN_CALLBACK = 0X0000102E,
	INSSTATUS_FILENAME_ERROR = 0X0000102F,
	INSSTATUS_OPEN_FILE_FAILED = 0X00001030,
	INSSTATUS_INVALID_PAM = 0X00001030,//非法参数



	//海康错误码
	INSSTATUS_INPUT_ERROR = 0X00002030,
	INSStatus_HANDLE = 0x80000000,  //错误或无效的句柄
	INSStatus_SUPPORT = 0x80000001,  //不支持的功能
	INSStatus_BUFOVER = 0x80000002,  //缓存已满
	INSStatus_CALLORDER = 0x80000003,  //函数调用顺序有误
	INSStatus_PARAMETER = 0x80000004,  //错误的参数
	INSStatus_RESOURCE = 0x80000006,  //资源申请失败
	INSStatus_NODATA = 0x80000007,  //无数据
	INSStatus_PRECONDITION = 0x80000008,  //前置条件有误，或运行环境已发生变化
	INSStatus_VERSION = 0x80000009,  //版本不匹配
	INSStatus_NOENOUGH_BUF = 0x8000000A,  //传入的内存空间不足
	INSStatus_ABNORMAL_IMAGE = 0x8000000B,  //异常图像，可能是丢包导致图像不完整
	INSStatus_LOAD_LIBRARY = 0x8000000C,  //动态导入DLL失败
	INSStatus_NOOUTBUF = 0x8000000D,  //没有可输出的缓存
	INSStatus_ENCRYPT = 0x8000000E, // 加密错误
	INSStatus_UNKNOW = 0x800000FF, // 未知的错误
	INSStatus_NOT_IMPLEMENTED = 0x80000200,  //命令不被设备支持
	INSStatus_INVALID_ADDRESS = 0x80000201,  //访问的目标地址不存在
	INSStatus_WRITE_PROTECT = 0x80000202,  //目标地址不可写
	INSStatus_ACCESS_DENIED = 0x80000203, // 设备无访问权限
	INSStatus_BUSY = 0x80000204,  //设备忙，或网络断开
	INSStatus_PACKET = 0x80000205,  //网络包数据错误
	INSStatus_NETER = 0x80000206,  //网络相关错误
	INSStatus_IP_CONFLICT = 0x80000221,  //设备IP冲突

	//通信
	INSUFFICIENT_COMMUNICATION_PRIVILEGES = 0x00002001,//权限不足
};

//光源设备信息结构体
struct INSCAMERA_API Ins2D5LightDevice
{
public:
	char manufacturer_name[8];					//公司："INS"
	char model_name[8];							//项目名："2D5"
	char device_model[16];						//设备型号" INS-CHVS-363-10GH"
	char length[8];								//光源板长度："30"
	char SN[16];								//设备条形码号：" A0100041000"
	char device_vision[16];						//软件版本号:"2"
	char ip_addr[16];							//ip地址:"192.168.1.11"
	char net_mask[16];							//子网掩码："255.255.255.0"
	char gateway[16];							//网关"192.168.1.1"
	char mac_addr[18];							//mac地址："0c:12:2b:56:0b:45"
	char bind_camera_SN[64];					//光源对应相机的SN号                 //todo              
};

struct  Ins2D5LightDeviceAndLocalIP 
{
	char localIP[16];							//光源连接的本机网卡的IP
	char localGateWay[16];						//光源连接的本机网卡的网段
	char localNetMask[16];			 			//光源连接的本机网卡的子网掩码
	Ins2D5LightDevice lightSouce;				//光源信息
};

//相机设备信息结构体
struct INSCAMERA_API InsCameraInfo
{
	const char* divice_full_name_;				//相机设备名称
	const char* device_class_;					//相机设备类型
	char SN_[64];								//SN号
};
//高级信息结构体
struct INSCAMERA_API OtherDeviceInfo
{
	const char* version_number_;				//fpga版本号
	const char* product_information_;			//产品信息
	const char* device_information_;			//设备信息
	const char* fpga_version_generation_date_;  //fpga版本生成日期
	const char* microblaze_vertion;			    //MB版本
};
//设备信息结构体
struct INSCAMERA_API Ins2D5Device
{
	InsCameraInfo* stInsCameraInfo;				//相机设备信息结构体指针
	Ins2D5LightDevice* stIns2D5LightDeviceInfo; //光源信息结构体指针
	OtherDeviceInfo* stOtherInfo;				//高级信息结构体指针
};

//图像数据结构体
struct INSCAMERA_API InsPMDL_Buffer
{
	unsigned char* pdata;						//图像指针
	uint32_t width;								//图像宽度
	uint32_t height;							//图像高度
	uint32_t step;								//图像步长
	INS_PMDL_PixelFormat imageTxpe;				//图像像素格式
	uint32_t frameIndex;						//图像 id
};


//融合图像数据结构体
struct INSCAMERA_API InsPMDL_FusionBuffer
{
	unsigned char* pdataNormal;                                            //图像指针1
	unsigned char* pdataSpecular;                                          //图像指针2
	unsigned char* pdataDiffuse;                                           //图像指针3
	unsigned char* pdataGlossratio;                                        //图像指针4
	unsigned char* pdataShape;                                             //图像指针5
	unsigned char* pdataX1;                                                //图像指针6
	unsigned char* pdataX2;                                                //图像指针7
	unsigned char* pdataX3;                                                //图像指针8
	unsigned char* pdataX4;                                                //图像指针9 
	unsigned char* pdataY1;                                                //图像指针10
	unsigned char* pdataY2;                                                //图像指针11
	unsigned char* pdataY3;                                                //图像指针12 
	unsigned char* pdataY4;                                                //图像指针13
	unsigned char* pdataIR;                                                //红外图像指针14
	uint32_t width;                                                                //图像宽度
	uint32_t height;                                                        //图像高度
	uint32_t step;                                                                //图像步长
	INS_PMDL_PixelFormat imageTxpe;                                //图像像素格式
	uint32_t frameIndex;
};

//条纹模式
enum INSCAMERA_API InsPMDL_LineMode//Max-极大模式，Big-大模式，Standard-标准模式，Small-小模式，Min-极小模式
{
	Max = 0,
	Big = 1,
	Standard = 2,
	Small  = 3,
	Min = 4,
};

enum INSCAMERA_API InsPMDL_Switch
{
	Off = 0,
	On = 1,
};


struct INSCAMERA_API InsPMDL_CameraInfo
{
	int  CamNum;
	int  id[SYSTEM_DEVICE_MAXNUM];//板号
};

struct INSCAMERA_API InsPMDL_LightInfo
{

	int LightNum;
	Ins2D5LightDeviceAndLocalIP* LightDeviceInfo[SYSTEM_DEVICE_MAXNUM];
};

//相机、光源个数信息
struct INSCAMERA_API InsPMDL_SystemInfo
{
	InsPMDL_CameraInfo CamInfo;

	InsPMDL_LightInfo  LightInfo;
};

