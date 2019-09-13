/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� usart.h
  * @����       �� LianHuaMaNong
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��09��
  * @ժҪ       �� USARTͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _USART_H
#define _USART_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f0xx.h" //stm32f0xx
#include "stdio.h"


/********************uart1****************************/

#define	USART1_BAUDRATE				9600		//�����ʣ�ȱʡֵ115200
#define USART1_BUF_SIZE 			256	//����һ��������������С��ȱʡֵ2000
#define USART1_RECEIVE_OVERTIME		2			//�ֽڳ�ʱʱ��, ��λΪms��ȱʡֵ100ms

extern uint8_t 					 USART1_ready_buf[];
extern volatile uint8_t  USART1_ready_buf_ok;
extern volatile uint16_t USART1_ready_buf_len;





/* �������� ------------------------------------------------------------------*/
void USART_Initializes(void);
void USART1_SendByte(uint8_t Data);
void USART1_SendNByte(uint8_t *pData, uint16_t Length);
void USART1_Printf(uint8_t *String);
void USART_ReceiveOvertimeProcess(void);
void USART_printfHex(const uint8_t* SendChars,uint16_t len);
#endif /* _USART_H */

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
