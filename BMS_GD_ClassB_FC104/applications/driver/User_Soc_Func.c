#include "User_Soc_Func.h"

rt_uint32_t 	mA_Seconds_Discharged;						//�ѷŵ���  ��λmAs
rt_uint32_t   capacityValue;		 								//����ĵ��������
rt_uint8_t 	  coulombSoc = 249;	 								//����Ŀ���SOC
rt_uint8_t  	socValue = 250;    								//��ʾSOC  ֵ��Χ0~250   ��λ0.4%
rt_uint8_t    Eeprom_Data0_Save_Flag;
rt_uint16_t    Test_Value;
rt_uint16_t    Test_Value1;
//�ϵ��ʼ����ʾSOC���ѷŵ�������
void Soc_ChangeValue(rt_uint8_t newvalue)
{
		socValue = newvalue;	
		mA_Seconds_Discharged = (rt_uint32_t)((mA_Seconds_Discharged_1bit/100)*Eeprom_Data0.member.SOH*(250-socValue));
}

//SOC��ʼ��
void SOC_Init_Process(void)
{
	//���ڱ���ϵ��SOC�Ƿ��ʼ���������ʼ����ϣ���ִ����������
	static  rt_uint8_t  soc_choose_flag=0;

	if(soc_choose_flag==0)
	{					
			soc_choose_flag = 1;   
			Soc_ChangeValue(Eeprom_Data0.member.SOC);
	}		
}	
 
//��ͬ�¶ȺͲ�ͬ�ŵ籶���µĿɷŵ��������� 
#define Discharge_C 		5
#define Temp_Ah 				7
 
const float Ah_Dischargeable[Temp_Ah][Discharge_C]=
{
		
		//1/10C  1/3C    1/2C     1C      2c
		{ 68.75,  87.69,  92.60, 103.80, 105.51},  // -20��
		
		{ 89.50,  96.11,  98.62, 104.88, 105.79},  // -10��	
		
		{103.84, 100.47, 102.02, 105.46, 105.18},  //0��
		
		{   105, 101.96,  98.56,  99.51, 103.17},  //10��	
		
		{   105,    105,    105,    105,    105},  //25��
		
		{   105,    105,    105,    105,    105},  //45��	

		{   105,    105,    105,    105,    105},  //55��	

};

//��ǰ�ɷŵ���������ѯ����
 
void TotalAh_Dischargeable_Calculate(void)
{
		rt_uint8_t  discharge_c=0,temp_ah=0;

		//�ж��б�
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

		//�ж��б�
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
		
		Sys_info.member.Available_Capacity=Ah_Dischargeable[temp_ah][discharge_c];//Ah  //��ͬ�¶ȺͲ�ͬ�ŵ籶���µĿɷŵ���������

}

//ѭ�������������Ĺ�ϵ
//1,50,100,150,.........
//SOH=Cycles%50
//if(SOH>91)
//��ȡÿ�μ�0.14%
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

//����ѭ���������㽡��״̬
// ѭ������/50�����õ�SOH
void  Get_SOH(void)
{
	 rt_uint16_t i=0,j=0;
	
	 i=Eeprom_Data0.member.Cycle_index/50;
 
	 if(i<92)//ǰ92�����б�ɲ�
		 
	 Sys_info.member.module_soh=table_soh[i];
	 
	 else if(i>=92)//93�������Ժ���Ҫ����  ÿ�μ�0.14
	 {
		 j=i-91;
		 Sys_info.member.module_soh=82.91-0.14*j;		 
	 }
}
 
 
//��ȡ��ͬ�ŵ籶�ʺ��¶��µĵ������ Ȼ�����SOH  ��õ�ǰʵ�ʿɷŵ������� ÿ��ִ��һ��
void Get_Capacity_Cx_Tx(void)
{
		 float temp=0;
	 
		 TotalAh_Dischargeable_Calculate();	//��ȡ��ͬ�ŵ籶�ʺ��¶��µĵ������ 
	 
		 Get_SOH();//��ȡSOH
	 
//		 temp=Sys_info.member.Available_Capacity*Sys_info.member.module_soh/100;//Ah ����SOH
		 //SOH������������㣬����100��
	   temp = Sys_info.member.Available_Capacity;
	 
		 capacityValue=temp*1000;//mAh  �˴θ�ֵ�����ͻ�
	 
		 capacityValue=capacityValue*3600;//mAs	 	 
}
 
