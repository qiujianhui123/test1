/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Mishew       the first version
 */
#ifndef _UART_API_H_
#define _UART_API_H_


/* 接收数据最大字节数 */
#define UART1_RX_SIZE   32
#define UART2_RX_SIZE   32

/* UART ID */
#define UART1_ID        0
#define UART2_ID        1


typedef struct {
    rt_uint8_t rx_flag;
    rt_uint16_t rx_max_size;
    rt_uint16_t rx_length;
    rt_uint16_t rx_count;
    rt_tick_t rx_tick;
    rt_uint8_t *rx_buffer;
}uart_rx_data_t;

extern struct rt_semaphore uart_rx_sem[2];
extern uart_rx_data_t rev_data_t[2];

void uart_init(void);
void uart_send(rt_uint8_t uart_id, const void *buffer, rt_size_t size);
void uart1_send(const void *buffer, rt_size_t byte_size);
void uart2_send(const void *buffer, rt_size_t byte_size);

#endif /* _UART_API_H_ */
