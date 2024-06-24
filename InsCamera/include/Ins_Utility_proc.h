#pragma once
#include <iostream>
#include "ins_global.h"



//读取版本号
//extern "C" INSCAMERA_API int get_microblaze_version(uint16_t * buffer);

// 保存配置，返回值为if_config_in_device_ok(int board_num)的返回值
extern "C" INSCAMERA_API int save_config_in_device(int board_num);
//读取是否配置成功
extern "C" INSCAMERA_API int if_config_in_device_ok(int board_num);
//激光器开关，置1开，置0关
extern "C" INSCAMERA_API int write_extern_laser1(void* ins_light,uint16_t * buffer);
extern "C" INSCAMERA_API int write_extern_laser2(void* ins_light,uint16_t * buffer);
//读激光器当前值
extern "C" INSCAMERA_API int read_extern_laser1(uint16_t * buffer);
extern "C" INSCAMERA_API int read_extern_laser2(uint16_t * buffer);
//读相机位姿，pirch为前向俯仰
extern "C" INSCAMERA_API int read_extern_pirch(void* ins_light,double * buffer);
//读相机位姿，roll为左右俯仰
extern "C" INSCAMERA_API int read_extern_roll(void* ins_light, double * buffer);
//读相机位姿，roll为水平旋转角
extern "C" INSCAMERA_API int read_extern_yaw(void* ins_light, double * buffer);
//读光源位姿，pirch为前向俯仰
extern "C" INSCAMERA_API int read_internl_pirch(void* ins_light, double * buffer);
//读光源位姿，roll为左右俯仰
extern "C" INSCAMERA_API int read_internl_roll(void* ins_light, double * buffer);
//读光源位姿，roll为水平旋转角
extern "C" INSCAMERA_API int read_internl_yaw(void* ins_light, double * buffer);
//保存相机行高
//extern "C" INSCAMERA_API int save_camera_height(void* ins_light,uint32_t * buffer);
//extern "C" INSCAMERA_API int read_camera_height(void* ins_light, uint32_t* buffer);



