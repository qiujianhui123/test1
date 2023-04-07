#include "GlobalVarShared.h"

//保护告警阈值
rt_uint16_t   Module_voltage_over_time[2][2];
rt_uint16_t   Module_voltage_under_time[2][2];
rt_uint16_t   Discharge_current_over_time[2][2];
rt_uint16_t   Discharge_current_over_time1[2][2];
rt_uint16_t   Charge_current_over_time[2][2]; 
rt_uint16_t   Cell_vol_over_time[2][2];
rt_uint16_t   Cell_vol_under_time[2][2];
rt_uint16_t   Cell_vol_differ_time[2][2]; 
rt_uint16_t   Charge_temp_under_time[2][2]; 
rt_uint16_t   Discharge_temp_under_time[2][2];
rt_uint16_t   Charge_Temp_over_time[2][2];
rt_uint16_t   Discharge_Temp_over_time[2][2];
rt_uint16_t   Temp_differ_time[2][2];  
rt_uint16_t   Mos_temp_over_time[2][2];

//充电低温时间判断
rt_uint16_t   Charge_LowTem_Count;																		

//电芯温差判断
rt_uint16_t		Find_TemCell_Error;
rt_uint16_t		Find_TemCell_Error1;
rt_uint16_t		Find_TemCell_Error2;
rt_uint16_t		Find_TemCell_Min1;
rt_uint16_t		Find_TemCell_Min2;
rt_uint16_t		Find_TemCell_Max1;
rt_uint16_t		Find_TemCell_Max2;
 
//通信超时判断
rt_uint8_t    PCS_Communication_Count;
rt_uint8_t    BCU_Communication_Count;
rt_uint8_t    AFE_Communication_Count;
rt_uint8_t    Switch_Number;
rt_uint8_t    SLAVE_Communication[5];
rt_uint8_t    SLAVE_Online[5];


static rt_uint8_t Module_voltage_over_warning0_detect(void);
static rt_uint8_t Module_voltage_over_warning1_detect(void);

static rt_uint8_t Module_voltage_under_warning0_detect(void);
static rt_uint8_t Module_voltage_under_warning1_detect(void);

static rt_uint8_t Discharge_current_over_warning0_detect(void);
static rt_uint8_t Discharge_current_over_warning1_detect(void);

static rt_uint8_t Discharge_current_over_warning0_detect1(void);
static rt_uint8_t Discharge_current_over_warning1_detect1(void);

static rt_uint8_t Charge_current_over_warning0_detect(void);
static rt_uint8_t Charge_current_over_warning1_detect(void);

static rt_uint8_t Cell_vol_over_warning0_detect(void);
static rt_uint8_t Cell_vol_over_warning1_detect(void);

static rt_uint8_t Cell_vol_under_warning0_detect(void);
static rt_uint8_t Cell_vol_under_warning1_detect(void);

static rt_uint8_t Cell_vol_differ_warning0_detect(void);
static rt_uint8_t Cell_vol_differ_warning1_detect(void);
                                                  
static rt_uint8_t Charge_temp_under_warning0_detect(void);
static rt_uint8_t Charge_temp_under_warning1_detect(void);

static rt_uint8_t Discharge_temp_under_warning0_detect(void);
static rt_uint8_t Discharge_temp_under_warning1_detect(void);

static rt_uint8_t Charge_temp_over_warning0_detect(void);
static rt_uint8_t Charge_temp_over_warning1_detect(void);

static rt_uint8_t Discharge_temp_over_warning0_detect(void);
static rt_uint8_t Discharge_temp_over_warning1_detect(void);

static rt_uint8_t Temp_differ_warning0_detect(void);
static rt_uint8_t Temp_differ_warning1_detect(void);

static rt_uint8_t Mos_temp_over_warning0_detect(void);
static rt_uint8_t Mos_temp_over_warning1_detect(void);

static rt_uint8_t Is_Charge_OK(void); 
static rt_uint8_t Is_Disharge_OK(void);

static rt_uint8_t Found_most_level(rt_uint8_t * data);

void Warning_time_init(void);
void Sys_Warning_Process(void);

//告警计时初始化
void Warning_time_init(void)
{
		rt_uint8_t j,i;
		
		for(j=0;j<2;j++)
		{
				for(i=0;i<2;i++)
				{
						Module_voltage_over_time[j][i] 		= 0;
						Module_voltage_under_time[j][i] 	= 0;
						Discharge_current_over_time[j][i] = 0;
						Charge_current_over_time[j][i] 		= 0; 
						Cell_vol_over_time[j][i] 					= 0;
						Cell_vol_under_time[j][i] 				= 0;
						Cell_vol_differ_time[j][i] 				= 0; 
						Charge_temp_under_time[j][i] 			= 0; 
						Discharge_temp_under_time[j][i] 	= 0;
						Discharge_Temp_over_time[j][i] 		= 0;
						Charge_Temp_over_time[j][i] 			= 0;
						Temp_differ_time[j][i] 						= 0;          
						Mos_temp_over_time[j][i] 					= 0;														
				}
		}   
		
		for(i=0;i<2;i++)
		{
						Sys_info.member.Module_voltage_over_warning[i]			=	1;
						Sys_info.member.Module_voltage_under_warning[i]			=	1;
						Sys_info.member.Discharge_current_over_warning[i]		=	1;
						Sys_info.member.Discharge_current_over_warning1[i]	=	1;
						Sys_info.member.Charge_current_over_warning[i]			=	1; 
						Sys_info.member.Cell_vol_over_warning[i]						=	1;
						Sys_info.member.Cell_vol_under_warning[i]						=	1;
						Sys_info.member.Cell_vol_differ_warning[i]					=	1; 
						Sys_info.member.Charge_temp_under_warning[i]				=	1; 
						Sys_info.member.Discharge_temp_under_warning[i]			=	1;
						Sys_info.member.Discharge_temp_over_warning[i]			=	1;
						Sys_info.member.Charge_temp_over_warning[i]					=	1;
						Sys_info.member.Temp_differ_warning[i]							=	1;          
						Sys_info.member.Mos_temp_over_warning[i]						=	1;  										
		}
          
}
 
           


