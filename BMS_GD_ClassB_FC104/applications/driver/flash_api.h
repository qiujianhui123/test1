#ifndef APPLICATIONS_DRIVER_FLASH_API_H_
#define APPLICATIONS_DRIVER_FLASH_API_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "stm32f1xx_hal.h"

rt_uint8_t CAN_BOOT_ErasePage(rt_uint32_t StartAddr,rt_uint32_t NbPage);
rt_uint8_t CAN_BOOT_ProgramWord(rt_uint32_t Address, rt_uint32_t Data);



#endif

