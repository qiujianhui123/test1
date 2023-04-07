#include "flash_api.h"



/**
  * @brief  �����û����򴢴�������
  * @param  StartAddr ��ʼ��ַ
	* @param  EndAddr ������ַ
  * @retval ���ݲ���״̬��0-�������ݳɹ���1-��������ʧ��
  */
rt_uint8_t CAN_BOOT_ErasePage(rt_uint32_t StartAddr,rt_uint32_t NbPage)
{
		FLASH_EraseInitTypeDef My_Flash;  							//���� FLASH_EraseInitTypeDef �ṹ��Ϊ My_Flash
		HAL_FLASH_Unlock();               							//����Flash
		
		My_Flash.TypeErase = FLASH_TYPEERASE_PAGES;  		//����Flashִ��ҳ��ֻ����������
		My_Flash.PageAddress = StartAddr;  							//����Ҫ�����ĵ�ַ
		My_Flash.NbPages = NbPage;                      //˵��Ҫ������ҳ�����˲���������Min_Data = 1��Max_Data =(���ҳ��-��ʼҳ��ֵ)֮���ֵ
		
		uint32_t PageError = 0;                    			//����PageError,������ִ�����������ᱻ����Ϊ�����FLASH��ַ
		HAL_FLASHEx_Erase(&My_Flash, &PageError);  			//���ò�����������
		
		if(PageError != 0xFFFFFFFF)
		{
				HAL_FLASH_Lock();
				return	1;	
		}
		
		HAL_FLASH_Lock();
		return 0;
}

/**
  * @brief  ����������д��ָ��Flash��ַ
  * @param  Address ��ʼ��ַ
	* @param  Data ׼��д�������
  * @retval ����д��״̬��0-д�����ݳɹ���1-д������ʧ��
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