/*handle_debouncing(故障发生标志位【1-未发生，初始化为故障未发生 0-发生】,
                                                当前时间故障触发判定函数, 故障发生时间累计,
                                                当前时间故障恢复判定函数, 故障恢复时间累计, 
                                                故障发生时间限值*10, 故障恢复时间限值*10)*/

static void handle_debouncing( rt_uint8_t *flag,
                                        rt_uint8_t (*false_condition_function_ptr)(void), rt_uint16_t *debounce_false_counter,
                                        rt_uint8_t (*true_condition_function_ptr)(void), rt_uint16_t *debounce_true_counter, 
                                        rt_uint16_t false_debounce_limit, rt_uint16_t true_debounce_limit)
{
		if(*flag == RT_TRUE )      //如果标识位是1  为真  故障未发生
		{
					if (debounce_false_counter)                     //如果指针是有效的
					{        
							if (false_condition_function_ptr() == RT_TRUE)
							{
									*debounce_false_counter = 0;
							} 
							else
							{
									(*debounce_false_counter)++;

									if (*debounce_false_counter >= false_debounce_limit)
									{
											*flag = RT_FALSE;

											if (debounce_true_counter)     //如果指针是有效的
											{                    
													*debounce_true_counter = 0;
											}
									}
							}
					}
		}
		else if(*flag == RT_FALSE )      //如果标识位是0 为假
		{
					if (debounce_true_counter)                                  //如果指针是有效的
					{        
							if ( true_condition_function_ptr() == RT_TRUE)
							{
									(*debounce_true_counter)++;

									if ( *debounce_true_counter >= true_debounce_limit)
									{
											*flag = RT_TRUE;

											if (debounce_false_counter)            //如果指针是有效的
											{
														*debounce_false_counter = 0;
											}                    
									}
							} 
							else
							{
										*debounce_true_counter = 0;
							}
					}
		}
}

