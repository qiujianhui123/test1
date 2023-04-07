#ifndef _USER_CONTROL_H_
#define _USER_CONTROL_H_

#include <rtdevice.h>

#define     STAND_BY                0   //550ms����
#define     PRECHARGE               1   //Ԥ��ģʽ
#define     READY                   9   //Ԥ�����ģʽ
#define     DC_DISCHARGE            2   //�ŵ�ģʽ
#define     DC_CHARGE               3   //���ģʽ
#define     DC_CHARGE_FULL          4   //���ģʽ
#define     DC_CHARGE_HEAT          5   //����ģʽ
#define     DC_CHARGE_READY         6   //�����������׼��
#define     DC_CHARGE_HEAT_READY    7   //�������׼��
#define     DC_CHARGE_HEAT_FINSH    8   //����ֹͣ

#define     Current_Limit           50
#define     Voltage_Diff            300 //����ѹ��

void Control_Mode(void);


#endif
