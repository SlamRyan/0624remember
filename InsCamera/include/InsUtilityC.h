#pragma once
#include <string>

#ifdef INSUTILITY_EXPORTS
#define INSUTILITY_API __declspec(dllexport)
#else
#define INSUTILITY_API __declspec(dllimport)
#endif



/*
struct INSUTILITY_API Device {
public:
	char ip[16];                    //192.168.1.11
	char manufacturer_name[8];		//INS
	char model_name[8];				//2D5
	uint16_t device_id;				//65535
	char device_vision[16];			//0.0.0.1
	char net_mask[16];				//255.255.255.0
	char gateway[16];				//192.168.1.1
	char mac_addr[18];				//0c:12:2b:56:0b:450
};
*/

class INSUTILITY_API Device {
public:
	char manufacturer_name[8];            //公司："INS"
public:
	char model_name[8];                   //项目名："2D5"
public:
	char device_model[16];                //设备型号" INS-CHVS-363-10GH"
public:
	char length[8];                       //光源板长度："30"
public:
	char SN[16];                          //设备条形码号：" A0100041000"
public:
	char device_vision[16];               //软件版本号:"2"
public:
	char ip_addr[16];                     //ip地址:"192.168.1.11"
public:
	char net_mask[16];                    //子网掩码："255.255.255.0"
public:
	char gateway[16];                     //网关"192.168.1.1"
public:
	char mac_addr[18];					  //mac地址："0c:12:2b:56:0b:45"
};

//初始化本机设备地址
extern "C" INSUTILITY_API void InitLocalInfo(const char* mip, const char* mnet_mask, const char* mgateway);
extern "C" INSUTILITY_API int SearchDevice(Device* ins_device);

//return -1 means that the ip is wrong,return 0 means success
extern "C" INSUTILITY_API int InsResetDeviceID(const char* mip);
//-1 failure，返回的Device内为当前设备信息
extern "C" INSUTILITY_API int factory_configuration(Device * ins_device);//11.18新增出厂配置，需要先把配置设置进device 类型,如果设置失败，返回的device 类型里字符串为空

//保存配置文件 //11.20新增
extern "C" INSUTILITY_API int save_config_file(int boardcount, const char* _config_filename);
//加载配置文件
extern "C" INSUTILITY_API int load_config_file(int _board_num, const char* _config_filename);

//设备IP
extern "C" INSUTILITY_API int InitModbusUdp(const char* mip);
extern "C" INSUTILITY_API int InsModbusUdpRead(int id, uint16_t address, uint16_t * buffer);
extern "C" INSUTILITY_API int InsModbusUdpWrite(int id,uint16_t address, uint16_t* buffer);


