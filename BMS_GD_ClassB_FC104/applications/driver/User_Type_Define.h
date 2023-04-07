
#ifndef __user_type_define_H_
#define __user_type_define_H_

#include <rtdevice.h>

/*
************************************************************************************************************************
*     派能通信协议
************************************************************************************************************************
*/
typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint8_t      															:1;
		rt_uint8_t     cell_or_module_over_voltage 		:1;	
		rt_uint8_t     cell_or_module_under_voltage 	:1;
		rt_uint8_t     cell_over_temperature 					:1;
		rt_uint8_t     cell_under_temperature 				:1;
		rt_uint8_t      															:1;
		rt_uint8_t      															:1;
		rt_uint8_t     Discharge_over_current					:1;
		
		rt_uint8_t     Charge_over_current 						:1;
		rt_uint8_t      															:1;	
		rt_uint8_t      															:1;
		rt_uint8_t     System_error 									:1;
		rt_uint8_t     																:1;
		rt_uint8_t      															:1;
		rt_uint8_t      															:1;
		rt_uint8_t     																:1;
		
		rt_uint8_t      															:1;
		rt_uint8_t     Cell_or_module_high_voltage 		:1;	
		rt_uint8_t     Cell_or_module_low_voltage			:1;
		rt_uint8_t     Cell_high_temperature 					:1;
		rt_uint8_t     Cell_low_temperature						:1;
		rt_uint8_t      															:1;
		rt_uint8_t      															:1;
		rt_uint8_t     Discharge_high_current					:1;
		
		rt_uint8_t     Charge_high_current 						:1;
		rt_uint8_t     																:1;	
		rt_uint8_t     																:1;
		rt_uint8_t     Internal_communication_fail 		:1;
		rt_uint8_t     																:1;
		rt_uint8_t      															:1;
		rt_uint8_t      															:1;
		rt_uint8_t     																:1;
		
		rt_uint8_t			Module_numbers;
		rt_uint8_t			P;
		rt_uint8_t			N;			
				 }member;
} PCS359;  

typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint16_t    Battery_charge_voltage;				//建议充电电压  0.1V
		rt_uint16_t    Charge_current_limit;					//充电限流  		0.1A
		rt_uint16_t    Discharge_current_limit;				//放电限流 			0.1A
				 }member;
} PCS351;
 
typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint16_t    SOC_of_single_module_or_average_value_of_system;				//1%
		rt_uint16_t    SOH_of_single_module_or_average_value_of_system;				//1%			
				}member;
} PCS355;  
	 
typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint16_t    voltage_of_system;							//0.01V
		rt_uint16_t    current_of_system;							//0.1A
		rt_uint16_t    Average_cell_temperature;			//0.1℃		
				 }member;
} PCS356;  

typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint16_t       voltage_of_system;					//0.01V
		rt_int16_t    		current_of_system;					//0.1A
		rt_uint16_t       Average_cell_temperature;		//0.1℃		
				 }member;
} PCS356_Aiswei; 

typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint8_t      									:1;
		rt_uint8_t     										:1;	
		rt_uint8_t     										:1;
		rt_uint8_t     charge_full_flag 	:1;
		rt_uint8_t     force_charge_flag2	:1;
		rt_uint8_t     force_charge_flag1	:1;
		rt_uint8_t     Discharge_enable 	:1;
		rt_uint8_t     Charge_enable			:1;		
				 }member;
} PCS35C; 

typedef union 
{
	rt_uint8_t buf[8];
	struct {
		rt_uint8_t     Producer1					:8;
		rt_uint8_t     Producer2					:8;	
		rt_uint8_t     Producer3					:8;
		rt_uint8_t     Producer4					:8;
		rt_uint8_t     Producer5					:8;
		rt_uint8_t     Producer6					:8;
		rt_uint8_t     Producer7					:8;
		rt_uint8_t     Producer8					:8;		
				 }member;
} PCS35E; 

