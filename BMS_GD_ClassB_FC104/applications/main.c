/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     RT-Thread    first version
 */

#include <rtthread.h>
#include "task.h"
#include "GlobalVarShared.h"
#include "uart_api.h"
#include "bms_can.h"
#include "drv_soft_i2c.h"


#include "bms_wakeup_sleep.h"
#ifdef SELF_TEST
#include "stm32fxx_STLmain.h"
#endif

static void application_init(void);
static void task_init(void);
static void ipc_init(void);

int main(void)
{
    /*运行时自检初始化*/
#ifdef SELF_TEST
    STL_InitRunTimeChecks();
#endif

    /*应用初始化*/
    application_init();

    /*创建线程通信量*/
    ipc_init();

    /*创建任务*/
    task_init();

    return RT_EOK;
}

/*应用程序配置初始化*/
static void application_init(void)
{
#ifdef WAKE_UP
    // 待机模式
    WAKEUP_Init();
#endif

    // GPIO配置
    config_gpio();

    // CAN1配置
		Can_ConfigInit();
	
		//I2C配置初始化
		rt_hw_i2c_init();
	
	  //EEPROM-I2C配置
	  I2C1_init();

		//AFE-I2C配置
		I2C2_init();

		//AFE-I2C配置
		I2C3_init();
		
		//Eeprom_Init
		Eeprom_Init();

		//AFE数据校验
		StartUp_UpdataAfeConfig();

		/*外部AFE芯片看门狗初始化，通信不正常1分钟会复位*/
	  EnableAFEWdtCadc();
		
		//ADC1+DMA配置
		ADC1_Init();
	 	
		//告警计数时间初始化
		Warning_time_init();
		
		//读取上次保存的SOC
		Soc_ChangeValue(Eeprom_Data0.member.SOC);
		
		//参数初始化
		Sys_info.member.Max_Charge_Voltage = 580;

		//用于调试时监控AFE-EEPROM擦写次数
		rt_thread_mdelay(50);
    AFE_EEPROM_Writes = M24256_Byte_Read(0x182);
		if(AFE_EEPROM_Writes > 200)
		{
				AFE_EEPROM_Writes = 0;
		}    
}

/*静态方式创建任务*/
static void task_init(void)
{
    rt_int16_t i;

    for (i = 0; i < MAX_APP_TASK_NUM; i++)
    {
        // 任务初始化
        rt_thread_init(&app_task[i],
                       app_task_table[i].name,
                       app_task_table[i].entry,
                       app_task_table[i].parameter,
                       app_task_table[i].stack_start,
                       app_task_table[i].stack_size,
                       app_task_table[i].priority,
                       app_task_table[i].tick);

        // 启动任务
        rt_thread_startup(&app_task[i]);
    }
}

/*创建线程通信量*/
static void ipc_init(void)
{
    /* 初始化信号量 */
    rt_sem_init(&uart_rx_sem[0], "rx_sem0", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&uart_rx_sem[1], "rx_sem1", 0, RT_IPC_FLAG_FIFO);
    rt_sem_init(&can1_rx_sem, "rx_sem3", 0, RT_IPC_FLAG_FIFO);
}
