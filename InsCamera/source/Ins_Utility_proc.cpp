
#include <iostream>
#include <fstream>
#include <chrono>
//#include <ctime>
#include <iomanip>


#include "..\include\Ins_Utility_proc.h"
#include "..\include\modbus_udp.h"

#define BUF_SIZE 128
using namespace std;

/*
Ins_Modbus* Ins_Modbus::mpIns_Modbus = nullptr;
*/
Ins_Modbus_udp* Ins_Modbus_udp::mpIns_Modbus_udp = nullptr;
Update_Modbus_udp* Update_Modbus_udp::mpUpdate_Modbus_udp = nullptr;

#define OFFSET

#ifdef OFFSET
#define ADDR_OFFSET_HALVE
#ifdef ADDR_OFFSET_HALVE
#define SYSTEM_ADDR 0x00000000
//#define DEVICE_PHY_ADDR 0x00001000
#define DEVICE_DAC_ADDR 0x00000880
#define DEVICE_temp_ADDR 0x00000900
//#define DEVICE_MPU0_ADDR 0x00001300
#define DEVICE_laser_ADDR 0x00000A00
//#define DEVICE_MPU1_ADDR 0x00001500
#define DEVICE_trig_cfg_ADDR 0x00000B00
#define DEVICE_trig_out_ADDR 0x00000B80
#define CONTROLLER_loop_mode_ADDR 0x00001000
#define CONTROLLER_test_mode_ADDR 0x00001080
#define CONTROLLER_core_ctrl_ADDR 0x00001100

#define CONTROLLER_step_forward_ADDR 0x00001180
#define CONTROLLER_step_phase_ADDR 0x00001200
#define CONTROLLER_step_base_ADDR 0x00001280

#define WAVE_wave_list_ADDR 0x00001800
#define WAVE_wave_adjust_ADDR 0x00001880
#define CHECK_num_check_ADDR 0x00003800
#define CHECK_miss_check_ADDR 0x00003880


struct  SYSTEM
{
    uint16_t COMPANY;
    uint16_t DEVICE;
    uint16_t DEVICE_SUB;
    uint16_t INTERFACETYPE;
    //uint4_t PIXELTYPE;
    //uint4_t DPITYPE;
    //uint4_t SPEEDTYPE;
    uint16_t YEAR;
    //uint4_t MONTH;
    //uint4_t DATE;
    uint8_t HOUR;
    uint8_t MINUTE;
    uint16_t REV;
    uint16_t reg_soft_reset;
    uint16_t reg_test;
    uint16_t reg_device_temp;
};


struct DEVICE_DAC
{
    uint16_t reg_mos_ack_time;
    uint16_t reg_dds_ack_time;
    uint16_t reg_dac_ack_time;
    uint16_t reg_sw_ack_time;
};



struct DEVICE_trig_cfg
{
    uint16_t reg_soft_trigger_cycle;
    uint16_t reg_soft_trigger_num;
    uint16_t reg_soft_trigger_en;
    uint16_t reg_trigger_cycle;
    uint16_t reg_trigger_num;
    uint16_t reg_trigger_mode;
    uint16_t reg_trigger_width;
    uint16_t reg_trigger_delay;
    uint16_t reg_trigger_pulse;
    uint16_t reg_trigger_polar;
    uint16_t reg_trigger_en;
    uint16_t reg_encoder_phase;
    uint16_t reg_encoder_cnt_mode;
    uint16_t reg_encoder_dis_mode;
    uint16_t reg_encoder_ignore;
    uint16_t reg_encoder_div;
    uint16_t reg_encoder_width;
    uint16_t reg_encoder_location;
    uint16_t reg_encoder_multi_en;
    uint16_t reg_encoder_multi_coe;
    uint16_t reg_encoder_a_cnt;
    uint16_t reg_encoder_b_cnt;
    uint16_t reg_encoder_clr;
    uint16_t reg_encoder_en;
    uint16_t reg_slave_device;
    uint16_t reg_status_cnt_clr;
    uint16_t reg_NULL0;//空寄存器
    uint16_t reg_NULL1;//空寄存器
    uint16_t reg_NULL2;//空寄存器
    uint16_t reg_NULL3;//空寄存器
    uint16_t reg_NULL4;//空寄存器
    uint16_t reg_NULL5;//空寄存器
    uint16_t reg_l1_soft_trigger_cycle;
    uint16_t reg_l1_soft_trigger_num;
    uint16_t reg_l1_soft_trigger_en;
    uint16_t reg_l1_trigger_cycle;
    uint16_t reg_l1_trigger_num;
    uint16_t reg_l1_trigger_mode;
    uint16_t reg_l1_trigger_width;
    uint16_t reg_l1_trigger_delay;
    uint16_t reg_l1_trigger_pulse;
    uint16_t reg_l1_trigger_polar;
    uint16_t reg_l1_trigger_en;
    uint16_t reg_l1_encoder_phase;
    uint16_t reg_l1_encoder_cnt_mode;
    uint16_t reg_l1_encoder_dis_mode;
    uint16_t reg_l1_encoder_ignore;
    uint16_t reg_l1_encoder_div;
    uint16_t reg_l1_encoder_width;
    uint16_t reg_l1_encoder_location;
    uint16_t reg_l1_encoder_multi_en;
    uint16_t reg_l1_encoder_multi_coe;
    uint16_t reg_l1_encoder_a_cnt;
    uint16_t reg_l1_encoder_b_cnt;
    uint16_t reg_l1_encoder_clr;
    uint16_t reg_l1_encoder_en;
    uint16_t reg_l1_status_cnt_clr;

};


struct DEVICE_trig_out
{
    uint16_t reg_exp_chan;
    uint16_t reg_exposure_time;
    uint16_t reg_led_cnt_max;
    uint16_t reg_trigger_multi_en;
    uint16_t reg_led_polar;
    uint16_t reg_led_pwm_start_0;
    uint16_t reg_led_pwm_end_0;
    uint16_t reg_led_pwm_start_1;
    uint16_t reg_led_pwm_end_1;
    uint16_t reg_led_pwm_start_2;
    uint16_t reg_led_pwm_end_2;
    uint16_t reg_led_pwm_start_3;
    uint16_t reg_led_pwm_end_3;
    uint16_t reg_led_pwm_start_4;
    uint16_t reg_led_pwm_end_4;
    uint16_t reg_led_pwm_start_5;
    uint16_t reg_led_pwm_end_5;
    uint16_t reg_led_pwm_start_6;
    uint16_t reg_led_pwm_end_6;
    uint16_t reg_led_pwm_start_7;
    uint16_t reg_led_pwm_end_7;
    uint16_t reg_led_pwm_start_8;
    uint16_t reg_led_pwm_end_8;
    uint16_t reg_led_pwm_start_9;
    uint16_t reg_led_pwm_end_9;
    uint16_t reg_led_pwm_start_10;
    uint16_t reg_led_pwm_end_10;
    uint16_t reg_led_pwm_start_11;
    uint16_t reg_led_pwm_end_11;
    uint16_t reg_trig_start_0;
    uint16_t reg_trig_end_0;
    uint16_t reg_trig_start_1;
    uint16_t reg_trig_end_1;
    uint16_t reg_trig_start_2;
    uint16_t reg_trig_end_2;
    uint16_t reg_trig_start_3;
    uint16_t reg_trig_end_3;
    uint16_t reg_trig_start_4;
    uint16_t reg_trig_end_4;
    uint16_t reg_trig_start_5;
    uint16_t reg_trig_end_5;
    uint16_t reg_trig_out_polar;

};


struct CONTROLLER_loop_mode
{
    uint16_t reg_dds_phase;
    uint16_t reg_dds_inc;
    uint16_t reg_exp_cycle;
    uint16_t reg_trigger_gap;
    uint16_t reg_pic_num;
    uint16_t reg_sw_status;
    uint16_t reg_sw_shift;
    uint16_t reg_sw_loop_gap;
    uint16_t reg_sw_loop_num;
    uint16_t reg_dds_phase_offset;
    uint16_t reg_dds_direction_x;
    uint16_t reg_dds_direction_y;
    uint16_t reg_current_offset;
    uint16_t reg_dac_value_forward;
    uint16_t reg_dac_value_backward;
    uint16_t reg_sw_wait;
    uint16_t reg_camera_delay;
    uint16_t reg_camera_cycle;
    uint16_t reg_dac_req;

};


struct CONTROLLER_test_mode
{
    uint16_t reg_dac_value_x0;
    uint16_t reg_sw_value_x0;
    uint16_t reg_mos_value_x0;
    uint16_t reg_trigger_req_x0;
    uint16_t reg_dac_value_x1;
    uint16_t reg_sw_value_x1;
    uint16_t reg_mos_value_x1;
    uint16_t reg_trigger_req_x1;
    uint16_t reg_dac_value_x2;
    uint16_t reg_sw_value_x2;
    uint16_t reg_mos_value_x2;
    uint16_t reg_trigger_req_x2;
    uint16_t reg_dac_value_x3;
    uint16_t reg_sw_value_x3;
    uint16_t reg_mos_value_x3;
    uint16_t reg_trigger_req_x3;
    uint16_t reg_dac_value_y0;
    uint16_t reg_sw_value_y0;
    uint16_t reg_mos_value_y0;
    uint16_t reg_trigger_req_y0;
    uint16_t reg_dac_value_y1;
    uint16_t reg_sw_value_y1;
    uint16_t reg_mos_value_y1;
    uint16_t reg_trigger_req_y1;
    uint16_t reg_dac_value_y2;
    uint16_t reg_sw_value_y2;
    uint16_t reg_mos_value_y2;
    uint16_t reg_trigger_req_y2;
    uint16_t reg_dac_value_y3;
    uint16_t reg_sw_value_y3;
    uint16_t reg_mos_value_y3;
    uint16_t reg_trigger_req_y3;
};


struct CONTROLLER_core_ctrl
{
    uint16_t reg_core_en;
    uint16_t reg_core_mode;
};

struct CONTROLLER_step_forward
{
    uint16_t reg_step_en;
    uint16_t reg_step_pic_num;
    uint16_t reg_step_x_seq;
    uint16_t reg_step_y_seq;
};
//11.24
struct CONTROLLER_step_phase
{
    uint16_t reg_step_phase_0;
    uint16_t reg_step_phase_1;
    uint16_t reg_step_phase_2;
    uint16_t reg_step_phase_3;
    uint16_t reg_step_phase_4;
    uint16_t reg_step_phase_5;
    uint16_t reg_step_phase_6;
    uint16_t reg_step_phase_7;
    uint16_t reg_step_phase_8;
    uint16_t reg_step_phase_9;
    uint16_t reg_step_phase_10;
    uint16_t reg_step_phase_11;
    uint16_t reg_step_phase_12;
    uint16_t reg_step_phase_13;
    uint16_t reg_step_phase_14;
    uint16_t reg_step_phase_15;
    uint16_t reg_step_phase_16;
    uint16_t reg_step_phase_17;
    uint16_t reg_step_phase_18;
    uint16_t reg_step_phase_19;
    uint16_t reg_step_phase_20;
    uint16_t reg_step_phase_21;
    uint16_t reg_step_phase_22;
    uint16_t reg_step_phase_23;
    uint16_t reg_step_phase_24;
    uint16_t reg_step_phase_25;
    uint16_t reg_step_phase_26;
    uint16_t reg_step_phase_27;
    uint16_t reg_step_phase_28;
    uint16_t reg_step_phase_29;
    uint16_t reg_step_phase_30;
    uint16_t reg_step_phase_31;
    uint16_t reg_step_inc_0;
    uint16_t reg_step_inc_1;
    uint16_t reg_step_inc_2;
    uint16_t reg_step_inc_3;
    uint16_t reg_step_inc_4;
    uint16_t reg_step_inc_5;
    uint16_t reg_step_inc_6;
    uint16_t reg_step_inc_7;
    uint16_t reg_step_inc_8;
    uint16_t reg_step_inc_9;
    uint16_t reg_step_inc_10;
    uint16_t reg_step_inc_11;
    uint16_t reg_step_inc_12;
    uint16_t reg_step_inc_13;
    uint16_t reg_step_inc_14;
    uint16_t reg_step_inc_15;
    uint16_t reg_step_inc_16;
    uint16_t reg_step_inc_17;
    uint16_t reg_step_inc_18;
    uint16_t reg_step_inc_19;
    uint16_t reg_step_inc_20;
    uint16_t reg_step_inc_21;
    uint16_t reg_step_inc_22;
    uint16_t reg_step_inc_23;
    uint16_t reg_step_inc_24;
    uint16_t reg_step_inc_25;
    uint16_t reg_step_inc_26;
    uint16_t reg_step_inc_27;
    uint16_t reg_step_inc_28;
    uint16_t reg_step_inc_29;
    uint16_t reg_step_inc_30;
    uint16_t reg_step_inc_31;
};

