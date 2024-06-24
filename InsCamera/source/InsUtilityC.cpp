#include "../include/InsUtilityC.h"
#include "../include/inscp_udp.h"
#include "../include/Ins_Utility_proc.h"


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
typedef WAVE_wave_list* WAVE_wave_list_PTR;
WAVE_wave_list_PTR WAVE_wave_list_MODULE = WAVE_wave_list_PTR(WAVE_wave_list_ADDR);
typedef WAVE_wave_adjust* WAVE_wave_adjust_PTR;
WAVE_wave_adjust_PTR WAVE_wave_adjust_MODULE = WAVE_wave_adjust_PTR(WAVE_wave_adjust_ADDR);
typedef CHECK_num_check* CHECK_num_check_PTR;
CHECK_num_check_PTR CHECK_num_check_MODULE = CHECK_num_check_PTR(CHECK_num_check_ADDR);
typedef CHECK_miss_check* CHECK_miss_check_PTR;
CHECK_miss_check_PTR CHECK_miss_check_MODULE = CHECK_miss_check_PTR(CHECK_miss_check_ADDR);

INSCP ins_cp;

void InitLocalInfo(const char* mip, const char* mnet_mask, const char* mgateway)
{
	LOCALDEVICE::get_instance()->init(mip, mnet_mask, mgateway);
}
int SearchDevice(Device* ins_device)
{
	//initial local device ip
	int ret = ins_cp.search_device();
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
	return ret;
}

int InsResetDeviceID(const char* mip)
{
	return ins_cp.ins_reset_device_id(mip);
}

int factory_configuration(Device* ins_device)
{
    strcpy(DEVICE::get_instance()->net_mask,ins_device->net_mask );
    strcpy(DEVICE::get_instance()->gateway,ins_device->gateway);
    strcpy(DEVICE::get_instance()->mac_addr, ins_device->mac_addr);
    strcpy(DEVICE::get_instance()->manufacturer_name, ins_device->manufacturer_name);
    strcpy(DEVICE::get_instance()->device_vision, ins_device->device_vision);
    strcpy(DEVICE::get_instance()->model_name, ins_device->model_name);
    strcpy(DEVICE::get_instance()->device_model, ins_device->device_model);
    strcpy(DEVICE::get_instance()->length, ins_device->length);
    strcpy(DEVICE::get_instance()->SN, ins_device->SN);
    strcpy(DEVICE::get_instance()->ip_addr, ins_device->ip_addr);
    //ÖÃÁã
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
    int ret =  ins_cp.factory_configuration();
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

int save_config_file(int boardcount, const char* _config_filename)
{
    
    return save_config_file(boardcount, _config_filename);
}
int load_config_file(int boardcount, const char* _config_filename)
{
    
    return load_config_file(boardcount, _config_filename);
}

int InitModbusUdp(const char* mip)
{
	return Ins_Modbus_udp::get_instance()->initmodbus_udp(mip, 502);
}

int InsModbusUdpRead(int id, uint16_t address, uint16_t* buffer)
{
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id,address, buffer);

}

int InsModbusUdpWrite(int id, uint16_t address, uint16_t* buffer)
{
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, address, buffer);
}


//system
int write_reg_soft_reset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (SYSTEM_MODULE->reg_soft_reset), buffer);
}
int write_reg_test(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (SYSTEM_MODULE->reg_test), buffer);
}


//DAC
int read_reg_mos_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), buffer);
}
int write_reg_mos_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_mos_ack_time), buffer);
}

int read_reg_dds_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), buffer);
}
int write_reg_dds_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dds_ack_time), buffer);
}

int read_reg_dac_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), buffer);
}
int write_reg_dac_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_dac_ack_time), buffer);
}
int read_reg_sw_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), buffer);
}
int write_reg_sw_ack_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVCIE_DAC_MODULE->reg_sw_ack_time), buffer);
}









//READ
// 
//system

int read_COMPANY(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->COMPANY), buffer);
}
int read_DEVICE(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->DEVICE), buffer);
}
int read_DEVICE_SUB(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->DEVICE_SUB), buffer);
}
int read_INTERFACETYPE(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->INTERFACETYPE), buffer);
}
int read_YEAR(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->YEAR), buffer);
}
int read_HOUR(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->HOUR), buffer);
}
int read_REV(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->REV), buffer);
}
int read_reg_soft_reset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->reg_soft_reset), buffer);
}
int read_reg_test(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->reg_test), buffer);
}
int read_reg_device_temp(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (SYSTEM_MODULE->reg_device_temp), buffer);
}


