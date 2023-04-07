
#include "GlobalVarShared.h"

rt_uint8_t Read_V_I_Complete_Flag;//电压电流采集完成标志
AFEDATA 		AFE;
AFE_RAM     Afe_Ram;
rt_uint8_t AFE_EEPROM_Writes;										//记录AFE的eeprom擦写次数

void StartUp_UpdataAfeConfig(void);

//调试变量
AFE_EEPROM_CONFIG Afe_Eeprom_Config;
rt_uint8_t data_test[30];



/*******************************************************************************
Function:StartUp_UpdataAfeConfig()
Description:检查SH367309的配置跟外置Eeprom是否一致，不一致就要更新SH367309的配置信息
Input:	NULL
Output: NULL
Others:
*******************************************************************************/
void StartUp_UpdataAfeConfig(void)
{
		rt_uint8_t data[30];
		// 读出AFE的eeprom数据
		MTPRead(0,26,data);

		//判断AFE的参数与外置Eeprom是否一致,memcmp一致，返回0
		if(rt_memcmp(&data,&Eeprom_Afe,25) != 0)
		{
				//不一致，重新擦写AFE的eeprom
				rt_uint8_t i;

				AFE_EEPROM_Writes = M24256_Byte_Read(0x182);
			
				//AFE的Eeprom烧写超过100次，报警
				if(AFE_EEPROM_Writes < 100)
				{
						//开启EEPROM  8V供电
						AFE_VPRO_ON();
						rt_thread_mdelay(10);
					
						//此处注意，AFE只能写前25个字节，0-0x18
						i = MTPWriteROM(0x00,25,(U8  *)&Eeprom_Afe.buf[0]);
					
						if(i)
						{
								//烧写失败，关闭8V供电
								AFE_VPRO_OFF();
								Sys_info.member.AFE_Product_Config_Err = 1;	
						}
						else
						{
								Soft_ResetAFE();																		 //软件复位	
								AFE_VPRO_OFF();																			 //断电
							
								//刷写次数增加，并保存，目前3次
								AFE_EEPROM_Writes++;
								M24256_Byte_Write(AFE_EEPROM_Writes,0x182);					 //记录擦写次数
						}
				}
				else
				{
						Sys_info.member.AFE_Product_Config_Err = 1;	
				}

			
		}

//			//AFE运行前需要复位
      Soft_ResetAFE();
}