//11.24
struct CONTROLLER_step_base
{
    uint16_t reg_step_base_0;
    uint16_t reg_step_base_1;
    uint16_t reg_step_base_2;
    uint16_t reg_step_base_3;
    uint16_t reg_step_base_4;
    uint16_t reg_step_base_5;
    uint16_t reg_step_base_6;
    uint16_t reg_step_base_7;
    uint16_t reg_step_base_8;
    uint16_t reg_step_base_9;
    uint16_t reg_step_base_10;
    uint16_t reg_step_base_11;
    uint16_t reg_step_base_12;
    uint16_t reg_step_base_13;
    uint16_t reg_step_base_14;
    uint16_t reg_step_base_15;
    uint16_t reg_step_base_16;
    uint16_t reg_step_base_17;
    uint16_t reg_step_base_18;
    uint16_t reg_step_base_19;
    uint16_t reg_step_base_20;
    uint16_t reg_step_base_21;
    uint16_t reg_step_base_22;
    uint16_t reg_step_base_23;
    uint16_t reg_step_base_24;
    uint16_t reg_step_base_25;
    uint16_t reg_step_base_26;
    uint16_t reg_step_base_27;
    uint16_t reg_step_base_28;
    uint16_t reg_step_base_29;
    uint16_t reg_step_base_30;
    uint16_t reg_step_base_31;
};

struct WAVE_wave_list
{
    uint16_t reg_ram_cfg_en;
    uint16_t reg_ram_whrl;
    uint16_t reg_ram_addr;
    uint16_t reg_ram_wdata;
    uint16_t reg_ram_rdata;
    uint16_t reg_ram_req;
    uint16_t reg_ram_done;
    uint16_t reg_wave_start_addr;
    uint16_t reg_wave_end_addr;
};

//弃用
struct WAVE_wave_adjust
{
    uint16_t reg_adjust_en;
    uint16_t reg_adjust_gain;
    uint16_t reg_adjust_offset;
};


struct CHECK_num_check
{
    uint16_t reg_num_check_clr;
    uint16_t reg_io_in_0_num;
    uint16_t reg_io_in_1_num;
    uint16_t reg_encoder_a_num;
    uint16_t reg_encoder_b_num;
    uint16_t reg_io_out_num;
    uint16_t reg_mos_req_num;
    uint16_t reg_mos_ack_num;
    uint16_t reg_dds_req_num;
    uint16_t reg_dds_ack_num;
    uint16_t reg_dac_req_num;
    uint16_t reg_dac_ack_num;
    uint16_t reg_sw_req_num;
    uint16_t reg_sw_ack_num;
    uint16_t reg_reg_req_num;
    uint16_t reg_reg_ack_num;
    uint16_t reg_sum_err_num;

};


struct CHECK_miss_check
{
    uint8_t reg_core_status;
};



#else

#define SYSTEM_ADDR 0x00000000
//#define DEVICE_PHY_ADDR 0x00001000
#define DEVICE_DAC_ADDR 0x00001100
#define DEVICE_temp_ADDR 0x00001200
//#define DEVICE_MPU0_ADDR 0x00001300
#define DEVICE_laser_ADDR 0x00001400
//#define DEVICE_MPU1_ADDR 0x00001500
#define DEVICE_trig_cfg_ADDR 0x00001600
#define DEVICE_trig_out_ADDR 0x00001700
#define CONTROLLER_loop_mode_ADDR 0x00002000
#define CONTROLLER_test_mode_ADDR 0x00002100
#define CONTROLLER_core_ctrl_ADDR 0x00002200
#define CONTROLLER_step_forward_ADDR 0x00002300
#define CONTROLLER_step_phase_ADDR 0x00002400
#define CONTROLLER_step_base_ADDR 0x00002500
#define WAVE_wave_list_ADDR 0x00003000
#define WAVE_wave_adjust_ADDR 0x00003100
#define CHECK_num_check_ADDR 0x00007000
#define CHECK_miss_check_ADDR 0x00007100

struct  SYSTEM
{
    uint32_t COMPANY;
    uint32_t DEVICE;
    uint32_t DEVICE_SUB;
    uint8_t INTERFACETYPE;
    uint8_t PIXELTYPE;
    uint8_t DPITYPE;
    uint8_t SPEEDTYPE;
    uint16_t YEAR;
    uint8_t MONTH;
    uint8_t DATE;
    uint16_t HOUR;
    uint16_t MINUTE;
    uint32_t REV;
    uint32_t reg_soft_reset;
    uint32_t reg_test;
    uint32_t reg_device_temp;
};
typedef SYSTEM* SYSTEM_PTR;
SYSTEM* SYSTEM_MODULE = (SYSTEM_PTR)(SYSTEM_ADDR);

struct DEVICE_DAC
{
    uint32_t reg_mos_ack_time;
    uint32_t reg_dds_ack_time;
    uint32_t reg_dac_ack_time;
    uint32_t reg_sw_ack_time;
};
typedef DEVICE_DAC* DEVICE_DAC_PTR;
DEVICE_DAC* DEVCIE_DAC_MODULE = (DEVICE_DAC*)(DEVICE_DAC_ADDR);

/*struct DEVICE_MPU0
{
    uint32_t reg_iic_whrl;
    uint32_t reg_iic_dev_addr;
    uint32_t reg_iic_addr;
    uint32_t reg_iic_wdata;
    uint32_t reg_iic_rdata;
    uint32_t reg_iic_req;
    uint32_t reg_iic_done;
};
typedef DEVICE_MPU0* DEVICE_MPU0_PTR;
DEVICE_MPU0_PTR DEVICE_MPU0_MODULE = DEVICE_MPU0_PTR(DEVICE_MPU0_ADDR);

struct DEVICE_MPU1
{
    uint32_t reg_mcu1_angle_x;
    uint32_t reg_mcu1_angle_y;
    uint32_t reg_mcu1_angle_z;
    uint32_t reg_mcu1_temp;
    uint32_t reg_mcu1_cfg_addr;
    uint32_t reg_mcu1_cfg_value;
    uint32_t reg_mcu1_cfg_req;
    uint32_t reg_mcu1_cfg_done;
    uint32_t reg_mcu1_bps;
};
typedef DEVICE_MPU1* DEVICE_MPU1_PTR;
DEVICE_MPU1_PTR DEVICE_MPU1_MODULE = DEVICE_MPU1_PTR(DEVICE_MPU1_ADDR);
*/
struct DEVICE_trig_cfg
{
    uint32_t reg_soft_trigger_cycle;
    uint32_t reg_soft_trigger_num;
    uint32_t reg_soft_trigger_en;
    uint32_t reg_trigger_cycle;
    uint32_t reg_trigger_num;
    uint32_t reg_trigger_mode;
    uint32_t reg_trigger_width;
    uint32_t reg_trigger_delay;
    uint32_t reg_trigger_pulse;
    uint32_t reg_trigger_polar;
    uint32_t reg_trigger_en;
    uint32_t reg_encoder_phase;
    uint32_t reg_encoder_cnt_mode;
    uint32_t reg_encoder_dis_mode;
    uint32_t reg_encoder_ignore;
    uint32_t reg_encoder_div;
    uint32_t reg_encoder_width;
    uint32_t reg_encoder_location;
    uint32_t reg_encoder_multi_en;
    uint32_t reg_encoder_multi_coe;
    uint32_t reg_encoder_a_cnt;
    uint32_t reg_encoder_b_cnt;
    uint32_t reg_encoder_clr;
    uint32_t reg_encoder_en;
    uint32_t reg_slave_device;
    uint32_t reg_status_cnt_clr;
};
typedef DEVICE_trig_cfg* DEVICE_trig_cfg_PTR;
DEVICE_trig_cfg_PTR DEVICE_trig_cfg_MODULE = DEVICE_trig_cfg_PTR(DEVICE_trig_cfg_ADDR);

struct DEVICE_trig_out
{
    uint32_t reg_exp_chan;
    uint32_t reg_exposure_time;
    uint32_t reg_led_cnt_max;
    uint32_t reg_trigger_multi_en;
    uint32_t reg_led_polar;
    uint32_t reg_led_pwm_start_0;
    uint32_t reg_led_pwm_end_0;
    uint32_t reg_led_pwm_start_1;
    uint32_t reg_led_pwm_end_1;
    uint32_t reg_led_pwm_start_2;
    uint32_t reg_led_pwm_end_2;
    uint32_t reg_led_pwm_start_3;
    uint32_t reg_led_pwm_end_3;
    uint32_t reg_led_pwm_start_4;
    uint32_t reg_led_pwm_end_4;
    uint32_t reg_led_pwm_start_5;
    uint32_t reg_led_pwm_end_5;
    uint32_t reg_led_pwm_start_6;
    uint32_t reg_led_pwm_end_6;
    uint32_t reg_led_pwm_start_7;
    uint32_t reg_led_pwm_end_7;
    uint32_t reg_led_pwm_start_8;
    uint32_t reg_led_pwm_end_8;
    uint32_t reg_led_pwm_start_9;
    uint32_t reg_led_pwm_end_9;
    uint32_t reg_led_pwm_start_10;
    uint32_t reg_led_pwm_end_10;
    uint32_t reg_led_pwm_start_11;
    uint32_t reg_led_pwm_end_11;
    uint32_t reg_trig_start_0;
    uint32_t reg_trig_end_0;
    uint32_t reg_trig_start_1;
    uint32_t reg_trig_end_1;
    uint32_t reg_trig_start_2;
    uint32_t reg_trig_end_2;
    uint32_t reg_trig_start_3;
    uint32_t reg_trig_end_3;
    uint32_t reg_trig_start_4;
    uint32_t reg_trig_end_4;
    uint32_t reg_trig_start_5;
    uint32_t reg_trig_end_5;
    uint32_t reg_trig_out_polar;
    uint32_t reg_camera_cycle;
    uint32_t reg_camera_delay;
};

typedef DEVICE_trig_out* DEVICE_trig_out_PTR;
DEVICE_trig_out_PTR DEVICE_trig_out_MODULE = DEVICE_trig_out_PTR(DEVICE_trig_out_ADDR);

struct CONTROLLER_loop_mode
{
    uint32_t reg_dds_phase;
    uint32_t reg_dds_inc;
    uint32_t reg_exp_cycle;
    uint32_t reg_trigger_gap;
    uint32_t reg_pic_num;
    uint32_t reg_sw_status;
    uint32_t reg_sw_shift;
    uint32_t reg_sw_loop_gap;
    uint32_t reg_sw_loop_num;
    uint32_t reg_dds_phase_offset;
    uint32_t reg_dds_direction_x;
    uint32_t reg_dds_direction_y;
};
typedef CONTROLLER_loop_mode* CONTROLLER_loop_mode_PTR;
CONTROLLER_loop_mode_PTR CONTROLLER_loop_mode_MODULE = CONTROLLER_loop_mode_PTR(CONTROLLER_loop_mode_ADDR);