#if 1
	extern "C" INSCAMERA_API int ins_write_reg_soft_reset(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_test(BoardID id,uint32_t* buffer);

	extern "C" INSCAMERA_API int ins_read_COMPANY(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_DEVICE(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_DEVICE_SUB(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_INTERFACETYPE(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_YEAR(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_HOUR(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_REV(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_soft_reset(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_test(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_device_temp(BoardID id,uint32_t* buffer);
	//DAC
	extern "C" INSCAMERA_API int ins_read_reg_mos_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dds_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_ack_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_ack_time(BoardID id,uint32_t* buffer);
	//phy
	//extern "C" INSCAMERA_API int ins_write_reg_phy_rst(BoardID id,uint32_t* buffer);
	
	//temp
	//extern "C" INSCAMERA_API int ins_write_reg_mainboard_temp(BoardID id,uint32_t* buffer);
	//laser
	extern "C" INSCAMERA_API int ins_write_reg_laser_status(BoardID id, uint32_t* buffer);
	/*
	//mpu0
	extern "C" INSCAMERA_API int ins_write_reg_iic_whrl(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_dev_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_wdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_rdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_req(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_iic_done(BoardID id,uint32_t* buffer);
	//mpu1
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_angle_x(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_angle_y(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_angle_z(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_temp(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mcu1_bps(BoardID id,uint32_t* buffer);
	*/
	//trig_cfg
	extern "C" INSCAMERA_API int ins_write_reg_soft_trigger_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_soft_trigger_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_soft_trigger_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_width(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_delay(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_pulse(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_polar(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_phase(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_cnt_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_dis_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_ignore(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_div(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_width(BoardID id,uint32_t* buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_encoder_location(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_multi_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_multi_coe(BoardID id, uint32_t* buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_encoder_a_cnt(BoardID id,uint32_t* buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_encoder_b_cnt(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_clr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_slave_device(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_status_cnt_clr(BoardID id,uint32_t* buffer);


	//11.23新增寄存器
	extern "C" INSCAMERA_API int ins_write_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_num(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_width(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_delay(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_polar(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_trigger_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_phase(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_div(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_width(BoardID id, uint32_t * buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_location(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_clr(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_encoder_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer);

	extern "C" INSCAMERA_API int ins_write_reg_trigger_level(BoardID id, uint32_t * buffer);//地址不能加结构体，地址写死



	//trig_out
	extern "C" INSCAMERA_API int ins_write_reg_exp_chan(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_exposure_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_cnt_max(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_multi_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_polar(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_6(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_6(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_7(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_7(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_8(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_8(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_9(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_9(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_10(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_10(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_start_11(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_led_pwm_end_11(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_start_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_end_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trig_out_polar(BoardID id, uint32_t* buffer);
	//READ
	// phy
	//int ins_read_reg_phy_rst(BoardID id,uint32_t* buffer);
	//temp
	extern "C" INSCAMERA_API int ins_read_reg_mainboard_temp(BoardID id,uint32_t* buffer);
	//LASER
	extern "C" INSCAMERA_API int ins_read_reg_laser_status(BoardID id, uint32_t* buffer);
	 /*
	//mpu0
	extern "C" INSCAMERA_API int ins_read_reg_iic_whrl(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_dev_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_wdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_rdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_req(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_iic_done(BoardID id,uint32_t* buffer);
	//mpu1
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_angle_x(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_angle_y(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_angle_z(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_temp(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_cfg_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_cfg_value(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_cfg_req(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_cfg_done(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mcu1_bps(BoardID id,uint32_t* buffer);
	*/
	 //trig_cfg
	extern "C" INSCAMERA_API int ins_read_reg_soft_trigger_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_soft_trigger_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_soft_trigger_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_width(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_delay(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_pulse(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_polar(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_phase(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_cnt_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_dis_mode(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_ignore(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_div(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_width(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_location(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_multi_en(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_multi_coe(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_a_cnt(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_b_cnt(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_clr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_slave_device(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_status_cnt_clr(BoardID id,uint32_t* buffer);

	//1123新增
	extern "C" INSCAMERA_API int ins_read_reg_l1_soft_trigger_cycle(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_soft_trigger_num(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_soft_trigger_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_cycle(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_num(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_width(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_delay(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_pulse(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_polar(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_trigger_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_phase(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_cnt_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_dis_mode(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_ignore(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_div(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_width(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_location(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_multi_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_multi_coe(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_a_cnt(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_b_cnt(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_clr(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_encoder_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_l1_status_cnt_clr(BoardID id, uint32_t * buffer);
	//地址写死
	extern "C" INSCAMERA_API int ins_read_reg_trigger_level(BoardID id, uint32_t * buffer);




    //trig_out
	extern "C" INSCAMERA_API int ins_read_reg_exp_chan(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_exposure_time(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_cnt_max(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_multi_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_polar(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_6(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_6(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_7(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_7(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_8(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_8(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_9(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_9(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_10(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_10(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_start_11(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_led_pwm_end_11(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_4(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_start_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_end_5(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trig_out_polar(BoardID id, uint32_t* buffer);
	 //loop_mode
	extern "C" INSCAMERA_API int ins_write_reg_dds_phase(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dds_inc(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_exp_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_gap(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_pic_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_status(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_shift(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_loop_gap(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_loop_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dds_phase_offset(BoardID id, uint32_t* buffer);
	//11.23新增



	//test
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_trigger_req_y3(BoardID id,uint32_t* buffer);
	//core
	extern "C" INSCAMERA_API int ins_write_reg_core_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_core_mode(BoardID id,uint32_t* buffer);
	//step forward
	extern "C" INSCAMERA_API int ins_write_reg_step_en(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_pic_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_x_seq(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_y_seq(BoardID id, uint32_t* buffer);


	
	//loop_mode
	extern "C" INSCAMERA_API int ins_read_reg_dds_phase(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_inc(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_exp_cycle(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_gap(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_pic_num(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_status(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_shift(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_loop_gap(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_loop_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_phase_offset(BoardID id, uint32_t* buffer);
	//11.23新增
	extern "C" INSCAMERA_API int ins_read_reg_dds_direction_x(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_direction_y(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_current_offset(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_forward(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_backward(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_wait(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_camera_delay(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_camera_cycle(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_req(BoardID id, uint32_t * buffer);
	//写死地址
	extern "C" INSCAMERA_API int ins_read_reg_dds_phase_y(BoardID id, uint32_t * buffer);





	//test
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_x0(BoardID id,uint32_t* buffer);         
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_x0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_x1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_x2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_x3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_y0(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_y1(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_y2(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_value_y3(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_trigger_req_y3(BoardID id,uint32_t* buffer);
	//core
	extern "C" INSCAMERA_API int ins_read_reg_core_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_core_mode(BoardID id,uint32_t* buffer);

	//step forward
	extern "C" INSCAMERA_API int ins_read_reg_step_en(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_pic_num(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_x_seq(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_y_seq(BoardID id, uint32_t * buffer);

	//step phase read
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_phase_31(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_inc_31(BoardID id, uint32_t * buffer);

	//step phase write
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_phase_31(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_inc_31(BoardID id, uint32_t * buffer);

	//step base read
	extern "C" INSCAMERA_API int ins_read_reg_step_base_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_read_reg_step_base_31(BoardID id, uint32_t * buffer);

	//step base write
	extern "C" INSCAMERA_API int ins_write_reg_step_base_0(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_1(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_2(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_3(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_4(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_5(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_6(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_7(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_8(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_9(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_10(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_11(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_12(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_13(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_14(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_15(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_16(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_17(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_18(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_19(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_20(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_21(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_22(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_23(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_24(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_25(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_26(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_27(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_28(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_29(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_30(BoardID id, uint32_t * buffer);
	extern "C" INSCAMERA_API int ins_write_reg_step_base_31(BoardID id, uint32_t * buffer);

	//wave_list
	extern "C" INSCAMERA_API int ins_write_reg_ram_cfg_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_ram_whrl(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_ram_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_ram_wdata(BoardID id,uint32_t* buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_ram_rdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_ram_req(BoardID id,uint32_t* buffer);
	//extern "C" INSCAMERA_API int ins_write_reg_ram_done(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_wave_start_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_wave_end_addr(BoardID id,uint32_t* buffer);
	//adjust
	extern "C" INSCAMERA_API int ins_write_reg_adjust_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_adjust_gain(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_adjust_offset(BoardID id,uint32_t* buffer);
	
	//wave_list
	extern "C" INSCAMERA_API int ins_read_reg_ram_cfg_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_whrl(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_wdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_rdata(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_req(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_ram_done(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_wave_start_addr(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_wave_end_addr(BoardID id,uint32_t* buffer);
	//adjust
	extern "C" INSCAMERA_API int ins_read_reg_adjust_en(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_adjust_gain(BoardID id,uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_adjust_offset(BoardID id,uint32_t* buffer);
	//num_check
	extern "C" INSCAMERA_API int ins_write_reg_num_check_clr(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_io_in_0_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_io_in_1_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_a_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_encoder_b_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_io_out_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_mos_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dds_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dds_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_dac_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sw_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_reg_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_reg_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_write_reg_sum_err_num(BoardID id, uint32_t* buffer);
	//miss_check
	extern "C" INSCAMERA_API int ins_write_reg_core_status(BoardID id, uint32_t* buffer);
	//num_check
	extern "C" INSCAMERA_API int ins_read_reg_num_check_clr(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_io_in_0_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_io_in_1_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_a_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_encoder_b_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_io_out_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_mos_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dds_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_dac_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sw_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_reg_req_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_reg_ack_num(BoardID id, uint32_t* buffer);
	extern "C" INSCAMERA_API int ins_read_reg_sum_err_num(BoardID id, uint32_t* buffer);
 
	//miss_check
	extern "C" INSCAMERA_API int ins_read_reg_core_status(BoardID id, uint32_t* buffer);

#endif



	//保存和加载配置文件
	extern "C" INSCAMERA_API int save_config_file(int _boardcount, const char* _config_filename);//如果是一块板子保存和加载都是1.3S
	extern "C" INSCAMERA_API int load_config_file(int _board_num, const char* _config_filename);


