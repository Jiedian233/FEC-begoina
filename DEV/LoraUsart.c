
	
	
#include "LoraUsart.h"
#include "LoraComm.h"
#include "usart.h"
#include "radio.h"
#include "stdio.h"	
#include "usart.h"
#include "sx1276-Hal.h"
#include "sx1276.h"
#include "spi.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "LoraComm.h"
#include "string.h"	
#include "BSP.h"	
#include "oled.h"	
//#include "debug.h"
typedef enum LORAUSART_t
{
	LORA_USART_START,
	LORA_USART_RECEIVE,
	LORA_USART_SEND,

}LORAUSART;
	
	

	double ASSS;
void UsartToLora(void )
{
	uint8_t BUFFER[MAXLOADLEN] ;
	uint32_t result = 0;
	uint16_t bufferLen = 0;
	static LORAUSART t_LoraUsart = LORA_USART_START;
	if(USART1_ready_buf_ok == 1)
	{
		USART1_ready_buf_ok = 0;
		t_LoraUsart = LORA_USART_SEND;
	}
	switch(t_LoraUsart)
	{
	case LORA_USART_START:
		LoraReadInit(0,UPLINK_FREQUENCY ,100 ,SHORT_PREAMBLE_LENGTH);
		t_LoraUsart = LORA_USART_RECEIVE;
	break;
	case LORA_USART_RECEIVE:
		result =  Radio->Process( );
		if(result == RF_RX_DONE)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
			Radio->GetRxPacket( BUFFER, ( uint16_t* )&bufferLen );
			USART1_SendNByte(BUFFER,bufferLen);	
			show_tx_rx_data(0,BUFFER,bufferLen);
			t_LoraUsart = LORA_USART_START;
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}
		else if(result ==RF_RX_TIMEOUT)
		{
			t_LoraUsart = LORA_USART_START;
		}
	break;
	case LORA_USART_SEND:
		
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		LoraWrite(USART1_ready_buf,USART1_ready_buf_len,UPLINK_FREQUENCY ,SHORT_PREAMBLE_LENGTH,3000);
		show_tx_rx_data(1,USART1_ready_buf,USART1_ready_buf_len);
		t_LoraUsart = LORA_USART_START;
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	break;
	}
}
unsigned short sendnum = 0;
unsigned short AutomaticSend = 0;
__IO unsigned int timesss2;
void keyworking(void)
{
	if(KEY==0)
	{
		delay_ms(1);
		if(KEY==0)
		{
			unsigned int timesss= 0x7fffff;
			while(KEY==0&&--timesss);
			if(timesss==0)
			{
				AutomaticSend = 1;
				timesss2= 0x7fffff;
				OLED_Clear();
        OLED_ShowString(30, 0, (unsigned char*)"Automatic!", 16);
				while(KEY==0);
			}else
			{
				if(AutomaticSend ==1)
				{
					AutomaticSend = 0;
				}
				USART1_ready_buf_ok = 1;
				sprintf((char*)USART1_ready_buf,"Lora Test Sending NO.%05d      By KEY",++sendnum);
				USART1_ready_buf_len = strlen((const char*)USART1_ready_buf);
			}
		}
	}else if(AutomaticSend ==1)
	{
		if(timesss2++>0xafff)
		{
			timesss2=0;
			USART1_ready_buf_ok = 1;
			sprintf((char*)USART1_ready_buf,"Lora Test Sending NO.%05d      By Automatic",++sendnum);
			USART1_ready_buf_len =  strlen((const char*)USART1_ready_buf);
		}
	}
}
