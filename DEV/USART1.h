
#ifndef _USART1_H
#define _USART1_H

#include "stm32f0xx.h"
#include <stdio.h>

#define SYSTEMTICK_PERIOD_MS		1			//�δ�ʱ������λms��ȱʡֵ10ms
#define	USART1_BAUDRATE				9600		//�����ʣ�ȱʡֵ115200
#define USART1_BUF_SIZE 			256	//����һ��������������С��ȱʡֵ2000
#define USART1_RECEIVE_OVERTIME		20			//�ֽڳ�ʱʱ��, ��λΪms��ȱʡֵ100ms

extern uint8_t 					 USART1_ready_buf[];
extern volatile uint8_t  USART1_ready_buf_ok;
extern volatile uint16_t USART1_ready_buf_len;

void USART_Initializes(void);

#endif
