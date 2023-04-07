
#include "GlobalVarShared.h"

void Eeprom_Init(void);
void Eeprom_Save(void);

EEP_DATA0 Eeprom_Data0;
EEP_ALARM_FAULT1 Eeprom_Alarm_Fault_Value;
EEP_ALARM_FAULT2 Eeprom_Alarm_Fault_Time;
EEP_ALARM_FAULT1_R Eeprom_Alarm_Fault_Value_r;
EEP_ALARM_FAULT2_R Eeprom_Alarm_Fault_Time_r;
EEP_AFE Eeprom_Afe;
rt_uint8_t Sys_charge_full_flag;
rt_uint8_t read_data_eep;

static rt_uint16_t crc16(rt_uint16_t crc, rt_uint8_t const *buffer, rt_uint8_t len);

// CRC16对照表
rt_uint16_t const crc16_table[256] =
	{
		0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
		0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
		0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
		0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
		0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
		0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
		0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
		0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
		0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
		0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
		0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
		0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
		0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
		0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
		0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
		0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
		0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
		0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
		0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
		0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
		0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
		0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
		0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
		0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
		0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
		0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
		0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
		0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
		0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
		0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
		0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
		0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040};

rt_uint16_t crc16_byte(rt_uint16_t crc, const rt_uint8_t data)
{
	return (crc >> 8) ^ crc16_table[(crc ^ data) & 0xff];
}

static rt_uint16_t crc16(rt_uint16_t crc, rt_uint8_t const *buffer, rt_uint8_t len)
{
	while (len--)
	{
		crc = crc16_byte(crc, *buffer++);
	}

	return crc;
}

/** ************************************************************************
 * @brief   Eeprom-page0默认参数
 **************************************************************************/
void Reset_Eeprom0(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Data0.buf[j] = 0;
	}

	Eeprom_Data0.member.SOC = 125;
	Eeprom_Data0.member.SOH = 98;
	Eeprom_Data0.member.Work_Model = 0;
	Eeprom_Data0.member.Charge_Full_Enable = 0;
	Eeprom_Data0.member.Cycle_index = 0;
}

/** ************************************************************************
 * @brief   Eeprom-page0,用于存储SOC等参数
 * @brief   使用page6的第一个字节作为校验 64*6=0x180
 * @brief   如果板子是第一次上电，将默认值写入Eeprom
 **************************************************************************/
void Eeprom0_Init(void)
{	
	read_data_eep = M24256_Byte_Read(0x181);
	
	if(read_data_eep != 0xAA)
	{
			M24256_Byte_Write(0xAA,0x181);
			Reset_Eeprom0();
		  rt_thread_mdelay(50);
			M24256_Page_Write(&Eeprom_Data0.buf[0], 0, 64);	
			Eeprom_Data0.member.Charge_Full_Enable = 0;
	}
	else
	{
			M24256_Page_Read(&Eeprom_Data0.buf[0], 0, 64);
	}
	
	//上电读取上次是否满充
	if((Eeprom_Data0.member.Charge_Full_Enable > 1) || (Eeprom_Data0.member.Request_Full_Charge > 1) || (Eeprom_Data0.member.SOC == 0xff))
	{
			//用于处理读取Eeprom数据很大的情况
			Sys_charge_full_flag = 0;
			Sys_info.member.sys_charge_full_flag = 0;
			Reset_Eeprom0();
	}
	else
	{
			Sys_charge_full_flag = Eeprom_Data0.member.Charge_Full_Enable;
			Sys_info.member.sys_charge_full_flag = Eeprom_Data0.member.Charge_Full_Enable;	
	}
	
	Eeprom_Data0.member.Work_Model = 0;
}

/** ************************************************************************
 * @brief   Eeprom-page1默认参数
 **************************************************************************/
