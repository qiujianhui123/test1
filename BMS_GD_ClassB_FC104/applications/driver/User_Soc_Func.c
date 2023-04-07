#include "User_Soc_Func.h"

rt_uint32_t 	mA_Seconds_Discharged;						//已放电量  单位mAs
rt_uint32_t   capacityValue;		 								//估算的电池总容量
rt_uint8_t 	  coulombSoc = 249;	 								//计算的库伦SOC
rt_uint8_t  	socValue = 250;    								//显示SOC  值范围0~250   单位0.4%
rt_uint8_t    Eeprom_Data0_Save_Flag;
rt_uint16_t    Test_Value;
rt_uint16_t    Test_Value1;
//上电初始化显示SOC和已放电量函数
void Soc_ChangeValue(rt_uint8_t newvalue)
{
		socValue = newvalue;	
		mA_Seconds_Discharged = (rt_uint32_t)((mA_Seconds_Discharged_1bit/100)*Eeprom_Data0.member.SOH*(250-socValue));
}

//SOC初始化
void SOC_Init_Process(void)
{
	//用于标记上电后SOC是否初始化过，如初始化完毕，则不执行下述程序
	static  rt_uint8_t  soc_choose_flag=0;

	if(soc_choose_flag==0)
	{					
			soc_choose_flag = 1;   
			Soc_ChangeValue(Eeprom_Data0.member.SOC);
	}		
}	
 
//不同温度和不同放电倍率下的可放电总容量表 
#define Discharge_C 		5
#define Temp_Ah 				7
 
const float Ah_Dischargeable[Temp_Ah][Discharge_C]=
{
		
		//1/10C  1/3C    1/2C     1C      2c
		{ 68.75,  87.69,  92.60, 103.80, 105.51},  // -20℃
		
		{ 89.50,  96.11,  98.62, 104.88, 105.79},  // -10℃	
		
		{103.84, 100.47, 102.02, 105.46, 105.18},  //0℃
		
		{   105, 101.96,  98.56,  99.51, 103.17},  //10℃	
		
		{   105,    105,    105,    105,    105},  //25℃
		
		{   105,    105,    105,    105,    105},  //45℃	

		{   105,    105,    105,    105,    105},  //55℃	

};

//当前可放电总容量查询函数
 
void TotalAh_Dischargeable_Calculate(void)
{
		rt_uint8_t  discharge_c=0,temp_ah=0;

		//判定列标
		if(Sys_info.member.Cur1<=100)//1/10C
		discharge_c=0;
		else if(Sys_info.member.Cur1<=330)//1/3C
		discharge_c=1;
		else if(Sys_info.member.Cur1<=500)//1/2C
		discharge_c=2;
		else if(Sys_info.member.Cur1<=105)//1C  ????2022.10.5 --MK
		discharge_c=3;
		else if(Sys_info.member.Cur1<=210)//2C
		discharge_c=4;

		//判定行标
		if(Sys_info.member.Cell_Tmp_Min<=200)//-20
		temp_ah=0;
		else if(Sys_info.member.Cell_Tmp_Min<=300)//-10
		temp_ah=1;
		else if(Sys_info.member.Cell_Tmp_Min<=400)//0
		temp_ah=2;
		else if(Sys_info.member.Cell_Tmp_Min<=500)//10
		temp_ah=3;
		else if(Sys_info.member.Cell_Tmp_Min<=650)//25
		temp_ah=4;
		else if(Sys_info.member.Cell_Tmp_Min<=850)//45
		temp_ah=5;
		else if(Sys_info.member.Cell_Tmp_Min<=950)//55
		temp_ah=6;
		
		Sys_info.member.Available_Capacity=Ah_Dischargeable[temp_ah][discharge_c];//Ah  //不同温度和不同放电倍率下的可放电总容量表

}

//循环次数与容量的关系
//1,50,100,150,.........
//SOH=Cycles%50
//if(SOH>91)
//采取每次减0.14%
const float table_soh[92]=
{
		98.19,99.42,98.84,98.09,97.63,97.23,96.86,96.56,96.18,95.89,//11
		95.62,95.39,95.13,94.91,94.64,94.42,94.24,93.98,93.76,93.53,//20
		93.35,93.14,92.98,92.79,92.50,92.35,92.34,92.13,91.95,91.78,//30
		91.62,91.46,91.31,91.16,91.00,90.85,90.69,90.55,90.39,90.25,//40
		90.12,89.98,89.41,89.53,89.40,89.25,89.12,88.97,88.86,88.70,//50
		88.54,88.40,88.23,88.09,87.93,87.81,87.67,87.53,87.38,87.24,//60
		87.11,86.97,86.83,86.69,86.55,86.41,86.27,86.13,85.99,85.85,//70
		85.71,85.57,85.43,85.29,85.15,85.01,84.87,84.73,84.59,84.45,//80
		84.31,84.17,84.03,83.89,83.75,83.61,83.47,83.33,83.19,83.05,//90
		82.91//91
}; 