void Sys_Warning_Process(void)
{	
		Sys_info.member.Switch_Number = Switch_Number;
	
//		WarnningCounter++;
		if(system_vol_cal)
		{
				//---------------------------------------------------------------------系统电压过压保护
				handle_debouncing( &Sys_info.member.Module_voltage_over_warning[0],//保护
																				Module_voltage_over_warning0_detect, &Module_voltage_over_time[0][0],
																				Module_voltage_over_warning0_detect, &Module_voltage_over_time[0][1], 
																				Eeprom_Alarm_Fault_Time.member.Module_voltage_over_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Module_voltage_over_time_r[0]*10);
				handle_debouncing( &Sys_info.member.Module_voltage_over_warning[1],//告警
																				Module_voltage_over_warning1_detect, &Module_voltage_over_time[1][0],
																				Module_voltage_over_warning1_detect, &Module_voltage_over_time[1][1], 
																				Eeprom_Alarm_Fault_Time.member.Module_voltage_over_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Module_voltage_over_time_r[1]*10);                                       
			 
				//---------------------------------------------------------------------系统电压欠压保护
				handle_debouncing( &Sys_info.member.Module_voltage_under_warning[0],//保护
																				Module_voltage_under_warning0_detect, &Module_voltage_under_time[0][0],
																				Module_voltage_under_warning0_detect, &Module_voltage_under_time[0][1], 
																				Eeprom_Alarm_Fault_Time.member.Module_voltage_under_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Module_voltage_under_time_r[0]*10);
				handle_debouncing( &Sys_info.member.Module_voltage_under_warning[1],//告警
																				Module_voltage_under_warning1_detect, &Module_voltage_under_time[1][0],
																				Module_voltage_under_warning1_detect, &Module_voltage_under_time[1][1], 
																				Eeprom_Alarm_Fault_Time.member.Module_voltage_under_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Module_voltage_under_time_r[1]*10); 					 
		}
        
		if(current_cal)
		{
					 //---------------------------------------------------------------------放电过流0 
						handle_debouncing( &Sys_info.member.Discharge_current_over_warning[0],
																						Discharge_current_over_warning0_detect, &Discharge_current_over_time[0][0],
																						Discharge_current_over_warning0_detect, &Discharge_current_over_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r[0]*10);
						handle_debouncing( &Sys_info.member.Discharge_current_over_warning[1],
																						Discharge_current_over_warning1_detect, &Discharge_current_over_time[1][0],
																						Discharge_current_over_warning1_detect, &Discharge_current_over_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r[1]*10);
						//---------------------------------------------------------------------放电过流1
						handle_debouncing( &Sys_info.member.Discharge_current_over_warning1[0],
																						Discharge_current_over_warning0_detect1, &Discharge_current_over_time1[0][0],
																						Discharge_current_over_warning0_detect1, &Discharge_current_over_time1[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time1[0]*10, Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r1[0]*10);
						handle_debouncing( &Sys_info.member.Discharge_current_over_warning1[1],
																						Discharge_current_over_warning1_detect1, &Discharge_current_over_time1[1][0],
																						Discharge_current_over_warning1_detect1, &Discharge_current_over_time1[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_current_over_time1[1]*10, Eeprom_Alarm_Fault_Time_r.member.Discharge_current_over_time_r1[1]*10);
						
						//---------------------------------------------------------------------充电过流
						handle_debouncing( &Sys_info.member.Charge_current_over_warning[0],
																						Charge_current_over_warning0_detect, &Charge_current_over_time[0][0],
																						Charge_current_over_warning0_detect, &Charge_current_over_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_current_over_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Charge_current_over_time_r[0]*10);
						handle_debouncing( &Sys_info.member.Charge_current_over_warning[1],
																						Charge_current_over_warning1_detect, &Charge_current_over_time[1][0],
																						Charge_current_over_warning1_detect, &Charge_current_over_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_current_over_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Charge_current_over_time_r[1]*10);
					 
		}
		
		//---------------------------------------------------------------------单体过压
		if(max_vol_cal)
		{
						
		handle_debouncing( &Sys_info.member.Cell_vol_over_warning[0],
																		Cell_vol_over_warning0_detect, &Cell_vol_over_time[0][0],
																		Cell_vol_over_warning0_detect, &Cell_vol_over_time[0][1], 
																		Eeprom_Alarm_Fault_Time.member.Cell_vol_over_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_over_time_r[0]*10);
		handle_debouncing( &Sys_info.member.Cell_vol_over_warning[1],
																		Cell_vol_over_warning1_detect, &Cell_vol_over_time[1][0],
																		Cell_vol_over_warning1_detect, &Cell_vol_over_time[1][1], 
																		Eeprom_Alarm_Fault_Time.member.Cell_vol_over_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_over_time_r[1]*10);
		
		}
		
		//---------------------------------------------------------------------单体欠压
		if(min_vol_cal)
		{ 
		
					/*  if(Sys_info.member.Cell_Tmp_Min<450) 
						{
										Eeprom_Alarm_Fault_Time.member.Cell_vol_under[0]=2500;                  //放电欠压，单位0.001V
										Eeprom_Alarm_Fault_Time.member.Cell_vol_under[1]=2600;                  //放电欠压，单位0.001V
										Eeprom_Alarm_Fault_Time.member.Cell_vol_under[2]=2700;                  //放电欠压，单位0.001V
										Eeprom_Alarm_Fault_Time.member.Cell_vol_under[3]=2800;                  //放电欠压，单位0.001V    
													
						}  */
		
						handle_debouncing( &Sys_info.member.Cell_vol_under_warning[0],
																						Cell_vol_under_warning0_detect, &Cell_vol_under_time[0][0],
																						Cell_vol_under_warning0_detect, &Cell_vol_under_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Cell_vol_under_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_under_time_r[0]*10);
						handle_debouncing( &Sys_info.member.Cell_vol_under_warning[1],
																						Cell_vol_under_warning1_detect, &Cell_vol_under_time[1][0],
																						Cell_vol_under_warning1_detect, &Cell_vol_under_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Cell_vol_under_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_under_time_r[1]*10);     
					 
		}
		
		//---------------------------------------------------------------------单体压差
		if((max_vol_cal)&&(min_vol_cal))
		{
						handle_debouncing( &Sys_info.member.Cell_vol_differ_warning[0],
																						Cell_vol_differ_warning0_detect, &Cell_vol_differ_time[0][0],
																						Cell_vol_differ_warning0_detect, &Cell_vol_differ_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Cell_vol_differ_time[0]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_differ_time_r[0]*10);
						handle_debouncing( &Sys_info.member.Cell_vol_differ_warning[1],
																						Cell_vol_differ_warning1_detect, &Cell_vol_differ_time[1][0],
																						Cell_vol_differ_warning1_detect, &Cell_vol_differ_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Cell_vol_differ_time[1]*10, Eeprom_Alarm_Fault_Time_r.member.Cell_vol_differ_time_r[1]*10);
					 
		}
		
		//---------------------------------------------------------------------充电温度过低
		if(min_temp_cal)
		{
						
						handle_debouncing( &Sys_info.member.Charge_temp_under_warning[0],
																						Charge_temp_under_warning0_detect, &Charge_temp_under_time[0][0],
																						Charge_temp_under_warning0_detect, &Charge_temp_under_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_temp_under_time[0], Eeprom_Alarm_Fault_Time_r.member.Charge_temp_under_time_r[0]);
						handle_debouncing( &Sys_info.member.Charge_temp_under_warning[1],
																						Charge_temp_under_warning1_detect, &Charge_temp_under_time[1][0],
																						Charge_temp_under_warning1_detect, &Charge_temp_under_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_temp_under_time[1], Eeprom_Alarm_Fault_Time_r.member.Charge_temp_under_time_r[1]);
					 
																						
						//---------------------------------------------------------------------放电温度过低
						handle_debouncing( &Sys_info.member.Discharge_temp_under_warning[0],
																						Discharge_temp_under_warning0_detect, &Discharge_temp_under_time[0][0],
																						Discharge_temp_under_warning0_detect, &Discharge_temp_under_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_temp_under_time[0], Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_under_time_r[0]);
						handle_debouncing( &Sys_info.member.Discharge_temp_under_warning[1],
																						Discharge_temp_under_warning1_detect, &Discharge_temp_under_time[1][0],
																						Discharge_temp_under_warning1_detect, &Discharge_temp_under_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_temp_under_time[1], Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_under_time_r[1]);
						
		}
                                        
		//---------------------------------------------------------------------温度过高
		if(max_temp_cal)
		{
					 //充电温度过高   750ms累加一次
						handle_debouncing( &Sys_info.member.Charge_temp_over_warning[0],
																						Charge_temp_over_warning0_detect, &Charge_Temp_over_time[0][0],
																						Charge_temp_over_warning0_detect, &Charge_Temp_over_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_temp_over_time[0], Eeprom_Alarm_Fault_Time_r.member.Charge_temp_over_time_r[0]);
						handle_debouncing( &Sys_info.member.Charge_temp_over_warning[1],
																						Charge_temp_over_warning1_detect, &Charge_Temp_over_time[1][0],
																						Charge_temp_over_warning1_detect, &Charge_Temp_over_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Charge_temp_over_time[1], Eeprom_Alarm_Fault_Time_r.member.Charge_temp_over_time_r[1]);  
						//放电温度过高
						handle_debouncing( &Sys_info.member.Discharge_temp_over_warning[0],
																						Discharge_temp_over_warning0_detect, &Discharge_Temp_over_time[0][0],
																						Discharge_temp_over_warning0_detect, &Discharge_Temp_over_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_temp_over_time[0], Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_over_time_r[0]);
						handle_debouncing( &Sys_info.member.Discharge_temp_over_warning[1],
																						Discharge_temp_over_warning1_detect, &Discharge_Temp_over_time[1][0],
																						Discharge_temp_over_warning1_detect, &Discharge_Temp_over_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Discharge_temp_over_time[1], Eeprom_Alarm_Fault_Time_r.member.Discharge_temp_over_time_r[1]);
					 
		}
		
		//---------------------------------------------------------------------温差
		if((max_temp_cal)&&(min_temp_cal))
		{
 
						handle_debouncing( &Sys_info.member.Temp_differ_warning[0],
																						Temp_differ_warning0_detect, &Temp_differ_time[0][0],
																						Temp_differ_warning0_detect, &Temp_differ_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Temp_differ_time[0], Eeprom_Alarm_Fault_Time_r.member.Temp_differ_time_r[0]);
						handle_debouncing( &Sys_info.member.Temp_differ_warning[1],
																						Temp_differ_warning1_detect, &Temp_differ_time[1][0],
																						Temp_differ_warning1_detect, &Temp_differ_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Temp_differ_time[1], Eeprom_Alarm_Fault_Time_r.member.Temp_differ_time_r[1]);                                 
																									
		}
				
		if(mos_temp_cal)
		{
			
					//---------------------------------------------------------------------MOS温度
						handle_debouncing( &Sys_info.member.Mos_temp_over_warning[0],
																						Mos_temp_over_warning0_detect, &Mos_temp_over_time[0][0],
																						Mos_temp_over_warning0_detect, &Mos_temp_over_time[0][1], 
																						Eeprom_Alarm_Fault_Time.member.Mos_temp_over_time[0], Eeprom_Alarm_Fault_Time_r.member.Mos_temp_over_time_r[0]);
						handle_debouncing( &Sys_info.member.Mos_temp_over_warning[1],
																						Mos_temp_over_warning1_detect, &Mos_temp_over_time[1][0],
																						Mos_temp_over_warning1_detect, &Mos_temp_over_time[1][1], 
																						Eeprom_Alarm_Fault_Time.member.Mos_temp_over_time[1], Eeprom_Alarm_Fault_Time_r.member.Mos_temp_over_time_r[1]);                                       
																									
		}
		
	  //标志位清零
		system_vol_cal=0;
		max_vol_cal=0;
		min_vol_cal=0;
		max_temp_cal=0;
		min_temp_cal=0;
		current_cal=0;
		mos_temp_cal=0;
	
		//总压过压
		Sys_info.member.Module_voltage_over_warning_level = Found_most_level(&Sys_info.member.Module_voltage_over_warning[0] );
		
		//总压欠压
		Sys_info.member.Module_voltage_under_warning_level = Found_most_level(&Sys_info.member.Module_voltage_under_warning[0]);
				
		//放电过流0
		if((Sys_info.member.Current_Status == 1) || (Sys_info.member.Discharge_current_over_warning_level == 2) || (Sys_info.member.Discharge_current_over_warning_level1 == 2))
		{
				Sys_info.member.Discharge_current_over_warning_level = Found_most_level(&Sys_info.member.Discharge_current_over_warning[0]); 
				//放电过流1
				Sys_info.member.Discharge_current_over_warning_level1 = Found_most_level(&Sys_info.member.Discharge_current_over_warning1[0]); 
		}
				
		//充电过流
		if((Sys_info.member.Current_Status == 2) || (Sys_info.member.Charge_current_over_warning_level == 2))
		{
				Sys_info.member.Charge_current_over_warning_level = Found_most_level(&Sys_info.member.Charge_current_over_warning[0]);
		}
		
		//单体过压
		Sys_info.member.Cell_vol_over_warning_level = Found_most_level(&Sys_info.member.Cell_vol_over_warning[0]); 
		
		//单体欠压
		Sys_info.member.Cell_vol_under_warning_level = Found_most_level(&Sys_info.member.Cell_vol_under_warning[0]);
		

		//压差过大
		Sys_info.member.Cell_vol_differ_warning_level = Found_most_level(&Sys_info.member.Cell_vol_differ_warning[0]);

		//修改温差保护，但是没有保护时间与恢复时间，到阈值会立即保护
		//测温序号0、1、2、3、4、5、6、7，其中0、2、5、7一组，1、3、4、6一组
		//第一组0、2、5、7  组内计算温差
		if(Sys_info.member.Cell_Temp[0] < Sys_info.member.Cell_Temp[2])
		{
				Find_TemCell_Min1 = Sys_info.member.Cell_Temp[0];
				Find_TemCell_Max1 = Sys_info.member.Cell_Temp[2];
		}
		else 
		{
				Find_TemCell_Min1 = Sys_info.member.Cell_Temp[2];
				Find_TemCell_Max1 = Sys_info.member.Cell_Temp[0];
		}
				
		if(Sys_info.member.Cell_Temp[5] < Find_TemCell_Min1)
		{
			Find_TemCell_Min1 = Sys_info.member.Cell_Temp[5];
		}
		else if(Sys_info.member.Cell_Temp[5] > Find_TemCell_Max1)
		{
			Find_TemCell_Max1 = Sys_info.member.Cell_Temp[5];
		}
		else
		{}
				
		if(Sys_info.member.Cell_Temp[7] < Find_TemCell_Min1)
		{
			Find_TemCell_Min1 = Sys_info.member.Cell_Temp[7];
		}	
		else if(Sys_info.member.Cell_Temp[7] > Find_TemCell_Max1)
		{
			Find_TemCell_Max1 = Sys_info.member.Cell_Temp[7];
		}
		else
		{}
				
		//第二组1、3、4、6  组内计算温差
		if(Sys_info.member.Cell_Temp[1] < Sys_info.member.Cell_Temp[3])
		{
			Find_TemCell_Min2 = Sys_info.member.Cell_Temp[1];
			Find_TemCell_Max2 = Sys_info.member.Cell_Temp[3];
		}
		else 
		{
			Find_TemCell_Min2 = Sys_info.member.Cell_Temp[3];
			Find_TemCell_Max2 = Sys_info.member.Cell_Temp[1];
		}
				
		if(Sys_info.member.Cell_Temp[4] < Find_TemCell_Min2)
		{
			Find_TemCell_Min2 = Sys_info.member.Cell_Temp[4];
		}
		else if(Sys_info.member.Cell_Temp[4] > Find_TemCell_Max2)
		{
			Find_TemCell_Max2 = Sys_info.member.Cell_Temp[4];
		}
		else
		{}
		
		if(Sys_info.member.Cell_Temp[6] < Find_TemCell_Min2)
		{
			Find_TemCell_Min2 = Sys_info.member.Cell_Temp[6];
		}	
		else if(Sys_info.member.Cell_Temp[6] > Find_TemCell_Max2)
		{
			Find_TemCell_Max2 = Sys_info.member.Cell_Temp[6];
		}
		else
		{}

		//计算温差 
		Find_TemCell_Error1 = Find_TemCell_Max1 - Find_TemCell_Min1;
		Find_TemCell_Error2 = Find_TemCell_Max2 - Find_TemCell_Min2;

		if(Find_TemCell_Error1 > Find_TemCell_Error2)
		{
				Find_TemCell_Error = Find_TemCell_Error1;
		}
		else
		{
				Find_TemCell_Error = Find_TemCell_Error2;
		}
				
		//温差过大
		Sys_info.member.Temp_differ_warning_level = Found_most_level(&Sys_info.member.Temp_differ_warning[0]);
														
		//在加模式中，充电低温故障清除
		if(Sys_info.member.Work_State == DC_CHARGE_HEAT) 
		{
				Sys_info.member.Charge_temp_under_warning_level = 0;		
		}
				
		//充电温度过高
		Sys_info.member.Charge_temp_over_warning_level = Found_most_level(&Sys_info.member.Charge_temp_over_warning[0]); 
				
//		if((Sys_info.member.Work_State == DC_CHARGE)\
//			||(Sys_info.member.Charge_temp_under_warning_level == 2))
//		{
//				//在充电模式中，如果温度低于0℃，则判断是否要进入加热模式，若进入加热模式，则不判断低温故障
//				if(Sys_info.member.Cell_Tmp_Min <= 400)
//				{
//						//充档低温时间判断				
//						if(Charge_LowTem_Count <= 500)//50s
//						{
//								Charge_LowTem_Count++;
//						}
//						else
//						{
//								//充电温度过低
//								Sys_info.member.Charge_temp_under_warning_level = Found_most_level(&Sys_info.member.Charge_temp_under_warning[0]);
//								Charge_LowTem_Count = 501;
//						}						
//				}
//				else
//				{
//						Charge_LowTem_Count = 0;
//				}
//				
//				if(Sys_info.member.Charge_temp_under_warning_level == 2)
//				{
//						Sys_info.member.Charge_temp_under_warning_level = Found_most_level(&Sys_info.member.Charge_temp_under_warning[0]);
//				}
//		}
//		else
//		{
//				Charge_LowTem_Count = 0;
//		}

		if((Sys_info.member.Work_State == DC_DISCHARGE)\
			 ||(Sys_info.member.Discharge_temp_under_warning_level == 2)\
			 ||(Sys_info.member.Discharge_temp_under_warning_level == 1))
		{				
				//放电温度过低
				Sys_info.member.Discharge_temp_under_warning_level = Found_most_level(&Sys_info.member.Discharge_temp_under_warning[0]);
		}
		
		if(Sys_info.member.Cell_Tmp_Min < 200)//温度低于-20℃，就直接进低温故障判断
		{
				//不进行充电低温判断，低温充电进加热，加热失败禁止充电，禁止充电失败报故障停机
				//充电温度过低
//				Sys_info.member.Charge_temp_under_warning_level = Found_most_level(&Sys_info.member.Charge_temp_under_warning[0]);
				//放电温度过低
				Sys_info.member.Discharge_temp_under_warning_level = Found_most_level(&Sys_info.member.Discharge_temp_under_warning[0]);
		}
				
	
		//放电温度过高
		Sys_info.member.Discharge_temp_over_warning_level = Found_most_level(&Sys_info.member.Discharge_temp_over_warning[0]); 				
			
		//MOS温度过高
		Sys_info.member.Mos_temp_over_warning_level = Found_most_level(&Sys_info.member.Mos_temp_over_warning[0]);
		//PCS通信超时见最下方
		
		//主控通信超时见最下方
		
		//AFE通信超时见最下方
		Sys_info.member.Charge_warning_level = Is_Charge_OK();              //返回2标志最严重级别，返回1表示报警 ，返回0表表示没有问题。
		Sys_info.member.Discharge_warning_level = Is_Disharge_OK();         //返回2标志最严重级别，返回1表示报警 ，返回0表表示没有问题。
		
		if(Sys_info.member.Current_Status == 2) 
		{								   
				Sys_info.member.F_BattFltLev = Sys_info.member.Charge_warning_level;				 
		}				
		else
		{				   				
				Sys_info.member.F_BattFltLev = Sys_info.member.Discharge_warning_level;		
		}		
//        WarnningCounterInv--;	//ClassB				
}

static rt_uint8_t Is_Charge_OK(void)             
{
	 //==2 最严重 ， ==1 报警 ==0  没事      
		static rt_uint8_t rt=0;

		if((Sys_info.member.Module_voltage_over_warning_level == 2)
			
		||(Sys_info.member.Cell_vol_over_warning_level == 2)

		||(Sys_info.member.Cell_vol_differ_warning_level == 2)
		
		||(Sys_info.member.Charge_current_over_warning_level == 2)     
	 
		||(Sys_info.member.Charge_temp_over_warning_level == 2)
		
		||(Sys_info.member.Charge_temp_under_warning_level == 2)
											 
		||(Sys_info.member.Temp_differ_warning_level == 2)
										
		||(Sys_info.member.Mos_temp_over_warning_level == 2)
		
		||(Sys_info.member.Pcs_commu_fail_warning_level == 2)   
		
		||(Sys_info.member.Afe_commu_fail_warning_level == 2)			
		
		||(Sys_info.member.Precharge_Fail == 1)
		
		||(Sys_info.member.Short_Cricuit_Flag == 1)
		
		||(Sys_info.member.Current_Error == 1)
		
		||(Sys_info.member.CS_Error == 1)
		
		||(Sys_info.member.Temperature_Error == 1)
			
		)
		{
				rt = 2;       
		}				
		else if((Sys_info.member.Module_voltage_over_warning_level == 1)
			
		||(Sys_info.member.Cell_vol_over_warning_level == 1)
		
		||(Sys_info.member.Charge_current_over_warning_level == 1)		      
	 
		||(Sys_info.member.Charge_temp_over_warning_level ==1)
		
		||(Sys_info.member.Charge_temp_under_warning_level == 1)
		
		||(Sys_info.member.Heat_Line_Error == 1)
											 
		||(Sys_info.member.Temp_differ_warning_level ==1)		 
		)
		{
				rt=1;          
		}     
		else
		{
				rt=0;         
		}

		if((Sys_info.member.Bcu_commu_fail_warning_level == 2) && (Switch_Number > 1))
		{
				rt=2; 
		}
		
		return rt;   
}


static rt_uint8_t Is_Disharge_OK(void)        
{
	//==2 最严重 ， ==1 报警 ==0  没事
		static rt_uint8_t rt=0;

		if((Sys_info.member.Module_voltage_over_warning_level == 2)
			
		||(Sys_info.member.Module_voltage_under_warning_level == 2)
		
		||(Sys_info.member.Cell_vol_differ_warning_level == 2)
		
		||(Sys_info.member.Cell_vol_under_warning_level == 2)   
		
		||(Sys_info.member.Discharge_current_over_warning_level == 2)   

		||(Sys_info.member.Discharge_current_over_warning_level1 == 2) 
		
		||(Sys_info.member.Discharge_temp_under_warning_level == 2)					
									 
		||(Sys_info.member.Temp_differ_warning_level == 2)
		
		||(Sys_info.member.Mos_temp_over_warning_level == 2)
		
		||(Sys_info.member.Pcs_commu_fail_warning_level == 2)   
		
		||(Sys_info.member.Afe_commu_fail_warning_level == 2)
		
		||(Sys_info.member.Precharge_Fail == 1)
						
		||(Sys_info.member.Short_Cricuit_Flag == 1)
		
		||(Sys_info.member.Current_Error == 1)
		
		||(Sys_info.member.CS_Error == 1)
		
		||(Sys_info.member.Temperature_Error == 1)
					
		)        
		{
				rt=2;        
		}
		else if((Sys_info.member.Module_voltage_over_warning_level == 1)
			
		||(Sys_info.member.Module_voltage_under_warning_level == 1)
					
		||(Sys_info.member.Cell_vol_under_warning_level == 1)   
		
		||(Sys_info.member.Discharge_current_over_warning_level == 1)
				 
		||(Sys_info.member.Discharge_temp_under_warning_level == 1)					
													 
		||(Sys_info.member.Temp_differ_warning_level == 1) 
		
		||(Sys_info.member.Heat_Line_Error == 1)				
		
		||(Sys_info.member.Mos_temp_over_warning_level == 1)
		)
		{
				rt=1;           
		}     
		else
		{
				rt=0;        
		}
		
		if((Sys_info.member.Bcu_commu_fail_warning_level == 2) && (Switch_Number > 1))
		{
				rt=2; 
		}
		
		return rt;                 
}

//计算最高等级故障
 static rt_uint8_t Found_most_level(rt_uint8_t * data)
 {
		rt_uint8_t rt = 0;
		rt_uint8_t error[2];
	 
		error[0] = *data;
		data++;
		error[1] = *data;

		if((error[0] == RT_TRUE)&&(error[1] == RT_TRUE))
		{
				rt=0;    
		}
		else
		{
				if(error[0] == RT_FALSE)
				{
						rt = 2;     
				}
				else  if(error[1] == RT_FALSE)
				{
						rt = 1;        
				}              
		}  
			
		return rt;
 }

//总压过压
static rt_uint8_t Module_voltage_over_warning0_detect(void)
{
		rt_uint16_t vol=0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Bat_Voltage;  //单位0.1伏    
																						
		if(vol > Eeprom_Alarm_Fault_Value.member.Module_voltage_over[0])//36.3*16 
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Module_voltage_over_r[0])//36*16 总电压充电过压，单位0.1V
		{
				rt = RT_TRUE;      
		}  
		
		return rt;
}

static rt_uint8_t Module_voltage_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Bat_Voltage;  //单位0.1伏    
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Module_voltage_over[1])//35.3*16
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Module_voltage_over_r[1])//35*16
		{
				rt = RT_TRUE;      
		}  
		
		return rt;
}

