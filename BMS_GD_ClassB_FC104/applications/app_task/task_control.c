#include <rtthread.h>
#include "config_io.h"
#include "User_Control.h"

//���Ա���
rt_uint16_t Control_count;


//Ӧ��
rt_uint16_t Start_Time;


void task_control_entry(void *para)
{
	
    while (1)
    {
			//count�����taskִ��
			Control_count++;

			//��ɫLED�����10msʱ��
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_1);		
//							rt_pin_write(LED_RED_PIN, PIN_HIGH);
			//��ʱ10s������why��
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



