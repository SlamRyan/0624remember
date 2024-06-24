#include <string>
#include "../include/Ins_Camera_proc.h"
#include "../include/ins_camera_c.h"
#include "../include/base_camera.h"
#include "../include/Ins_Utility_proc.h"
#include "../include/ins_2D5_log.h"
#include"../../Demo/include/ins_image_processing.h"
#include <map>

#define GET_INS_CAMERA_HANDLE(ins_handle) \
    ((InsCameraIkap*)((InsPMDL_System*)(ins_handle))->SystemCamera.Camera->ins_camera)

#define GET_INS_LIGHT_HANDLE(ins_handle) \
    ((InsPMDL_lightSouce*)((InsPMDL_System*)(ins_handle))->SystemLight.light->ins_light)



namespace Ins2D5Camera
{


    std::map<void*, InsPMDL_SystemInfo> globalSystemMap;

    #define CHECKFAULT(x) if (x == 0||x == 4097)

    //int TriggerModeInt = 0;

    uint32_t __cdecl InsPMDL_GetSdkVersion()
    {
        return VERSION;
    }

    INSCAMERA_API InsResult __cdecl InsPMDL_FindCameraDevice(int* _num)
    {
        return  FindCamera(*_num);
    }

    INSCAMERA_API InsResult __cdecl InsPMDL_GetCameraDeviceInfo(int _num, InsCameraInfo* _insCameraInfo)
    {
        return  GetCameraDeviceInfo(_num, *_insCameraInfo);
    }
    void InsPMDL_CreateCameraHandle(void** _ins_handle, int _index) //0429
    {
        //Ins2D5CameraLog::Ins2D5Logger::mpLogger->info("Losd 2D5 SDK successfully {} {}", 1, "23");
        *_ins_handle = new InsCameraIkap();//һ����ʲô�������ʲô���ָ�룬�����ָ��=new���࣬���ǲ��ܸ���ָ��=new����,��ǿת�����࣬���ණ���ȸ���࣬���ò�������ĳ�Ա
        ((InsCameraIkap*)*_ins_handle)->mpCommunication = Ins_Modbus_udp::get_instance();
        ((InsCameraIkap*)*_ins_handle)->ConnectIndexCamera(_index);
        Ins2D5CameraLog::Ins2D5Logger::init();
        Ins2D5CameraLog::Ins2D5Logger::info("Load 2D5 SDK successfully.");
    }


    //-----------0511--------------

    void __cdecl InsPMDL_LoadSystemInfoFromFile(const char* filename, InsPMDL_SystemInfo& systemInfo)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        std::string key;
        int value;

        // ��ȡ�����ļ�ǰ 6 �в�����
        for (int i = 0; i < 6; i++) {
            if (!std::getline(file, line)) break;  // �����ȡʧ�ܻ򵽴��ļ�β���˳�ѭ��

            std::istringstream iss(line);
            std::string key, value;

            // ��ȡ�ؼ���ֱ������ð��
            if (!std::getline(iss, key, ':')) {
                std::cerr << "Failed to parse key from line: " << line << std::endl;
                continue;
            }

            // ʹ�� getline �ٴζ�ȡ���µĲ�����Ϊֵ
            if (!std::getline(iss, value))
            {
                std::cerr << "Failed to parse value from line: " << line << std::endl;
                continue;
            }

            // �Ƴ�ֵǰ��Ŀո�
            value.erase(0, value.find_first_not_of(" \t\n\r\f\v")); // �Ƴ�ǰ��Ŀհ�
            value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1); // �Ƴ�����Ŀհ�

