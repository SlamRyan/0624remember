#pragma once
#include "inscp_udp.h"
#include "ins_global.h"
#ifdef _MSC_VER
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
#else
#include <pcap.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "device.h"
#include "winsock.h"
#include <iostream>
#include <fstream> 



class  INSCAMERA_API modbus_udp
{

public:
    bool err{};
    int err_no{};
    std::string error_msg;

    modbus_udp() = default;
    ~modbus_udp() = default;
    //modbus_udp的初始化
    int initmodbus_udp(const char* host, const char* local_ip, uint16_t port = 502);//host——光源IP，local_ip——本机ip
    //自定义的协议初始化
    int init_ins_modbus_udp(const char* host, const char* local_ip, uint16_t port = 65530);//host——光源IP，local_ip——本机ip
    void modbus_udp_close() const;
    int modbus_udp_write_single_register(BoardID id, uint16_t address,  uint16_t* buffer);
    int modbus_udp_write_registers(BoardID id, uint16_t address, uint16_t amount, const uint16_t* value);
    int modbus_udp_read_registers(BoardID id, uint16_t address, uint16_t amount, uint16_t* buffer);
    int update_udp_write_registers(BoardID id, uint16_t address, uint16_t amount, const uint16_t* value);//在线升级专用
    bool is_connected() const { return _connected; }
	int set_timeout(int _time);

private:
    bool _connected{};
    uint16_t PORT{};
    static __int32 tran_num;
    int _slaveid{};
    std::string HOST;
    int timeout_ = 1000;
    int len = sizeof(SOCKADDR);
    SOCKET ClistSock = {};
    SOCKADDR_IN SrvAddr;
    SOCKADDR_IN CliAddr;

#ifdef _WIN32
    WSADATA wsadata;
#endif

    //检查返回码是否错误码
    int modbuserror_handle(unsigned char* msg, int func);
    int continous_receive(uint8_t(&send_data)[SENDBUFFERNUM], uint16_t* buffer, int func);
};

//INSCAMERA_API void PrintTime();

class INSCAMERA_API InsPMDL_lightSouce : public modbus_udp
{
public:


    InsPMDL_lightSouce(Ins2D5LightDeviceAndLocalIP _device) :mDevice(_device) {};
    ~InsPMDL_lightSouce() = default;


	//----------0315变量
	uint32_t MOSACKZ;
	uint32_t DDSACKZ;
	uint32_t PicNumZ;
	uint32_t SWGAPZ;
	uint32_t TriigerGapZ;
	uint32_t SWLoopNumZ;
	uint32_t CamTriNumZ;

	//----------
    InsResult init_communication();
    //----------读写单个寄存器------------//0304读写
    InsResult ins_write(BoardID id, uint16_t address, uint16_t* buffer);
    InsResult ins_read(BoardID id, uint16_t address, uint16_t* buffer);
    InsResult read_register(BoardID _id, uint16_t _address, uint32_t& _buffer);//在前两个的基础上拼好的32位
    InsResult write_register(BoardID _id, uint16_t _address, uint32_t _buffer);
	//—————读写多个寄存器------------
	InsResult write_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer);
	InsResult read_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer);//只支持读一个或者两个


	InsResult save_SN(uint16_t* _buffer);
	InsResult read_SN(uint16_t* _buffer);


