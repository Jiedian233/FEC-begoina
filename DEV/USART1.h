
#ifndef _USART1_H
#define _USART1_H

#include "stm32f0xx.h"
#include <stdio.h>

#define SYSTEMTICK_PERIOD_MS		1			//滴答时基，单位ms，缺省值10ms
#define	USART1_BAUDRATE				9600		//波特率，缺省值115200
#define USART1_BUF_SIZE 			256	//接收一、二级缓冲区大小，缺省值2000
#define USART1_RECEIVE_OVERTIME		20			//字节超时时间, 单位为ms，缺省值100ms

extern uint8_t 					 USART1_ready_buf[];
extern volatile uint8_t  USART1_ready_buf_ok;
extern volatile uint16_t USART1_ready_buf_len;

void USART_Initializes(void);

#endif
