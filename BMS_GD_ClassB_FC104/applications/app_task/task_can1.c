/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     NEW       the first version
 */
#include <rtthread.h>
#include "bms_can.h"


void task_can1_entry(void *para)
{

    while (1)
    {
        /* 阻塞等待接收信号量，等到信号量后对接收数据进行处理 */
        rt_sem_take(&can1_rx_sem, RT_WAITING_FOREVER);

        /* 数据处理 */

    }
}