void Reset_Eeprom1(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Alarm_Fault_Value.buf[j] = 0;
	}

	Eeprom_Alarm_Fault_Value.member.Module_voltage_over[0] = 36.3 * 16; // 总电压充电过压，单位0.1V

	Eeprom_Alarm_Fault_Value.member.Module_voltage_over[1] = 35.3 * 16; // 总电压充电过压，单位0.1V

	Eeprom_Alarm_Fault_Value.member.Module_voltage_under[0] = 25 * 16;

	Eeprom_Alarm_Fault_Value.member.Module_voltage_under[1] = 31 * 16;

	Eeprom_Alarm_Fault_Value.member.Cell_vol_over[0] = 3630; // 充电过压，单位0.001V

	Eeprom_Alarm_Fault_Value.member.Cell_vol_over[1] = 3550;

	Eeprom_Alarm_Fault_Value.member.Cell_vol_under[0] = 2500;

	Eeprom_Alarm_Fault_Value.member.Cell_vol_under[1] = 3100;

	Eeprom_Alarm_Fault_Value.member.Cell_vol_differ[0] = 500;

	Eeprom_Alarm_Fault_Value.member.Cell_vol_differ[1] = 400;

	Eeprom_Alarm_Fault_Value.member.Charge_current_over[0] = 1500; // 单位0.1A

	Eeprom_Alarm_Fault_Value.member.Charge_current_over[1] = 1200;

	Eeprom_Alarm_Fault_Value.member.Discharge_current_over[0] = 1500; // 单位0.1A

	Eeprom_Alarm_Fault_Value.member.Discharge_current_over[1] = 1200;

	Eeprom_Alarm_Fault_Value.member.Discharge_current_over1[0] = 1500; // 单位0.1A

	Eeprom_Alarm_Fault_Value.member.Discharge_current_over1[1] = 1200;

	Eeprom_Alarm_Fault_Value.member.Charge_temp_over[0] = 400 + 530; // 充电温度过高，单位0.1°，偏移量40

	Eeprom_Alarm_Fault_Value.member.Charge_temp_over[1] = 400 + 480; // 充电温度过高，单位0.1°，偏移量40
    
    Eeprom_Alarm_Fault_Value.member.Charge_temp_under[0] = 400; //
    
    Eeprom_Alarm_Fault_Value.member.Charge_temp_under[1] = 400 + 20; //400

	Eeprom_Alarm_Fault_Value.member.Discharge_temp_over[0] = 400 + 550;

	Eeprom_Alarm_Fault_Value.member.Discharge_temp_over[1] = 400 + 540;

	Eeprom_Alarm_Fault_Value.member.Discharge_temp_under[0] = 400 - 280; // 400-280

	Eeprom_Alarm_Fault_Value.member.Discharge_temp_under[1] = 400 - 260; // 400-250

	Eeprom_Alarm_Fault_Value.member.Temp_differ[0] = 150;

	Eeprom_Alarm_Fault_Value.member.Temp_differ[1] = 70;

	Eeprom_Alarm_Fault_Value.member.Mos_temp_over = 400 + 900;
}

/** ************************************************************************
 * @brief   Eeprom-page1,用于存储保护参数
 * @brief   使用page6的第一个字节作为校验 64*6=0x180
 * @brief   如果板子是第一次上电，将默认值写入Eeprom
 **************************************************************************/
void Eeprom1_Init(void)
{
	rt_uint8_t data[64];
	rt_uint16_t j = 0, crc = 0, temp16 = 0;

	M24256_Page_Read(&data[0], 1, 64); // 将PAGE0的数据读入缓存
	crc = crc16(0xffff, &data[0], 62);
    Reset_Eeprom1();
    data[62] = Eeprom_Alarm_Fault_Value.buf[62];
    data[63] = Eeprom_Alarm_Fault_Value.buf[63];
	temp16 = (data[62] << 8) + data[63];

	if (crc == temp16)
	{
		for (j = 0; j < 64; j++)
		{
			Eeprom_Alarm_Fault_Value.buf[j] = data[j];
		}
	}
	else
	{
		Reset_Eeprom1();
		crc = crc16(0xffff, &Eeprom_Alarm_Fault_Value.buf[0], 62);
		Eeprom_Alarm_Fault_Value.buf[62] = (crc >> 8) & 0xff;
		Eeprom_Alarm_Fault_Value.buf[63] = crc & 0xff;
		M24256_Page_Write(&Eeprom_Alarm_Fault_Value.buf[0], 1, 64);
	}
}

/** ************************************************************************
 * @brief   Eeprom-page2默认参数
 **************************************************************************/
