
#include "spi.h"


void SPI_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitSturcture;

	/*CS*/
	GPIO_InitSturcture.GPIO_Pin = PIN_SPI_CS;
  GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitSturcture.GPIO_OType = GPIO_OType_PP;
  GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_SPI_CS,&GPIO_InitSturcture);
	
	/*SCK*/
	GPIO_InitSturcture.GPIO_Pin = PIN_SPI_SCK;
  GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitSturcture.GPIO_OType = GPIO_OType_PP;
  GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_SPI_SCK,&GPIO_InitSturcture);
	
	/*MISO*/
	GPIO_InitSturcture.GPIO_Pin = PIN_SPI_MISO;
	GPIO_InitSturcture.GPIO_Mode =   GPIO_Mode_IN ;//浮空输入
  GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PORT_SPI_MISO,&GPIO_InitSturcture);	
	
	/*MOSI*/
	GPIO_InitSturcture.GPIO_Pin = PIN_SPI_MOSI;
  GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitSturcture.GPIO_OType = GPIO_OType_PP;
  GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_SPI_MOSI,&GPIO_InitSturcture);
	
//	GPIO_InitSturcture.GPIO_Pin = PIN_LED;
//  GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitSturcture.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitSturcture.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(PORT_LED,&GPIO_InitSturcture);	 

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	 //使能A端口时钟
    GPIO_InitSturcture.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitSturcture.GPIO_Mode = GPIO_Mode_IN; 		 //浮空输入
    GPIO_InitSturcture.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitSturcture);	  
	
	CS_DISABLE; 
	SPI_SCK_LOW;
	SPI_MOSI_HIGH;
}


uint8_t SPI_WriteReadByte(uint8_t TxData)
{
uint8_t cnt; 
	for(cnt=0; cnt<8 ;cnt++)
	{
		if(TxData & 0x80)                
		{
			SPI_MOSI_HIGH;
		}
		else
		{
			SPI_MOSI_LOW;
		}
		SPI_SCK_HIGH; 	    
		//if(is_init) delay(200);		
		//delay_us(1);	 
		TxData = TxData << 1;
		if(SPI_MISO_READ)                
		{
			TxData |= 0x01;
		}
    SPI_SCK_LOW;  
		//if(is_init) delay(200);	
		//delay_us(1);		
		
	}
	return TxData;  
}


/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