// phy
//int write_reg_phy_rst(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t)(DEVICE_PHY_ADDR), buffer);
//}

/*

//MPU0
int write_reg_iic_whrl(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), buffer);
}
int write_reg_iic_dev_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), buffer);
}
int write_reg_iic_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), buffer);
}
int write_reg_iic_wdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), buffer);
}
//int write_reg_iic_rdata(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), buffer);
//}
int write_reg_iic_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), buffer);
}
//int write_reg_iic_done(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), buffer);
//}
*/


//laser
int write_reg_laser_status(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t)(DEVICE_laser_ADDR), buffer);
}

/*
////mpu1
//int write_reg_mcu1_angle_x(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), buffer);
//}
//int write_reg_mcu1_angle_y(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), buffer);
//}
//int write_reg_mcu1_angle_z(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), buffer);
//}
//int write_reg_mcu1_temp(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), buffer);
//}
int write_reg_mcu1_cfg_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), buffer);
}
int write_reg_mcu1_cfg_value(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), buffer);
}
int write_reg_mcu1_cfg_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), buffer);
}
//int write_reg_mcu1_cfg_done(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), buffer);
//}
int write_reg_mcu1_bps(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), buffer);
}
*/


//trig_cfg
int write_reg_soft_trigger_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), buffer);
}
int write_reg_soft_trigger_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), buffer);
}
int write_reg_soft_trigger_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), buffer);
}
int write_reg_trigger_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), buffer);
}
int write_reg_trigger_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), buffer);
}
int write_reg_trigger_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), buffer);
}
int write_reg_trigger_width(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), buffer);
}
int write_reg_trigger_delay(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), buffer);
}
int write_reg_trigger_pulse(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), buffer);
}
int write_reg_trigger_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), buffer);
}
int write_reg_trigger_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), buffer);
}
int write_reg_encoder_phase(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), buffer);
}
int write_reg_encoder_cnt_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), buffer);
}
int write_reg_encoder_dis_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), buffer);
}
int write_reg_encoder_ignore(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), buffer);
}
int write_reg_encoder_div(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), buffer);
}
int write_reg_encoder_width(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), buffer);
}
//int write_reg_encoder_location(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), buffer);
//}
int write_reg_encoder_multi_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_en), buffer);
}
int write_reg_encoder_multi_coe(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), buffer);
}
//int write_reg_encoder_a_cnt(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), buffer);
//}
//int write_reg_encoder_b_cnt(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), buffer);
//}
int write_reg_encoder_clr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), buffer);
}
int write_reg_encoder_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), buffer);
}
int write_reg_slave_device(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), buffer);
}
int write_reg_status_cnt_clr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), buffer);
}
////trig_out
int write_reg_exp_chan(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), buffer);
}
int write_reg_exposure_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), buffer);
}
int write_reg_led_cnt_max(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), buffer);
}
int write_reg_trigger_multi_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), buffer);
}
int write_reg_led_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), buffer);
}
int write_reg_led_pwm_start_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), buffer);
}
int write_reg_led_pwm_end_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), buffer);
}
int write_reg_led_pwm_start_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), buffer);
}
int write_reg_led_pwm_end_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), buffer);
}
int write_reg_led_pwm_start_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), buffer);
}
int write_reg_led_pwm_end_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), buffer);
}
int write_reg_led_pwm_start_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), buffer);
}
int write_reg_led_pwm_end_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), buffer);
}
int write_reg_led_pwm_start_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), buffer);
}
int write_reg_led_pwm_end_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), buffer);
}
int write_reg_led_pwm_start_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), buffer);
}
int write_reg_led_pwm_end_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), buffer);
}
int write_reg_led_pwm_start_6(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), buffer);
}
int write_reg_led_pwm_end_6(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), buffer);
}
int write_reg_led_pwm_start_7(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), buffer);
}
int write_reg_led_pwm_end_7(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), buffer);
}
int write_reg_led_pwm_start_8(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), buffer);
}
int write_reg_led_pwm_end_8(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), buffer);
}
int write_reg_led_pwm_start_9(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), buffer);
}
int write_reg_led_pwm_end_9(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), buffer);
}
int write_reg_led_pwm_start_10(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), buffer);
}
int write_reg_led_pwm_end_10(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), buffer);
}
int write_reg_led_pwm_start_11(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), buffer);
}
int write_reg_led_pwm_end_11(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), buffer);
}
int write_reg_trig_start_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), buffer);
}
int write_reg_trig_end_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), buffer);
}
int write_reg_trig_start_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), buffer);
}
int write_reg_trig_end_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), buffer);
}
int write_reg_trig_start_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), buffer);
}
int write_reg_trig_end_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), buffer);
}
int write_reg_trig_start_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), buffer);
}
int write_reg_trig_end_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), buffer);
}
int write_reg_trig_start_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), buffer);
}
int write_reg_trig_end_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), buffer);
}
int write_reg_trig_start_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), buffer);
}
int write_reg_trig_end_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), buffer);
}
int write_reg_trig_out_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_out_polar), buffer);
}