/*
************************************************************************************************************************
*     Pack间数据交互参数定义
************************************************************************************************************************
*/
typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint8_t       soc_of_module;							 //模块SOC，单位0.4%													
			rt_uint8_t       soh_of_module;							 //模块SOH，单位0.4%
			rt_uint16_t      max_vol_of_cell;            //电池单体电压最大值，单位0.001伏				
			rt_uint16_t      min_vol_of_cell;            //电池单体电压最小值，单位0.001伏
			rt_uint8_t       max_vol_of_cellnumber;      //最大单体电压编号				
			rt_uint8_t       min_vol_of_cellnumber;      //最小单体电压编号
					 }member;
} LMU_info0;    

typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t      max_temp_of_cell;            //电池温度最大值，单位0.1度，偏差-40
			rt_uint16_t      min_temp_of_cell;            //电池温度最小值，单位0.1度，偏差-40			
			rt_uint8_t       max_temp_of_sensenumber;     //最大温度采样点编号			
			rt_uint8_t       min_temp_of_sensenumber;     //最小温度采样点编号
			rt_uint8_t       work_state;                  //工作状态
			rt_uint8_t       charge_full_flag;            //满充状态				
					 }member;		
} LMU_info1;
	 
typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t      balance_bit;                 //16串均衡位          	
			rt_uint16_t      dc_allowed_current;          //放电允许电流			
			rt_uint16_t      ch_allowed_current;          //充电允许电流          							
			rt_uint8_t 			 f_battfltlev;    					  //故障告警等级																		
					 }member;
		
} LMU_info2;
	 
//分类故障告警等级	 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint8_t          Module_voltage_over_warning_level;  			
			rt_uint8_t          Module_voltage_under_warning_level;
			rt_uint8_t          Cell_vol_over_warning_level;     	
			rt_uint8_t          Cell_vol_under_warning_level;
			rt_uint8_t          Cell_vol_differ_warning_level;
			rt_uint8_t          Charge_current_over_warning_level;    
			rt_uint8_t          Discharge_current_over_warning_level;    
			rt_uint8_t          Charge_temp_over_warning_level;												
					 }member;	
	 } LMU_info3;	 
	 	 	 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint8_t          Charge_temp_under_warning_level;		
			rt_uint8_t          Discharge_temp_over_warning_level;																				
			rt_uint8_t          Discharge_temp_under_warning_level;  
			rt_uint8_t          Temp_differ_warning_level;     
			rt_uint8_t          Mos_temp_over_warning_level;           
			rt_uint8_t          Afe_commu_fail_warning_level;  
			rt_uint8_t          Pcs_commu_fail_warning_level;  							
			rt_uint8_t          Bcu_commu_fail_warning_level;					
					 }member;				
	 } LMU_info4;	 
	                                                                                         
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t         vol_of_cell1;             //模块单体1电压，单位0.001伏																		 
			rt_uint16_t         vol_of_cell2;                   																					
			rt_uint16_t         vol_of_cell3;                  																			 
			rt_uint16_t         vol_of_cell4;                  						 
						}member;
	 } LMU_info5;
			 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t          vol_of_cell5;             //模块单体1电压，单位0.001伏																			 
			rt_uint16_t          vol_of_cell6;                   																					
			rt_uint16_t          vol_of_cell7;                  																			 
			rt_uint16_t          vol_of_cell8;                  						 
						}member;
	 } LMU_info6;
			 
			 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t           vol_of_cell9;             //模块单体1电压，单位0.001伏																			 
			rt_uint16_t           vol_of_cell10;                   																				
			rt_uint16_t           vol_of_cell11;                  																			 
			rt_uint16_t           vol_of_cell12;                  						 
								}member;
	 } LMU_info7;
	 		 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t            vol_of_cell13;             //模块单体1电压，单位0.001伏																			 
			rt_uint16_t            vol_of_cell14;                   																					
			rt_uint16_t            vol_of_cell15;                  																			 
			rt_uint16_t            vol_of_cell16;                  						 
						}member;
	 } LMU_info8;
      

typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t            temp_of_sense1;																			
			rt_uint16_t            temp_of_sense2;                 																			
			rt_uint16_t            temp_of_sense3;              									 										
			rt_uint16_t            temp_of_sense4;             									 
						}member;
	 } LMU_info9;      
        
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t            temp_of_sense5;																				
			rt_uint16_t            temp_of_sense6;                 																			
			rt_uint16_t            temp_of_sense7;              									 										
			rt_uint16_t            temp_of_sense8;             								 
						}member;
	 } LMU_info10;  

typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t            temp_of_mos;
			rt_uint16_t            current_of_module;
			rt_uint8_t             Work_Model;
			rt_uint8_t             Force_Work;									 
						}member;
	 } LMU_info11; 
	 
typedef union 
	 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t       		voltage_of_system;//0.01V
			rt_int16_t    			  current_of_system;	//0.1A
			rt_uint8_t            DisChargr_Empty_Flag;
			rt_uint8_t            Para_Num;	
      rt_uint8_t            Online_State;			
						}member;
	 } LMU_info12; 	 

 typedef union 
 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t       		year;								 
						}member;
 } LMU_info13; 
 
 typedef union 
 {
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t       		Soft_Version;						 
						}member;
 } LMU_info14; 


typedef union 
{
	rt_uint8_t buf[120];
	struct {
			 LMU_info0  frame0;
			 LMU_info1  frame1;
			 LMU_info2  frame2;
			 LMU_info3  frame3;
			 LMU_info4  frame4;
			 LMU_info5  frame5;
			 LMU_info6  frame6;
			 LMU_info7  frame7;
			 LMU_info8  frame8;
			 LMU_info9  frame9;
			 LMU_info10  frame10;
			 LMU_info11  frame11;
			 LMU_info12  frame12;
			 LMU_info13  frame13;
			 LMU_info14  frame14;
					}part;
} CELL_BUFFER; 

typedef union 
{
	rt_uint8_t  buf[8];	
	struct {
		   rt_uint8_t 	Slave_Num1;
		   rt_uint8_t 	Slave_Num2;
			 rt_uint8_t 	Slave_Num3;
			 rt_uint8_t 	Slave_Num4;
			 rt_uint8_t 	Slave_Num5;
			 rt_uint8_t 	Slave_Num6;
			 rt_uint8_t 	Slave_Num7;
			 rt_uint8_t 	Sys_Heat_Flag;
	}member;
}COMM_PACK;
 
/*
************************************************************************************************************************
*     精简后的并机数据
************************************************************************************************************************
*/
typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint8_t       soc_of_module;							 //模块SOC，单位0.4%													
			rt_uint8_t       soh_of_module;							 //模块SOH，单位0.4%
			rt_uint16_t      max_vol_of_cell;            //电池单体电压最大值，单位0.001伏				
			rt_uint16_t      min_vol_of_cell;            //电池单体电压最小值，单位0.001伏
			rt_uint8_t       max_vol_of_cellnumber;      //最大单体电压编号				
			rt_uint8_t       min_vol_of_cellnumber;      //最小单体电压编号
					 }member;
} SLAVE_info0;    

typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t      max_temp_of_cell;            //电池温度最大值，单位0.1度，偏差-40
			rt_uint16_t      min_temp_of_cell;            //电池温度最小值，单位0.1度，偏差-40			
			rt_uint8_t       max_temp_of_sensenumber;     //最大温度采样点编号			
			rt_uint8_t       min_temp_of_sensenumber;     //最小温度采样点编号
			rt_uint8_t       work_state;                  //工作状态
			rt_uint8_t       charge_full_flag;            //满充状态				
					 }member;		
} SLAVE_info1;
	 
typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t      balance_bit;                 //16串均衡位          	
			rt_uint16_t      dc_allowed_current;          //放电允许电流			
			rt_uint16_t      ch_allowed_current;          //充电允许电流          							
			rt_uint8_t 			 f_battfltlev;    					  //故障告警等级
      rt_uint8_t 			 force_work;    					    //强制启动		
					 }member;	
} SLAVE_info2;

