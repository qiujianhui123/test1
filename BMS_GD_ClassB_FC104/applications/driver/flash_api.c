#include "flash_api.h"



/**
  * @brief  擦出用户程序储存区数据
  * @param  StartAddr 起始地址
	* @param  EndAddr 结束地址
  * @retval 数据擦出状态，0-擦出数据成功，1-擦出数据失败
  */
rt_uint8_t CAN_BOOT_ErasePage(rt_uint32_t StartAddr,rt_uint32_t NbPage)
{
		FLASH_EraseInitTypeDef My_Flash;  							//声明 FLASH_EraseInitTypeDef 结构体为 My_Flash
		HAL_FLASH_Unlock();               							//解锁Flash
		
		My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  		//标明Flash执行页面只做擦除操作
		My_Flash.PageAddress = StartAddr;  							//声明要擦除的地址
		My_Flash.NbPages = NbPage;                      //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值
		
		uint32_t PageError = 0;                    			//设置PageError,如果出现错误这个变量会被设置为出错的FLASH地址
		HAL_FLASHEx_Erase(&My_Flash, &PageError);  			//调用擦除函数擦除
		
		if(PageError != 0xFFFFFFFF)
		{
				HAL_FLASH_Lock();
				return	1;	
		}
		
		HAL_FLASH_Lock();
		return 0;
}

/**
  * @brief  将程序数据写入指定Flash地址
  * @param  Address 起始地址
	* @param  Data 准备写入的数据
  * @retval 数据写入状态，0-写入数据成功，1-写入数据失败
  */
rt_uint8_t CAN_BOOT_ProgramWord(rt_uint32_t Address, rt_uint32_t Data)
{
		uint32_t WriteError = 0;
		HAL_FLASH_Unlock();
		
		WriteError = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,Data);

		if(WriteError != HAL_OK)
		{
				HAL_FLASH_Lock();
				return	1;	
		}
		else{
				HAL_FLASH_Lock();
				return	0;	
		}
}





