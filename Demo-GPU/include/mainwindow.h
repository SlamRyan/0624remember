//void on_pushButton_9_clicked();
//void on_comboBox_6_currentIndexChanged(int index);
//void on_pushButton_4_clicked();
//void on_FenPinSpinBox_2_valueChanged(int arg1);
//void on_LightMode_3_currentIndexChanged(int index);
//void on_checkBox_SP2_3_stateChanged(int arg1);
//void on_LightMode3_currentIndexChanged(int index);
//void on_lineEdit_12_returnPressed();
//void on_lineEdit_11_returnPressed();
//void on_lineEdit_8_returnPressed();
//void on_lineEdit_7_returnPressed();
//void on_insInterface_4_returnPressed();
//void on_lineEdit_9_returnPressed();
//void on_pushButton_3_clicked();
//void on_pushButton_6_clicked();
//void on_pushButton_5_clicked();
//void on_checkBox_stateChanged(int arg1);
//void on_ChuFaCombox_currentIndexChanged(int index);
//void on_insWorkMode_currentIndexChanged(int index);//insWorkMode
//void on_insImageType_currentIndexChanged(int index);//insImageType
//void on_insWorkMode_activated(int index);//insWorkMode
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ins_camera_c.h"//0103
#include"Ins_Camera_proc.h"

#include <QMainWindow>
#include <QModbusClient>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QTimer>
#include <modbuscommit.h>
#include "InsRegister.h"
#include "ins_widget.h"
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QButtonGroup>

//#include"Ins_Utility_proc.h"//这里不能加
//#include"ins_image_processing.h"
//#include"ins_light_proc.h"
#include"ins_devicechoose.h"

#include"ins_DeviceConnect.h" //333


#include <cstdint>
#include <QCheckBox>
#include "ins_imagevalue.h"
//#include "modbus_udp.h"
//#include "Ins_Utility_proc.h"
#include"ins_encodeverify.h"
#include"ins_directionchoose.h"

#include"MyGraphicsView.h"//0428


#include "updatethread.h"
#include "baseAddr.h"
#include "string"

#include "ImageProcessor.h"

#include"ins_image_processing_tool.h"






using namespace std;



struct InsImage
{
    int width;
    int height;
    int stride;
    unsigned char* data;
};


struct Data_25D//0815
{
    uint8_t * data;
    uint32_t length;
    uint32_t height;

    // 构造函数
    Data_25D(): data(nullptr), length(0), height(0)
    {

    }

    // 析构函数
    ~Data_25D()
    {
        delete[] data;
    }
};

enum ViewMode
{
    SingleView,
    DoubleView,
    QuadView,
    AllView
};

//ViewMode currentViewMode = SingleView;



/* @brief：判断 IKapBoard 函数是否成功调用。
 * @param[in] errc：函数返回值。
 */
#define CHECK_IKAPBOARD(errc) if (errc != 1) printIKapBoardErrorAndExit()
/* @brief：判断 IKapC 函数是否成功调用。
 * @param[in] errc：函数返回值。
 */
