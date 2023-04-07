
#include "bms_can.h"


CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 8;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {

  }

	/*中断设置，抢占优先级0，子优先级为0*/
	HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0 ,0);
	HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  }
}

static void CAN_Filter_Config(void)
{
		uint32_t StdId1 = 0x130;						//这里采用4个标准CAN ID作为例子
		uint32_t StdId2 = 0x131;
		uint32_t StdId3 = 0x132;
		uint32_t StdId4 = 0x13C;
	
		uint32_t StdId5 = 0x140;						//这里采用4个标准CAN ID作为例子
		uint32_t StdId6 = 0x141;
		uint32_t StdId7 = 0x142;
		uint32_t StdId8 = 0x14C;
	
	  uint32_t StdId9 = 0x305;
	  uint32_t StdId10 = 0x601;
	  uint32_t StdId11 = 0x401;
	  uint32_t StdId12 = 0x603;
	
		uint32_t StdId13 = 0x150;						//这里采用4个标准CAN ID作为例子
		uint32_t StdId14 = 0x151;
		uint32_t StdId15 = 0x152;
		uint32_t StdId16 = 0x15C;
	
		CAN_FilterTypeDef  CAN_FilterInitStructure;
	  CAN_FilterTypeDef  CAN_FilterInitStructure1;
	  CAN_FilterTypeDef  CAN_FilterInitStructure2;
		CAN_FilterTypeDef  CAN_FilterInitStructure3;
			
		CAN_FilterInitStructure.FilterBank = 0;
		CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDLIST;
		CAN_FilterInitStructure.FilterScale = CAN_FILTERSCALE_16BIT;
		CAN_FilterInitStructure.FilterIdHigh = StdId1 << 5;
	  CAN_FilterInitStructure.FilterIdLow = StdId2 << 5;
	
	  CAN_FilterInitStructure.FilterMaskIdHigh = StdId3 << 5;
		CAN_FilterInitStructure.FilterMaskIdLow = StdId4 << 5;
	
		CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;		//关联到FIFO0
		CAN_FilterInitStructure.FilterActivation = ENABLE;
	
	  HAL_CAN_ConfigFilter(&hcan,&CAN_FilterInitStructure);
	
		CAN_FilterInitStructure1 = CAN_FilterInitStructure;
		
		CAN_FilterInitStructure1.FilterBank = 1;
		CAN_FilterInitStructure1.FilterIdHigh = StdId9 << 5;
	  CAN_FilterInitStructure1.FilterIdLow = StdId10 << 5;	
	  CAN_FilterInitStructure1.FilterMaskIdHigh = StdId11 << 5;
		CAN_FilterInitStructure1.FilterMaskIdLow = StdId12 << 5;
		
		HAL_CAN_ConfigFilter(&hcan,&CAN_FilterInitStructure1);
		
		CAN_FilterInitStructure2 = CAN_FilterInitStructure1;
		
		CAN_FilterInitStructure2.FilterBank = 2;
		CAN_FilterInitStructure2.FilterIdHigh = StdId5 << 5;
	  CAN_FilterInitStructure2.FilterIdLow = StdId6 << 5;	
	  CAN_FilterInitStructure2.FilterMaskIdHigh = StdId7 << 5;
		CAN_FilterInitStructure2.FilterMaskIdLow = StdId8 << 5;
			
		HAL_CAN_ConfigFilter(&hcan,&CAN_FilterInitStructure2);
		
		CAN_FilterInitStructure3 = CAN_FilterInitStructure2;
		
		CAN_FilterInitStructure3.FilterBank = 3;
		CAN_FilterInitStructure3.FilterIdHigh = StdId13 << 5;
	  CAN_FilterInitStructure3.FilterIdLow = StdId14 << 5;	
	  CAN_FilterInitStructure3.FilterMaskIdHigh = StdId15 << 5;
		CAN_FilterInitStructure3.FilterMaskIdLow = StdId16 << 5;
			
		HAL_CAN_ConfigFilter(&hcan,&CAN_FilterInitStructure3);
}

void Can_ConfigInit(void)
{
		MX_CAN_Init();
		CAN_Filter_Config();
		HAL_CAN_Start(&hcan);
	
		HAL_CAN_ActivateNotification(&hcan,CAN_IT_RX_FIFO0_MSG_PENDING);//使能中断
}

CAN_TxHeaderTypeDef Can_HandleTxMsg;
uint32_t TxMailbox = 0;//Tx mailbox

void CAN_Send(void)
{
		uint8_t temp[8] = {1,2,3,4,5,6,7,8};
		
		Can_HandleTxMsg.StdId = 0x355;
		Can_HandleTxMsg.ExtId = 0x1314;	
		Can_HandleTxMsg.IDE = CAN_ID_STD;					///扩展模式
		Can_HandleTxMsg.RTR = CAN_RTR_DATA;				///发送数据帧
		Can_HandleTxMsg.DLC = 8;
		
		HAL_CAN_AddTxMessage(&hcan,&Can_HandleTxMsg,temp,&TxMailbox);
}

void CAN_Send_data(uint32_t id,uint32_t len,uint8_t *data)
{
	
		Can_HandleTxMsg.StdId = id;
		Can_HandleTxMsg.DLC = len;
	
		Can_HandleTxMsg.IDE = CAN_ID_STD;					///扩展模式
		Can_HandleTxMsg.RTR = CAN_RTR_DATA;				///发送数据帧
	
		HAL_CAN_AddTxMessage(&hcan,&Can_HandleTxMsg,data,&TxMailbox);	
}

