
#include <rtthread.h>
#include <rtdevice.h>
#include "m24256_i2c.h"

/* Note
 * 1. 测试发现，在有自检的程序中和使用RTT时，初始时必须先有写操作，后续的第一次读取才能成功
 * 2. 在无自检程序中，写操作与读操作之间不加延时、无须初始时的写操作，均可正常读写；
 * 		而在有自检的工程或RTT中，必须在连续读写之间加延时才行
 * 3. 在无自检程序中，初始时加入写操作，反而在第一次读取时读取不到数据，与第一条矛盾
 *   原因待查。。。
 * */

/* Note
 * M24256有256kbit空间，每一页的大小Page Size =64，所以计算可得页的取值范围为：256*1024/8/64 = 512 页
 * */

/* 从机地址(不含读写位) */
#define EEPROM_DEV_ADDR (0x57)
/* 每页字节数 */
#define EEPROM_PAGE_SIZE (64)
/* 最大页数 */
#define EEPROM_MAX_PAGE (512)

static struct rt_i2c_bus_device *eeprom_i2c = RT_NULL;
rt_uint8_t test;

void I2C1_init(void)
{
    /* 发现设备 */
    eeprom_i2c = (struct rt_i2c_bus_device *)rt_device_find("i2c1");

    // 在初始化中加入一个写操作()
    // NOTE 初始化时暂按此处理
//    M24256_Byte_Write(0xFF, 0x7FFF); // EEPROM的最后一个字节写入0xFF
}

/** ************************************************************************
 * @brief   向指定地址address写入单个字节数据 WriteData
 **************************************************************************/
rt_err_t M24256_Byte_Write(rt_uint8_t WriteData, rt_uint16_t address)
{
    struct rt_i2c_msg msg;
    // 先发送地址高字节，再发低字节
    rt_uint8_t data[3] = {address >> 8, address & 0xFF, WriteData};

    // 发送设备地址、写状态、数据地址、数据
    msg.addr = EEPROM_DEV_ADDR;
    msg.flags = RT_I2C_WR;
    msg.buf = data;
    msg.len = 3;

    if (rt_i2c_transfer(eeprom_i2c, &msg, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

/** ************************************************************************
 * @brief   向指定页Page写入Size个字节数据 WriteData
 **************************************************************************/
rt_err_t M24256_Page_Write(rt_uint8_t *WriteBuf, rt_uint16_t Page, rt_uint16_t Size)
{
    struct rt_i2c_msg msg[2];

    rt_uint16_t usAddr = Page * EEPROM_PAGE_SIZE;

    // 先发送地址高字节，再发低字节
    rt_uint8_t data[2] = {usAddr >> 8, usAddr & 0xFF};

    // 发送设备地址、写状态、数据地址
    msg[0].addr = EEPROM_DEV_ADDR;
    msg[0].flags = RT_I2C_WR;
    msg[0].buf = data;
    msg[0].len = 2;

    // 发送数据
    msg[1].addr = EEPROM_DEV_ADDR;
    msg[1].flags = RT_I2C_WR | RT_I2C_NO_START;
    msg[1].buf = WriteBuf;
    msg[1].len = Size;

    if (rt_i2c_transfer(eeprom_i2c, msg, 2) == 2)
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
rt_uint8_t M24256_Byte_Read(rt_uint16_t address)
{
    struct rt_i2c_msg msg[2];
    rt_uint8_t ReadData = 0;

    // 先发送地址高字节，再发低字节
    rt_uint8_t data[2] = {address >> 8, address & 0xFF};

    // 发送设备地址、写状态、数据地址
    msg[0].addr = EEPROM_DEV_ADDR;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].buf = data;
    msg[0].len = 2;

    // 发送设备地址、读状态
    msg[1].addr = EEPROM_DEV_ADDR;
    msg[1].flags = RT_I2C_RD;
    msg[1].buf = &ReadData;
    msg[1].len = 1;

    if (rt_i2c_transfer(eeprom_i2c, msg, 2) == 2)
    {
        return ReadData;
    }
    else
    {
        return 0;
    }
}

/** ************************************************************************
 * @brief   从指定页读取多个字节数据 ReadBuf
 **************************************************************************/
rt_err_t M24256_Page_Read(rt_uint8_t *ReadBuf, rt_uint16_t Page, rt_uint16_t Size)
{
    struct rt_i2c_msg msg[2];

    rt_uint16_t usAddr = Page * EEPROM_PAGE_SIZE;

    // 先发送地址高字节，再发低字节
    rt_uint8_t data[2] = {usAddr >> 8, usAddr & 0xFF};

    // 发送设备地址、写状态、数据地址
    msg[0].addr = EEPROM_DEV_ADDR;
    msg[0].flags = RT_I2C_WR | RT_I2C_NO_STOP;
    msg[0].buf = data;
    msg[0].len = 2;

    // 发送设备地址、读状态
    msg[1].addr = EEPROM_DEV_ADDR;
    msg[1].flags = RT_I2C_RD;
    msg[1].buf = ReadBuf;
    msg[1].len = Size;

    if (rt_i2c_transfer(eeprom_i2c, msg, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
