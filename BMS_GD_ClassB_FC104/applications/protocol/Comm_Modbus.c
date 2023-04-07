/** ************************************************************************
 * @copyright Copyright (C), 2022, Qingdao xxx Co. Ltd.
 * @file    Comm_Modbus.c
 * @brief   Modbus协议处理
 * @author  Mishew
 * @date    2022/03/15
 * @version 1.0
 **************************************************************************/


#include "Comm_Modbus.h"


uint16_t ModuleReplyFlag = 0;//模块回复标志
uint16_t ReadDevNo = 0;//读取指定的从机

uint16_t Dev_Num_Count = 0;//统计12位设备编号是否全为0
uint16_t Distrbute_Fault[2] = {0,0};//配电模块故障信息

static uint16_t UpdateDataSaveFlag;//存储升级文件标志

extern volatile union DEV_STATUS_REG DevStatus;
extern uint32_t CPU_ID;
extern uint16_t CurDevNum;
extern uint16_t Dev_PowerOn_Comm_Flag;
extern EXPORT_READ_DATA Export_Read_Data;

uint16_t ModbusHandle(uint16_t *pRegValue,volatile uint16_t *pRxBuffer,volatile uint16_t *pTxBuffer,uint16_t RxLength)
{
    uint16_t frameLength = 0;
    uint16_t crc_flag;
    crc_flag = ModbusCRC(pRxBuffer,RxLength);

    if(crc_flag == TRUE)
    {
        if((ReadDevNo == CurDevNum) || (ReadDevNo == 0x0) \
           || (DevStatus.bit.AutoTest_Enable == TRUE))
        {
            ModuleReplyFlag = TRUE;
        }
        else
        {
            ModuleReplyFlag = FALSE;
        }

        //数据刷新，只有选中的模块可回复
        //广播时，所有模块都刷新；选定模块时，只有选定模块刷新
        if((ReadDevNo == CurDevNum) || (ReadDevNo == 0xFF) || (ReadDevNo == 0x0) \
           || (DevStatus.bit.AutoTest_Enable == TRUE))
        {
            frameLength = ModbusProcess(pRxBuffer,pTxBuffer,pRegValue);
        }
    }

    return frameLength;

}


/** ************************************************************************
 * @brief   接收数据校验
 * @return  None
 **************************************************************************/