struct CONTROLLER_test_mode
{
    uint32_t reg_dac_value_x0;
    uint32_t reg_sw_value_x0;
    uint32_t reg_mos_value_x0;
    uint32_t reg_trigger_req_x0;
    uint32_t reg_dac_value_x1;
    uint32_t reg_sw_value_x1;
    uint32_t reg_mos_value_x1;
    uint32_t reg_trigger_req_x1;
    uint32_t reg_dac_value_x2;
    uint32_t reg_sw_value_x2;
    uint32_t reg_mos_value_x2;
    uint32_t reg_trigger_req_x2;
    uint32_t reg_dac_value_x3;
    uint32_t reg_sw_value_x3;
    uint32_t reg_mos_value_x3;
    uint32_t reg_trigger_req_x3;
    uint32_t reg_dac_value_y0;
    uint32_t reg_sw_value_y0;
    uint32_t reg_mos_value_y0;
    uint32_t reg_trigger_req_y0;
    uint32_t reg_dac_value_y1;
    uint32_t reg_sw_value_y1;
    uint32_t reg_mos_value_y1;
    uint32_t reg_trigger_req_y1;
    uint32_t reg_dac_value_y2;
    uint32_t reg_sw_value_y2;
    uint32_t reg_mos_value_y2;
    uint32_t reg_trigger_req_y2;
    uint32_t reg_dac_value_y3;
    uint32_t reg_sw_value_y3;
    uint32_t reg_mos_value_y3;
    uint32_t reg_trigger_req_y3;
};
typedef CONTROLLER_test_mode* CONTROLLER_test_mode_PTR;
CONTROLLER_test_mode_PTR CONTROLLER_test_mode_MODULE = CONTROLLER_test_mode_PTR(CONTROLLER_test_mode_ADDR);

struct CONTROLLER_core_ctrl
{
    uint32_t reg_core_en;
    uint32_t reg_core_mode;
};
typedef CONTROLLER_core_ctrl* CONTROLLER_core_ctrl_PTR;
CONTROLLER_core_ctrl_PTR CONTROLLER_core_ctrl_MODULE = CONTROLLER_core_ctrl_PTR(CONTROLLER_core_ctrl_ADDR);

struct WAVE_wave_list
{
    uint32_t reg_ram_cfg_en;
    uint32_t reg_ram_whrl;
    uint32_t reg_ram_addr;
    uint32_t reg_ram_wdata;
    uint32_t reg_ram_rdata;
    uint32_t reg_ram_req;
    uint32_t reg_ram_done;
    uint32_t reg_wave_start_addr;
    uint32_t reg_wave_end_addr;
};
typedef WAVE_wave_list* WAVE_wave_list_PTR;
WAVE_wave_list_PTR WAVE_wave_list_MODULE = WAVE_wave_list_PTR(WAVE_wave_list_ADDR);

struct WAVE_wave_adjust
{
    uint32_t reg_adjust_en;
    uint32_t reg_adjust_gain;
    uint32_t reg_adjust_offset;
};
typedef WAVE_wave_adjust* WAVE_wave_adjust_PTR;
WAVE_wave_adjust_PTR WAVE_wave_adjust_MODULE = WAVE_wave_adjust_PTR(WAVE_wave_adjust_ADDR);

struct CHECK_num_check
{
    uint32_t reg_num_check_clr;
    uint32_t reg_io_in_0_num;
    uint32_t reg_io_in_1_num;
    uint32_t reg_encoder_a_num;
    uint32_t reg_encoder_b_num;
    uint32_t reg_io_out_num;
    uint32_t reg_mos_req_num;
    uint32_t reg_mos_ack_num;
    uint32_t reg_dds_req_num;
    uint32_t reg_dds_ack_num;
    uint32_t reg_dac_req_num;
    uint32_t reg_dac_ack_num;
    uint32_t reg_sw_req_num;
    uint32_t reg_sw_ack_num;
    uint32_t reg_reg_req_num;
    uint32_t reg_reg_ack_num;
    uint32_t reg_sum_err_num;

};
typedef CHECK_num_check* CHECK_num_check_PTR;
CHECK_num_check_PTR CHECK_num_check_MODULE = CHECK_num_check_PTR(CHECK_num_check_ADDR);

struct CHECK_miss_check
{
    uint16_t reg_core_status;
};
typedef CHECK_miss_check* CHECK_miss_check_PTR;
CHECK_miss_check_PTR CHECK_miss_check_MODULE = CHECK_miss_check_PTR(CHECK_miss_check_ADDR);


#endif
#endif

#ifdef VERSION_OLD
typedef SYSTEM* SYSTEM_PTR;
SYSTEM* SYSTEM_MODULE = (SYSTEM_PTR)(SYSTEM_ADDR);
typedef DEVICE_DAC* DEVICE_DAC_PTR;
DEVICE_DAC* DEVCIE_DAC_MODULE = (DEVICE_DAC*)(DEVICE_DAC_ADDR);
typedef DEVICE_trig_cfg* DEVICE_trig_cfg_PTR;
DEVICE_trig_cfg_PTR DEVICE_trig_cfg_MODULE = DEVICE_trig_cfg_PTR(DEVICE_trig_cfg_ADDR);
typedef DEVICE_trig_out* DEVICE_trig_out_PTR;
DEVICE_trig_out_PTR DEVICE_trig_out_MODULE = DEVICE_trig_out_PTR(DEVICE_trig_out_ADDR);
typedef CONTROLLER_loop_mode* CONTROLLER_loop_mode_PTR;
CONTROLLER_loop_mode_PTR CONTROLLER_loop_mode_MODULE = CONTROLLER_loop_mode_PTR(CONTROLLER_loop_mode_ADDR);
typedef CONTROLLER_test_mode* CONTROLLER_test_mode_PTR;
CONTROLLER_test_mode_PTR CONTROLLER_test_mode_MODULE = CONTROLLER_test_mode_PTR(CONTROLLER_test_mode_ADDR);
typedef CONTROLLER_core_ctrl* CONTROLLER_core_ctrl_PTR;
CONTROLLER_core_ctrl_PTR CONTROLLER_core_ctrl_MODULE = CONTROLLER_core_ctrl_PTR(CONTROLLER_core_ctrl_ADDR);



typedef CONTROLLER_step_forward* CONTROLLER_step_forward_PTR;
CONTROLLER_step_forward_PTR CONTROLLER_step_forward_MODULE = CONTROLLER_step_forward_PTR(CONTROLLER_step_forward_ADDR);

typedef CONTROLLER_step_phase* CONTROLLER_step_phase_PTR;
CONTROLLER_step_phase_PTR CONTROLLER_step_phase_MODULE = CONTROLLER_step_phase_PTR(CONTROLLER_step_phase_ADDR);

typedef CONTROLLER_step_base* CONTROLLER_step_base_PTR;
CONTROLLER_step_base_PTR CONTROLLER_step_base_MODULE = CONTROLLER_step_base_PTR(CONTROLLER_step_base_ADDR);




typedef WAVE_wave_list* WAVE_wave_list_PTR;
WAVE_wave_list_PTR WAVE_wave_list_MODULE = WAVE_wave_list_PTR(WAVE_wave_list_ADDR);
typedef WAVE_wave_adjust* WAVE_wave_adjust_PTR;
WAVE_wave_adjust_PTR WAVE_wave_adjust_MODULE = WAVE_wave_adjust_PTR(WAVE_wave_adjust_ADDR);
typedef CHECK_num_check* CHECK_num_check_PTR;
CHECK_num_check_PTR CHECK_num_check_MODULE = CHECK_num_check_PTR(CHECK_num_check_ADDR);
typedef CHECK_miss_check* CHECK_miss_check_PTR;
CHECK_miss_check_PTR CHECK_miss_check_MODULE = CHECK_miss_check_PTR(CHECK_miss_check_ADDR);
#endif

/*
void PrintTime() {
   /*
    auto currentTime = std::chrono::system_clock::now();

    // 将时间戳转换为时间结构
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm* localTime = std::localtime(&currentTime_t);

    // 输出时间戳
    std::cout << "当前时间戳: " << currentTime_t << std::endl;

    // 输出格式化的时间
    std::cout << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << std::endl;

    auto currentTime = std::chrono::system_clock::now();

    // 将时间戳转换为时间结构
    std::time_t currentTime_t = std::chrono::system_clock::to_time_t(currentTime);
    std::tm* localTime = std::localtime(&currentTime_t);

    // 精确到毫秒的时间
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch()) % 1000;
    /*
    // 输出时间戳
    std::cout << "当前时间戳: " << std::chrono::duration_cast<std::chrono::seconds>(currentTime.time_since_epoch()).count()
        << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;

    // 输出格式化的时间，精确到毫秒
    std::cout << "当前时间: " << std::put_time(localTime, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << milliseconds.count() << std::endl;
}
*/