#define CHECK_IKAPC(errc) if (errc != ITKSTATUS_OK) printIKapCErrorAndExit(errc)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool AntiAliasing = false;
    bool DisplayOriImage = false;
    bool DisplayOriImageCheck = true;
    bool isPassed = false;
    bool StopProcess;
    bool WindowsJudge = true;

    ImageProcessor* imageProcessor;

    QThread* imageProcessorThread;

    bool SecJug = false;

    int WindowCount;

    int CamHeightClear;

    int CamWidthClear;

    void* InsSystemHandle;//0520

    double MyLine; //0415

    double FreeLine;

    int CamType;

    int transbool = 0;

    ins_DirectionChoose* direction;//0314

    double dLineFre; //0319
    double dExp;//0319

    double TriExposure;//0319


    ModbusCommit * m_modbusClient;
    QTimer *timerModbusRead;
    bool m_connect = false;//通信状态
    //ITKDEVICE g_hCamera = NULL;// 相机设备句柄
    HANDLE g_hBoard = INVALID_HANDLE_VALUE;// 采集卡设备句柄
    bool m_camState = false;//相机状态

    Ins_Widget *m_Image;
    Ins_Widget *m_Image2;
    //Ins_Widget *widgetForView1;//0808多窗口
    QGraphicsScene *qgraphicsScene;
    //QGraphicsScene *qgraphicsScene2;//0816

    QGraphicsScene* qgraphicsScenes[12];//多窗口显示
    Ins_Widget* m_Images[12];


    std::vector<cv::Mat> ins_buffer;
    cv::Mat ins_ResultBuffer;
    std::vector<cv::Mat> ins_ShowBuffer;

    QList<QCheckBox*> selectedCheckBoxes;//0816
    QQueue<QCheckBox*> checkedCheckBoxes;

    double Coordinx1;
    double Coordiny1;
    double gray1;

    int PixWindow = 1;//1115

    double myexposure;

    bool m_RefreshX1;//0828
    QTimer testTimer;//0828

    ins_imagevalue *mainImageValue;//0904

    double normalGain =1;
    double specualrGain =1 ;
    double diffuseGain =1 ;
    double shapeGain =1 ;

    //InsCameraIkap camcam; //1214修改

    void* mpCamera = NULL;

    void* mpLight = NULL;

    //InsCamera camcam;

    double heightself;

    double widthself;

    QGraphicsTextItem* m_TextItems[12];

    //INS_REG_OPT op1;//1008

    int testpose = 1;

    double resultPosex = 10000;

    double resultPosey;

    double resultPosez;

    ins_encodeVerify *insencode;

    double encodeX;

    double encodeY;

    QString EncodeRead1;

    QString EncodeRead2;

    QString EncodeRead3;

    QTimer *timer9;

    QTimer *timer10;

    QTimer *timer11;

    bool FreeCollect;

    double LightPosePirch;

    double LightPoseRoll;

    double LightPoseYaw;

    double CamPosePirch;

    double CamPoseRoll;

    double CamPoseYaw;

    //======================

    short LightPose2Pirch;

    short LightPose2Roll;

    short LightPose2Yaw;

    short CamPose2Pirch;

    short CamPose2Roll;

    short CamPose2Yaw;

    QString systemModel;

    QString LightSignStr;

    int LightSignInt ;


    uint32_t MOSACKZ;
    uint32_t DDSACKZ;
    uint32_t PicNumZ;
    uint32_t SWGAPZ;
    uint32_t TriigerGapZ;
    uint32_t SWLoopNumZ;
    uint32_t CamTriNumZ;

    uint32_t Zeroz =0;
    uint32_t Onez = 1;//IO模式 1206
    uint32_t Twoz = 2;
    uint32_t Threez = 3;
    uint32_t Fourz =4;
    uint32_t Fivez = 5;
    uint32_t Sixz = 6;
    uint32_t Sevenz =7;

    //=================1220

    int PicNumCount;
    //===================1222在线升级
    UpDateThread *m_upgrad;
    QTimer *upTimer;

    int UpdateTenLength; //0422


    friend class ins_devicechoose;

    //======0307

    struct MainDeviceHandles
    {
        void* lightHandle;
        void* cameraHandle;
        int DeviceCamInfo;
        int DeviceLigInfo;
    };

    MainDeviceHandles deviceHandles;


    Ins2D5LightDevice mLightInfo;


signals:

    void sig_updateBuffer(unsigned char *pbuffer);

    //void sig_InitWindow(InsCam cam);

    void imageProcessedFinish();//0828

    void startImageProcessing(unsigned char* pbuffer, int cameraWidth, int cameraHeight, std::string appDirCv, bool freeCollect, int picNumCount);





