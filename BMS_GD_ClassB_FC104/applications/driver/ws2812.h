#ifndef APPLICATIONS_DRIVER_WS2812_H_
#define APPLICATIONS_DRIVER_WS2812_H_

#include <rtthread.h>
#include <rtdevice.h>

//‘§∂®“Â—’…´
#define WS_DARK 			0,0,0
#define WS_WHITE 			255,255,255
#define WS_RED 				227,0,0
#define WS_GREEN 			0,176,0
#define WS_BLUE 			0,174,239
#define WS_YELLOW 		255,255,35
#define WS_PURPLE   	191,55,232
#define WS_CYAN 			0,50,50
#define WS_BROWN    	165,42,42
#define WS_ORANGE   	255,128,0
#define WS_ANGILE   	0,153,153


void TX0(void);
void TX1(void);
void RGB_Reset(void);
void ws2812_rgb(rt_uint8_t ws_num,rt_uint8_t ws_r,rt_uint8_t ws_g,rt_uint8_t ws_b);
void ws2812_refresh(rt_uint8_t ws_count);

#endif