int save_config_in_device(int board_num)
{
    int ret = 0;
    uint16_t buffer[2] = { 0,1 };
    if (board_num == 1)
    {
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
    }
    else if (board_num == 3)
    {
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
    }
    else if (board_num == 5)
    {
        int ret = 0;
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIRSTBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(SECONDBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(THIRDBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FOURTHBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(FIFTHBOARD, 0x4000, buffer);
        if (ret != 0) { return ret; }
    }
    Sleep(1000);
    return if_config_in_device_ok(board_num);
}

int if_config_in_device_ok(int board_num)//返回-1没读成功，0保存成功，1没保存完，-2板数不对
{   //写完1后，开始保存，当寄存器值恢复为0则保存成功
    uint16_t buffer[2] = { 0 };
    int ret = 0;
    if (board_num == 1)
    {
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4000, buffer);
        if (ret != 0) {
            return -1;
        }
        else
            return buffer[1];
    }
    else if (board_num == 3)
    {
        
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4000, buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        return buffer[1];
    }
    else if (board_num == 5)
    {
        int ret = 0;
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIRSTBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(SECONDBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(THIRDBOARD, 0x4000, buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FOURTHBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        if (buffer != 0) {  return buffer[1]; }
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(FIFTHBOARD, 0x4000,  buffer);
        if (ret != 0) { return -1; }
        return buffer[1]; 
    }
    else { return -2; }//输入的板块数不是1，3，5
}



int read_extern_laser1(uint16_t* buffer) 
{
    return Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(EXTSTM32, 10, 1,buffer);
}

int read_extern_laser2(uint16_t * buffer)
{
    return Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(EXTSTM32, 11,1, buffer);
}

int write_extern_laser1(void* ins_light,uint16_t* buffer)
{
    return (int)((InsPMDL_lightSouce*)ins_light)->ins_write_Laser(EXTSTM32, 10, buffer);//0320   //0419
    //0320
}

int write_extern_laser2(void* ins_light,uint16_t* buffer)
{
    return (int)((InsPMDL_lightSouce*)ins_light)->ins_write_Laser(EXTSTM32, 11, buffer);
}

int read_extern_pirch(void* ins_light, double * buffer)
{
    //读到的角度需要除以十
    int ret = 0;
    uint16_t temp = 0;
    //short temp_short = 0;
    //ret = Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(EXTSTM32, 12, 1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(EXTSTM32, 12,1 ,&temp);
    if (ret == 0)
    {
        *buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}
int read_extern_roll(void* ins_light, double* buffer)
{
    int ret = 0;
    uint16_t temp = 0;
    short temp_short = 0;
    //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(EXTSTM32, 13,1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(EXTSTM32, 13,1, &temp);
    if (ret == 0)
    {
		*buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}
int read_extern_yaw(void* ins_light, double* buffer)
{
    int ret = 0;
    uint16_t temp = 0;
    short temp_short = 0;
    //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(EXTSTM32, 14,1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(EXTSTM32, 14,1, &temp);
    if (ret == 0)
    {
		*buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}

int read_internl_pirch(void* ins_light, double* buffer)
{
    int ret = 0;
    uint16_t temp = 0;
    short temp_short = 0;
    //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(INTSTM32, 10, 1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(INTSTM32, 10,1, &temp);
    if (ret == 0)
    {
		*buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}

int read_internl_roll(void* ins_light, double* buffer)
{
    int ret = 0;
    uint16_t temp = 0;
    short temp_short = 0;
    //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(INTSTM32, 11, 1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(INTSTM32, 11,1, &temp);
    if (ret == 0)
    {
		*buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}


int read_internl_yaw(void* ins_light, double* buffer)
{
    int ret = 0;
    uint16_t temp = 0;
    short temp_short = 0;
    //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(INTSTM32, 12,1, &temp);
    ret = (int)((InsPMDL_lightSouce*)ins_light)->modbus_udp_read_registers(INTSTM32, 12,1, &temp);
    if (ret == 0)
    {
		*buffer = (static_cast<short>(temp)) / -10.0;
    }
    return ret;
}

int read_register(void* ins_light,BoardID _id, uint16_t _address, uint32_t& _buffer)
{
    uint16_t buffer1[2] = { 0 };
    //int ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read(_id, _address, buffer1);
    int ret = (int)((InsPMDL_lightSouce*)ins_light)->ins_read((BoardID)_id, _address, buffer1);
    _buffer = (static_cast<uint32_t>(buffer1[0]) << 16) | (static_cast<uint32_t>(buffer1[1]));
    return ret;
}

int write_register(void* ins_light,BoardID _id, uint16_t _address, uint32_t _buffer)
{
    uint32_t valueToWrite = _buffer;
    uint16_t highValue = (valueToWrite >> 16) & 0xFFFF;
    uint16_t lowValue = valueToWrite & 0xFFFF;
    uint16_t buffer1[2] = { highValue,lowValue };
    //return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)_id, _address, buffer1);
    int ret = (int)((InsPMDL_lightSouce*)ins_light)->ins_write((BoardID)_id, _address, buffer1);
    return ret;
}

//int save_camera_height(void* ins_light,uint32_t* buffer)
//{
//    int ret = 0;
//    int num = 2;
//    uint32_t temp = 1;  //16->32
//    uint32_t buffer_temp = *buffer;
//    //[2] = {};
//    //buffer_temp[0] = static_cast<uint16_t>(*buffer >> 16);
//    //buffer_temp[1] = static_cast<uint16_t>(*buffer & 0xFFFF);
//    //Ins_Modbus_udp::get_instance()->modbus_udp_write_registers(INTSTM32, 1, 2, buffer_temp);
//    write_register(ins_light,INTSTM32, 1, buffer_temp);//0319 to do
//
//    //Ins_Modbus_udp::get_instance()->modbus_udp_write_registers(INTSTM32, 0, 1, &temp);
//
//    write_register(ins_light,INTSTM32, 1, temp);//0319 to do
//    while (temp != 0 && num > 0) {
//        Sleep(100);
//        num--;
//        //Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(INTSTM32, 0, 1, &temp);
//     read_register(ins_light,INTSTM32, 1, temp);//0319 to do
//    }
//       
//    if (temp == 0)
//    {
//        return 0;
//    }
//    return -1;
//}

//int read_camera_height(void* ins_light,uint32_t* buffer)
//{
//    //uint16_t buffer_temp[2] = {};
//    int ret = 0;
//    //ret = Ins_Modbus_udp::get_instance()->modbus_udp_read_registers(INTSTM32, 1, 2, buffer_temp);
//    ret  = read_register(ins_light, INTSTM32, 1, *buffer);
//    //*buffer = (static_cast<uint32_t>(buffer_temp[0]) << 16) | buffer_temp[1];
//    return ret;
//}





/*测试用的
int ins_write_reg_1(BoardID id, uint32_t* buffer)
{
    return write_register(id, 1, *buffer);
}

int ins_read_reg_1(BoardID id, uint32_t* buffer)
{
    return read_register(id, 1, *buffer);
}
*/

#if 0

//system
int ins_write_reg_soft_reset(BoardID id,uint32_t* buffer)
{
    return write_register(id,(uint16_t) & (SYSTEM_MODULE->reg_soft_reset), *buffer);
}
int ins_write_reg_test(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (SYSTEM_MODULE->reg_test), *buffer);
}


//DAC
int ins_read_reg_mos_ack_time(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), *buffer);
}
int ins_write_reg_mos_ack_time(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), *buffer);
}

int ins_read_reg_dds_ack_time(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), *buffer);
}
int ins_write_reg_dds_ack_time(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), *buffer);
}

int ins_read_reg_dac_ack_time(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), *buffer);
}
int ins_write_reg_dac_ack_time(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), *buffer);
}
int ins_read_reg_sw_ack_time(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), *buffer);
}
int ins_write_reg_sw_ack_time(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), *buffer);
}

//temp
/*
int ins_write_reg_mainboard_temp(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(DEVICE_temp_ADDR), *buffer);
}
*/

//num_check
int ins_write_reg_num_check_clr(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_num_check_clr), *buffer);
}

int ins_write_reg_io_in_0_num(BoardID id, uint32_t* buffer) 
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_0_num), *buffer);
}
int ins_write_reg_io_in_1_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_1_num), *buffer);
}
int ins_write_reg_encoder_a_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_a_num), *buffer);
}

int ins_write_reg_encoder_b_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_b_num), *buffer);
}

int ins_write_reg_io_out_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_out_num), *buffer);
}

int ins_write_reg_mos_req_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_req_num), *buffer);
}

int ins_write_reg_mos_ack_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_ack_num), *buffer);
}

int ins_write_reg_dds_req_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_req_num), *buffer);
}

int ins_write_reg_dds_ack_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_ack_num), *buffer);
}

int ins_write_reg_dac_req_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_req_num), *buffer);
}

int ins_write_reg_dac_ack_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_ack_num), *buffer);
}

int ins_write_reg_sw_req_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_req_num), *buffer);
}

int ins_write_reg_sw_ack_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_ack_num), *buffer);
}

int ins_write_reg_reg_req_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_req_num), *buffer);
}

int ins_write_reg_reg_ack_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_ack_num), *buffer);
}

int ins_write_reg_sum_err_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sum_err_num), *buffer);
}

//miss_check
int ins_write_reg_core_status(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t)(CHECK_miss_check_MODULE->reg_core_status), *buffer);
}








//READ
// 
//system

int ins_read_COMPANY(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->COMPANY), *buffer);
}
int ins_read_DEVICE(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->DEVICE), *buffer);
}
int ins_read_DEVICE_SUB(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->DEVICE_SUB), *buffer);
}
int ins_read_INTERFACETYPE(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->INTERFACETYPE), *buffer);
}
int ins_read_YEAR(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->YEAR), *buffer);
}
int ins_read_HOUR(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->HOUR), *buffer);
}
int ins_read_REV(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->REV), *buffer);
}
int ins_read_reg_soft_reset(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_soft_reset), *buffer);
}
int ins_read_reg_test(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_test), *buffer);
}
int ins_read_reg_device_temp(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (SYSTEM_MODULE->reg_device_temp), *buffer);
}


// phy
//int ins_write_reg_phy_rst(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t)(DEVICE_PHY_ADDR), *buffer);
//}


/*
//MPU0
int ins_write_reg_iic_whrl(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), *buffer);
}
int ins_write_reg_iic_dev_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), *buffer);
}
int ins_write_reg_iic_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), *buffer);
}
int ins_write_reg_iic_wdata(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), *buffer);
}
//int ins_write_reg_iic_rdata(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), *buffer);
//}
int ins_write_reg_iic_req(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), *buffer);
}
//int ins_write_reg_iic_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), *buffer);
//}
*/
//laser
int ins_write_reg_laser_status(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t)(DEVICE_laser_ADDR), *buffer);
}


////mpu1
//int ins_write_reg_mcu1_angle_x(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), *buffer);
//}
//int ins_write_reg_mcu1_angle_y(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), *buffer);
//}
//int ins_write_reg_mcu1_angle_z(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), *buffer);
//}
//int ins_write_reg_mcu1_temp(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), *buffer);
//}

/*
int ins_write_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), *buffer);
}
int ins_write_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), *buffer);
}
int ins_write_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), *buffer);
}
//int ins_write_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), *buffer);
//}
int ins_write_reg_mcu1_bps(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), *buffer);
}
*/
//trig_cfg
int ins_write_reg_soft_trigger_cycle(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), *buffer);
}
int ins_write_reg_soft_trigger_num(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), *buffer);
}
int ins_write_reg_soft_trigger_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), *buffer);
}
int ins_write_reg_trigger_cycle(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), *buffer);
}
int ins_write_reg_trigger_num(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), *buffer);
}
int ins_write_reg_trigger_mode(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), *buffer);
}
int ins_write_reg_trigger_width(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), *buffer);
}
int ins_write_reg_trigger_delay(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), *buffer);
}
int ins_write_reg_trigger_pulse(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), *buffer);
}
int ins_write_reg_trigger_polar(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), *buffer);
}
int ins_write_reg_trigger_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), *buffer);
}
int ins_write_reg_encoder_phase(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), *buffer);
}
int ins_write_reg_encoder_cnt_mode(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), *buffer);
}
int ins_write_reg_encoder_dis_mode(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), *buffer);
}
int ins_write_reg_encoder_ignore(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), *buffer);
}
int ins_write_reg_encoder_div(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), *buffer);
}
int ins_write_reg_encoder_width(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), *buffer);
}
//int ins_write_reg_encoder_location(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), *buffer);
//}
int ins_write_reg_encoder_multi_en(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), *buffer);
}

int ins_write_reg_encoder_multi_coe(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), *buffer);
}
//int ins_write_reg_encoder_a_cnt(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), *buffer);
//}
//int ins_write_reg_encoder_b_cnt(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), *buffer);
//}
int ins_write_reg_encoder_clr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), *buffer);
}
int ins_write_reg_encoder_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), *buffer);
}
int ins_write_reg_slave_device(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), *buffer);
}
int ins_write_reg_status_cnt_clr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), *buffer);
}
//1123新增


int ins_write_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_cycle), *buffer);
}

int ins_write_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_num), *buffer);
}

int ins_write_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_en), *buffer);
}

int ins_write_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_cycle), *buffer);
}

int ins_write_reg_l1_trigger_num(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_num), *buffer);
}

int ins_write_reg_l1_trigger_mode(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_mode), *buffer);
}

int ins_write_reg_l1_trigger_width(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_width), *buffer);
}

int ins_write_reg_l1_trigger_delay(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_delay), *buffer);
}

int ins_write_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_pulse), *buffer);
}

int ins_write_reg_l1_trigger_polar(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_polar), *buffer);
}

int ins_write_reg_l1_trigger_en(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_en), *buffer);
}

int ins_write_reg_l1_encoder_phase(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_phase), *buffer);
}

int ins_write_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_cnt_mode), *buffer);
}

int ins_write_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_dis_mode), *buffer);
}

int ins_write_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_ignore), *buffer);
}

int ins_write_reg_l1_encoder_div(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_div), *buffer);
}

int ins_write_reg_l1_encoder_width(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_width), *buffer);
}
/*
int ins_write_reg_l1_encoder_location(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_location), *buffer);
}
*/
int ins_write_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_en), *buffer);
}