//loopmode
int write_reg_dds_phase(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), buffer);
}
int write_reg_dds_inc(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), buffer);
}
int write_reg_exp_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), buffer);
}
int write_reg_trigger_gap(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), buffer);
}
int write_reg_pic_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), buffer);
}
int write_reg_sw_status(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), buffer);
}
int write_reg_sw_shift(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), buffer);
}
int write_reg_sw_loop_gap(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), buffer);
}
int write_reg_sw_loop_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), buffer);
}
int write_reg_dds_phase_offset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), buffer);
}

//test
int write_reg_dac_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), buffer);
}
int write_reg_sw_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), buffer);
}
int write_reg_mos_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), buffer);
}
int write_reg_trigger_req_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), buffer);
}
int write_reg_dac_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), buffer);
}
int write_reg_sw_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), buffer);
}
int write_reg_mos_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), buffer);
}
int write_reg_trigger_req_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), buffer);
}
int write_reg_dac_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), buffer);
}
int write_reg_sw_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), buffer);
}
int write_reg_mos_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), buffer);
}
int write_reg_trigger_req_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), buffer);
}
int write_reg_dac_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), buffer);
}
int write_reg_sw_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), buffer);
}
int write_reg_mos_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), buffer);
}
int write_reg_trigger_req_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), buffer);
}
int write_reg_dac_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), buffer);
}
int write_reg_sw_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), buffer);
}
int write_reg_mos_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), buffer);
}
int write_reg_trigger_req_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), buffer);
}
int write_reg_dac_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), buffer);
}
int write_reg_sw_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), buffer);
}
int write_reg_mos_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), buffer);
}
int write_reg_trigger_req_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), buffer);
}
int write_reg_dac_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), buffer);
}
int write_reg_sw_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), buffer);
}
int write_reg_mos_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), buffer);
}
int write_reg_trigger_req_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), buffer);
}
int write_reg_dac_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), buffer);
}
int write_reg_sw_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), buffer);
}
int write_reg_mos_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), buffer);
}
int write_reg_trigger_req_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), buffer);
}
//core
int write_reg_core_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), buffer);
}
int write_reg_core_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), buffer);
}
int write_reg_ram_cfg_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), buffer);
}
int write_reg_ram_whrl(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), buffer);
}
int write_reg_ram_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), buffer);
}
//wave
int write_reg_ram_wdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), buffer);
}
//int write_reg_ram_rdata(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), buffer);
//}
int write_reg_ram_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), buffer);
}
//int write_reg_ram_done(int id, uint16_t* buffer)
//{
//    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), buffer);
//}
int write_reg_wave_start_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), buffer);
}
int write_reg_wave_end_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), buffer);
}
//adjust
int write_reg_adjust_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), buffer);
}
int write_reg_adjust_gain(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), buffer);
}
int write_reg_adjust_offset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), buffer);
}




//READ
// phy
//int read_reg_phy_rst(int id, uint16_t* buffer)
//{
//     return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t)(DEVICE_PHY_ADDR), buffer);
//}

//temp
int read_reg_mainboard_temp(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t)(DEVICE_temp_ADDR), buffer);
}


