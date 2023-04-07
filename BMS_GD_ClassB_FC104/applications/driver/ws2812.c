
#include <rtthread.h>
#include <rtdevice.h>
#include "ws2812.h"
#include "config_io.h"


#define WAIT300NS()						__NOP();__NOP();__NOP();__NOP();__NOP();
#define WAIT850NS()  					__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
														  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
														  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
														  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
														  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP(); 
	
#define WS_ARRAY_SIZE 27
rt_uint8_t ws_data[WS_ARRAY_SIZE]={0};

/** ************************************************************************
 * @brief   ʹ��RTT/HAL��IO������ʱ̫�������üĴ�����ʽ
 * @brief   ��Ҫʵ��ߵ͵�ƽʱ�������ܳ����ֲ�涨����ֵ
 **************************************************************************/
/************************** 0��**************************/
void TX0(void) 
{
	GPIOC->BSRR = GPIO_PIN_12;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//	WAIT300NS();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	GPIOC->BSRR = GPIO_PIN_12 << 16u;
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();   
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP(); 
}

/************************** 1��**************************/
void TX1(void) 
{
	GPIOC->BSRR = GPIO_PIN_12;
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();\
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
//	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();   
    __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	  __NOP();__NOP();__NOP();__NOP();__NOP();__NOP(); 
	GPIOC->BSRR = GPIO_PIN_12 << 16u;
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
}

/** ************************************************************************
 * @brief   us��ʱ����̫׼
 **************************************************************************/
void delay_us(rt_uint32_t nus)
{
 rt_uint32_t Delay = nus * 72/4;
 do
 {
  __NOP();
 }
 while (Delay --);
}

/************************** ��λ**************************/
void RGB_Reset(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
  delay_us(200);//ʵ��250us
}

/** ************************************************************************
 * @brief   ws_num�������
 * @brief   ws_r/g/b
 **************************************************************************/
void ws2812_rgb(rt_uint8_t ws_num,rt_uint8_t ws_r,rt_uint8_t ws_g,rt_uint8_t ws_b)	
{
	ws_data[(ws_num-1)*3]=ws_g;
	ws_data[(ws_num-1)*3+1]=ws_r;
	ws_data[(ws_num-1)*3+2]=ws_b;
}

/** ************************************************************************
 * @brief   ws_count��������
 **************************************************************************/
void ws2812_refresh(rt_uint8_t ws_count)
{
	rt_uint8_t ws_ri=0;
    
	for(;ws_ri<ws_count*3;ws_ri++)
	{
		if((ws_data[ws_ri]&0x80)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x40)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x20)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x10)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x08)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x04)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x02)==0)	TX0();	else TX1();
		if((ws_data[ws_ri]&0x01)==0)	TX0();	else TX1();
	}
    
  //��ʱһ��ʱ��
	RGB_Reset();
}