int ins_write_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_coe), *buffer);
}
/*
int ins_write_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_a_cnt), *buffer);
}

int ins_write_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_b_cnt), *buffer);
}
*/
int ins_write_reg_l1_encoder_clr(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_clr), *buffer);
}

int ins_write_reg_l1_encoder_en(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_en), *buffer);
}

int ins_write_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_status_cnt_clr), *buffer);
}

//写死
int ins_write_reg_trigger_level(BoardID id, uint32_t * buffer)
{
	return write_register(id, 2942, *buffer);
}



////trig_out
int ins_write_reg_exp_chan(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), *buffer);
}
int ins_write_reg_exposure_time(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), *buffer);
}
int ins_write_reg_led_cnt_max(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), *buffer);
}
int ins_write_reg_trigger_multi_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), *buffer);
}
int ins_write_reg_led_polar(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), *buffer);
}
int ins_write_reg_led_pwm_start_0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), *buffer);
}
int ins_write_reg_led_pwm_end_0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), *buffer);
}
int ins_write_reg_led_pwm_start_1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), *buffer);
}
int ins_write_reg_led_pwm_end_1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), *buffer);
}
int ins_write_reg_led_pwm_start_2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), *buffer);
}
int ins_write_reg_led_pwm_end_2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), *buffer);
}
int ins_write_reg_led_pwm_start_3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), *buffer);
}
int ins_write_reg_led_pwm_end_3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), *buffer);
}
int ins_write_reg_led_pwm_start_4(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), *buffer);
}
int ins_write_reg_led_pwm_end_4(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), *buffer);
}
int ins_write_reg_led_pwm_start_5(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), *buffer);
}
int ins_write_reg_led_pwm_end_5(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), *buffer);
}
int ins_write_reg_led_pwm_start_6(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), *buffer);
}
int ins_write_reg_led_pwm_end_6(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), *buffer);
}
int ins_write_reg_led_pwm_start_7(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), *buffer);
}
int ins_write_reg_led_pwm_end_7(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), *buffer);
}
int ins_write_reg_led_pwm_start_8(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), *buffer);
}
int ins_write_reg_led_pwm_end_8(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), *buffer);
}
int ins_write_reg_led_pwm_start_9(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), *buffer);
}
int ins_write_reg_led_pwm_end_9(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), *buffer);
}
int ins_write_reg_led_pwm_start_10(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), *buffer);
}
int ins_write_reg_led_pwm_end_10(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), *buffer);
}
int ins_write_reg_led_pwm_start_11(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), *buffer);
}
int ins_write_reg_led_pwm_end_11(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), *buffer);
}
int ins_write_reg_trig_start_0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), *buffer);
}
int ins_write_reg_trig_end_0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), *buffer);
}
int ins_write_reg_trig_start_1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), *buffer);
}
int ins_write_reg_trig_end_1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), *buffer);
}
int ins_write_reg_trig_start_2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), *buffer);
}
int ins_write_reg_trig_end_2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), *buffer);
}
int ins_write_reg_trig_start_3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), *buffer);
}
int ins_write_reg_trig_end_3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), *buffer);
}
int ins_write_reg_trig_start_4(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), *buffer);
}
int ins_write_reg_trig_end_4(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), *buffer);
}
int ins_write_reg_trig_start_5(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), *buffer);
}
int ins_write_reg_trig_end_5(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), *buffer);
}
int ins_write_reg_trig_out_polar(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), *buffer);
}
//loopmode
int ins_write_reg_dds_phase(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), *buffer);
}
int ins_write_reg_dds_inc(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), *buffer);
}
int ins_write_reg_exp_cycle(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), *buffer);
}
int ins_write_reg_trigger_gap(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), *buffer);
}
int ins_write_reg_pic_num(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), *buffer);
}
int ins_write_reg_sw_status(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), *buffer);
}
int ins_write_reg_sw_shift(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), *buffer);
}
int ins_write_reg_sw_loop_gap(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), *buffer);
}
int ins_write_reg_sw_loop_num(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), *buffer);
}
int ins_write_reg_dds_phase_offset(BoardID id, uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), *buffer);
}

//11.23
extern "C" INSCAMERA_API int ins_write_reg_dds_direction_x(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_dds_direction_y(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_current_offset(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_current_offset), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_dac_value_forward(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_forward), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_dac_value_backward(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_backward), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_sw_wait(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_wait), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_camera_delay(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_camera_cycle(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle), *buffer);
}

extern "C" INSCAMERA_API int ins_write_reg_dac_req(BoardID id, uint32_t * buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_req), *buffer);
}
//写死地址
extern "C" INSCAMERA_API int ins_write_reg_dds_phase_y(BoardID id, uint32_t * buffer)
{
	return write_register(id, 4152, *buffer);
}


//test
int ins_write_reg_dac_value_x0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), *buffer);
}
int ins_write_reg_sw_value_x0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), *buffer);
}
int ins_write_reg_mos_value_x0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), *buffer);
}
int ins_write_reg_trigger_req_x0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), *buffer);
}
int ins_write_reg_dac_value_x1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), *buffer);
}
int ins_write_reg_sw_value_x1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), *buffer);
}
int ins_write_reg_mos_value_x1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), *buffer);
}
int ins_write_reg_trigger_req_x1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), *buffer);
}
int ins_write_reg_dac_value_x2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), *buffer);
}
int ins_write_reg_sw_value_x2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), *buffer);
}
int ins_write_reg_mos_value_x2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), *buffer);
}
int ins_write_reg_trigger_req_x2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), *buffer);
}
int ins_write_reg_dac_value_x3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), *buffer);
}
int ins_write_reg_sw_value_x3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), *buffer);
}
int ins_write_reg_mos_value_x3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), *buffer);
}
int ins_write_reg_trigger_req_x3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), *buffer);
}
int ins_write_reg_dac_value_y0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), *buffer);
}
int ins_write_reg_sw_value_y0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), *buffer);
}
int ins_write_reg_mos_value_y0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), *buffer);
}
int ins_write_reg_trigger_req_y0(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), *buffer);
}
int ins_write_reg_dac_value_y1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), *buffer);
}
int ins_write_reg_sw_value_y1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), *buffer);
}
int ins_write_reg_mos_value_y1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), *buffer);
}
int ins_write_reg_trigger_req_y1(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), *buffer);
}
int ins_write_reg_dac_value_y2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), *buffer);
}
int ins_write_reg_sw_value_y2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), *buffer);
}
int ins_write_reg_mos_value_y2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), *buffer);
}
int ins_write_reg_trigger_req_y2(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), *buffer);
}
int ins_write_reg_dac_value_y3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), *buffer);
}
int ins_write_reg_sw_value_y3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), *buffer);
}
int ins_write_reg_mos_value_y3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), *buffer);
}
int ins_write_reg_trigger_req_y3(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), *buffer);
}
//core
int ins_write_reg_core_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), *buffer);
}
int ins_write_reg_core_mode(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), *buffer);
}


//step forward
int ins_write_reg_step_en(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_en), *buffer);
}

int ins_write_reg_step_pic_num(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_pic_num), *buffer);
}

int ins_write_reg_step_x_seq(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_x_seq), *buffer);
}

int ins_write_reg_step_y_seq(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_y_seq), *buffer);
}
//step phase write
int ins_write_reg_step_phase_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_0), *buffer);
}

int ins_write_reg_step_phase_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_1), *buffer);
}

int ins_write_reg_step_phase_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_2), *buffer);
}

int ins_write_reg_step_phase_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_3), *buffer);
}

int ins_write_reg_step_phase_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_4), *buffer);
}

int ins_write_reg_step_phase_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_5), *buffer);
}

int ins_write_reg_step_phase_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_6), *buffer);
}

int ins_write_reg_step_phase_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_7), *buffer);
}

int ins_write_reg_step_phase_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_8), *buffer);
}

int ins_write_reg_step_phase_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_9), *buffer);
}

int ins_write_reg_step_phase_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_10), *buffer);
}

int ins_write_reg_step_phase_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_11), *buffer);
}

int ins_write_reg_step_phase_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_12), *buffer);
}

int ins_write_reg_step_phase_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_13), *buffer);
}

int ins_write_reg_step_phase_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_14), *buffer);
}

int ins_write_reg_step_phase_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_15), *buffer);
}

int ins_write_reg_step_phase_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_16), *buffer);
}

int ins_write_reg_step_phase_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_17), *buffer);
}

int ins_write_reg_step_phase_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_18), *buffer);
}

int ins_write_reg_step_phase_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_19), *buffer);
}

int ins_write_reg_step_phase_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_20), *buffer);
}

int ins_write_reg_step_phase_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_21), *buffer);
}

int ins_write_reg_step_phase_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_22), *buffer);
}

int ins_write_reg_step_phase_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_23), *buffer);
}

int ins_write_reg_step_phase_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_24), *buffer);
}

int ins_write_reg_step_phase_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_25), *buffer);
}

int ins_write_reg_step_phase_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_26), *buffer);
}

int ins_write_reg_step_phase_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_27), *buffer);
}

int ins_write_reg_step_phase_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_28), *buffer);
}

int ins_write_reg_step_phase_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_29), *buffer);
}

int ins_write_reg_step_phase_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_30), *buffer);
}

int ins_write_reg_step_phase_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_31), *buffer);
}

int ins_write_reg_step_inc_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_0), *buffer);
}

int ins_write_reg_step_inc_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_1), *buffer);
}

int ins_write_reg_step_inc_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_2), *buffer);
}

int ins_write_reg_step_inc_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_3), *buffer);
}

int ins_write_reg_step_inc_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_4), *buffer);
}

int ins_write_reg_step_inc_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_5), *buffer);
}

int ins_write_reg_step_inc_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_6), *buffer);
}

int ins_write_reg_step_inc_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_7), *buffer);
}

int ins_write_reg_step_inc_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_8), *buffer);
}

int ins_write_reg_step_inc_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_9), *buffer);
}

int ins_write_reg_step_inc_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_10), *buffer);
}

int ins_write_reg_step_inc_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_11), *buffer);
}

int ins_write_reg_step_inc_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_12), *buffer);
}

int ins_write_reg_step_inc_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_13), *buffer);
}

int ins_write_reg_step_inc_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_14), *buffer);
}

int ins_write_reg_step_inc_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_15), *buffer);
}

int ins_write_reg_step_inc_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_16), *buffer);
}

int ins_write_reg_step_inc_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_17), *buffer);
}

int ins_write_reg_step_inc_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_18), *buffer);
}

int ins_write_reg_step_inc_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_19), *buffer);
}

int ins_write_reg_step_inc_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_20), *buffer);
}

int ins_write_reg_step_inc_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_21), *buffer);
}

int ins_write_reg_step_inc_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_22), *buffer);
}

int ins_write_reg_step_inc_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_23), *buffer);
}

int ins_write_reg_step_inc_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_24), *buffer);
}

int ins_write_reg_step_inc_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_25), *buffer);
}

int ins_write_reg_step_inc_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_26), *buffer);
}

int ins_write_reg_step_inc_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_27), *buffer);
}

int ins_write_reg_step_inc_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_28), *buffer);
}

int ins_write_reg_step_inc_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_29), *buffer);
}

int ins_write_reg_step_inc_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_30), *buffer);
}

int ins_write_reg_step_inc_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_31), *buffer);
}


//step base read
int ins_read_reg_step_base_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_0), *buffer);
}

int ins_read_reg_step_base_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_1), *buffer);
}

int ins_read_reg_step_base_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_2), *buffer);
}

int ins_read_reg_step_base_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_3), *buffer);
}

int ins_read_reg_step_base_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_4), *buffer);
}