public slots:

    void onCheckBox_Ori_2_changed();

    void onCheckBox_Ori_changed();

    void onPushButton_11_clicked();

    void onTabIndexChange(int index);

    void on_insName_4_textChanged();
    void process_buffer(unsigned char* pbuffer);//0529

    void lineEdit_8returnPress();

    void ReturnToCenter();


    int checkCheckedCheckBoxes(const QQueue<QCheckBox*>& checkedCheckBoxes);
    //---------------0407 setimagevalue

    void SetNormalGain();
    void SetSpecularGain();
    void SetDiffuseGain();
    void SetShapeGain();

    void DirectionChange(int index);


    void on_DirectionPoseBtn_clicked();//位姿选择

    //0326
    void LanChange();

    void LanChange2();

    //0321

    void ABcheckboxConverse();

    //=========0307 一拖多

    void DeviceInfoDisplay(int CameraInfo, int LightInfo);

    void handleDeviceSelection(void* lightHandle, void* cameraHandle,void* systemHandle);//0521
 
    void slot_sendPercent();//1222在线升级

    InsUpgrad ReadUpgradInfo();//1222在线升级

    void WriteUpgradInfo(InsUpgrad info);//1222在线升级

    uint32_t ArrayToVariable32(uint8_t* array,uint8_t length);//1222在线升级

    int FormatStrData(std::string * aStr, uint32_t aStrLen);//1222在线升级

    void on_comboBox_7_currentIndexChanged(int index);//0422在线升级

    QString generateSystemModel(QString cameraModel, QString lightSourceModel);

    void ReadPose2();

    void ABconverse2();

    void ABencodePose();

    void ABconverse();

    void ReadPose();
    void writeRegisters();

    void ReadCamTem();

    void ReadBoardTem();

    void ReadFpgaTem();//1010

    void DevIp_clicked();//0918

    void updateImages();//0905

    void simulateUpdateBuffer();//0905ceshitest ks


    void updateValues(double normal, double specular, double diffuse, double shape);


    void ImageValue_clicked();//0904

    void PoseBtn2_clicked();

    //void writeOModbusRegister(uint16_t address);

    void showXY(double x ,double y,int grayvalue);
    void showXY2(double x, double y,int grayvalue);
    void showXY3(double x, double y,int grayvalue);

    void PoseBtn_clicked();//0817


    void RegionBtn_clicked();//0818

    Data_25D copyImageData(uint8_t* CamImage,uint32_t l_image_show_width,uint32_t l_image_show_height);

    void on_FreLine_returnPressed();
    //void on_FreLine_Changed();

    void on_TriggerFre_currentIndexChanged(int index);
    //void on_comboBox_currentIndexChanged(const QString &selectedMode);
    void on_LightMode_currentIndexChanged(int index);
    //void on_applyButton_clicked();

    void slot_btnOffsetMethod();//显示多画面

    void on_LightBtn();

    int countCheckedCheckBoxes();//0815判断选择了几个checkbox

    void onCheckBoxChanged(int state);
    void showImage(const QString& filePath,const QString &text, int viewIndex);
    //void showImage2(const QString& filePath);//0816
    void onComboBoxChanged(int index);//update checkbox
    void clearImage(); 
    void clearAllGraphicsViews();
    void processAndDisplayImage();


    void slot_Comm();
    void slot_DeviceInfo();
    void slot_LightEdit();
    void slot_MainPage();
    //void slot_InitWindow(InsCam cam);
    void InitWindow();
    //about ikap
   // void printIKapBoardErrorAndExit();

    /* @brief：打印 IKapC 错误信息后退出程序*/
    //void printIKapCErrorAndExit(ITKSTATUS errc);

    /* @brief：用户输入回车键后退出程序*/
    //void pressEnterToExit(void);

    /* @brief：选择用户配置文件*/
    //bool GetOption(char *lpConfigFileName);

    /* @brief：配置相机设备*/
    //void ConfigureCamera(ITKDEVICE *hCamera, HANDLE *hBoard);

    /* @brief：配置采集卡设备*/
    //void ConfigureFrameGrabber(ITKDEVICE hCamera, HANDLE hBoard);

    /* @brief：设置行触发参数*/
    //void SetLineTrigger(ITKDEVICE hCamera, HANDLE hBoard);

    /* @brief：设置帧触发参数*/
    //void SetFrameTrigger(ITKDEVICE hCamera, HANDLE hBoard);

    /* @brief：清除回调函数*/
    //void UnRegisterCallback();
    static void update_buffer(InsPMDL_Buffer* pbuffer);//wdj
    void slot_updateBuffer(unsigned char *pbuffer);
    //static void new_updateBuffer(unsigned char *pbuffer);
    void addToStateMessage(QString str, bool Error);
    void modbus_state_change_slot(QModbusDevice::State state);
    void modbus_error_occurred_slot(QModbusDevice::Error state);
    void modbus_read_over_slot(QModbusDataUnit::RegisterType type,bool isErr,QVector<quint16> readData);
    void modbusPeriodRead();
    void readReady();

    void slot_infoUpdate();
    //cv::Mat extractImg(cv::Mat src,cv::Mat &dst,int index);