/*
//MPU0
int read_reg_iic_whrl(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_whrl), buffer);
}
int read_reg_iic_dev_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_dev_addr), buffer);
}
int read_reg_iic_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_addr), buffer);
}
int read_reg_iic_wdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_wdata), buffer);
}
int read_reg_iic_rdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_rdata), buffer);
}
int read_reg_iic_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_req), buffer);
}
int read_reg_iic_done(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU0_MODULE->reg_iic_done), buffer);
}


////mpu1
int read_reg_mcu1_angle_x(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_x), buffer);
}
int read_reg_mcu1_angle_y(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_y), buffer);
}
int read_reg_mcu1_angle_z(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_angle_z), buffer);
}
int read_reg_mcu1_temp(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_temp), buffer);
}
int read_reg_mcu1_cfg_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_addr), buffer);
}
int read_reg_mcu1_cfg_value(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_value), buffer);
}
int read_reg_mcu1_cfg_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_req), buffer);
}
int read_reg_mcu1_cfg_done(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_cfg_done), buffer);
}
int read_reg_mcu1_bps(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_MPU1_MODULE->reg_mcu1_bps), buffer);
}
*/

//laser
int read_reg_laser_status(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t)(DEVICE_laser_ADDR), buffer);
}

//trig_cfg
int read_reg_soft_trigger_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_cycle), buffer);
}
int read_reg_soft_trigger_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_num), buffer);
}
int read_reg_soft_trigger_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_soft_trigger_en), buffer);
}
int read_reg_trigger_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_cycle), buffer);
}
int read_reg_trigger_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_num), buffer);
}
int read_reg_trigger_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_mode), buffer);
}
int read_reg_trigger_width(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_width), buffer);
}
int read_reg_trigger_delay(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_delay), buffer);
}
int read_reg_trigger_pulse(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_pulse), buffer);
}
int read_reg_trigger_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_polar), buffer);
}
int read_reg_trigger_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_trigger_en), buffer);
}
int read_reg_encoder_phase(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_phase), buffer);
}
int read_reg_encoder_cnt_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_cnt_mode), buffer);
}
int read_reg_encoder_dis_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_dis_mode), buffer);
}
int read_reg_encoder_ignore(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_ignore), buffer);
}
int read_reg_encoder_div(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_div), buffer);
}
int read_reg_encoder_width(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_width), buffer);
}
int read_reg_encoder_location(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_location), buffer);
}
int read_reg_encoder_multi_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), buffer);
}
int read_reg_encoder_multi_coe(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_multi_coe), buffer);
}
int read_reg_encoder_a_cnt(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_a_cnt), buffer);
}
int read_reg_encoder_b_cnt(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_b_cnt), buffer);
}
int read_reg_encoder_clr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_clr), buffer);
}
int read_reg_encoder_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_encoder_en), buffer);
}
int read_reg_slave_device(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_slave_device), buffer);
}
int read_reg_status_cnt_clr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_cfg_MODULE->reg_status_cnt_clr), buffer);
}
////trig_out
int read_reg_exp_chan(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exp_chan), buffer);
}
int read_reg_exposure_time(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_exposure_time), buffer);
}
int read_reg_led_cnt_max(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_cnt_max), buffer);
}
int read_reg_trigger_multi_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trigger_multi_en), buffer);
}
int read_reg_led_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_polar), buffer);
}
int read_reg_led_pwm_start_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), buffer);
}
int read_reg_led_pwm_end_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), buffer);
}
int read_reg_led_pwm_start_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), buffer);
}
int read_reg_led_pwm_end_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), buffer);
}
int read_reg_led_pwm_start_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), buffer);
}
int read_reg_led_pwm_end_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), buffer);
}
int read_reg_led_pwm_start_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), buffer);
}
int read_reg_led_pwm_end_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), buffer);
}
int read_reg_led_pwm_start_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), buffer);
}
int read_reg_led_pwm_end_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), buffer);
}
int read_reg_led_pwm_start_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), buffer);
}
int read_reg_led_pwm_end_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), buffer);
}
int read_reg_led_pwm_start_6(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), buffer);
}
int read_reg_led_pwm_end_6(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), buffer);
}
int read_reg_led_pwm_start_7(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), buffer);
}
int read_reg_led_pwm_end_7(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), buffer);
}
int read_reg_led_pwm_start_8(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), buffer);
}
int read_reg_led_pwm_end_8(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), buffer);
}
int read_reg_led_pwm_start_9(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), buffer);
}
int read_reg_led_pwm_end_9(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), buffer);
}
int read_reg_led_pwm_start_10(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), buffer);
}
int read_reg_led_pwm_end_10(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), buffer);
}
int read_reg_led_pwm_start_11(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), buffer);
}
int read_reg_led_pwm_end_11(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), buffer);
}
int read_reg_trig_start_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), buffer);
}
int read_reg_trig_end_0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), buffer);
}
int read_reg_trig_start_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), buffer);
}
int read_reg_trig_end_1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), buffer);
}
int read_reg_trig_start_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), buffer);
}
int read_reg_trig_end_2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), buffer);
}
int read_reg_trig_start_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), buffer);
}
int read_reg_trig_end_3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), buffer);
}
int read_reg_trig_start_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), buffer);
}
int read_reg_trig_end_4(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), buffer);
}
int read_reg_trig_start_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), buffer);
}
int read_reg_trig_end_5(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), buffer);
}
int read_reg_trig_out_polar(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), buffer);
}
//loopmode
int read_reg_dds_phase(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), buffer);
}
int read_reg_dds_inc(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), buffer);
}
int read_reg_exp_cycle(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), buffer);
}
int read_reg_trigger_gap(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), buffer);
}
int read_reg_pic_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), buffer);
}
int read_reg_sw_status(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), buffer);
}
int read_reg_sw_shift(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), buffer);
}
int read_reg_sw_loop_gap(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_gap), buffer);
}
int read_reg_sw_loop_num(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_loop_num), buffer);
}
int read_reg_dds_phase_offset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase_offset), buffer);
}

