
#ifndef APPLICATIONS_DRIVER_BMS_AD_H_
#define APPLICATIONS_DRIVER_BMS_AD_H_

#include "stm32f1xx.h"


extern ADC_HandleTypeDef hadc1;

extern volatile uint16_t ADC_DMA_ConvertedValue[10][3];

void ADC1_Init(void);

#endif

