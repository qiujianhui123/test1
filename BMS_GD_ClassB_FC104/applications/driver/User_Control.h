#ifndef _USER_CONTROL_H_
#define _USER_CONTROL_H_

#include <rtdevice.h>

#define     STAND_BY                0   //550ms待机
#define     PRECHARGE               1   //预充模式
#define     READY                   9   //预充完成模式
#define     DC_DISCHARGE            2   //放电模式
#define     DC_CHARGE               3   //充电模式
#define     DC_CHARGE_FULL          4   //充电模式
#define     DC_CHARGE_HEAT          5   //加热模式
#define     DC_CHARGE_READY         6   //加热完进入充电准备
#define     DC_CHARGE_HEAT_READY    7   //进入加热准备
#define     DC_CHARGE_HEAT_FINSH    8   //加热停止

#define     Current_Limit           50
#define     Voltage_Diff            300 //切入压差

void Control_Mode(void);


#endif