//根据循环次数计算健康状态
// 循环次数/50，查表得到SOH
void  Get_SOH(void)
{
	 rt_uint16_t i=0,j=0;
	
	 i=Eeprom_Data0.member.Cycle_index/50;
 
	 if(i<92)//前92个数有表可查
		 
	 Sys_info.member.module_soh=table_soh[i];
	 
	 else if(i>=92)//93个数及以后需要计算  每次减0.14
	 {
		 j=i-91;
		 Sys_info.member.module_soh=82.91-0.14*j;		 
	 }
}
 
 
//获取不同放电倍率和温度下的电池容量 然后叠加SOH  获得当前实际可放电总容量 每秒执行一次
void Get_Capacity_Cx_Tx(void)
{
		 float temp=0;
	 
		 TotalAh_Dischargeable_Calculate();	//获取不同放电倍率和温度下的电池容量 
	 
		 Get_SOH();//获取SOH
	 
//		 temp=Sys_info.member.Available_Capacity*Sys_info.member.module_soh/100;//Ah 叠加SOH
		 //SOH不参与电量计算，按照100算
	   temp = Sys_info.member.Available_Capacity;
	 
		 capacityValue=temp*1000;//mAh  此次赋值后被整型化
	 
		 capacityValue=capacityValue*3600;//mAs	 	 
}
 
rt_uint8_t Soc_GetCoulombSoc(void)//根据温度和放电倍率以及已放电量计算当前可以放电容量MK
{
		rt_uint32_t g1 = 0;
		rt_uint32_t g2 = 0;
		rt_uint8_t CoulombSoc = 0;

		Get_Capacity_Cx_Tx();//获取当前温度和倍率下可充放电容量

		g1= mA_Seconds_Discharged;//已放电量  

		g2= capacityValue;//根据当前温度与放电电流得到电池容量
		
		if(g2 > g1)
		{
				g1 = g2 - g1;//当前温度/电流下的电池容量 - 已放电量 = 可放容量		
		}	
		else
		{
				g1 = 0;//剩余容量		
		}
					
	 //同除以1000，防止后续相乘数据溢出
		g1 = g1/1000; 
		
		g2 = g2/1000;
	 //下述归一化到250范围
		g1 = g1*250;
		
		g1 = g1/g2;//
		
		if(g1)//此处禁止校准为100%
		{
		
			if(g1>249)
				g1=249;

		}
        else
        {
            g1 = 1;
        }
		
		CoulombSoc = (rt_uint8_t)g1;
		
		return CoulombSoc;
}

//SOC  0%强制校准(电压欠压校准)
rt_uint8_t Soc_DisCharge_Empty(rt_uint16_t system_min_v)
{
	  static rt_uint8_t  Discharge_empty_save = 0;	
	
		//如果单体电压<2850，放空标志置位
		if((Sys_info.member.Cell_Volt_Min <= 2900) && (Sys_info.member.DisCharge_Empty_Falg == 0) && (Sys_info.member.Discharge_warning_level < 2))
		{			
				Test_Value1 = Sys_info.member.Cell_Volt_Min;
				Sys_info.member.DisCharge_Empty_Falg = 1;
		}
		
		if(Sys_info.member.DisCharge_Empty_Falg == 1)
		{		
			 if(Discharge_empty_save==0)
			 {
					 mA_Seconds_Discharged = (uint32_t)((mA_Seconds_Discharged_1bit/100)*Eeprom_Data0.member.SOH*250);				 
					 socValue = 0;				 
					 Save_Soc_And_EEPROM0();    			 
					 Discharge_empty_save=1;
			 }			 
		}
	  else
		{
					Discharge_empty_save=0;
		}
		
		//如果SOC再次被充电到20%  放空校准恢复	
		if(Sys_info.member.module_soc >= 50)
		{
				Sys_info.member.DisCharge_Empty_Falg = 0; 		
		}
		
		//确定返回值
		return 0;
} 

//充电完成校准
void Soc_ChargeComplete(void)
{
//	  static rt_uint8_t charge_full_save = 0;	
	   //电池满充校准标志=1时，将SOC校准到100%并保存SOC
		if(Sys_info.member.sys_charge_full_flag == 1)
		{			
//			 if(charge_full_save==0)
//			 {
				 Eeprom_Data0.member.Charge_Full_Enable = 0;
				 mA_Seconds_Discharged = 0;				
				 socValue = 250;				 
				 Save_Soc_And_EEPROM0();    
			 
//				 charge_full_save=1;
//			 }			 
//		}
//	  else
//		{
//					charge_full_save=0;
		}
}

