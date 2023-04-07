
#include "GlobalVarShared.h"
#include "bms_can.h"

PCS359 pcs359;
PCS351 pcs351;
PCS355 pcs355;
PCS356 pcs356;
PCS35C pcs35C;
PCS35E pcs35E;
PCS356_Aiswei pcs356_Aiswei;

CELL_BUFFER Cell_Buffer;						//���������ϱ�����
PARA_BUFFER Para_Buffer0;						//���մӻ�1����
PARA_BUFFER Para_Buffer1;						//���մӻ�2����
PARA_BUFFER Para_Buffer2;						//���մӻ�3����
COMM_PACK   Comm_Pack;              //���ӻ�����

rt_uint8_t Send_Num = 0;
rt_int16_t  Current_Total;
rt_uint16_t Charge_Current;
rt_uint16_t Current_Min[4];

void Pack_Send_data(void);
void Can1_rece_do(void);
/** ************************************************************************
* @brief   Pack���ݷ��ͺ���
* @brief   ���������ϱ�PCS������Pack�����ϱ���������
 **************************************************************************/
void Pack_Send_data(void)
{
		/** ************************************************************************
		*                         ����ģʽ
		 **************************************************************************/
		if(Switch_Number == 0)
		{								
				pcs355.member.SOC_of_single_module_or_average_value_of_system = (Sys_info.member.module_soc*0.4); //1%
				pcs355.member.SOH_of_single_module_or_average_value_of_system = Sys_info.member.module_soh;			  //1%
																		
				//ǿ���ǣ�ֻʹ��bit5,SOC��<=12ʱ��λ
				if(pcs355.member.SOC_of_single_module_or_average_value_of_system <= 12)
				{
						pcs35C.member.force_charge_flag1 = 1;
				}
				else
				{
						pcs35C.member.force_charge_flag1 = 0;
				}
			
				if((Sys_info.member.sys_charge_full_flag == 0) && (Sys_info.member.Charge_warning_level < 2))
				{
						pcs35C.member.Charge_enable = 1;	
				}
				else
				{
						pcs35C.member.Charge_enable = 0;	
				}
				
				//����7��SOC������95%�����������������������豸δ����Ӧ
				if(Eeprom_Data0.member.Request_Full_Charge == 1)
				{
						pcs35C.member.charge_full_flag = 1;
				}
				else
				{
						pcs35C.member.charge_full_flag = 0;
				}
				
				if((Sys_info.member.DisCharge_Empty_Falg == 0) && (Sys_info.member.Discharge_warning_level < 2))
				{
						pcs35C.member.Discharge_enable = 1;				
				}
				else
				{
						pcs35C.member.Discharge_enable = 0;				
				}

				//pcs351
				pcs351.member.Battery_charge_voltage = Sys_info.member.Max_Charge_Voltage;//58V
				pcs351.member.Charge_current_limit = Cell_Buffer.part.frame2.member.ch_allowed_current;
				pcs351.member.Discharge_current_limit = Cell_Buffer.part.frame2.member.dc_allowed_current;
				
				//����ģʽ���������Ҫ���µ���
				if(Sys_info.member.Work_State == DC_CHARGE_HEAT)
				{
						pcs351.member.Charge_current_limit = 70;
				}
				
				
				//pcs359
				pcs359.member.Module_numbers = 0x01;
				
				pcs356.member.voltage_of_system = (Sys_info.member.Bat_Voltage*10);										//0.01V
				pcs356.member.current_of_system = (Sys_info.member.Cur1/100);				     							//0.1A
				
				//����άƥ��
				pcs356_Aiswei.member.voltage_of_system = pcs356.member.voltage_of_system;
				pcs356_Aiswei.member.current_of_system = (int16_t)pcs356.member.current_of_system;
				
				//�ŵ�ģʽ
				if(Sys_info.member.Work_State == DC_DISCHARGE)
				{
					pcs356_Aiswei.member.current_of_system = -pcs356_Aiswei.member.current_of_system;
				}
		}
		else if(Switch_Number == 1)
		{
				rt_uint8_t SOC_Max;
			  rt_uint8_t SOC_Max1;
				rt_uint8_t SOH_Max;
			  rt_uint8_t SOH_Max1;
				rt_uint8_t SOC_P0;
				rt_uint8_t SOC_P1;
				rt_uint8_t SOC_P2;
				rt_uint8_t SOC_P3;
				rt_uint16_t Current_Max;
				rt_uint16_t Current_Max1;
			
				rt_uint16_t CH_Current_P0;
				rt_uint16_t CH_Current_P1;
				rt_uint16_t CH_Current_P2;
				rt_uint16_t CH_Current_P3;
				rt_uint16_t CH_Current_Max;
				rt_uint16_t CH_Current_Max1;
			
				rt_uint16_t DC_Current_P0;
				rt_uint16_t DC_Current_P1;
				rt_uint16_t DC_Current_P2;
				rt_uint16_t DC_Current_P3;
				rt_uint16_t DC_Current_Max;
				rt_uint16_t DC_Current_Max1;
				/** ************************************************************************
				*                         ����ģʽ������
				 **************************************************************************/
				//�ж�ϵͳ��ŵ�
				Current_Total = Cell_Buffer.part.frame12.member.current_of_system \
					+ Para_Buffer0.part.frame12.member.current_of_system \
					+ Para_Buffer1.part.frame12.member.current_of_system \
					+ Para_Buffer2.part.frame12.member.current_of_system;
			
				pcs356_Aiswei.member.current_of_system = Current_Total;
						
				//ʶ�������豸����硢�ŵ����ֵ��SOC
				SOC_P0 = Sys_info.member.module_soc;
				SOC_P1 = Para_Buffer0.part.frame0.member.soc_of_module;
				SOC_P2 = Para_Buffer1.part.frame0.member.soc_of_module;
				SOC_P3 = Para_Buffer2.part.frame0.member.soc_of_module;
				
				CH_Current_P0 = Cell_Buffer.part.frame2.member.ch_allowed_current;
				CH_Current_P1 = Para_Buffer0.part.frame2.member.ch_allowed_current;
				CH_Current_P2 = Para_Buffer1.part.frame2.member.ch_allowed_current;
				CH_Current_P3 = Para_Buffer2.part.frame2.member.ch_allowed_current;
				
				DC_Current_P0 = Cell_Buffer.part.frame2.member.dc_allowed_current;
				DC_Current_P1 = Para_Buffer0.part.frame2.member.dc_allowed_current;
				DC_Current_P2 = Para_Buffer1.part.frame2.member.dc_allowed_current;
				DC_Current_P3 = Para_Buffer2.part.frame2.member.dc_allowed_current;
				
				if(Master_On == 0)
				{
						//���������ߣ��ŵ������0
						DC_Current_P0 = 0;
					  SOC_P0 = 0;
						CH_Current_P0 = 0;
				}
				
				if(Para_Buffer0.part.frame12.member.Online_State == 0)
				{
						DC_Current_P1 = 0;
						SOC_P1 = 0;
						CH_Current_P1 = 0;
				}

				if(Para_Buffer1.part.frame12.member.Online_State == 0)
				{
						DC_Current_P2 = 0;
						SOC_P2 = 0;
					  CH_Current_P2 = 0;
				}

				if(Para_Buffer2.part.frame12.member.Online_State == 0)
				{
						DC_Current_P3 = 0;
						SOC_P3 = 0;
						CH_Current_P3 = 0;
				}

				//ʶ����������ֵ  
				if(CH_Current_P0 >= CH_Current_P1)
				{
						CH_Current_Max = CH_Current_P0;
				}
				else
				{
						CH_Current_Max = CH_Current_P1;
				}
				
				if(CH_Current_P2 >= CH_Current_P3)
				{
						CH_Current_Max1 = CH_Current_P2;
				}
				else
				{
						CH_Current_Max1 = CH_Current_P3;
				}
				
				if(CH_Current_Max >= CH_Current_Max1)
				{
						Charge_Current = CH_Current_Max;
				}
				else
				{
						Charge_Current = CH_Current_Max1;
				}
				
				//2̨�����豸���ߣ������������ӵ�70A
				if((Charge_Current > 400) && (Online_Num > 2))
				{
						Charge_Current = 700;
				}
				
				if(Charge_Current == 0)
				{
						Charge_Current = Cell_Buffer.part.frame2.member.ch_allowed_current;
				}
				
				pcs351.member.Charge_current_limit = Charge_Current;
				
				//����ģʽ���������Ҫ���µ���
				if((Sys_info.member.Work_State == DC_CHARGE_HEAT) || (Para_Buffer0.part.frame1.member.work_state == DC_CHARGE_HEAT) \
					|| (Para_Buffer1.part.frame1.member.work_state == DC_CHARGE_HEAT) \
					|| (Para_Buffer2.part.frame1.member.work_state == DC_CHARGE_HEAT))
				{
						pcs351.member.Charge_current_limit = (70 * Online_Num);
				}
				
				//�ŵ������ɸѡ���ֵ
				if(DC_Current_P0 >= DC_Current_P1)
				{
						DC_Current_Max = DC_Current_P0;
				}
				else
				{
						DC_Current_Max = DC_Current_P1;
				}
				
				if(DC_Current_P2 >= DC_Current_P3)
				{
						DC_Current_Max1 = DC_Current_P2;
				}
				else
				{
						DC_Current_Max1 = DC_Current_P3;
				}
				
				if(DC_Current_Max >= DC_Current_Max1)
				{
						Current_Max = DC_Current_Max;
				}
				else
				{
						Current_Max = DC_Current_Max1;
				}
				
				if(Current_Max == 0)
				{
						Current_Max = Cell_Buffer.part.frame2.member.dc_allowed_current;
				}
				
				pcs351.member.Discharge_current_limit = Current_Max;
						
				// ����SOH����,ɸѡ���
				if(Sys_info.member.module_soh >= Para_Buffer0.part.frame0.member.soh_of_module)
				{
						SOH_Max = Sys_info.member.module_soh;						
				}
				else
				{
						SOH_Max = Para_Buffer0.part.frame0.member.soh_of_module;
				}		

				if(Para_Buffer1.part.frame0.member.soh_of_module >= Para_Buffer2.part.frame0.member.soh_of_module)
				{
						SOH_Max1 = Para_Buffer1.part.frame0.member.soh_of_module;
				}
				else
				{
						SOH_Max1 = Para_Buffer2.part.frame0.member.soh_of_module;
				}
				
				if(SOH_Max >= SOH_Max1)
				{
						pcs355.member.SOH_of_single_module_or_average_value_of_system = SOH_Max;
				}
				else
				{
						pcs355.member.SOH_of_single_module_or_average_value_of_system = SOH_Max1;				
				}
			
				//SOC��ɸѡ�����ģ��ϱ����޶�4̨
				if(SOC_P0 >= SOC_P1)
				{
						SOC_Max = SOC_P0;
				}
				else
				{
						SOC_Max = SOC_P1;
				}
				
				if(SOC_P2 >= SOC_P3)
				{
						SOC_Max1 = SOC_P2;
				}
				else
				{
						SOC_Max1 = SOC_P3;
				}
				
				if(SOC_Max >= SOC_Max1)
				{
						pcs355.member.SOC_of_single_module_or_average_value_of_system = (SOC_Max*0.4);
				}
				else
				{
						pcs355.member.SOC_of_single_module_or_average_value_of_system = (SOC_Max1*0.4);
				}
				
				//ǿ���ǣ�ֻʹ��bit5,SOC��<=12ʱ��λ
				if(pcs355.member.SOC_of_single_module_or_average_value_of_system <= 12)
				{
						pcs35C.member.force_charge_flag1 = 1;
				}
				else
				{
						pcs35C.member.force_charge_flag1 = 0;
				}
				
				//����7��SOC������95%�����������������������豸δ����Ӧ
				//����δ������
				
				
				//��ŵ�ʹ���ж�
				//��ֻʣ���1̨���ߵ�ʱ���ж��Ƿ�����
				if(Online_Num == 1)
				{
						//��������
						if(Master_On == 1)
						{
								if(Sys_info.member.sys_charge_full_flag == 1)
								{
										pcs35C.member.Charge_enable = 0;
								}
								else
								{
										pcs35C.member.Charge_enable = 1;						
								}
								
								//�ſ�
								if(Sys_info.member.DisCharge_Empty_Falg == 1)
								{
										pcs35C.member.Discharge_enable = 0;			
								}
								else
								{
										pcs35C.member.Discharge_enable = 1;			
								}	
						}
						else if(Para_Buffer0.part.frame12.member.Online_State == 1)
						{
								//1�Ŵӻ�����
								if(Para_Buffer0.part.frame1.member.charge_full_flag == 1)
								{
										pcs35C.member.Charge_enable = 0;
								}
								else
								{
										pcs35C.member.Charge_enable = 1;						
								}
								
								//�ſ�
								if(Para_Buffer0.part.frame12.member.DisChargr_Empty_Flag == 1)
								{
										pcs35C.member.Discharge_enable = 0;			
								}
								else
								{
										pcs35C.member.Discharge_enable = 1;			
								}
						}
						else if(Para_Buffer1.part.frame12.member.Online_State == 1)
						{
								//2�Ŵӻ�����
								if(Para_Buffer1.part.frame1.member.charge_full_flag == 1)
								{
										pcs35C.member.Charge_enable = 0;
								}
								else
								{
										pcs35C.member.Charge_enable = 1;						
								}
								
								//�ſ�
								if(Para_Buffer1.part.frame12.member.DisChargr_Empty_Flag == 1)
								{
										pcs35C.member.Discharge_enable = 0;			
								}
								else
								{
										pcs35C.member.Discharge_enable = 1;			
								}								
						}						
						else if(Para_Buffer2.part.frame12.member.Online_State == 1)
						{
								//2�Ŵӻ�����
								if(Para_Buffer2.part.frame1.member.charge_full_flag == 1)
								{
										pcs35C.member.Charge_enable = 0;
								}
								else
								{
										pcs35C.member.Charge_enable = 1;							
								}
								
								//�ſ�
								if(Para_Buffer2.part.frame12.member.DisChargr_Empty_Flag == 1)
								{
										pcs35C.member.Discharge_enable = 0;			
								}
								else
								{
										pcs35C.member.Discharge_enable = 1;			
								}								
						}							
						else
						{}
				}	
				else
				{
						//��̨���ߣ��ɳ�硢�ŵ�
						pcs35C.member.Charge_enable = 1;
					  pcs35C.member.Discharge_enable = 1;		
				}
					
					
				//��������ȷ��
				//��ŵ�ʹ�ܣ����ǹ��ϻ�Ͽ��̵�����ȥ�������ж�(Sys_info.member.Charge_warning_level < 2)
				//���ݲ���̨���жϳ�ŵ�ʹ��
				if(Para_Buffer2.part.frame12.member.Para_Num == 4)
				{
						pcs359.member.Module_numbers = 4;																			
				}
				else if(Para_Buffer1.part.frame12.member.Para_Num == 3)
				{
					  pcs359.member.Module_numbers = 3;
				}
				else if(Para_Buffer0.part.frame12.member.Para_Num == 2)
				{
				    pcs359.member.Module_numbers = 2;						
						//�ϱ�������
						
				}
				else
				{}
				
				//pcs351����ѹҪ���Ǽ���ģʽ
				pcs351.member.Battery_charge_voltage = Sys_info.member.Max_Charge_Voltage;          //58V			
				pcs356_Aiswei.member.voltage_of_system = Sys_info.member.Bat_Voltage*10;										//0.01V
				//����356
		}
		else
		{
				//�������еĴӻ�

				
		}
		
		//��Ҫʵ��һ�¸��¶�,����Ĭ���ϱ������¶�
		//pcs356

		if(Sys_info.member.Cell_Tmp_Max >= 400)
		{
				pcs356_Aiswei.member.Average_cell_temperature = Sys_info.member.Cell_Tmp_Max - 400;//0.1��
		}
		else
		{
				pcs356_Aiswei.member.Average_cell_temperature = 400 - Sys_info.member.Cell_Tmp_Max;
				pcs356_Aiswei.member.Average_cell_temperature = pcs356_Aiswei.member.Average_cell_temperature|0x8000;  		
		}

		
		//����Ĵ�������
		//ÿ7��ǿ������һ��168Сʱ*3600��=604800��
//				if(Eeprom_Data0.member.Charge_Full_Enable==1)
//				{
//					pcs35C.member.force_charge_flag1=1;
//					pcs35C.member.force_charge_flag2=1;
//				}		
		
		
		//pcs359
		pcs359.member.P = 0x50;
		pcs359.member.N = 0x4E;
				
		//pcs35E��ANGILE
		pcs35E.member.Producer1=0x41;
		pcs35E.member.Producer2=0x4E;
		pcs35E.member.Producer3=0x47;
		pcs35E.member.Producer4=0x49;
		pcs35E.member.Producer5=0x4C;
		pcs35E.member.Producer6=0x45;
		pcs35E.member.Producer7=0x00;
		pcs35E.member.Producer8=0x00;
		

			
/** ************************************************************************
*                         ����Pack�ϱ�����
 **************************************************************************/
		Cell_Buffer.part.frame0.member.soc_of_module = Sys_info.member.module_soc;
		Cell_Buffer.part.frame0.member.soh_of_module = (rt_uint8_t)Sys_info.member.module_soh;					//ģ��SOH����λ0.4%		
		Cell_Buffer.part.frame0.member.max_vol_of_cell = Sys_info.member.Cell_Volt_Max; 								//��ص����ѹ���ֵ����λ0.001��
		Cell_Buffer.part.frame0.member.min_vol_of_cell = Sys_info.member.Cell_Volt_Min;            			//��ص����ѹ��Сֵ����λ0.001��
		Cell_Buffer.part.frame0.member.max_vol_of_cellnumber = Sys_info.member.Cell_Volt_Max_num;       //������ѹ���	
		Cell_Buffer.part.frame0.member.min_vol_of_cellnumber = Sys_info.member.Cell_Volt_Min_num;       //��С�����ѹ���
	
	  Cell_Buffer.part.frame1.member.max_temp_of_cell = Sys_info.member.Cell_Tmp_Max;            			//����¶����ֵ����λ0.1�ȣ�ƫ��-40				
		Cell_Buffer.part.frame1.member.min_temp_of_cell = Sys_info.member.Cell_Tmp_Min;            			//����¶���Сֵ����λ0.1�ȣ�ƫ��-40						
		Cell_Buffer.part.frame1.member.max_temp_of_sensenumber = Sys_info.member.Cell_Tmp_Max_num;      //����¶Ȳ�������		
		Cell_Buffer.part.frame1.member.min_temp_of_sensenumber = Sys_info.member.Cell_Tmp_Min_num;      //��С�¶Ȳ�������			
		Cell_Buffer.part.frame1.member.work_state = Sys_info.member.Work_State;                         //����״̬				
		Cell_Buffer.part.frame1.member.charge_full_flag = Sys_info.member.sys_charge_full_flag;         //����״̬
	
		Cell_Buffer.part.frame2.member.balance_bit = Sys_info.member.Balance_BIT;                 		  //16������λ          
		Cell_Buffer.part.frame2.member.dc_allowed_current = (Sys_info.member.DC_Allowed_Current * 10);  //�ŵ��������   *10????
		Cell_Buffer.part.frame2.member.ch_allowed_current = (Sys_info.member.CH_Allowed_Current * 10);  //0.1A ����������          			
		Cell_Buffer.part.frame2.member.f_battfltlev = Sys_info.member.F_BattFltLev;    					        //���ϸ澯�ȼ�
								
		Cell_Buffer.part.frame3.member.Module_voltage_over_warning_level = Sys_info.member.Module_voltage_over_warning_level;  
		Cell_Buffer.part.frame3.member.Module_voltage_under_warning_level = Sys_info.member.Module_voltage_under_warning_level;
		Cell_Buffer.part.frame3.member.Cell_vol_over_warning_level = Sys_info.member.Cell_vol_over_warning_level;     	
		Cell_Buffer.part.frame3.member.Cell_vol_under_warning_level = Sys_info.member.Cell_vol_under_warning_level;
		Cell_Buffer.part.frame3.member.Cell_vol_differ_warning_level = Sys_info.member.Cell_vol_differ_warning_level;
		Cell_Buffer.part.frame3.member.Charge_current_over_warning_level = Sys_info.member.Charge_current_over_warning_level;    
		Cell_Buffer.part.frame3.member.Discharge_current_over_warning_level = Sys_info.member.Discharge_current_over_warning_level;		
		Cell_Buffer.part.frame3.member.Charge_temp_over_warning_level = Sys_info.member.Charge_temp_over_warning_level;
								
		Cell_Buffer.part.frame4.member.Charge_temp_under_warning_level = Sys_info.member.Charge_temp_under_warning_level;		
		Cell_Buffer.part.frame4.member.Discharge_temp_over_warning_level = Sys_info.member.Discharge_temp_over_warning_level;																				
		Cell_Buffer.part.frame4.member.Discharge_temp_under_warning_level = Sys_info.member.Discharge_temp_under_warning_level;  
		Cell_Buffer.part.frame4.member.Temp_differ_warning_level = Sys_info.member.Temp_differ_warning_level;     
		Cell_Buffer.part.frame4.member.Mos_temp_over_warning_level = Sys_info.member.Mos_temp_over_warning_level; 
		Cell_Buffer.part.frame4.member.Afe_commu_fail_warning_level = Sys_info.member.Afe_commu_fail_warning_level; 
		
		Cell_Buffer.part.frame5.member.vol_of_cell1 = Sys_info.member.Cell_volt[0];             //ģ�鵥��1��ѹ����λ0.001��			
		Cell_Buffer.part.frame5.member.vol_of_cell2 = Sys_info.member.Cell_volt[1];                  				
		Cell_Buffer.part.frame5.member.vol_of_cell3 = Sys_info.member.Cell_volt[2];                  		
		Cell_Buffer.part.frame5.member.vol_of_cell4 = Sys_info.member.Cell_volt[3];  

		Cell_Buffer.part.frame6.member.vol_of_cell5 = Sys_info.member.Cell_volt[4];             //ģ�鵥��1��ѹ����λ0.001��												 
		Cell_Buffer.part.frame6.member.vol_of_cell6 = Sys_info.member.Cell_volt[5];                   															
		Cell_Buffer.part.frame6.member.vol_of_cell7 = Sys_info.member.Cell_volt[6];                  												 
		Cell_Buffer.part.frame6.member.vol_of_cell8 = Sys_info.member.Cell_volt[7];    
		
		Cell_Buffer.part.frame7.member.vol_of_cell9 = Sys_info.member.Cell_volt[8];             //ģ�鵥��1��ѹ����λ0.001��			
		Cell_Buffer.part.frame7.member.vol_of_cell10 = Sys_info.member.Cell_volt[9];                  				
		Cell_Buffer.part.frame7.member.vol_of_cell11 = Sys_info.member.Cell_volt[10];                  			
		Cell_Buffer.part.frame7.member.vol_of_cell12 = Sys_info.member.Cell_volt[11];  

		Cell_Buffer.part.frame8.member.vol_of_cell13 = Sys_info.member.Cell_volt[12];             //ģ�鵥��1��ѹ����λ0.001��												 
		Cell_Buffer.part.frame8.member.vol_of_cell14 = Sys_info.member.Cell_volt[13];                   															
		Cell_Buffer.part.frame8.member.vol_of_cell15 = Sys_info.member.Cell_volt[14];                  													 
		Cell_Buffer.part.frame8.member.vol_of_cell16 = Sys_info.member.Cell_volt[15];  
	 
		Cell_Buffer.part.frame9.member.temp_of_sense1 = Sys_info.member.Cell_Temp[0];														
		Cell_Buffer.part.frame9.member.temp_of_sense2 = Sys_info.member.Cell_Temp[1];                 														
		Cell_Buffer.part.frame9.member.temp_of_sense3 = Sys_info.member.Cell_Temp[2];              													
		Cell_Buffer.part.frame9.member.temp_of_sense4 = Sys_info.member.Cell_Temp[3];  
			
		Cell_Buffer.part.frame10.member.temp_of_sense5 = Sys_info.member.Cell_Temp[4];													
		Cell_Buffer.part.frame10.member.temp_of_sense6 = Sys_info.member.Cell_Temp[5];                 														
		Cell_Buffer.part.frame10.member.temp_of_sense7 = Sys_info.member.Cell_Temp[6];              													
		Cell_Buffer.part.frame10.member.temp_of_sense8 = Sys_info.member.Cell_Temp[7]; 
		
		Cell_Buffer.part.frame11.member.current_of_module = Sys_info.member.Cur1/1000;
		Cell_Buffer.part.frame11.member.Work_Model = Eeprom_Data0.member.Work_Model;
//		Cell_Buffer.part.frame11.member.Force_Work = Force_Work_Model_Flag;		
		

		Cell_Buffer.part.frame12.member.voltage_of_system = Sys_info.member.Bat_Voltage*10;				  //0.01V
		Cell_Buffer.part.frame12.member.current_of_system = (int16_t)(Sys_info.member.Cur1/100);				//0.1A
		Cell_Buffer.part.frame12.member.DisChargr_Empty_Flag = Sys_info.member.DisCharge_Empty_Falg;
		Cell_Buffer.part.frame12.member.Para_Num = Switch_Number;
	
		//�ŵ�ģʽ
		if(Sys_info.member.Work_State == DC_DISCHARGE)
		{
				Cell_Buffer.part.frame12.member.current_of_system = -Cell_Buffer.part.frame12.member.current_of_system;
		}

								
		//�ϱ�����汾
		Cell_Buffer.part.frame14.member.Soft_Version = SOFT_VERSION;
		
		//�ϱ�����״̬
		if(Sys_info.member.Work_State == STAND_BY)
		{
				Cell_Buffer.part.frame12.member.Online_State = 0;
		}
		else
		{
				Cell_Buffer.part.frame12.member.Online_State = 1;
		}
		
/** ************************************************************************
*                         ������ѯ�ϱ�
 **************************************************************************/
		switch(Send_Num)
		{
			case 0:
					if(Switch_Number <= 1)
					{
							CAN_Send_data(0x359,8,&pcs359.buf[0]);
						  Send_Num = 1;
					}
					else
					{
							Send_Num = 6;
					}				
				break;
			case 1:
					CAN_Send_data(0x351,8,&pcs351.buf[0]);
					
					Send_Num = 2;
				break;
			case 2:
					CAN_Send_data(0x355,8,&pcs355.buf[0]);
					
					Send_Num = 3;			
				break;
			case 3:
					CAN_Send_data(0x356,8,&pcs356_Aiswei.buf[0]);	
					
					Send_Num = 4;		
				break;
			case 4:
					CAN_Send_data(0x35C,8,&pcs35C.buf[0]);
				  
					Send_Num = 5;	
				 break;
			case 5:
					CAN_Send_data(0x35E,8,&pcs35E.buf[0]);
						
					Send_Num = 6;	
				 break;
			case 6:
					CAN_Send_data(0x110+(Switch_Number<<4),8,&Cell_Buffer.part.frame0.buf[0]);
					Send_Num = 7;	
				 break;
			case 7:
					CAN_Send_data(0x111+(Switch_Number<<4),8,&Cell_Buffer.part.frame1.buf[0]);
					Send_Num = 8;	
				 break;		
			case 8:
					CAN_Send_data(0x112+(Switch_Number<<4),8,&Cell_Buffer.part.frame2.buf[0]);
					Send_Num = 9;	
				 break;	
			case 9:
					CAN_Send_data(0x113+(Switch_Number<<4),8,&Cell_Buffer.part.frame3.buf[0]);
					Send_Num = 10;	
				 break;			
			case 10:
          CAN_Send_data(0x114+(Switch_Number<<4),8,&Cell_Buffer.part.frame4.buf[0]);			
					Send_Num = 11;	
				 break;			
			case 11:
					CAN_Send_data(0x115+(Switch_Number<<4),8,&Cell_Buffer.part.frame5.buf[0]);
					Send_Num = 12;	
				 break;	
			case 12:
					CAN_Send_data(0x116+(Switch_Number<<4),8,&Cell_Buffer.part.frame6.buf[0]);
					Send_Num = 13;	
				 break;		
			case 13:
					CAN_Send_data(0x117+(Switch_Number<<4),8,&Cell_Buffer.part.frame7.buf[0]);
					Send_Num = 14;	
				 break;	
			case 14:
					CAN_Send_data(0x118+(Switch_Number<<4),8,&Cell_Buffer.part.frame8.buf[0]);
					Send_Num = 15;	
				 break;				
			case 15:
				  CAN_Send_data(0x119+(Switch_Number<<4),8,&Cell_Buffer.part.frame9.buf[0]);				
					Send_Num = 16;	
				 break;	
			case 16:
					CAN_Send_data(0x11A+(Switch_Number<<4),8,&Cell_Buffer.part.frame10.buf[0]);
					Send_Num = 17;	
				 break;		
			case 17:
					CAN_Send_data(0x11B+(Switch_Number<<4),8,&Cell_Buffer.part.frame11.buf[0]);
					Send_Num = 18;	
				 break;		
			case 18:
					CAN_Send_data(0x11C+(Switch_Number<<4),8,&Cell_Buffer.part.frame12.buf[0]);
					Send_Num = 19;	
				 break;	
			case 19:
//					CAN_Send_data(0x11D+(Switch_Number<<4),8,&Cell_Buffer.part.frame13.buf[0]);
					Send_Num = 20;	
				 break;
			case 20:
			    CAN_Send_data(0x11E+(Switch_Number<<4),8,&Cell_Buffer.part.frame14.buf[0]);
					Send_Num = 21;	
				 break;	
			case 21:
					//���ӻ���������
//					CAN_Send_data(0x11E+(Switch_Number<<4),8,&Cell_Buffer.part.frame14.buf[0]);
					Send_Num = 22;	
				 break;	
			case 22:
					if(Switch_Number == 1)
					{
					    CAN_Send_data(0x601,8,&Comm_Pack.buf[0]);				
					}
				  Send_Num = 0;
			  break;
			default:
				break;
		}
																	
}



