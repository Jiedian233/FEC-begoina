/**
  ********************************  STM32F0xx  *********************************
  * @文件名     ： LoraComm.h
  * @作者       ： LianHuaMaNong
  * @库版本     ： V1.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年06月04日
  * @摘要       ： LoraComm头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _LORACOMM_H
#define _LORACOMM_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f0xx.h"

#define COMMOM_RF           		433920000
#define UPLINK_FREQUENCY				433920000             //435000000
#define DOWNLINK_FREQUENCY			433920000              //435000000



#define SHORT_PREAMBLE_LENGTH		12
#define LONG_PREAMBLE_LENGTH		60



#define MAXLOADLEN        			128	

void LoraReadInit(uint8_t SingleMode,uint32_t RF_Frequency,uint32_t Timeout, uint16_t preamble );
//void LoraWrite   (uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble );
uint8_t LoraWrite(uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble,uint32_t Timeout );
int16_t LoraSpecialRead( void *Buffer ,uint16_t PreambleLength,uint32_t Timeout, int8_t *snr,int16_t *rssi );

//void LoraSpecialWrite(uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble );
void LoraSpecialWrite(uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble,uint32_t Timeout );
#endif
