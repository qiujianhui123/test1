#ifndef _M24256_I2C_H_
#define _M24256_I2C_H_

#include <rtdevice.h>

void I2C1_init(void);
rt_err_t M24256_Byte_Write(rt_uint8_t WriteData,rt_uint16_t address);
rt_err_t M24256_Page_Write(rt_uint8_t *WriteBuf, rt_uint16_t Page, rt_uint16_t Size);
rt_uint8_t M24256_Byte_Read(rt_uint16_t address);
rt_err_t M24256_Page_Read(rt_uint8_t *ReadBuf, rt_uint16_t Page, rt_uint16_t Size);


#endif

