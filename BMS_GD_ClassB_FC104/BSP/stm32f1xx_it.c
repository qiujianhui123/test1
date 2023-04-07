/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "rtthread.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "stm32fxx_STLlib.h"
#include "stm32fxx_STLparam.h"
#include "GlobalVarShared.h"
#include "bms_can.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
volatile static uint16_t tmpCC4_last; /* keep last TIM4/Chn3 captured value */

volatile rt_uint32_t  AD_Cell_Temp_Value[8];
volatile rt_uint32_t  AD_Cell_Temp_Value_ADC2[8];
volatile rt_uint32_t  AD_MOS_Temp_Value;
volatile rt_uint32_t  AD_Total_Voltage_Value;
rt_uint8_t   Complete_Flag = 0;
rt_uint8_t   Complete_Count = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern CAN_HandleTypeDef Can_Handle;
extern DMA_HandleTypeDef hdma_adc1;
extern void uart_rx_timeout(void);
void RAM_Test_Handler(void);
void FailProcess(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
	FailProcess();
}

/**
 * @brief This function handles Hard fault interrupt.
 */
// void HardFault_Handler(void)
//{
//       FailSafePOR();
// }

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
	FailProcess();
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
	FailProcess();
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
	FailProcess();
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
	FailProcess();
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
	/* USER CODE BEGIN DebugMonitor_IRQn 0 */

	/* USER CODE END DebugMonitor_IRQn 0 */
	/* USER CODE BEGIN DebugMonitor_IRQn 1 */

	/* USER CODE END DebugMonitor_IRQn 1 */
}

///**
//  * @brief This function handles Pendable request for system service.
//  */
// void PendSV_Handler(void)
//{
//  /* USER CODE BEGIN PendSV_IRQn 0 */
//
//  /* USER CODE END PendSV_IRQn 0 */
//  /* USER CODE BEGIN PendSV_IRQn 1 */
//
//  /* USER CODE END PendSV_IRQn 1 */
//}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	HAL_IncTick();

	rt_tick_increase();

#ifdef SELF_TEST
	RAM_Test_Handler();
	HAL_WWDG_Refresh(&WwdgHandle);
	HAL_IWDG_Refresh(&IwdgHandle);
#endif

	uart_rx_timeout();

	/* leave interrupt */
	rt_interrupt_leave();
}

