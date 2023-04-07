/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Mishew       the first version
 */

#include <rtthread.h>
#include "ws2812.h"
#include "config_io.h"
#include "GlobalVarShared.h"
#include "bms_can.h"

rt_uint16_t Test_count;

rt_uint8_t  data[8] = {1,5,3,6,4,5,2,1};

void task_uart2_entry(void *para)
{
//    rt_uint16_t i;

    while (1)
    {

				Test_count++;
			
				Pack_Send_data();
			

			
				rt_thread_mdelay(500);
			
			
    }
}
