
#include <rtthread.h>
#include "MCP79410.h"

static struct rt_i2c_bus_device *MCP79410_i2c = RT_NULL;

/* 从机地址(不含读写位) */
#define EEPROM_DEV_ADDR (0x57)

rt_uint8_t sec=0, min=0, hr=0,day=0, dat=0, mon=0, yr=0 ; // time variables, BCD format

rt_uint16_t sec_10,sec_1,f_sec;
rt_uint16_t min_10,min_1,f_min;
rt_uint16_t hour_12_24,hour_AM_PM,hour_10,hour_1,f_hour;
rt_uint16_t date_10,date_1,f_date;
rt_uint16_t month_10,month_1,f_month;
rt_uint16_t year_10,year_1,f_year;
rt_uint16_t day_1;
 
void I2C3_init(void)
{
    /* 发现设备 */
    MCP79410_i2c = (struct rt_i2c_bus_device *)rt_device_find("i2c3");
}

/** ************************************************************************
 * @brief   向指定地址address写入单个字节数据 WriteData
 **************************************************************************/
rt_err_t MCP79410_Byte_Write(rt_uint8_t WriteData, rt_uint8_t address)
{
    struct rt_i2c_msg msg;
    // 先发送RTC地址、寄存器地址、数据
    rt_uint8_t data[2] = {address,WriteData};

    // 发送设备地址、写状态、数据地址、数据
    msg.addr = ADDR_RTCC;
    msg.flags = RT_I2C_WR;
    msg.buf = data;
    msg.len = 2;

    if (rt_i2c_transfer(MCP79410_i2c, &msg, 1) == 1)
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
rt_uint8_t MCP79410_Byte_Read(rt_uint8_t address)
{
    struct rt_i2c_msg msg[2];
    rt_uint8_t ReadData = 0;

    // 先发送地址高字节，再发低字节
    rt_uint8_t data[1] = {address};

    // 发送设备地址、写状态、数据地址
    msg[0].addr = ADDR_RTCC;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].buf = data;
    msg[0].len = 1;

    // 发送设备地址、读状态
    msg[1].addr = ADDR_RTCC;
    msg[1].flags = RT_I2C_RD;
    msg[1].buf = &ReadData;
    msg[1].len = 1;

    if (rt_i2c_transfer(MCP79410_i2c, msg, 2) == 2)
    {
        return ReadData;
    }
    else
    {
        return 0;
    }
}


void ini_time(void) 
{
		 MCP79410_Byte_Write(0x23,ADDR_YEAR);           
		 MCP79410_Byte_Write(0x02,ADDR_MNTH);      
		 MCP79410_Byte_Write(0x24,ADDR_DATE);     
		 MCP79410_Byte_Write(0x21,ADDR_HOUR);     
		 MCP79410_Byte_Write(0x02,ADDR_MIN);      
		 MCP79410_Byte_Write(0x00|START_32KHZ,ADDR_SEC); 
		 MCP79410_Byte_Write(0x05|VBATEN,ADDR_DAY);//DAY of WEEK 
}

void rtcc_get_data(void)
{
		yr  = MCP79410_Byte_Read(ADDR_YEAR) ;
		mon = MCP79410_Byte_Read(ADDR_MNTH) ;
		dat = MCP79410_Byte_Read(ADDR_DATE) ;
		hr  = MCP79410_Byte_Read(ADDR_HOUR) ;
		min = MCP79410_Byte_Read(ADDR_MIN)  ;
		sec = MCP79410_Byte_Read(ADDR_SEC)  ;
		day = MCP79410_Byte_Read(ADDR_DAY)  ;	
		/************sec******************/
		sec_10=((sec & 0x70)>>4)*10;
		sec_1=(sec & 0x0f);
		f_sec=sec_10+sec_1;
		/************min******************/
		min_10=((min & 0x70)>>4)*10;
		min_1=(min & 0x0f);
		f_min=min_10+min_1;
		/************hour****************/
//		hour_12_24=((hr & 0x40)>>6);
//		hour_AM_PM=((hr & 0x20)>>5);
//		hour_10=((hr & 0x10)>>4)*10;
		hour_10 = ((hr & 0x30) >> 4) * 10;
		hour_1=(hr & 0x0f);
		f_hour=hour_10+hour_1;
		/***********date***************/
		date_10=((dat & 0x30)>>4)*10;
		date_1=(dat & 0x0f);
		f_date=date_10+date_1;
		/***********mon****************/
		month_10=((mon & 0x10)>>4)*10;
		month_1=(mon & 0x0f);
		f_month=month_10+month_1;
		/***********year***************/
		year_10=((yr & 0xf0)>>4)*10;
		year_1=(yr & 0x0f);
		f_year=year_10+year_1;
		/************day***************/
		day_1=(day & 0x07);

}
 
 
 
