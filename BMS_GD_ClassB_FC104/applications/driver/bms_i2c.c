#include <rtthread.h>
#include <rtdevice.h>
#include "GlobalVarShared.h"

/* Note
 * M24256有256kbit空间，每一页的大小Page Size =64，所以计算可得页的取值范围为：256*1024/8/64 = 512 页
 * */

/* 从机地址(不含读写位) */
#define AFE_DEV_ADDR     (0x1A)
/* 每页字节数 */
#define BMS_PAGE_SIZE    (64)
/* 最大页数 */
#define BMS_MAX_PAGE     (512)


rt_uint8_t MTPConfVal = 0;//配置SH367309控制使用，关键变量

static struct rt_i2c_bus_device *bms_i2c = RT_NULL;

rt_uint8_t  CRC8Table[]=
{						
	0x00,0x07,0x0E,0x09,0x1C,0x1B,0x12,0x15,0x38,0x3F,0x36,0x31,0x24,0x23,0x2A,0x2D,
	0x70,0x77,0x7E,0x79,0x6C,0x6B,0x62,0x65,0x48,0x4F,0x46,0x41,0x54,0x53,0x5A,0x5D,
	0xE0,0xE7,0xEE,0xE9,0xFC,0xFB,0xF2,0xF5,0xD8,0xDF,0xD6,0xD1,0xC4,0xC3,0xCA,0xCD,
	0x90,0x97,0x9E,0x99,0x8C,0x8B,0x82,0x85,0xA8,0xAF,0xA6,0xA1,0xB4,0xB3,0xBA,0xBD,
	0xC7,0xC0,0xC9,0xCE,0xDB,0xDC,0xD5,0xD2,0xFF,0xF8,0xF1,0xF6,0xE3,0xE4,0xED,0xEA,
	0xB7,0xB0,0xB9,0xBE,0xAB,0xAC,0xA5,0xA2,0x8F,0x88,0x81,0x86,0x93,0x94,0x9D,0x9A,
	0x27,0x20,0x29,0x2E,0x3B,0x3C,0x35,0x32,0x1F,0x18,0x11,0x16,0x03,0x04,0x0D,0x0A,
	0x57,0x50,0x59,0x5E,0x4B,0x4C,0x45,0x42,0x6F,0x68,0x61,0x66,0x73,0x74,0x7D,0x7A,
	0x89,0x8E,0x87,0x80,0x95,0x92,0x9B,0x9C,0xB1,0xB6,0xBF,0xB8,0xAD,0xAA,0xA3,0xA4,
	0xF9,0xFE,0xF7,0xF0,0xE5,0xE2,0xEB,0xEC,0xC1,0xC6,0xCF,0xC8,0xDD,0xDA,0xD3,0xD4,
	0x69,0x6E,0x67,0x60,0x75,0x72,0x7B,0x7C,0x51,0x56,0x5F,0x58,0x4D,0x4A,0x43,0x44,
	0x19,0x1E,0x17,0x10,0x05,0x02,0x0B,0x0C,0x21,0x26,0x2F,0x28,0x3D,0x3A,0x33,0x34,
	0x4E,0x49,0x40,0x47,0x52,0x55,0x5C,0x5B,0x76,0x71,0x78,0x7F,0x6A,0x6D,0x64,0x63,
	0x3E,0x39,0x30,0x37,0x22,0x25,0x2C,0x2B,0x06,0x01,0x08,0x0F,0x1A,0x1D,0x14,0x13,
	0xAE,0xA9,0xA0,0xA7,0xB2,0xB5,0xBC,0xBB,0x96,0x91,0x98,0x9F,0x8A,0x8D,0x84,0x83,
	0xDE,0xD9,0xD0,0xD7,0xC2,0xC5,0xCC,0xCB,0xE6,0xE1,0xE8,0xEF,0xFA,0xFD,0xF4,0xF3
};