/*
************************************************************************************************************************
*     ���ݽ��ս���
************************************************************************************************************************
*/
void Can1_rece_do(void)
{
		CAN_RxHeaderTypeDef rx_header;
	  rt_uint8_t i;
		rt_uint8_t rx_data[8];
	
		HAL_CAN_GetRxMessage(&hcan,CAN_RX_FIFO0,&rx_header,rx_data);

		switch(rx_header.StdId)
		{
			case 0x305:
					//���PCSͨ�Ź���
					Sys_info.member.Pcs_commu_fail_warning_level = 0;	
					PCS_Communication_Count = 0;	
				break;
			case 0x130:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[0] = 0;
				
					for(i=0;i<8;i++)
					{
							Para_Buffer0.part.frame0.buf[i] = rx_data[i];
					}
			  break;
			case 0x131:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[0] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer0.part.frame1.buf[i] = rx_data[i];
					}
			  break;
			case 0x132:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[0] = 0;				
					for(i=0;i<8;i++)
					{
							Para_Buffer0.part.frame2.buf[i] = rx_data[i];
					}
			  break;
			case 0x13C:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[0] = 0;				
					for(i=0;i<8;i++)
					{
							Para_Buffer0.part.frame12.buf[i] = rx_data[i];
					}
			  break;			
			case 0x140:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[1] = 0;
				
					for(i=0;i<8;i++)
					{
							Para_Buffer1.part.frame0.buf[i] = rx_data[i];
					}
			  break;
			case 0x141:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[1] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer1.part.frame1.buf[i] = rx_data[i];
					}
			  break;
			case 0x142:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[1] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer1.part.frame2.buf[i] = rx_data[i];
					}
			  break;
			case 0x14C:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[1] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer1.part.frame12.buf[i] = rx_data[i];
					}
			  break;		
			case 0x150:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[2] = 0;
				
					for(i=0;i<8;i++)
					{
							Para_Buffer2.part.frame0.buf[i] = rx_data[i];
					}
			  break;
			case 0x151:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[2] = 0;
			
					for(i=0;i<8;i++)
					{
							Para_Buffer2.part.frame1.buf[i] = rx_data[i];
					}
			  break;
			case 0x152:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[2] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer2.part.frame2.buf[i] = rx_data[i];
					}
			  break;
			case 0x15C:
					//���ͨ���쳣�жϼ���
					SLAVE_Communication[2] = 0;
					for(i=0;i<8;i++)
					{
							Para_Buffer2.part.frame12.buf[i] = rx_data[i];
					}
			  break;		
			case 0x601:
					//�ӻ����յ�����ָ��
					BCU_Communication_Count = 0;
					Sys_info.member.Bcu_commu_fail_warning_level = 0;
			
					for(i=0;i<8;i++)
					{
							Comm_Pack.buf[i] = rx_data[i];
					}
			  break;		
			case 0x401:
					//������λָ��			
					if((rx_data[4] == 0x53) && (rx_data[5] == 0x4C))
					{
							Sys_info.member.Update_Flag = 1;
//							__set_PRIMASK(1);
//							HAL_NVIC_SystemReset();
					}
			  break;					
			default:
				break;
		}
}


