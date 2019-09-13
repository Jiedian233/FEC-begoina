/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) SEMTECH S.A.
 */
/*!
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    1.0.B2 ( PRELIMINARY )
 * \date       Nov 21 2012
 * \author     Miguel Luis
 */

#include <stdbool.h>


#if defined( USE_SX1276_RADIO )

#include "spi.h"
#include "stm32f0xx.h"
#include "sx1276-Hal.h"




#define RESET_IOPORT                                GPIOA
#define RESET_PIN                                   GPIO_Pin_6


/*!
 * SX1276 DIO pins  I/O definitions
 */
#define DIO0_IOPORT                                 GPIOC
#define DIO0_PIN                                    GPIO_Pin_12

#define DIO1_IOPORT                                 GPIOC
#define DIO1_PIN                                    GPIO_Pin_13

#define DIO2_IOPORT                                 GPIOC
#define DIO2_PIN                                    GPIO_Pin_14

#define DIO3_IOPORT                                 GPIOC
#define DIO3_PIN                                    GPIO_Pin_15

#define DIO4_IOPORT                                 GPIOC
#define DIO4_PIN                                    GPIO_Pin_8

#define DIO5_IOPORT                                 GPIOC
#define DIO5_PIN                                    GPIO_Pin_2


#define RXE_PORT       				GPIOC
#define RXE_PIN  							GPIO_Pin_1

#define RXE_CLOCK  						RCC_APB2Periph_GPIOB
#define RXE_HIGH()         		GPIO_SetBits(RXE_PORT,RXE_PIN)
#define RXE_LOW()          		GPIO_ResetBits(RXE_PORT,RXE_PIN)
#define RXE_STATE()        		GPIO_ReadOutputDataBit(RXE_PORT,RXE_PIN)

#define TXE_PORT       				GPIOC
#define TXE_PIN  							GPIO_Pin_2
#define TXE_CLOCK  						RCC_APB2Periph_GPIOB
#define TXE_HIGH()         		GPIO_SetBits(TXE_PORT,TXE_PIN)
#define TXE_LOW()          		GPIO_ResetBits(TXE_PORT,TXE_PIN)
#define TXE_STATE()        		GPIO_ReadOutputDataBit(TXE_PORT,TXE_PIN)








/************************************************
函数名称 ： SX127x硬件层函数，SX1276InitIo  SX127X IO口初始化
功    能 ： NVIC配置
参    数 ： 无
返 回 值 ： 无
作    者 ： LianHuaMaNong
*************************************************/
void SX1276InitIo( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	/*sx1278 射频芯片 接收引脚初始化 ---输出*/
	GPIO_InitStructure.GPIO_Pin =  RXE_PIN;       			 //引脚    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RXE_PORT, &GPIO_InitStructure);


	
	/*sx1278 射频芯片 发射引脚初始化 ---输出*/
	
	GPIO_InitStructure.GPIO_Pin =  TXE_PIN;       			 //引脚      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(TXE_PORT, &GPIO_InitStructure);
	
	/*sx1278 RESET引脚初始化       ---输出*/
	
	GPIO_InitStructure.GPIO_Pin =  RESET_PIN;       			 //引脚     
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RESET_IOPORT, &GPIO_InitStructure);
	
}
/************************************************
函数名称 ： SX127x硬件层函数，SX1276初始化重置
功    能 ： SX1278重启引脚初始化加输出
参    数 ： RESET 输出的电平高低
返 回 值 ： 
作    者 ： LianHuaMaNong
*************************************************/
void SX1276SetReset( uint8_t state )
{
    if( state == RADIO_RESET_ON )
    {	
				// Set RESET pin to 0
       // GPIO_WriteBit( RESET_IOPORT, RESET_PIN, Bit_RESET );
				GPIO_ResetBits( RESET_IOPORT, RESET_PIN);
    }
    else
    {
				// Set RESET pin to 1
			GPIO_SetBits( RESET_IOPORT, RESET_PIN);

    }
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}
/************************************************
函数名称 ： SX127x硬件层函数，SX1276 SPI写寄存器功能实现
功    能 ： SX1276 SPI写寄存器功能实现
参    数 ： addr：寄存器地址  buffer 待写入值    size ：写入数据的长度
返 回 值 ： 无
作    者 ： LianHuaMaNong
*************************************************/
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{

	uint8_t i;

    CS_ENABLE;

    SPI_WriteReadByte( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
        SPI_WriteReadByte( buffer[i] );
    }

		CS_DISABLE;
		
}
/************************************************
函数名称 ： SX127x硬件层函数，SX1276 SPI读寄存器功能实现
功    能 ： SX1276 SPI读寄存器功能实现
参    数 ： addr：所读寄存器地址  buffer 返回值    size ：读取数据长度
返 回 值 ： 无
作    者 ： LianHuaMaNong
*************************************************/
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	 uint8_t i;

    CS_ENABLE;

    SPI_WriteReadByte( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
        buffer[i] = SPI_WriteReadByte( 0 );
    }

	CS_DISABLE;
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

uint8_t SX1276ReadDio0( void )
{
    return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );;
}

uint8_t SX1276ReadDio1( void )
{
    return GPIO_ReadInputDataBit( DIO1_IOPORT, DIO1_PIN );;
}

uint8_t SX1276ReadDio2( void )
{
    return GPIO_ReadInputDataBit( DIO2_IOPORT, DIO2_PIN );;
}

uint8_t SX1276ReadDio3( void )
{
    return GPIO_ReadInputDataBit( DIO3_IOPORT, DIO3_PIN );;
}

uint8_t SX1276ReadDio4( void )
{
    return GPIO_ReadInputDataBit( DIO4_IOPORT, DIO4_PIN );;
}

uint8_t SX1276ReadDio5( void )
{
    return GPIO_ReadInputDataBit( DIO5_IOPORT, DIO5_PIN );;
}

/************************************************
函数名称 ： SX127x硬件层函数，射频芯片收发切换
功    能 ： 射频芯片收发切换
参    数 ： txEnable 1发送  0接收
返 回 值 ： 无
作    者 ： LianHuaMaNong
*************************************************/
void SX1276WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {
			RXE_LOW();
			TXE_HIGH();    
		}
    else
    {
			RXE_HIGH();
			TXE_LOW();   
		}
}

#endif // USE_SX1276_RADIO
