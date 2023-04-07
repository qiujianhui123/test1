/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Mishew       the first version
 */

#include <rtthread.h>
#include "uart_api.h"
#include "config_io.h"

#ifdef RT_USING_SERIAL

uart_rx_data_t rev_data_t[2];

/* 串口设备句柄 */
static rt_device_t uart_dev[2];
/* 接收数据缓存 */
static rt_uint8_t uart1_rx_buffer[UART1_RX_SIZE] = {0};
static rt_uint8_t uart2_rx_buffer[UART2_RX_SIZE] = {0};

static rt_err_t uart_rx_callback(rt_device_t dev, rt_size_t size);
static void uart_buffer_init(rt_device_t dev,rt_uint8_t *buffer,rt_uint16_t size);
static rt_uint8_t uart1_init(void);
static rt_uint8_t uart2_init(void);

extern void uart_rx_timeout(void);

/** ************************************************************************
 * @brief   UART接收超时
 * @return  None
 * @note    释放对应的信号量
 **************************************************************************/
void uart_rx_timeout(void)
{
    rt_uint8_t i;
    for(i=0;i<2;i++)
    {
        if((RT_TRUE == rev_data_t[i].rx_flag) && ((rt_tick_get() - rev_data_t[i].rx_tick) > 5))
        {
            rev_data_t[i].rx_flag = RT_FALSE;
            rev_data_t[i].rx_length = rev_data_t[i].rx_count;
            rev_data_t[i].rx_count = 0;

            rt_sem_release(&uart_rx_sem[i]);
        }
    }

}


/* 接收中断回调函数 */
static rt_err_t uart_rx_callback(rt_device_t dev, rt_size_t size)
{
    /*读取接收数据，当前配置下没接收一个字节产生一次中断，一次读取1个字节*/
    if(rev_data_t[dev->device_id].rx_count < rev_data_t[dev->device_id].rx_max_size)
    {
        rev_data_t[dev->device_id].rx_tick = rt_tick_get();
        rt_device_read(dev, -1, rev_data_t[dev->device_id].rx_buffer + (rev_data_t[dev->device_id].rx_count++), 1);
        rev_data_t[dev->device_id].rx_flag = RT_TRUE;
    }

    return RT_EOK;
}



static void uart_buffer_init(rt_device_t dev,rt_uint8_t *buffer,rt_uint16_t size)
{
    rev_data_t[dev->device_id].rx_buffer = buffer;
    rev_data_t[dev->device_id].rx_max_size = size;
}


/****************************UART初始化**********************************/
void uart_init(void)
{
    uart1_init();
    uart2_init();
}


/****************************UART1**********************************/
static rt_uint8_t uart1_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 查找系统中的串口设备 */
    uart_dev[0] = rt_device_find("uart1");
    if (!uart_dev[0])
    {
        return RT_ERROR;
    }

    uart_dev[0]->device_id = UART1_ID;

    /* 修改串口配置参数 */
    struct serial_configure uart_config = RT_SERIAL_CONFIG_DEFAULT;
    uart_config.baud_rate = BAUD_RATE_115200;      //修改波特率为 115200
    uart_config.data_bits = DATA_BITS_8;           //数据位 8
    uart_config.stop_bits = STOP_BITS_1;           //停止位 1
    uart_config.bufsz     = 32;                    //修改缓冲区 buff size 为 32
    uart_config.parity    = PARITY_NONE;           //无奇偶校验位

    /* 通过控制接口传入命令控制字和控制参数 */
    rt_device_control(uart_dev[0], RT_DEVICE_CTRL_CONFIG, &uart_config);

    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart_dev[0], RT_DEVICE_FLAG_INT_RX);

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(uart_dev[0], uart_rx_callback);

    /* 接收缓存初始化 */
    uart_buffer_init(uart_dev[0],uart1_rx_buffer,UART1_RX_SIZE);


    return ret;
}


/****************************UART2**********************************/
static rt_uint8_t uart2_init(void)
{
    rt_err_t ret = RT_EOK;

    /* 查找系统中的串口设备 */
    uart_dev[1] = rt_device_find("uart2");
    if (!uart_dev[1])
    {
        return RT_ERROR;
    }

    uart_dev[1]->device_id = UART2_ID;

    /* 修改串口配置参数 */
    struct serial_configure uart_config = RT_SERIAL_CONFIG_DEFAULT;
    uart_config.baud_rate = BAUD_RATE_115200;      //修改波特率为 115200
    uart_config.data_bits = DATA_BITS_8;           //数据位 8
    uart_config.stop_bits = STOP_BITS_1;           //停止位 1
    uart_config.bufsz     = 32;                    //修改缓冲区 buff size 为 32
    uart_config.parity    = PARITY_NONE;           //无奇偶校验位

    /* 通过控制接口传入命令控制字和控制参数 */
    rt_device_control(uart_dev[1], RT_DEVICE_CTRL_CONFIG, &uart_config);

    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(uart_dev[1], RT_DEVICE_FLAG_INT_RX);

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(uart_dev[1], uart_rx_callback);

    /* 接收缓存初始化 */
    uart_buffer_init(uart_dev[1],uart2_rx_buffer,UART2_RX_SIZE);

    return ret;
}


/* UART发送字节 */
//从第一个元素开始发送
//buffer元素为 字节 时，byte_size为buffer的长度
//buffer元素为 字 时，byte_size为buffer的长度的两倍，1字=2字节，发送时先发低字节，再发高字节
void uart_send(rt_uint8_t uart_id, const void *buffer, rt_size_t byte_size)
{
    /* 数据处理 */
    rt_device_write(uart_dev[uart_id],0,buffer,byte_size);
}

void uart1_send(const void *buffer, rt_size_t byte_size)
{
    /* 数据处理 */

    RS485_EN1_TX();
    rt_device_write(uart_dev[0],0,buffer,byte_size);
    RS485_EN1_RX();
}

void uart2_send(const void *buffer, rt_size_t byte_size)
{
    /* 数据处理 */
    RS485_EN2_TX();
    rt_device_write(uart_dev[1],0,buffer,byte_size);
    RS485_EN2_RX();
}

#endif