//总压欠压
 static rt_uint8_t Module_voltage_under_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Bat_Voltage;  //单位0.1伏    
					
		if(vol < Eeprom_Alarm_Fault_Value.member.Module_voltage_under[0])//25*16 单位0.1V
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Module_voltage_under_r[0])//28.3*16
		{
				rt = RT_TRUE;      
		}        
		return rt;
}


 static rt_uint8_t Module_voltage_under_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Bat_Voltage;  //单位0.1伏    
																							 		
		if(vol < Eeprom_Alarm_Fault_Value.member.Module_voltage_under[1])//31*16 单位0.1V
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Module_voltage_under_r[1])//31.3*16
		{
				rt = RT_TRUE;      
		}          
		return rt;
}

//放电过流0
static rt_uint8_t Discharge_current_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_current_over[0])//1200;   //单位0.1A
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r[0])//900
		{
				rt = RT_TRUE;      
		}                 
		return rt;
}

static rt_uint8_t Discharge_current_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_current_over[1])//1000
		{
				rt = RT_FALSE;     
		}
		else if(vol<Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r[1])//700
		{
				rt = RT_TRUE;      
		}  
			
		return rt;
}
//放电过流1
 static rt_uint8_t Discharge_current_over_warning0_detect1(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_current_over1[0])//1030;//单位0.1A
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r1[0])//1000
		{
				rt = RT_TRUE;      
		}                 
		return rt;
}

 static rt_uint8_t Discharge_current_over_warning1_detect1(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_current_over1[1])//1000
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Discharge_current_over_r1[1])//700
		{
				rt = RT_TRUE;      
		}  
			
		return rt;
}

