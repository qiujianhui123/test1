#include "User_Control.h"
#include "GlobalVarShared.h"

//全局变量定义，后期考虑是否要拆分
SYS_INFO Sys_info; 

//常规宏定义
#define   DELAY_1S					100																	//延时宏只在本函数生效
#define   DELAY_1S5					150	
#define   DELAY_10S					1000

rt_bool_t    Force_Work_Model_Flag;														  //黑启动标志，长按置1，带故障启动

//常规变量
rt_uint8_t   DC_CHARGE_HEAT_Flag;

rt_uint8_t   Pre_Mos_delay;																			//预充MOS延时断开
rt_uint8_t   pre_count;                                         //预充次数计数，两次失败报警
rt_uint8_t   Standby_to_prech_count;                            //standby转预充模式延时计数
rt_uint8_t   Heat_Flag[4];                                      //系统设备加热标志
rt_uint8_t   Heat_Num;                                          //系统加热设备统计
rt_uint16_t  shutdown_delay;												  					//故障处理延时
rt_uint16_t  Standby_delay;																		  //standby阶段的延时启动
rt_uint16_t  pre_delay;																				  //预充阶段延时计数
rt_uint16_t  pre_fail_delay;																		//预充失败后延时处理计数
rt_uint16_t  Heat_Time_delay;
rt_uint16_t  PCS_LoseVoltageCount;
rt_uint16_t  HeatErro_ChargeCount;
rt_uint16_t  Heat_Complate_OverCount;
rt_uint16_t  NoFaultDetectDelay;
rt_uint16_t  Sys_Cell_Temp_Max;                                 //系统所有pack最大电芯温度
rt_uint16_t  Sys_Cell_Temp_Max1;

rt_uint32_t  heat_time;
rt_uint32_t  Error_time;
rt_uint32_t  Vol_under_time;

//并机变量
rt_uint8_t   Online_Num;
rt_uint16_t  Online_Voltage;
rt_uint8_t   Master_On;								//主机进预充指令
rt_uint8_t   Slave_On;								//从机进预充指令

//32 bit数据后期统一处理，尽量不用
volatile rt_uint32_t  LowTemCount;

/** ************************************************************************
 * @brief   控制逻辑函数
 **************************************************************************/
