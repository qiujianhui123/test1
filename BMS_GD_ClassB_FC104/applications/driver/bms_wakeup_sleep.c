
#include <rtthread.h>
#include "bms_wakeup_sleep.h"
#include "stm32_hal_legacy.h"

rt_uint8_t start_flag;

// 系统进入待机模式
void Sys_Enter_Standby(void)
{
//	__HAL_RCC_AHB1_FORCE_RESET();       					// 复位所有IO口

//	__HAL_RCC_PWR_CLK_ENABLE();				  					// 使能PWR时钟
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);  		// 禁止所有唤醒源
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);		  				// 清除Wake_UP标志
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); 			// 设置WKUP用于唤醒
	
	HAL_SuspendTick();
	HAL_PWR_EnterSTANDBYMode();				  						// 进入待机模式
}

// 唤醒按键初始化
void WAKEUP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure; 							// 定义初始化GPIO结构体变量
	__HAL_RCC_GPIOA_CLK_ENABLE();  							// 开始GPIOA时钟

	GPIO_Initure.Pin = GPIO_PIN_0;			   			// PA0引脚
	GPIO_Initure.Pull = GPIO_PULLDOWN;		   		// 上拉
	GPIO_Initure.Mode = GPIO_MODE_IT_RISING;   	// 下降沿触发
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; 	// 高速
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   		// GPIOE初始化

	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	// 检查是否是正常开机
	if (Check_WKUP() == 0)
	{
		Sys_Enter_Standby(); // 不是开机，进入待机模式
	}
}

// 检测WKUP脚的信号
// 返回值1:连续按下3s以上
//       0:错误的触发
rt_uint8_t Check_WKUP(void)
{
	volatile rt_uint8_t t = 0;
	rt_uint8_t tx = 0; // 记录松开的次数

	while (1)
	{
		if (WKUP_KD) // 已经按下了
		{
			t++;
			tx = 0;
		}
		else
		{
			t = 0;
			tx++;
			if (tx > 10) // 超过90ms内没有WKUP信号
			{
				return 0; // 错误的按键,按下次数不够
			}
		}
		rt_thread_mdelay(30);

		if (t >= 100) // 按下超过3秒钟
		{
			return 1; // 按下3s以上了
		}
	}
}

// 外部中断线0中断服务函数
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

// 中断线0中断处理过程
// 此函数会被HAL_GPIO_EXTI_IRQHandler()调用
// GPIO_Pin:引脚
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0) // PA0
	{
		start_flag = 1;
	}
}