void Reset_Eeprom2(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Alarm_Fault_Time.buf[j] = 0;
	}
	Eeprom_Alarm_Fault_Time.member.Module_voltage_over_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Module_voltage_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Module_voltage_under_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Module_voltage_under_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_over_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_under_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_under_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_differ_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Cell_vol_differ_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Charge_current_over_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Charge_current_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time[0] = 1;

	Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time1[0] = 30;

	Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time1[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Charge_temp_over_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Charge_temp_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Charge_temp_under_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Charge_temp_under_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_temp_over_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_temp_over_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_temp_under_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Discharge_temp_under_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Temp_differ_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Temp_differ_time[1] = 5;

	Eeprom_Alarm_Fault_Time.member.Mos_temp_over_time[0] = 5;

	Eeprom_Alarm_Fault_Time.member.Mos_temp_over_time[1] = 5;
}

/** ************************************************************************
 * @brief   Eeprom-page2,用于存储保护时间参数
 * @brief   使用page6的第一个字节作为校验 64*6=0x180
 * @brief   如果板子是第一次上电，将默认值写入Eeprom
 **************************************************************************/
void Eeprom2_Init(void)
{
	rt_uint8_t data[64];
	rt_uint16_t j = 0, crc = 0, temp16 = 0;

	M24256_Page_Read(&data[0], 2, 64); // 将PAGE0的数据读入缓存
	crc = crc16(0xffff, &data[0], 62);
	temp16 = (data[62] << 8) + data[63];

	if (crc == temp16)
	{
		for (j = 0; j < 64; j++)
		{
			Eeprom_Alarm_Fault_Time.buf[j] = data[j];
		}
	}
	else
	{
		Reset_Eeprom2();
		crc = crc16(0xffff, &Eeprom_Alarm_Fault_Time.buf[0], 62);
		Eeprom_Alarm_Fault_Time.buf[62] = (crc >> 8) & 0xff;
		Eeprom_Alarm_Fault_Time.buf[63] = crc & 0xff;
		M24256_Page_Write(&Eeprom_Alarm_Fault_Time.buf[0], 2, 64);
	}
}

/** ************************************************************************
 * @brief   Eeprom-page3默认参数
 **************************************************************************/
void Reset_Eeprom3(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Alarm_Fault_Value_r.buf[j] = 0;
	}

	Eeprom_Alarm_Fault_Value_r.member.Module_voltage_over_r[0] = 36 * 16; // 总电压充电过压，单位0.1V

	Eeprom_Alarm_Fault_Value_r.member.Module_voltage_over_r[1] = 35 * 16; // 总电压充电过压，单位0.1V

	Eeprom_Alarm_Fault_Value_r.member.Module_voltage_under_r[0] = 28.3 * 16;

	Eeprom_Alarm_Fault_Value_r.member.Module_voltage_under_r[1] = 31.3 * 16;

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_over_r[0] = 3600; // 充电过压，单位0.001V

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_over_r[1] = 3500;

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_under_r[0] = 2830;

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_under_r[1] = 3130;

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_differ_r[0] = 400;

	Eeprom_Alarm_Fault_Value_r.member.Cell_vol_differ_r[1] = 200;

	Eeprom_Alarm_Fault_Value_r.member.Charge_current_over_r[0] = 1000; // 单位0.1A，

	Eeprom_Alarm_Fault_Value_r.member.Charge_current_over_r[1] = 700;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r[0] = 1100; // 单位0.1A，

	Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r[1] = 1000;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r1[0] = 1100; // 单位0.1A，

	Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r1[1] = 1000;

	Eeprom_Alarm_Fault_Value_r.member.Charge_temp_over_r[0] = 400 + 500; // 充电温度过高，单位0.1°，偏移量40

	Eeprom_Alarm_Fault_Value_r.member.Charge_temp_over_r[1] = 400 + 450;

	Eeprom_Alarm_Fault_Value_r.member.Charge_temp_under_r[0] = 400 + 70;

	Eeprom_Alarm_Fault_Value_r.member.Charge_temp_under_r[1] = 400 + 150;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_over_r[0] = 400 + 450;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_over_r[1] = 400 + 450;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_under_r[0] = 400 - 250;

	Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_under_r[1] = 400 - 230;

	Eeprom_Alarm_Fault_Value_r.member.Temp_differ_r[0] = 80;

	Eeprom_Alarm_Fault_Value_r.member.Temp_differ_r[1] = 60;

	Eeprom_Alarm_Fault_Value_r.member.Mos_temp_over_r = 400 + 600;
}