#ifdef SELF_TEST
void RAM_Test_Handler(void)
{
	/* Verify TickCounter integrity */
	if ((TickCounter ^ TickCounterInv) == 0xFFFFFFFFuL)
	{
		TickCounter++;
		TickCounterInv = ~TickCounter;

		if (TickCounter >= SYSTICK_10ms_TB)
		{
			uint32_t RamTestResult;

#if defined STL_EVAL_MODE
			/* Toggle LED2 for debug purposes */
			BSP_LED_Toggle(LED2);
#endif /* STL_EVAL_MODE */

			/* Reset timebase counter */
			TickCounter = 0u;
			TickCounterInv = 0xFFFFFFFFuL;

			/* Set Flag read in main loop */
			TimeBaseFlag = 0xAAAAAAAAuL;
			TimeBaseFlagInv = 0x55555555uL;

			ISRCtrlFlowCnt += RAM_MARCHC_ISR_CALLER;
			__disable_irq();
			RamTestResult = STL_TranspMarch();
			__enable_irq();
			ISRCtrlFlowCntInv -= RAM_MARCHC_ISR_CALLER;

			switch (RamTestResult)
			{
			case TEST_RUNNING:
				break;
			case TEST_OK:
#ifdef STL_VERBOSE
				/* avoid any long string output here in the interrupt, '#' marks ram test completed ok */
#ifndef __GNUC__
				putchar((int16_t)'#');
#else
				__io_putchar((int16_t)'#');
#endif /* __GNUC__ */
#endif /* STL_VERBOSE */
#ifdef STL_EVAL_MODE
				/* Toggle LED2 for debug purposes */
				BSP_LED_Toggle(LED2);
#endif /* STL_EVAL_MODE */
#if defined(STL_EVAL_LCD)
				++MyRAMCounter;
#endif /* STL_EVAL_LCD */
				break;
			case TEST_FAILURE:
			case CLASS_B_DATA_FAIL:
			default:
#ifdef STL_VERBOSE
				printf("\n\r >>>>>>>>>>>>>>>>>>>  RAM Error (March C- Run-time check)\n\r");
#endif /* STL_VERBOSE */
				FailSafePOR();
				break;
			} /* End of the switch */

			/* Do we reached the end of RAM test? */
			/* Verify 1st ISRCtrlFlowCnt integrity */
			if ((ISRCtrlFlowCnt ^ ISRCtrlFlowCntInv) == 0xFFFFFFFFuL)
			{
				if (RamTestResult == TEST_OK)
				{
					/* ==============================================================================*/
					/* MISRA violation of rule 17.4 - pointer arithmetic is used to check RAM test control flow */
#ifdef __IAR_SYSTEMS_ICC__ /* IAR Compiler */
#pragma diag_suppress = Pm088
#endif /* __IAR_SYSTEMS_ICC__ */
					if (ISRCtrlFlowCnt != RAM_TEST_COMPLETED)
#ifdef __IAR_SYSTEMS_ICC__ /* IAR Compiler */
#pragma diag_default = Pm088
#endif				/* __IAR_SYSTEMS_ICC__ */
					/* ==============================================================================*/
					{
#ifdef STL_VERBOSE
						printf("\n\r Control Flow error (RAM test) \n\r");
#endif /* STL_VERBOSE */
						FailSafePOR();
					}
					else /* Full RAM was scanned */
					{
						ISRCtrlFlowCnt = 0u;
						ISRCtrlFlowCntInv = 0xFFFFFFFFuL;
					}
				} /* End of RAM completed if*/
			}	  /* End of control flow monitoring */
			else
			{
#ifdef STL_VERBOSE
				printf("\n\r Control Flow error in ISR \n\r");
#endif /* STL_VERBOSE */
				FailSafePOR();
			}
#if defined STL_EVAL_MODE
			/* Toggle LED2 for debug purposes */
			BSP_LED_Toggle(LED2);
#endif	  /* STL_EVAL_MODE */
		} /* End of the 10 ms timebase interrupt */
	}
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  * @brief 此处屏蔽双ADC模式，仅读取ADC1的数据
  */
void DMA1_Channel1_IRQHandler(void)
{
	 HAL_DMA_IRQHandler(&hdma_adc1);
	
	 static rt_uint8_t channel_set = 0;

	 rt_uint8_t   i=0;
   rt_uint32_t  AD_Cell_Temp_Value_temp[8] = {0};
   rt_uint32_t  AD_MOS_Temp_Value_temp = 0;
   rt_uint32_t  AD_Total_Voltage_Value_temp = 0;

	 //此处需要后期处理
	 //DMA传输一半数据置位
   if(hdma_adc1.XferCpltCallback != RESET)
	 {
	 			Complete_Flag++;
				if(Complete_Flag == 2)
				{
						Complete_Flag = 0;
						//DMA传输完成所有数据置位
					  for(i=0;i<10;i++)
					  {
							 AD_MOS_Temp_Value_temp += (ADC_DMA_ConvertedValue[i][0]&0xFFFF);
							 AD_Cell_Temp_Value_temp[channel_set] += (ADC_DMA_ConvertedValue[i][1]&0xFFFF);
							 AD_Total_Voltage_Value_temp += (ADC_DMA_ConvertedValue[i][2]&0xFFFF);
					  }
						
						AD_MOS_Temp_Value = AD_MOS_Temp_Value_temp / 10;
					  AD_Cell_Temp_Value[channel_set] = AD_Cell_Temp_Value_temp[channel_set] / 10;
					  AD_Total_Voltage_Value = AD_Total_Voltage_Value_temp / 10;
						
						//中间变量清零
					  AD_MOS_Temp_Value_temp=0;
					  AD_Cell_Temp_Value_temp[channel_set]=0;
					  AD_Total_Voltage_Value_temp= 0;
						
						Sys_info.member.AD_Channel = channel_set;
						if(Sys_info.member.AD_Channel == 7)
						{
								 Sys_info.member.Temp_Ad_ending = 1;
						}

					  if(channel_set<7)//8通道循环
					  {
								channel_set++;
					  }
					  else
						{
								channel_set = 0;	
						}
						
						RS2251_select(channel_set);
		 		 
				}
	 }
	 


}

/**
  * @brief 所有的CAN中断事件集中点，判断具体发生何种中断，进入相应回调.
  */
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hcan);
}

/**
  * @brief CAN-FIFO0接收完一帧数据进入一次.
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
		Can1_rece_do();	
}

/******************************************************************************/
/**
 * @brief  This function handles TIM5 global interrupt request.
 * @param  : None
 * @retval : None
 */
