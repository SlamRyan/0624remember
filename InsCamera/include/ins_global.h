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

//�����������ݴ����
#define SENDBUFFERNUM 4096*2
///Exception Codes
//ͨѶ�쳣
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

//ģ���������
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
	INSCSTATUS_DEVICE_PERMISSION_DENY = 0x000A,//Ȩ�޲��㣬���豸�ѱ�ռ��
	INSCSTATUS_REGISTRY_NOT_FOUND = 0x000B,
	INSCSTATUS_XML_PARSE_ERROR = 0x000C,
	INSCSTATUS_INVALID_ARG = 0x000D,
	INSCSTATUS_INVALID_NAME = 0x000E,//ѡ����ļ�������
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
	INSCSTATUS_FILE_ERR = 0X0034,//�����ļ����豸�ͺŲ�ƥ��


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
	INSSTATUS_INVALID_PAM = 0X00001030,//�Ƿ�����



	//����������
	INSSTATUS_INPUT_ERROR = 0X00002030,
	INSStatus_HANDLE = 0x80000000,  //�������Ч�ľ��
	INSStatus_SUPPORT = 0x80000001,  //��֧�ֵĹ���
	INSStatus_BUFOVER = 0x80000002,  //��������
	INSStatus_CALLORDER = 0x80000003,  //��������˳������
	INSStatus_PARAMETER = 0x80000004,  //����Ĳ���
	INSStatus_RESOURCE = 0x80000006,  //��Դ����ʧ��
	INSStatus_NODATA = 0x80000007,  //������
	INSStatus_PRECONDITION = 0x80000008,  //ǰ���������󣬻����л����ѷ����仯
	INSStatus_VERSION = 0x80000009,  //�汾��ƥ��
	INSStatus_NOENOUGH_BUF = 0x8000000A,  //������ڴ�ռ䲻��
	INSStatus_ABNORMAL_IMAGE = 0x8000000B,  //�쳣ͼ�񣬿����Ƕ�������ͼ������
	INSStatus_LOAD_LIBRARY = 0x8000000C,  //��̬����DLLʧ��
	INSStatus_NOOUTBUF = 0x8000000D,  //û�п�����Ļ���
	INSStatus_ENCRYPT = 0x8000000E, // ���ܴ���
	INSStatus_UNKNOW = 0x800000FF, // δ֪�Ĵ���
	INSStatus_NOT_IMPLEMENTED = 0x80000200,  //������豸֧��
	INSStatus_INVALID_ADDRESS = 0x80000201,  //���ʵ�Ŀ���ַ������
	INSStatus_WRITE_PROTECT = 0x80000202,  //Ŀ���ַ����д
	INSStatus_ACCESS_DENIED = 0x80000203, // �豸�޷���Ȩ��
	INSStatus_BUSY = 0x80000204,  //�豸æ��������Ͽ�
	INSStatus_PACKET = 0x80000205,  //��������ݴ���
	INSStatus_NETER = 0x80000206,  //������ش���
	INSStatus_IP_CONFLICT = 0x80000221,  //�豸IP��ͻ

	//ͨ��
	INSUFFICIENT_COMMUNICATION_PRIVILEGES = 0x00002001,//Ȩ�޲���
};

//��Դ�豸��Ϣ�ṹ��
struct INSCAMERA_API Ins2D5LightDevice
{
public:
	char manufacturer_name[8];					//��˾��"INS"
	char model_name[8];							//��Ŀ����"2D5"
	char device_model[16];						//�豸�ͺ�" INS-CHVS-363-10GH"
	char length[8];								//��Դ�峤�ȣ�"30"
	char SN[16];								//�豸������ţ�" A0100041000"
	char device_vision[16];						//����汾��:"2"
	char ip_addr[16];							//ip��ַ:"192.168.1.11"
	char net_mask[16];							//�������룺"255.255.255.0"
	char gateway[16];							//����"192.168.1.1"
	char mac_addr[18];							//mac��ַ��"0c:12:2b:56:0b:45"
	char bind_camera_SN[64];					//��Դ��Ӧ�����SN��                 //todo              
};