/** ************************************************************************
 * @brief  Eeprom-page3,用于存储保护恢复阈值
 * @brief  使用page6的第一个字节作为校验 64*6=0x180
 * @brief  如果板子是第一次上电，将默认值写入Eeprom
 **************************************************************************/
void Eeprom3_Init(void)
{
	rt_uint8_t data[64];
	rt_uint16_t j = 0, crc = 0, temp16 = 0;

	M24256_Page_Read(&data[0], 3, 64); // 将PAGE0的数据读入缓存
	crc = crc16(0xffff, &data[0], 62);
    Reset_Eeprom3();
    data[62] = Eeprom_Alarm_Fault_Value_r.buf[62];
    data[63] = Eeprom_Alarm_Fault_Value_r.buf[63];
	temp16 = (data[62] << 8) + data[63];

	if (crc == temp16)
	{
		for (j = 0; j < 64; j++)
		{
			Eeprom_Alarm_Fault_Value_r.buf[j] = data[j];
		}
	}
	else
	{
		Reset_Eeprom3();
		crc = crc16(0xffff, &Eeprom_Alarm_Fault_Value_r.buf[0], 62);
		Eeprom_Alarm_Fault_Value_r.buf[62] = (crc >> 8) & 0xff;
		Eeprom_Alarm_Fault_Value_r.buf[63] = crc & 0xff;
		M24256_Page_Write(&Eeprom_Alarm_Fault_Value_r.buf[0], 3, 64);
	}
}

/** ************************************************************************
 * @brief   Eeprom-page4默认参数
 **************************************************************************/
void Reset_Eeprom4(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Alarm_Fault_Time_r.buf[j] = 0;
	}

	Eeprom_Alarm_Fault_Time_r.member.Module_voltage_over_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Module_voltage_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Module_voltage_under_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Module_voltage_under_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_over_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_under_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_under_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_differ_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Cell_vol_differ_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Charge_current_over_time_r[0] = 120;
	Eeprom_Alarm_Fault_Time_r.member.Charge_current_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r[0] = 120;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r1[0] = 120;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r1[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Charge_temp_over_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Charge_temp_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Charge_temp_under_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Charge_temp_under_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_over_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_over_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_under_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_under_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Temp_differ_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Temp_differ_time_r[1] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Mos_temp_over_time_r[0] = 10;
	Eeprom_Alarm_Fault_Time_r.member.Mos_temp_over_time_r[1] = 10;
}

/** ************************************************************************
 * @brief  Eeprom-page4,用于存储保护恢复时间
 * @brief  使用page6的第一个字节作为校验 64*6=0x180
 * @brief  如果板子是第一次上电，将默认值写入Eeprom
 **************************************************************************/
void Eeprom4_Init(void)
{
	rt_uint8_t data[64];
	rt_uint16_t j = 0, crc = 0, temp16 = 0;

	M24256_Page_Read(&data[0], 4, 64); // 将PAGE0的数据读入缓存
	crc = crc16(0xffff, &data[0], 62);
	temp16 = (data[62] << 8) + data[63];

	if (crc == temp16)
	{
		for (j = 0; j < 64; j++)
		{
			Eeprom_Alarm_Fault_Time_r.buf[j] = data[j];
		}
	}
	else
	{
		Reset_Eeprom4();
		crc = crc16(0xffff, &Eeprom_Alarm_Fault_Time_r.buf[0], 62);
		Eeprom_Alarm_Fault_Time_r.buf[62] = (crc >> 8) & 0xff;
		Eeprom_Alarm_Fault_Time_r.buf[63] = crc & 0xff;
		M24256_Page_Write(&Eeprom_Alarm_Fault_Time_r.buf[0], 4, 64);
	}
}

/** ************************************************************************
 * @brief   Eeprom-Page5备份AFE数据初始化
 **************************************************************************/