/*
//system
extern "C" INSUTILITY_API int write_reg_soft_reset(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_test(int id, uint16_t * buffer);

//DAC
extern "C" INSUTILITY_API int read_reg_mos_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dds_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dds_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_ack_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_ack_time(int id, uint16_t * buffer);
//system
extern "C" INSUTILITY_API int read_COMPANY(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_DEVICE(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_DEVICE_SUB(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_INTERFACETYPE(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_YEAR(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_HOUR(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_REV(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_soft_reset(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_test(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_device_temp(int id, uint16_t * buffer);
/*
extern "C" INSUTILITY_API int write_reg_phy_rst(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_iic_whrl(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_iic_dev_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_iic_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_iic_wdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_iic_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mcu1_cfg_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mcu1_cfg_value(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mcu1_cfg_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mcu1_bps(int id, uint16_t * buffer);

//laser
extern "C" INSUTILITY_API int write_reg_laser_status(int id, uint16_t * buffer);
//trig cfg
extern "C" INSUTILITY_API int write_reg_soft_trigger_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_soft_trigger_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_soft_trigger_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_width(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_delay(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_pulse(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_polar(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_phase(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_cnt_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_dis_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_ignore(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_div(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_width(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_multi_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_multi_coe(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_clr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_encoder_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_slave_device(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_status_cnt_clr(int id, uint16_t * buffer);
//trig_out
extern "C" INSUTILITY_API int write_reg_exp_chan(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_exposure_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_cnt_max(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_multi_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_polar(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_4(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_4(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_5(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_5(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_6(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_6(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_7(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_7(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_8(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_8(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_9(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_9(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_10(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_10(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_start_11(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_led_pwm_end_11(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_0(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_0(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_1(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_1(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_2(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_2(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_3(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_3(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_4(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_4(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_start_5(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_end_5(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int write_reg_trig_out_polar(int id, uint16_t* buffer);

// Loop Mode
extern "C" INSUTILITY_API int write_reg_dds_phase(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dds_inc(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_exp_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_gap(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_pic_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_status(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_shift(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_loop_gap(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_loop_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dds_phase_offset(int id, uint16_t * buffer);

// Test Mode
extern "C" INSUTILITY_API int write_reg_dac_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_dac_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_sw_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_mos_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_trigger_req_y3(int id, uint16_t * buffer);

// core ctrl
extern "C" INSUTILITY_API int write_reg_core_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_core_mode(int id, uint16_t * buffer);

// Wave List
extern "C" INSUTILITY_API int write_reg_ram_cfg_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_ram_whrl(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_ram_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_ram_wdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_ram_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_wave_start_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_wave_end_addr(int id, uint16_t * buffer);

// Adjust
extern "C" INSUTILITY_API int write_reg_adjust_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_adjust_gain(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int write_reg_adjust_offset(int id, uint16_t * buffer);


// READ
// phy
//extern "C" INSUTILITY_API int read_reg_phy_rst(int id, uint16_t * buffer);

// temp
extern "C" INSUTILITY_API int read_reg_mainboard_temp(int id, uint16_t * buffer);

// MPU0
extern "C" INSUTILITY_API int read_reg_iic_whrl(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_dev_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_wdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_rdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_iic_done(int id, uint16_t * buffer);

extern "C" INSUTILITY_API int read_reg_laser_status(int id, uint16_t * buffer);

// MPU1
extern "C" INSUTILITY_API int read_reg_mcu1_angle_x(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_angle_y(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_angle_z(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_temp(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_cfg_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_cfg_value(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_cfg_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_cfg_done(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mcu1_bps(int id, uint16_t * buffer);

// trig_cfg
extern "C" INSUTILITY_API int read_reg_soft_trigger_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_soft_trigger_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_soft_trigger_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_width(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_delay(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_pulse(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_polar(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_phase(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_cnt_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_dis_mode(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_ignore(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_div(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_width(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_location(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_multi_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_multi_coe(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_a_cnt(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_b_cnt(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_clr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_encoder_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_slave_device(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_status_cnt_clr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_exp_chan(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_exposure_time(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_cnt_max(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_multi_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_polar(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_4(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_4(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_5(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_5(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_6(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_6(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_7(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_7(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_8(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_8(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_9(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_9(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_10(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_10(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_start_11(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_led_pwm_end_11(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_0(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_0(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_1(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_1(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_2(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_2(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_3(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_3(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_4(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_4(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_start_5(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_end_5(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_trig_out_polar(int id, uint16_t* buffer);


//loop mode
extern "C" INSUTILITY_API int read_reg_dds_phase(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dds_inc(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_exp_cycle(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_gap(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_pic_num(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_status(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_shift(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_loop_gap(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_sw_loop_num(int id, uint16_t* buffer);
extern "C" INSUTILITY_API int read_reg_dds_phase_offset(int id, uint16_t* buffer);

//test mode
extern "C" INSUTILITY_API int read_reg_dac_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_x0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_x1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_x2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_x3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_y0(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_y1(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_y2(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_dac_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_sw_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_mos_value_y3(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_trigger_req_y3(int id, uint16_t * buffer);
//core ctrl
extern "C" INSUTILITY_API int read_reg_core_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_core_mode(int id, uint16_t * buffer);
//wave list
extern "C" INSUTILITY_API int read_reg_ram_cfg_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_whrl(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_wdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_rdata(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_req(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_ram_done(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_wave_start_addr(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_wave_end_addr(int id, uint16_t * buffer);
//wave adjust
extern "C" INSUTILITY_API int read_reg_adjust_en(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_adjust_gain(int id, uint16_t * buffer);
extern "C" INSUTILITY_API int read_reg_adjust_offset(int id, uint16_t * buffer);
*/