#include "bms_ad.h"

volatile uint16_t ADC_DMA_ConvertedValue[10][3];

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
}

void GPIO_Init(void)
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
}
void DMA_Init(void)
{
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/* ADC1 init function */
//此处需要考虑是否采用双ADC模式，这样DMA部分需要使用word模式，它上半个字包含ADC2的转换数据，低半个字包含ADC1的转换数据
void ADC1_Init(void)
{
	GPIO_Init();
	DMA_Init();

	ADC_ChannelConfTypeDef sConfig = {0};

	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;		  // 开启扫描模式，通道>1
	hadc1.Init.ContinuousConvMode = DISABLE;		  // 禁止连续模式，手动开启扫描
	hadc1.Init.DiscontinuousConvMode = DISABLE;		  // 禁止间断模式，不分组，每次扫描3个通道数据
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // 软件启动每次扫描
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;		  // 数据右对齐
	hadc1.Init.NbrOfConversion = 3;					  // 开启3个通道
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_5; // 配置通道转换序列
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // 采样时间，（239.5+12.5） / 9M = 28us
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_DMA_ConvertedValue, 30); // 开启ADC转换并向DMA传输数据，完成half、全部数据传输都会产生中断
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *adcHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if (adcHandle->Instance == ADC1)
	{
		/* ADC1 clock enable */
		__HAL_RCC_ADC1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**ADC1 GPIO Configuration
		PA5     ------> ADC1_IN5
		PA6     ------> ADC1_IN6
		PA7     ------> ADC1_IN7
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; // 配置ADC-IO
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* ADC1 DMA Init */
		/* ADC1 Init */
		hdma_adc1.Instance = DMA1_Channel1;							  // 使用DMA1
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;			  // 数据传输外设到内存
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;				  // 禁止外设地址增长
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;					  // 使能内存地址增长
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 16 bit
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR; // 开启DMA数据循环覆盖模式
		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
		if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
		{
			Error_Handler();
		}

		__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1); // 关联ADC1与DMA
	}
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *adcHandle)
{

	if (adcHandle->Instance == ADC1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_ADC1_CLK_DISABLE();

		/**ADC1 GPIO Configuration
		PA5     ------> ADC1_IN5
		PA6     ------> ADC1_IN6
		PA7     ------> ADC1_IN7
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

		/* ADC1 DMA DeInit */
		HAL_DMA_DeInit(adcHandle->DMA_Handle);
	}
}
