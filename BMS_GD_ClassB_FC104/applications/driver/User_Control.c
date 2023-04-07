#include "User_Control.h"
#include "GlobalVarShared.h"

//ȫ�ֱ������壬���ڿ����Ƿ�Ҫ���
SYS_INFO Sys_info; 

//����궨��
#define   DELAY_1S					100																	//��ʱ��ֻ�ڱ�������Ч
#define   DELAY_1S5					150	
#define   DELAY_10S					1000

rt_bool_t    Force_Work_Model_Flag;														  //��������־��������1������������

//�������
rt_uint8_t   DC_CHARGE_HEAT_Flag;

rt_uint8_t   Pre_Mos_delay;																			//Ԥ��MOS��ʱ�Ͽ�
rt_uint8_t   pre_count;                                         //Ԥ���������������ʧ�ܱ���
rt_uint8_t   Standby_to_prech_count;                            //standbyתԤ��ģʽ��ʱ����
rt_uint8_t   Heat_Flag[4];                                      //ϵͳ�豸���ȱ�־
rt_uint8_t   Heat_Num;                                          //ϵͳ�����豸ͳ��
rt_uint16_t  shutdown_delay;												  					//���ϴ�����ʱ
rt_uint16_t  Standby_delay;																		  //standby�׶ε���ʱ����
rt_uint16_t  pre_delay;																				  //Ԥ��׶���ʱ����
rt_uint16_t  pre_fail_delay;																		//Ԥ��ʧ�ܺ���ʱ�������
rt_uint16_t  Heat_Time_delay;
rt_uint16_t  PCS_LoseVoltageCount;
rt_uint16_t  HeatErro_ChargeCount;
rt_uint16_t  Heat_Complate_OverCount;
rt_uint16_t  NoFaultDetectDelay;
rt_uint16_t  Sys_Cell_Temp_Max;                                 //ϵͳ����pack����о�¶�
rt_uint16_t  Sys_Cell_Temp_Max1;

rt_uint32_t  heat_time;
rt_uint32_t  Error_time;
rt_uint32_t  Vol_under_time;

//��������
rt_uint8_t   Online_Num;
rt_uint16_t  Online_Voltage;
rt_uint8_t   Master_On;								//������Ԥ��ָ��
rt_uint8_t   Slave_On;								//�ӻ���Ԥ��ָ��

//32 bit���ݺ���ͳһ������������
volatile rt_uint32_t  LowTemCount;

/** ************************************************************************
 * @brief   �����߼�����
 **************************************************************************/