#if  1





	double Calculate16kLinePeriod(double x);
	double Calculate8kLinePeriod(double x);

    //-------------------------------工具----------------------
    int readFromModbusOne(uint16_t regAdd, uint32_t& value);
	void writeToModbusSix(uint16_t regAdd, uint32_t value);//6
    void writeToModbusFive(uint16_t regAdd, uint32_t value);//5
	void writeToModbusThree(uint16_t regAdd, uint32_t value);//3;
	void writeToModbusFour(uint16_t regAdd, uint32_t value);//4
    void writeToModbusOne(uint16_t regAdd, uint32_t value);//1
	void writeToModbusTwo(uint16_t regAdd, uint32_t value);//2
	void writeToModbusSecondThird(uint16_t regAdd, uint32_t value);

    int fpga_device_initialization(int _board_num);

	int get_other_info(OtherDeviceInfo* _stOtherInfo);


	InsResult ins_write_Laser(BoardID id, uint16_t address, uint16_t* buffer); //0419 激光雷达

	uint16_t getTriggerFre(int modeIndex);
	//自由取向关的时候要按照帧数计算行频
	InsResult FreeRunForFocus(int _boardnum, int _picture_num, int _index,int CamType);

	//----------位姿选择------------
	int select_direction(int index ,int _borad_num);//

	//----------触发设置------------
    //设置当前系统扫描周期_line_scanning_period(单位：微秒),和获取对应的行频_line_fre(单位：KHz),行扫描周期应大于曝光时间+2,输入范围（4~100）
    //自动计算相机曝光给出并设置
	int set_scanning_period(int _board_num, int _picture_um, double _line_scanning_period, double& _line_fre, double& _TriExposure,int CamType);
	//获得当前系统扫描周期_line_scanning_period(单位：微秒)和行频_line_fre(单位：KHz)
    int get_scanning_period(double& _line_scanning_period, double& _line_fre);
	//设置触发模式，触发使能开，在相机开始取图后调用使能
	int set_trigger_mode_on(int _index);
	//触发使能关
	int set_trigger_mode_off();

    //----------编码器触发设置------------
    //编码器校验，A-B相取反1开、0关
    int set_a_b_phase_inversion_on_off(int _index);
    //触发方向,0-单向行程，1-回退跟踪，2-回退忽略，3-双向扫描
    int set_trigger_direction(int _index);
    //编码器触发模式，0-A相上升沿，单向计数，1-A相上升沿，双向计数，1-A相双边沿，双向计数，A&B相双边沿，双向
    int set_encoder_trigger_mode(int _index);
    //设置分频
    int set_frequency_division(uint32_t _frequency_division);
    //设置倍频
    int set_frequency_multiplier(uint32_t _frequency_multiplier);

    //获取触发方向,0-单向行程，1-回退跟踪，2-回退忽略，3-双向扫描
    int get_trigger_direction(uint32_t& _index);
    //获取编码器触发模式，0-A相上升沿，单向计数，1-A相上升沿，双向计数，1-A相双边沿，双向计数，A&B相双边沿，双向
    int get_encoder_trigger_mode(uint32_t& _index);
    //获取分频
    int get_frequency_division(uint32_t& _index);
    //获取倍频
    int get_frequency_multiplier(uint32_t& _index);
    //----------IO触发设置------------

    //设置io触发模式
    //0-模式1：IO“0”开始，IO“1”结束,
    //1-模式2：IO“1”开始，IO“0”结束,
    //2-模式3：IO“0”开始，输出固定触发脉冲,
    //3-模式4：IO“1”开始，输出固定触发脉冲,
    //4-模式5：IO“0”有效，上升沿有效，下降沿结束,
    //5-模式6：IO“1”有效，上升沿有效，下降沿结束,
    //6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
    //7-模式8：多个IO"1"作为开始，输出固定触发脉冲
    int set_io_trigger_mode(int _board_num, int _index);
    //设置io触发数量
    int set_io_trigger_num(int _board_num, uint32_t _trinum);
    //设置io脉冲数量
    int set_io_pulse_count(int _board_num, uint32_t _count);
    //设置io触发脉冲宽度
    int set_io_trigger_pulse_width(int _board_num, uint32_t _width);
    //设置io触发延迟
    int set_io_trigger_delay(int _board_num, uint32_t _delay);
    //设置io触发周期
    int set_io_trigger_period(int _board_num, uint32_t _period);
    //设置io触发输入极性,0-保持，1-反转
    int set_io_trigger_input_polarity(int _board_num, uint32_t _index);


    /*获取io触发模式,
    0-模式1：IO“0”开始，IO“1”结束,
    1-模式2：IO“1”开始，IO“0”结束,
    2-模式3：IO“0”开始，输出固定触发脉冲,
    3-模式4：IO“1”开始，输出固定触发脉冲,
    4-模式5：IO“0”有效，上升沿有效，下降沿结束,
    5-模式6：IO“1”有效，上升沿有效，下降沿结束,
    6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
    7-模式8：多个IO"1"作为开始，输出固定触发脉冲
    */
    int get_io_trigger_mode(uint32_t& _index);
    //获取io触发数量
    int get_io_trigger_num(uint32_t& _trinum);
    //获取io脉冲数量
    int get_io_pulse_count(uint32_t& _count);
    //获取io触发脉冲宽度
    int get_io_trigger_pulse_width(uint32_t& _width);
    //获取io触发延迟
    int get_io_trigger_delay(uint32_t& _delay);
    //获取io触发周期
    int get_io_trigger_period(uint32_t& _period);
    //获取io触发输入极性，0-保持，1-反转
    int get_io_trigger_input_polarity(uint32_t& _index);

    //----------光源条纹设置------------
    //设置光源发光模式，0-极大模式，1-大模式，2-标准模式，3-小模式，4-极小模式(只有三块板的情况)
    int set_emission_mode(int _index, int _board_num);
    //获取当前光源发光模式0-极大，1-大，2-标准，3-小，4-极小
    int get_emission_mode(int& _index);
    //设置X条纹移动方向,0-正向，1-反向
    int set_x_stripe_movement_direction(int _boardnum, int _index);
    //设置Y条纹移动方向,0-正向，1-反向
    int set_y_stripe_movement_direction(int _boardnum, int _index);
    //设置X方向起始相位,0-0°，1-90°，2-180°，3-270°
    int set_initial_phase_in_x_direction(int _boardnum, int _index);
    //设置Y方向起始相位,0-0°，1-90°，2-180°，3-270°
    int set_initial_phase_in_y_direction(int _boardnum, int _index);

    //获取X条纹移动方向,0-正向，1-反向
    int get_x_stripe_movement_direction(uint32_t& _index);
    //获取Y条纹移动方向,0-正向，1-反向
    int get_y_stripe_movement_direction(uint32_t& _index);
    //获取X方向起始相位
    int get_initial_phase_in_x_direction(uint32_t& _initial_phase);
    //获取Y方向起始相位
    int get_initial_phase_in_y_direction(uint32_t& _initial_phase);
    //----------光源条纹设置------------

    //--------------二级触发光源触发模式--------------
    //二级触发使能和停止使能,_index:0-disable,1-enable
    int secondary_trigger_enable_and_disable(int _boardnum, double _index);

    //设置编码器二级触发脉宽(单位：微秒)
    int set_encoder_secondary_trigger_pulse_width(int _boardnum, double _encodewidth);
    //触发方向,0-单向行程，1-回退跟踪(反向不计)，2-回退忽略，3-双向扫描
    int set_encoder_secondary_trigger_travel_direction(int _boardnum, int _index);
    //to do  没找到编码器触发模式
    //设置二级触发分频
    int set_secondary_frequency_division(uint32_t _frequency_division);
    //设置二级触发分频
    int set_secondary_frequency_multiplier(uint32_t _frequency_multiplier);
    //------------
    //设置软触发触发次数
    int set_software_trigger_trigger_count(uint32_t _count);
    //设置触发周期
    int set_software_trigger_trigger_period(double _period);
    //获得软触发触发次数
    int get_software_trigger_trigger_count(uint32_t& _count);
    //获得触发周期
    int get_software_trigger_trigger_period(double& _period);

    //-------IO触发-----
    /*设置io触发模式,
    0-模式1：IO“0”开始，IO“1”结束,
    1-模式2：IO“1”开始，IO“0”结束,
    2-模式3：IO“0”开始，输出固定触发脉冲,
    3-模式4：IO“1”开始，输出固定触发脉冲,
    4-模式5：IO“0”有效，上升沿有效，下降沿结束,
    5-模式6：IO“1”有效，上升沿有效，下降沿结束,
    6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
    7-模式8：多个IO"1"作为开始，输出固定触发脉冲
    */
    int set_io_secondary_trigger_mode(int _board_num, int _index);
    //设置io触发数量
    int set_io_secondary_trigger_num(int _board_num, uint32_t _trinum);
    //设置io脉冲数量
    int set_io_secondary_pulse_count(int _board_num, uint32_t _count);
    //设置io触发脉冲宽度
    int set_io_secondary_trigger_pulse_width(int _board_num, uint32_t _width);
    //设置io触发延迟
    int set_io_secondary_trigger_delay(int _board_num, uint32_t _delay);
    //设置io触发周期
    int set_io_secondary_trigger_period(int _board_num, uint32_t _period);
    //设置io触发输入极性,0-保持，1-反转
    int set_io_secondary_trigger_input_polarity(int _board_num, uint32_t _index);

    /*获取io触发模式,
    0-模式1：IO“0”开始，IO“1”结束,
    1-模式2：IO“1”开始，IO“0”结束,
    2-模式3：IO“0”开始，输出固定触发脉冲,
    3-模式4：IO“1”开始，输出固定触发脉冲,
    4-模式5：IO“0”有效，上升沿有效，下降沿结束,
    5-模式6：IO“1”有效，上升沿有效，下降沿结束,
    6-模式7：多个IO"0"作为开始，输出固定触发脉冲,
    7-模式8：多个IO"1"作为开始，输出固定触发脉冲
    */
    int get_io_secondary_trigger_mode(uint32_t& _index);
    //获取io触发数量
    int get_io_secondary_trigger_num(uint32_t& _trinum);
    //获取io脉冲数量
    int get_io_secondary_pulse_count(uint32_t& _count);
    //获取io触发脉冲宽度
    int get_io_secondary_trigger_pulse_width(uint32_t& _width);
    //获取io触发延迟
    int get_io_secondary_trigger_delay(uint32_t& _delay);
    //获取io触发周期
    int get_io_secondary_trigger_period(uint32_t& _period);
    //获取io触发输入极性，0-保持，1-反转
    int get_io_secondary_trigger_input_polarity(uint32_t& _index);
    //--------------二级触发光源触发模式--------------




    //----------红外模式------------
    // 红外使能和停止使能,_index:0-disable,1-enable,_quantity:扩展出发输出数量,一般为1（单位：次）
    int extended_trigger_enable_and_disable(int _boardnum, double _index, int _quantity);
    //设置扩展触发输出数量(单位：次)
    int set_extended_trigger_output_quantity(int _board_num, uint32_t _quantity);
    //设置扩展触发输出延迟(单位：微秒)
    int set_extended_trigger_output_delay(int _board_num, uint32_t _delay);
    //设置扩展触发输出宽度(单位：微秒)
    int set_extended_trigger_output_width(int _board_num, uint32_t _width);
    //设置扩展触发输出极性,0-上升沿，1-下降沿
    int set_extended_trigger_output_polarity(int _board_num, int _index);

    //获取扩展触发输出数量(单位：次)
    int get_extended_trigger_output_quantity(uint32_t& _quantity);
    //获取扩展触发输出延迟(单位：微秒)
    int get_extended_trigger_output_delay(uint32_t& _delay);
    //获取扩展触发输出宽度(单位：微秒)
    int get_extended_trigger_output_width(uint32_t& _width);
    //获取扩展触发输出极性,0-上升沿，1-下降沿
    int get_extended_trigger_output_polarity(uint32_t& _index);
    //----------红外模式------------



    //----------高级设置------------
    //设置光源响应时间(单位：微秒)值必须＞0
    int set_light_source_response_time(int _board_num, double _delay);
    //设置光源触发宽度(单位：微秒)值必须<=系统行扫描周期的一半
    int set_light_source_trigger_output_width(int _board_num, double _width);
    //设置光源亮度调节(单位：%),亮度输入应为整数 , 范围：1~100
    int set_light_source_brightness_adjustment(int _board_num, uint32_t _percentage);
    //设置编码器触发脉宽(单位：微秒)
    int set_encoder_trigger_pulse_width(int _board_num, double _encode_width);
    //设置光源触发输出数量(单位：次)
    int set_light_source_trigger_output_quantity(int _board_num, uint32_t _quantity);

    //获取光源响应时间(单位：微秒)
    int get_light_source_response_time(double& _delay);
    //获取光源触发宽度(单位：微秒)
    int get_light_source_trigger_output_width(int _board_num, double& _width);
    //获取光源亮度调节(单位：%),输出为整数 , 范围：1~100
    int get_light_source_brightness_adjustment(int _board_num, uint32_t& _percentage);
    //获取编码器触发脉宽(单位：微秒)
    int get_encoder_trigger_pulse_width(int _board_num, double& _encode_width);
    //获取光源触发输出数量(单位：次)
    int get_light_source_trigger_output_quantity(uint32_t& _quantity);


    //设置软触发次数(单位：次)值必须＞0
    int set_soft_trigger_num(uint32_t _num);
    //设置软触发周期(单位：微秒)
    int set_soft_trigger_period(double _period);

    //获取软触发次数(单位：次)
    int get_soft_trigger_num(uint32_t& _num);
    //获取软触发周期(单位：微秒)
    int get_soft_trigger_period(double& _period);

    //----------高级设置------------
