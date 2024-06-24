#pragma once
#include "inscp_udp.h"
#include "ins_global.h"
#ifdef _MSC_VER
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��
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
    //modbus_udp�ĳ�ʼ��
    int initmodbus_udp(const char* host, const char* local_ip, uint16_t port = 502);//host������ԴIP��local_ip��������ip
    //�Զ����Э���ʼ��
    int init_ins_modbus_udp(const char* host, const char* local_ip, uint16_t port = 65530);//host������ԴIP��local_ip��������ip
    void modbus_udp_close() const;
    int modbus_udp_write_single_register(BoardID id, uint16_t address,  uint16_t* buffer);
    int modbus_udp_write_registers(BoardID id, uint16_t address, uint16_t amount, const uint16_t* value);
    int modbus_udp_read_registers(BoardID id, uint16_t address, uint16_t amount, uint16_t* buffer);
    int update_udp_write_registers(BoardID id, uint16_t address, uint16_t amount, const uint16_t* value);//��������ר��
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

    //��鷵�����Ƿ������
    int modbuserror_handle(unsigned char* msg, int func);
    int continous_receive(uint8_t(&send_data)[SENDBUFFERNUM], uint16_t* buffer, int func);
};

//INSCAMERA_API void PrintTime();

class INSCAMERA_API InsPMDL_lightSouce : public modbus_udp
{
public:


    InsPMDL_lightSouce(Ins2D5LightDeviceAndLocalIP _device) :mDevice(_device) {};
    ~InsPMDL_lightSouce() = default;


	//----------0315����
	uint32_t MOSACKZ;
	uint32_t DDSACKZ;
	uint32_t PicNumZ;
	uint32_t SWGAPZ;
	uint32_t TriigerGapZ;
	uint32_t SWLoopNumZ;
	uint32_t CamTriNumZ;

	//----------
    InsResult init_communication();
    //----------��д�����Ĵ���------------//0304��д
    InsResult ins_write(BoardID id, uint16_t address, uint16_t* buffer);
    InsResult ins_read(BoardID id, uint16_t address, uint16_t* buffer);
    InsResult read_register(BoardID _id, uint16_t _address, uint32_t& _buffer);//��ǰ�����Ļ�����ƴ�õ�32λ
    InsResult write_register(BoardID _id, uint16_t _address, uint32_t _buffer);
	//������������д����Ĵ���------------
	InsResult write_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer);
	InsResult read_registers(BoardID _id, uint16_t _address, uint16_t reg_num, uint16_t* _buffer);//ֻ֧�ֶ�һ����������


	InsResult save_SN(uint16_t* _buffer);
	InsResult read_SN(uint16_t* _buffer);