int ins_read_reg_step_base_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_5), *buffer);
}

int ins_read_reg_step_base_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_6), *buffer);
}

int ins_read_reg_step_base_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_7), *buffer);
}

int ins_read_reg_step_base_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_8), *buffer);
}

int ins_read_reg_step_base_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_9), *buffer);
}

int ins_read_reg_step_base_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_10), *buffer);
}

int ins_read_reg_step_base_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_11), *buffer);
}

int ins_read_reg_step_base_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_12), *buffer);
}

int ins_read_reg_step_base_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_13), *buffer);
}

int ins_read_reg_step_base_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_14), *buffer);
}

int ins_read_reg_step_base_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_15), *buffer);
}

int ins_read_reg_step_base_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_16), *buffer);
}

int ins_read_reg_step_base_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_17), *buffer);
}

int ins_read_reg_step_base_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_18), *buffer);
}

int ins_read_reg_step_base_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_19), *buffer);
}

int ins_read_reg_step_base_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_20), *buffer);
}

int ins_read_reg_step_base_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_21), *buffer);
}

int ins_read_reg_step_base_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_22), *buffer);
}

int ins_read_reg_step_base_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_23), *buffer);
}

int ins_read_reg_step_base_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_24), *buffer);
}

int ins_read_reg_step_base_25(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_25), *buffer);
}

int ins_read_reg_step_base_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_26), *buffer);
}

int ins_read_reg_step_base_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_27), *buffer);
}

int ins_read_reg_step_base_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_28), *buffer);
}

int ins_read_reg_step_base_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_29), *buffer);
}

int ins_read_reg_step_base_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_30), *buffer);
}

int ins_read_reg_step_base_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_31), *buffer);
}

//step base write
int ins_write_reg_step_base_0(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_0), *buffer);
}

int ins_write_reg_step_base_1(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_1), *buffer);
}

int ins_write_reg_step_base_2(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_2), *buffer);
}

int ins_write_reg_step_base_3(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_3), *buffer);
}

int ins_write_reg_step_base_4(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_4), *buffer);
}

int ins_write_reg_step_base_5(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_5), *buffer);
}

int ins_write_reg_step_base_6(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_6), *buffer);
}

int ins_write_reg_step_base_7(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_7), *buffer);
}

int ins_write_reg_step_base_8(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_8), *buffer);
}

int ins_write_reg_step_base_9(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_9), *buffer);
}

int ins_write_reg_step_base_10(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_10), *buffer);
}

int ins_write_reg_step_base_11(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_11), *buffer);
}

int ins_write_reg_step_base_12(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_12), *buffer);
}

int ins_write_reg_step_base_13(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_13), *buffer);
}

int ins_write_reg_step_base_14(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_14), *buffer);
}

int ins_write_reg_step_base_15(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_15), *buffer);
}

int ins_write_reg_step_base_16(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_16), *buffer);
}

int ins_write_reg_step_base_17(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_17), *buffer);
}

int ins_write_reg_step_base_18(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_18), *buffer);
}

int ins_write_reg_step_base_19(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_19), *buffer);
}

int ins_write_reg_step_base_20(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_20), *buffer);
}

int ins_write_reg_step_base_21(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_21), *buffer);
}

int ins_write_reg_step_base_22(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_22), *buffer);
}

int ins_write_reg_step_base_23(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_23), *buffer);
}

int ins_write_reg_step_base_24(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_24), *buffer);
}

int ins_write_reg_step_base_25(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_25), *buffer);
}

int ins_write_reg_step_base_26(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_26), *buffer);
}

int ins_write_reg_step_base_27(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_27), *buffer);
}

int ins_write_reg_step_base_28(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_28), *buffer);
}

int ins_write_reg_step_base_29(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_29), *buffer);
}

int ins_write_reg_step_base_30(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_30), *buffer);
}

int ins_write_reg_step_base_31(BoardID id, uint32_t* buffer)
{
	return write_register(id, (uint16_t) & (CONTROLLER_step_base_MODULE->reg_step_base_31), *buffer);
}


//wave_list
int ins_write_reg_ram_cfg_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), *buffer);
}
int ins_write_reg_ram_whrl(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), *buffer);
}
int ins_write_reg_ram_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), *buffer);
}
//wave
int ins_write_reg_ram_wdata(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), *buffer);
}
//int ins_write_reg_ram_rdata(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), *buffer);
//}
int ins_write_reg_ram_req(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), *buffer);
}
//int ins_write_reg_ram_done(BoardID id,uint32_t* buffer)
//{
//    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), *buffer);
//}
int ins_write_reg_wave_start_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), *buffer);
}
int ins_write_reg_wave_end_addr(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), *buffer);
}
//adjust
int ins_write_reg_adjust_en(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), *buffer);
}
int ins_write_reg_adjust_gain(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), *buffer);
}
int ins_write_reg_adjust_offset(BoardID id,uint32_t* buffer)
{
    return write_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), *buffer);
}




//READ
// phy
//int ins_read_reg_phy_rst(BoardID id,uint32_t* buffer)
//{
//    return read_register(id, (uint16_t)(DEVICE_PHY_ADDR), *buffer);
//}

//temp
int ins_read_reg_mainboard_temp(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) (DEVICE_temp_ADDR), *buffer);
}
/*
//MPU0
int ins_read_reg_iic_whrl(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), *buffer);
}
int ins_read_reg_iic_dev_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), *buffer);
}
int ins_read_reg_iic_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), *buffer);
}
int ins_read_reg_iic_wdata(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), *buffer);
}
int ins_read_reg_iic_rdata(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), *buffer);
}
int ins_read_reg_iic_req(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), *buffer);
}
int ins_read_reg_iic_done(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), *buffer);
}
*/
int ins_read_reg_laser_status(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t)(DEVICE_laser_ADDR), *buffer);
}
/*
////mpu1
int ins_read_reg_mcu1_angle_x(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), *buffer);
}
int ins_read_reg_mcu1_angle_y(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), *buffer);
}
int ins_read_reg_mcu1_angle_z(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), *buffer);
}
int ins_read_reg_mcu1_temp(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), *buffer);
}
int ins_read_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), *buffer);
}
int ins_read_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), *buffer);
}
int ins_read_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), *buffer);
}
int ins_read_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), *buffer);
}
int ins_read_reg_mcu1_bps(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), *buffer);
}

*/

//trig_cfg
int ins_read_reg_soft_trigger_cycle(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), *buffer);
}
int ins_read_reg_soft_trigger_num(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), *buffer);
}
int ins_read_reg_soft_trigger_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), *buffer);
}
int ins_read_reg_trigger_cycle(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), *buffer);
}
int ins_read_reg_trigger_num(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), *buffer);
}
int ins_read_reg_trigger_mode(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), *buffer);
}
int ins_read_reg_trigger_width(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), *buffer);
}
int ins_read_reg_trigger_delay(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), *buffer);
}
int ins_read_reg_trigger_pulse(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), *buffer);
}
int ins_read_reg_trigger_polar(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), *buffer);
}
int ins_read_reg_trigger_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), *buffer);
}
int ins_read_reg_encoder_phase(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), *buffer);
}
int ins_read_reg_encoder_cnt_mode(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), *buffer);
}
int ins_read_reg_encoder_dis_mode(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), *buffer);
}
int ins_read_reg_encoder_ignore(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), *buffer);
}
int ins_read_reg_encoder_div(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), *buffer);
}
int ins_read_reg_encoder_width(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), *buffer);
}
int ins_read_reg_encoder_location(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), *buffer);
}
int ins_read_reg_encoder_multi_en(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), *buffer);
}
int ins_read_reg_encoder_multi_coe(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), *buffer);
}
int ins_read_reg_encoder_a_cnt(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), *buffer);
}
int ins_read_reg_encoder_b_cnt(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), *buffer);
}
int ins_read_reg_encoder_clr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), *buffer);
}
int ins_read_reg_encoder_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), *buffer);
}
int ins_read_reg_slave_device(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), *buffer);
}
int ins_read_reg_status_cnt_clr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), *buffer);
}
//1123新增

int ins_read_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_cycle), *buffer);
}

int ins_read_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_num), *buffer);
}

int ins_read_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_soft_trigger_en), *buffer);
}

int ins_read_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_cycle), *buffer);
}

int ins_read_reg_l1_trigger_num(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_num), *buffer);
}

int ins_read_reg_l1_trigger_mode(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_mode), *buffer);
}

int ins_read_reg_l1_trigger_width(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_width), *buffer);
}

int ins_read_reg_l1_trigger_delay(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_delay), *buffer);
}

int ins_read_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_pulse), *buffer);
}

int ins_read_reg_l1_trigger_polar(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_polar), *buffer);
}

int ins_read_reg_l1_trigger_en(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_trigger_en), *buffer);
}

int ins_read_reg_l1_encoder_phase(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_phase), *buffer);
}

int ins_read_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_cnt_mode), *buffer);
}

int ins_read_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_dis_mode), *buffer);
}

int ins_read_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_ignore), *buffer);
}

int ins_read_reg_l1_encoder_div(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_div), *buffer);
}

int ins_read_reg_l1_encoder_width(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_width), *buffer);
}

int ins_read_reg_l1_encoder_location(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_location), *buffer);
}

int ins_read_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_en), *buffer);
}

int ins_read_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_multi_coe), *buffer);
}

int ins_read_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_a_cnt), *buffer);
}

int ins_read_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_b_cnt), *buffer);
}

int ins_read_reg_l1_encoder_clr(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_clr), *buffer);
}

int ins_read_reg_l1_encoder_en(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_encoder_en), *buffer);
}

int ins_read_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_l1_status_cnt_clr), *buffer);
}

//写死
int ins_read_reg_trigger_level(BoardID id, uint32_t * buffer)
{
	return read_register(id, 2942, *buffer);
}



////trig_out
int ins_read_reg_exp_chan(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), *buffer);
}
int ins_read_reg_exposure_time(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), *buffer);
}
int ins_read_reg_led_cnt_max(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), *buffer);
}
int ins_read_reg_trigger_multi_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), *buffer);
}
int ins_read_reg_led_polar(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), *buffer);
}
int ins_read_reg_led_pwm_start_0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), *buffer);
}
int ins_read_reg_led_pwm_end_0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), *buffer);
}
int ins_read_reg_led_pwm_start_1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), *buffer);
}
int ins_read_reg_led_pwm_end_1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), *buffer);
}
int ins_read_reg_led_pwm_start_2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), *buffer);
}
int ins_read_reg_led_pwm_end_2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), *buffer);
}
int ins_read_reg_led_pwm_start_3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), *buffer);
}
int ins_read_reg_led_pwm_end_3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), *buffer);
}
int ins_read_reg_led_pwm_start_4(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), *buffer);
}
int ins_read_reg_led_pwm_end_4(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), *buffer);
}
int ins_read_reg_led_pwm_start_5(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), *buffer);
}
int ins_read_reg_led_pwm_end_5(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), *buffer);
}
int ins_read_reg_led_pwm_start_6(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), *buffer);
}
int ins_read_reg_led_pwm_end_6(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), *buffer);
}
int ins_read_reg_led_pwm_start_7(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), *buffer);
}
int ins_read_reg_led_pwm_end_7(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), *buffer);
}
int ins_read_reg_led_pwm_start_8(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), *buffer);
}
int ins_read_reg_led_pwm_end_8(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), *buffer);
}
int ins_read_reg_led_pwm_start_9(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), *buffer);
}
int ins_read_reg_led_pwm_end_9(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), *buffer);
}
int ins_read_reg_led_pwm_start_10(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), *buffer);
}
int ins_read_reg_led_pwm_end_10(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), *buffer);
}
int ins_read_reg_led_pwm_start_11(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), *buffer);
}
int ins_read_reg_led_pwm_end_11(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), *buffer);
}
int ins_read_reg_trig_start_0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), *buffer);
}
int ins_read_reg_trig_end_0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), *buffer);
}
int ins_read_reg_trig_start_1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), *buffer);
}
int ins_read_reg_trig_end_1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), *buffer);
}
int ins_read_reg_trig_start_2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), *buffer);
}
int ins_read_reg_trig_end_2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), *buffer);
}
int ins_read_reg_trig_start_3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), *buffer);
}
int ins_read_reg_trig_end_3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), *buffer);
}
int ins_read_reg_trig_start_4(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), *buffer);
}
int ins_read_reg_trig_end_4(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), *buffer);
}
int ins_read_reg_trig_start_5(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), *buffer);
}
int ins_read_reg_trig_end_5(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), *buffer);
}
int ins_read_reg_trig_out_polar(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), *buffer);
}