#endif //  0


//读取版本号
	int get_microblaze_version(uint32_t * buffer);

	// 保存配置，返回值为if_config_in_device_ok(int board_num)的返回值
	int save_config_in_device(int board_num);
	//读取是否配置成功
	int if_config_in_device_ok(int board_num);
	//激光器开关，置1开，置0关
	int write_extern_laser1(uint16_t * buffer);
	int write_extern_laser2(uint16_t * buffer);
	//读激光器当前值
	int read_extern_laser1(uint16_t * buffer);
	int read_extern_laser2(uint16_t * buffer);
	//读相机位姿，pirch为前向俯仰
	int read_extern_pirch(double* buffer);
	//读相机位姿，roll为左右俯仰
	int read_extern_roll(double* buffer);
	//读相机位姿，roll为水平旋转角
	int read_extern_yaw(double* buffer);
	//读光源位姿，pirch为前向俯仰
	int read_internl_pirch(double* buffer);
	//读光源位姿，roll为左右俯仰
	int read_internl_roll(double* buffer);
	//读光源位姿，roll为水平旋转角
	int read_internl_yaw(double* buffer);
	//保存相机行高
	int save_camera_height(uint32_t * buffer);
	int read_camera_height(uint32_t * buffer);

	//临时保存相机行高
	int save_camera_heightTemp(uint32_t* buffer);
	int read_camera_heightTemp(uint32_t* buffer);//0523