#if  1





	double Calculate16kLinePeriod(double x);
	double Calculate8kLinePeriod(double x);

    //-------------------------------����----------------------
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


	InsResult ins_write_Laser(BoardID id, uint16_t address, uint16_t* buffer); //0419 �����״�

	uint16_t getTriggerFre(int modeIndex);
	//����ȡ��ص�ʱ��Ҫ����֡��������Ƶ
	InsResult FreeRunForFocus(int _boardnum, int _picture_num, int _index,int CamType);

	//----------λ��ѡ��------------
	int select_direction(int index ,int _borad_num);//

	//----------��������------------
    //���õ�ǰϵͳɨ������_line_scanning_period(��λ��΢��),�ͻ�ȡ��Ӧ����Ƶ_line_fre(��λ��KHz),��ɨ������Ӧ�����ع�ʱ��+2,���뷶Χ��4~100��
    //�Զ���������ع����������
	int set_scanning_period(int _board_num, int _picture_um, double _line_scanning_period, double& _line_fre, double& _TriExposure,int CamType);
	//��õ�ǰϵͳɨ������_line_scanning_period(��λ��΢��)����Ƶ_line_fre(��λ��KHz)
    int get_scanning_period(double& _line_scanning_period, double& _line_fre);
	//���ô���ģʽ������ʹ�ܿ����������ʼȡͼ�����ʹ��
	int set_trigger_mode_on(int _index);
	//����ʹ�ܹ�
	int set_trigger_mode_off();

    //----------��������������------------
    //������У�飬A-B��ȡ��1����0��
    int set_a_b_phase_inversion_on_off(int _index);
    //��������,0-�����г̣�1-���˸��٣�2-���˺��ԣ�3-˫��ɨ��
    int set_trigger_direction(int _index);
    //����������ģʽ��0-A�������أ����������1-A�������أ�˫�������1-A��˫���أ�˫�������A&B��˫���أ�˫��
    int set_encoder_trigger_mode(int _index);
    //���÷�Ƶ
    int set_frequency_division(uint32_t _frequency_division);
    //���ñ�Ƶ
    int set_frequency_multiplier(uint32_t _frequency_multiplier);

    //��ȡ��������,0-�����г̣�1-���˸��٣�2-���˺��ԣ�3-˫��ɨ��
    int get_trigger_direction(uint32_t& _index);
    //��ȡ����������ģʽ��0-A�������أ����������1-A�������أ�˫�������1-A��˫���أ�˫�������A&B��˫���أ�˫��
    int get_encoder_trigger_mode(uint32_t& _index);
    //��ȡ��Ƶ
    int get_frequency_division(uint32_t& _index);
    //��ȡ��Ƶ
    int get_frequency_multiplier(uint32_t& _index);
    //----------IO��������------------

    //����io����ģʽ
    //0-ģʽ1��IO��0����ʼ��IO��1������,
    //1-ģʽ2��IO��1����ʼ��IO��0������,
    //2-ģʽ3��IO��0����ʼ������̶���������,
    //3-ģʽ4��IO��1����ʼ������̶���������,
    //4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
    //5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
    //6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
    //7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
    int set_io_trigger_mode(int _board_num, int _index);
    //����io��������
    int set_io_trigger_num(int _board_num, uint32_t _trinum);
    //����io��������
    int set_io_pulse_count(int _board_num, uint32_t _count);
    //����io����������
    int set_io_trigger_pulse_width(int _board_num, uint32_t _width);
    //����io�����ӳ�
    int set_io_trigger_delay(int _board_num, uint32_t _delay);
    //����io��������
    int set_io_trigger_period(int _board_num, uint32_t _period);
    //����io�������뼫��,0-���֣�1-��ת
    int set_io_trigger_input_polarity(int _board_num, uint32_t _index);


    /*��ȡio����ģʽ,
    0-ģʽ1��IO��0����ʼ��IO��1������,
    1-ģʽ2��IO��1����ʼ��IO��0������,
    2-ģʽ3��IO��0����ʼ������̶���������,
    3-ģʽ4��IO��1����ʼ������̶���������,
    4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
    5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
    6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
    7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
    */
    int get_io_trigger_mode(uint32_t& _index);
    //��ȡio��������
    int get_io_trigger_num(uint32_t& _trinum);
    //��ȡio��������
    int get_io_pulse_count(uint32_t& _count);
    //��ȡio����������
    int get_io_trigger_pulse_width(uint32_t& _width);
    //��ȡio�����ӳ�
    int get_io_trigger_delay(uint32_t& _delay);
    //��ȡio��������
    int get_io_trigger_period(uint32_t& _period);
    //��ȡio�������뼫�ԣ�0-���֣�1-��ת
    int get_io_trigger_input_polarity(uint32_t& _index);

    //----------��Դ��������------------
    //���ù�Դ����ģʽ��0-����ģʽ��1-��ģʽ��2-��׼ģʽ��3-Сģʽ��4-��Сģʽ(ֻ�����������)
    int set_emission_mode(int _index, int _board_num);
    //��ȡ��ǰ��Դ����ģʽ0-����1-��2-��׼��3-С��4-��С
    int get_emission_mode(int& _index);
    //����X�����ƶ�����,0-����1-����
    int set_x_stripe_movement_direction(int _boardnum, int _index);
    //����Y�����ƶ�����,0-����1-����
    int set_y_stripe_movement_direction(int _boardnum, int _index);
    //����X������ʼ��λ,0-0�㣬1-90�㣬2-180�㣬3-270��
    int set_initial_phase_in_x_direction(int _boardnum, int _index);
    //����Y������ʼ��λ,0-0�㣬1-90�㣬2-180�㣬3-270��
    int set_initial_phase_in_y_direction(int _boardnum, int _index);

    //��ȡX�����ƶ�����,0-����1-����
    int get_x_stripe_movement_direction(uint32_t& _index);
    //��ȡY�����ƶ�����,0-����1-����
    int get_y_stripe_movement_direction(uint32_t& _index);
    //��ȡX������ʼ��λ
    int get_initial_phase_in_x_direction(uint32_t& _initial_phase);
    //��ȡY������ʼ��λ
    int get_initial_phase_in_y_direction(uint32_t& _initial_phase);
    //----------��Դ��������------------

    //--------------����������Դ����ģʽ--------------
    //��������ʹ�ܺ�ֹͣʹ��,_index:0-disable,1-enable
    int secondary_trigger_enable_and_disable(int _boardnum, double _index);

    //���ñ�����������������(��λ��΢��)
    int set_encoder_secondary_trigger_pulse_width(int _boardnum, double _encodewidth);
    //��������,0-�����г̣�1-���˸���(���򲻼�)��2-���˺��ԣ�3-˫��ɨ��
    int set_encoder_secondary_trigger_travel_direction(int _boardnum, int _index);
    //to do  û�ҵ�����������ģʽ
    //���ö���������Ƶ
    int set_secondary_frequency_division(uint32_t _frequency_division);
    //���ö���������Ƶ
    int set_secondary_frequency_multiplier(uint32_t _frequency_multiplier);
    //------------
    //����������������
    int set_software_trigger_trigger_count(uint32_t _count);
    //���ô�������
    int set_software_trigger_trigger_period(double _period);
    //���������������
    int get_software_trigger_trigger_count(uint32_t& _count);
    //��ô�������
    int get_software_trigger_trigger_period(double& _period);

    //-------IO����-----
    /*����io����ģʽ,
    0-ģʽ1��IO��0����ʼ��IO��1������,
    1-ģʽ2��IO��1����ʼ��IO��0������,
    2-ģʽ3��IO��0����ʼ������̶���������,
    3-ģʽ4��IO��1����ʼ������̶���������,
    4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
    5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
    6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
    7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
    */
    int set_io_secondary_trigger_mode(int _board_num, int _index);
    //����io��������
    int set_io_secondary_trigger_num(int _board_num, uint32_t _trinum);
    //����io��������
    int set_io_secondary_pulse_count(int _board_num, uint32_t _count);
    //����io����������
    int set_io_secondary_trigger_pulse_width(int _board_num, uint32_t _width);
    //����io�����ӳ�
    int set_io_secondary_trigger_delay(int _board_num, uint32_t _delay);
    //����io��������
    int set_io_secondary_trigger_period(int _board_num, uint32_t _period);
    //����io�������뼫��,0-���֣�1-��ת
    int set_io_secondary_trigger_input_polarity(int _board_num, uint32_t _index);

    /*��ȡio����ģʽ,
    0-ģʽ1��IO��0����ʼ��IO��1������,
    1-ģʽ2��IO��1����ʼ��IO��0������,
    2-ģʽ3��IO��0����ʼ������̶���������,
    3-ģʽ4��IO��1����ʼ������̶���������,
    4-ģʽ5��IO��0����Ч����������Ч���½��ؽ���,
    5-ģʽ6��IO��1����Ч����������Ч���½��ؽ���,
    6-ģʽ7�����IO"0"��Ϊ��ʼ������̶���������,
    7-ģʽ8�����IO"1"��Ϊ��ʼ������̶���������
    */
    int get_io_secondary_trigger_mode(uint32_t& _index);
    //��ȡio��������
    int get_io_secondary_trigger_num(uint32_t& _trinum);
    //��ȡio��������
    int get_io_secondary_pulse_count(uint32_t& _count);
    //��ȡio����������
    int get_io_secondary_trigger_pulse_width(uint32_t& _width);
    //��ȡio�����ӳ�
    int get_io_secondary_trigger_delay(uint32_t& _delay);
    //��ȡio��������
    int get_io_secondary_trigger_period(uint32_t& _period);
    //��ȡio�������뼫�ԣ�0-���֣�1-��ת
    int get_io_secondary_trigger_input_polarity(uint32_t& _index);
    //--------------����������Դ����ģʽ--------------




    //----------����ģʽ------------
    // ����ʹ�ܺ�ֹͣʹ��,_index:0-disable,1-enable,_quantity:��չ�����������,һ��Ϊ1����λ���Σ�
    int extended_trigger_enable_and_disable(int _boardnum, double _index, int _quantity);
    //������չ�����������(��λ����)
    int set_extended_trigger_output_quantity(int _board_num, uint32_t _quantity);
    //������չ��������ӳ�(��λ��΢��)
    int set_extended_trigger_output_delay(int _board_num, uint32_t _delay);
    //������չ����������(��λ��΢��)
    int set_extended_trigger_output_width(int _board_num, uint32_t _width);
    //������չ�����������,0-�����أ�1-�½���
    int set_extended_trigger_output_polarity(int _board_num, int _index);

    //��ȡ��չ�����������(��λ����)
    int get_extended_trigger_output_quantity(uint32_t& _quantity);
    //��ȡ��չ��������ӳ�(��λ��΢��)
    int get_extended_trigger_output_delay(uint32_t& _delay);
    //��ȡ��չ����������(��λ��΢��)
    int get_extended_trigger_output_width(uint32_t& _width);
    //��ȡ��չ�����������,0-�����أ�1-�½���
    int get_extended_trigger_output_polarity(uint32_t& _index);
    //----------����ģʽ------------



    //----------�߼�����------------
    //���ù�Դ��Ӧʱ��(��λ��΢��)ֵ���룾0
    int set_light_source_response_time(int _board_num, double _delay);
    //���ù�Դ�������(��λ��΢��)ֵ����<=ϵͳ��ɨ�����ڵ�һ��
    int set_light_source_trigger_output_width(int _board_num, double _width);
    //���ù�Դ���ȵ���(��λ��%),��������ӦΪ���� , ��Χ��1~100
    int set_light_source_brightness_adjustment(int _board_num, uint32_t _percentage);
    //���ñ�������������(��λ��΢��)
    int set_encoder_trigger_pulse_width(int _board_num, double _encode_width);
    //���ù�Դ�����������(��λ����)
    int set_light_source_trigger_output_quantity(int _board_num, uint32_t _quantity);

    //��ȡ��Դ��Ӧʱ��(��λ��΢��)
    int get_light_source_response_time(double& _delay);
    //��ȡ��Դ�������(��λ��΢��)
    int get_light_source_trigger_output_width(int _board_num, double& _width);
    //��ȡ��Դ���ȵ���(��λ��%),���Ϊ���� , ��Χ��1~100
    int get_light_source_brightness_adjustment(int _board_num, uint32_t& _percentage);
    //��ȡ��������������(��λ��΢��)
    int get_encoder_trigger_pulse_width(int _board_num, double& _encode_width);
    //��ȡ��Դ�����������(��λ����)
    int get_light_source_trigger_output_quantity(uint32_t& _quantity);


    //������������(��λ����)ֵ���룾0
    int set_soft_trigger_num(uint32_t _num);
    //������������(��λ��΢��)
    int set_soft_trigger_period(double _period);

    //��ȡ��������(��λ����)
    int get_soft_trigger_num(uint32_t& _num);
    //��ȡ��������(��λ��΢��)
    int get_soft_trigger_period(double& _period);

    //----------�߼�����------------