void Reset_Eeprom5(void)
{
	rt_uint8_t j;

	for (j = 0; j < 64; j++)
	{
		Eeprom_Afe.buf[j] = 0;
	}

	// SCONF1系统配置    00H
	Eeprom_Afe.member.CellNum = 0x0;   // 配置＜5代表16串
	Eeprom_Afe.member.Balance = 0x1;   // 1:平衡由外部MCU控制，平衡时序仍由SH367309内部逻辑控制 0:由SH367309内部逻辑控制
	Eeprom_Afe.member.OverCurPM = 0x0; // 充电过流只关闭充电MOSFET，放电过流只关闭放电MOSFET
	Eeprom_Afe.member.ENMOS = 0x1;	   // 检测到放电过流1或放电状态，则开启充电MOS
	Eeprom_Afe.member.ENPCH = 0x0;	   // 禁用预充电功能
	//
	// SCONF2系统配置  01H
	Eeprom_Afe.member.EUVR = 0x00;	 // 过放电保护状态释放与负载无关
	Eeprom_Afe.member.OCRA = 0x01;	 // 允许电流保护定时恢复，恢复时间32秒
	Eeprom_Afe.member.CTLC = 0x00;	 // 充放电和预充电由内部逻辑控制  CTL引脚无效
	Eeprom_Afe.member.DIS_PF = 0x01; // 禁止二次过充电保护
	Eeprom_Afe.member.UV_OP = 0x00;	 // 过放电只关闭放电MOS
	Eeprom_Afe.member.E0VB = 0x00;	 // 关闭“禁止低压电芯充电功能”
	//
	// 02H一次过充电保护延时40S
	Eeprom_Afe.member.OVT = 0x0F; // 过充电保护延时40S
	// 一次过充电保护电压
	Eeprom_Afe.member.OV8_9 = 0x02; // 寄存器值*5mV
	//
	// 03H 一次过充电保护电压
	Eeprom_Afe.member.OV0_7 = 0xF8; // 过充保护电压3.8V   单位 X5mV

	// 一次过充电保护恢复电压
	Eeprom_Afe.member.OVR8_9 = 0x02;

	// 04H欠压保护延时
	Eeprom_Afe.member.UVT = 0x0F; // 过放电保护延时40S

	// 05H一次过充电保护恢复电压
	Eeprom_Afe.member.OVR0_7 = 0xD0; // 过充电保护恢复电压3600mV  单位 X5mV

	// 06H过放电保护电压
	Eeprom_Afe.member.UV = 0x5A; //*20mV  1800mV

	// 07H过放电恢复电压
	Eeprom_Afe.member.UVR = 0x6E; //*20mV  2200mV

	// 08H平衡开启电压---不启用
	Eeprom_Afe.member.BALV = 0xAA; //*20mV  3400mV

	// 09H预充开启电压---不启用     					//*20mV  2500mV
	Eeprom_Afe.member.PREV = 0x7D;

	// 0AH低电压禁止充电电压---不启用
	Eeprom_Afe.member.L0V = 0x64; //*20mV  2000mV

	// 0BH二次过充电保护电压---不启用
	Eeprom_Afe.member.PFV = 0xB9; // 过充保护电压3.7V   X20mV

	// 0CH放电过流1电流设置
	Eeprom_Afe.member.OCD1V = 0x0F; // 200mV  /0.2mΩ = 1000A

	// 放电过流1延时40S
	Eeprom_Afe.member.OCD1T = 0x0F; // 40S

	// 0DH放电过流2设置   1250A
	Eeprom_Afe.member.OCD2V = 0x0F; // 2500A

	// 放电过流2延时  20S
	Eeprom_Afe.member.OCD2T = 0x0F;

	// 0EH短路保护设置寄存器
	Eeprom_Afe.member.SCV = 0x0F; // 1000mV-5000A

	// 短路保护延时
	Eeprom_Afe.member.SCT = 0x0F; // 960us

	// 0FH充电过流1设置寄存器   500A
	Eeprom_Afe.member.OCCV = 0x0F; //   200mV/0.2mΩ=1000A

	// 充电过流1延时1S
	Eeprom_Afe.member.OCCT = 0x0F; // 20s

	// 10H  CHS/MOST/OCRT/PFT
	// 二次过充电保护延时---不启用
	Eeprom_Afe.member.PFT = 0x00; // 8S

	// 充放电过流自恢复延时设置  32S
	Eeprom_Afe.member.OCRT = 0x02;

	// 充放电MOS开启延时设置 512us
	Eeprom_Afe.member.MOST = 0x03;

	// 充放电状态检测电压设置  1A   200uV
	Eeprom_Afe.member.CHS = 0x00; // 200uV  / 0.2mΩ   1A

	////11H充电高温保护110℃---不启用
	Eeprom_Afe.member.OTC = 0x21;

	////12H充电高温保护释放100℃---不启用
	Eeprom_Afe.member.OTCR = 0x2C;

	////13H充电低温保护0℃---不启用
	Eeprom_Afe.member.UTC = 0xA5;

	////14H充电低温保护释放5℃---不启用
	Eeprom_Afe.member.UTCR = 0x91;

	////15H放电高温保护110℃---不启用
	Eeprom_Afe.member.OTD = 0x21;

	////16H放电高温保护释放100℃---不启用
	Eeprom_Afe.member.OTDR = 0x2C;

	////17H放电低温保护-30℃---不启用
	Eeprom_Afe.member.UTD = 0xEC;

	////18H放电低温保护释放-25℃---不启用
	Eeprom_Afe.member.UTDR = 0xE6;

	////19H温度内部参考电阻系数---不启用
	Eeprom_Afe.member.TR = 0x04;
}

