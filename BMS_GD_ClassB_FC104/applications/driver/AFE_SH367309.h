#ifndef _AFE_SH367309_H
#define _AFE_SH367309_H

#include <rtdevice.h>

extern  rt_uint8_t   MTPConfVal;
#define AFE_ID 0x34

////Define MTP register addr
// #define MTP_OTC             0x11
// #define MTP_OTCR            0x12
// #define MTP_UTC             0x13
// #define MTP_UTCR            0x14
// #define MTP_OTD             0x15
// #define MTP_OTDR            0x16
// #define MTP_UTD             0x17
// #define MTP_UTDR            0x18
// #define MTP_TR              0x19

#define MTP_CONF			  		0x40
#define MTP_BALANCEH				0x41
// #define MTP_BALANCEL				0x42
// #define MTP_BSTATUS1				0x43
// #define MTP_BSTATUS2				0x44
// #define MTP_BSTATUS3				0x45
// #define MTP_TEMP1			0x46
// #define MTP_TEMP2			0x48
// #define MTP_TEMP3			0x4A
#define MTP_CUR				0x4C
// #define MTP_CELL1			0x4E
// #define MTP_CELL2			0x50
// #define MTP_CELL3			0x52
// #define MTP_CELL4			0x54
// #define MTP_CELL5			0x56
// #define MTP_CELL6			0x58
// #define MTP_CELL7			0x5A
// #define MTP_CELL8			0x5C
// #define MTP_CELL9			0x5E
// #define MTP_CELL10				0x60
// #define MTP_CELL11				0x62
// #define MTP_CELL12				0x64
// #define MTP_CELL13				0x66
// #define MTP_CELL14				0x68
// #define MTP_CELL15				0x6A
// #define MTP_CELL16				0x6C
// #define MTP_ADC2				0x6E
#define MTP_BFLAG1			0x70
// #define MTP_BFLAG2			0x71
// #define MTP_RSTSTAT			0x72
typedef struct	_AFEDATA_
{
	rt_int16_t  Cur1;
	rt_uint16_t Cell[16];
	rt_int16_t  Cadc;
}AFEDATA;