void TIM5_IRQHandler(void)
{
	uint16_t tmpCC4_last_cpy;

	if ((TIM5->SR & TIM_SR_CC4IF) != 0u)
	{
		TIM5->SR = 0; // Note 清除中断标志，否则一直中断，频率很高无法执行中断外的程序

		/* store previous captured value */
		tmpCC4_last_cpy = tmpCC4_last;
		/* get currently captured value */
		tmpCC4_last = (uint16_t)(TIM5->CCR4);
		/* The CC4IF flag is already cleared here be reading CCR4 register */

		/* overight results only in case the data is required */
		if (LSIPeriodFlag == 0u)
		{
			/* take correct measurement only */
			if ((TIM5->SR & TIM_SR_CC4OF) == 0u)
			{
				/* Compute period length */
				PeriodValue = ((uint32_t)(tmpCC4_last) - (uint32_t)(tmpCC4_last_cpy)) & 0xFFFFuL;
				PeriodValueInv = ~PeriodValue;

				/* Set Flag tested at main loop */
				LSIPeriodFlag = 0xAAAAAAAAuL;
				LSIPeriodFlagInv = 0x55555555uL;
			}
			else
			{
				/* ignore computation in case of IC overflow */
				TIM5->SR &= (uint16_t)(~TIM_SR_CC4OF);
			}
		}
		/* ignore computation in case data is not required */
	}
}

#endif

/******************************************************************************/
/**
 * @brief Configure TIM5 to measure LSI period
 * @param  : None
 * @retval : ErrorStatus = (ERROR, SUCCESS)
 */
ErrorStatus STL_InitClock_Xcross_Measurement(void)
{
#ifdef SELF_TEST
	ErrorStatus result = SUCCESS;
	TIM_HandleTypeDef tim_capture_handle;
	TIM_IC_InitTypeDef tim_input_config;

	/*## Enable peripherals and GPIO Clocks ####################################*/
	/* TIMx Peripheral clock enable */
	__HAL_RCC_TIM5_CLK_ENABLE();

	/*## Configure the NVIC for TIMx ###########################################*/
	HAL_NVIC_SetPriority(TIM5_IRQn, 4u, 0u);

	/* Enable the TIM5 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM5_IRQn);

	/* TIM5 configuration: Input Capture mode ---------------------
	The LSI oscillator is connected to TIM5 CH4.
	The Rising edge is used as active edge, ICC input divided by 8
	The TIM5 CCR4 is used to compute the frequency value.
	------------------------------------------------------------ */
	tim_capture_handle.Instance = TIM5;
	tim_capture_handle.Init.Prescaler = 0u;
	tim_capture_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim_capture_handle.Init.Period = 0xFFFFFFFFul;
	tim_capture_handle.Init.ClockDivision = 0u;
	tim_capture_handle.Init.RepetitionCounter = 0u;
	/* define internal HAL driver status here as handle structure is defined locally */
	__HAL_RESET_HANDLE_STATE(&tim_capture_handle);
	if (HAL_TIM_IC_Init(&tim_capture_handle) != HAL_OK)
	{
		/* Initialization Error */
		result = ERROR;
	}
	/* Connect internally the TIM5_CH4 Input Capture to the LSI clock output */
	/*
	  HAL_TIMEx_RemapConfig(&tim_capture_handle, TIM_TIM5_LSI);
	*/
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_TIM5CH4_ENABLE();
	/* Configure the TIM5 Input Capture of channel 4 */
	tim_input_config.ICPolarity = TIM_ICPOLARITY_RISING;
	tim_input_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim_input_config.ICPrescaler = TIM_ICPSC_DIV8;
	tim_input_config.ICFilter = 0u;
	if (HAL_TIM_IC_ConfigChannel(&tim_capture_handle, &tim_input_config, TIM_CHANNEL_4) != HAL_OK)
	{
		/* Initialization Error */
		result = ERROR;
	}

	/* Reset the flags */
	tim_capture_handle.Instance->SR = 0u;
	LSIPeriodFlag = 0u;

	/* Start the TIM Input Capture measurement in interrupt mode */
	if (HAL_TIM_IC_Start_IT(&tim_capture_handle, TIM_CHANNEL_4) != HAL_OK)
	{
		/* Initialization Error */
		result = ERROR;
	}
	return (result);
#else
	return ERROR;
#endif
}


void FailProcess(void)
{
#ifdef SELF_TEST
	FailSafePOR();
#else
	while (1)
	{
	}
#endif
}

/* USER CODE END 1 */
