#include "GlobalVarShared.h"

void module_balance_cal(rt_uint8_t enable);

 /*
********************************************************************************************************* 
������㣺�����Ľڵ����Ҫ����
����ֻ���ڳ��״̬���ܿ��� byte enable
������ʧȥͨ�������رվ���
���е�ѹ����3400mV���ϣ�ѹ�����30mV�������⣬С��10mVʱ�رվ���
����¶�С�ڿ�������¶�
*********************************************************************************************************
*/
void module_balance_cal(rt_uint8_t enable)
{
	  rt_uint16_t 	temp16=0;
		rt_uint16_t 	i=0;
		rt_uint8_t 		j=0;
    rt_uint8_t 		balance[2];   
   
    //ȷ���Ƿ��ڳ��״̬     
		if(Sys_info.member.Current_Status == 2)
	  {	
			if(Sys_info.member.sys_charge_full_flag == 0)	
			{
					//���⹦������
			    if(enable>0)
          {	    
							if(Sys_info.member.Cell_Volt_Max>3400)     //��ߵ�ѹ��3400mV���ϣ�ѹ�����30mV�������⣬С��10mVʱ�رվ���
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
		
		 //�������
		 MTPWriteROM(MTP_BALANCEH, 2, &balance[0]);	  
		 		
}