rt_uint8_t Soc_GetCoulombSoc(void)//�����¶Ⱥͷŵ籶���Լ��ѷŵ������㵱ǰ���Էŵ�����MK
{
		rt_uint32_t g1 = 0;
		rt_uint32_t g2 = 0;
		rt_uint8_t CoulombSoc = 0;

		Get_Capacity_Cx_Tx();//��ȡ��ǰ�¶Ⱥͱ����¿ɳ�ŵ�����

		g1= mA_Seconds_Discharged;//�ѷŵ���  

		g2= capacityValue;//���ݵ�ǰ�¶���ŵ�����õ��������
		
		if(g2 > g1)
		{
				g1 = g2 - g1;//��ǰ�¶�/�����µĵ������ - �ѷŵ��� = �ɷ�����		
		}	
		else
		{
				g1 = 0;//ʣ������		
		}
					
	 //ͬ����1000����ֹ��������������
		g1 = g1/1000; 
		
		g2 = g2/1000;
	 //������һ����250��Χ
		g1 = g1*250;
		
		g1 = g1/g2;//
		
		if(g1)//�˴���ֹУ׼Ϊ100%
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

//SOC  0%ǿ��У׼(��ѹǷѹУ׼)
rt_uint8_t Soc_DisCharge_Empty(rt_uint16_t system_min_v)
{
	  static rt_uint8_t  Discharge_empty_save = 0;	
	
		//��������ѹ<2850���ſձ�־��λ
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
		
		//���SOC�ٴα���絽20%  �ſ�У׼�ָ�	
		if(Sys_info.member.module_soc >= 50)
		{
				Sys_info.member.DisCharge_Empty_Falg = 0; 		
		}
		
		//ȷ������ֵ
		return 0;
} 

//������У׼
void Soc_ChargeComplete(void)
{
//	  static rt_uint8_t charge_full_save = 0;	
	   //�������У׼��־=1ʱ����SOCУ׼��100%������SOC
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

//SOC��ʾˢ��
void Soc_UpdateSoc(void)
{
		static rt_uint8_t slewControl_charge = 0;
		static rt_uint8_t slewControl_discharge = 0;
		rt_uint8_t c1 = 0;
//		rt_uint8_t c2 = 0;
	
		coulombSoc = Soc_GetCoulombSoc();
		//����SOC
		c1 = coulombSoc;
		//��ѹ������10%SOC  ǿ��У׼
		//�˴��Ƿ�����������д���һ����֤����������������������������������
//			c2=SOC10_OCV_Check(Sys_info.member.Cell_Volt_Min);
//			
//			if(c2<c1)
//			{
//			c1=c2;
//			}
		
		//�������SOC������ʾSOC��ֻ�г��ʱ�����������
		if(c1 > socValue)
		{ 
			c1 = socValue;
			//���
			if(Sys_info.member.Cur1 > 0&&Sys_info.member.Current_Status==2)
			{
				//���ÿ27��仯һ��
				if(slewControl_charge < 11)
					slewControl_charge++;
				else 
				{
					slewControl_charge = 0;
					c1++;
				}
			}
		}
		//�������SOCС����ʾSOC���ŵ�ʱ
		else if(c1 < socValue)
		{
			c1 = socValue;
			//�ŵ�
			if(Sys_info.member.Cur1 > 0&&Sys_info.member.Current_Status==1)//����״̬ 2��ʾ��磬 1��ʾ�ŵ�
			 {
					//���ÿ10��仯һ��
					if(slewControl_discharge < 11)
						slewControl_discharge++;
					else 
					{
						slewControl_discharge = 0;
						c1--;
					}
			 }
		}
		socValue = c1;//���ֵΪ249	
		
		if(Sys_info.member.Cell_Volt_Max>3550&&Sys_info.member.Current_Status == 2)//����״̬ 2��ʾ��磬 1��ʾ�ŵ�
		{
				static rt_uint8_t Cell_Volt_Max_Count=0;
				Cell_Volt_Max_Count++;
				if(Cell_Volt_Max_Count > 2)
				{
					Cell_Volt_Max_Count=0;
					Test_Value = Sys_info.member.Cell_Volt_Max;
					Sys_info.member.sys_charge_full_flag = 1;			
					Soc_ChargeComplete();		//������У׼	
				}					
		}
		Soc_DisCharge_Empty(Sys_info.member.Cell_Volt_Min);
			
		Sys_info.member.module_soc = socValue;
}

//����SOC��EEPROM0
void Save_Soc_And_EEPROM0(void) 
{    
		Eeprom_Data0.member.SOC = socValue;
		Eeprom_Data0.member.SOH = Sys_info.member.module_soh;
		
		//����һ������ָ�ȥ������ֹһֱ��дEeprom
		Eeprom_Data0_Save_Flag = 1;
	
//	  //�����Ӹ澯�ȼ�<2���ܱ���
//		__set_PRIMASK(1);  /* ��ֹȫ���ж�*/
//		Write_eeprom_data(&Eeprom_Data0.buf[0], 0, 1); 	
//    __set_PRIMASK(0);  /*  ʹ��ȫ���ж� */
//	  STL_InitRunTimeChecks(); 
}

//ѭ�������ۼ�  
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
		if(Eeprom_Data0.member.Charing_times>=31) //����һ�ξ���һ��ѭ��
		{
			 Eeprom_Data0.member.Charing_times = Eeprom_Data0.member.Charing_times %31;  				
			 Eeprom_Data0.member.Cycle_index = Eeprom_Data0.member.Cycle_index + 1 ;      
		}

		if(soc_diff>=8) 
		{
				Save_Soc_And_EEPROM0();
		}
} 

//SOC�ĸ����뱣��
void soc_1000ms_process(void)
{	
	Soc_UpdateSoc();
	CyclesCalcu_And_SOC_Save() ;      		       
}

/*
current=Sys_info.member.Cur1
Status=Sys_info.member.Current_Status   1  �ŵ�   2���
*/
rt_uint8_t Soc_CountmAh_100ms(rt_uint32_t current, rt_uint8_t Status)
{
 
	rt_uint32_t present;
	
	present = mA_Seconds_Discharged;

	if(Status==1)//�ŵ�״̬
	{
			present+=current/10;
			if(present < mA_Seconds_Discharged)//����Ƿ������������
			{
					//���������ѷŵ�������Ϊ��ǰ������ɷ�����
					return  1;
			}
			else//��ֹ�ѷŵ���һֱ�ۼ�
			{
					mA_Seconds_Discharged = present;	
					if(mA_Seconds_Discharged>mA_Seconds_Rating)	
					mA_Seconds_Discharged=	mA_Seconds_Rating	;	
			}		 
	}
	else if(Status==2)//���״̬
	{
			present-=current/10;
			if(present>mA_Seconds_Discharged)//����Ƿ������������
			{
					//���������ѷŵ�������Ϊ0,�����س���
					mA_Seconds_Discharged=0;
					return  1;
			}
			else
			{
					mA_Seconds_Discharged=present;	
			}		 
	 }
	
	 //��Ҫȷ������ֵ
	 return 0; 
}




