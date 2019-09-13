/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： spi.h
  * @作者       ： LianHuaMaNong
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月09日
  * @摘要       ： SPI头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

/* 包含的头文件 --------------------------------------------------------------*/
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

/* 函数申明 ------------------------------------------------------------------*/
void SPI_Initializes(void);
uint8_t SPI_WriteReadByte(uint8_t TxData);


#endif

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