#endif //  0


//��ȡ�汾��
	int get_microblaze_version(uint32_t * buffer);

	// �������ã�����ֵΪif_config_in_device_ok(int board_num)�ķ���ֵ
	int save_config_in_device(int board_num);
	//��ȡ�Ƿ����óɹ�
	int if_config_in_device_ok(int board_num);
	//���������أ���1������0��
	int write_extern_laser1(uint16_t * buffer);
	int write_extern_laser2(uint16_t * buffer);
	//����������ǰֵ
	int read_extern_laser1(uint16_t * buffer);
	int read_extern_laser2(uint16_t * buffer);
	//�����λ�ˣ�pirchΪǰ����
	int read_extern_pirch(double* buffer);
	//�����λ�ˣ�rollΪ���Ҹ���
	int read_extern_roll(double* buffer);
	//�����λ�ˣ�rollΪˮƽ��ת��
	int read_extern_yaw(double* buffer);
	//����Դλ�ˣ�pirchΪǰ����
	int read_internl_pirch(double* buffer);
	//����Դλ�ˣ�rollΪ���Ҹ���
	int read_internl_roll(double* buffer);
	//����Դλ�ˣ�rollΪˮƽ��ת��
	int read_internl_yaw(double* buffer);
	//��������и�
	int save_camera_height(uint32_t * buffer);
	int read_camera_height(uint32_t * buffer);

	//��ʱ��������и�
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


	//11.23�����Ĵ���
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

	InsResult ins_write_reg_trigger_level(BoardID id, uint32_t * buffer);//��ַ���ܼӽṹ�壬��ַд��



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

	//1123����
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
	//��ַд��
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
	//11.23����

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
	//11.23����
	InsResult ins_read_reg_dds_direction_x(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dds_direction_y(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_current_offset(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_forward(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_value_backward(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_sw_wait(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_camera_delay(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_camera_cycle(BoardID id, uint32_t * buffer);
	InsResult ins_read_reg_dac_req(BoardID id, uint32_t * buffer);
	//д����ַ
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

	//����ͼ��������ļ�
	int save_config_file(int _boardcount, const char* _config_filename);
	int LoadRegisters(BoardID id, uint16_t addr, uint16_t* buffer);
	//�����һ����ӱ���ͼ��ض���1.3S
	int load_config_file(int _board_num, const char* _config_filename);


	int FreeRunSinToContinue(int boardNum);





    //----------��������------------ //0304




	//============0304
	// 
	// 
	// //��Դ��ʼ��
	int Ins_Initial(int BoardNum);



	// д��Ĵ��� ��16λ��
	int write_fpga_reg(int32_t index, uint32_t _addr, uint32_t _value);

	// ���������ԴIO��������
	int Ins_SetIOTriggerCycle(uint32_t _index, double _time_ms);

	// ���������ԴIO��������
	int Ins_SetIOTriggerNum(uint32_t _index, int _num);

	// ���������ԴIO����ʹ��
	int Ins_SetIOTriggerEn(uint32_t _index, bool _enable);

	//��Դ����ʱ��(��λ��ms)
	int Ins_SetIlluminationTime(uint32_t _index, double _time);

	//���󷢹�ģʽ 0����,1��2��׼��3С��4��С
	int Ins_SetLightMode(uint32_t _index, int index);

	//����ȡ�񿪣�  
	int Ins_SetFreeRunOn(uint32_t _index);

	//����ȡ��أ�  
	int Ins_SetFreeRunOff(uint32_t _index);

	//������8�ţ�����
	int Ins_Trigger_8_ImagesOn(int BoardNum, uint32_t _time, uint32_t delaytime);

	//������8�ţ��أ�
	int Ins_Trigger_8_ImagesOff(int BoardNum);

	//���ù�Դ���ȵ���(��λ��%),��������ӦΪ���� , ��Χ��1~100������ɨһ��
	int Ins_SetBrightness(uint32_t _index, uint32_t _percentage);

	// Ϊ����ƽ�������뷢�� ���ù�Դ����ʱ��Ϊ�̶�10ms//����Ϊ��Դ����ʱ��10ms�¶�Ӧ�������㼰����
	int Ins_SetIlluminationTime10ms(uint32_t index, double _time);

	//��������ģʽʹ��
	int Ins_SetArrayEn(uint32_t _index, bool _enable);


	//�����������ԡ�������һ��
	int Ins_SetFirstLightProperty(uint32_t _index);

	//�����������ԡ���ȫ����
	int Ins_Set8LightProperty(uint32_t _index, uint32_t _illuminate_mode);

	//���÷���˳�򣨷���
	int Ins_SetDirection(int _BoardNum, int _Direction);


	//��Դ��λ����  //_index��ԭ�����,_Direction�Ƕ�,LightMode����ģʽ
    int Ins_SetPhaseOffset(uint32_t _index, int _Direction, int LightMode);
	





private:
    Ins2D5LightDeviceAndLocalIP mDevice;
};






//ԭ���ĵ���ģʽ
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

