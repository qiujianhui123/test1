#ifndef _USER_SOC_FUNC_H_
#define _USER_SOC_FUNC_H_

#include <rtdevice.h>
#include "GlobalVarShared.h"

#define  	NumCells 												16   
#define 	NUM_Parellel   									1    
#define 	AmpHour_1C                      105//*Eeprom_Data0.member.SOH*0.4/100
#define 	AMP_HOUR_RATING 		  					(rt_uint32_t)(NUM_Parellel * AmpHour_1C )
#define 	AMP_Seconds_Rating     					(rt_uint32_t)(AMP_HOUR_RATING  * 3600)
#define 	mA_Seconds_Rating     					(rt_uint32_t)(AMP_Seconds_Rating  * 1000)
#define 	mA_Seconds_Discharged_1bit 			(rt_uint32_t)(mA_Seconds_Rating/250)

void TotalAh_Dischargeable_Calculate(void);
void Soc_ChangeValue(rt_uint8_t newvalue);
void SOC_Init_Process(void);
void  Get_Capacity_Cx_Tx(void);
rt_uint8_t Soc_CountmAh_100ms(rt_uint32_t current, rt_uint8_t Status);
void Save_Soc_And_EEPROM0(void) ;
void CyclesCalcu_And_SOC_Save(void) ;
void  Get_SOH(void);

//SOC  10%强制校准
rt_uint8_t Soc_DisCharge_Empty(rt_uint16_t system_min_v);
rt_uint8_t Soc_GetCoulombSoc(void);
void Soc_UpdateSoc(void);
void Soc_ChargeComplete(void);
void soc_1000ms_process(void);//SOC的更新与保存


#endif



