/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     Mishew       the first version
 */

#include "config_io.h"

const rt_base_t output_pin_table[] = {
	LED_GREEN_PIN,
	LED_RED_PIN,
	RS485_EN1_PIN,
	RS485_EN2_PIN,
	AFE_VPRO_PIN,
	PC10_LIM1_PIN,
	PC11_LIM2_PIN,
	MOS_PRECHARGE_PIN,
	RELAY_PIN,
	MOS_HEAT_PIN,
	POWER_5VFUN_PIN,
	POWER_5VCOM_PIN,
	RGB_PIN,
	NTC_SEL1_PIN,
	NTC_SEL2_PIN,
	NTC_SEL3_PIN,
	POWER_RGB_PIN,
#ifdef WDI_IO
	WDI_PIN,
#endif
};

const rt_base_t input_pin_table[] = {
	SWITCH_NUMBER1_PIN,
	SWITCH_NUMBER2_PIN,
	SWITCH_NUMBER3_PIN,
	SWITCH_NUMBER4_PIN,
};

/*
 * GPIO输入输出功能配置
 */
void config_gpio(void)
{
	rt_uint8_t i;

	/* set xx pin mode to output */
	rt_uint8_t output_pin_num = sizeof(output_pin_table) / sizeof(rt_base_t);
	for (i = 0; i < output_pin_num; i++)
	{
		rt_pin_mode(output_pin_table[i], PIN_MODE_OUTPUT);
	}

	/* set xx pin mode to input */
	rt_uint8_t input_pin_num = sizeof(input_pin_table) / sizeof(rt_base_t);
	for (i = 0; i < input_pin_num; i++)
	{
		rt_pin_mode(input_pin_table[i], PIN_MODE_INPUT_PULLDOWN);
	}
	/* GPIO初始化 */

	// AFE eeprom控制关闭
	AFE_VPRO_OFF();

	PC10_LIM1_OFF();
	PC11_LIM2_OFF();

	// 预充MOS关闭
	MOS_PRECHARGE_OFF();

	// 继电器断开
	RELAY_OFF();

	// 加热MOS断开
	MOS_HEAT_OFF();

	// RGB电源供电
	POWER_RGB_ON();

	// 5VFUN供电
	POWER_5VFUN_ON();

	// 5V COM供电
	POWER_5VCOM_ON();

	// 喂狗
	WDI();

	// uart2收发使能：使能接收
	RS485_EN2_RX();
}
