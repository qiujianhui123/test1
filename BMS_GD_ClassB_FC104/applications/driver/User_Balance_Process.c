#include "GlobalVarShared.h"

void module_balance_cal(rt_uint8_t enable);

 /*
********************************************************************************************************* 
均衡计算：测算哪节电池需要均衡
均衡只有在充电状态才能开启 byte enable
与主机失去通信立即关闭均衡
所有电压都在3400mV以上，压差大于30mV开启均衡，小于10mV时关闭均衡
最高温度小于开启最高温度
*********************************************************************************************************
*/
void module_balance_cal(rt_uint8_t enable)
{
	  rt_uint16_t 	temp16=0;
		rt_uint16_t 	i=0;
		rt_uint8_t 		j=0;
    rt_uint8_t 		balance[2];   
   
    //确认是否在充电状态     
		if(Sys_info.member.Current_Status == 2)
	  {	
			if(Sys_info.member.sys_charge_full_flag == 0)	
			{
					//均衡功能启用
			    if(enable>0)
          {	    
							if(Sys_info.member.Cell_Volt_Max>3400)     //最高电压在3400mV以上，压差大于30mV开启均衡，小于10mV时关闭均衡
							{
									for(j=0;j<16;j++)
									{
											i=1;
											i=(i<<j);
                    
											if(Sys_info.member.Cell_volt[j] > Sys_info.member.Cell_Volt_Min)
											{
													if((Sys_info.member.Cell_volt[j] - Sys_info.member.Cell_Volt_Min) > 50)
													{
															temp16|=i;
													}
													else if((Sys_info.member.Cell_volt[j] - Sys_info.member.Cell_Volt_Min) < 10)
													{
															temp16&=(~i);
													}
											}
											else
											{
													temp16&=(~i);  
											}
									}      
							}             
							else 
							{
									temp16=0;        
							}       
          }
          else
          {
							temp16=0;  
          }						
			}
			else
			{
          temp16=0;  
      }
			
			Sys_info.member.Balance_BIT=temp16; 	
		}  
	  else
	  {
			Sys_info.member.Balance_BIT=0;
	  }	
		
		 balance[0]=Sys_info.member.Balance_BIT>>8;
		 balance[1]=Sys_info.member.Balance_BIT;
		
		 //均衡控制
		 MTPWriteROM(MTP_BALANCEH, 2, &balance[0]);	  
		 		
}