typedef union 
{
		rt_uint8_t buf[8];
		struct {
			rt_uint16_t       voltage_of_system;//0.01V
			rt_int16_t    		current_of_system;	//0.1A
			rt_uint8_t        DisChargr_Empty_Flag;
			rt_uint8_t        Para_Num;	
      rt_uint8_t        Online_State;			
						}member;
} SLAVE_info12; 	
	 
typedef union 
{
	rt_uint8_t buf[40];
	struct {
			 SLAVE_info0   frame0;
			 SLAVE_info1   frame1;
			 SLAVE_info2   frame2;
			 SLAVE_info12  frame12;
					}part;
} PARA_BUFFER; 

	 

 
/*
************************************************************************************************************************
*     EEPROM信息参数定义
************************************************************************************************************************
*每个page64byte，实际数据buf只能用62个，剩余两个做crc
*/
typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		rt_uint8_t  		SOC;
		rt_uint8_t 			SOH;

		rt_uint8_t 			Charing_times;
		rt_uint16_t 		Cycle_index; // 循环次数
		rt_uint32_t 		mA_Seconds_Discharged;
		rt_uint16_t 		UVC_times;			 // 欠压充电次数
		rt_uint16_t 		Balance_Enable_V;	 // 均衡开启电压
		rt_uint16_t 		Balance_Disable_V;	 // 均衡关闭电压
		rt_uint16_t 		Heat_Temp_on;		 // 加热打开的温度
		rt_uint16_t 		Heat_Temp_off;		 // 加热关闭的温度
		rt_uint16_t 		Current_Refer;		 // 电流校准值
		rt_uint8_t 			Limit_Current_Enable; // 电流限流模块使能
		rt_uint16_t 		Relay_Close_Times;	 // 继电器闭合次数
		rt_uint8_t 			Heat_Control_Enable;
		rt_uint8_t 			Relay_Control_Enable;
		rt_uint16_t 		Wait_Time;			   // 允许待机时间  单位  分钟
		rt_uint16_t 		Copacity_Of_Product;   // 出厂容量  Ah
		rt_uint16_t 		year;				   // 年
		rt_uint8_t 			month;				   // 月
		rt_uint8_t 			day;					   // 日   出厂日期
		rt_uint8_t 			Parelle_Cell_Number;	   // 电池并数
		rt_uint8_t 			Serial_Cell_Number;	   // 电池串数
		rt_uint32_t 		Serinumber_Of_Battery; // 电池系统编号 编码规则：数据范围：0~FFFFFFFF，有效值：0~4294967296
		rt_uint8_t 			Work_Model;
		rt_uint32_t 		Time_Accumulate;
		rt_uint8_t 			Charge_Full_Enable;
		rt_uint8_t      DisCharge_Empty_Falg;
		rt_uint8_t      Request_Full_Charge;
	} member;
} EEP_DATA0;

typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		rt_uint16_t Module_voltage_over[2]; // 总电压充电过压，单位0.1V
		rt_uint16_t Module_voltage_under[2];
		rt_uint16_t Cell_vol_over[2]; // 充电过压，单位0.001V
		rt_uint16_t Cell_vol_under[2];
		rt_uint16_t Cell_vol_differ[2];
		rt_uint16_t Charge_current_over[2];
		rt_uint16_t Discharge_current_over[2];	// 单位0.1A，偏移量3200
		rt_uint16_t Discharge_current_over1[2]; // 单位0.1A，偏移量3200
		rt_uint16_t Charge_temp_over[2];
		rt_uint16_t Charge_temp_under[2];
		rt_uint16_t Discharge_temp_over[2];
		rt_uint16_t Discharge_temp_under[2];
		rt_uint16_t Temp_differ[2];
		rt_uint16_t Mos_temp_over; // MOS过温去掉告警阈值，用作crc
	} member;
} EEP_ALARM_FAULT1;

typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		rt_uint8_t Module_voltage_over_time[2];
		rt_uint8_t Module_voltage_under_time[2];
		rt_uint8_t Cell_vol_over_time[2];
		rt_uint8_t Cell_vol_under_time[2];
		rt_uint8_t Cell_vol_differ_time[2];
		rt_uint8_t Charge_current_over_time[2];
		rt_uint8_t Discharge_current_over_time[2];
		rt_uint8_t Discharge_current_over_time1[2];
		rt_uint8_t Charge_temp_over_time[2];
		rt_uint8_t Charge_temp_under_time[2];
		rt_uint8_t Discharge_temp_over_time[2];
		rt_uint8_t Discharge_temp_under_time[2];
		rt_uint8_t Temp_differ_time[2];
		rt_uint8_t Mos_temp_over_time[2];
	} member;
} EEP_ALARM_FAULT2;

typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		rt_uint16_t Module_voltage_over_r[2]; // 总电压充电过压，单位0.1V
		rt_uint16_t Module_voltage_under_r[2];
		rt_uint16_t Cell_vol_over_r[2]; // 充电过压，单位0.001V
		rt_uint16_t Cell_vol_under_r[2];
		rt_uint16_t Cell_vol_differ_r[2];
		rt_uint16_t Charge_current_over_r[2];
		rt_uint16_t Discharge_current_over_r[2];  // 单位0.1A，偏移量3200
		rt_uint16_t Discharge_current_over_r1[2]; // 单位0.1A，偏移量3200
		rt_uint16_t Charge_temp_over_r[2];
		rt_uint16_t Charge_temp_under_r[2];
		rt_uint16_t Discharge_temp_over_r[2];
		rt_uint16_t Discharge_temp_under_r[2];
		rt_uint16_t Temp_differ_r[2];
		rt_uint16_t Mos_temp_over_r; // MOS过温恢复告警阈值，用作CRC
	} member;
} EEP_ALARM_FAULT1_R;

typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		rt_uint8_t Module_voltage_over_time_r[2];
		rt_uint8_t Module_voltage_under_time_r[2];
		rt_uint8_t Cell_vol_over_time_r[2];
		rt_uint8_t Cell_vol_under_time_r[2];
		rt_uint8_t Cell_vol_differ_time_r[2];
		rt_uint8_t Charge_current_over_time_r[2];
		rt_uint8_t Discharge_current_over_time_r[2];
		rt_uint8_t Discharge_current_over_time_r1[2];
		rt_uint8_t Charge_temp_over_time_r[2];
		rt_uint8_t Charge_temp_under_time_r[2];
		rt_uint8_t Discharge_temp_over_time_r[2];
		rt_uint8_t Discharge_temp_under_time_r[2];
		rt_uint8_t Temp_differ_time_r[2];
		rt_uint8_t Mos_temp_over_time_r[2];
	} member;
} EEP_ALARM_FAULT2_R;

