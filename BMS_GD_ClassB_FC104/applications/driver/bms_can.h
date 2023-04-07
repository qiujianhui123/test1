/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     NEW       the first version
 */
#ifndef APPLICATIONS_DRIVER_BMS_CAN_H_
#define APPLICATIONS_DRIVER_BMS_CAN_H_

#include <rtdevice.h>
#include "stm32f1xx.h"


extern struct rt_semaphore can1_rx_sem;
extern CAN_HandleTypeDef hcan;

void Can_ConfigInit(void);
void CAN_Send(void);
 
void CAN_Send_data(uint32_t id,uint32_t len,uint8_t *data);



#endif /* APPLICATIONS_DRIVER_BMS_CAN_H_ */