/** ************************************************************************
 * @brief  Eeprom-page5,用于存储AFE的数据
 **************************************************************************/
void Eeprom5_Init(void)
{
	rt_uint8_t data[64];
	rt_uint16_t j = 0, crc = 0, temp16 = 0;

	M24256_Page_Read(&data[0], 5, 64); // 将PAGE5的数据读入缓存
	crc = crc16(0xffff, &data[0], 62);
	temp16 = (data[62] << 8) + data[63];

	if (crc == temp16)
	{
		for (j = 0; j < 64; j++)
		{
			Eeprom_Afe.buf[j] = data[j];
		}
	}
	else
	{
		Reset_Eeprom5();
		crc = crc16(0xffff, &Eeprom_Alarm_Fault_Time_r.buf[0], 62);
		Eeprom_Alarm_Fault_Time_r.buf[62] = (crc >> 8) & 0xff;
		Eeprom_Alarm_Fault_Time_r.buf[63] = crc & 0xff;
		M24256_Page_Write(&Eeprom_Afe.buf[0], 5, 64);
	}
}

/** ************************************************************************
 * @brief   Eeprom备份数据初始化
 **************************************************************************/
void Eeprom_Init(void)
{
	// Eeprom-page0
	Eeprom0_Init();

	// Eeprom-page1
	Eeprom1_Init();

	// Eeprom-page2
	Eeprom2_Init();

	// Eeprom-page3
	Eeprom3_Init();

	// Eeprom-page4
	Eeprom4_Init();

	// Eeprom-page5
	Eeprom5_Init();
	
	//严禁page读之后进行byte读，会导致SCL总线持续拉低，无法恢复，原因未知
//	rt_thread_mdelay(50);
//	//读取AFE的Eeprom擦写次数
//	AFE_EEPROM_Writes = M24256_Byte_Read(0x182);
	
	//板子第一次上电，擦除AFE存储区擦写计数
	if(read_data_eep != 0xAA)
	{
			M24256_Byte_Write(0,0x182);
	}
}

/** ************************************************************************
 * @brief   Eeprom接收指令保存函数
 **************************************************************************/
void Eeprom_Save(void)
{	
		//满充标志翻转时存储一下,防止满充后重启再次充电导致过压故障
		if(Sys_info.member.sys_charge_full_flag != Sys_charge_full_flag)
		{
				Sys_charge_full_flag = Sys_info.member.sys_charge_full_flag;
			  Eeprom_Data0.member.Charge_Full_Enable = Sys_info.member.sys_charge_full_flag;
			
				Eeprom_Data0_Save_Flag = 1;
		}
		
		if(Eeprom_Data0_Save_Flag == 1)
		{
				M24256_Page_Write(&Eeprom_Data0.buf[0], 0, 64);
			
				Eeprom_Data0_Save_Flag = 0;
		}
		else
		{
				//强制PCS对电池满充判断逻辑：满1周强充一次/////
				Eeprom_Data0.member.Time_Accumulate++;
				if(Eeprom_Data0.member.Time_Accumulate > 604800)
				{
						 Eeprom_Data0.member.Time_Accumulate = 0;					 
						 Eeprom_Data0.member.Request_Full_Charge = 1;
				}	
		}
		
		//SOC > 97%,清零计数标志,协能协议要求，连续30天SOC未达到97%，开启强制满充,此处考虑掉电时间快，改为95%
		if(Sys_info.member.module_soc > 238)
		{
				Eeprom_Data0.member.Time_Accumulate = 0;
				Eeprom_Data0.member.Request_Full_Charge = 0;
		}
				
}