typedef union
{
	rt_uint8_t buf[64];
	struct
	{
		// SCONF1      00H
		rt_uint8_t CellNum : 4;	  // 电池串数配置  支持5-16串电芯保护
		rt_uint8_t Balance : 1;	  // 平衡配置位  1：平衡由外部MCU控制
		rt_uint8_t OverCurPM : 1; // 0：充电过流只关闭充电MOSFET 放电过流只关闭放电MOSFET
		rt_uint8_t ENMOS : 1;	  // 充电MOSFET恢复控制位  0：禁用  1：开启
		rt_uint8_t ENPCH : 1;	  // 0：禁用预充电功能
		// SCONF2       01H

		rt_uint8_t EUVR : 1;   // 过放电恢复设置控制位 0：过放电保护释放与负载无关
		rt_uint8_t OCRA : 1;   // 电流恢复设置控制位  1：允许电流保护定时恢复
		rt_uint8_t CTLC : 2;   // CTL管脚功能设置 00 充放电MOSFET由内部逻辑控制
		rt_uint8_t DIS_PF : 1; //	0：启用二次过充电保护
		rt_uint8_t UV_OP : 1;  // 过放电时MOSFET控制位  0：过放电只关闭放电MOSFET
		rt_uint8_t : 1;		   // 保留
		rt_uint8_t E0VB : 1;   // 0：关闭“禁止低压电芯充电功能”
		// OVT/LDRT/OVH 02H
		rt_uint8_t OV8_9 : 2; // 过充保护单体电压高位mV
		rt_uint8_t LDRT : 2;  // 负载释放延时控制位
		rt_uint8_t OVT : 4;	  // 过充电保护延时0110   ：1S
		// OVL          03H
		rt_uint8_t OV0_7; // 过充保护单体电压低位mV
		// UVT/OVRH     04H
		rt_uint8_t OVR8_9 : 2; // 过充电恢复电压高位  mV
		rt_uint8_t : 2;		   // 保留
		rt_uint8_t UVT : 4;	   // 过放电保护延时1S--0110
		// OVRL         05H
		rt_uint8_t OVR0_7; // 过充电恢复电压低位  mV
		// UV           06H
		rt_uint8_t UV; // 过放电保护电压设置寄存器  *20mV
		// UVR          07H
		rt_uint8_t UVR; // 过放电恢复电压*20mV  UV<UVR
		// BALV     		08H
		rt_uint8_t BALV; // 平衡开启电压设置寄存器*20mV
		// PREV				09H
		rt_uint8_t PREV; // 预充电电压设置寄存器*20mV
		// L0V    			0AH
		rt_uint8_t L0V; // 低电压禁止充电电压设定值*20mV
		// PFV					0BH
		rt_uint8_t PFV; // 二次过充电保护电压*20mV
		// OCD1V/OCD1T  0CH
		rt_uint8_t OCD1T : 4; // 放电过流1保护延时50ms~40S  此处锁死，需修改程序更改
		rt_uint8_t OCD1V : 4; // 放电过流1保护电压20mV~200mV
		// OCD2V/OCD2T  0DH
		rt_uint8_t OCD2T : 4; // 放电过流2保护延时10ms~20S
		rt_uint8_t OCD2V : 4; // 放电过流2保护电压30mV~500mV  VRS2-VRS1
		// SCV/SCT      0EH
		rt_uint8_t SCT : 4; // 短路保护延时设置时间 0~960us
		rt_uint8_t SCV : 4; // 短路保护电压50~1000mV  VRS2-VRS1
		// OCCV/OCCT    0FH
		rt_uint8_t OCCT : 4; // 充电过流保护延时10ms~20S
		rt_uint8_t OCCV : 4; // 充电过流保护电压20~200mV   VRS1-VRS2
		// MOST/OCRT/PFT  10H
		rt_uint8_t PFT : 2;	 // 二次过充电保护延时  8~64S
		rt_uint8_t OCRT : 2; // 充放电过流自动恢复延时8~64S
		rt_uint8_t MOST : 2; // 充放电MOSFET开启延时
		rt_uint8_t CHS : 2;	 // 充放电状态检测电压  200uV~2000uV
		// OTC             11H
		rt_uint8_t OTC; // 充电高温保护设置寄存器
		// OTCR						12H
		rt_uint8_t OTCR; // 充电高温保护释放设置寄存器
		// UTC							13H
		rt_uint8_t UTC; // 充电低温保护设置寄存器
		// UTCR						14H
		rt_uint8_t UTCR; // 充电低温保护释放设置寄存器
		// OTD							15H
		rt_uint8_t OTD; // 放电高温保护设置寄存器
		// OTCR						16H
		rt_uint8_t OTDR; // 放电高温保护释放设置寄存器
		// UTC							17H
		rt_uint8_t UTD; // 放电低温保护设置寄存器
		// UTCR						18H
		rt_uint8_t UTDR; // 放电低温保护释放设置寄存器
		// TR							19H
		rt_uint8_t TR; // 放电低温保护释放设置寄存器
	} member;
} EEP_AFE;