// EEPROM ��������д��ѹVPROΪ�ߵ�ƽʱ���ſ��Զ�д��
typedef union
{
	rt_uint8_t buf[26];
	struct
	{
		// SCONF1      00H
		rt_uint8_t CellNum : 4;	  // ��ش�������  ֧��5-16����о����
		rt_uint8_t Balance : 1;	  // ƽ������λ  1��ƽ�����ⲿMCU����
		rt_uint8_t OverCurPM : 1; // 0��������ֻ�رճ��MOSFET �ŵ����ֻ�رշŵ�MOSFET
		rt_uint8_t ENMOS : 1;	  // ���MOSFET�ָ�����λ  0������  1������
		rt_uint8_t ENPCH : 1;	  // 0������Ԥ��繦��
		// SCONF2      01H
		rt_uint8_t EUVR : 1;   // ���ŵ�ָ����ÿ���λ 0�����ŵ籣���ͷ��븺���޹�
		rt_uint8_t OCRA : 1;   // �����ָ����ÿ���λ  1����������������ʱ�ָ�
		rt_uint8_t CTLC : 2;   // CTL�ܽŹ������� 00 ��ŵ�MOSFET���ڲ��߼�����
		rt_uint8_t DIS_PF : 1; //	0�����ö��ι���籣��
		rt_uint8_t UV_OP : 1;  // ���ŵ�ʱMOSFET����λ  0�����ŵ�ֻ�رշŵ�MOSFET
		rt_uint8_t : 1;		   // ����
		rt_uint8_t E0VB : 1;   // 0���رա���ֹ��ѹ��о��繦�ܡ�
		// OVT/LDRT/OVH 02H
		rt_uint8_t OV8_9 : 2; // ���䱣�������ѹ��λmV
		rt_uint8_t LDRT : 2;  // �����ͷ���ʱ����λ
		rt_uint8_t OVT : 4;	  // ����籣����ʱ0110   ��1S
		// OVL          03H
		rt_uint8_t OV0_7; // ���䱣�������ѹ��λmV
		// UVT/OVRH     04H
		rt_uint8_t OVR8_9 : 2; // �����ָ���ѹ��λ  mV
		rt_uint8_t : 2;		   // ����
		rt_uint8_t UVT : 4;	   // ���ŵ籣����ʱ1S--0110
		// OVRL         05H
		rt_uint8_t OVR0_7; // �����ָ���ѹ��λ  mV
		// UV           06H
		rt_uint8_t UV; // ���ŵ籣����ѹ���üĴ���  *20mV
		// UVR          07H
		rt_uint8_t UVR; // ���ŵ�ָ���ѹ*20mV  UV<UVR
		// BALV     		 08H
		rt_uint8_t BALV; // ƽ�⿪����ѹ���üĴ���*20mV
		// PREV				 09H
		rt_uint8_t PREV; // Ԥ����ѹ���üĴ���*20mV
		// L0V    			 0AH
		rt_uint8_t L0V; // �͵�ѹ��ֹ����ѹ�趨ֵ*20mV
		// PFV					 0BH
		rt_uint8_t PFV; // ���ι���籣����ѹ*20mV
		// OCD1V/OCD1T  0CH
		rt_uint8_t OCD1T : 4; // �ŵ����1������ʱ50ms~40S  �˴����������޸ĳ������
		rt_uint8_t OCD1V : 4; // �ŵ����1������ѹ20mV~200mV
		// OCD2V/OCD2T  0DH
		rt_uint8_t OCD2T : 4; // �ŵ����2������ʱ10ms~20S
		rt_uint8_t OCD2V : 4; // �ŵ����2������ѹ30mV~500mV  VRS2-VRS1
		// SCV/SCT      0EH
		rt_uint8_t SCT : 4; // ��·������ʱ����ʱ�� 0~960us
		rt_uint8_t SCV : 4; // ��·������ѹ50~1000mV  VRS2-VRS1
		// OCCV/OCCT    0FH
		rt_uint8_t OCCT : 4; // ������������ʱ10ms~20S
		rt_uint8_t OCCV : 4; // ������������ѹ20~200mV   VRS1-VRS2
		// MOST/OCRT/PFT   10H
		rt_uint8_t PFT : 2;	 // ���ι���籣����ʱ  8~64S
		rt_uint8_t OCRT : 2; // ��ŵ�����Զ��ָ���ʱ8~64S
		rt_uint8_t MOST : 2; // ��ŵ�MOSFET������ʱ
		rt_uint8_t CHS : 2;	 // ��ŵ�״̬����ѹ  200uV~2000uV
		// OTC             11H
		rt_uint8_t OTC; // �����±������üĴ���
		// OTCR						12H
		rt_uint8_t OTCR; // �����±����ͷ����üĴ���
		// UTC							13H
		rt_uint8_t UTC; // �����±������üĴ���
		// UTCR						14H
		rt_uint8_t UTCR; // �����±����ͷ����üĴ���
		// OTD							15H
		rt_uint8_t OTD; // �ŵ���±������üĴ���
		// OTCR						16H
		rt_uint8_t OTDR; // �ŵ���±����ͷ����üĴ���
		// UTC							17H
		rt_uint8_t UTD; // �ŵ���±������üĴ���
		// UTCR						18H
		rt_uint8_t UTDR; // �ŵ���±����ͷ����üĴ���
		// TR							19H
		rt_uint8_t TR; // �ŵ���±����ͷ����üĴ���
	} member;
} AFE_EEPROM_CONFIG;

