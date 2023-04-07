
#ifndef __GLOBALVARSHARED_H_
#define __GLOBALVARSHARED_H_

#include "User_Type_Define.h"
#include "m24256_i2c.h"
#include "bms_i2c.h"
#include "AFE_SH367309.h"
#include "User_Control.h"
#include "config_io.h"
#include "bms_ad.h"
#include "ws2812.h"
#include "User_Soc_Func.h"
#include "bms_wakeup_sleep.h"
#include "MCP79410.h"

//软件版本
#define     SOFT_VERSION      205

//故障等级
#define     FAULT_LEVEL       2
#define     WARNING_LEVEL     1
#define     NORMAL            0

//Current_Status
#define     DISCHARGE         1
#define     CHARGE            2

//全局变量
extern SYS_INFO 						Sys_info; 
extern rt_uint8_t           Eeprom_Data0_Save_Flag;

//Eeprom相关
extern void Eeprom_Init(void);
extern EEP_AFE 							Eeprom_Afe;
extern EEP_DATA0 	          Eeprom_Data0;
extern EEP_ALARM_FAULT1 	  Eeprom_Alarm_Fault_Value;
extern EEP_ALARM_FAULT2 		Eeprom_Alarm_Fault_Time;
extern EEP_ALARM_FAULT1_R   Eeprom_Alarm_Fault_Value_r;
extern EEP_ALARM_FAULT2_R   Eeprom_Alarm_Fault_Time_r;
extern rt_uint8_t AFE_EEPROM_Writes;
extern void Eeprom_Save(void);

//AFE相关
extern rt_uint8_t CRC8cal(rt_uint8_t *p, rt_uint8_t Length);
extern void Cell_voltage_Current_Process(void);
extern void StartUp_UpdataAfeConfig(void);
//void Reset_AFE_EEPROM(void);

extern rt_uint8_t Read_V_I_Complete_Flag;
extern AFEDATA    AFE;
extern AFE_RAM    Afe_Ram;

//AD相关
extern volatile rt_uint8_t system_vol_cal;
extern volatile rt_uint32_t  AD_Cell_Temp_Value[8];
extern volatile rt_uint32_t  AD_Cell_Temp_Value_ADC2[8];
extern volatile rt_uint32_t  AD_MOS_Temp_Value;
extern volatile rt_uint32_t  AD_Total_Voltage_Value;
extern volatile rt_uint8_t current_cal;
extern volatile rt_uint8_t max_vol_cal;
extern volatile rt_uint8_t min_vol_cal;
extern volatile rt_uint8_t min_temp_cal;
extern volatile rt_uint8_t max_temp_cal;
extern volatile rt_uint8_t mos_temp_cal;
extern void AD_To_Temp_OutsideTotalVoltage(void);

//Control相关
extern rt_uint8_t   PCS_Communication_Count;
extern rt_uint8_t   BCU_Communication_Count;
extern rt_uint8_t   AFE_Communication_Count;
extern rt_bool_t    Force_Work_Model_Flag;

//保护部分
extern void Warning_time_init(void);
extern void Sys_Warning_Process(void);

//LED
extern void SOC_Warning_LED_Display(void);

//IO控制
//放电继电器控制
extern void Main_Positive_Relay_Con(rt_uint8_t en);
//预充MOS控制
extern void Precharge_MosFET_Con(rt_uint8_t en);
//加热MOS控制 
extern void Heater_MosFET_Con(rt_uint8_t en);
//开启或关闭放电MOS
extern void DisCharge_MosFET_Con(rt_uint8_t en);
//开启或关闭充电MOS
extern void Charge_MosFET_Con(rt_uint8_t en);
//模拟开关控制
extern void RS2251_select(rt_uint8_t sel);
//拨码开关读取
extern void Switch_4_Check(void);

//并机
extern rt_uint8_t  Switch_Number;
extern void Commu_fail_warning_detect(void);
extern rt_uint8_t  SLAVE_Communication[];
extern rt_uint8_t  SLAVE_Online[];
extern rt_uint8_t  Online_Num;
extern rt_int16_t  Current_Total;
extern PARA_BUFFER Para_Buffer0;						//接收从机1数据
extern PARA_BUFFER Para_Buffer1;						//接收从机2数据
extern PARA_BUFFER Para_Buffer2;						//接收从机3数据
extern COMM_PACK   Comm_Pack;
extern rt_uint8_t  Master_On;

//CAN
extern void can_send_data(rt_uint32_t id,rt_uint8_t len,rt_uint8_t *Data);
extern void Pack_Send_data(void);
extern void Can1_rece_do(void);


//SOC，充放电倍率计算
extern void Continous_CH_current_C_Check(void);
extern void Continous_DC_current_C_Check(void);

//均衡
extern void module_balance_cal(rt_uint8_t enable);

//加热
extern rt_uint8_t   Heat_Num;

#endif