            // ����ؼ��֣�ȥ������ո�
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());

            if (key == "CamNum")
            {
                systemInfo.CamInfo.CamNum = std::stoi(value);
                cout << "CamNum :" << systemInfo.CamInfo.CamNum << endl;
            }
            else if (key == "LigNum")
            {
                systemInfo.LightInfo.LightNum = std::stoi(value);

                //const char* len = "30";
                //strcpy(systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length, len);

                cout << "LightNum :" << systemInfo.LightInfo.LightNum << endl;
                //cout << "LightBoardNum :" << systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length << endl;
            }
            else if (key == "LocalIP")
            {
                strcpy(systemInfo.LightInfo.LightDeviceInfo[0]->localIP, value.c_str());
                cout << "LocalIP:::" << systemInfo.LightInfo.LightDeviceInfo[0]->localIP << endl;

            }
            else if (key == "LightIP")
            {
                strcpy(systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.ip_addr, value.c_str());
                cout << "LightIP:::" << systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.ip_addr << endl;
            }
            else if (key == "Board")
            {
                systemInfo.CamInfo.id[0] = std::stoi(value);
                cout << "IkapBoardInex :" << systemInfo.CamInfo.id[0] << endl;
            }
            else if (key == "Length")
            {
                strcpy(systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length, value.c_str());
                cout << "Length:::" << systemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length << endl;

            }
        }

        file.close();
    }


    InsResult __cdecl InsPMDL_Initial25DSystem(void** _ins_handle,const char* filename)
    {


        int num = 0;
        InsPMDL_SystemInfo SystemInfo;
        SystemInfo.LightInfo.LightDeviceInfo[0] = new Ins2D5LightDeviceAndLocalIP;

        InsPMDL_FindCameraDevice(&num);
        //SystemInfo.LightInfo.LightDeviceInfo[0] = new Ins2D5LightDeviceAndLocalIP;
        InsPMDL_LoadSystemInfoFromFile(filename, SystemInfo);
        InsPMDL_GetSystemHandle(_ins_handle, SystemInfo);
        InsPMDL_ConfigureBoardWith250K(*_ins_handle);
        InsPMDL_LoadConfigureFileToSystem(*_ins_handle, filename);

        return  (InsResult)0;
    }
   


    //------------

    InsResult __cdecl InsPMDL_SaveSNToLight(void* _ins_handle, char _SN[64])
    {

        //return  (InsResult)((InsPMDL_lightSouce*)_ins_handle)->save_SN((uint16_t*)_SN);//SNSNSN
        return  (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->save_SN((uint16_t*)_SN);

 
    }

     InsResult __cdecl InsPMDL_GetSNFromLight(void* _ins_handle, const char* _SN)
    {
        return  (InsResult)((InsPMDL_lightSouce*)_ins_handle)->read_SN((uint16_t*)_SN);
        //return (InsResult)(InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->read_SN((uint16_t*)_SN);
    }

     InsResult __cdecl InsPMDL_GetLightSourceHandle(void** _ins_light, Ins2D5LightDeviceAndLocalIP _deviceInfo)
    {
        *_ins_light = new InsPMDL_lightSouce(_deviceInfo);
        return  (InsResult)((InsPMDL_lightSouce*)*_ins_light)->init_communication();
    }

    InsResult __cdecl ins_SaveBoardConfigurationToFile(void* _ins_handle)
    {
        return (InsResult)(static_cast<InsCameraIkap*>(_ins_handle)->SaveBoardConfigurationToFile());
    }

    InsResult __cdecl SaveCameraConfigurationToFile(void* _ins_handle, char* _filename)
    {
        return (InsResult)(static_cast<InsCameraIkap*>(_ins_handle)->SaveBoardConfigurationToFile());
    }


    std::string generateSystemModel(const std::string& cameraModel, const std::string& lightSourceModel)//const std::string& cameraModel, const std::string& lightSourceModel
    {
        // Camera
        size_t start = cameraModel.find("INS-LC-") + 7; //0409
        size_t end = cameraModel.find('-', start);
        std::string cameraKeyPart = cameraModel.substr(start, end - start);

        // Light source
        std::string lightSourceKeyPart = "";
        size_t lastDash = lightSourceModel.find_last_of('-');
        for (size_t i = lastDash + 1; i < lightSourceModel.length(); ++i) 
        {
            if (std::isdigit(lightSourceModel[i])) {
                lightSourceKeyPart += lightSourceModel[i];
            }
        }

        // System
        std::string systemModel = "INS-PMDL-25D-" + cameraKeyPart + lightSourceKeyPart + "M";

        return systemModel;
    }


    //--------------------------

    std::string InsPMDL_GenerateSystemModel(std::string cameraModel, std::string lightSourceModel)
    {
        // Camera
        size_t start = cameraModel.find("INS-LC-") + 7;
        size_t end = cameraModel.find('-', start);
        std::string cameraKeyPart = cameraModel.substr(start, end - start);

        // Light source
        std::string lightSourceKeyPart = "";
        size_t lastDash = lightSourceModel.find_last_of('-');
        for (size_t i = lastDash + 1; i < lightSourceModel.length(); ++i)
        {
            if (std::isdigit(lightSourceModel[i])) {
                lightSourceKeyPart += lightSourceModel[i];
            }
        }

        // System
        std::string systemModel = "INS-PMDL-25D-" + cameraKeyPart + lightSourceKeyPart + "M";

        return systemModel;
    }

    //--------------------
    InsResult __cdecl InsPMDL_FindOtherInfo(void* _ins_handle, OtherDeviceInfo* _insdeviceinfo)//������Ϣ
    {
        //uint32_t ret = 0;
        //ret = (uint32_t)(static_cast<InsCameraIkap*>(_ins_handle)->FindCamera());//������dynamicת,�������ﲻ��
        //CHECKFAULT(ret){
        //    ret = (uint32_t)static_cast<InsCameraIkap*>(_ins_handle)->ConnectCamera();
        //    CHECKFAULT(ret) 
        //    {
        //        if ( GET_INS_CAMERA_HANDLE(_ins_handle)->full_name_.size() > 0) {
        //            std::cout <<  GET_INS_CAMERA_HANDLE(_ins_handle)->full_name_[0] << std::endl;
        //        }
        //        else {
        //            std::cout << "Error: full_name_ array is empty" << std::endl;
        //        }
        //        _insdeviceinfo->stInsCameraInfo->divice_full_name_ =  GET_INS_CAMERA_HANDLE(_ins_handle)->full_name_[0].c_str();
        //        _insdeviceinfo->stInsCameraInfo->device_class_ =  GET_INS_CAMERA_HANDLE(_ins_handle)->device_class_[0].c_str();
        //    }
        //}
        //InsPMDL_SearchDevice(_ins_handle, _insdeviceinfo->stIns2D5LightDeviceInfo);
        // GET_INS_CAMERA_HANDLE(_ins_handle)->mpCommunication->initmodbus_udp(DEVICE::get_instance()->ip_addr, LOCALDEVICE::get_instance()->ip, 502);//modbus
        //std::string str(DEVICE::get_instance()->length);
        //int num = std::stoi(str);
        //
        //GET_INS_LIGHT_HANDLE(_ins_handle)->fpga_device_initialization(num);
        GET_INS_LIGHT_HANDLE(_ins_handle)->get_other_info(_insdeviceinfo);
        //std::string camName = (_insdeviceinfo->stInsCameraInfo->divice_full_name_);
        //std::string lightName = _insdeviceinfo->stIns2D5LightDeviceInfo->device_model;
        //std::string systemModel = generateSystemModel(camName, lightName);
        //_insdeviceinfo->stOtherInfo->system_name_ = _strdup(systemModel.c_str());//���ֱ�ӵ��ڵĻ����������ΪgenerateSystemModel���ص���һ����ʱ��ָ�룬����������ͻ��Զ��ͷ�
        //return (InsResult)ret;

        return (InsResult)0;

    }


    InsResult __cdecl InsPMDL_FPGAInitial(void* _ins_handle)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        GET_INS_LIGHT_HANDLE(_ins_handle)->fpga_device_initialization(boardNum);

        return InsResult::INSCSTATUS_OK;
    }

    InsResult __cdecl InsPMDL_SelectDirection(void* _ins_handle, int index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        GET_INS_LIGHT_HANDLE(_ins_handle)->select_direction(index, boardNum);
        return InsResult::INSCSTATUS_OK;

    }

    INSCAMERA_API InsResult __cdecl InsPMDL_SetTimeOut(void* _ins_handle, int time)
    {
        return  InsResult(GET_INS_LIGHT_HANDLE(_ins_handle)->set_timeout(time));
    }

    INSCP ins_cp1;
    InsResult InsPMDL_FindLightSourceDevice(int *_num)
    {
        ins_cp1.search_all_devices(*_num);
        //ins_cp1.InsPMDL_ResetDeviceIp("192.168.1.25");
        return InsResult::INSCSTATUS_OK;
    }

    InsResult __cdecl  InsPMDL_GetLightSouceDeviceInfo(int num, Ins2D5LightDeviceAndLocalIP* _deviceInfo)
    {
        return (InsResult)ins_cp1.get_device_info(num, *_deviceInfo);
    }
    InsResult __cdecl  InsPMDL_ResetLightSourceIP(const char* _ip, Ins2D5LightDeviceAndLocalIP* _deviceInfo)
    {
        return (InsResult)ins_cp1.reset_device_ip(_ip, *_deviceInfo);
    }

    InsResult __cdecl ins_grab_once(void* _ins_handle)
    {
        return (InsResult)((InsCameraIkap*) _ins_handle)->GrabOnce();
    }

    InsResult __cdecl InsPMDL_GrabContinous(void* _ins_handle)//0429K
    {
        //int LightSignInt = boardNum * 10;

        InsResult ret = GET_INS_CAMERA_HANDLE(_ins_handle)->GrabContinous();
        if (0X00001001 == (int)ret)
        {
            if (((InsPMDL_System*)(_ins_handle))->SystemLight.light->TriggerMode == 1) 
            {
                return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_on(1);
            }
            else if (((InsPMDL_System*)(_ins_handle))->SystemLight.light->TriggerMode == 2)
            {
                return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_on(2);
            }
            else if (((InsPMDL_System*)(_ins_handle))->SystemLight.light->TriggerMode == 3)
            {
                return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_on(3);
            }
           
        }
        return ret;

        //----------------------------------------------------------------�� before
        //InsResult ret =  GET_INS_CAMERA_HANDLE(_ins_handle)->GrabContinous();
        // if(0X00001001 ==(int)ret)
        // { 
        //     return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_on(_index);
        // }
        // return ret;
    }



    /*
    InsResult __cdecl ins_cxpgrab_continous(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->CXPGrabContinous();
    }
*/
    InsResult __cdecl InsPMDL_GrabStop(void* _ins_handle)
    {
 /*       InsResult ret = GET_INS_CAMERA_HANDLE(_ins_handle)->GrabStop();
        if (0X00001001 == (int)ret)

        {
            return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_off();
        }
        return ret;*/


        (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_mode_off();
        InsResult ret = GET_INS_CAMERA_HANDLE(_ins_handle)->GrabStop();
        return ret;


    }

    //_index:1-����0-��
    //��λ���������ȡ��,ֻ�е���ʱ�õ�,��ʼ�ᱣ���иߣ���Ƶ���ع⣬�ڹ�ʱ�Զ�����
    //����ȡ���ȫ�ֲ���
    int myLineHigh = 0;//�и�
    static bool freeRunFlag = false;//��־λ
    double CameraTriggerCycle = 0;//��ɨ������
    double mylineFree = 0;///��Ƶ
    uint32_t TriNum1 = 0;//��������
    double maxExposure = 0;///��Ƶ
    double myExposure = 0;///�ع�
    int myLineWidth = 0;
    InsResult __cdecl InsPMDL_FreerunForFocus(void* _ins_handle, InsPMDL_Switch _index)//�������CamType//����ȡ��
    {
   
        int CamType;
        std::string name = GET_INS_CAMERA_HANDLE(_ins_handle)->get_cam_full_name();
        cout << "Full Name :" << name << endl;
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;

        uint32_t _picture_num;
        ((InsPMDL_lightSouce*)((InsPMDL_System*)_ins_handle)->SystemLight.light->ins_light)->readFromModbusOne(0x0C08, _picture_num);
        if (_picture_num == 0)
        {
            _picture_num = 8;
        }
        else if (_picture_num == 1)
        {
            _picture_num = 9;
        }

        int LightSignInt = boardNum * 10;
        if ((_index == 1) && (freeRunFlag == false))//0318
        {
            myLineWidth =  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_width();
            std::cout << "MyLineWidth:" << myLineWidth << std::endl;
            myLineHigh =  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_height();
            // GET_INS_CAMERA_HANDLE(_ins_handle)->freerunReset(0);//0402��ӣ�������Ϊ8000
            GET_INS_LIGHT_HANDLE(_ins_handle)->get_scanning_period(CameraTriggerCycle, mylineFree);
            GET_INS_LIGHT_HANDLE(_ins_handle)->get_soft_trigger_num(TriNum1);
            myExposure =  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_exposure();
            //===============�̶��иߺ�Ƶ��================================
            int FoucusHigh = 500;//0401->800->300
             GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_height(FoucusHigh);
             GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(20);
            // GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_width(8000);
             GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFrameGrabber();
             if (name.find("4K") != string::npos)
             {
                 CamType = 0;
             }

             else if (name.find("8KCXP") != string::npos)
             {
                 CamType = 1;
             }

             else if (name.find("16KCXP") != string::npos)
             {
                 CamType = 2;
             }

            GET_INS_LIGHT_HANDLE(_ins_handle)->FreeRunForFocus(boardNum, _picture_num, _index,CamType);
            double CameraTriggerCycle1 = 240; //30->100 //100->800  800->240
             //0415;

            if (name.find("4K") != string::npos)
            {
                CameraTriggerCycle1 = CameraTriggerCycle1 / _picture_num;
    
            }

            else if (name.find("8KCXP") != string::npos)
            {
                CameraTriggerCycle1 = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate8kLinePeriod(CameraTriggerCycle1 / _picture_num);
       
            }

            else if (name.find("16KCXP") != string::npos)
            {
                CameraTriggerCycle1 = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate16kLinePeriod(CameraTriggerCycle1 / _picture_num);
                
            }

            //if (CamType == 0)//4K
            //{
            //    CameraTriggerCycle1 = CameraTriggerCycle1 / _picture_num;
            //}
            //else if (CamType == 1)//8K
            //{
            //    CameraTriggerCycle1 = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate8kLinePeriod(CameraTriggerCycle1 / _picture_num);
            //}
            //else if (CamType == 2)
            //{
            //    CameraTriggerCycle1 = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate16kLinePeriod(CameraTriggerCycle1 / _picture_num);
            //}
            std::cout << "calculateNum:" << CameraTriggerCycle1 << std::endl;

             GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_line_period_time(CameraTriggerCycle1);//����calculat  P//_line_scanning_period/_picture_num 0408
            // GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_line_period_time(CameraTriggerCycle1);

            //InsPMDL_SetScanningPeriod();0415
            // GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(28);//��������Ƶ֮�������ع�
            freeRunFlag = true;
            //----------
            GET_INS_LIGHT_HANDLE(_ins_handle)->FreeRunSinToContinue(LightSignInt); //0423
            // ----------
            //return InsPMDL_GrabContinous(_ins_handle, _ins_light, 2);//����ȡ��
            INS_PMDL_TriggerModeSelect TriMode = SoftTrigger;
            InsPMDL_SetTriggerMode(_ins_handle,TriMode);
            InsPMDL_GrabContinous(_ins_handle);//����ȡ��

            //write_fpga_reg(_ins_light,"INSNEX", 0x1608,1, 1);//0318

            return InsResult::INSCSTATUS_OK;

        }
        else if ((_index == 0) &&( freeRunFlag == true))

        {

            freeRunFlag = false;

            InsPMDL_GrabStop(_ins_handle);

            if (name.find("4K") != string::npos)
            {
                CamType = 0;
            }

            else if (name.find("8KCXP") != string::npos)
            {
                CamType = 1;
            }

            else if (name.find("16KCXP") != string::npos)
            {
                CamType = 2;
            }

            if (LightSignInt ==30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x0C0A, 8);//���ɹ�0407
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1180, 0);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x0B00, 138);

                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101A, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101C, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1024, 1);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x0C0A, 8);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1180, 0);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x0B00, 138);

                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101A, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101C, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1024, 1);
            }
            else if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x0C0A, 8);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1180, 0);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x0B00, 138);

                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101A, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101C, 2815);
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1024, 1);


            }


            //---------------0423 -------�������������

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x0C0A, 8);//1814
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x0C0A, 8);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x0C0A, 8);
            }

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1182, 32);//2304
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1182, 32);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1182, 32);
            }

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1200, 0);//2400
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1200, 0);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1200, 0);
            }

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1240,12);//2480
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1240,12);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1240,12);
            }

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1280, 0);//2500
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1280, 0);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1280, 0);
            }

            if (LightSignInt == 10)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1180, 0);//2300
            }
            else if (LightSignInt == 30)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1180, 0);
            }
            else if (LightSignInt == 50)
            {
                GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1180, 0);
            }



            //--------------



            myLineWidth =  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_width();
            std::cout << "MyLineWidth222222:" << myLineWidth << std::endl;


            //================�Ļ��û����иߺ�Ƶ�ʺʹ����������ع�================================

            // GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_width(4000);
            if (CamType == 0)
            {
                uint32_t Rh;
                ins_read_camera_height(((InsPMDL_lightSouce*)_ins_handle), &Rh);//0418 ɨ������
                GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_height(Rh);
            }
            else
            {
                 GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_height(myLineHigh);
            }
             GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFrameGrabber();
            GET_INS_LIGHT_HANDLE(_ins_handle)->set_scanning_period(boardNum, _picture_num, CameraTriggerCycle, mylineFree, maxExposure,CamType);
             GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(myExposure);
            // GET_INS_CAMERA_HANDLE(_ins_handle)->freerunReset(1);
            

            //�������� ----
            GET_INS_LIGHT_HANDLE(_ins_handle)->set_soft_trigger_num(TriNum1);
            
            
      



            return InsResult::INSCSTATUS_OK;
        }
        else
        {
            return InsResult::INSSTATUS_INVALID_PAM;
        }
         
    }
    InsResult __cdecl InsPMDL_ConfigureBoard(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureBoard();
    }
    InsResult __cdecl InsPMDL_ConfigureBoardWith250K(void* _ins_handle)
    {
        return (InsResult)GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureBoardWith250K();
    }
    InsResult __cdecl LoadLocalFile(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->LoadLocalFile();//��ʱ���£�����Ҫ����200k
    }
    /*
    InsResult __cdecl ins_ConfigureFileToCamera(char* filename_)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFileToCamera(filename_);
    }
*/
    InsResult __cdecl ins_save_board_configuration_to_file(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->SaveBoardConfigurationToFile();
    }
    /*
    InsResult __cdecl ins_SaveCameraConfigurationToFile(char* filename_)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->SaveCameraConfigurationToFile(filename_);
    }*/
    InsResult __cdecl InsPMDL_LoadConfigureFileToSystem(void* _ins_handle,const char* filename_)
    {
        
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        cout << "LoadConfigureICF0511::"<<boardNum << endl;
        //------------
        uint32_t ret = 0;
        InsResult camJudge =  GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFileToFrameGrabber(filename_);//icf
         if (camJudge == InsResult::INSSTATUS_SUCCESS)
         {
             InsPMDL_ConfigureFrameGrabber(_ins_handle);
             ret = GET_INS_LIGHT_HANDLE(_ins_handle)->load_config_file(boardNum, filename_);//Length/10
             return (InsResult)ret;
         }
         return (InsResult)camJudge;
    }

    InsResult InsPMDL_SaveSystemConfigFile(void* _ins_handle, const char* _config_filename)//save icf
    {

        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;

        uint32_t ret = 0;
        

        auto it = globalSystemMap.find(_ins_handle);
        if (it != globalSystemMap.end())
        {
            &it->second;
        }
        else 
        {
            return (InsResult)-1;
        }
        ret = (uint32_t) GET_INS_CAMERA_HANDLE(_ins_handle)->SaveConfigureFileFromeFrameGrabber(_config_filename, ((InsPMDL_System*)_ins_handle)->SystemCamera.CameraCount, ((InsPMDL_System*)_ins_handle)->SystemLight.LightCount, it->second);
        if (ret == 0 || ret == 4097)
        {
            return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->save_config_file(boardNum, _config_filename);
        }
        else
        {
            return (InsResult)ret;
        }
    }

    InsResult __cdecl InsPMDL_Close(void** _ins_handle)
    {
        //(InsResult)((InsCameraIkap*)(*_ins_handle))->Close();//�ͷ�һЩ��Դ��to do
        //((InsCameraIkap*)(*_ins_handle))->~InsCameraIkap();
        //(*_ins_handle) = nullptr;
        ////(*_ins_light) = nullptr;//0411
        //return (InsResult)0;



        InsPMDL_System* system = (InsPMDL_System*)(*_ins_handle);

        // �ͷ�ϵͳ�е������Դ
        for (uint32_t i = 0; i < system->SystemCamera.CameraCount; ++i) 
        {
            InsCameraIkap* camera = (InsCameraIkap*)(system->SystemCamera.Camera[i].ins_camera);
            if (camera) 
            {
                camera->Close();  
                delete camera;  
            }
        }

        // �ͷ�ϵͳ�еĹ�Դ��Դ
        for (uint32_t i = 0; i < system->SystemLight.LightCount; ++i) 
        {
            InsPMDL_lightSouce* light = (InsPMDL_lightSouce*)(system->SystemLight.light[i].ins_light);
            if (light) 
            {
                delete light;                 // ɾ����Դ����
            }
        }

        // ����ͷ�InsPMDL_System������
        delete system;
        *_ins_handle = nullptr;

        return (InsResult)0; 
    }

    void  __cdecl InsPMDL_SetDataCallback(void* _ins_handle,DataPocessCallBack _callback)
    {
        GET_INS_CAMERA_HANDLE(_ins_handle)->set_data_callback(_callback);
    }
    void  __cdecl InsPMDL_UnregisterDataCallback(void* _ins_handle) 
    {
         GET_INS_CAMERA_HANDLE(_ins_handle)->unregister_data_callback();
    }
    InsResult __cdecl InsPMDL_ConfigureFrameGrabber(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFrameGrabber();
    }

    void InsPMDL_SetDeviceRemoveCallback(void* _ins_handle, OffLineCallBack _callback)
    {
         GET_INS_CAMERA_HANDLE(_ins_handle)->set_device_remove_callback(_callback);
    }

    void InsPMDL_UnregisterDeviceRemoveCallback(void* _ins_handle)
    {
         GET_INS_CAMERA_HANDLE(_ins_handle)->unregister_device_remove_callback();
    }

    int __cdecl InsPMDL_GetFrameSize(void* _ins_handle)
    {
        return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_frame_size();
    }

    InsResult __cdecl ins_set_trigger_source_activated(void* _ins_handle,int _index)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_trigger_source_activated(_index);
    }

    InsResult __cdecl ins_set_work_mode_activated(void* _ins_handle, int _index)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_work_mode_activated(_index);
    }

    InsResult __cdecl InsPMDL_SetWidth(void* _ins_handle, int _ins_width)
    {
        return (InsResult)GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_width(_ins_width);
    }

    InsResult __cdecl InsPMDL_SetHeight(void* _ins_handle, int _ins_height)
    {
        GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_height(_ins_height);
        //return (InsResult) ((InsCameraIkap*)_ins_handle)->set_ins_height(_ins_height)

        //std::string name = GET_INS_CAMERA_HANDLE(_ins_handle)->get_cam_full_name();
        //cout << "Full Name :" << name << endl;


        //std::string name2 = GET_INS_CAMERA_HANDLE(_ins_handle)->CamFullName;
        //cout << "Full Name2 :" << name2<< endl;


        return (InsResult)0;
    }

    int __cdecl InsPMDL_GetWidth(void* _ins_handle)
    {
        return GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_width();
    }

    int __cdecl InsPMDL_GetHeight(void* _ins_handle)
    {

        //return ((InsCameraIkap*)((InsPMDL_System*)_ins_handle)->SystemCamera.Camera->ins_camera)->get_ins_height();
        //       ((InsCameraIkap*)((InsPMDL_System*)(ins_handle))->SystemCamera.Camera->ins_camera)
        return GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_height();
        //return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_height();//0429H
    }

    InsResult __cdecl InsPMDL_SetFrameCount(void* _ins_handle, int _ins_frame_count)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_frame_count(_ins_frame_count);
    }

    InsResult __cdecl ins_set_image_type(void* _ins_handle, int _index)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_image_type(_index);
    }

    InsResult __cdecl ins_set_line_trigger(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_line_trigger();
    }

    InsResult __cdecl ins_set_frame_trigger(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_frame_trigger();
    }

    InsResult __cdecl InsPMDL_ConfigureExternalPulseFileToCamera(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureExternalPulseFileToCamera();//���ɹ�ʱ�õ�������ʵ����һֱû�õ�
    }

    InsResult __cdecl InsPMDL_ConfigureFreeRunFileToCamera(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ConfigureFreeRunFileToCamera();//���ɿ��õ�������ʵ����һֱû�õ�
    }

   
    InsResult __cdecl InsPMDL_SetExposure(void* _ins_handle, double _val)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(_val);
    }

    double __cdecl InsPMDL_GetExposure(void* _ins_handle)
    {
        //Ins2D5CameraLog::Ins2D5Logger::info("GetExposure.");
        //double exposure;
        //exposure =  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_exposure();
        // Ins2D5CameraLog::Ins2D5Logger::info("exposure is:" + std::to_string(exposure));
        //return GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_exposure();

        //return ((InsCameraIkap*)_ins_handle)->get_ins_exposure();
       // return GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_exposure();

        //------------------0527
        log_message("InsPMDL_GetExposure called");
        if (_ins_handle == nullptr) 
        {
            log_message("Error: ins_handle is null");
            return -1;  // ���󷵻�ֵ
        }

        InsPMDL_System* system = (InsPMDL_System*)_ins_handle;
        if (system->SystemCamera.Camera == nullptr) 
        {
            log_message("Error: SystemCamera.Camera is null");
            return -1;  // ���󷵻�ֵ
        }

        if (system->SystemCamera.Camera->ins_camera == nullptr) 
        {
            log_message("Error: SystemCamera.Camera->ins_camera is null");
            return -1;  // ���󷵻�ֵ
        }

        log_message("GET_INS_CAMERA_HANDLE: all pointers are valid");

        InsCameraIkap* cameraHandle = (InsCameraIkap*)system->SystemCamera.Camera->ins_camera;
        double exposure = cameraHandle->get_ins_exposure();
        log_message("Exposure value obtained: " + std::to_string(exposure));
        return exposure;
    }


    InsResult __cdecl ins_set_line_period_time(void* _ins_handle, double _val)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_line_period_time(_val);//���ɿ��õ�
    }
    
    double __cdecl ins_get_line_period_time(void* _ins_handle)
    {
        return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_line_period_time();
    }

    InsResult __cdecl InsPMDL_SetDigitalGain(void* _ins_handle, double _val)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_digital_gain(_val);
    }

    double __cdecl InsPMDL_GetDigitalGain(void* _ins_handle)
    {
        return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_digital_gain();
    }

    InsResult __cdecl InsPMDL_SetPacGain(void* _ins_handle,char* _ch)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_PAC_gain(_ch);
    }

    InsResult __cdecl InsPMDL_GetPacGain(void* _ins_handle, char* _ch)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_PAC_gain(_ch);
    }

    InsResult __cdecl InsPMDL_SetShaftEncoder1MinWidth(void* _ins_handle, int _ins_shaft_encoder1_min_width)//�����������ź��˲�����
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_shaft_encoder1_min_width_(_ins_shaft_encoder1_min_width);
    }
    int __cdecl InsPMDL_GetShaftEncoder1MinWidth(void* _ins_handle)//���󷵻ش�����Ϊint���ֵ
    {
        return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_shaft_encoder1_min_width_();//���󷵻ش�����Ϊint���ֵ
    }
    int __cdecl InsPMDL_GetFrameId(void* _ins_handle)
    {
        return  GET_INS_CAMERA_HANDLE(_ins_handle)->get_ins_frame_id_();//�޴�����
    }
    InsResult __cdecl ins_user_set_save(void* _ins_handle)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->UserSetSave();//�޴�����
    }

    void __cdecl InsPMDL_InitLocalInfo(const char* _ip, const char* _NET_mask, const char* _gateway)
    {
        LOCALDEVICE::get_instance()->init(_ip, _NET_mask, _gateway);
    }
    InsResult __cdecl InsPMDL_SearchDevice(void* _ins_handle, Ins2D5LightDevice* _ins_device)
    {
        //initial local device ip
        int lightNum = 0;
        uint32_t ret =  GET_INS_CAMERA_HANDLE(_ins_handle)->ins_cp.search_all_devices(lightNum);
        strcpy(_ins_device->net_mask, DEVICE::get_instance()->net_mask);
        strcpy(_ins_device->gateway, DEVICE::get_instance()->gateway);
        strcpy(_ins_device->mac_addr, DEVICE::get_instance()->mac_addr);
        strcpy(_ins_device->manufacturer_name, DEVICE::get_instance()->manufacturer_name);
        strcpy(_ins_device->device_vision, DEVICE::get_instance()->device_vision);
        strcpy(_ins_device->model_name, DEVICE::get_instance()->model_name);
        strcpy(_ins_device->device_model, DEVICE::get_instance()->device_model);
        strcpy(_ins_device->length, DEVICE::get_instance()->length);
        strcpy(_ins_device->SN, DEVICE::get_instance()->SN);
        strcpy(_ins_device->ip_addr, DEVICE::get_instance()->ip_addr);
        return (InsResult)ret;
    }


    InsResult __cdecl InsPMDL_ResetDeviceIp(void* _ins_handle, const char* _ip)
    {
        return (InsResult) GET_INS_CAMERA_HANDLE(_ins_handle)->ins_cp.InsPMDL_ResetDeviceIp(_ip);
    }

    InsResult __cdecl InsPMDL_InitCommunication(void* _ins_handle)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->init_communication();//modbus
    }

    /*
    int ins_factory_configuration(void* _ins_handle, Ins2D5Device* ins_device)
    {
        strcpy(DEVICE::get_instance()->net_mask, ins_device->net_mask);
        strcpy(DEVICE::get_instance()->gateway, ins_device->gateway);
        strcpy(DEVICE::get_instance()->mac_addr, ins_device->mac_addr);
        strcpy(DEVICE::get_instance()->manufacturer_name, ins_device->manufacturer_name);
        strcpy(DEVICE::get_instance()->device_vision, ins_device->device_vision);
        strcpy(DEVICE::get_instance()->model_name, ins_device->model_name);
        strcpy(DEVICE::get_instance()->device_model, ins_device->device_model);
        strcpy(DEVICE::get_instance()->length, ins_device->length);
        strcpy(DEVICE::get_instance()->SN, ins_device->SN);
        strcpy(DEVICE::get_instance()->ip_addr, ins_device->ip_addr);
        //����
        memset(ins_device->manufacturer_name, 0, sizeof(DEVICE::get_instance()->manufacturer_name));
        memset(ins_device->model_name, 0, sizeof(DEVICE::get_instance()->model_name));
        memset(ins_device->device_model, 0, sizeof(DEVICE::get_instance()->device_model));
        memset(ins_device->length, 0, sizeof(DEVICE::get_instance()->length));
        memset(ins_device->SN, 0, sizeof(DEVICE::get_instance()->SN));
        memset(ins_device->device_vision, 0, sizeof(DEVICE::get_instance()->device_vision));
        memset(ins_device->ip_addr, 0, sizeof(DEVICE::get_instance()->ip_addr));
        memset(ins_device->net_mask, 0, sizeof(DEVICE::get_instance()->net_mask));
        memset(ins_device->gateway, 0, sizeof(DEVICE::get_instance()->gateway));
        memset(ins_device->mac_addr, 0, sizeof(DEVICE::get_instance()->mac_addr));
        int ret =  GET_INS_CAMERA_HANDLE(_ins_handle)->ins_cp.factory_configuration();
        if (ret == 0)
        {
            strcpy(ins_device->net_mask, DEVICE::get_instance()->net_mask);
            strcpy(ins_device->gateway, DEVICE::get_instance()->gateway);
            strcpy(ins_device->mac_addr, DEVICE::get_instance()->mac_addr);
            strcpy(ins_device->manufacturer_name, DEVICE::get_instance()->manufacturer_name);
            strcpy(ins_device->device_vision, DEVICE::get_instance()->device_vision);
            strcpy(ins_device->model_name, DEVICE::get_instance()->model_name);
            strcpy(ins_device->device_model, DEVICE::get_instance()->device_model);
            strcpy(ins_device->length, DEVICE::get_instance()->length);
            strcpy(ins_device->SN, DEVICE::get_instance()->SN);
            strcpy(ins_device->ip_addr, DEVICE::get_instance()->ip_addr);
        }
        return ret;
    }
    */
 

    InsResult __cdecl InsPMDL_PowerOffSave(void* _ins_handle)
    {
       
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;

        //uint32_t ret = ((InsPMDL_lightSouce*)_ins_handle)->save_config_in_device(boardNum);
        uint32_t ret = GET_INS_LIGHT_HANDLE(_ins_handle)->save_config_in_device(boardNum);
        if (ret == 0)
        { 
            InsResult e =  GET_INS_CAMERA_HANDLE(_ins_handle)->UserSetSave();
            //return (InsResult)e;  0417
        }
        return (InsResult)ret;
    }


    //0315---------------------------------------------
    InsResult InsPMDL_FpgaRegInitial(void* _ins_handle)

    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)((InsPMDL_lightSouce*)_ins_handle)->fpga_device_initialization(boardNum);
    }

    int  InsPMDL_ResetSin(void* _ins_handle)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x0C0A, 8);//1814
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x0C0A, 8);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x0C0A, 8);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x0C0A, 8);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x0C0A, 8);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x0C0A, 8);
        }

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1182, 32);//2304
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1182, 32);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1182, 32);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1182, 32);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1182, 32);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1182, 32);
        }

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1200, 0);//2400
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1200, 0);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1200, 0);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1200, 0);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1200, 0);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1200, 0);
        }

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1240, 12);//2480
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1240, 12);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1240, 12);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1240, 12);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1240, 12);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1240, 12);
        }

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1280, 0);//2500
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1280, 0);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1280, 0);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1280, 0);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1280, 0);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1280, 0);
        }

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1180, 0);//2300
        }
        else if (LightSignInt == 20)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1180, 0);
        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1180, 0);
        }
        else if (LightSignInt == 40)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1180, 0);
        }
        else if (LightSignInt == 50)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1180, 0);
        }
        else if (LightSignInt == 60)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1180, 0);
        }

        return 0;
    }

    int  InsPMDL_InitialLightBrightness(void* _ins_handle)
    {
       
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;

        if (LightSignInt == 10)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1024, 1);//2048

        }
        else if (LightSignInt == 20)
        {

            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1024, 1);//2048

        }
        else if (LightSignInt == 30)
        {
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1024, 1);//2048
        }
        else if (LightSignInt == 40)
        {

            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1024, 1);//2048

        }
        else if (LightSignInt == 50)
        {

            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1024, 1);//2048

        }
        else if (LightSignInt == 60)
        {

            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x101A, 2047);//2034
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x101C, 2047);//2038
            GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1024, 1);//2048

        }

        return 0;

    }

    //                                                           int CamDeviceIndex[],Ins2D5LightDeviceAndLocalIP* LightDevice[],int OpenCameraNum, int OpenLightNum
    InsResult __cdecl InsPMDL_GetSystemHandle(void** ins_handle, InsPMDL_SystemInfo SystemInfo)//0429
    {
        if (SystemInfo.CamInfo.CamNum == 1 && SystemInfo.LightInfo.LightNum == 1)
        {

            *ins_handle = new InsPMDL_System();
            ((InsPMDL_System*)*ins_handle)->SystemCamera.CameraCount = 1;  //Ŀǰ��һ�������Ч
            ((InsPMDL_System*)*ins_handle)->SystemCamera.Camera->ins_camera = new InsCameraIkap();//һ����ʲô�������ʲô���ָ�룬�����ָ��=new���࣬���ǲ��ܸ���ָ��=new����,��ǿת�����࣬���ණ���ȸ���࣬���ò�������ĳ�Ա
            ((InsCameraIkap*)((InsPMDL_System*)*ins_handle)->SystemCamera.Camera->ins_camera)->mpCommunication = Ins_Modbus_udp::get_instance();

            int camIndex = SystemInfo.CamInfo.id[0];
            cout << "Initial Camera Info0527 :" << camIndex << endl;

            ((InsCameraIkap*)((InsPMDL_System*)*ins_handle)->SystemCamera.Camera->ins_camera)->ConnectIndexCamera(SystemInfo.CamInfo.id[0]);//CamDeviceIndex[0]//��Ҫ�޸� ����������id[0]
            //�����0622

            cout << "CamInfo.id[0]:" << SystemInfo.CamInfo.id[0] << endl;
            
            //0527�� test+1

            Ins2D5CameraLog::Ins2D5Logger::init();
            Ins2D5CameraLog::Ins2D5Logger::info("Load 2D5 SDK successfully.");


            ((InsPMDL_System*)*ins_handle)->SystemLight.LightCount = 1;
            ((InsPMDL_System*)*ins_handle)->SystemLight.light->ins_light = new InsPMDL_lightSouce(*SystemInfo.LightInfo.LightDeviceInfo[0]);//*LightDevice[0]


            ((InsPMDL_System*)*ins_handle)->diffuseGain = 1;
            ((InsPMDL_System*)*ins_handle)->normalGain = 1;
            ((InsPMDL_System*)*ins_handle)->shapeGain = 1;
            ((InsPMDL_System*)*ins_handle)->specualrGain = 1;
            
            const char* a = SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length;
            cout << "SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length:" << SystemInfo.LightInfo.LightDeviceInfo[0]->lightSouce.length << endl;
            int boardNumAsInt = std::atoi(a)/10;
            INS_PMDL_BoardNum boardNum = (INS_PMDL_BoardNum)boardNumAsInt;
            cout << "boardNumAsInt:"<<boardNumAsInt << endl;
            cout << "boardNum:"<<boardNum << endl;
            //((InsPMDL_System*)(ins_handle))->SystemLight.light->BoardNum = boardNum;
            ((InsPMDL_System*)*ins_handle)->SystemLight.light->BoardNum = boardNum;
            std::cout << " ((InsPMDL_System*)(ins_handle))->SystemLight.light->BoardNum: " << ((InsPMDL_System*)(*ins_handle))->SystemLight.light->BoardNum <<std:: endl;//>>> //������0510
            globalSystemMap[*ins_handle] = SystemInfo; // ��ϵͳ��Ϣ��ӵ� map ��
            return  (InsResult)((InsPMDL_lightSouce*)((InsPMDL_System*)*ins_handle)->SystemLight.light->ins_light)->init_communication();
        }
        else 
        {
            return (InsResult)-1;
            //һ�϶�
            *ins_handle = new InsPMDL_System();
            ((InsPMDL_System*)*ins_handle)->SystemCamera.CameraCount = 2;  //2�������Ч


        }
    }

    InsResult __cdecl InsPMDL_GetImage(void* ins_handle, InsPMDL_Buffer InputBuffer, InsPMDL_FusionBuffer* Output, INS_PMDL_ImageType ImageIndex)
    {
         uint32_t Confirm;
         uint32_t Num;
        ((InsPMDL_lightSouce*)((InsPMDL_System*)ins_handle)->SystemLight.light->ins_light)->readFromModbusOne(0x0001,Confirm);
    

        if (Confirm == 4804179)
        {
            ((InsPMDL_lightSouce*)((InsPMDL_System*)ins_handle)->SystemLight.light->ins_light)->readFromModbusOne(0x0C08, Num);//����ʹ��

            int cameraHeight = InsPMDL_GetHeight(ins_handle);
            int cameraWidth = InsPMDL_GetWidth(ins_handle);
            //int cameraFrameSize1 = InsPMDL_GetFrameSize((((InsPMDL_System*)ins_handle)->SystemCamera.Camera->ins_camera));
            std::cout << "height" << cameraHeight;
            int cameraFrameSize = cameraHeight * cameraWidth;
            if (Num == 1)//9��
            {
                void* toolPtr;
                uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize * sizeof(uint8_t));
                memcpy(l_img_buffer, InputBuffer.pdata, cameraFrameSize);
                InsImage srcImage[9];
                ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 9, srcImage);//1211
                CreateLumitraxToolPtr(toolPtr);
                SetLumitraxToolParam(toolPtr, ((InsPMDL_System*)ins_handle)->normalGain, ((InsPMDL_System*)ins_handle)->specualrGain, ((InsPMDL_System*)ins_handle)->diffuseGain, false, ((InsPMDL_System*)ins_handle)->shapeGain, 0);//1.0����ֵģʽ
                InsImage dstImage[5];//IR�ں�
                CreateLumitrxImage(toolPtr, srcImage, dstImage);


                //����ȥ
                if (ImageIndex >0)
                {
                    //Output->pdataX1 = (unsigned char*)malloc(cameraFrameSize);
                    //memcpy(Output->pdataX1, srcImage[0].data, cameraFrameSize);


                    unsigned char** pdata_ptrs[14] = 
                    {
                     &Output->pdataNormal,& Output->pdataSpecular,& Output->pdataDiffuse,&Output->pdataShape,&Output->pdataGlossratio,
                     &Output->pdataX1, &Output->pdataX2, &Output->pdataX3, &Output->pdataX4,
                     &Output->pdataY1, &Output->pdataY2, &Output->pdataY3, &Output->pdataY4,
                     &Output->pdataIR
                    };


                    //dstImage[5]
                    for (int i = 0; i < 5; i++)
                    {
                        if (ImageIndex & (1 << i))
                        {
                            *pdata_ptrs[i] = (unsigned char*)malloc(cameraFrameSize);
                            memcpy(*pdata_ptrs[i], dstImage[i].data, cameraFrameSize);


                        }
                    }

                    //srcImage[9]
                    for (int i = 5; i < 14; i++)
                    {
                        if (ImageIndex & (1 << i))
                        {
                            int srcIndex = i - 5;
                            if (srcIndex < 5)
                            {
                                *pdata_ptrs[i] = (unsigned char*)malloc(cameraFrameSize);
                                memcpy(*pdata_ptrs[i], srcImage[srcIndex].data, cameraFrameSize);
                            }
                        }
                    }

                }

                Output->height = srcImage->height;
                Output->width = srcImage->width;
                Output->step = srcImage->stride;

                //�ͷ�
                free(l_img_buffer);//�ͷ�
                l_img_buffer = NULL;
                //qDebug()<<"CCC";
                DestroyImageArray(srcImage, 8);
                DestroyImageArray(dstImage, 5);
                DestroyLumitraxToolPtr(toolPtr);
            }
            else if (Num == 0)//8��
            {
                void* toolPtr;
                uint8_t* l_img_buffer = (uint8_t*)malloc(cameraFrameSize * sizeof(uint8_t));
                memcpy(l_img_buffer, InputBuffer.pdata, cameraFrameSize);
                InsImage srcImage[8];
                ImageExtarction(l_img_buffer, cameraWidth, cameraHeight, cameraWidth, 8, srcImage);//1211
                CreateLumitraxToolPtr(toolPtr);
                SetLumitraxToolParam(toolPtr, ((InsPMDL_System*)ins_handle)->normalGain, ((InsPMDL_System*)ins_handle)->specualrGain, ((InsPMDL_System*)ins_handle)->diffuseGain, false, ((InsPMDL_System*)ins_handle)->shapeGain, 0);//1.0����ֵģʽ
                InsImage dstImage[5];//IR�ں�
                CreateLumitrxImage(toolPtr, srcImage, dstImage);

                //����ȥ
                if (ImageIndex > 0)
                {
                    //Output->pdataX1 = (unsigned char*)malloc(cameraFrameSize);
                    //memcpy(Output->pdataX1, srcImage[0].data, cameraFrameSize);


                    unsigned char** pdata_ptrs[13] =
                    {
                     &Output->pdataNormal,&Output->pdataSpecular,&Output->pdataDiffuse,&Output->pdataGlossratio ,& Output->pdataShape,
                     &Output->pdataX1, &Output->pdataX2, &Output->pdataX3, &Output->pdataX4,
                     &Output->pdataY1, &Output->pdataY2, &Output->pdataY3, &Output->pdataY4
                   
                    };

           
                  
                    /*unsigned char* pdata_ptr2 = NULL;

                    pdata_ptr2 = (unsigned char*)malloc(cameraFrameSize);*/

                    //memcpy(Output->pdataX1, srcImage[0].data, cameraFrameSize);

                    //std::cout << "here" << std::endl;


                    //dstImage[5] �ں�

                    for (int i = 0; i < 5; i++)
                    {
                        if (ImageIndex & (1 << i))
                        {

                            int dstIndex = i;
                            if (dstIndex <5)
                            {
                                free(*pdata_ptrs[i]);
                                *pdata_ptrs[i] = NULL;
                            }
                            *pdata_ptrs[i] = (unsigned char*)malloc(sizeof(unsigned char* )*(cameraFrameSize/8));

                            memcpy(*pdata_ptrs[i], dstImage[i].data, cameraFrameSize/8);
                        }
                    }

                    //srcImage[8] ��ͼ
                    for (int i = 5; i < 13; i++)
                    {
                        if (ImageIndex & (1 << i))
                        {
                            int srcIndex = i - 5;
                            std::cout << "srcIndex Num is :" << srcIndex << std::endl;
                            if (srcIndex < 8)
                            {

                                if (*pdata_ptrs[i] != nullptr) 
                                {
                                    free(*pdata_ptrs[i]);
                                    *pdata_ptrs[i] = NULL;
                                }
                                *pdata_ptrs[i] = (unsigned char*)malloc(sizeof(unsigned char*) * (cameraFrameSize/8));


                                if (*pdata_ptrs[i] != nullptr)
                                {
                                    std::cout << "here11" <<"scrImage.height:" << srcImage[srcIndex].height << "   " << "scrImage.width:" << srcImage[srcIndex].width << "   "<<"FrameSize: " << cameraFrameSize << std::endl;
                                    memcpy(*pdata_ptrs[i], srcImage[srcIndex].data, cameraFrameSize/8);
                                    std::cout << "here" << std::endl;
                                   
                                }
                            }
                        }
                    }
                }


                Output->height = srcImage->height;
                Output->width = srcImage->width;
                Output->step = srcImage->stride;

                free(l_img_buffer);//�ͷ�
                l_img_buffer = NULL;
                //qDebug()<<"CCC";
                DestroyImageArray(srcImage, 8);
                DestroyImageArray(dstImage, 5);
                DestroyLumitraxToolPtr(toolPtr);
            }

        }
        else
        {
            return (InsResult)-1;//�Ƿ�ϵͳ
        }

        return (InsResult)0;

    }

   InsResult __cdecl InsPMDL_SetTriggerMode(void* ins_handle ,INS_PMDL_TriggerModeSelect TriMode)
    {
  
       ((InsPMDL_System*)(ins_handle))->SystemLight.light->TriggerMode = TriMode; //��Դ��Ϣ
        return (InsResult)0;
    }

 

    //--------------------------------------------------

    //������������������������������������������������(����)����������������������������������������

    InsResult InsPMDL_SetScanningPeriod(void* _ins_handle , double _line_scanning_period, double& _line_fre, double& _exposure )//��Ƶ
    {


        int CamType;
        std::string name = GET_INS_CAMERA_HANDLE(_ins_handle)->get_cam_full_name();
        cout << "Full Name :" << name << endl;
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;

        uint32_t _picture_num;
        ((InsPMDL_lightSouce*)((InsPMDL_System*)_ins_handle)->SystemLight.light->ins_light)->readFromModbusOne(0x0C08, _picture_num);
        if (_picture_num == 0)
        {
            _picture_num = 8;
        }
        else if (_picture_num == 1)
        {
            _picture_num = 9;
        }

        int LightSignInt = boardNum * 10;
        cout << "LightSignInt SetScanPeriod:" << LightSignInt << endl;

        if (name.find("4K") != string::npos)
        {
            CamType = 0;
        }

        else if (name.find("8KCXP") != string::npos)
        {
            CamType = 1;
        }

        else if (name.find("16KCXP") != string::npos)
        {
            CamType = 2;
        }
        //------------------------------------------

      int ret = GET_INS_LIGHT_HANDLE(_ins_handle)->set_scanning_period(boardNum, _picture_num, _line_scanning_period, _line_fre, _exposure, CamType);
      double calculateNum;
      if (ret == 0)
      {
          //����෽���������ﴦ��

          if (CamType == 0)
          {
              calculateNum = _line_scanning_period / _picture_num;
          }
          else if (CamType == 1)
          {
              calculateNum = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate8kLinePeriod(_line_scanning_period / _picture_num);
          }
          else if (CamType == 2)
          {
              calculateNum = GET_INS_LIGHT_HANDLE(_ins_handle)->Calculate16kLinePeriod(_line_scanning_period / _picture_num);
          }
         
          std::cout << "calculateNum:" << calculateNum << std::endl;

          InsResult d =  GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(_exposure);

          InsResult e =  GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_line_period_time(calculateNum);//����calculat  P//_line_scanning_period/_picture_num 0408

          //InsResult d =  GET_INS_CAMERA_HANDLE(_ins_handle)->set_ins_exposure(_exposure);

          //------0513------------����Ƶ�ʸı�����

          double pf = 1000 / calculateNum;
        
          int value1 = 2212;
        
          if (pf <= 1) //20%
          {
              value1 = 1638;
          }
          else if (pf > 1 && pf < 40)  //22%
          {
              value1 = 1802;
          }
          else if (pf >= 40 && pf < 80) //25%
          {
              value1 = 2047;
          }
          else if (pf >= 80 && pf < 110)  //27%
          {
              value1 = 2212;
          }
          else if (pf >= 110 && pf < 150) //30%
          {
              value1 = 2457;
          }
          else if (pf >= 150 && pf < 170) //33%
          {
              value1 = 2703;
          } 
          else if (pf >= 170 && pf < 190) //35%
          {
              value1 = 2867;
          }
          else if (pf >= 190 && pf <= 200) //38%
          {
              value1 = 3113;
          }

          
          switch (LightSignInt) 
          {
          case 10:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1024, 1);
              break;
          case 20:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusTwo(0x1024, 1);
              break;
          case 30:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusThree(0x1024, 1);
              break;
          case 40:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFour(0x1024, 1);
              break;
          case 50:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusFive(0x1024, 1);
              break;
          case 60:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusSix(0x1024, 1);
              break;
          default:
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101A, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x101C, value1);
              GET_INS_LIGHT_HANDLE(_ins_handle)->writeToModbusOne(0x1024, 1);
              break;
          }

          //---------------------------------------------------------------------
          
      }
      return (InsResult)ret;
    }

    InsResult InsPMDL_GetScanningPeriod(void* _ins_handle, double& _line_scanning_period, double& _line_fre)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_scanning_period(_line_scanning_period, _line_fre);
    }


    //�����������������������������������������á���������������������������������������
    InsResult InsPMDL_SetABPhaseInversionOnOff(void* _ins_handle, InsPMDL_Switch _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_a_b_phase_inversion_on_off(_index);
    }
    InsResult InsPMDL_SetTriggerDirection(void* _ins_handle, int _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_trigger_direction(_index);
    }
    InsResult InsPMDL_SetEncoderTriggerMode(void* _ins_handle, int _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_encoder_trigger_mode(_index);
    }
    InsResult InsPMDL_SetFrequencyDivision(void* _ins_handle, uint32_t _frequency_division)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_frequency_division(_frequency_division);
    }
    InsResult InsPMDL_SetFrequencyMultiplier(void* _ins_handle, uint32_t _frequency_multiplier)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_frequency_multiplier(_frequency_multiplier);
    }
    InsResult InsPMDL_GetTriggerDirection(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_trigger_direction(_index);
    }
    InsResult InsPMDL_GetEncoderTriggerMode(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_encoder_trigger_mode(_index);
    }
    InsResult InsPMDL_GetFrequencyDivision(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_frequency_division(_index);
    }
    InsResult InsPMDL_GetFrequencyMultiplier(void* _ins_handle, uint32_t & _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_frequency_multiplier(_index);
    }
    //����������������������������������������IO�������á���������������������������������������
    InsResult InsPMDL_SetIOTriggerMode(void* _ins_handle, int _index)//ioio
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_mode(boardNum,_index);
    }
    InsResult InsPMDL_SetIOTriggerNum(void* _ins_handle,  uint32_t _trinum)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_num(boardNum, _trinum);
    }
    InsResult InsPMDL_SetIOPulseCount(void* _ins_handle, uint32_t _count)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_pulse_count(boardNum, _count);
    }
    InsResult InsPMDL_SetIOTriggerPulseWidth(void* _ins_handle,  uint32_t _width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_pulse_width(boardNum, _width);
    }
    InsResult InsPMDL_SetIOTriggerDelay(void* _ins_handle,  uint32_t _delay)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_delay(boardNum, _delay);
    }
    InsResult InsPMDL_SetIOTriggerPeriod(void* _ins_handle, uint32_t _period)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_period(boardNum, _period);
    }
    InsResult InsPMDL_SetIOTriggerInputPolarity(void* _ins_handle,  uint32_t _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        int LightSignInt = boardNum * 10;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_trigger_input_polarity(boardNum, _index);
    }
    InsResult InsPMDL_GetIOTriggerMode(void* _ins_handle, uint32_t & _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_mode(_index);
    }
    InsResult InsPMDL_GetIOTriggerNum(void* _ins_handle, uint32_t & _trinum)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_num(_trinum);
    }
    InsResult InsPMDL_GetIOPulseCount(void* _ins_handle, uint32_t & _count)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_pulse_count(_count);
    }
    InsResult InsPMDL_GetIOTriggerPulseWidth(void* _ins_handle, uint32_t & _width)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_pulse_width(_width);
    }
    InsResult InsPMDL_GetIOTriggerDelay(void* _ins_handle, uint32_t & _delay)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_delay(_delay);
    }
    InsResult InsPMDL_GetIOTriggerPeriod(void* _ins_handle, uint32_t & _period)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_period(_period);
    }
    InsResult InsPMDL_GetIOTriggerInputPolarity(void* _ins_handle, uint32_t & _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_trigger_input_polarity(_index);
    }


    //����������������������������������Դ�������á���������������������������������������

    InsResult InsPMDL_SetEmissionMode(void* _ins_handle, InsPMDL_LineMode LineMode)//���������޸�0429 0507boardnum
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        //std::cout << " SetEmissionMode2: " << ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum << std::endl;
        //cout << "SetEmiisionMode:" << boardNum << endl;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_emission_mode(LineMode,boardNum);
    }
    InsResult InsPMDL_GetEmissionMode(void* _ins_handle, int& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_emission_mode(_index);
    }
    InsResult InsPMDL_SetXStripeMovementDirection(void* _ins_handle,int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        cout << "XMOVE BOARDnum::"<<boardNum << endl;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_x_stripe_movement_direction(boardNum,_index);
    }
    InsResult InsPMDL_SetYStripeMovementDirection(void* _ins_handle,  int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_y_stripe_movement_direction(boardNum, _index);
    }
    InsResult InsPMDL_SetInitialPhaseInXDirection(void* _ins_handle, int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_initial_phase_in_x_direction(boardNum, _index);
    }
    InsResult InsPMDL_SetInitialPhaseInYDirection(void* _ins_handle,  int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_initial_phase_in_y_direction(boardNum, _index);
    }
    InsResult InsPMDL_GetXStripeMovementDirection(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_x_stripe_movement_direction(_index);
    }
    InsResult InsPMDL_GetYStripeMovementDirection(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_y_stripe_movement_direction(_index);
    }
    InsResult InsPMDL_GetInitialPhaseInXDirection(void* _ins_handle, uint32_t& _initial_phase)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_initial_phase_in_x_direction(_initial_phase);
    }
    InsResult InsPMDL_GetInitialPhaseInYDirection(void* _ins_handle, uint32_t& _initial_phase)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_initial_phase_in_y_direction(_initial_phase);
    }

    //��������������������������������������Դ�������á�����������������������������������

    //������������������������������������������Դ����ģʽ������������������������������
    InsResult InsPMDL_SecondaryTriggerEnableAndDisable(void* _ins_handle, double _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->secondary_trigger_enable_and_disable(boardNum, _index);
    }
    InsResult InsPMDL_SetEncoderSecondaryTriggerPulseWidth(void* _ins_handle,  double _encodewidth)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_encoder_secondary_trigger_pulse_width(boardNum, _encodewidth);
    }
    InsResult InsPMDL_SetEncoderSecondaryTriggerTravelDirection(void* _ins_handle,int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_encoder_secondary_trigger_travel_direction(boardNum, _index);
    }
    InsResult InsPMDL_SetSecondaryFrequencyDivision(void* _ins_handle, uint32_t _frequency_division)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_secondary_frequency_division(_frequency_division);
    }
    InsResult InsPMDL_SetSecondaryFrequencyMultiplier(void* _ins_handle, uint32_t _frequency_multiplier) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_secondary_frequency_multiplier(_frequency_multiplier);
    }

    //��������������������������������������������������������������������������������
    InsResult InsPMDL_SetSoftwareTriggerTriggerCount(void* _ins_handle, uint32_t _count)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_software_trigger_trigger_count(_count);
    }
    InsResult InsPMDL_SetSoftwareTriggerTriggerPeriod(void* _ins_handle, double _period)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_software_trigger_trigger_period(_period);
    }
    InsResult get_software_trigger_trigger_count(void* _ins_handle, uint32_t & _count)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_software_trigger_trigger_count(_count);
    }
    InsResult get_software_trigger_trigger_period(void* _ins_handle, double& _period)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_software_trigger_trigger_period(_period);
    }
    //������������������������������������IO������������������������������������������������
    InsResult InsPMDL_SetIoSecondaryTriggerMode(void* _ins_handle,  int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_mode(boardNum, _index);
    }
    InsResult InsPMDL_SetIoSecondaryTriggerNum(void* _ins_handle, uint32_t _trinum) 
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_num(boardNum, _trinum);
    }
    InsResult InsPMDL_SetIoSecondaryPulseCount(void* _ins_handle, uint32_t _count)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_pulse_count(boardNum, _count);
    }
    InsResult InsPMDL_SetIoSecondaryTriggerPulseWidth(void* _ins_handle, uint32_t _width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_pulse_width(boardNum, _width);
    }
    InsResult InsPMDL_SetIoSecondaryTriggerDelay(void* _ins_handle, uint32_t _delay)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_delay(boardNum, _delay);
    }
    InsResult InsPMDL_SetIoSecondaryTriggerPeriod(void* _ins_handle, uint32_t _period)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_period(boardNum, _period);
    }
    InsResult InsPMDL_SetIoSecondaryTriggerInputPolarity(void* _ins_handle,uint32_t _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_io_secondary_trigger_input_polarity(boardNum, _index);
    }

    InsResult InsPMDL_GetIoSecondaryTriggerMode(void* _ins_handle, uint32_t & _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_mode(_index);
    }
    InsResult InsPMDL_GetIoSecondaryTriggerNum(void* _ins_handle, uint32_t & _trinum)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_num(_trinum);
    }
    InsResult InsPMDL_GetIoSecondaryPulseCount(void* _ins_handle, uint32_t & _count)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_pulse_count(_count);
    }
    InsResult InsPMDL_GetIoSecondaryTriggerPulseWidth(void* _ins_handle, uint32_t & _width)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_pulse_width(_width);
    }
    InsResult InsPMDL_GetIoSecondaryTriggerDelay(void* _ins_handle, uint32_t & _delay) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_delay(_delay);
    }
    InsResult InsPMDL_GetIoSecondaryTriggerPeriod(void* _ins_handle, uint32_t & _period) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_period(_period);
    }
    InsResult InsPMDL_GetIoSecondaryTriggerInputPolarity(void* _ins_handle, uint32_t & _index) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_io_secondary_trigger_input_polarity(_index);
    }

    //��������������������������������Դ����ģʽ������������������������



    //��������������������������������ģʽ������������������������
    InsResult InsPMDL_ExtendedTriggerEnableAndDisable(void* _ins_handle,  double _index, int _quantity)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->extended_trigger_enable_and_disable(boardNum, _index, _quantity);
    }
    InsResult InsPMDL_SetExtendedTriggerOutputQuantity(void* _ins_handle, uint32_t _quantity)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_extended_trigger_output_quantity(boardNum, _quantity);
    }
    InsResult InsPMDL_SetExtendedTriggerOutputDelay(void* _ins_handle, uint32_t _delay)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_extended_trigger_output_delay(boardNum, _delay);
    }
    InsResult InsPMDL_SetExtendedTriggerOutputWidth(void* _ins_handle,uint32_t _width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_extended_trigger_output_width(boardNum, _width);
    }
    InsResult InsPMDL_SetExtendedTriggerOutputPolarity(void* _ins_handle, int _index)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_extended_trigger_output_polarity(boardNum, _index);
    }
    InsResult InsPMDL_GetExtendedTriggerOutputQuantity(void* _ins_handle, uint32_t& _quantity) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_extended_trigger_output_quantity(_quantity);
    }
    InsResult InsPMDL_GetExtendedTriggerOutputDelay(void* _ins_handle, uint32_t& _delay)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_extended_trigger_output_delay(_delay);
    }
    InsResult InsPMDL_GetExtendedTriggerOutputWidth(void* _ins_handle, uint32_t& _width)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_extended_trigger_output_width(_width);
    }
    InsResult InsPMDL_GetExtendedTriggerOutputPolarity(void* _ins_handle, uint32_t& _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_extended_trigger_output_polarity(_index);
    }
    //��������������������������������ģʽ������������������������


    //�����������������������������߼����á���������������������������
    InsResult InsPMDL_SetLightSourceResponseTime(void* _ins_handle,double _delay)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_light_source_response_time(boardNum, _delay);
    }
    InsResult InsPMDL_SetLightSourceTriggerOutputWidth(void* _ins_handle,  double _width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_light_source_trigger_output_width(boardNum, _width);
    }
    InsResult InsPMDL_SetLightSourceBrightnessAdjustment(void* _ins_handle,  uint32_t _percentage)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_light_source_brightness_adjustment(boardNum, _percentage);
    }
    InsResult InsPMDL_SetEncoderTriggerPulseWidth(void* _ins_handle,  double _encode_width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_encoder_trigger_pulse_width(boardNum, _encode_width);
    }
    InsResult InsPMDL_SetLightSourceTriggerOutputQuantity(void* _ins_handle, uint32_t _quantity)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_light_source_trigger_output_quantity(boardNum, _quantity);
    }
    InsResult InsPMDL_GetLightSourceResponseTime(void* _ins_handle,  double& _delay)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_light_source_response_time( _delay);
    }
    InsResult InsPMDL_GetLightSourceTriggerOutputWidth(void* _ins_handle,  double& _width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_light_source_trigger_output_width(boardNum, _width);
    }
    InsResult InsPMDL_GetLightSourceBrightnessAdjustment(void* _ins_handle, uint32_t & _percentage)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_light_source_brightness_adjustment(boardNum, _percentage);
    }
    InsResult InsPMDL_GetEncoderTriggerPulseWidth(void* _ins_handle, double& _encode_width)
    {
        int boardNum = ((InsPMDL_System*)(_ins_handle))->SystemLight.light->BoardNum;
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_encoder_trigger_pulse_width(boardNum, _encode_width);
    }
    InsResult InsPMDL_GetLightSourceTriggerOutputQuantity(void* _ins_handle, uint32_t & _quantity)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_light_source_trigger_output_quantity(_quantity);
    }
    //������������(��λ����)ֵ���룾0
    InsResult InsPMDL_SetSoftTriggerNum(void* _ins_handle, uint32_t _num)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_soft_trigger_num(_num);
    }
    //������������(��λ��΢��)
    InsResult InsPMDL_SetSoftTriggerPeriod(void* _ins_handle, double _period)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->set_soft_trigger_period(_period);
    }
    //��ȡ��������(��λ����)
    InsResult InsPMDL_GetSoftTriggerNum(void* _ins_handle, uint32_t & _num) 
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_soft_trigger_num(_num);
    }
    //��ȡ��������(��λ��΢��)
    InsResult InsPMDL_GetSoftTriggerPeriod(void* _ins_handle, double& _period)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->get_soft_trigger_period(_period);
    }


    //�����������������������������߼����á���������������������������



    //���������أ���1������0��
    InsResult InsPMDL_OpenOrCloseLasers(void* _ins_light, uint16_t buffer)
    {
        int ret = write_extern_laser1(_ins_light,&buffer);
        if (ret == 0)
        { 
            ret = write_extern_laser2(_ins_light,&buffer);
        }
        return (InsResult)ret;
    }

    //�����λ�ˣ�pitchΪǰ����
    int InsPMDL_GetCameraPitch(void* ins_light,double* buffer)
    { 
        return read_extern_pirch(ins_light, buffer);
    }
    //�����λ�ˣ�rollΪ���Ҹ���
    int InsPMDL_GetCameraRoll(void* ins_light, double* buffer)
    {
        return read_extern_roll(ins_light, buffer);
    }
    //�����λ�ˣ�rollΪˮƽ��ת��
    int InsPMDL_GetCameraYaw(void* ins_light, double* buffer)
    {
        return read_extern_yaw(ins_light, buffer);
    }
    //����Դλ�ˣ�pirchΪǰ����
    int InsPMDL_GetLightSourcePitch(void* ins_light, double* buffer)
    {
        return read_internl_pirch(ins_light, buffer);
    }
    //����Դλ�ˣ�rollΪ���Ҹ���
    int InsPMDL_GetLightSourceRoll(void* ins_light, double* buffer)
    {
        return read_internl_roll(ins_light, buffer);
    }
    //����Դλ�ˣ�rollΪˮƽ��ת��
    int InsPMDL_GetLightSourceYaw(void* ins_light, double* buffer)
    {
        return read_internl_yaw(ins_light, buffer);
    }
    int InsPMDL_GetFpgaTemp(void* _ins_handle, double* buffer1)
    {
        uint16_t buffer[2];
        //int abb = (int)GET_INS_LIGHT_HANDLE(_ins_handle)->ins_read(FIRSTBOARD, 0x0012, buffer);//0321
        int abb = (int)((InsPMDL_lightSouce*)_ins_handle)->ins_read(FIRSTBOARD, 0x0012, buffer);
        uint32_t FpgaTem = (static_cast<uint32_t>((buffer[0]) << 16) | buffer[1]);
        *buffer1 = ((FpgaTem * 503.975) / 4096) - 273.15;
        return abb;
    }

    int InsPMDL_GetBoardTemp(void* _ins_handle, double* buffer)
    {
        uint16_t buffer2[2];
        //int ret = (int)GET_INS_LIGHT_HANDLE(_ins_handle)->ins_read(FIRSTBOARD, 0x0900, buffer2);//West
        int ret = (int)((InsPMDL_lightSouce*)_ins_handle)->ins_read(FIRSTBOARD, 0x0900, buffer2);//West
        uint32_t BoardTem = (static_cast<uint32_t>((buffer2[0]) << 16) | buffer2[1]);
        //12λ0��1��  5-11λΪ����λ  1-4С��λ  С��λ��*0.0625
        BoardTem &= 0x0FFF;

        bool isNegative = BoardTem & (1 << 11);

        int integerPart = (BoardTem >> 4) & 0x7F;

        double decimalPart = (BoardTem & 0x0F) * 0.0625;

        double totalTemperature = integerPart + decimalPart;
        if (isNegative)
        {
            totalTemperature = -totalTemperature;
        }
        *buffer = totalTemperature;
        return ret;
    }



    //��������и�
    int ins_save_camera_height(void* ins_light,uint32_t *buffer)
    {
        //return ((InsPMDL_lightSouce*)ins_light)->save_camera_height(buffer);
        return (int)GET_INS_LIGHT_HANDLE(ins_light)->save_camera_height(buffer);//0522
    }
    int ins_read_camera_height(void* ins_light,uint32_t *buffer)
    {
        //return ((InsPMDL_lightSouce*)ins_light)->read_camera_height(buffer);
        return (int)GET_INS_LIGHT_HANDLE(ins_light)->read_camera_height(buffer);//0522
    }

    //-------------

        //------------0514-------------------

    int ins_save_camera_heightTemp(void* ins_light, uint32_t* buffer)
    {
        return (int)GET_INS_LIGHT_HANDLE(ins_light)->save_camera_heightTemp(buffer);
    }
    int ins_read_camera_heightTemp(void* ins_light, uint32_t* buffer)
    {
        return (int)GET_INS_LIGHT_HANDLE(ins_light)->read_camera_heightTemp(buffer);
    }



    //-------------0514-�����и�����-----------------

    int InsPMDL_SaveCameraHeight(void* ins_light, uint32_t* buffer)
    {
        GET_INS_LIGHT_HANDLE(ins_light)->writeToModbusOne(0x10, 2345);
        GET_INS_LIGHT_HANDLE(ins_light)->save_camera_heightTemp(buffer);

        return 0;

    }

    int InsPMDL_GetCameraHeight(void* ins_light, uint32_t* buffer)
    {
        uint32_t value;
        GET_INS_LIGHT_HANDLE(ins_light)->readFromModbusOne(0x10, value);
        if (value == 2863289685)//AAAA5555
        {

            GET_INS_LIGHT_HANDLE(ins_light)->read_camera_height(buffer);
            std::cout << "Height is  not temp :" << *buffer << std::endl;
            std::cout << "value:" << value << std::endl;
            return 0;

        }
        else
        {
            GET_INS_LIGHT_HANDLE(ins_light)->read_camera_heightTemp(buffer);

            std::cout << "Height is  temp :" << *buffer << std::endl;
            std::cout << "value:" << value << std::endl;
            return 0;
        }

        return 0;
    }


    //-----------------------------------------------



    //------------


    InsResult InsPMDL_InitCommunication(void* _ins_handle, const char* _ip)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->init_communication();
    }

    InsResult InsPMDL_ReadRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t& _buffer)
    {
        if (strcmp(_input, "INSNEX") == 0)
        {
            //return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->read_register((BoardID)_id, _address/2, _buffer);
            return (InsResult)((InsPMDL_lightSouce*)_ins_handle)->read_register((BoardID)_id, _address / 2, _buffer);
        }
        else
        {
            return InsResult::INSUFFICIENT_COMMUNICATION_PRIVILEGES;
        }
    }

    InsResult InsPMDL_WriteRegister(void* _ins_handle, const char* _input, int _id, uint16_t _address, uint32_t _buffer)
    {
        if (strcmp(_input, "INSNEX") == 0)
        {
            cout << "INSNEX" << endl;
            //return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->write_register((BoardID)_id, _address/2, _buffer);
            return (InsResult)((InsPMDL_lightSouce*)_ins_handle)->write_register((BoardID)_id, _address / 2, _buffer);
        }
        else
        {
            return InsResult::INSUFFICIENT_COMMUNICATION_PRIVILEGES;
        }
    }


    //==================0304�������ӿ�===================================================
    InsResult  write_fpga_reg(void* _ins_handle, const char* _input, uint32_t index, uint32_t _addr, uint32_t _value)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->write_fpga_reg(  index, _addr, _value);//���²���һ��

    }


    InsResult InsPMDLight_SetIOTriggerCycle(void* _ins_handle, uint32_t _index, double _time_ms)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetIOTriggerCycle(_index, _time_ms);
    }


    InsResult InsPMDLight_SetIOTriggerNum(void* _ins_handle, uint32_t _index, int _num)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetIOTriggerNum(_index, _num);
    }

    InsResult InsPMDLight_SetIOTriggerEn(void* _ins_handle, uint32_t _index, bool _enable)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetIOTriggerEn(_index, _enable);
    }

    InsResult InsPMDLight_SetIlluminationTime(void* _ins_handle, uint32_t _index, double _time)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetIlluminationTime(_index, _time);
    }


    InsResult InsPMDLight_SetLightMode(void* _ins_handle, uint32_t _index, int index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetLightMode(_index, index);
    }

    InsResult InsPMDLight_SetFreeRunOn(void* _ins_handle, uint32_t _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetFreeRunOn(_index);
    }

    InsResult InsPMDLight_SetFreeRunOff(void* _ins_handle, uint32_t _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetFreeRunOff(_index);
    }


    InsResult InsPMDLight_Trigger_8_ImagesOn(void* _ins_handle, uint32_t index, uint32_t _time, uint32_t _delaytime)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_Trigger_8_ImagesOn(index,_time,_delaytime);
    }


    InsResult InsPMDLight_Trigger_8_ImagesOff(void* _ins_handle, uint32_t index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_Trigger_8_ImagesOff(index);
    }

    InsResult InsPMDLight_SetBrightness(void* _ins_handle, uint32_t _index, uint32_t _percentage)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetBrightness(_index, _percentage);
    }


    InsResult InsPMDLight_SetIlluminationTime10ms(void* _ins_handle, uint32_t index, double _time)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetIlluminationTime10ms(index, _time);
    }

    InsResult InsPMDLight_SetArrayEn(void* _ins_handle, uint32_t _index, bool _enable)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetArrayEn(_index, _enable);
    }

    InsResult InsPMDLight_SetFirstLightProperty(void* _ins_handle, uint32_t _index)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetFirstLightProperty(_index);
    }

    InsResult InsPMDLight_Set8LightProperty(void* _ins_handle, uint32_t _index, uint32_t _illuminate_mode)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_Set8LightProperty(_index, _illuminate_mode);
    }


    InsResult  InsPMDLight_Initial(void* _ins_handle, int BoardNum)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_Initial(BoardNum);
    }

    InsResult  InsPMDLight_SetDirection(void* _ins_handle, int _BoardNum, int _Direction)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetDirection(_BoardNum, _Direction);
    }


    InsResult InsPMDLight_SetPhaseOffset(void* _ins_handle, uint32_t _index, int _Direction, int LightMode)
    {
        return (InsResult)GET_INS_LIGHT_HANDLE(_ins_handle)->Ins_SetPhaseOffset(_index, _Direction,LightMode);
    }

    


}

