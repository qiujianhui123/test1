#include <rtthread.h>
#include "GlobalVarShared.h"
#include "bms_ad.h"

rt_uint32_t AD_count = 0;

/** ************************************************************************
 * @brief   50ms/100msTask
 **************************************************************************/
void task_readdata_entry(void *para)
{
		static rt_bool_t readdata_count = 0;
	
    while (1)
		{	
				AD_count++;
				readdata_count = 1 - readdata_count;
				//50ms����һ��ADת��
				HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_DMA_ConvertedValue, 30);
			
				//100ms����AFE���ݶ�ȡ
				if(readdata_count == 0)
				{			
//						HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);	
						//ι��
						WDI();
					
						//AFE���ݴ���
						Read_V_I_Complete_Flag = MTPRead(MTP_CUR, sizeof(AFE), (U8  *)&AFE);
					
						if(!Read_V_I_Complete_Flag)
						{ 
								AFE_Communication_Count = 0;
								Sys_info.member.Afe_commu_fail_warning_level = 0;	
								Cell_voltage_Current_Process();
						}	
					
						//ʵʱ���ϼ��
						Sys_Warning_Process();//��������
						
						//SOC	 �ѷŵ�������  ��λmAs	
						Soc_CountmAh_100ms(Sys_info.member.Cur1,Sys_info.member.Current_Status);
				}
			
				rt_thread_mdelay(50);
		}
}