//充电过流 
 static rt_uint8_t Charge_current_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     
		if(vol > Eeprom_Alarm_Fault_Value.member.Charge_current_over[0])//1030;   //单位0.1A
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Charge_current_over_r[0])//1000
		{
				rt = RT_TRUE;      
		}  
	 		
		return rt;
}

static rt_uint8_t Charge_current_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cur1/100;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Charge_current_over[1])//1000;   //单位0.1A
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Charge_current_over_r[1])//700
		{
				rt = RT_TRUE;      
		}            
		return rt;
}
//电芯单体电压过高
static rt_uint8_t Cell_vol_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Volt_Max;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Cell_vol_over[0])//3630
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Cell_vol_over_r[0])//3600
		{
				rt = RT_TRUE;      
		}        
		
		return rt;
}

static rt_uint8_t Cell_vol_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Volt_Max;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Cell_vol_over[1])//3550
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Cell_vol_over_r[1])//3500
		{
				rt = RT_TRUE;      
		}           
		return rt;     
}

//电芯单体电压过低
static rt_uint8_t Cell_vol_under_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Volt_Min;     
				
		if(vol < Eeprom_Alarm_Fault_Value.member.Cell_vol_under[0])//2500
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Cell_vol_under_r[0])//2830
		{
				rt = RT_TRUE;      
		} 

		return rt;
}