/*******************************************************************************
Function:Reset_AFE_EEPROM(void)
Description:  check SH367309 is ready, and initialization MTP Buffer
Input:	NULL
Output: NULL
Others:

-----------------------------------------------------------------------------------------------------------------------------------
|	PFV							|							OV	|		OVR				|		UVR					|		UV	        |	VPD	         |	PREV					|		L0V	        |
-----------------------------------------------------------------------------------------------------------------------------------
|二次过充电保护电压|过充电保护电压	|过充电恢复电压|过放电恢复电压|过放电保护电压 | 下电允许电压 |  预充电开启电压 |低电压禁止充电电压|
由高到低的关系
*******************************************************************************/
void Reset_AFE_EEPROM(void)
{
	//--------------------------------------------------MOS的动作保护要提升到2.5V ---------------------------------------------
	// SCONF1系统配置    00H
	Afe_Eeprom_Config.member.CellNum = 0x0;	  // 配置＜5代表16串
	Afe_Eeprom_Config.member.Balance = 0x1;	  // 1:平衡由外部MCU控制，平衡时序仍由SH367309内部逻辑控制 0:由SH367309内部逻辑控制
	Afe_Eeprom_Config.member.OverCurPM = 0x0; // 充电过流只关闭充电MOSFET，放电过流只关闭放电MOSFET
	Afe_Eeprom_Config.member.ENMOS = 0x1;	  // 检测到放电过流1或放电状态，则开启充电MOS
	Afe_Eeprom_Config.member.ENPCH = 0x0;	  // 禁用预充电功能
	//
	// SCONF2系统配置  01H
	Afe_Eeprom_Config.member.EUVR = 0x00;	// 过放电保护状态释放与负载无关
	Afe_Eeprom_Config.member.OCRA = 0x01;	// 允许电流保护定时恢复，恢复时间32秒
	Afe_Eeprom_Config.member.CTLC = 0x00;	// 充放电和预充电由内部逻辑控制  CTL引脚无效
	Afe_Eeprom_Config.member.DIS_PF = 0x01; // 禁止二次过充电保护
	Afe_Eeprom_Config.member.UV_OP = 0x00;	// 过放电只关闭放电MOS
	Afe_Eeprom_Config.member.E0VB = 0x00;	// 关闭“禁止低压电芯充电功能”
	//
	// 02H一次过充电保护延时40S
	Afe_Eeprom_Config.member.OVT = 0x0F; // 过充电保护延时40S
	// 一次过充电保护电压3.65V
	Afe_Eeprom_Config.member.OV8_9 = 0x02; // 寄存器值*5mV
	//
	// 03H 一次过充电保护电压
	Afe_Eeprom_Config.member.OV0_7 = 0xF8; // 过充保护电压3.8V   单位 X5mV

	// 04H欠压保护延时
	Afe_Eeprom_Config.member.UVT = 0x0F; // 过放电保护延时40S
	// 一次过充电保护恢复电压
	Afe_Eeprom_Config.member.OVR8_9 = 0x02;

	// 05H一次过充电保护恢复电压
	Afe_Eeprom_Config.member.OVR0_7 = 0xD0; // 过充电保护恢复电压3600mV  单位 X5mV

	// 06H过放电保护电压
	Afe_Eeprom_Config.member.UV = 0x5A; //*20mV  1800mV

	// 07H过放电恢复电压
	Afe_Eeprom_Config.member.UVR = 0x6E; //*20mV  2200mV

	// 08H平衡开启电压---不启用
	Afe_Eeprom_Config.member.BALV = 0xAA; //*20mV  3400mV

	// 09H预充开启电压---不启用     //*20mV  2500mV
	Afe_Eeprom_Config.member.PREV = 0x7D;
	// 0AH低电压禁止充电电压---不启用
	Afe_Eeprom_Config.member.L0V = 0x64; //*20mV  2000mV

	// 0BH二次过充电保护电压---不启用
	Afe_Eeprom_Config.member.PFV = 0xB9; // 过充保护电压3.7V   X20mV

	// 0CH放电过流1电流设置
	Afe_Eeprom_Config.member.OCD1V = 0x0F; // 200mV  最大值--500A
	// 放电过流1延时40S
	Afe_Eeprom_Config.member.OCD1T = 0x0F; // 40S

	// 0DH放电过流2设置   1250A
	Afe_Eeprom_Config.member.OCD2V = 0x0F; //   1250A
	// 放电过流2延时  20S
	Afe_Eeprom_Config.member.OCD2T = 0x0F;

	// 0EH短路保护设置寄存器
	Afe_Eeprom_Config.member.SCV = 0x0F; // 1000mV-2500A
	// 短路保护延时  64us
	Afe_Eeprom_Config.member.SCT = 0x0F; // 960us

	// 0FH充电过流1设置寄存器   500A
	Afe_Eeprom_Config.member.OCCV = 0x0F; //   200mV/0.4mΩ=500A
	// 充电过流1延时1S
	Afe_Eeprom_Config.member.OCCT = 0x0F; // 20s

	// 10H  CHS/MOST/OCRT/PFT
	// 二次过充电保护延时---不启用
	Afe_Eeprom_Config.member.PFT = 0x00; // 8S
	// 充放电过流自恢复延时设置  32S
	Afe_Eeprom_Config.member.OCRT = 0x02;
	// 充放电MOS开启延时设置 512us
	Afe_Eeprom_Config.member.MOST = 0x03;
	// 充放电状态检测电压设置  1A   200uV
	Afe_Eeprom_Config.member.CHS = 0x00; // 40mV   0.2mΩ   160A   400ms

	////11H充电高温保护110℃---不启用
	Afe_Eeprom_Config.member.OTC = 0x21;

	////12H充电高温保护释放100℃---不启用
	Afe_Eeprom_Config.member.OTCR = 0x2C;

	////13H充电低温保护0℃---不启用
	Afe_Eeprom_Config.member.UTC = 0xA5;

	////14H充电低温保护释放5℃---不启用
	Afe_Eeprom_Config.member.UTCR = 0x91;

	////15H放电高温保护110℃---不启用
	Afe_Eeprom_Config.member.OTD = 0x21;

	////16H放电高温保护释放100℃---不启用
	Afe_Eeprom_Config.member.OTDR = 0x2C;

	////17H放电低温保护-30℃---不启用
	Afe_Eeprom_Config.member.UTD = 0xEC;

	////18H放电低温保护释放-25℃---不启用
	Afe_Eeprom_Config.member.UTDR = 0xE6;

	////19H温度内部参考电阻系数---不启用
	Afe_Eeprom_Config.member.TR = 0x04;
}

