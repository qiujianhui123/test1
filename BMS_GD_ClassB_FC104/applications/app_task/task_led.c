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
#include "GlobalVarShared.h"
#include "bms_can.h"

uint8_t data_ram[6];

void task_led_entry(void *para)
{
    static rt_bool_t pin_state = PIN_LOW;
	
    while (1)
    {
        //电平翻转
        pin_state = 1 - pin_state;

        rt_pin_write(LED_GREEN_PIN, pin_state);
//        rt_pin_write(LED_RED_PIN, 1 - pin_state);

				//喂狗
				//rt_pin_write(WDI_PIN,pin_state);
			
				//数据接收上位机指令处理
			
				//充放电-电流计算
				Continous_DC_current_C_Check();
				Continous_CH_current_C_Check();
			
				//1s刷新任务
				if(pin_state == PIN_LOW)
				{
						//RGB灯控制
						SOC_Warning_LED_Display();
					
						//获取RTC时间
					
						//SOC更新与保存
						soc_1000ms_process();
					
						//Eeprom数据更新与保存
						Eeprom_Save();
					
						//获取拨码开关值
						Switch_4_Check();
					
						//读取AFE配置数据，意义不大
					
						//读取AFE-ram数据，不知道干嘛
//						MTPRead(MTP_CONF,sizeof(Afe_Ram), (uint8_t *)&Afe_Ram.buf[0]);
					  MTPRead(MTP_CONF,6, (uint8_t *)&data_ram[0]);
						//判断均衡,暂时关闭
						//module_balance_cal(1);
					
						//通信告警检测
						Commu_fail_warning_detect();
								
				}
		
        rt_thread_mdelay(500);
    }
}
