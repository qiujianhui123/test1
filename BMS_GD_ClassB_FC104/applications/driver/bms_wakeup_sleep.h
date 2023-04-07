
#ifndef APPLICATIONS_DRIVER_BMS_WAKEUP_SLEEP_H_
#define APPLICATIONS_DRIVER_BMS_WAKEUP_SLEEP_H_


#include <rtdevice.h>
#include "stm32f1xx_hal.h"


#define WKUP_KD HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //PA0 检测是否外部WK_UP按键按下

rt_uint8_t Check_WKUP(void);  			//检测WKUP脚的信号
void Sys_Enter_Standby(void);	//系统进入待机模式
void WAKEUP_Init(void);

extern rt_uint8_t	start_flag;





#endif

