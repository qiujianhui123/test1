/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-29     Mishew       the first version
 */
#ifndef _CONFIG_IO_H_
#define _CONFIG_IO_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

void config_gpio(void);

/******GPIO引脚定义*******/
//LED-GREEN
#define LED_GREEN_PIN       GET_PIN(C, 0)
//LED-RED
#define LED_RED_PIN         GET_PIN(C, 1)
//RS481-1
#define RS485_EN1_PIN       GET_PIN(A, 8)
//RS485-2
#define RS485_EN2_PIN       GET_PIN(B, 3)
//AFE_VPRO
#define AFE_VPRO_PIN        GET_PIN(C, 3)
//PC10_LIM1
#define PC10_LIM1_PIN       GET_PIN(C, 10)
//PC11_LIM2
#define PC11_LIM2_PIN       GET_PIN(C, 11)
//预充MOS
#define MOS_PRECHARGE_PIN   GET_PIN(A, 1)
//继电器
#define RELAY_PIN						GET_PIN(B, 9)
//加热MOS
#define MOS_HEAT_PIN  			GET_PIN(B, 5)
//MCU 5V供电
#define POWER_5VFUN_PIN     GET_PIN(B, 6)
//通信5V供电
#define POWER_5VCOM_PIN			GET_PIN(B, 7)
//RGB控制IO
#define RGB_PIN							GET_PIN(C, 12)
//NTC_SEL
#define NTC_SEL1_PIN				GET_PIN(C, 7)
#define NTC_SEL2_PIN				GET_PIN(C, 8)
#define NTC_SEL3_PIN				GET_PIN(C, 9)
//RGB电源
#define POWER_RGB_PIN				GET_PIN(A, 15)
//外部看门狗喂狗
#define WDI_PIN							GET_PIN(C, 2)
//4路拨码开关
#define SWITCH_NUMBER1_PIN  GET_PIN(B, 1)
#define SWITCH_NUMBER2_PIN  GET_PIN(C, 6)
#define SWITCH_NUMBER3_PIN  GET_PIN(B, 15)
#define SWITCH_NUMBER4_PIN  GET_PIN(B, 12)

/******GPIO功能宏定义*******/
#define LED_GREEN_ON()      rt_pin_write(LED_GREEN_PIN, PIN_HIGH)
#define LED_GREEN_OFF()     rt_pin_write(LED_GREEN_PIN, PIN_LOW)

#define LED_RED_ON()        rt_pin_write(LED_RED_PIN, PIN_HIGH)
#define LED_RED_OFF()       rt_pin_write(LED_RED_PIN, PIN_LOW)

/*UART1收发使能*/
#define RS485_EN1_RX()      rt_pin_write(RS485_EN1_PIN, PIN_LOW)
#define RS485_EN1_TX()      rt_pin_write(RS485_EN1_PIN, PIN_HIGH)

/*UART2收发使能*/
#define RS485_EN2_RX()      rt_pin_write(RS485_EN2_PIN, PIN_LOW)
#define RS485_EN2_TX()      rt_pin_write(RS485_EN2_PIN, PIN_HIGH)

//AFE的EEPROM控制
#define AFE_VPRO_ON() 			rt_pin_write(AFE_VPRO_PIN, PIN_HIGH)
#define AFE_VPRO_OFF() 			rt_pin_write(AFE_VPRO_PIN, PIN_LOW)

//PC10_LIM1控制
#define PC10_LIM1_ON() 			rt_pin_write(PC10_LIM1_PIN, PIN_HIGH)
#define PC10_LIM1_OFF() 		rt_pin_write(PC10_LIM1_PIN, PIN_LOW)

//PC11_LIM2控制
#define PC11_LIM2_ON() 			rt_pin_write(PC11_LIM2_PIN, PIN_HIGH)
#define PC11_LIM2_OFF() 		rt_pin_write(PC11_LIM2_PIN, PIN_LOW)

//预充MOS控制
#define MOS_PRECHARGE_ON()  rt_pin_write(MOS_PRECHARGE_PIN, PIN_HIGH)
#define MOS_PRECHARGE_OFF() rt_pin_write(MOS_PRECHARGE_PIN, PIN_LOW)

//继电器控制
#define RELAY_ON()					rt_pin_write(RELAY_PIN, PIN_HIGH)
#define RELAY_OFF()					rt_pin_write(RELAY_PIN, PIN_LOW)

//加热MOS控制
#define MOS_HEAT_ON()				rt_pin_write(MOS_HEAT_PIN, PIN_HIGH)
#define MOS_HEAT_OFF()			rt_pin_write(MOS_HEAT_PIN, PIN_LOW)

//MCU 5V控制
#define POWER_5VFUN_ON()		rt_pin_write(POWER_5VFUN_PIN, PIN_HIGH)
#define POWER_5VFUN_OFF()		rt_pin_write(POWER_5VFUN_PIN, PIN_LOW)

//通信 5V控制
#define POWER_5VCOM_ON()		rt_pin_write(POWER_5VCOM_PIN, PIN_HIGH)
#define POWER_5VCOM_OFF()		rt_pin_write(POWER_5VCOM_PIN, PIN_LOW)

//RGB供电
#define POWER_RGB_ON()			rt_pin_write(POWER_RGB_PIN, PIN_LOW)
#define POWER_RGB_OFF()			rt_pin_write(POWER_RGB_PIN, PIN_HIGH)

//WDI 
#define WDI()               HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);


#endif /* _CONFIG_IO_H_ */