void Control_Mode(void)
{
	//�����޷���ȡ��·ʱ�̵�����ȥ����·�ж�

	/******************************�����߼���ʼ������*****************************/
	//�����Ĺ���
	if(Switch_Number == 1)
	{
			//ͳ�����ߴӻ�������11sˢ��һ��
			if(Para_Buffer0.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[0] = 1;
			}
			else
			{
					SLAVE_Online[0] = 0;
				
					//�ӻ�1�г���־��0
//					Comm_Pack.member.Slave_Num1 = 0;
			}
			
			if(Para_Buffer1.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[1] = 1;
			}
			else
			{
					SLAVE_Online[1] = 0;
				
					//�ӻ�2�г���־��0
//					Comm_Pack.member.Slave_Num2 = 0;
			}
			
			if(Para_Buffer2.part.frame12.member.Online_State == 1)
			{
					SLAVE_Online[2] = 1;
			}
			else
			{
					SLAVE_Online[2] = 0;
				
					//�ӻ�3�г���־��0
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
			
			
			//���������豸�Ĺ�����ѹ
			if(Sys_info.member.Work_State != STAND_BY)
			{
					//�������ߣ������������Ƿ����ߣ��Դ�Ϊ׼
					Online_Voltage = (Sys_info.member.Bat_Voltage * 10);
			}
			else
			{
					//�������ߵĴӻ���ѹ
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
							//�����޹��ϲ���������
							if(Sys_info.member.F_BattFltLev < 2)
							{
									//ȫ�����ߣ�Ĭ����������
									Online_Voltage = (Sys_info.member.Bat_Voltage * 10);
									Master_On = 1;							
							}
							else
							{
									//�������ϣ�ǿ������ӻ�1
								  Comm_Pack.member.Slave_Num1 = 1;
//									if(Para_Buffer0.part.frame2.member.f_battfltlev < 2)
//									{
//									
//									}
							}
					}						
			}
		  
			//ѹ���жϣ��������� > 0�Ž��������ж�
			if(Online_Num > 0)
			{
					//ɸѡѹ��,ʹ��*100ֵ
					if(((Online_Voltage - (Sys_info.member.Bat_Voltage * 10)) < Voltage_Diff) \
						&& (((Sys_info.member.Bat_Voltage * 10) - Online_Voltage) < Voltage_Diff))
					{
							if((Sys_info.member.DisCharge_Empty_Falg == 0) && (Sys_info.member.sys_charge_full_flag == 0))
							{
									//����������
									Master_On = 1;
							}
							else
							{
									//�����ʱ��ŵ������
									if((Sys_info.member.sys_charge_full_flag == 1) && (Current_Total < (-Current_Limit)))
									{
											//����������
											Master_On = 1;
									}
									
									//�ſյ�ʱ���������
									if((Sys_info.member.DisCharge_Empty_Falg == 1) && (Current_Total > Current_Limit))
									{
											//����������
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
									//�ӻ�1������
									Comm_Pack.member.Slave_Num1 = 1;
							}
							else
							{
									//�����ʱ��ŵ������
									if((Para_Buffer0.part.frame1.member.charge_full_flag == 1) \
										&& (Current_Total < (-Current_Limit)))
									{
												//�ӻ�1������
												Comm_Pack.member.Slave_Num1 = 1;
									}
									
									//�ſյ�ʱ���������
									if((Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
									{
											//�ӻ�1������
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
										//�ӻ�2������
										Comm_Pack.member.Slave_Num2 = 1;
							}
							else
							{
										//�����ʱ��ŵ������
										if((Para_Buffer1.part.frame1.member.charge_full_flag == 1) \
											&& (Current_Total < (-Current_Limit)))
										{
													//�ӻ�2������
													Comm_Pack.member.Slave_Num2 = 1;
										}
										
										//�ſյ�ʱ���������
										if((Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
										{
												//�ӻ�2������
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
										//�ӻ�3������
										Comm_Pack.member.Slave_Num3 = 1;
							}
							else
							{
										//�����ʱ��ŵ������
										if((Para_Buffer2.part.frame1.member.charge_full_flag == 1) \
											&& (Current_Total < (-Current_Limit)))
										{
													//�ӻ�3������
													Comm_Pack.member.Slave_Num3 = 1;
										}
										
										//�ſյ�ʱ���������
										if((Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total > Current_Limit))
										{
												//�ӻ�3������
												Comm_Pack.member.Slave_Num3 = 1;
										}
							}
					}					
			}

			//�����жϣ��������� > 1�Ž����г��ж�
			if(Online_Num > 1)
			{
					//�ſգ��һ��ڷŵ磬�г�
					if((Sys_info.member.DisCharge_Empty_Falg == 1) && (Current_Total < (-Current_Limit)))
					{							
							//�������г�
							Master_On = 0;
						  Sys_info.member.Work_State = STAND_BY;					
					}
					
					//���䣬�һ��ڳ䣬�г�
					if((Sys_info.member.sys_charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//�������г�
							Master_On = 0;
							Sys_info.member.Work_State = STAND_BY;	
					}
				
					//�ӻ�1
					//�ſգ��һ��ڷŵ磬�г�
					if((Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//�ӻ�1���г�
							Comm_Pack.member.Slave_Num1 = 0;				
					}
					
					//���䣬�һ��ڳ䣬�г�
					if((Para_Buffer0.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//�ӻ�1���г�
							Comm_Pack.member.Slave_Num1 = 0;
					}

					//�ӻ�2
					//�ſգ��һ��ڷŵ磬�г�
					if((Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//�ӻ�2���г�
							Comm_Pack.member.Slave_Num2 = 0;				
					}
					
					//���䣬�һ��ڳ䣬�г�
					if((Para_Buffer1.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//�ӻ�2���г�
							Comm_Pack.member.Slave_Num2 = 0;
					}

					//�ӻ�3
					//�ſգ��һ��ڷŵ磬�г�
					if((Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 1) && (Current_Total < (-Current_Limit)))
					{
							//�ӻ�3���г�
							Comm_Pack.member.Slave_Num3 = 0;				
					}
					
					//���䣬�һ��ڳ䣬�г�
					if((Para_Buffer2.part.frame1.member.charge_full_flag == 1) && (Current_Total > Current_Limit))
					{
							//�ӻ�3���г�
							Comm_Pack.member.Slave_Num3 = 0;
					}									
			}		
			
	}
	else if(Switch_Number == 2)
	{
			//�ӻ�1����
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
			//�ӻ�2����
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
			//�ӻ�3����
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

	/******************************�����������������*****************************/
	//�������ģʽ
	if(Sys_info.member.Sleep_Flag == 1)
	{
			Sys_info.member.Sleep_Flag = 0;
		
			//���Debug��ؼĴ���
//			*((uint32_t*)DBGMCU_BASE) = 0;
//			*((uint32_t*)(DBGMCU_BASE+4)) = 0;
		
			Sys_Enter_Standby();
	}
	
	//TODO����ȥ����·���˴��߼����޸�
	/******************************״̬ת��************************************/
	if((Sys_info.member.Charge_warning_level >= FAULT_LEVEL) && (Force_Work_Model_Flag == RT_FALSE))
	{
			Sys_info.member.Work_State = STAND_BY;
			
			//�����ϣ��ȶϿ��̵���
			shutdown_delay++;
			if(shutdown_delay < DELAY_1S)
			{
					Main_Positive_Relay_Con(0);					//���̵���	
			}
			else
			{
					shutdown_delay = DELAY_1S;
					Precharge_MosFET_Con(0);						//Ԥ��MOS				 
					Heater_MosFET_Con(0);								//����MOS
					DisCharge_MosFET_Con(0);						//�ŵ�MOS
					Charge_MosFET_Con(0);								//���MOS
			}	
	}
	else if((Sys_info.member.Discharge_warning_level >= FAULT_LEVEL) && (Force_Work_Model_Flag == RT_FALSE))
	{
			Sys_info.member.Work_State = STAND_BY;
			
			shutdown_delay++;
			if(shutdown_delay < DELAY_1S)
			{
					Main_Positive_Relay_Con(0);					//���̵���	
			}
			else
			{
					shutdown_delay = DELAY_1S;
					Precharge_MosFET_Con(0);						//Ԥ��MOS				 
					Heater_MosFET_Con(0);								//����MOS
					DisCharge_MosFET_Con(0);						//�ŵ�MOS
					Charge_MosFET_Con(0);								//���MOS
			}	
	}
	else
	{
			if(Sys_info.member.Work_State == STAND_BY) 					//1S����            
			{				
					Standby_delay++;
					if(Standby_delay < DELAY_1S)
					{
							Main_Positive_Relay_Con(0);									//���̵���
					}
					else
					{							
							//��ʱ500ms����Ԥ��ģʽ
							if(Standby_delay > DELAY_1S5)
							{
									Standby_delay = 151;
								
									//TODO���º˶Կ�������
									//�������У��޹��ϻ����������ֱ������
									//�������У��ӻ������������
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
									Precharge_MosFET_Con(0);										//Ԥ��MOS				 
									Heater_MosFET_Con(0);												//����MOS
									DisCharge_MosFET_Con(0);										//�ŵ�MOS------��ָ����ʱ25ms�������Task������������ʹ��
									Charge_MosFET_Con(0);												//���MOS------��ָ����ʱ25ms�������Task������������ʹ��
								
									//������ʱ������0
									shutdown_delay = 0;
									pre_delay = 0;
									Pre_Mos_delay = 0;
							}


					}
			}
			else if(Sys_info.member.Work_State == PRECHARGE)    //Ԥ��ģʽ
			{			
					Standby_delay = 0;
				
					//�ⲿ��ѹ�ﵽ��ص�ѹ90%
					if(Sys_info.member.Pack_Voltage > Sys_info.member.Bat_Voltage * 0.95)
					{
							Sys_info.member.Work_State = READY;
						
							//Ԥ��ok��Ԥ�������0
							pre_count = 0;
					}
				
					//Ԥ��10s����Ԥ��
					if((pre_delay < DELAY_10S) && (Sys_info.member.Precharge_Fail == 0))
					{
							pre_delay++;
							Precharge_MosFET_Con(1);										    //Ԥ��MOS	
							Main_Positive_Relay_Con(1);									    //���̵���
					}
					else//Ԥ��ʧ��
					{
							pre_delay = 0;
						
							//���ж���Ԥ��
							Sys_info.member.Work_State = STAND_BY;
							pre_count++; 

							//����Ԥ��ʧ�ܱ���
						  if(pre_count >= 2)
							{
									Sys_info.member.Precharge_Fail = 1;	
									Force_Work_Model_Flag = 0;								
							}			

							//TODOԤ��ʧ�ܣ������ǰ��Ĺ��ϴ����˴�ֻ�Ͽ��̵������ɣ���Ҫ��֤һ��
							Main_Positive_Relay_Con(0);									    //���̵���				
					}																		
			}
			else if(Sys_info.member.Work_State == READY)  			    //Ԥ��ģʽ���
			{
					 Pre_Mos_delay++;
				
					 if(Pre_Mos_delay < 5)
					 {
							 Main_Positive_Relay_Con(0);										//���̵���					
					 }
					 else if((Pre_Mos_delay > 30) && (Pre_Mos_delay < 60))
					 {
							 DisCharge_MosFET_Con(1);												//�ŵ�MOS
							 Charge_MosFET_Con(1);													//���MOS					 					 
							 Heater_MosFET_Con(0);													//����MOS					 
					 }									
					 else if(Pre_Mos_delay > DELAY_1S)
					 {
							 Pre_Mos_delay = 101;
						   Main_Positive_Relay_Con(1);										//���̵���
							 Precharge_MosFET_Con(0);										    //Ԥ��MOS
						 
							 if(Sys_info.member.Current_Status == DISCHARGE)
							 {
									Sys_info.member.Work_State = DC_DISCHARGE; 							 
							 }

							 //�������״̬Ϊ���������¶ȴ��ڵ���2��
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
					 Heater_MosFET_Con(0);														//����MOS
					 LowTemCount = 0;
				
					 //SOCС��90%�����������־
					 if(Sys_info.member.sys_charge_full_flag == 1)
					 {
							if(Sys_info.member.module_soc < 225)						
							{								
									Sys_info.member.sys_charge_full_flag = 0;
							}														
																			
					 }	
					 
					 //TODO�������״̬Ϊ����Ҵ���1A
					 if((Sys_info.member.Current_Status == CHARGE) && (Sys_info.member.Cur1 > 1000))
					 {
							Sys_info.member.Work_State = DC_CHARGE; 	
					 }											
			}
			else if(Sys_info.member.Work_State == DC_CHARGE) 
			{				
					//TODO�¶��ж���Ҫ����
				  //�޸ļ����߼���ȥ�����ȹ���
					if(Sys_info.member.Cell_Tmp_Min <= 400)				
					{
							//��������С��������5A
							if(LowTemCount <= 4000)									//���³���40s��֮���ڽ������ģʽ
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
					else//һ�����������Ҫ�󣬾����¼���
					{
							LowTemCount = 0;					
					}					
													
					/****************************����߼�******************************/
					if(Sys_info.member.Cell_Tmp_Min >= 420)
					{
							Sys_info.member.Max_Charge_Voltage = 580;
																					  								
							//������ɣ��պ����̵���
							if(Sys_info.member.Heat_Complete_Flag == 1)
							{
									Main_Positive_Relay_Con(1);															//���̵���
								
									//��ʱ1s�Ͽ�����MOS
									if(Heat_Complate_OverCount <= 100)
									{
											Heat_Complate_OverCount++;
									}
									else
									{
											Heater_MosFET_Con(0);																	//����MOS
											Heat_Complate_OverCount = 0;
											Sys_info.member.Heat_Complete_Flag = 0;
									}									
							}																																					
										
								//���Σ���ֹSOC��׼���������־����0������У׼1sִ��һ�Σ��ᱻ����
//							if(Sys_info.member.module_soc < 225)											//����90%��ֹ���
//							{								
//									Sys_info.member.sys_charge_full_flag = 0;
//							}																																		
					}
					
				  if((Sys_info.member.Current_Status == DISCHARGE) && (Sys_info.member.Cur1 > 1000))
					{
							Sys_info.member.Work_State = DC_DISCHARGE; 					
					}			
			}
			else if(Sys_info.member.Work_State == DC_CHARGE_HEAT)  					//����ģʽ 
			{
					//Pack��ѹ����10V���¶Ȳ�������
					if(Sys_info.member.Pack_Voltage >= 100)
					{  
							//��о�¶Ⱥ��¶Ȳ������²�Ϊ2��   ���������¶�Ϊ5��ʱ,�������
							if(Sys_info.member.Cell_Tmp_Min >= 450) 
							{			
									Sys_info.member.Heat_Complete_Flag = 1;//������ɺ���Ҫ���в������Ⱥ����̵�����ȷ������֮���ٶϿ�����mos��������ѭ����������ģʽ�������̵���															
									Sys_info.member.Work_State = DC_CHARGE;							//�����ɺ���ת�����״̬
									LowTemCount = 0;																		//���ȳɹ�����ʧ�ܾ�������¼�ʱ
									heat_time = 0;																			//���ȳɹ��������ʱ���ʱ
							}
							else
							{
									Heater_MosFET_Con(1);																//����MOS
								
								  if(Heat_Time_delay <= 100)													//�պϼ���MOS��1s��ʱ���Ͽ����̵���
								  {
											Heat_Time_delay ++;
								  }
									else
									{
											Main_Positive_Relay_Con(0);											//���̵���
											Heat_Time_delay = 101;
											
											heat_time++;
											//TODO���ǼӼ�ʱ�ж�
										
									}								
							}												
					}
					else	
					{
							Heater_MosFET_Con(0);																		//�Ͽ�����MOS		
							LowTemCount = 0;																				//���ȳɹ�����ʧ�ܾ�������¼�ʱ
							Sys_info.member.Work_State = STAND_BY;	
              heat_time = 0;						
					}	
			}			
	}		
	
	if(Sys_info.member.F_BattFltLev >= 2)
	{		
			Error_time++;
			if(Error_time > 360000)//1��Сʱ
			{
					//TODO���油��
//					Standby_Entering();	//�������ģʽ			
			}
			NoFaultDetectDelay = 0;
	}
	else
	{
			Error_time=0;
			//��ʱ�����80s�������ϼ����ɺ��ټ���Ƿ���� ǿ���־λ
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
        if(Vol_under_time > 360000)//1��Сʱ
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

