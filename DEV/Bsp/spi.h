/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� spi.h
  * @����       �� LianHuaMaNong
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��09��
  * @ժҪ       �� SPIͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f0xx.h"
#define PORT_SPI_CS      	 GPIOB
#define PORT_SPI_SCK       GPIOB
#define PORT_SPI_MISO      GPIOB
#define PORT_SPI_MOSI      GPIOB
#define PORT_LED      GPIOB

#define PIN_SPI_CS       	 GPIO_Pin_12
#define PIN_SPI_SCK        GPIO_Pin_13
#define PIN_SPI_MISO       GPIO_Pin_15
#define PIN_SPI_MOSI       GPIO_Pin_14
#define PIN_LED       GPIO_Pin_0


#define CS_ENABLE  (PORT_SPI_CS->BRR  = PIN_SPI_CS) 
#define CS_DISABLE (PORT_SPI_CS->BSRR = PIN_SPI_CS)

#define SPI_SCK_LOW        (PORT_SPI_SCK->BRR  = PIN_SPI_SCK) 
#define SPI_SCK_HIGH       (PORT_SPI_SCK->BSRR = PIN_SPI_SCK) 

#define SPI_MISO_READ      (PORT_SPI_MISO->IDR & PIN_SPI_MISO)

#define SPI_MOSI_LOW       (PORT_SPI_MOSI->BRR  = PIN_SPI_MOSI)
#define SPI_MOSI_HIGH      (PORT_SPI_MOSI->BSRR = PIN_SPI_MOSI)

#define SPI_LED_ON       (PORT_LED->BRR  = PIN_LED)
#define SPI_LED_OFF      (PORT_LED->BSRR = PIN_LED)


#define KEY      (GPIOA->IDR & GPIO_Pin_0)

/* �������� ------------------------------------------------------------------*/
void SPI_Initializes(void);
uint8_t SPI_WriteReadByte(uint8_t TxData);


#endif

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