//loopmode
int ins_read_reg_dds_phase(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), *buffer);
}
int ins_read_reg_dds_inc(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), *buffer);
}
int ins_read_reg_exp_cycle(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), *buffer);
}
int ins_read_reg_trigger_gap(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), *buffer);
}
int ins_read_reg_pic_num(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), *buffer);
}
int ins_read_reg_sw_status(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), *buffer);
}
int ins_read_reg_sw_shift(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), *buffer);
}
int ins_read_reg_sw_loop_gap(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), *buffer);
}
int ins_read_reg_sw_loop_num(BoardID id, uint32_t* buffer) 
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), *buffer);
}
int ins_read_reg_dds_phase_offset(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), *buffer);
}

//11.23
extern "C" INSCAMERA_API int ins_read_reg_dds_direction_x(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_dds_direction_y(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_current_offset(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_current_offset), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_dac_value_forward(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_forward), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_dac_value_backward(BoardID id, uint32_t * buffer)
{ 
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_value_backward), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_sw_wait(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_wait), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_camera_delay(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_camera_cycle(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle), *buffer);
}

extern "C" INSCAMERA_API int ins_read_reg_dac_req(BoardID id, uint32_t * buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dac_req), *buffer);
}
//写死地址
extern "C" INSCAMERA_API int ins_read_reg_dds_phase_y(BoardID id, uint32_t * buffer)
{
	return read_register(id,4152, *buffer);
}



//test
int ins_read_reg_dac_value_x0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), *buffer);
}
int ins_read_reg_sw_value_x0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), *buffer);
}
int ins_read_reg_mos_value_x0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), *buffer);
}
int ins_read_reg_trigger_req_x0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), *buffer);
}
int ins_read_reg_dac_value_x1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), *buffer);
}
int ins_read_reg_sw_value_x1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), *buffer);
}
int ins_read_reg_mos_value_x1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), *buffer);
}
int ins_read_reg_trigger_req_x1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), *buffer);
}
int ins_read_reg_dac_value_x2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), *buffer);
}
int ins_read_reg_sw_value_x2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), *buffer);
}
int ins_read_reg_mos_value_x2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), *buffer);
}
int ins_read_reg_trigger_req_x2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), *buffer);
}
int ins_read_reg_dac_value_x3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), *buffer);
}
int ins_read_reg_sw_value_x3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), *buffer);
}
int ins_read_reg_mos_value_x3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), *buffer);
}
int ins_read_reg_trigger_req_x3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), *buffer);
}
int ins_read_reg_dac_value_y0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), *buffer);
}
int ins_read_reg_sw_value_y0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), *buffer);
}
int ins_read_reg_mos_value_y0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), *buffer);
}
int ins_read_reg_trigger_req_y0(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), *buffer);
}
int ins_read_reg_dac_value_y1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), *buffer);
}
int ins_read_reg_sw_value_y1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), *buffer);
}
int ins_read_reg_mos_value_y1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), *buffer);
}
int ins_read_reg_trigger_req_y1(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), *buffer);
}
int ins_read_reg_dac_value_y2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), *buffer);
}
int ins_read_reg_sw_value_y2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), *buffer);
}
int ins_read_reg_mos_value_y2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), *buffer);
}
int ins_read_reg_trigger_req_y2(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), *buffer);
}
int ins_read_reg_dac_value_y3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), *buffer);
}
int ins_read_reg_sw_value_y3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), *buffer);
}
int ins_read_reg_mos_value_y3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), *buffer);
}
int ins_read_reg_trigger_req_y3(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), *buffer);
}
//core
int ins_read_reg_core_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), *buffer);
}
int ins_read_reg_core_mode(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), *buffer);
}
//step forward
int ins_read_reg_step_en(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_en), *buffer);
}

int ins_read_reg_step_pic_num(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_pic_num), *buffer);
}

int ins_read_reg_step_x_seq(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_x_seq), *buffer);
}

int ins_read_reg_step_y_seq(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_forward_MODULE->reg_step_y_seq), *buffer);
}
//step phase
int ins_read_reg_step_phase_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_0), *buffer);
}

int ins_read_reg_step_phase_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_1), *buffer);
}

int ins_read_reg_step_phase_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_2), *buffer);
}

int ins_read_reg_step_phase_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_3), *buffer);
}

int ins_read_reg_step_phase_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_4), *buffer);
}

int ins_read_reg_step_phase_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_5), *buffer);
}

int ins_read_reg_step_phase_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_6), *buffer);
}

int ins_read_reg_step_phase_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_7), *buffer);
}

int ins_read_reg_step_phase_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_8), *buffer);
}

int ins_read_reg_step_phase_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_9), *buffer);
}

int ins_read_reg_step_phase_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_10), *buffer);
}

int ins_read_reg_step_phase_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_11), *buffer);
}

int ins_read_reg_step_phase_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_12), *buffer);
}

int ins_read_reg_step_phase_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_13), *buffer);
}

int ins_read_reg_step_phase_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_14), *buffer);
}

int ins_read_reg_step_phase_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_15), *buffer);
}

int ins_read_reg_step_phase_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_16), *buffer);
}

int ins_read_reg_step_phase_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_17), *buffer);
}

int ins_read_reg_step_phase_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_18), *buffer);
}

int ins_read_reg_step_phase_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_19), *buffer);
}

int ins_read_reg_step_phase_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_20), *buffer);
}

int ins_read_reg_step_phase_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_21), *buffer);
}

int ins_read_reg_step_phase_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_22), *buffer);
}

int ins_read_reg_step_phase_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_23), *buffer);
}

int ins_read_reg_step_phase_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_24), *buffer);
}

int ins_read_reg_step_phase_25(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_25), *buffer);
}

int ins_read_reg_step_phase_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_26), *buffer);
}

int ins_read_reg_step_phase_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_27), *buffer);
}

int ins_read_reg_step_phase_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_28), *buffer);
}

int ins_read_reg_step_phase_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_29), *buffer);
}

int ins_read_reg_step_phase_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_30), *buffer);
}

int ins_read_reg_step_phase_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_phase_31), *buffer);
}

int ins_read_reg_step_inc_0(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_0), *buffer);
}

int ins_read_reg_step_inc_1(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_1), *buffer);
}

int ins_read_reg_step_inc_2(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_2), *buffer);
}

int ins_read_reg_step_inc_3(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_3), *buffer);
}

int ins_read_reg_step_inc_4(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_4), *buffer);
}

int ins_read_reg_step_inc_5(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_5), *buffer);
}

int ins_read_reg_step_inc_6(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_6), *buffer);
}

int ins_read_reg_step_inc_7(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_7), *buffer);
}

int ins_read_reg_step_inc_8(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_8), *buffer);
}

int ins_read_reg_step_inc_9(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_9), *buffer);
}

int ins_read_reg_step_inc_10(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_10), *buffer);
}

int ins_read_reg_step_inc_11(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_11), *buffer);
}

int ins_read_reg_step_inc_12(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_12), *buffer);
}

int ins_read_reg_step_inc_13(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_13), *buffer);
}

int ins_read_reg_step_inc_14(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_14), *buffer);
}

int ins_read_reg_step_inc_15(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_15), *buffer);
}

int ins_read_reg_step_inc_16(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_16), *buffer);
}

int ins_read_reg_step_inc_17(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_17), *buffer);
}

int ins_read_reg_step_inc_18(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_18), *buffer);
}

int ins_read_reg_step_inc_19(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_19), *buffer);
}

int ins_read_reg_step_inc_20(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_20), *buffer);
}

int ins_read_reg_step_inc_21(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_21), *buffer);
}

int ins_read_reg_step_inc_22(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_22), *buffer);
}

int ins_read_reg_step_inc_23(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_23), *buffer);
}

int ins_read_reg_step_inc_24(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_24), *buffer);
}

int ins_read_reg_step_inc_25(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_25), *buffer);
}

int ins_read_reg_step_inc_26(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_26), *buffer);
}

int ins_read_reg_step_inc_27(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_27), *buffer);
}

int ins_read_reg_step_inc_28(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_28), *buffer);
}

int ins_read_reg_step_inc_29(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_29), *buffer);
}

int ins_read_reg_step_inc_30(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_30), *buffer);
}

int ins_read_reg_step_inc_31(BoardID id, uint32_t* buffer)
{
	return read_register(id, (uint16_t) & (CONTROLLER_step_phase_MODULE->reg_step_inc_31), *buffer);
}





//wave list
int ins_read_reg_ram_cfg_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), *buffer);
}
int ins_read_reg_ram_whrl(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), *buffer);
}
int ins_read_reg_ram_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), *buffer);
}
//wave
int ins_read_reg_ram_wdata(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), *buffer);
}
int ins_read_reg_ram_rdata(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), *buffer);
}
int ins_read_reg_ram_req(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), *buffer);
}
int ins_read_reg_ram_done(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), *buffer);
}
int ins_read_reg_wave_start_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), *buffer);
}
int ins_read_reg_wave_end_addr(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), *buffer);
}
//adjust
int ins_read_reg_adjust_en(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), *buffer);
}
int ins_read_reg_adjust_gain(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), *buffer);
}
int ins_read_reg_adjust_offset(BoardID id,uint32_t* buffer)
{
    return read_register(id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), *buffer);
}


//num_check
int ins_read_reg_num_check_clr(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_num_check_clr), *buffer);
}

int ins_read_reg_io_in_0_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_0_num), *buffer);
}
int ins_read_reg_io_in_1_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_in_1_num), *buffer);
}
int ins_read_reg_encoder_a_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_a_num), *buffer);
}

int ins_read_reg_encoder_b_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_encoder_b_num), *buffer);
}

int ins_read_reg_io_out_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_io_out_num), *buffer);
}

int ins_read_reg_mos_req_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_req_num), *buffer);
}

int ins_read_reg_mos_ack_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_mos_ack_num), *buffer);
}

int ins_read_reg_dds_req_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_req_num), *buffer);
}

int ins_read_reg_dds_ack_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dds_ack_num), *buffer);
}

int ins_read_reg_dac_req_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_req_num), *buffer);
}

int ins_read_reg_dac_ack_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_dac_ack_num), *buffer);
}

int ins_read_reg_sw_req_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_req_num), *buffer);
}

int ins_read_reg_sw_ack_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sw_ack_num), *buffer);
}

int ins_read_reg_reg_req_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_req_num), *buffer);
}

int ins_read_reg_reg_ack_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_reg_ack_num), *buffer);
}

int ins_read_reg_sum_err_num(BoardID id, uint32_t* buffer)
{
    return read_register(id, (uint16_t)(CHECK_num_check_MODULE->reg_sum_err_num), *buffer);
}

//miss_check
int ins_read_reg_core_status(BoardID id, uint32_t* buffer) 
{
    return read_register(id, (uint16_t)(CHECK_miss_check_MODULE->reg_core_status), *buffer);
}

