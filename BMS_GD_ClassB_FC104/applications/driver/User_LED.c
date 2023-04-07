
#include "GlobalVarShared.h"

void SOC_Warning_LED_Display(void);

//LED灯正常运行时的指示
void SOC_Warning_LED_Display(void)
{
	//放电模式，LED5蓝色，LED1-4显示SOC
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
	else if(Sys_info.member.Work_State == DC_CHARGE)		//充电模式，所有的LED绿色，最后一颗LED闪烁
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

				//充满，LED常亮
				if(Sys_info.member.sys_charge_full_flag == 1)
				{
						ws2812_rgb(1, WS_YELLOW); 
						ws2812_rgb(2, WS_YELLOW);
						ws2812_rgb(3, WS_YELLOW);
						ws2812_rgb(4, WS_YELLOW);	
				}
				
				ws2812_rgb(5, WS_YELLOW);	
	}
	else if(Sys_info.member.Work_State == DC_CHARGE_HEAT)		//加热模式，LED5紫色，SOC绿色常亮
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
	else			//待机状态，LED5闪烁
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
	
	//故障状态LED处理
	// 1)LED5闪烁，SOC正常显示：温差异常、高温保护、低温保护、MOS过温错误、低温加热失败
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
	
	// 2)LED1\3\5常亮红色:充电过流、放电过流、单体电压过压、总压过压
	if((Sys_info.member.Charge_current_over_warning[0] == 0) || (Sys_info.member.Discharge_current_over_warning[0] == 0) \
		|| (Sys_info.member.Cell_vol_over_warning[0] == 0) || (Sys_info.member.Module_voltage_over_warning[0] == 0))
	{
				ws2812_rgb(1, WS_RED); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_RED);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);
	}
	
	// 3)LED1\5常亮：单体电压过低、总压过低
	if((Sys_info.member.Cell_vol_under_warning[0] == 0) || (Sys_info.member.Module_voltage_under_warning[0] == 0))
	{
				ws2812_rgb(1, WS_RED); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_DARK);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);		
	}
	
	// 4)LED5常亮：主从机通信失败、与PCS通信失败
	if((Sys_info.member.Pcs_commu_fail_warning_level == 2) || (Sys_info.member.Bcu_commu_fail_warning_level == 2))
	{
				ws2812_rgb(1, WS_DARK); 
				ws2812_rgb(2, WS_DARK);
				ws2812_rgb(3, WS_DARK);
				ws2812_rgb(4, WS_DARK);
				ws2812_rgb(5, WS_RED);	
	}
	
	//主从机通信失败，只有从机显示红灯，主机不做显示
//	if((Sys_info.member.Bcu_commu_fail_warning_level == 2) && (Switch_Number > 1))
//	{
//				ws2812_rgb(1, WS_DARK); 
//				ws2812_rgb(2, WS_DARK);
//				ws2812_rgb(3, WS_DARK);
//				ws2812_rgb(4, WS_DARK);
//				ws2812_rgb(5, WS_RED);	
//	}
	
	// 5)全部亮红色：温度传感器失效、电芯压差过大、充电不停止、放电不停止、加热不停止、多主机错误
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