//电压处理  最快支持100ms处理一次  电压转换周期为100ms/16串
void Cell_voltage_Current_Process(void)
{	
		rt_uint8_t i=0,first_max=0,first_min=0,number_max=0,number_min=0;
		rt_uint16_t max_temp16,min_temp16=0;
		rt_uint32_t sum=0;
		
		float Current=0,Sample_R=0.202;
		rt_uint16_t Temp_Curl;
		rt_uint16_t Curl_H=0;
		rt_uint16_t Curl_L=0;
		
//		static rt_uint32_t temp=0;

		Read_V_I_Complete_Flag=0;
	
		//计算电流及工作模式切换
		Curl_H = AFE.Cur1&0x00ff;
		Curl_L = AFE.Cur1&0xff00;
		Temp_Curl=(Curl_H<<8)|(Curl_L>>8);
	
		if(Temp_Curl&0x8000)//如果电流值为负  放电
		{
				Temp_Curl =~ Temp_Curl+1;					  
				Current = Temp_Curl*7.45;
				Current = Current/Sample_R;//mA
				if(Current < 1000)
				{							
						Sys_info.member.Cur1 = 0;	
						Sys_info.member.Current_Status = 0;
				}
				else
				{
						Sys_info.member.Current_Status = DISCHARGE;
						Sys_info.member.Cur1 = Current;								//单位：mA
				}
			
		}
		else//如果电流值为正  充电
		{					  
				Current = Temp_Curl*7.45;//mA
				Current = Current/Sample_R;
				
				if(Current < 1000)
				{						
						Sys_info.member.Cur1 = 0;	
						Sys_info.member.Current_Status = 0;
				}
				else
				{
						Sys_info.member.Current_Status = CHARGE;
						Sys_info.member.Cur1 = Current;
				}					 
		}
		
		for(i=0;i<16;i++)
		{
				rt_uint16_t Volt_Cell[16];
				rt_uint16_t Cell_H=0;
				rt_uint16_t Cell_L=0;
			
				Cell_H = AFE.Cell[i]&0x00ff;
				Cell_L = AFE.Cell[i]&0xff00;
				Volt_Cell[i] = (Cell_H<<8)|(Cell_L>>8);
			
				Volt_Cell[i] = Volt_Cell[i]/32;
				Volt_Cell[i] = Volt_Cell[i]*5;
				Sys_info.member.Cell_volt[i] = Volt_Cell[i];
				//模块总电压，单位：0.001伏
			
		}
		//电压补偿
		if(Sys_info.member.Current_Status == DISCHARGE)//放电  第九串电压低  铜排内阻1.2欧姆
		{
				Sys_info.member.Cell_volt[8] = Sys_info.member.Cell_volt[8] + Sys_info.member.Cur1/1000*0.3;
		}
		else if(Sys_info.member.Current_Status == CHARGE)//充电  第九串电压高  铜排内阻1.3欧姆
		{
				Sys_info.member.Cell_volt[8] = Sys_info.member.Cell_volt[8] - Sys_info.member.Cur1/1000*0.3;				
		}
		
		//最大、最小电压及编号          
		for(i=0;i<16;i++)
		{
				sum += Sys_info.member.Cell_volt[i]; 
				//计算最大值
				if(first_max==0)     							
				{			
					first_max = 1;																		
					max_temp16 = Sys_info.member.Cell_volt[i];       
					number_max = i+1;       																		                                     
				}
				else
				{
					if(Sys_info.member.Cell_volt[i] > max_temp16)
					{
							max_temp16 = Sys_info.member.Cell_volt[i];       
							number_max = i+1;       												 
					}
				}
				
				//计算最小值
				if(first_min==0)
				{
					first_min = 1;										
					min_temp16 = Sys_info.member.Cell_volt[i];       
					number_min = i+1;       																			
				}
				else
				{
					if(Sys_info.member.Cell_volt[i] < min_temp16)
					{
							min_temp16 = Sys_info.member.Cell_volt[i];       
							number_min = i+1; 				
					}
				}
		}       
		
		Sys_info.member.Cell_Volt_Max = max_temp16;
		Sys_info.member.Cell_Volt_Min = min_temp16;
		Sys_info.member.Cell_Volt_Max_num = number_max;
		Sys_info.member.Cell_Volt_Min_num = number_min;
		Sys_info.member.Bat_Voltage = sum/100;//总电压0.1V
		
	  //计算整个板子的压差
		if(max_temp16 > min_temp16)
		{
				Sys_info.member.Cell_volt_Deff = max_temp16 - min_temp16;
		}
		else
		{
				Sys_info.member.Cell_volt_Deff = 0; 
		}    	
		
		system_vol_cal=1;
		max_vol_cal=1;
		min_vol_cal=1;			
		current_cal=1;
}