#endif

#if 0




static const char* user_u32toStr(uint32_t aInt)
{
    static char HEXSEQ[] = "0123456789abcdef";
    static string lRegAddrStr;
    lRegAddrStr.clear();
    for (int8_t i = 7; i >= 0; i--)
    {
        int8_t tChar = (aInt >> (4 * i)) & 0xf;
        lRegAddrStr.push_back(HEXSEQ[tChar]);
    }
    return lRegAddrStr.c_str();
}


//ch传001,boardcount板子id, writeFile要写入的文件，buffer存放读出来的数据，addr为寄存器地址,读不上来就再读三次
static int write_file(const char* ch, int boardcount, ofstream* writeFile, uint16_t* buffer, uint16_t addr)
{
    int y = 0;
    int flag = 0;
    int ret = 0;
    uint32_t buffer_u32 = 0;
    for (int id = 1; id < boardcount + 1; id++ ) {
        for (; y < 3; y++ )
        { 
            ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, addr, buffer);
            if (ret == 0)
            { break; }
            if ((ret != 0) && (y != 2))
                continue;
            else if (y == 2)
            {
                flag++;
                buffer[0] = 0xffff;
                buffer[1] = 0xffff;
            }
        } 
        buffer_u32 = ((uint32_t)buffer[0] << 16) | (uint32_t)buffer[1];
        //cout << "buffer[0]" << buffer[0]<<endl;
        //cout << "buffer[1]" << buffer[1] << endl;
        //cout << "buffer_u32" << buffer_u32 << endl;
        *writeFile << "_S_" << ch << "_" << id << "_" << user_u32toStr(buffer_u32) << "_E_;\n";
        
    }
    return flag;
}

//写寄存器值，格式为寄存器名称、地址、板选号、写入值
int save_config_file(int boardcount, const char* _config_filename)
{
    if (!((boardcount == 1) || (boardcount == 3) || (boardcount == 5)))
        return -1;
    uint16_t buffer[2] = { 0 };
   
    string endname = ".icf";
    string temp(_config_filename);
    int found = temp.find(endname);
    if (found == string::npos) return -1;//文件地址错误
    //以写模式打开文件
    //ofstream outfile;
    //out表示只写，trunc表示截断，当不存在文件将新建文件，当存在则清空重新写
    //ofstream writeFile(_config_filename, std::ios::out | std::ios::trunc);
    //ofstream writeFile(_config_filename, std::ios::out | std::ios::app);

    const int startingLine = 9;

    // 读取前八行
    std::ifstream inFile(_config_filename);
    if (!inFile.is_open()) {
        std::cerr << "无法打开文件进行读取：" << _config_filename << std::endl;
        return -2;
    }

    std::vector<std::string> lines;
    std::string line;
    for (int i = 0; i < startingLine - 1 && std::getline(inFile, line); ++i) {
        lines.push_back(line);
    }

    inFile.close();

    // 打开文件并设置为在文件末尾追加
    std::ofstream writeFile(_config_filename, std::ios::out | std::ios::trunc);

    if (!writeFile.is_open()) {
        std::cerr << "无法打开文件进行写入：" << _config_filename << std::endl;
        return -2;
    }

    // 写入前八行
    for (const auto& savedLine : lines) {
        writeFile << savedLine << '\n';
    }

    cout << "start save registers configure file" << endl;
    string start_str = "--->INS_2D5_Registers_Configure_File:";
    //向文件写入用户输入的数据
    writeFile << start_str << endl;
    int flag = 0;
    {
        int ret = 0;
        if (0 != write_file("001", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("002", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("003", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("004", boardcount, &writeFile, buffer, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        //用slave测试C#
        //writeFile.close();
        //return 0;
        if(0 != write_file("005", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("006", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("007", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("008", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("009", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("010", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("011", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("012", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("013", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("015", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("014", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("016", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("017", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("018", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("019", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("020", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("021", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("022", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("023", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("024", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("025", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("026", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("027", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("028", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("029", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("030", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("031", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("032", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("033", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("034", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("035", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("036", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("037", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("038", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("039", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("040", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("041", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("042", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("043", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("044", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("045", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("046", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("047", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("048", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("049", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("050", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("051", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("052", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("053", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("054", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("055", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("056", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("057", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("058", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("059", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("060", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("061", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("062", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("063", boardcount, &writeFile, buffer, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("064", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_cycle)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("065", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_camera_delay)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("066", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("067", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("068", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("069", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("070", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("071", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("072", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("073", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("074", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("075", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("076", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_x)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("077", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_direction_y)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("078", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("079", boardcount, &writeFile, buffer, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("080", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en)))  {
			writeFile.close();
			std::remove(_config_filename);
			return -1;
		}
        if(0 != write_file("081", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain))) {
            writeFile.close();
            std::remove(_config_filename);
            return -1;
        }
        if (0 != write_file("082", boardcount, &writeFile, buffer, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset))) {
            writeFile.close();
            std::remove(_config_filename);
            return -1;
        }
        
    }
    //关闭打开的文件
    writeFile.close();
    return 0;
    
}





int LoadRegisters(BoardID id, uint16_t addr, uint16_t* buffer)
{
    int ret = 0;
    int i = 0;
    while (i++ < 3)
    { 
        ret = Ins_Modbus_udp::get_instance()->ins_modbus_udp_write(id, addr, buffer);
        if (ret == 0) { break; }
    }
    return ret;
}

int load_config_file(int _board_num, const char* _config_filename)
{
    
    PRINTTIME("start load");
    int flag = 0;
    if (!((_board_num == 1) || (_board_num == 3) ||  (_board_num == 5)))
        return -3;

    int ret = 0;//返回值
    if (_config_filename) 
    {
        const char* fileExtension = strrchr(_config_filename, '.'); // 获取最后一个点的位置
        if (!(fileExtension && strcmp(fileExtension, ".icf") == 0)) {
            std::cout << "读取失败;" << endl;
            return -1;  // 文件后缀bu是 ".icf"
        }
    }
    ifstream readFile(_config_filename, std::ios::in);
    if (!readFile.is_open()) 
    {
        std::cout << "打开失败;" << endl;
        return -1;
    }
    string l_readstr;
    string l_tmpstr;
    uint32_t l_writeNum;
    uint32_t l_writeBoard;

    char* pEndTemp;
    uint32_t l_writeBuffer;
    l_readstr.clear();
    
    int start_line_num = 0;//出现"--->INS_2D5_Registers_Configure_File:"的行数
    while(true)
    { 
        start_line_num++;
        std::getline(readFile, l_readstr);
        if (strcmp(l_readstr.c_str(), "--->INS_2D5_Registers_Configure_File:") == 0)
        {
            std::cout << l_readstr << endl;
            break;
        }
    }
    // 除了"--->INS_2D5_Registers_Configure_File:"这一行，获取后面文件行数
    int lineCount = 0;
    std::string line;
    while (std::getline(readFile, line)) {
        ++lineCount;
    }
    if (lineCount != _board_num * 82) {  //寄存器数量变化要改此处
        return -2;
    }
    // 将文件指针返回到开头
    readFile.clear();  // 清除文件状态
    readFile.seekg(0, std::ios::beg);
    while (true)
    {
        std::getline(readFile, l_readstr);
        if (strcmp(l_readstr.c_str(), "--->INS_2D5_Registers_Configure_File:") == 0)
        {
            std::cout << l_readstr << endl;
            break;
        }
    }

    int cycle_index = 0;//循环次数
         
     while (true)
    {
         //PRINTTIME("循环开始");
         cycle_index++;
         //std::cout <<dec<< cycle_index << std::endl;
        if (readFile.eof()) {
            // Code to handle reaching the end of the file
            break;
        }
        std::getline(readFile, l_readstr);
        if (l_readstr == "")
        {
            if (cycle_index == lineCount+1) { return 0; }
            return -1;//lRtVal = INS_Result::INS_FILE_OPEN_ERR;
        }
        l_tmpstr = l_readstr.substr(0, 3);//从0开始读3个
        if (l_tmpstr != "_S_")
        {
            continue;
        }
        l_tmpstr = l_readstr.substr(3, 3);
        l_writeNum = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 10);
        l_tmpstr = l_readstr.substr(7, 1);
        l_writeBoard = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 10);
        if (!((l_writeBoard == 1) || (l_writeBoard == 2)||(l_writeBoard == 3) || (l_writeBoard == 4) || (l_writeBoard == 5)))
            return -1;
        l_tmpstr = l_readstr.substr(9, 8);
        l_writeBuffer = (uint32_t)strtoul(&(l_tmpstr[0]), &pEndTemp, 16);
        l_writeBuffer = (l_writeBuffer << 16) | (l_writeBuffer >> 16);
        //PrintTime("开始写寄存器");
        switch (l_writeNum) {
        case 1:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
            if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVCIE_DAC_MODULE->reg_mos_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 2:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVCIE_DAC_MODULE->reg_dds_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 3:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVCIE_DAC_MODULE->reg_dac_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 4:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVCIE_DAC_MODULE->reg_sw_ack_time), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 5:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 6:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 7:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 8:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_num), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 9:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 10:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_width), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 11:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_delay), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 12:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_pulse), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 13:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_trigger_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 14:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_phase), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 15:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 16:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 17:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_ignore), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 18:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_div), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 19:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_width), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 20:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_location), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 21:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 22:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 23:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_cfg_MODULE->reg_slave_device), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 24:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_cnt_max), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 25:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trigger_multi_en), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 26:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 27:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_0), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 28:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_0), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 29:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_1), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 30:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_1), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 31:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_2), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 32:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_2), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 33:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_3), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 34:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_3), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 35:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_4), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 36:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_4), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 37:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_5), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 38:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_5), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 39:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_6), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 40:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_6), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 41:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_7), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 42:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_7), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 43:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_8), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 44:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_8), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 45:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_9), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 46:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_9), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 47:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_10), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 48:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_10), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 49:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_start_11), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 50:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_led_pwm_end_11), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 51:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_0), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 52:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_0), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 53:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_1), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 54:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_1), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 55:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_2), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 56:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_2), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 57:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_3), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 58:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_3), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 59:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_4), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 60:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_4), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 61:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_start_5), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 62:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_end_5), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 63:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(DEVICE_trig_out_MODULE->reg_trig_out_polar), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 64:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_camera_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 65:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_camera_delay), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 66:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_dds_phase), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 67:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_dds_inc), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 68:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_exp_cycle), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 69:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_trigger_gap), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 70:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_pic_num), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 71:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_sw_status), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 72:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_sw_shift), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 73:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 74:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 75:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 76:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_dds_direction_x), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 77:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_loop_mode_MODULE->reg_dds_direction_y), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 78:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_core_ctrl_MODULE->reg_core_en), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 79:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t)&(CONTROLLER_core_ctrl_MODULE->reg_core_mode), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 80:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 81:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        case 82:
            //std::this_thread::sleep_for(std::chrono::milliseconds(MODBUS_SLEEP_TIME));
			if (LoadRegisters((BoardID)l_writeBoard, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), (uint16_t*)(&l_writeBuffer)) != 0) {
                readFile.close();
                cout<<++flag<<endl;//return -1;
            }
            break;
        default:
            cout << "无对应序号";
        }
     }
     l_writeBuffer = 1;
     for (l_writeBoard = 0; l_writeBoard <_board_num; l_writeBoard++)
     { 
	    if (LoadRegisters((BoardID)l_writeBoard, 0x1024, (uint16_t*)(&l_writeBuffer)) != 0) 
        {
		    readFile.close();
		    cout << ++flag << endl;//return -1;
	    }
     }
     readFile.close();
     PRINTTIME("end");
     if (flag != 0)
     {
         return flag;
     }
     return 0;
}

#endif // 0

