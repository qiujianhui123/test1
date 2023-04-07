/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-30     Mishew       the first version
 */

#include <rtthread.h>
#ifdef SELF_TEST
#include "stm32fxx_STLmain.h"
#endif

#include "bms_wakeup_sleep.h"
#include "GlobalVarShared.h"
#include "flash_api.h"
#include "MCP79410.h"

#define  Update_Addr       0x08032000      //每页2kB，使用第101页前4个字节作为升级判断标志

rt_uint32_t count = 0;
rt_uint8_t WKUP_KD_t = 0;
rt_uint8_t SLEEP_KD_t = 0;
rt_uint8_t ForceWork_t = 0;
rt_uint8_t UpdateWork_t = 0;
rt_uint16_t value[3];
rt_uint8_t RTC_Count = 0;

void task_cycle_entry(void *para)
{
	while (1)
	{
			count++;
			AD_To_Temp_OutsideTotalVoltage();

			//RTC
			if(RTC_Count == 1)
			{
					RTC_Count = 0;
					rtcc_get_data();
			}
			else if(RTC_Count == 2)
			{
					RTC_Count = 0;
					ini_time();
			}
			else
			{}
				
			//关机代码
			if(start_flag)
			{
					if(WKUP_KD)
					{
							WKUP_KD_t++;
							SLEEP_KD_t = 0;

							if(WKUP_KD_t >= 3)
							{
									WKUP_KD_t = 0;
									start_flag = 0;
									
									//转入standby，断开开关
									Sys_info.member.Work_State = STAND_BY;
								  Sys_info.member.Sleep_Flag = 1;
//									Sys_Enter_Standby();
							}
					}
					else
					{
							WKUP_KD_t = 0;						
							SLEEP_KD_t++;
						
							if(SLEEP_KD_t > 2)
							{
									SLEEP_KD_t = 0;
									start_flag = 0;
							}
					}
			}
			else
			{
					//故障强制启动部分
					if(WKUP_KD)
					{
							ForceWork_t++;

							if(ForceWork_t >= 5)
							{
									ForceWork_t = 0;
                  Force_Work_Model_Flag = RT_TRUE;									
							}
					}
					else
					{
							ForceWork_t = 0;						
					}						
			}
						
			//升级指令的处理，片内flash写入升级标志，触发复位
			if(Sys_info.member.Update_Flag == 1)
			{
					UpdateWork_t++;
					if(UpdateWork_t == 1)
					{
							CAN_BOOT_ErasePage(Update_Addr,1);					
					}
					else if(UpdateWork_t == 2)
					{
					    CAN_BOOT_ProgramWord(Update_Addr,0x00000001);
					}
					else if(UpdateWork_t == 3)
					{
							Sys_info.member.Update_Flag = 0;
							UpdateWork_t = 0;
						  __set_PRIMASK(1);
							HAL_NVIC_SystemReset();
					}
					else
					{}

			}

#ifdef SELF_TEST
		STL_DoRunTimeChecks();
#endif

		rt_thread_mdelay(2000);
	}
}