private slots:


    //void on_ModbusClientBtn_clicked();

    void on_StateSaveBtn_clicked();

    void on_StateClearBtn_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void on_connectBtn_clicked();

    void on_startAcqBtn_clicked();

    void on_stopAcqBtn_clicked();

    void on_disConnectBtn_clicked();

    //void on_insWorkMode_activated(int index);//insWorkMode

    //void on_insTriggerSource_activated(int index);//insTriggerSource

    //void on_setFrameCountBtn_clicked();//setFrameCountBtn

    //void on_insImageType_currentIndexChanged(int index);//insImageType

    void on_checkBox_clicked(bool checked);

    //void on_pushButton_3_clicked();

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_ChuFaCombox_currentIndexChanged(int index);

    void on_BeiPinSpinBox_valueChanged(int arg1);

    void on_FenPinSpinBox_valueChanged(int arg1);

    void on_ModbusClientBtn_10_clicked();

    void on_ClearBtn_2_clicked();

    void on_EnCodeTriComBox_currentIndexChanged(int index);

    void on_LightMode_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_ModbusClientBtn_11_clicked();

    void on_ClearBtn_3_clicked();

    void on_insInterface_5_returnPressed();

    void on_insInterface_8_returnPressed();


    void on_insInterface_6_returnPressed();

    void on_comboBox_4_currentIndexChanged(int index);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();


    void on_lineEdit_9_returnPressed();

    void on_lineEdit_10_returnPressed();



    //void on_UpdateIpLine_returnPressed();//1027 UpdateIP

    void on_lineEdit_7_returnPressed();



    void on_insName_10_textChanged(const QString &arg1);

    void on_ExNumUI_returnPressed();

    void on_ChuFaCombox_2_currentIndexChanged(int index);


    void on_ExDelayUI_returnPressed();

    void on_ExWidthUI_returnPressed();

    void on_CamTriUI_returnPressed();

    void on_lineEdit_8_returnPressed();

    void on_lineEdit_11_returnPressed();

    void on_LightMode_5_currentIndexChanged(int index);

    void on_LightMode_4_currentIndexChanged(int index);

    void on_LightMode_6_currentIndexChanged(int index);



    void loadConfigure();//1124
    void saveConfigure();

    void saveConfigureFlash();

    void on_checkBox_SP2_2_stateChanged(int arg1);

    void on_insInterface_4_textChanged(const QString &arg1);



    void on_lineEdit_12_returnPressed();

    void on_LightMode3_currentIndexChanged(int index);




    void on_checkBox_SP2_3_stateChanged(int arg1);

    void on_lineEdit_18_returnPressed();

    void on_LightMode_3_currentIndexChanged(int index);

    void on_lineEdit_13_returnPressed();

    void on_lineEdit_16_returnPressed();

    void on_lineEdit_15_returnPressed();

    void on_lineEdit_17_returnPressed();

    void on_lineEdit_14_returnPressed();

    void on_LightMode_7_currentIndexChanged(int index);

    void on_lineEdit_23_returnPressed();

    void on_ChuFaCombox_3_currentIndexChanged(int index);


    void on_FenPinSpinBox_2_valueChanged(int arg1);

    void on_BeiPinSpinBox_2_valueChanged(int arg1);

    void on_lineEdit_24_returnPressed();

    void on_lineEdit_25_returnPressed();

    void on_LightMode_8_currentIndexChanged(int index);

    void on_lineEdit_26_returnPressed();

    void on_lineEdit_27_returnPressed();

    void on_lineEdit_28_returnPressed();

    void on_lineEdit_29_returnPressed();

    void on_lineEdit_30_returnPressed();

    void on_LightMode_14_currentIndexChanged(int index);

    void on_LightMode_12_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_comboBox_6_currentIndexChanged(int index);

    void on_lineEdit_19_returnPressed();

    void on_lineEdit_20_returnPressed();

    void on_lineEdit_21_returnPressed();


    void on_pushButton_8_clicked();


//============在线升级1222==========================
    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;
    QVector<quint16> m_data;
    quint16 addr;
    quint16 num;
    QTimer *info_timer;
    cv::Mat img;
    static MainWindow* mpMainWindow;//wdj


    //static MainWindow* mpMainWindow;
    QButtonGroup *imageCheckBoxGroup;//0807checkgroup
    void new_titlebar_init();
};

Q_DECLARE_METATYPE(QModbusDevice::State)
Q_DECLARE_METATYPE(QModbusDevice::Error)
//Q_DECLARE_METATYPE(QModbusDataUnit::RegisterType)
Q_DECLARE_METATYPE(QVector<quint16>)
Q_DECLARE_METATYPE(unsigned char *)
//Q_DECLARE_METATYPE(InsCam)

#endif // MAINWINDOW_H
