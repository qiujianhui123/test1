#ifndef _BMS_I2C_H_
#define _BMS_I2C_H_

#include <rtdevice.h>

void I2C2_init(void);
rt_err_t BMS_Byte_Write(rt_uint8_t *WriteData,rt_uint8_t address);
rt_err_t BMS_Page_Write(rt_uint8_t *WriteBuf, rt_uint16_t Page, rt_uint16_t Size);
rt_uint8_t BMS_Byte_Read(rt_uint16_t address);
rt_err_t BMS_Page_Read(rt_uint8_t *ReadBuf, rt_uint8_t usAddr, rt_uint8_t Size);

rt_err_t MTPRead(rt_uint8_t RdAddr, rt_uint8_t Length, rt_uint8_t  *RdBuf);
rt_err_t MTPWriteROM(rt_uint8_t WrAddr, rt_uint8_t Length, rt_uint8_t  *WrBuf);
void Soft_ResetAFE(void);
void EnableAFEWdtCadc(void);
#endif