struct  Ins2D5LightDeviceAndLocalIP 
{
	char localIP[16];							//��Դ���ӵı���������IP
	char localGateWay[16];						//��Դ���ӵı�������������
	char localNetMask[16];			 			//��Դ���ӵı�����������������
	Ins2D5LightDevice lightSouce;				//��Դ��Ϣ
};

//����豸��Ϣ�ṹ��
struct INSCAMERA_API InsCameraInfo
{
	const char* divice_full_name_;				//����豸����
	const char* device_class_;					//����豸����
	char SN_[64];								//SN��
};
//�߼���Ϣ�ṹ��
struct INSCAMERA_API OtherDeviceInfo
{
	const char* version_number_;				//fpga�汾��
	const char* product_information_;			//��Ʒ��Ϣ
	const char* device_information_;			//�豸��Ϣ
	const char* fpga_version_generation_date_;  //fpga�汾��������
	const char* microblaze_vertion;			    //MB�汾
};
//�豸��Ϣ�ṹ��
struct INSCAMERA_API Ins2D5Device
{
	InsCameraInfo* stInsCameraInfo;				//����豸��Ϣ�ṹ��ָ��
	Ins2D5LightDevice* stIns2D5LightDeviceInfo; //��Դ��Ϣ�ṹ��ָ��
	OtherDeviceInfo* stOtherInfo;				//�߼���Ϣ�ṹ��ָ��
};

//ͼ�����ݽṹ��
struct INSCAMERA_API InsPMDL_Buffer
{
	unsigned char* pdata;						//ͼ��ָ��
	uint32_t width;								//ͼ����
	uint32_t height;							//ͼ��߶�
	uint32_t step;								//ͼ�񲽳�
	INS_PMDL_PixelFormat imageTxpe;				//ͼ�����ظ�ʽ
	uint32_t frameIndex;						//ͼ�� id
};


//�ں�ͼ�����ݽṹ��
struct INSCAMERA_API InsPMDL_FusionBuffer
{
	unsigned char* pdataNormal;                                            //ͼ��ָ��1
	unsigned char* pdataSpecular;                                          //ͼ��ָ��2
	unsigned char* pdataDiffuse;                                           //ͼ��ָ��3
	unsigned char* pdataGlossratio;                                        //ͼ��ָ��4
	unsigned char* pdataShape;                                             //ͼ��ָ��5
	unsigned char* pdataX1;                                                //ͼ��ָ��6
	unsigned char* pdataX2;                                                //ͼ��ָ��7
	unsigned char* pdataX3;                                                //ͼ��ָ��8
	unsigned char* pdataX4;                                                //ͼ��ָ��9 
	unsigned char* pdataY1;                                                //ͼ��ָ��10
	unsigned char* pdataY2;                                                //ͼ��ָ��11
	unsigned char* pdataY3;                                                //ͼ��ָ��12 
	unsigned char* pdataY4;                                                //ͼ��ָ��13
	unsigned char* pdataIR;                                                //����ͼ��ָ��14
	uint32_t width;                                                                //ͼ����
	uint32_t height;                                                        //ͼ��߶�
	uint32_t step;                                                                //ͼ�񲽳�
	INS_PMDL_PixelFormat imageTxpe;                                //ͼ�����ظ�ʽ
	uint32_t frameIndex;
};

//����ģʽ
enum INSCAMERA_API InsPMDL_LineMode//Max-����ģʽ��Big-��ģʽ��Standard-��׼ģʽ��Small-Сģʽ��Min-��Сģʽ
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
	int  id[SYSTEM_DEVICE_MAXNUM];//���
};

struct INSCAMERA_API InsPMDL_LightInfo
{

	int LightNum;
	Ins2D5LightDeviceAndLocalIP* LightDeviceInfo[SYSTEM_DEVICE_MAXNUM];
};

//�������Դ������Ϣ
struct INSCAMERA_API InsPMDL_SystemInfo
{
	InsPMDL_CameraInfo CamInfo;

	InsPMDL_LightInfo  LightInfo;
};