static rt_uint8_t Cell_vol_under_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Volt_Min;     
		
		if(vol < Eeprom_Alarm_Fault_Value.member.Cell_vol_under[1])//3100
		{
				rt = RT_FALSE;     
		}
		else if(vol>Eeprom_Alarm_Fault_Value_r.member.Cell_vol_under_r[1])//3130
		{
				rt = RT_TRUE;      
		}        
		
		return rt;
}


//电芯压差过大
static rt_uint8_t Cell_vol_differ_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_volt_Deff;     
			
		if(vol > Eeprom_Alarm_Fault_Value.member.Cell_vol_differ[0])//
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Cell_vol_differ_r[0])//
		{
				rt = RT_TRUE;      
		} 
		
		return rt;       
}

static rt_uint8_t Cell_vol_differ_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_volt_Deff;     

		if(vol > Eeprom_Alarm_Fault_Value.member.Cell_vol_differ[1])//
		{
				rt = RT_FALSE;     
		}
		else if(vol<Eeprom_Alarm_Fault_Value_r.member.Cell_vol_differ_r[1])//
		{
				rt = RT_TRUE;      
		}     
		
		return rt;   
}


//充电温度过低
static rt_uint8_t Charge_temp_under_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Min;     
		
		if(vol < Eeprom_Alarm_Fault_Value.member.Charge_temp_under[0])//400+20
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Charge_temp_under_r[0])//400+70
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}

