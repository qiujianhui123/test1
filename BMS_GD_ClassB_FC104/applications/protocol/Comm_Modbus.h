/** ************************************************************************
 * @copyright Copyright (C), 2017, Qingdao xxx Co. Ltd.
 * @file    Comm_Modbus.h
 * @brief   Modbus协议处理
 * @author  Mishew
 * @date    2017/08/04
 * @version 2.00.00
 **************************************************************************/

#ifndef COMM_SCI_H_
#define COMM_SCI_H_


uint16_t ModbusHandle(uint16_t *pRegValue,volatile uint16_t *pRxBuffer,volatile uint16_t *pTxBuffer,uint16_t RxLength);
uint16_t ModbusCRC(volatile uint16_t *rxBuffer,uint16_t rxBufferLength);
uint16_t ModbusProcess(volatile uint16_t *rxBuffer,volatile uint16_t *txBuffer,volatile uint16_t *pRegValue);
uint16_t nor_flash_SaveData(Uint32 offset, volatile uint16_t *data, Uint32 size);

#endif /* COMM_SCI_H_ */