/*******************************************************************************
Function: CRC8cal()
Description:  calculate CRC
Input: 	      *p--data to calculate
          Length--data length
Output: crc8
********************************************************************************/
rt_uint8_t CRC8cal(rt_uint8_t *p, rt_uint8_t Length)    		   //look-up table calculte CRC 
{    
    rt_uint8_t crc8 = 0;    
    
		for(; Length > 0; Length--)
		{    
        crc8 = CRC8Table[crc8^*p];    
				p++;    
    } 
       
    return(crc8);       
}


void I2C2_init(void)
{
	  /* 发现设备 */
    bms_i2c = (struct rt_i2c_bus_device *)rt_device_find("i2c2");
}


/** ************************************************************************
 * @brief   向指定地址address写入单个字节数据 WriteData
 **************************************************************************/
rt_err_t BMS_Byte_Write(rt_uint8_t *WriteData,rt_uint8_t address)
{
    struct rt_i2c_msg msg;
	  rt_uint8_t TempBuf[4];

		TempBuf[0] = AFE_DEV_ADDR << 1;
    TempBuf[1] = address;
    TempBuf[2] = *WriteData;
    TempBuf[3] = CRC8cal(TempBuf, 3);
		
	  rt_uint8_t data[3] = {address,*WriteData,TempBuf[3]};
			
	//发送设备地址、写状态、数据地址、数据
    msg.addr = AFE_DEV_ADDR;
    msg.flags = RT_I2C_WR;
    msg.buf = data;
    msg.len = 3;

		if(rt_i2c_transfer(bms_i2c, &msg, 1) == 1)
		{
			return RT_EOK;
		}
		else
		{
			return RT_ERROR;
		}
}


/** ************************************************************************
 * @brief   向指定页Page写入Size个字节数据 WriteData
 **************************************************************************/
rt_err_t BMS_Page_Write(rt_uint8_t *WriteBuf, rt_uint16_t Page, rt_uint16_t Size)
{
    struct rt_i2c_msg msg[2];

    rt_uint16_t usAddr = Page * BMS_PAGE_SIZE;

    //先发送地址高字节，再发低字节
    rt_uint8_t data[2] = {usAddr>>8,usAddr&0xFF};

    //发送设备地址、写状态、数据地址
    msg[0].addr = AFE_DEV_ADDR;
    msg[0].flags = RT_I2C_WR;
    msg[0].buf = data;
    msg[0].len = 2;

	//发送数据
    msg[1].addr = AFE_DEV_ADDR;
    msg[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msg[1].buf = WriteBuf;
    msg[1].len = Size;

	if(rt_i2c_transfer(bms_i2c, msg, 2) == 2)
	{
		return RT_EOK;
	}
	else
	{
		return -RT_ERROR;
	}
}


/** ************************************************************************
 * @brief   从指定地址address读取单个字节数据
 **************************************************************************/
rt_uint8_t BMS_Byte_Read(rt_uint16_t address)
{
    struct rt_i2c_msg msg[2];
    rt_uint8_t ReadData = 0;

    //先发送地址高字节，再发低字节
    rt_uint8_t data[2] = {address>>8,address&0xFF};

    //发送设备地址、写状态、数据地址
    msg[0].addr = AFE_DEV_ADDR;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].buf = data;
    msg[0].len = 2;

	//发送设备地址、读状态
    msg[1].addr = AFE_DEV_ADDR;
    msg[1].flags = RT_I2C_RD;
    msg[1].buf = &ReadData;
    msg[1].len = 1;

	if(rt_i2c_transfer(bms_i2c, msg, 2) == 2)
	{
		return ReadData;
	}
	else
	{
		return 0;
	}
}


/** ************************************************************************
 * @brief   从AFE读取多个字节数据 ReadBuf
 * @brief   根据协议，读取size个数据后，会自动附件一个crc
 **************************************************************************/