static rt_uint8_t Charge_temp_under_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Min;     
		
		if(vol < Eeprom_Alarm_Fault_Value.member.Charge_temp_under[1])//400+100
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Charge_temp_under_r[1])//400+150
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}
//放电温度过低
static rt_uint8_t Discharge_temp_under_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Min;     
		
		if(vol < Eeprom_Alarm_Fault_Value.member.Discharge_temp_under[0])//400-180
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_under_r[0])//400-150
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}

static rt_uint8_t Discharge_temp_under_warning1_detect(void) 
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Min;     
		
		if(vol < Eeprom_Alarm_Fault_Value.member.Discharge_temp_under[1])//400-80
		{
				rt = RT_FALSE;     
		}
		else if(vol > Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_under_r[1])//400-50
		{
				rt = RT_TRUE;      
		}        
		return rt;
}
//充电温度过高
static rt_uint8_t Charge_temp_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Max;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Charge_temp_over[0])//400+530
		{
				rt = RT_FALSE;    
					
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Charge_temp_over_r[0])//400+500
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}

static rt_uint8_t Charge_temp_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Max;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Charge_temp_over[1])//400+480
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Charge_temp_over_r[1])//400+450
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}
//放电温度过高
static rt_uint8_t Discharge_temp_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Cell_Tmp_Max;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_temp_over[0])//400+53
		{
				rt = RT_FALSE;     
		}
		else if(vol<Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_over_r[0])//400+500
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}

