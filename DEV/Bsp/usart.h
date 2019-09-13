/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： usart.h
  * @作者       ： LianHuaMaNong
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月09日
  * @摘要       ： USART头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _USART_H
#define _USART_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f0xx.h" //stm32f0xx
#include "stdio.h"


/********************uart1****************************/

#define	USART1_BAUDRATE				9600		//波特率，缺省值115200
#define USART1_BUF_SIZE 			256	//接收一、二级缓冲区大小，缺省值2000
#define USART1_RECEIVE_OVERTIME		2			//字节超时时间, 单位为ms，缺省值100ms

extern uint8_t 					 USART1_ready_buf[];
extern volatile uint8_t  USART1_ready_buf_ok;
extern volatile uint16_t USART1_ready_buf_len;





/* 函数申明 ------------------------------------------------------------------*/
void USART_Initializes(void);
void USART1_SendByte(uint8_t Data);
void USART1_SendNByte(uint8_t *pData, uint16_t Length);
void USART1_Printf(uint8_t *String);
void USART_ReceiveOvertimeProcess(void);
void USART_printfHex(const uint8_t* SendChars,uint16_t len);
#endif /* _USART_H */

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