//SOC显示刷新
void Soc_UpdateSoc(void)
{
		static rt_uint8_t slewControl_charge = 0;
		static rt_uint8_t slewControl_discharge = 0;
		rt_uint8_t c1 = 0;
//		rt_uint8_t c2 = 0;
	
		coulombSoc = Soc_GetCoulombSoc();
		//库伦SOC
		c1 = coulombSoc;
		//电压反馈的10%SOC  强制校准
		//此处是否加限制条件有待进一步验证？？？？？？？？？？？？？？？？？
//			c2=SOC10_OCV_Check(Sys_info.member.Cell_Volt_Min);
//			
//			if(c2<c1)
//			{
//			c1=c2;
//			}
		
		//如果计算SOC大于显示SOC，只有充电时发生此种情况
		if(c1 > socValue)
		{ 
			c1 = socValue;
			//充电
			if(Sys_info.member.Cur1 > 0&&Sys_info.member.Current_Status==2)
			{
				//最快每27秒变化一次
				if(slewControl_charge < 11)
					slewControl_charge++;
				else 
				{
					slewControl_charge = 0;
					c1++;
				}
			}
		}
		//如果计算SOC小于显示SOC。放电时
		else if(c1 < socValue)
		{
			c1 = socValue;
			//放电
			if(Sys_info.member.Cur1 > 0&&Sys_info.member.Current_Status==1)//电流状态 2表示充电， 1表示放电
			 {
					//最快每10秒变化一次
					if(slewControl_discharge < 11)
						slewControl_discharge++;
					else 
					{
						slewControl_discharge = 0;
						c1--;
					}
			 }
		}
		socValue = c1;//最大值为249	
		
		if(Sys_info.member.Cell_Volt_Max>3550&&Sys_info.member.Current_Status == 2)//电流状态 2表示充电， 1表示放电
		{
				static rt_uint8_t Cell_Volt_Max_Count=0;
				Cell_Volt_Max_Count++;
				if(Cell_Volt_Max_Count > 2)
				{
					Cell_Volt_Max_Count=0;
					Test_Value = Sys_info.member.Cell_Volt_Max;
					Sys_info.member.sys_charge_full_flag = 1;			
					Soc_ChargeComplete();		//充电完成校准	
				}					
		}
		Soc_DisCharge_Empty(Sys_info.member.Cell_Volt_Min);
			
		Sys_info.member.module_soc = socValue;
}

//保存SOC至EEPROM0
void Save_Soc_And_EEPROM0(void) 
{    
		Eeprom_Data0.member.SOC = socValue;
		Eeprom_Data0.member.SOH = Sys_info.member.module_soh;
		
		//传递一个保存指令，去掉，防止一直在写Eeprom
		Eeprom_Data0_Save_Flag = 1;
	
//	  //需增加告警等级<2才能保存
//		__set_PRIMASK(1);  /* 禁止全局中断*/
//		Write_eeprom_data(&Eeprom_Data0.buf[0], 0, 1); 	
//    __set_PRIMASK(0);  /*  使能全局中断 */
//	  STL_InitRunTimeChecks(); 
}

//循环次数累计  
void CyclesCalcu_And_SOC_Save(void) 
{
		volatile rt_uint8_t soc_diff=0;
		volatile rt_uint8_t eeprom_soc=0;

		eeprom_soc = Eeprom_Data0.member.SOC;

		if(eeprom_soc>socValue) 
		{
				soc_diff = eeprom_soc - socValue;
		} 
		else
		{
				soc_diff = socValue- eeprom_soc;
	
				if(soc_diff>=8)
				{
					 Eeprom_Data0.member.Charing_times++; 
				} 
		}
		if(Eeprom_Data0.member.Charing_times>=31) //满充一次就算一个循环
		{
			 Eeprom_Data0.member.Charing_times = Eeprom_Data0.member.Charing_times %31;  				
			 Eeprom_Data0.member.Cycle_index = Eeprom_Data0.member.Cycle_index + 1 ;      
		}

		if(soc_diff>=8) 
		{
				Save_Soc_And_EEPROM0();
		}
} 

//SOC的更新与保存
void soc_1000ms_process(void)
{	
	Soc_UpdateSoc();
	CyclesCalcu_And_SOC_Save() ;      		       
}

/*
current=Sys_info.member.Cur1
Status=Sys_info.member.Current_Status   1  放电   2充电
*/
rt_uint8_t Soc_CountmAh_100ms(rt_uint32_t current, rt_uint8_t Status)
{
 
	rt_uint32_t present;
	
	present = mA_Seconds_Discharged;

	if(Status==1)//放电状态
	{
			present+=current/10;
			if(present < mA_Seconds_Discharged)//检查是否溢出，如果溢出
			{
					//如果溢出将已放电量设置为当前电池最大可放容量
					return  1;
			}
			else//防止已放电量一直累计
			{
					mA_Seconds_Discharged = present;	
					if(mA_Seconds_Discharged>mA_Seconds_Rating)	
					mA_Seconds_Discharged=	mA_Seconds_Rating	;	
			}		 
	}
	else if(Status==2)//充电状态
	{
			present-=current/10;
			if(present>mA_Seconds_Discharged)//检查是否溢出，如果溢出
			{
					//如果溢出将已放电量设置为0,代表电池充满
					mA_Seconds_Discharged=0;
					return  1;
			}
			else
			{
					mA_Seconds_Discharged=present;	
			}		 
	 }
	
	 //需要确定返回值
	 return 0; 
}




