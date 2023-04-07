#include <rtthread.h>
#include "config_io.h"
#include "User_Control.h"

//调试变量
rt_uint16_t Control_count;


//应用
rt_uint16_t Start_Time;


void task_control_entry(void *para)
{
	
    while (1)
    {
			//count，监控task执行
			Control_count++;

			//红色LED，监控10ms时基
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);		
//							rt_pin_write(LED_RED_PIN, PIN_HIGH);
			//延时10s启动，why？
			if(Start_Time < 1000)
			{
				Start_Time++;			
			}
			else
			{
				Start_Time = 1001;
				Control_Mode();
			}
//			rt_pin_write(LED_RED_PIN, PIN_LOW);

			rt_thread_mdelay(10);
    }
}