typedef union
{
     rt_uint8_t buf[51];
     struct
     {
			//CONF         40H
			rt_uint8_t        IDLE:1;   //0����ֹ�������״̬
			rt_uint8_t        SLEEP:1;	//0������������״̬  1����������״̬������������
			rt_uint8_t        ENWDT:1;  //0���رտ��Ź�  1���������Ź�
			rt_uint8_t        CADCON:1; //1���������ɼ�
			rt_uint8_t        CHGMOS:1; //0���رճ��MOS   1����Ӳ������
			rt_uint8_t        DSGMOS:1; //0���رշŵ�MOS   1����Ӳ������
			rt_uint8_t        PCHMOS:1; //0���ر�Ԥ���MOS 1����Ӳ������
			rt_uint8_t        OCRC:1;   //��������״̬�������OCRCλ����д��0-1-0 ������Ҫ��
		  //BALANCEH     41H
		  rt_uint8_t  			BALANCEH;	//�������CB9-CB16
			//BALANCEL     42H
		  rt_uint8_t  			BALANCEL;	//�������CB1-CB8
			//BSTATUS1     43H
			rt_uint8_t        OV:1;		//��ѹ����״̬λ  1����
		  rt_uint8_t       	UV:1;		//Ƿѹ����״̬λ	1����
		  rt_uint8_t  			OCD1:1;	//�ŵ����1����״̬λ	1����
		  rt_uint8_t  			OCD2:1;	//�ŵ����2����״̬λ	1����
		  rt_uint8_t  			OCC:1;	//����������״̬λ	1����
		  rt_uint8_t  			SC	:1;	//��·����״̬λ		1����
		  rt_uint8_t  			PF:1;		//���ι���籣��״̬λ  1����
			rt_uint8_t  			WDT:1;	//���Ź���� 1����
			//BSTATUS2     44H
		  rt_uint8_t  			UTC:1;	//1 ���������±���
		  rt_uint8_t  			OTC:1;	//1 ���������±���
		  rt_uint8_t  			UTD:1;	//1 �����ŵ���±���
		  rt_uint8_t  			OTD:1;	//1 �����ŵ���±���
			rt_uint8_t       		:4;	//����
			//BSTATUS3     45H
			rt_uint8_t        DSG_FET:1;	//1���ŵ�MOSFET����
		  rt_uint8_t       	CHG_FET:1;	//1�����MOSFET����
		  rt_uint8_t  			PCHG_FET:1;	//1��Ԥ���MOSFET����
		  rt_uint8_t  			L0V:1;	//1�������͵�ѹ��ֹ���
		  rt_uint8_t  			EEPR_WR:1;	//1������д��������
		  rt_uint8_t  				:1;	//����
		  rt_uint8_t  			DSGING:1;	//1���ŵ�״̬
			rt_uint8_t  			CHGING:1;	//1�����״̬
			//TEMP1H       46H
			rt_uint8_t  			TEMP1H;
			//TEMP1L       47H
			rt_uint8_t  			TEMP1L;
			//TEMP2H       48H
			rt_uint8_t  			TEMP2H;
			//TEMP2L       49H
			rt_uint8_t  			TEMP2L;
			//TEMP3H       4AH
			rt_uint8_t  			TEMP3H;
			//TEMP3L       4BH
			rt_uint8_t  			TEMP3L;
			//CURH       	 4CH
			rt_uint8_t  			CURH;
			//CURL         4DH
			rt_uint8_t  			CURL;
			//CELL1H       4EH
			rt_uint8_t  			CELL1H;
			//CELL1L       4FH
			rt_uint8_t  			CELL1L;
			//CELL2H       50H
			rt_uint8_t  			CELL2H;
			//CELL2L       51H
			rt_uint8_t  			CELL2L;
			//CELL3H       52H
			rt_uint8_t  			CELL3H;
			//CELL3L       53H
			rt_uint8_t  			CELL3L;
			//CELL4H       54H
			rt_uint8_t  			CELL4H;
			//CELL4L       55H
			rt_uint8_t  			CELL4L;
			//CELL5H       56H
			rt_uint8_t  			CELL5H;
			//CELL5L       57H
			rt_uint8_t  			CELL5L;
			//CELL6H       58H
			rt_uint8_t  			CELL6H;
			//CELL6L       59H
			rt_uint8_t  			CELL6L;
			//CELL7H       5AH
			rt_uint8_t  			CELL7H;
			//CELL7L       5BH
			rt_uint8_t  			CELL7L;
			//CELL8H       5CH
			rt_uint8_t  			CELL8H;
			//CELL8L       5DH
			rt_uint8_t  			CELL8L;
			//CELL9H       5EH
			rt_uint8_t  			CELL9H;
			//CELL9L       5FH
			rt_uint8_t  			CELL9L;
			//CELL10H       60H
			rt_uint8_t  			CELL10H;
			//CELL10L       61H
			rt_uint8_t  			CELL10L;
			//CELL11H       62H
			rt_uint8_t  			CELL11H;
			//CELL11L       63H
			rt_uint8_t  			CELL11L;
			//CELL12H       64H
			rt_uint8_t  			CELL12H;
			//CELL12L       65H
			rt_uint8_t  			CELL12L;
			//CELL13H       66H
			rt_uint8_t  			CELL13H;
			//CELL13L       67H
			rt_uint8_t  			CELL13L;
			//CELL14H       68H
			rt_uint8_t  			CELL14H;
			//CELL14L       69H
			rt_uint8_t  			CELL14L;
			//CELL15H       6AH
			rt_uint8_t  			CELL15H;
			//CELL15L       6BH
			rt_uint8_t  			CELL15L;
			//CELL16H       6CH
			rt_uint8_t  			CELL16H;
			//CELL16L       6DH
			rt_uint8_t  			CELL16L;

			//CADCDH       6EH
			rt_uint8_t  			CADCDH;
			//CADCDL       6FH
			rt_uint8_t  			CADCDL;
			//BFLAG1       70H
			rt_uint8_t  			OV_FLAG:1;//��ѹ������־λ ������1
			rt_uint8_t  			UV_FLAG:1;//Ƿѹ������־λ ������1
			rt_uint8_t  			OCD_FLAG:1;//�ŵ����������־λ  ������1
			rt_uint8_t  			LOAD_FLAG:1;//LDO3������־λ  ������1
			rt_uint8_t  			OCC_FLAG:1;//��������־λ  ������1
			rt_uint8_t  			SC_FLAG:1;//��������·����  ������1
			rt_uint8_t  			PF_FLAG:1;//���ι���籣����־λ ������1
			rt_uint8_t  			WDT_FLAG:1;//���������Ź����  ������1
			//BFLAG2       71H
			rt_uint8_t  			UTC_FLAG:1;//�����±�����־λ  ������1
			rt_uint8_t  			OTC_FLAG:1;//�����±�����־λ  ������1
			rt_uint8_t  			UTD_FLAG:1;//�ŵ���±�����־λ  ������1
			rt_uint8_t  			OTD_FLAG:1;//�ŵ���±�����־λ  ������1
			rt_uint8_t  			VADC_FLAG:1;//��ѹ�ɼ���ɱ�־λ  ������1
			rt_uint8_t  			CADC_FLAG:1;//�����ɼ���ɱ�־λ  ������1
			rt_uint8_t  			WAKE_FLAG:1;//�����жϱ�־λ  ������1
			rt_uint8_t  			RST_FLAG:1;//ϵͳ��λ���Զ���1  ��MCU����
			//RSTSTAT      72H
			rt_uint8_t  			WDT0:1;
			rt_uint8_t  			WDT1:1;
			rt_uint8_t  			:6;

     }member;

 }AFE_RAM;

// extern AFE_EEPROM_CONFIG afe_eeprom_check;
// U8 MTPWrite(U8 WrAddr, U8 Length, U8  *WrBuf);
// U8 MTPRead(U8 RdAddr, U8 Length, U8  *RdBuf);
// void Soft_ResetAFE(void);
// U8 MTPWriteROM(U8 WrAddr, U8 Length, U8  *WrBuf);
// void  AFE_Go_To_IDLE(void);
// void  AFE_Go_To_SLEEP(void);
// void EnableAFEWdtCadc(void);
// void ClearAFEWdt(void);

#endif
