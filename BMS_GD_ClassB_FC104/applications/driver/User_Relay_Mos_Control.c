#include <rtdevice.h>
#include "GlobalVarShared.h"
#include "config_io.h"

void Main_Positive_Relay_Con(rt_uint8_t en);
void Precharge_MosFET_Con(rt_uint8_t en);
void Heater_MosFET_Con(rt_uint8_t en);
void DisCharge_MosFET_Con(rt_uint8_t en);
void Charge_MosFET_Con(rt_uint8_t en);
void RS2251_select(rt_uint8_t sel);

//Ԥ��MOS����
void Precharge_MosFET_Con(rt_uint8_t en)
{            
	if(en)
	{						
			MOS_PRECHARGE_ON();		
			Sys_info.member.St_MosFET_PreChrg = 1;             //Ԥ��MOS״̬
	}
	else
	{
			MOS_PRECHARGE_OFF();  
			Sys_info.member.St_MosFET_PreChrg = 0;                           								
	}     
}

//�ŵ�̵�������
void Main_Positive_Relay_Con(rt_uint8_t en)
{
	if(en)
	{
		RELAY_ON();
		Sys_info.member.St_RlyHVPos = 1;                       //�����̵���״̬
	}
	else
	{
		RELAY_OFF();
		Sys_info.member.St_RlyHVPos = 0;                       
	}
}

//����MOS���� 
void Heater_MosFET_Con(rt_uint8_t en)
{ 
	if(en)
	{                      
		MOS_HEAT_ON();                                                      
		Sys_info.member.St_Heater_MosFET = 1;                  //����MOS״̬
	}
	else
	{
		MOS_HEAT_OFF();     
		Sys_info.member.St_Heater_MosFET = 0;                                                  
	}                 
}

//������رշŵ�MOS
void DisCharge_MosFET_Con(rt_uint8_t en)
{  
	if(en)
	{                      
		MTPConfVal |= 0x20;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				
		Sys_info.member.St_DisCharge_MosFET = 1;                        //�ŵ�MOS״̬
	}
	else
	{
		MTPConfVal=MTPConfVal &0xDF;//0b11011111;  
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);		
		Sys_info.member.St_DisCharge_MosFET = 0;                                                  
	}                 
}

//������رճ��MOS
void Charge_MosFET_Con(rt_uint8_t en)
{
   if(en)
	{                      
		MTPConfVal |=0x10; //0b00010000;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				                                                    
		Sys_info.member.St_Charge_MosFET = 1;                           //���MOS״̬
	}
	else
	{
		MTPConfVal &= 0xEF;//0b11101111;    
		BMS_Byte_Write(&MTPConfVal,MTP_CONF);				        
		Sys_info.member.St_Charge_MosFET = 0;                                                  
	}                 
}

//��·ѡ����ͨ��ѡ���� PC7-A(��)/PC8-B/PC9-C���ߣ�
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