/*
************************************************************************************************************************
*   系统信息参数定义                                     
************************************************************************************************************************
*/
typedef union 
{
	rt_uint8_t buf[256];
	struct {
	 //定时器信息
		rt_uint8_t                    _10ms:1;					     
		rt_uint8_t            				_50ms:1;
		rt_uint8_t            				_100ms:1;
		rt_uint8_t            				_200ms:1;
		rt_uint8_t            				_500ms:1;
		rt_uint8_t            				_1000ms:1;
		rt_uint8_t            				Sleep_Wakeup_Flag:1;
		rt_uint8_t                    Sleep_Flag;
	 //系统软硬件信息 
		rt_uint32_t                   Version_Of_Firmware;             //软件版本					    
		rt_uint8_t                    Cell_Number;                     //采样数
		rt_uint8_t                    Actual_Cell_Number;              //实际电池串数
		rt_uint8_t                    Temperature_Number;              //温度点个数
		rt_uint8_t                    Work_State;										   //工作状态																									//use
		rt_uint8_t  									St_RlyHVPos;									 	 //总正继电器状态		
		rt_uint8_t   									St_MosFET_PreChrg		;			       //预充MOS																								//use
		rt_uint8_t										St_Charge_MosFET;																																					//use
		rt_uint8_t										St_DisCharge_MosFET;																																			//use
		rt_uint8_t 										St_Heater_MosFET;																																					//use
		rt_uint8_t                    Switch_Number;
		rt_uint8_t                    Precharge_Fail;																																						//use
		rt_uint8_t                    Current_Status;																																						//use
		float                   			Available_Capacity;
		rt_uint8_t            		 		module_soc;                      //系统的SOC，单位：0.4% 																	//use
		rt_uint8_t              			module_soh;                      //系统的SOH，单位：0.4%   //占4个字节							                       								
		rt_uint8_t 										sys_charge_full_flag;																																			//use
		rt_uint8_t 							  		DisCharge_Empty_Falg;
		rt_uint16_t           		  	Bat_Voltage; 																																					//use
		rt_uint8_t                 		Short_Cricuit_Flag;																																	
	
	 //AD采样信息
		rt_uint8_t                    AD_Channel;                    //AD选择的采集通道
		rt_uint8_t                    Temp_Ad_ending;                //9个温度点的AD全部采集结束 
		rt_uint16_t                   Pack_Voltage;																																					//use
		//温度信息               
		rt_uint16_t                   Cell_Temp[8]; 							 	
		rt_uint16_t                   Mos_Temp;
		rt_uint16_t                   Cell_Tmp_Max;                  //最高单体温度,单位0.1度，偏差40
		rt_uint16_t                   Cell_Tmp_Min;                  //最低单体温度,单位0.1度，偏差40														//use
		rt_uint8_t                    Cell_Tmp_Max_num;              //最高单体温度序号        从1到16
		rt_uint8_t                    Cell_Tmp_Min_num;              //最低单体温度序号         从1到16
		rt_uint16_t                   Cell_Tmp_Ave;                  //平均单体温度,单位0.1度，偏差40
		rt_uint16_t                   Cell_Tmp_Deff;                 //最大温度差,单位0.1度，偏差40    
		//电流信息								
		rt_uint32_t                   Cur1; 																																								
		//电压信息               
		rt_uint16_t                   Cell_Volt_Max;                 //最高单体电压
		rt_uint16_t                   Cell_Volt_Min;                 //最低单体电压
		rt_uint8_t                    Cell_Volt_Max_num;             //最高单体电压序号       
		rt_uint8_t                    Cell_Volt_Min_num;             //最低单体电压序号                         
		rt_uint16_t                   Cell_volt_Deff;                //最大压差
		rt_uint16_t										Cell_volt[16];
	 //充放电信息
		//最大可充电电压
		rt_uint16_t                   Max_Charge_Voltage;
		//允许可充放电电流
		rt_uint16_t                   DC_Allowed_Current;        //放电允许电流
		rt_uint16_t                   CH_Allowed_Current;        //充电允许电流              								
		//均衡信息
		rt_uint16_t                   Balance_BIT;          //16串均衡位
		//加热信息
		rt_uint8_t                    Heat_Complete_Flag;   //加热完成标志
		rt_uint8_t                    Heat_Line_Error;   //加热线路故障标志
		rt_uint8_t                    Update_Flag;          //升级标志
	 //分类故障告警发生标志  1未发生   0发生
		rt_uint8_t          					Module_voltage_over_warning[2];      //总电压充电过压
		rt_uint8_t          					Module_voltage_under_warning[2];
		rt_uint8_t          					Cell_vol_over_warning[2];     //充电过压
		rt_uint8_t          					Cell_vol_under_warning[2];
		rt_uint8_t          					Cell_vol_differ_warning[2];
		rt_uint8_t          					Charge_current_over_warning[2];    
		rt_uint8_t          					Discharge_current_over_warning[2];  
		rt_uint8_t          					Discharge_current_over_warning1[2];  
		rt_uint8_t          					Charge_temp_over_warning[2];			
		rt_uint8_t          					Charge_temp_under_warning[2];		
		rt_uint8_t          					Discharge_temp_over_warning[2];																				
		rt_uint8_t          					Discharge_temp_under_warning[2];  
		rt_uint8_t          					Temp_differ_warning[2];  
		rt_uint8_t          					Mos_temp_over_warning[2];		 
		//分类故障告警等级
		rt_uint8_t          					Module_voltage_over_warning_level;      //总电压充电过压，单位0.1V
		rt_uint8_t          					Module_voltage_under_warning_level;
		rt_uint8_t          					Cell_vol_over_warning_level;     //充电过压，单位0.001V
		rt_uint8_t          					Cell_vol_under_warning_level;
		rt_uint8_t          					Cell_vol_differ_warning_level;
		rt_uint8_t          					Charge_current_over_warning_level;    
		rt_uint8_t          					Discharge_current_over_warning_level;      //单位0.1A，偏移量3200 
		rt_uint8_t          					Discharge_current_over_warning_level1;      //单位0.1A，偏移量3200  
		rt_uint8_t          					Charge_temp_over_warning_level;			
		rt_uint8_t          					Charge_temp_under_warning_level;		
		rt_uint8_t          					Discharge_temp_over_warning_level;																				
		rt_uint8_t          					Discharge_temp_under_warning_level;  
		rt_uint8_t          					Temp_differ_warning_level;     
		rt_uint8_t          					Mos_temp_over_warning_level;
		rt_uint8_t          					Afe_commu_fail_warning_level;      
		rt_uint8_t          					Pcs_commu_fail_warning_level;  		
		rt_uint8_t          					Bcu_commu_fail_warning_level; 
		//系统故障等级
		rt_uint8_t 				 						Discharge_warning_level;		
		rt_uint8_t          					Charge_warning_level;																																							//user
		rt_uint8_t 				 						F_BattFltLev;
		//SH367309恢复出厂设置使能位
		rt_uint8_t 				 						AFE_Product_Config;							
		rt_uint8_t					 					AFE_Product_Config_Err;
		//出厂手动测试专用                                
		rt_uint8_t                    Force_Open_Close_Heat;		 //强制开启\关闭加热 0关闭                
		rt_uint8_t                    Force_Open_Close_Balance;	 //强制开启\关闭均衡 0关闭
		rt_uint32_t                   Force_Balance_Flag;		
		//故障检测
		rt_uint8_t                 		CS_Error:1;
		rt_uint8_t                 		Current_Error:1;
		rt_uint8_t             				Temperature_Error:1;
		rt_uint8_t             				BLMU_Send_Flag:1;
		rt_uint8_t             				BLMU_Error:1;
	}member;
}SYS_INFO;























#endif