static rt_uint8_t Discharge_temp_over_warning1_detect(void) 
{
	rt_uint16_t vol = 0;
	static rt_uint8_t rt = RT_TRUE;
	
	vol = Sys_info.member.Cell_Tmp_Max;     
	
	if(vol > Eeprom_Alarm_Fault_Value.member.Discharge_temp_over[1])//400+480
	{
			rt = RT_FALSE;     
	}
	else if(vol < Eeprom_Alarm_Fault_Value_r.member.Discharge_temp_over_r[1])//400+450
	{
			rt = RT_TRUE;      
	}        
	return rt;
}


//电芯温差过大

static rt_uint8_t Temp_differ_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		//vol=Sys_info.member.Cell_Tmp_Deff;     
		vol = Find_TemCell_Error;
		if(vol > Eeprom_Alarm_Fault_Value.member.Temp_differ[0])//150
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Temp_differ_r[0])//400+80
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}

static rt_uint8_t Temp_differ_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		//vol=Sys_info.member.Cell_Tmp_Deff;     
		vol = Find_TemCell_Error;
		if(vol > Eeprom_Alarm_Fault_Value.member.Temp_differ[1])//70
		{
				rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Temp_differ_r[1])//400+60
		{
				rt = RT_TRUE;      
		} 
		
		return rt;
}
//MOS过温告警
static rt_uint8_t Mos_temp_over_warning0_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Mos_Temp;     
		
		if(vol > Eeprom_Alarm_Fault_Value.member.Mos_temp_over)//400+1000																			//c此处阈值保留了一个，需要重新处理
		{
						rt = RT_FALSE;     
		}
		else if(vol < Eeprom_Alarm_Fault_Value_r.member.Mos_temp_over_r)//400+800															//c此处阈值保留了一个，需要重新处理
		{
						rt = RT_TRUE;      
		}     
		
		return rt;
}


static rt_uint8_t Mos_temp_over_warning1_detect(void)
{
		rt_uint16_t vol = 0;
		static rt_uint8_t rt = RT_TRUE;
		
		vol = Sys_info.member.Mos_Temp;     
		
		if(vol>Eeprom_Alarm_Fault_Value.member.Mos_temp_over)//400+900																		//c此处阈值保留了一个，需要重新处理
		{
				rt = RT_FALSE;     
		}
		else if(vol<Eeprom_Alarm_Fault_Value_r.member.Mos_temp_over_r)//400+700														//c此处阈值保留了一个，需要重新处理
		{
				rt = RT_TRUE;      
		}     
		
		return rt;
}

/*
************************************************************************************************************************
*     PCS/BCU/AFE通信告警
*     1s执行一次
************************************************************************************************************************
*/
void Commu_fail_warning_detect(void)
{
		rt_uint16_t i;
	  rt_uint16_t j;
	  static rt_uint16_t Comm_Fail_Count = 0;
	
		//启动延时1min判断通信故障
		if(Comm_Fail_Count < 60)
		{
				Comm_Fail_Count++;		
		}
		else
		{
				if(Switch_Number == 0)
				{
						//如果单机运行
						//判断与PCS通信线路
						if(PCS_Communication_Count < 15)
						{
								PCS_Communication_Count++;							//5s没收到数据，报通信异常					
						}
						else
						{
								PCS_Communication_Count = 15;
								Sys_info.member.Pcs_commu_fail_warning_level = 2;							
						}				
				}
				else if(Switch_Number == 1)
				{
						//并机的主控
						//判断与PCS通信线路
						if(PCS_Communication_Count < 15)
						{
								PCS_Communication_Count++;							//5s没收到数据，报通信异常					
						}
						else
						{
								PCS_Communication_Count = 15;
								Sys_info.member.Pcs_commu_fail_warning_level = 2;							
						}
										
						//失去从板通信判断,默认最多6台并机
						for(i=0;i<5;i++)
						{
								SLAVE_Communication[i]++;
							
								//10s超时
								if(SLAVE_Communication[i] > 10)
								{
										SLAVE_Communication[i] = 11;
										SLAVE_Online[i] = 0;
									
										//主机通知该从机关机，并清除前期接收数据
										switch(i)
										{
											case 0:
												for(j=0;j<40;j++)
												{
														Para_Buffer0.buf[j] = 0;												
												}
												break;
											case 1:
												for(j=0;j<40;j++)
												{
														Para_Buffer1.buf[j] = 0;												
												}
												break;
											case 2:
												for(j=0;j<40;j++)
												{
														Para_Buffer2.buf[j] = 0;												
												}
												break;	
											default:
												break;
										}								
								}
						}
						 				 
				}
				else
				{
						//并机的从控
					  //从机屏蔽与PCS通信故障检测
						PCS_Communication_Count = 0;
						Sys_info.member.Pcs_commu_fail_warning_level = 0;	
					
						//BCU通信检测
						if(BCU_Communication_Count < 30)
						{
								BCU_Communication_Count++;//失去通信							
						}
						else
						{
								BCU_Communication_Count = 30;
								Sys_info.member.Bcu_commu_fail_warning_level = 2;
						}
					
				}				
		}

	
		
		 //AFE通信检测
		if(AFE_Communication_Count < 5)
		{
				AFE_Communication_Count++;//失去通信							
		}
		else
		{
				AFE_Communication_Count = 5;
				Sys_info.member.Afe_commu_fail_warning_level = 2;							
		}
		

}
