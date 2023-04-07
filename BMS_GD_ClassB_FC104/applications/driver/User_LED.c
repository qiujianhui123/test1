
#include "GlobalVarShared.h"

void SOC_Warning_LED_Display(void);

//LED����������ʱ��ָʾ
void SOC_Warning_LED_Display(void)
{
	//�ŵ�ģʽ��LED5��ɫ��LED1-4��ʾSOC
	if(Sys_info.member.Work_State == DC_DISCHARGE)
	{
		if(Sys_info.member.module_soc <= 63)//25%
		{
				ws2812_rgb(1, WS_DARK); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_DARK);
				ws2812_rgb(4, WS_YELLOW);	
		}
		else if(Sys_info.member.module_soc <= 125)//50%
	  {
				ws2812_rgb(1, WS_DARK); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_YELLOW);
				ws2812_rgb(4, WS_YELLOW);	
	  }
	  else if(Sys_info.member.module_soc <= 187)//75%
	  {
				ws2812_rgb(1, WS_DARK); 
				ws2812_rgb(2, WS_YELLOW);
				ws2812_rgb(3, WS_YELLOW);
				ws2812_rgb(4, WS_YELLOW);	
	  }
	  else if(Sys_info.member.module_soc <= 250)//100%
	  {
				ws2812_rgb(1, WS_YELLOW); 
				ws2812_rgb(2, WS_YELLOW);
				ws2812_rgb(3, WS_YELLOW);
				ws2812_rgb(4, WS_YELLOW);		
	  }
		else
		{}
			
	 	ws2812_rgb(5, WS_BLUE);	 
	}
	else if(Sys_info.member.Work_State == DC_CHARGE)		//���ģʽ�����е�LED��ɫ�����һ��LED��˸
	{
		static rt_uint8_t count=0;
		
		if(count == 0)
		{
				if(Sys_info.member.module_soc <= 63)//25%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_DARK);
						ws2812_rgb(4, WS_YELLOW);	
				}
				else if(Sys_info.member.module_soc <= 125)//50%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);		
				}
				else if(Sys_info.member.module_soc <= 187)//75%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
				}
				else if(Sys_info.member.module_soc <= 250)//100%
				{
						ws2812_rgb(1, WS_YELLOW); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);		
				}
				else
				{}		
					
				count = 1;
		}
		else if(count == 1)
		{
				if(Sys_info.member.module_soc <= 63)//25%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_DARK);
						ws2812_rgb(4, WS_DARK);	
				}
				else if(Sys_info.member.module_soc <= 125)//50%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_DARK);
						ws2812_rgb(4, WS_YELLOW);		
				}
				else if(Sys_info.member.module_soc <= 187)//75%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);		
				}
				else if(Sys_info.member.module_soc <= 250)//100%
				{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
				}
				else
				{}		
					
				count = 0;
		}
		else
		{}

				//������LED����
				if(Sys_info.member.sys_charge_full_flag == 1)
				{
						ws2812_rgb(1, WS_YELLOW); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
				}
				
				ws2812_rgb(5, WS_YELLOW);	
	}
	else if(Sys_info.member.Work_State == DC_CHARGE_HEAT)		//����ģʽ��LED5��ɫ��SOC��ɫ����
	{
		if(Sys_info.member.module_soc <= 63)//25%
		{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_DARK);
						ws2812_rgb(4, WS_YELLOW);	
		}
		else if(Sys_info.member.module_soc <= 125)//50%
	  {
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
	  }
	  else if(Sys_info.member.module_soc <= 187)//75%
	  {
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);		
	  }
	  else if(Sys_info.member.module_soc <= 250)//100%
	  {
						ws2812_rgb(1, WS_YELLOW); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
	  }
		else
		{}
			
	 	ws2812_rgb(5, WS_ORANGE);	
	}
	else			//����״̬��LED5��˸
	{
		static rt_uint8_t Ready_count=0;
		
		if(Sys_info.member.module_soc <= 63)//25%
		{
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_DARK);
						ws2812_rgb(4, WS_YELLOW);	
		}
		else if(Sys_info.member.module_soc <= 125)//50%
	  {
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_DARK);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
	  }
	  else if(Sys_info.member.module_soc <= 187)//75%
	  {
						ws2812_rgb(1, WS_DARK); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
	  }
	  else if(Sys_info.member.module_soc <= 250)//100%
	  {
						ws2812_rgb(1, WS_YELLOW); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);		
	  }
		else
		{}
			
		if(Ready_count == 0)
		{
	 			ws2812_rgb(5, WS_WHITE);
				Ready_count = 1;			
		}
		else
		{
			 	ws2812_rgb(5, WS_DARK);
				Ready_count = 0;	
		}			
	}
	
	//����״̬LED����
	// 1)LED5��˸��SOC������ʾ���²��쳣�����±��������±�����MOS���´��󡢵��¼���ʧ��
	if((Sys_info.member.Temp_differ_warning_level == 2) || (Sys_info.member.Charge_temp_over_warning_level == 2) \
		|| (Sys_info.member.Discharge_temp_over_warning_level == 2) || (Sys_info.member.Charge_temp_under_warning_level == 2) \
		|| (Sys_info.member.Discharge_temp_under_warning_level == 2) || (Sys_info.member.Mos_temp_over_warning_level == 2))
	{
			static rt_uint8_t Fault_count=0;
		
			if(Fault_count == 0)
			{
	 			ws2812_rgb(5, WS_RED);
				Fault_count = 1;			
			}
			else
			{
			 	ws2812_rgb(5, WS_DARK);
				Fault_count = 0;	
			}
	}
	
	// 2)LED1\3\5������ɫ:���������ŵ�����������ѹ��ѹ����ѹ��ѹ
	if((Sys_info.member.Charge_current_over_warning[0] == 0) || (Sys_info.member.Discharge_current_over_warning[0] == 0) \
		|| (Sys_info.member.Cell_vol_over_warning[0] == 0) || (Sys_info.member.Module_voltage_over_warning[0] == 0))
	{
				ws2812_rgb(1, WS_RED); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_RED);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);
	}
	
	// 3)LED1\5�����������ѹ���͡���ѹ����
	if((Sys_info.member.Cell_vol_under_warning[0] == 0) || (Sys_info.member.Module_voltage_under_warning[0] == 0))
	{
				ws2812_rgb(1, WS_RED); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_DARK);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);		
	}
	
	// 4)LED5���������ӻ�ͨ��ʧ�ܡ���PCSͨ��ʧ��
	if((Sys_info.member.Pcs_commu_fail_warning_level == 2) || (Sys_info.member.Bcu_commu_fail_warning_level == 2))
	{
				ws2812_rgb(1, WS_DARK); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_DARK);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);	
	}
	
	//���ӻ�ͨ��ʧ�ܣ�ֻ�дӻ���ʾ��ƣ�����������ʾ
//	if((Sys_info.member.Bcu_commu_fail_warning_level == 2) && (Switch_Number > 1))
//	{
//				ws2812_rgb(1, WS_DARK); 
//				ws2812_rgb(2, WS_DARK);
//				ws2812_rgb(3, WS_DARK);
//				ws2812_rgb(4, WS_DARK);
//				ws2812_rgb(5, WS_RED);	
//	}
	
	// 5)ȫ������ɫ���¶ȴ�����ʧЧ����оѹ����󡢳�粻ֹͣ���ŵ粻ֹͣ�����Ȳ�ֹͣ������������
	if((Sys_info.member.Temperature_Error == 1) || (Sys_info.member.Cell_vol_differ_warning[0] == 0) || (Sys_info.member.Precharge_Fail == 1))
	{
				ws2812_rgb(1, WS_RED); 
				ws2812_rgb(2, WS_RED);
				ws2812_rgb(3, WS_RED);
				ws2812_rgb(4, WS_RED);
				ws2812_rgb(5, WS_RED);	
	}
	
	
	 ws2812_refresh(5);	

}