//test
int read_reg_dac_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), buffer);
}
int read_reg_sw_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), buffer);
}
int read_reg_mos_value_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), buffer);
}
int read_reg_trigger_req_x0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), buffer);
}
int read_reg_dac_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), buffer);
}
int read_reg_sw_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), buffer);
}
int read_reg_mos_value_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), buffer);
}
int read_reg_trigger_req_x1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), buffer);
}
int read_reg_dac_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), buffer);
}
int read_reg_sw_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), buffer);
}
int read_reg_mos_value_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), buffer);
}
int read_reg_trigger_req_x2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), buffer);
}
int read_reg_dac_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), buffer);
}
int read_reg_sw_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), buffer);
}
int read_reg_mos_value_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), buffer);
}
int read_reg_trigger_req_x3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), buffer);
}
int read_reg_dac_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), buffer);
}
int read_reg_sw_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), buffer);
}
int read_reg_mos_value_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), buffer);
}
int read_reg_trigger_req_y0(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), buffer);
}
int read_reg_dac_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), buffer);
}
int read_reg_sw_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), buffer);
}
int read_reg_mos_value_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), buffer);
}
int read_reg_trigger_req_y1(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), buffer);
}
int read_reg_dac_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), buffer);
}
int read_reg_sw_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), buffer);
}
int read_reg_mos_value_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), buffer);
}
int read_reg_trigger_req_y2(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), buffer);
}
int read_reg_dac_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), buffer);
}
int read_reg_sw_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), buffer);
}
int read_reg_mos_value_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), buffer);
}
int read_reg_trigger_req_y3(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), buffer);
}
//core
int read_reg_core_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), buffer);
}
int read_reg_core_mode(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), buffer);
}
int read_reg_ram_cfg_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), buffer);
}
int read_reg_ram_whrl(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), buffer);
}
int read_reg_ram_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), buffer);
}
//wave
int read_reg_ram_wdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), buffer);
}
int read_reg_ram_rdata(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_rdata), buffer);
}
int read_reg_ram_req(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), buffer);
}
int read_reg_ram_done(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_done), buffer);
}
int read_reg_wave_start_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), buffer);
}
int read_reg_wave_end_addr(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), buffer);
}
//adjust
int read_reg_adjust_en(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), buffer);
}
int read_reg_adjust_gain(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), buffer);
}
int read_reg_adjust_offset(int id, uint16_t* buffer)
{
    return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), buffer);
}