#if 1
	InsResult ins_write_reg_soft_reset(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_test(BoardID id, uint32_t * buffer);

	InsResult ins_read_COMPANY(BoardID id, uint32_t * buffer);
	InsResult ins_read_DEVICE(BoardID id, uint32_t * buffer);
	InsResult ins_read_DEVICE_SUB(BoardID id, uint32_t * buffer);
	InsResult ins_read_INTERFACETYPE(BoardID id, uint32_t * buffer);
	InsResult ins_read_YEAR(BoardID id, uint32_t * buffer);
	InsResult ins_read_HOUR(BoardID id, uint32_t * buffer);
	InsResult ins_read_REV(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_soft_reset(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_test(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_device_temp(BoardID id, uint32_t * buffer);
	//DAC
	InsResult ins_read_reg_mos_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dds_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_ack_time(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_ack_time(BoardID id, uint32_t * buffer);
	//phy
	//InsResult ins_write_reg_phy_rst(BoardID id,uint32_t* buffer);

	//temp
	//InsResult ins_write_reg_mainboard_temp(BoardID id,uint32_t* buffer);
	//laser
	InsResult ins_write_reg_laser_status(BoardID id, uint32_t * buffer);
	/*
	//mpu0
	InsResult ins_write_reg_iic_whrl(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_dev_addr(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_addr(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_wdata(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_rdata(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_req(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_iic_done(BoardID id,uint32_t* buffer);
	//mpu1
	InsResult ins_write_reg_mcu1_angle_x(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_angle_y(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_angle_z(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_temp(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_mcu1_bps(BoardID id,uint32_t* buffer);
	*/
	//trig_cfg
	InsResult ins_write_reg_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_soft_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_soft_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_width(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_delay(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_pulse(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_polar(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_phase(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_dis_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_ignore(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_div(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_width(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_encoder_location(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_encoder_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_multi_coe(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_encoder_a_cnt(BoardID id,uint32_t* buffer);
	//InsResult ins_write_reg_encoder_b_cnt(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_encoder_clr(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_slave_device(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_status_cnt_clr(BoardID id, uint32_t * buffer);


	//11.23新增寄存器
	InsResult ins_write_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_width(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_delay(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_polar(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_phase(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_div(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_width(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_l1_encoder_location(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_clr(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_encoder_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer);

	InsResult ins_write_reg_trigger_level(BoardID id, uint32_t * buffer);//地址不能加结构体，地址写死



	//trig_out
	InsResult ins_write_reg_exp_chan(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_exposure_time(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_cnt_max(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_polar(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_6(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_6(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_7(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_7(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_8(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_8(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_9(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_9(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_10(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_10(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_start_11(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_led_pwm_end_11(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_start_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_end_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trig_out_polar(BoardID id, uint32_t * buffer);
	//READ
	// phy
	//InsResult ins_read_reg_phy_rst(BoardID id,uint32_t* buffer);
	//temp
	InsResult ins_read_reg_mainboard_temp(BoardID id, uint32_t * buffer);
	//LASER
	InsResult ins_read_reg_laser_status(BoardID id, uint32_t * buffer);
	/*
   //mpu0
   InsResult ins_read_reg_iic_whrl(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_dev_addr(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_addr(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_wdata(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_rdata(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_req(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_iic_done(BoardID id,uint32_t* buffer);
   //mpu1
   InsResult ins_read_reg_mcu1_angle_x(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_angle_y(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_angle_z(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_temp(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer);
   InsResult ins_read_reg_mcu1_bps(BoardID id,uint32_t* buffer);
   */
   //trig_cfg
	InsResult ins_read_reg_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_soft_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_soft_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_width(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_delay(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_pulse(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_polar(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_phase(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_dis_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_ignore(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_div(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_width(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_location(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_multi_coe(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_a_cnt(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_b_cnt(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_clr(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_slave_device(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_status_cnt_clr(BoardID id, uint32_t * buffer);

	//1123新增
	InsResult ins_read_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_width(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_delay(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_polar(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_trigger_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_phase(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_div(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_width(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_location(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_clr(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_encoder_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer);
	//地址写死
	InsResult ins_read_reg_trigger_level(BoardID id, uint32_t * buffer);




	//trig_out
	InsResult ins_read_reg_exp_chan(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_exposure_time(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_cnt_max(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_multi_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_polar(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_6(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_6(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_7(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_7(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_8(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_8(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_9(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_9(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_10(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_10(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_start_11(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_led_pwm_end_11(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_start_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_end_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trig_out_polar(BoardID id, uint32_t * buffer);
	//loop_mode
	InsResult ins_write_reg_dds_phase(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dds_inc(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_exp_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_gap(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_pic_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_status(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_shift(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_loop_gap(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_loop_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dds_phase_offset(BoardID id, uint32_t * buffer);
	//11.23新增

	InsResult ins_write_reg_dds_direction_x(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_dds_direction_y(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_current_offset(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_dac_value_forward(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_dac_value_backward(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_sw_wait(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_camera_delay(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_camera_cycle(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_dac_req(BoardID id, uint32_t* buffer);

	InsResult ins_write_reg_dds_phase_y(BoardID id, uint32_t* buffer);


	//test
	InsResult ins_write_reg_dac_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_x0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_x1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_x2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_x3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_y0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_y1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_y2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_trigger_req_y3(BoardID id, uint32_t * buffer);
	//core
	InsResult ins_write_reg_core_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_core_mode(BoardID id, uint32_t * buffer);
	//step forward
	InsResult ins_write_reg_step_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_pic_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_x_seq(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_y_seq(BoardID id, uint32_t * buffer);



	//loop_mode
	InsResult ins_read_reg_dds_phase(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_inc(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_exp_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_gap(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_pic_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_status(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_shift(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_loop_gap(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_loop_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_phase_offset(BoardID id, uint32_t * buffer);
	//11.23新增
	InsResult ins_read_reg_dds_direction_x(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_direction_y(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_current_offset(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_forward(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_backward(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_wait(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_camera_delay(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_camera_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_req(BoardID id, uint32_t * buffer);
	//写死地址
	InsResult ins_read_reg_dds_phase_y(BoardID id, uint32_t * buffer);





	//test
	InsResult ins_read_reg_dac_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_x0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_x0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_x1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_x1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_x2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_x2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_x3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_x3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_y0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_y0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_y1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_y1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_y2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_y2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_value_y3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_trigger_req_y3(BoardID id, uint32_t * buffer);
	//core
	InsResult ins_read_reg_core_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_core_mode(BoardID id, uint32_t * buffer);

	//step forward
	InsResult ins_read_reg_step_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_pic_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_x_seq(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_y_seq(BoardID id, uint32_t * buffer);

	//step phase read
	InsResult ins_read_reg_step_phase_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_6(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_7(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_8(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_9(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_10(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_11(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_12(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_13(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_14(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_15(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_16(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_17(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_18(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_19(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_20(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_21(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_22(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_23(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_24(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_25(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_26(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_27(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_28(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_29(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_30(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_phase_31(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_6(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_7(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_8(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_9(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_10(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_11(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_12(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_13(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_14(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_15(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_16(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_17(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_18(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_19(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_20(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_21(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_22(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_23(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_24(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_25(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_26(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_27(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_28(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_29(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_30(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_inc_31(BoardID id, uint32_t * buffer);

	//step phase write
	InsResult ins_write_reg_step_phase_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_6(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_7(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_8(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_9(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_10(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_11(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_12(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_13(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_14(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_15(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_16(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_17(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_18(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_19(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_20(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_21(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_22(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_23(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_24(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_25(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_26(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_27(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_28(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_29(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_30(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_phase_31(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_6(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_7(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_8(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_9(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_10(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_11(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_12(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_13(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_14(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_15(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_16(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_17(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_18(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_19(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_20(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_21(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_22(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_23(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_24(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_25(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_26(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_27(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_28(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_29(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_30(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_inc_31(BoardID id, uint32_t * buffer);

	//step base read
	InsResult ins_read_reg_step_base_0(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_1(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_2(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_3(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_4(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_5(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_6(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_7(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_8(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_9(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_10(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_11(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_12(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_13(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_14(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_15(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_16(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_17(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_18(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_19(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_20(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_21(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_22(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_23(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_24(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_25(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_26(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_27(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_28(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_29(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_30(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_step_base_31(BoardID id, uint32_t * buffer);

	//step base write
	InsResult ins_write_reg_step_base_0(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_1(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_2(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_3(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_4(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_5(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_6(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_7(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_8(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_9(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_10(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_11(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_12(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_13(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_14(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_15(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_16(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_17(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_18(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_19(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_20(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_21(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_22(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_23(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_24(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_25(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_26(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_27(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_28(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_29(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_30(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_step_base_31(BoardID id, uint32_t * buffer);

	//wave_list
	InsResult ins_write_reg_ram_cfg_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_ram_whrl(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_ram_addr(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_ram_wdata(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_ram_rdata(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_ram_req(BoardID id, uint32_t * buffer);
	//InsResult ins_write_reg_ram_done(BoardID id,uint32_t* buffer);
	InsResult ins_write_reg_wave_start_addr(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_wave_end_addr(BoardID id, uint32_t * buffer);
	//adjust
	InsResult ins_write_reg_adjust_en(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_adjust_gain(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_adjust_offset(BoardID id, uint32_t * buffer);

	//wave_list
	InsResult ins_read_reg_ram_cfg_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_whrl(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_addr(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_wdata(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_rdata(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_req(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_ram_done(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_wave_start_addr(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_wave_end_addr(BoardID id, uint32_t * buffer);
	//adjust
	InsResult ins_read_reg_adjust_en(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_adjust_gain(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_adjust_offset(BoardID id, uint32_t * buffer);
	//num_check
	InsResult ins_write_reg_num_check_clr(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_io_in_0_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_io_in_1_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_a_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_encoder_b_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_io_out_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_mos_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dds_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dds_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_dac_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sw_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_reg_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_reg_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_write_reg_sum_err_num(BoardID id, uint32_t * buffer);
	//miss_check
	InsResult ins_write_reg_core_status(BoardID id, uint32_t * buffer);
	//num_check
	InsResult ins_read_reg_num_check_clr(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_io_in_0_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_io_in_1_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_a_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_encoder_b_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_io_out_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_mos_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_reg_req_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_reg_ack_num(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sum_err_num(BoardID id, uint32_t * buffer);

	//miss_check
	InsResult ins_read_reg_core_status(BoardID id, uint32_t * buffer);

#endif


	

	int write_file(const char* ch, int boardcount, std::ofstream* writeFile, uint16_t* buffer, uint16_t addr);

	//保存和加载配置文件
	int save_config_file(int _boardcount, const char* _config_filename);
	int LoadRegisters(BoardID id, uint16_t addr, uint16_t* buffer);
	//如果是一块板子保存和加载都是1.3S
	int load_config_file(int _board_num, const char* _config_filename);


	int FreeRunSinToContinue(int boardNum);





    //----------面阵需求------------ //0304




	//============0304
	// 
	// 
	// //光源初始化
	int Ins_Initial(int BoardNum);



	// 写入寄存器 （16位）
	int write_fpga_reg(int32_t index, uint32_t _addr, uint32_t _value);

	// 设置面阵光源IO触发周期
	int Ins_SetIOTriggerCycle(uint32_t _index, double _time_ms);

	// 设置面阵光源IO触发数量
	int Ins_SetIOTriggerNum(uint32_t _index, int _num);

	// 设置面阵光源IO触发使能
	int Ins_SetIOTriggerEn(uint32_t _index, bool _enable);

	//光源点亮时间(单位：ms)
	int Ins_SetIlluminationTime(uint32_t _index, double _time);

	//面阵发光模式 0极大,1大，2标准，3小，4极小
	int Ins_SetLightMode(uint32_t _index, int index);

	//自由取像开：  
	int Ins_SetFreeRunOn(uint32_t _index);

	//自由取像关：  
	int Ins_SetFreeRunOff(uint32_t _index);

	//触发（8张）开：
	int Ins_Trigger_8_ImagesOn(int BoardNum, uint32_t _time, uint32_t delaytime);

	//触发（8张）关：
	int Ins_Trigger_8_ImagesOff(int BoardNum);

	//设置光源亮度调节(单位：%),亮度输入应为整数 , 范围：1~100，和线扫一致
	int Ins_SetBrightness(uint32_t _index, uint32_t _percentage);

	// 为降低平均功耗与发热 设置光源点亮时间为固定10ms//以下为光源点亮时间10ms下对应参数计算及配置
	int Ins_SetIlluminationTime10ms(uint32_t index, double _time);

	//设置面阵模式使能
	int Ins_SetArrayEn(uint32_t _index, bool _enable);


	//设置面阵属性——仅第一张
	int Ins_SetFirstLightProperty(uint32_t _index);

	//设置面阵属性——全八张
	int Ins_Set8LightProperty(uint32_t _index, uint32_t _illuminate_mode);

	//设置发光顺序（方向）
	int Ins_SetDirection(int _BoardNum, int _Direction);


	//光源相位补偿  //_index光原版序号,_Direction角度,LightMode发光模式
    int Ins_SetPhaseOffset(uint32_t _index, int _Direction, int LightMode);
	





private:
    Ins2D5LightDeviceAndLocalIP mDevice;
};






//原来的单例模式
class INSCAMERA_API Ins_Modbus_udp : public modbus_udp
{
public:
    ~Ins_Modbus_udp() = default;
    static Ins_Modbus_udp* get_instance();
    int ins_modbus_udp_write(BoardID id, uint16_t address, uint16_t* buffer);
    int ins_modbus_udp_read(BoardID id, uint16_t address, uint16_t* buffer);

private:
    Ins_Modbus_udp() = default;
    static Ins_Modbus_udp* mpIns_Modbus_udp;
};


class INSCAMERA_API Update_Modbus_udp : public modbus_udp
{
public:
    ~Update_Modbus_udp() = default;
    static Update_Modbus_udp* get_instance();
    int update_udp_write(BoardID id, uint16_t address, uint16_t reg_num, uint16_t* buffer);

private:
    Update_Modbus_udp() = default;
    static Update_Modbus_udp* mpUpdate_Modbus_udp;
};