void Control_Mode(void)
{
	//由于无法读取短路时刻电流，去掉短路判断

	/******************************并机逻辑开始↓↓↓*****************************/
	//主机的工作
	if(Switch_Number == 1)
	{
			//统计在线从机数量，11s刷新一次
			if(Para_Buffer0.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[0] = 1;
			}
			else
			{
					SLAVE_Online[0] = 0;
				
					//从机1切除标志清0
//					Comm_Pack.member.Slave_Num1 = 0;
			}
			
			if(Para_Buffer1.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[1] = 1;
			}
			else
			{
					SLAVE_Online[1] = 0;
				
					//从机2切除标志清0
//					Comm_Pack.member.Slave_Num2 = 0;
			}
			
			if(Para_Buffer2.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[2] = 1;
			}
			else
			{
					SLAVE_Online[2] = 0;
				
					//从机3切除标志清0
//					Comm_Pack.member.Slave_Num3 = 0;
			}
						
			if(Sys_info.member.Work_State != STAND_BY)
			{
					Online_Num = SLAVE_Online[0] + SLAVE_Online[1] + SLAVE_Online[2] + 1;
			}
			else
			{
					Online_Num = SLAVE_Online[0] + SLAVE_Online[1] + SLAVE_Online[2];
			}
			
			
			//搜索在线设备的工作电压
			if(Sys_info.member.Work_State != STAND_BY)
			{
					//主机在线，不考虑其他是否在线，以此为准
					Online_Voltage = (Sys_info.member.Bat_Voltage * 10);
			}
			else
			{
					//搜索在线的从机电压
					if(SLAVE_Online[0] == 1)
					{
							Online_Voltage = Para_Buffer0.part.frame12.member.voltage_of_system;
					}
					else if(SLAVE_Online[1] == 1)
					{
							Online_Voltage = Para_Buffer1.part.frame12.member.voltage_of_system;
					}	
					else if(SLAVE_Online[2] == 1)
					{
							Online_Voltage = Para_Buffer2.part.frame12.member.voltage_of_system;
					}	
					else
					{
							//主机无故障才允许切入
							if(Sys_info.member.F_BattFltLev < 2)
							{
									//全部离线，默认主机切入
									Online_Voltage = (Sys_info.member.Bat_Voltage * 10);
									Master_On = 1;							
							}
							else
							{
									//主机故障，强制切入从机1
								  Comm_Pack.member.Slave_Num1 = 1;
//									if(Para_Buffer0.part.frame2.member.f_battfltlev < 2)
//									{
//									
//									}
							}
					}						
			}
		  
			//压差判断，在线数量 > 0才进行切入判断
			if(Online_Num > 0)
			{
					//筛选压差,使用*100值
					if(((Online_Voltage - (Sys_info.member.Bat_Voltage * 10)) < Voltage_Diff) \
						&& (((Sys_info.member.Bat_Voltage * 10) - Online_Voltage) < Voltage_Diff))
					{
							if((Sys_info.member.DisCharge_Empty_Falg == 0) && (Sys_info.member.sys_charge_full_flag == 0))
							{
									//主机可切入
									Master_On = 1;
							}
							else
							{
									//满充的时候放电可切入
									if((Sys_info.member.sys_charge_full_flag == 1) && (Current_Total < (-Current_Limit)))
									{
											//主机可切入
											Master_On = 1;
									}
									
									//放空的时候充电可切入
									if((Sys_info.member.DisCharge_Empty_Falg == 1) && (Current_Total > Current_Limit))
									{
											//主机可切入
											Master_On = 1;
									}
							}

					}
					
					if(((Online_Voltage - Para_Buffer0.part.frame12.member.voltage_of_system) < Voltage_Diff) \
						&& ((Para_Buffer0.part.frame12.member.voltage_of_system - Online_Voltage) < Voltage_Diff))
					{
							if((Para_Buffer0.part.frame1.member.charge_full_flag == 0) \
								&& (Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 0))
							{
									//从机1可切入
									Comm_Pack.member.Slave_Num1 = 1;
							}
							else
							{
									//满充的时候放电可切入
									if((Para_Buffer0.part.frame1.member.charge_full_flag == 1) \
										&& (Current_Total < (-Current_Limit)))
									{
												//从机1可切入
												Comm_Pack.member.Slave_Num1 = 1;
									}
									
									//放空的时候充电可切入
									if((Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
									{
											//从机1可切入
											Comm_Pack.member.Slave_Num1 = 1;
									}
							}
					}
					
					if(((Online_Voltage - Para_Buffer1.part.frame12.member.voltage_of_system) < Voltage_Diff) \
						&& ((Para_Buffer1.part.frame12.member.voltage_of_system - Online_Voltage) < Voltage_Diff))
					{
							if((Para_Buffer1.part.frame1.member.charge_full_flag == 0) \
								&& (Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 0))
							{
										//从机2可切入
										Comm_Pack.member.Slave_Num2 = 1;
							}
							else
							{
										//满充的时候放电可切入
										if((Para_Buffer1.part.frame1.member.charge_full_flag == 1) \
											&& (Current_Total < (-Current_Limit)))
										{
													//从机2可切入
													Comm_Pack.member.Slave_Num2 = 1;
										}
										
										//放空的时候充电可切入
										if((Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
										{
												//从机2可切入
												Comm_Pack.member.Slave_Num2 = 1;
										}
							}
					}
					
					if(((Online_Voltage - Para_Buffer2.part.frame12.member.voltage_of_system) < Voltage_Diff) \
						&& ((Para_Buffer2.part.frame12.member.voltage_of_system - Online_Voltage) < Voltage_Diff))
					{
							if((Para_Buffer2.part.frame1.member.charge_full_flag == 0) \
								&& (Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 0))
							{
										//从机3可切入
										Comm_Pack.member.Slave_Num3 = 1;
							}
							else
							{
										//满充的时候放电可切入
										if((Para_Buffer2.part.frame1.member.charge_full_flag == 1) \
											&& (Current_Total < (-Current_Limit)))
										{
													//从机3可切入
													Comm_Pack.member.Slave_Num3 = 1;
										}
										
										//放空的时候充电可切入
										if((Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
										{
												//从机3可切入
												Comm_Pack.member.Slave_Num3 = 1;
										}
							}
					}					
			}

			//满充判断，在线数量 > 1才进行切出判断
			if(Online_Num > 1)
			{
					//放空，且还在放电，切出
					if((Sys_info.member.DisCharge_Empty_Falg == 1) && (Current_Total < (-Current_Limit)))
					{							
							//主机可切出
							Master_On = 0;
						  Sys_info.member.Work_State = STAND_BY;					
					}
					
					//满充，且还在充，切出
					if((Sys_info.member.sys_charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//主机可切出
							Master_On = 0;
							Sys_info.member.Work_State = STAND_BY;	
					}
				
					//从机1
					//放空，且还在放电，切出
					if((Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//从机1可切出
							Comm_Pack.member.Slave_Num1 = 0;				
					}
					
					//满充，且还在充，切出
					if((Para_Buffer0.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//从机1可切出
							Comm_Pack.member.Slave_Num1 = 0;
					}

					//从机2
					//放空，且还在放电，切出
					if((Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//从机2可切出
							Comm_Pack.member.Slave_Num2 = 0;				
					}
					
					//满充，且还在充，切出
					if((Para_Buffer1.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//从机2可切出
							Comm_Pack.member.Slave_Num2 = 0;
					}

					//从机3
					//放空，且还在放电，切出
					if((Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//从机3可切出
							Comm_Pack.member.Slave_Num3 = 0;				
					}
					
					//满充，且还在充，切出
					if((Para_Buffer2.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//从机3可切出
							Comm_Pack.member.Slave_Num3 = 0;
					}									
			}		
			
	}
	else if(Switch_Number == 2)
	{
			//从机1代码
			if(Comm_Pack.member.Slave_Num1 == 1)
			{
					Slave_On = 1;
			}
			else if(Comm_Pack.member.Slave_Num1 == 0)
			{
					Slave_On = 0;
					Sys_info.member.Work_State = STAND_BY;
			}
			else
			{}
							
		
	}
	else if(Switch_Number == 3)
	{
			//从机2代码
			if(Comm_Pack.member.Slave_Num2 == 1)
			{
					Slave_On = 1;
			}
			else if(Comm_Pack.member.Slave_Num2 == 0)
			{
					Slave_On = 0;
					Sys_info.member.Work_State = STAND_BY;
			}
			else
			{}
	}
	else if(Switch_Number == 4)
	{
			//从机3代码
			if(Comm_Pack.member.Slave_Num3 == 1)
			{
					Slave_On = 1;
			}
			else if(Comm_Pack.member.Slave_Num3 == 0)
			{
					Slave_On = 0;
					Sys_info.member.Work_State = STAND_BY;
			}
			else
			{}
	}
	else
	{}

	/******************************并机程序结束↑↑↑*****************************/
	//进入待机模式
	if(Sys_info.member.Sleep_Flag == 1)
	{
			Sys_info.member.Sleep_Flag = 0;
		
			//清除Debug相关寄存器
//			*((uint32_t*)DBGMCU_BASE) = 0;
//			*((uint32_t*)(DBGMCU_BASE+4)) = 0;
		
			Sys_Enter_Standby();
	}
	
	//TODO由于去掉短路，此处逻辑有修改
	/******************************状态转换************************************/
	if((Sys_info.member.Charge_warning_level >= FAULT_LEVEL) && (Force_Work_Model_Flag == RT_FALSE))
	{
			Sys_info.member.Work_State = STAND_BY;
			
			//充电故障，先断开继电器
			shutdown_delay++;
			if(shutdown_delay < DELAY_1S)
			{
					Main_Positive_Relay_Con(0);					//主继电器	
			}
			else
			{
					shutdown_delay = DELAY_1S;
					Precharge_MosFET_Con(0);						//预充MOS				 
					Heater_MosFET_Con(0);								//加热MOS
					DisCharge_MosFET_Con(0);						//放电MOS
					Charge_MosFET_Con(0);								//充电MOS
			}	
	}
	else if((Sys_info.member.Discharge_warning_level >= FAULT_LEVEL) && (Force_Work_Model_Flag == RT_FALSE))
	{
			Sys_info.member.Work_State = STAND_BY;
			
			shutdown_delay++;
			if(shutdown_delay < DELAY_1S)
			{
					Main_Positive_Relay_Con(0);					//主继电器	
			}
			else
			{
					shutdown_delay = DELAY_1S;
					Precharge_MosFET_Con(0);						//预充MOS				 
					Heater_MosFET_Con(0);								//加热MOS
					DisCharge_MosFET_Con(0);						//放电MOS
					Charge_MosFET_Con(0);								//充电MOS
			}	
	}
	else
	{
			if(Sys_info.member.Work_State == STAND_BY) 					//1S待机            
			{				
					Standby_delay++;
					if(Standby_delay < DELAY_1S)
					{
							Main_Positive_Relay_Con(0);									//主继电器
					}
					else
					{							
							//延时500ms进入预充模式
							if(Standby_delay > DELAY_1S5)
							{
									Standby_delay = 151;
								
									//TODO重新核对开机条件
									//单机运行，无故障或黑启动，可直接启动
									//并机运行，从机不允许黑启动
									if(Switch_Number == 0)
									{
											if((Force_Work_Model_Flag == RT_TRUE) || (Sys_info.member.Pcs_commu_fail_warning_level < FAULT_LEVEL))
											{											 
													Sys_info.member.Work_State = PRECHARGE;
											}
									}
									else if(Switch_Number == 1)
									{
											if(((Force_Work_Model_Flag == RT_TRUE) \
												|| (Sys_info.member.Pcs_commu_fail_warning_level < FAULT_LEVEL)) && (Master_On == 1))
											{											 
													Sys_info.member.Work_State = PRECHARGE;
											}
									}
									else
									{
											if((Sys_info.member.Pcs_commu_fail_warning_level < FAULT_LEVEL) && (Slave_On == 1))
											{											 
													Sys_info.member.Work_State = PRECHARGE;
											}
									}
							}		
							else
							{
									Precharge_MosFET_Con(0);										//预充MOS				 
									Heater_MosFET_Con(0);												//加热MOS
									DisCharge_MosFET_Con(0);										//放电MOS------该指令用时25ms，会打破Task，尽量不连续使用
									Charge_MosFET_Con(0);												//充电MOS------该指令用时25ms，会打破Task，尽量不连续使用
								
									//待机延时计数清0
									shutdown_delay = 0;
									pre_delay = 0;
									Pre_Mos_delay = 0;
							}


					}
			}
			else if(Sys_info.member.Work_State == PRECHARGE)    //预充模式
			{			
					Standby_delay = 0;
				
					//外部电压达到电池电压90%
					if(Sys_info.member.Pack_Voltage > Sys_info.member.Bat_Voltage * 0.95)
					{
							Sys_info.member.Work_State = READY;
						
							//预充ok，预充计数清0
							pre_count = 0;
					}
				
					//预留10s进行预充
					if((pre_delay < DELAY_10S) && (Sys_info.member.Precharge_Fail == 0))
					{
							pre_delay++;
							Precharge_MosFET_Con(1);										    //预充MOS	
							Main_Positive_Relay_Con(1);									    //主继电器
					}
					else//预充失败
					{
							pre_delay = 0;
						
							//进行二次预充
							Sys_info.member.Work_State = STAND_BY;
							pre_count++; 

							//两次预充失败报警
						  if(pre_count >= 2)
							{
									Sys_info.member.Precharge_Fail = 1;	
									Force_Work_Model_Flag = 0;								
							}			

							//TODO预充失败，会进入前面的故障处理，此处只断开继电器即可，需要验证一下
							Main_Positive_Relay_Con(0);									    //主继电器				
					}																		
			}
			else if(Sys_info.member.Work_State == READY)  			    //预充模式完成
			{
					 Pre_Mos_delay++;
				
					 if(Pre_Mos_delay < 5)
					 {
							 Main_Positive_Relay_Con(0);										//主继电器					
					 }
					 else if((Pre_Mos_delay > 30) && (Pre_Mos_delay < 60))
					 {
							 DisCharge_MosFET_Con(1);												//放电MOS
							 Charge_MosFET_Con(1);													//充电MOS					 					 
							 Heater_MosFET_Con(0);													//加热MOS					 
					 }									
					 else if(Pre_Mos_delay > DELAY_1S)
					 {
							 Pre_Mos_delay = 101;
						   Main_Positive_Relay_Con(1);										//主继电器
							 Precharge_MosFET_Con(0);										    //预充MOS
						 
							 if(Sys_info.member.Current_Status == DISCHARGE)
							 {
									Sys_info.member.Work_State = DC_DISCHARGE; 							 
							 }

							 //如果电流状态为充电且最低温度大于等于2℃
							 if(Sys_info.member.Current_Status == CHARGE)
							 {
									LowTemCount = 0;
									Sys_info.member.Work_State = DC_CHARGE;
							 }
					 }
					 else
					 {}
			}
			else if(Sys_info.member.Work_State == DC_DISCHARGE)
			{
					 Heater_MosFET_Con(0);														//加热MOS
					 LowTemCount = 0;
				
					 //SOC小于90%，清除充满标志
					 if(Sys_info.member.sys_charge_full_flag == 1)
					 {
							if(Sys_info.member.module_soc < 225)						
							{								
									Sys_info.member.sys_charge_full_flag = 0;
							}														
																			
					 }	
					 
					 //TODO如果电流状态为充电且大于1A
					 if((Sys_info.member.Current_Status == CHARGE) && (Sys_info.member.Cur1 > 1000))
					 {
							Sys_info.member.Work_State = DC_CHARGE; 	
					 }											
			}
			else if(Sys_info.member.Work_State == DC_CHARGE) 
			{				
					//TODO温度判定需要分析
				  //修改加热逻辑，去掉加热故障
					if(Sys_info.member.Cell_Tmp_Min <= 400)				
					{
							//低温下最小充电电流是5A
							if(LowTemCount <= 4000)									//低温持续40s，之后在进入加热模式
							{
									LowTemCount++;
							}
							else
							{
									Sys_info.member.Work_State = DC_CHARGE_HEAT;
									LowTemCount = 0;
									Heat_Time_delay = 0;
									PCS_LoseVoltageCount = 0;
									heat_time = 0;
							}												 
					}
					else//一旦不满足低温要求，就重新计数
					{
							LowTemCount = 0;					
					}					
													
					/****************************充电逻辑******************************/
					if(Sys_info.member.Cell_Tmp_Min >= 420)
					{
							Sys_info.member.Max_Charge_Voltage = 580;
																					  								
							//加热完成，闭合主继电器
							if(Sys_info.member.Heat_Complete_Flag == 1)
							{
									Main_Positive_Relay_Con(1);															//主继电器
								
									//延时1s断开加热MOS
									if(Heat_Complate_OverCount <= 100)
									{
											Heat_Complate_OverCount++;
									}
									else
									{
											Heater_MosFET_Con(0);																	//加热MOS
											Heat_Complate_OverCount = 0;
											Sys_info.member.Heat_Complete_Flag = 0;
									}									
							}																																					
										
								//屏蔽，防止SOC不准导致满充标志被清0，满充校准1s执行一次，会被覆盖
//							if(Sys_info.member.module_soc < 225)											//大于90%禁止充电
//							{								
//									Sys_info.member.sys_charge_full_flag = 0;
//							}																																		
					}
					
				  if((Sys_info.member.Current_Status == DISCHARGE) && (Sys_info.member.Cur1 > 1000))
					{
							Sys_info.member.Work_State = DC_DISCHARGE; 					
					}			
			}
			else if(Sys_info.member.Work_State == DC_CHARGE_HEAT)  					//加热模式 
			{
					//Pack电压高于10V且温度采样正常
					if(Sys_info.member.Pack_Voltage >= 100)
					{  
							//电芯温度和温度采样点温差为2℃   当采样点温度为5℃时,加热完成
							if(Sys_info.member.Cell_Tmp_Min >= 450) 
							{			
									Sys_info.member.Heat_Complete_Flag = 1;//加热完成后需要进行操作，先合主继电器，确保合上之后再断开加热mos，再跳出循环，进入充电模式，合主继电器															
									Sys_info.member.Work_State = DC_CHARGE;							//充电完成后跳转到充电状态
									LowTemCount = 0;																		//加热成功或者失败均清除低温计时
									heat_time = 0;																			//加热成功清除加热时间计时
							}
							else
							{
									Heater_MosFET_Con(1);																//加热MOS
								
								  if(Heat_Time_delay <= 100)													//闭合加热MOS后1s延时，断开主继电器
								  {
											Heat_Time_delay ++;
								  }
									else
									{
											Main_Positive_Relay_Con(0);											//主继电器
											Heat_Time_delay = 101;
											
											heat_time++;
											//TODO考虑加计时判断
										
									}								
							}												
					}
					else	
					{
							Heater_MosFET_Con(0);																		//断开加热MOS		
							LowTemCount = 0;																				//加热成功或者失败均清除低温计时
							Sys_info.member.Work_State = STAND_BY;	
              heat_time = 0;						
					}	
			}			
	}		
	
	if(Sys_info.member.F_BattFltLev >= 2)
	{		
			Error_time++;
			if(Error_time > 360000)//1个小时
			{
					//TODO后面补充
//					Standby_Entering();	//进入待机模式			
			}
			NoFaultDetectDelay = 0;
	}
	else
	{
			Error_time=0;
			//延时清除，80s，待故障检测完成后再检测是否清除 强起标志位
			NoFaultDetectDelay++;
			if(NoFaultDetectDelay >= 8000)
			{
					Force_Work_Model_Flag = 0;
					NoFaultDetectDelay = 8001;
			}
	}
    if(Sys_info.member.Cell_vol_under_warning[0] == 0)
    {
        Vol_under_time++;
        if(Vol_under_time > 360000)//1个小时
			{
				Sys_info.member.Sleep_Flag = 1;	
                Vol_under_time = 0;
			}
    }
    else
    {
        Vol_under_time = 0;
    }
		
	
}

