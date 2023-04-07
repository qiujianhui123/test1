/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-30     Mishew       the first version
 */
#ifndef _TASK_H_
#define _TASK_H_

//任务栈大小，地址必须是int的整数倍
#define STACK_SIZE       (512)
#define STACK_SIZE_1024  (1024)
#define STACK_SIZE_1536  (1536)
#define STACK_SIZE_2048  (2048)
#define STACK_SIZE_3072  (3072)
#define STACK_SIZE_4096  (4096)

//任务优先级
#define PRIO(x)     (x)
//任务时间片(每个tick表示1ms)
#define TICK(x)     (x)

struct rt_thread_task
{
    const char       *name;
    void (*entry)(void *parameter);
    void             *parameter;
    void             *stack_start;
    rt_uint32_t       stack_size;
    rt_uint8_t        priority;
    rt_uint32_t       tick;
};

/*********************任务配置************************/
//最大任务数
#define MAX_APP_TASK_NUM    (6)
//任务入口
extern void task_led_entry(void *para);
extern void task_cycle_entry(void *para);
extern void task_uart2_entry(void *para);
extern void task_can1_entry(void *para);
extern void task_control_entry(void *para);
extern void task_readdata_entry(void *para);

//任务栈大小
static char task_cycle_stack[STACK_SIZE_1024];
static char task_led_stack[STACK_SIZE_1024];
static char task_uart2_stack[STACK_SIZE_4096];
static char task_can1_stack[STACK_SIZE_1024];
static char task_control_stack[STACK_SIZE_1024];
static char task_readdata_stack[STACK_SIZE_1024];

/* uart2接收消息的信号量 */
struct rt_semaphore uart_rx_sem[2];

/* can1接收消息的信号量 */
struct rt_semaphore can1_rx_sem;

//声明线程
static struct rt_thread app_task[MAX_APP_TASK_NUM];
//任务列表
const struct rt_thread_task app_task_table[MAX_APP_TASK_NUM] = {
{"t_cycle",  &task_cycle_entry,  RT_NULL, &task_cycle_stack[0],  sizeof(task_cycle_stack),   PRIO(22),    TICK(10)},
{"t_led",    &task_led_entry,    RT_NULL, &task_led_stack[0],    sizeof(task_led_stack),     PRIO(21),    TICK(10)},
{"t_uart2",  &task_uart2_entry,  RT_NULL, &task_uart2_stack[0],  sizeof(task_uart2_stack),   PRIO(25),    TICK(10)},
{"t_can1",  &task_can1_entry,  RT_NULL, &task_can1_stack[0],  sizeof(task_can1_stack),   PRIO(23),    TICK(10)},
{"t_control",  &task_control_entry,  RT_NULL, &task_control_stack[0],  sizeof(task_control_stack),   PRIO(18),    TICK(10)},
{"t_readdata",  &task_readdata_entry,  RT_NULL, &task_readdata_stack[0],  sizeof(task_readdata_stack),   PRIO(19),    TICK(10)},

};



#endif /* _TASK_H_ */