/*

int write_reg_soft_reset(uint16_t* buffer)
{
	return op1.ins_system.write_reg_soft_reset(buffer);
}
int write_reg_test(uint16_t* buffer)
{
	return op1.ins_system.write_reg_test(buffer);
}

// DAC

int read_reg_mos_ack_time(uint16_t* buffer)
{
	return op1.ins_device.read_reg_mos_ack_time(buffer);
}

int write_reg_mos_ack_time(uint16_t* buffer)
{
	return op1.ins_device.write_reg_mos_ack_time(buffer);
}

int read_reg_dds_ack_time(uint16_t* buffer)
{
	return op1.ins_device.read_reg_dds_ack_time(buffer);
}

int write_reg_dds_ack_time(uint16_t* buffer)
{
	return op1.ins_device.write_reg_dds_ack_time(buffer);
}

int read_reg_dac_ack_time(uint16_t* buffer)
{
	return op1.ins_device.read_reg_dac_ack_time(buffer);
}

int write_reg_dac_ack_time(uint16_t* buffer)
{
	return op1.ins_device.write_reg_dac_ack_time(buffer);
}

int read_reg_sw_ack_time(uint16_t* buffer)
{
	return op1.ins_device.read_reg_sw_ack_time(buffer);
}

int write_reg_sw_ack_time(uint16_t* buffer)
{
	return op1.ins_device.write_reg_sw_ack_time(buffer);
}

//READ
// 
//system
int read_COMPANY(uint16_t* buffer)
{
	return op1.ins_system.read_COMPANY(buffer);
}

int read_DEVICE(uint16_t* buffer)
{
	return op1.ins_system.read_DEVICE(buffer);
}

int read_DEVICE_SUB(uint16_t* buffer)
{
	return op1.ins_system.read_DEVICE_SUB(buffer);
}

int read_INTERFACETYPE(uint16_t* buffer)
{
	return op1.ins_system.read_INTERFACETYPE(buffer);
}

int read_YEAR(uint16_t* buffer)
{
	return op1.ins_system.read_YEAR(buffer);
}

int read_HOUR(uint16_t* buffer)
{
	return op1.ins_system.read_HOUR(buffer);
}

int read_REV(uint16_t* buffer)
{
	return op1.ins_system.read_REV(buffer);
}

int read_reg_soft_reset(uint16_t* buffer)
{
	return op1.ins_system.read_reg_soft_reset(buffer);
}

int read_reg_test(uint16_t* buffer)
{
	return op1.ins_system.read_reg_test(buffer);
}

int read_reg_device_temp(uint16_t* buffer)
{
	return op1.ins_system.read_reg_device_temp(buffer);
}

// phy
int write_reg_phy_rst(uint16_t* buffer)
{
	return op1.ins_device.write_reg_phy_rst(buffer);
}

// MPU0
int write_reg_iic_whrl(uint16_t* buffer)
{
	return op1.ins_device.write_reg_iic_whrl(buffer);
}

int write_reg_iic_dev_addr(uint16_t* buffer)
{
	return op1.ins_device.write_reg_iic_dev_addr(buffer);
}

int write_reg_iic_addr(uint16_t* buffer)
{
	return op1.ins_device.write_reg_iic_addr(buffer);
}

int write_reg_iic_wdata(uint16_t* buffer)
{
	return op1.ins_device.write_reg_iic_wdata(buffer);
}

int write_reg_iic_req(uint16_t* buffer)
{
	return op1.ins_device.write_reg_iic_req(buffer);
}

// laser
int write_reg_laser_status(uint16_t* buffer)
{
	return op1.ins_device.write_reg_laser_status(buffer);
}

int write_reg_mcu1_cfg_addr(uint16_t* buffer)
{
	return op1.ins_device.write_reg_mcu1_cfg_addr(buffer);
}

int write_reg_mcu1_cfg_value(uint16_t* buffer)
{
	return op1.ins_device.write_reg_mcu1_cfg_value(buffer);
}

int write_reg_mcu1_cfg_req(uint16_t* buffer)
{
	return op1.ins_device.write_reg_mcu1_cfg_req(buffer);
}
int write_reg_mcu1_bps(uint16_t* buffer)
{
	return op1.ins_device.write_reg_mcu1_bps(buffer);
}

// trig_cfg
int write_reg_soft_trigger_cycle(uint16_t* buffer)
{
	return op1.ins_device.write_reg_soft_trigger_cycle(buffer);
}

int write_reg_soft_trigger_num(uint16_t* buffer)
{
	return op1.ins_device.write_reg_soft_trigger_num(buffer);
}

int write_reg_soft_trigger_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_soft_trigger_en(buffer);
}

int write_reg_trigger_cycle(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_cycle(buffer);
}

int write_reg_trigger_num(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_num(buffer);
}

int write_reg_trigger_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_mode(buffer);
}

int write_reg_trigger_width(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_width(buffer);
}

int write_reg_trigger_delay(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_delay(buffer);
}

int write_reg_trigger_pulse(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_pulse(buffer);
}

int write_reg_trigger_polar(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_polar(buffer);
}

int write_reg_trigger_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_en(buffer);
}

int write_reg_encoder_phase(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_phase(buffer);
}

int write_reg_encoder_cnt_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_cnt_mode(buffer);
}

int write_reg_encoder_dis_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_dis_mode(buffer);
}

int write_reg_encoder_ignore(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_ignore(buffer);
}

int write_reg_encoder_div(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_div(buffer);
}

int write_reg_encoder_width(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_width(buffer);
}


// trig_cfg

int write_reg_soft_trigger_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_soft_trigger_en(buffer);
}

int write_reg_trigger_cycle(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_cycle(buffer);
}

int write_reg_trigger_num(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_num(buffer);
}

int write_reg_trigger_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_mode(buffer);
}

int write_reg_trigger_width(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_width(buffer);
}

int write_reg_trigger_delay(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_delay(buffer);
}

int write_reg_trigger_pulse(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_pulse(buffer);
}

int write_reg_trigger_polar(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_polar(buffer);
}

int write_reg_trigger_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_en(buffer);
}

int write_reg_encoder_phase(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_phase(buffer);
}

int write_reg_encoder_cnt_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_cnt_mode(buffer);
}

int write_reg_encoder_dis_mode(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_dis_mode(buffer);
}

int write_reg_encoder_ignore(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_ignore(buffer);
}

int write_reg_encoder_div(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_div(buffer);
}

int write_reg_encoder_width(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_width(buffer);
}

int write_reg_encoder_multi_coe(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_multi_coe(buffer);
}

int write_reg_encoder_clr(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_clr(buffer);
}

int write_reg_encoder_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_encoder_en(buffer);
}

int write_reg_slave_device(uint16_t* buffer)
{
	return op1.ins_device.write_reg_slave_device(buffer);
}

int write_reg_status_cnt_clr(uint16_t* buffer)
{
	return op1.ins_device.write_reg_status_cnt_clr(buffer);
}

// trig_out
int write_reg_exp_chan(uint16_t* buffer)
{
	return op1.ins_device.write_reg_exp_chan(buffer);
}

int write_reg_exposure_time(uint16_t* buffer)
{
	return op1.ins_device.write_reg_exposure_time(buffer);
}

int write_reg_led_cnt_max(uint16_t* buffer)
{
	return op1.ins_device.write_reg_led_cnt_max(buffer);
}

int write_reg_trigger_multi_en(uint16_t* buffer)
{
	return op1.ins_device.write_reg_trigger_multi_en(buffer);
}

int write_reg_led_polar(uint16_t* buffer)
{
	return op1.ins_device.write_reg_led_polar(buffer);
}

int write_reg_led_pwm_start_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), buffer);
}

int write_reg_led_pwm_end_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), buffer);
}

int write_reg_led_pwm_start_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_0), buffer);
}

int write_reg_led_pwm_end_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_0), buffer);
}

int write_reg_led_pwm_start_1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_1), buffer);
}

int write_reg_led_pwm_end_1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_1), buffer);
}

int write_reg_led_pwm_start_2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_2), buffer);
}

int write_reg_led_pwm_end_2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_2), buffer);
}

int write_reg_led_pwm_start_3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_3), buffer);
}

int write_reg_led_pwm_end_3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_3), buffer);
}

int write_reg_led_pwm_start_4(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_4), buffer);
}

int write_reg_led_pwm_end_4(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_4), buffer);
}

int write_reg_led_pwm_start_5(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_5), buffer);
}

int write_reg_led_pwm_end_5(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_5), buffer);
}

int write_reg_led_pwm_start_6(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_6), buffer);
}

int write_reg_led_pwm_end_6(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_6), buffer);
}

int write_reg_led_pwm_start_7(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_7), buffer);
}

int write_reg_led_pwm_end_7(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_7), buffer);
}

int write_reg_led_pwm_start_8(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_8), buffer);
}

int write_reg_led_pwm_end_8(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_8), buffer);
}

int write_reg_led_pwm_start_9(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_9), buffer);
}

int write_reg_led_pwm_end_9(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_9), buffer);
}

int write_reg_led_pwm_start_10(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_10), buffer);
}

int write_reg_led_pwm_end_10(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_10), buffer);
}

int write_reg_led_pwm_start_11(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_start_11), buffer);
}

int write_reg_led_pwm_end_11(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_led_pwm_end_11), buffer);
}

int write_reg_trig_start_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_0), buffer);
}

int write_reg_trig_end_0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_0), buffer);
}

int write_reg_trig_start_1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_1), buffer);
}

int write_reg_trig_end_1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_1), buffer);
}

int write_reg_trig_start_2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_2), buffer);
}

int write_reg_trig_end_2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_2), buffer);
}

int write_reg_trig_start_3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_3), buffer);
}

int write_reg_trig_end_3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_3), buffer);
}

int write_reg_trig_start_4(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_4), buffer);
}

int write_reg_trig_end_4(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_4), buffer);
}

int write_reg_trig_start_5(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_start_5), buffer);
}

int write_reg_trig_end_5(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (DEVICE_trig_out_MODULE->reg_trig_end_5), buffer);
}

int write_reg_dds_phase(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_phase), buffer);
}

int write_reg_dds_inc(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_dds_inc), buffer);
}

int write_reg_exp_cycle(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_exp_cycle), buffer);
}

int write_reg_trigger_gap(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_trigger_gap), buffer);
}

int write_reg_pic_num(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_pic_num), buffer);
}

int write_reg_sw_status(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_status), buffer);
}

int write_reg_sw_shift(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_loop_mode_MODULE->reg_sw_shift), buffer);
}

// Test Mode
int write_reg_dac_value_x0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x0), buffer);
}

int write_reg_sw_value_x0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x0), buffer);
}

int write_reg_mos_value_x0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x0), buffer);
}

int write_reg_trigger_req_x0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x0), buffer);
}

int write_reg_dac_value_x1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x1), buffer);
}

int write_reg_sw_value_x1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x1), buffer);
}

int write_reg_mos_value_x1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x1), buffer);
}

int write_reg_trigger_req_x1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x1), buffer);
}

int write_reg_dac_value_x2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x2), buffer);
}

int write_reg_sw_value_x2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x2), buffer);
}

int write_reg_mos_value_x2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x2), buffer);
}

int write_reg_trigger_req_x2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x2), buffer);
}

int write_reg_dac_value_x3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_x3), buffer);
}

int write_reg_sw_value_x3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_x3), buffer);
}

int write_reg_mos_value_x3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_x3), buffer);
}

int write_reg_trigger_req_x3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_x3), buffer);
}

int write_reg_dac_value_y0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y0), buffer);
}

int write_reg_sw_value_y0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y0), buffer);
}

int write_reg_mos_value_y0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y0), buffer);
}

int write_reg_trigger_req_y0(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y0), buffer);
}

int write_reg_dac_value_y1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y1), buffer);
}

int write_reg_sw_value_y1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y1), buffer);
}

int write_reg_mos_value_y1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y1), buffer);
}

int write_reg_trigger_req_y1(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y1), buffer);
}

int write_reg_dac_value_y2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y2), buffer);
}

int write_reg_sw_value_y2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y2), buffer);
}

int write_reg_mos_value_y2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y2), buffer);
}

int write_reg_trigger_req_y2(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y2), buffer);
}


// Test Mode (continued)
int write_reg_dac_value_y3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_dac_value_y3), buffer);
}

int write_reg_sw_value_y3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_sw_value_y3), buffer);
}

int write_reg_mos_value_y3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_mos_value_y3), buffer);
}

int write_reg_trigger_req_y3(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_test_mode_MODULE->reg_trigger_req_y3), buffer);
}

// Core Mode
int write_reg_core_en(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_en), buffer);
}

int write_reg_core_mode(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (CONTROLLER_core_ctrl_MODULE->reg_core_mode), buffer);
}

// Wave
int write_reg_ram_cfg_en(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_cfg_en), buffer);
}

int write_reg_ram_whrl(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_whrl), buffer);
}

int write_reg_ram_addr(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_addr), buffer);
}

int write_reg_ram_wdata(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_wdata), buffer);
}

int write_reg_ram_req(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_ram_req), buffer);
}

int write_reg_wave_start_addr(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_start_addr), buffer);
}

int write_reg_wave_end_addr(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_list_MODULE->reg_wave_end_addr), buffer);
}

// Adjust
int write_reg_adjust_en(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_en), buffer);
}

int write_reg_adjust_gain(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_gain), buffer);
}

int write_reg_adjust_offset(uint16_t* buffer) {
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_write((BoardID)id, (uint16_t) & (WAVE_wave_adjust_MODULE->reg_adjust_offset), buffer);
}

//READ
// phy
int read_reg_phy_rst(uint16_t* buffer)
{
	return Ins_Modbus_udp::get_instance()->ins_modbus_udp_read((uint16_t)(DEVICE_PHY_ADDR), buffer);
}





*/

