#include <rtdevice.h>
#include "GlobalVarShared.h"
#include "config_io.h"

void Main_Positive_Relay_Con(rt_uint8_t en);
void Precharge_MosFET_Con(rt_uint8_t en);
void Heater_MosFET_Con(rt_uint8_t en);
void DisCharge_MosFET_Con(rt_uint8_t en);
void Charge_MosFET_Con(rt_uint8_t en);
void RS2251_select(rt_uint8_t sel);

//预充MOS控制
void Precharge_MosFET_Con(rt_uint8_t en)
{            
	if(en)
	{						
			MOS_PRECHARGE_ON();		
			Sys_info.member.St_MosFET_PreChrg = 1;             //预充MOS状态
	}
	else
	{
			MOS_PRECHARGE_OFF();  
			Sys_info.member.St_MosFET_PreChrg = 0;                           								
	}     
}

//放电继电器控制
void Main_Positive_Relay_Con(rt_uint8_t en)
{
	if(en)
	{
		RELAY_ON();
		Sys_info.member.St_RlyHVPos = 1;                       //总正继电器状态
	}
	else
	{
		RELAY_OFF();
		Sys_info.member.St_RlyHVPos = 0;                       
	}
}

//加热MOS控制 
void Heater_MosFET_Con(rt_uint8_t en)
{ 
	if(en)
	{                      
		MOS_HEAT_ON();                                                      
		Sys_info.member.St_Heater_MosFET = 1;                  //加热MOS状态
	}
	else
	{
		MOS_HEAT_OFF();     
		Sys_info.member.St_Heater_MosFET = 0;                                                  
	}                 
}

//开启或关闭放电MOS
void DisCharge_MosFET_Con(rt_uint8_t en)
{  
	if(en)
	{                      
		MTPConfVal |= 0x20;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				
		Sys_info.member.St_DisCharge_MosFET = 1;                        //放电MOS状态
	}
	else
	{
		MTPConfVal=MTPConfVal &0xDF;//0b11011111;  
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);		
		Sys_info.member.St_DisCharge_MosFET = 0;                                                  
	}                 
}

//开启或关闭充电MOS
void Charge_MosFET_Con(rt_uint8_t en)
{
   if(en)
	{                      
		MTPConfVal |=0x10; //0b00010000;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				                                                    
		Sys_info.member.St_Charge_MosFET = 1;                           //充电MOS状态
	}
	else
	{
		MTPConfVal &= 0xEF;//0b11101111;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				        
		Sys_info.member.St_Charge_MosFET = 0;                                                  
	}                 
}

//多路选择器通道选择函数 PC7-A(低)/PC8-B/PC9-C（高）
void RS2251_select(rt_uint8_t sel)
{	
		switch(sel)
		{		
				case 0:
						rt_pin_write(NTC_SEL1_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL2_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL3_PIN, PIN_HIGH);
				break;
				case 1:
						rt_pin_write(NTC_SEL1_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL2_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL3_PIN, PIN_HIGH);							
				break;
				case 2:
						rt_pin_write(NTC_SEL1_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL2_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL3_PIN, PIN_HIGH);
				break;
				case 3:
						rt_pin_write(NTC_SEL1_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL2_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL3_PIN, PIN_HIGH);
				break;				
				case 4:
						rt_pin_write(NTC_SEL1_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL2_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL3_PIN, PIN_LOW);								
				break;		
				case 5:
						rt_pin_write(NTC_SEL1_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL2_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL3_PIN, PIN_LOW);	
				break;
				case 6:
						rt_pin_write(NTC_SEL1_PIN, PIN_LOW);
						rt_pin_write(NTC_SEL2_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL3_PIN, PIN_LOW);	
				break;
				case 7:
						rt_pin_write(NTC_SEL1_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL2_PIN, PIN_HIGH);
						rt_pin_write(NTC_SEL3_PIN, PIN_LOW);
				break;	
				default:
				break;					
			}
}

void Switch_4_Check(void)
{
	 static rt_uint8_t Temp_Number = 0;

	 if(rt_pin_read(SWITCH_NUMBER1_PIN) == 1)
	 {
			Temp_Number = Temp_Number|0x08;	 
	 }

	 if(rt_pin_read(SWITCH_NUMBER2_PIN) == 1)
	 {
			Temp_Number = Temp_Number|0x04;	 
	 }
	 
	 if(rt_pin_read(SWITCH_NUMBER3_PIN) == 1)
	 {
			Temp_Number = Temp_Number|0x02;	 
	 }
	 
	 if(rt_pin_read(SWITCH_NUMBER4_PIN) == 1)
	 {
			Temp_Number = Temp_Number|0x01;	 
	 }
	 
	 Switch_Number = Temp_Number;		 
	 Temp_Number=0;
}