uint16_t ModbusCRC(volatile uint16_t *rxBuffer,uint16_t rxBufferLength)
{
    uint16_t afnTemp;
    uint16_t rx_CRC = 0xFF;

    afnTemp = rxBuffer[1]; //功能码
    rx_CRC = CRC16_ByteArray(rxBuffer,rxBufferLength,0); //计算校验位

    if (rx_CRC == 0)  //检验通过，数据接收正确
    {
        ReadDevNo = rxBuffer[0]; //从机号

        if(afnTemp == 0x8)
        {
            UpdateDataSaveFlag = TRUE;
        }
        return TRUE;
    }
    else
    {
        if(afnTemp == 0x8)
        {
            //接收文件校验失败，也回复状态
            UpdateDataSaveFlag = FALSE;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}

/** ************************************************************************
 * @brief   Modbus协议回复
 * @return  None
 **************************************************************************/
uint16_t ModbusProcess(volatile uint16_t *rxBuffer,volatile uint16_t *txBuffer,volatile uint16_t *pRegValue)
{
    uint16_t afn;
    uint16_t registerNum;
    uint16_t registerByteNum;
    uint16_t i,j;
    uint16_t registerStartAddr;
    volatile uint16_t *pRegVar;
    uint16_t regValue;
    uint16_t crc16;
    uint16_t frameLength = 0;
    uint16_t data_temp;

    afn = rxBuffer[1] & 0xFF; //功能码
    registerStartAddr = rxBuffer[2] * 256 + rxBuffer[3]; //起始地址
    registerNum = rxBuffer[4] * 256 + rxBuffer[5]; //寄存器数量或寄存器值
    pRegVar = pRegValue + registerStartAddr;
    switch (afn)
    {
        case 0x03:
        case 0x04:
            /// registerNum是寄存器数量，每次最大读取数据为60个(0x3C)
            if((TRUE == ModuleReplyFlag) && (registerNum < 0x3C) \
                    && (registerStartAddr + registerNum <= REG_MAX_SIZE))
            {
                txBuffer[0] = rxBuffer[0];
                txBuffer[1] = rxBuffer[1];
                registerByteNum = registerNum * 2;//单字节
                txBuffer[2] = registerByteNum;

                for (i = 0; i < registerNum; i++)
                {
                    regValue = *(pRegVar++);
                    txBuffer[3+2*i] = regValue >> 8;//高字节
                    txBuffer[4+2*i] = regValue & 0xFF;//低字节
                }

                //蓝牙mac地址
                //未设置出厂编号或编号设置不正确，则发送芯片UID作为蓝牙mac地址
                //重设设备编号后，使用设备编号作为蓝牙mac地址
                if(registerStartAddr == 534)
                {
                    Dev_Num_Count = 0;//统计12位设备编号是否全为0

                    //出厂编号
                    for(i = 0;i < 12;i++)
                    {
                        if(*(pRegValue + 534 + i) == 0)
                        {
                            Dev_Num_Count++;
                        }

                        //设备编号12位地址全0或者其中一个大于9，使用DSP芯片UID作为蓝牙mac地址
                        if((*(pRegValue + 534 + i) > 9) || (Dev_Num_Count == 12)) //534-545
                        {
                            //UID只有4个字节，填充mac后8位，mac前4位写0
                            for(j = 0;j < 8;j++)
                            {
                                txBuffer[3+j] = 0;
                            }

                            for(j = 0;j < 8;j++)
                            {
                                txBuffer[26 - 2*j] = (CPU_ID >> (4 * j)) & 0xF;
                            }
                            break;
                        }
                    }
                }

                //低字节在前,CRC先发低字节
                crc16 = CRC16_ByteArray(txBuffer,registerByteNum + 3,0);
                txBuffer[registerByteNum+3] = crc16 & 0xFF;
                txBuffer[registerByteNum+4] = crc16>>8;

                frameLength = registerByteNum + 5;
            }
            break;
        case 0x06:
            ///registerNum 在功能码为0x06时是寄存器值，非寄存器数量
            if(registerStartAddr < REG_MAX_SIZE)
            {
                *pRegVar = registerNum;

                //选中模块回复数据
                if(TRUE == ModuleReplyFlag)
                {
                    for(uint16_t k=0;k<8;k++)
                    {
                        txBuffer[k] = rxBuffer[k];
                    }
                    frameLength = 8;
                }
            }
            break;
        case 0x07:
            //回复上位机的监控数据(特定部分数据)
            if((TRUE == ModuleReplyFlag) && (registerStartAddr == 0xFFFF))
            {
                j = 0;
                txBuffer[j++] = rxBuffer[0];
                txBuffer[j++] = rxBuffer[1];
                txBuffer[j++] = 0x4C; //单字节
                //运行模式
                data_temp = *(pRegValue + 531);
                txBuffer[j++] = data_temp >> 8;
                txBuffer[j++] = data_temp & 0x00FF;

                //运行状态
                data_temp = *(pRegValue + 502);
                txBuffer[j++] = data_temp >> 8;
                txBuffer[j++] = data_temp & 0x00FF;
                //故障码
                for (i = 0; i < 8;i++)
                {
                    data_temp = *(pRegValue + 505 + i);
                    txBuffer[j++] = data_temp >> 8;
                    txBuffer[j++] = data_temp & 0x00FF;
                }

                if(DEV_MODE_AC2DC_CURRENT == ParasToCPU2.Compensate_Mode)
                {
                    //整流侧读逆变电流
                    for (i = 0; i < 3; i++)
                    {
                        data_temp = *(pRegValue + 654 + i);
                        txBuffer[j++] = data_temp >> 8;
                        txBuffer[j++] = data_temp & 0x00FF;
                    }
                }
                else
                {
                    //逆变侧读装置电流
                    for (i = 0; i < 3; i++)
                    {
                        data_temp = *(pRegValue + 650 + i);
                        txBuffer[j++] = data_temp >> 8;
                        txBuffer[j++] = data_temp & 0x00FF;
                    }
                }

                data_temp = *(pRegValue + 653); //装置N线电流
                txBuffer[j++] = data_temp >> 8;
                txBuffer[j++] = data_temp & 0x00FF;

                //电网电压
                for (i = 0; i < 3; i++)
                {
                    data_temp = *(pRegValue + 625 + i);
                    txBuffer[j++] = data_temp >> 8;
                    txBuffer[j++] = data_temp & 0x00FF;
                }

                //直流电压
                for (i = 0; i < 3; i++)
                {
                    data_temp = *(pRegValue + 622 + i);
                    txBuffer[j++] = data_temp >> 8;
                    txBuffer[j++] = data_temp & 0x00FF;
                }

                //温度
                for (i = 0; i < 9; i++)
                {
                    data_temp = *(pRegValue + 584 + i);
                    txBuffer[j++] = data_temp >> 8;
                    txBuffer[j++] = data_temp & 0x00FF;
                }

                //风扇转速
                for (i = 0; i < 9; i++)
                {
                    data_temp = *(pRegValue + 593 + i);
                    txBuffer[j++] = data_temp >> 8;
                    txBuffer[j++] = data_temp & 0x00FF;
                }

                //低字节在前,CRC先发低字节
                crc16 = CRC16_ByteArray(txBuffer,j,0);
                txBuffer[j++] = crc16 & 0xFF;
                txBuffer[j++] = crc16>>8;
                frameLength = j;
            }
            break;
        case 0x10:
            ///写多个寄存器参数，registerNum 在功能码为0x10时是寄存器数量
            if ((registerStartAddr + registerNum <= REG_MAX_SIZE) && ((registerNum<<1) == rxBuffer[6]))
            {
                for(i=0;i<registerNum;i++)
                {
                    *(pRegVar+i) = rxBuffer[7+2*i] * 256 + rxBuffer[8+2*i];
                }

                if(TRUE == ModuleReplyFlag)
                {
                    for(uint16_t k=0;k<6;k++)
                    {
                        txBuffer[k] = rxBuffer[k];
                    }

                    crc16 = CRC16_ByteArray(txBuffer,6,0);
                    txBuffer[6] = crc16 & 0xFF;
                    txBuffer[7] = crc16>>8;
                    frameLength = 8;
                }
            }
            break;
        case 0x08:
            //保存数据
            //返回数据接收状态：当前帧数据已存储
            if(UpdateDataSaveFlag == TRUE)
            {
                uint16_t SaveFile_Flag = 0;
                uint16_t ResPackage;
                uint16_t CurPackage;
                uint16_t CurPackgeByte;

                static uint32_t AddressOffset = 0;
                static uint16_t RevPackageCount = 0;
                static uint16_t TotalPackage = 0;//文件总数据包数
                static uint32_t TotalSize = 0;//文件总字节数

                CurPackage = (rxBuffer[2]<<8) + rxBuffer[3];//当前包数
                CurPackgeByte = (rxBuffer[4]<<8) + rxBuffer[5];//当前包字节数
                ResPackage = (rxBuffer[6]<<8) + rxBuffer[7];//剩余包数

                for(uint16_t k=0;k<4;k++)
                {
                    txBuffer[k] = rxBuffer[k];
                }

                if (CurPackage == 0)
                {
                    RevPackageCount = 0;
                    if(CurPackgeByte != 32)
                    {
                        SaveFile_Flag = 3; //数据包格式异常
                    }
                }
                //接收的当前包号与成功接收包数相同，表示数据包有效，保证数据包按顺序收发
                if(CurPackage == RevPackageCount)
                {
                    //当前包号为0时，表示文件信息
                    if((CurPackage == 0) && (CurPackgeByte == 32))
                    {
                        TotalPackage = (rxBuffer[6]<<8) + rxBuffer[7];//剩余包数

                        //程序大小超出Flash存储范围，无法升级
                        //共12扇区，可存储字节数为0x1000 * 2 * 12 = 0x18000 = 98304字节
                        TotalSize = (rxBuffer[24] + (rxBuffer[25]<<8));
                        TotalSize = (TotalSize<<16) + (rxBuffer[22] + (rxBuffer[23]<<8)) + 48;

                        if(TotalSize > MAX_UPGRADE_SIZE)
                        {
                            SaveFile_Flag = 2;//文件大小超出范围
                            RevPackageCount = 0;
                        }
                        else
                        {
                            //文件大小未超出范围，可以接收并存储，擦除Flash,准备存储数据
                            Erase_Flash_Block(SECTOR_BEGIN,SECTOR_END);

                            AddressOffset = SPI_FLASH_BLOCK_SIZE * SECTOR_BEGIN;
                            SaveFile_Flag = 0;
                        }
                    }

                    if((CurPackage + ResPackage) != TotalPackage)
                    {
                        SaveFile_Flag = 3;//数据包格式异常
                        RevPackageCount = 0;
                        AddressOffset = SPI_FLASH_BLOCK_SIZE * SECTOR_BEGIN;
                    }

                    if(SaveFile_Flag < 2)
                    {
                        RevPackageCount++;//成功接收数据包计数
                        //写入Flash
                        SaveFile_Flag = nor_flash_SaveData(AddressOffset, rxBuffer, CurPackgeByte);

                        AddressOffset += CurPackgeByte;
                    }

                    if(SaveFile_Flag == 0)
                    {
                        //存储失败
                        AddressOffset = SPI_FLASH_BLOCK_SIZE * SECTOR_BEGIN;
                        RevPackageCount = 0;
                    }

                    if((SaveFile_Flag == 1) && (ResPackage == 0) && (TotalPackage > 0))
                    {
                        RevPackageCount = 0;
                        TotalPackage = 0;
                        //复位重启
                        DevStatus.bit.Cpu_Reset_Flag = TRUE;

                    }
                }
                else
                {
                    //数据包未按顺序成功接收，防止随意发送数据
                    AddressOffset = SPI_FLASH_BLOCK_SIZE * SECTOR_BEGIN;
                    RevPackageCount = 0;
                    SaveFile_Flag = 4;//回复数据包顺序异常
                }

                //0xF0:接收成功，保存失败
                //0xF1:接收成功，保存成功 OK
                //0xF2:程序大小超出范围
                //0xF3:接收成功，数据包格式异常
                //0xF4:接收成功，数据包顺序异常

                txBuffer[5] = 0xF0 + SaveFile_Flag;

                if(1 != SaveFile_Flag)
                {
                    //文件接收或存储失败，擦除扇区，防止重启后升级错误文件导致系统异常
                    Erase_Flash_Block(SECTOR_BEGIN,SECTOR_END);
                }
            }
            else
            {
                //0xFA:接收失败
                txBuffer[5] = 0xFA;
            }

            if(TRUE == ModuleReplyFlag)
            {
                txBuffer[4] = 0;
                crc16 = CRC16_ByteArray(txBuffer,6,0);
                txBuffer[6] = crc16 & 0xFF;
                txBuffer[7] = crc16>>8;

                frameLength = 8;
                UpdateDataSaveFlag = FALSE;
            }

            break;
        case 0x09:
            //回复上位机：设备信息
            if((TRUE == ModuleReplyFlag) && (TRUE == Dev_PowerOn_Comm_Flag))
            {
                for(uint16_t k=0;k<4;k++)
                {
                    txBuffer[k] = rxBuffer[k];
                }

                if(registerStartAddr == 0xFEFE)
                {
                    //回复设备最大接收字节数
                    txBuffer[4] = PACKAGE_DATA_READ>>8;//高字节
                    txBuffer[5] = PACKAGE_DATA_READ & 0xFF;//低字节
                }

                if(registerStartAddr == 0xFDFD)
                {
                    //回复设备最大存储容量
                    //默认大小不会超过0x00FFFFFF,高字节保留为功能识别0xFD
                    txBuffer[3] = (MAX_UPGRADE_SIZE>>16) & 0xFF;
                    txBuffer[4] = (MAX_UPGRADE_SIZE>>8) & 0xFF;
                    txBuffer[5] = MAX_UPGRADE_SIZE & 0xFF;//低字节
                }

                if(registerStartAddr == 0xFCFC)
                {
                    //回复设备UID
                    txBuffer[2] = (CPU_ID>>24) & 0xFF;
                    txBuffer[3] = (CPU_ID>>16) & 0xFF;
                    txBuffer[4] = (CPU_ID>>8) & 0xFF;
                    txBuffer[5] = CPU_ID & 0xFF;//低字节
                }

                if(registerStartAddr == 0xFBFB)
                {
                    //回复当前程序版本号
                    txBuffer[2] = 0xAA;
                    txBuffer[3] = DEV_GEN_VERSION;
                    txBuffer[4] = DSP_VERSION/100;
                    txBuffer[5] = DSP_VERSION%100;
                }

                //整机测试
                if(registerStartAddr == 0xFAFA)
                {
                    txBuffer[4] = 0;
                    txBuffer[5] = (CAN_Comm_Test > CAN_COMM_TEST_NUM)?1:0;
                }

                //低字节在前,CRC先发低字节
                crc16 = CRC16_ByteArray(txBuffer,6,0);
                txBuffer[6] = crc16 & 0xFF;
                txBuffer[7] = crc16 >> 8;
                frameLength = 8;
            }
            break;

        case 0x23: //配电读功率单元数据
            /// registerNum是寄存器数量，根据发送空间长度限制长度
            if((TRUE == ModuleReplyFlag) && ((registerNum << 1) <= (MAX_REV_SIZE - 6))\
                && (registerStartAddr + registerNum <= REG_MAX_SIZE))
            {
                //配电模块故障信息
                Distrbute_Fault[0] = (rxBuffer[6] << 8) + rxBuffer[7];
                Distrbute_Fault[1] = (rxBuffer[8] << 8) + rxBuffer[9];

                txBuffer[0] = rxBuffer[0];
                txBuffer[1] = rxBuffer[1];
                registerByteNum = registerNum * 2;//单字节
                txBuffer[2] = registerByteNum >> 8;
                txBuffer[3] = registerByteNum & 0x00FF;

                for (i = 0; i < registerNum; i++)
                {
                    regValue = *(pRegVar++);
                    txBuffer[4+2*i] = regValue >> 8;//高字节
                    txBuffer[5+2*i] = regValue & 0xFF;//低字节
                }

                //低字节在前,CRC先发低字节
                crc16 = CRC16_ByteArray(txBuffer,registerByteNum + 4,0);
                txBuffer[registerByteNum+4] = crc16 & 0xFF;
                txBuffer[registerByteNum+5] = crc16>>8;

                frameLength = registerByteNum + 6;
            }
            break;

        case 0x30://配电写功率单元数据
            ///写多个寄存器参数，registerNum 在功能码为0x30时是寄存器数量
            if ((registerStartAddr + registerNum <= REG_MAX_SIZE) && ((registerNum<<1) == (rxBuffer[6]*256 + rxBuffer[7])))
            {
                for(i=0;i<registerNum;i++)
                {
                    *(pRegVar+i) = rxBuffer[8+2*i] * 256 + rxBuffer[9+2*i];
                }

                if(TRUE == ModuleReplyFlag)
                {
                    for(uint16_t k=0;k<6;k++)
                    {
                        txBuffer[k] = rxBuffer[k];
                    }

                    crc16 = CRC16_ByteArray(txBuffer,6,0);
                    txBuffer[6] = crc16 & 0xFF;
                    txBuffer[7] = crc16>>8;
                    frameLength = 8;
                }
            }
            break;

        case 0x14:
            //配电读功率单元数据故障录波数据
            if((TRUE == ModuleReplyFlag) && (rxBuffer[3] >= 1))
            {
                Export_Read_Data.Num_Order = rxBuffer[3]; //次数
                Export_Read_Data.FunCrtl = (EXPORT_READ_STATE)rxBuffer[4]; //读取控制
                if(rxBuffer[4] == 0)
                {
                    Export_Read_Data.PackageBytes = (rxBuffer[5] << 8) + rxBuffer[6]; //每包字节数
                }

                txBuffer[0] = rxBuffer[0];
                txBuffer[1] = rxBuffer[1];

                frameLength = DISTRUTE_EXPORT_FAULT_FLAG; //故障录波导出标志
            }
            break;

        case 0x15:
            //上位机读功率单元数据事件数据、录波数据
            if(TRUE == ModuleReplyFlag)
            {
                Export_Read_Data.ReadType = (EXPORT_READ_TYPE)rxBuffer[2]; //读取数据类型
                Export_Read_Data.Num_Order = (rxBuffer[3] << 8) + rxBuffer[4]; //次数
                Export_Read_Data.FunCrtl = (EXPORT_READ_STATE)rxBuffer[5]; //读取控制
                if(rxBuffer[5] == 0)
                {
                    Export_Read_Data.PackageBytes = (rxBuffer[6] << 8) + rxBuffer[7]; //每包字节数
                }

                txBuffer[0] = rxBuffer[0];
                txBuffer[1] = rxBuffer[1];

                frameLength = PC_EXPORT_DATA_FLAG;
            }
            break;

        default:
            break;
    }

    return frameLength;
}


/** ************************************************************************
 * @brief   Nor-Flash数据存储（按16-bit数）
 * @param   \b offset is the start address of flash to read.
 * @param   \b data is the pointer to data buffer, 16-bit data.
 * @param   \b size is the size of 16-bit data.
 * @return  TRUE/FALSE
 * @details Read data from flash.
 **************************************************************************/
//#pragma CODE_SECTION(nor_flash_SaveData, "ramfuncs");
uint16_t nor_flash_SaveData(uint32_t offset, volatile uint16_t *data, uint32_t size)
{
    //写入数据
    if (FALSE == nor_flash_writeByte(offset, data+10, size))
    {
        return FALSE;
    }

    WATCHDOG_SERVICE();//喂狗

    //读回数据并校验
    nor_flash_readByte(offset, data+10, size);

    if (0 == CRC16_ByteArray(data, 12 + size, 0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