rt_err_t BMS_Page_Read(rt_uint8_t *ReadBuf, rt_uint8_t usAddr, rt_uint8_t Size)
{
		rt_uint8_t i;
    struct rt_i2c_msg msg[2];
		rt_uint8_t RdCrc;
	  rt_uint8_t TempBuf[46];
    rt_uint8_t data[2] = {usAddr,Size};
		
    //发送设备地址、写状态、数据地址
    msg[0].addr = AFE_DEV_ADDR;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].buf = data;
    msg[0].len = 2;

	  //发送设备地址、读状态
    msg[1].addr = AFE_DEV_ADDR;
    msg[1].flags = RT_I2C_RD;
    msg[1].buf = ReadBuf;
    msg[1].len = Size+1;

		//用于crc校验的帧头
		TempBuf[0] = AFE_DEV_ADDR << 1;
		TempBuf[1] = usAddr;
		TempBuf[2] = Size;
		TempBuf[3] = TempBuf[0] | 0x01;
		
	if(rt_i2c_transfer(bms_i2c, msg, 2) == 2)
	{
		//帧头+接收数据
		for(i=0;i<Size;i++)
		{
				TempBuf[4+i] = ReadBuf[i];
		}
		
		//读取接收数据的size+1位
		RdCrc = ReadBuf[Size];
		if(RdCrc != CRC8cal(TempBuf, 4+Size))
		{
				return RT_ERROR;
		}
		
		return RT_EOK;
	}
	else
	{
		return RT_ERROR;
	}
}

/** ************************************************************************
 * @brief   重新封装一个AFE数据读取函数，用于异常后的二次读取
 **************************************************************************/
rt_err_t MTPRead(rt_uint8_t RdAddr, rt_uint8_t Length, rt_uint8_t  *RdBuf)
{
		rt_bool_t result;
	
    //屏蔽中断	
		result = BMS_Page_Read(RdBuf,RdAddr,Length);
	
		if(result)
		{
			result = BMS_Page_Read(RdBuf,RdAddr,Length);
		}	
		
		return result;
}

/** ************************************************************************
 * @brief   重新封装一个AFE数据写函数，用于异常后的二次写入
 **************************************************************************/
rt_err_t MTPWriteROM(rt_uint8_t WrAddr, rt_uint8_t Length, rt_uint8_t  *WrBuf)
{
		rt_bool_t result;
		rt_uint8_t i;
    
		for(i=0; i<Length; i++)
		{
				result = BMS_Byte_Write(WrBuf, WrAddr);
				if(result)
				{
					rt_thread_mdelay(40);
					result = BMS_Byte_Write(WrBuf, WrAddr);
					if(result)
					{
							break;
					}
				}
				
				WrAddr++;
				WrBuf++;
				rt_thread_mdelay(40);
		} 
		
    return result;  
}

/*******************************************************************************
Function:ResetAFE() 
Description:  Reset SH367309 IC, Send Data:0xEA, 0xC0, CRC
Input:	 NULL
Output: NULL
Others:软件复位
*******************************************************************************/
void Soft_ResetAFE(void)
{
	rt_uint8_t  WrBuf[2];

	WrBuf[0] = 0xC0;
	WrBuf[1] = 0xA5;
   
	if(BMS_Byte_Write(WrBuf, 0xEA))               //0xEA, 0xC0 CRC
	{
			BMS_Byte_Write(WrBuf, 0xEA);
	}
    
}

/*******************************************************************************
Function:EnableAFEWdtCadc()  
Description:使能CHG&DSG&PCHG输出，且使能WDT和CADC模块
Input: 	
Output: 
Others:
*******************************************************************************/
void EnableAFEWdtCadc(void)
{
	MTPConfVal |= 0x3C;//0b00111100;
	BMS_Byte_Write(&MTPConfVal,MTP_CONF);
}
/*******************************************************************************
清零WDG
*******************************************************************************/
void ClearAFEWdt(void)
{
	MTPConfVal |= 0x00;//0b00000000;
	BMS_Byte_Write(&MTPConfVal,MTP_BFLAG1);
}

void  AFE_Go_To_SLEEP(void)//？？
{
	MTPConfVal |= 0x02;//0b00000010;  
	BMS_Byte_Write(&MTPConfVal,MTP_CONF);
}

void  AFE_Go_To_IDLE(void)//？？
{
	MTPConfVal |= 0x02;//0b00000010;  
	BMS_Byte_Write(&MTPConfVal,MTP_CONF);
}


