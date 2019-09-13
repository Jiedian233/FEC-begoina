
	
#include "radio.h"
#include "stdio.h"	
//#include "usart.h"
#include "sx1276-Hal.h"
#include "sx1276.h"
//#include "debug.h"
#include "sx1276-LoRaMisc.h"
	
#include "sx1276-LoRa.h"
#include "LoraComm.h"

#define LENGTH_OF_DAT_PAYLOAD		25	





void LoraReadInit(uint8_t SingleMode,uint32_t RF_Frequency,uint32_t Timeout, uint16_t preamble )
{
	Radio->LoRaSetRxSingleOn( SingleMode );
	Radio->LoRaSetRFFrequency( RF_Frequency );
	Radio->LoRaSetPreambleLength( preamble );
	Radio->LoRaSetRxPacketTimeout( Timeout );
	Radio->StartRx( );
}



uint8_t LoraWrite(uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble,uint32_t Timeout )
{
	uint32_t result = 0;
		Radio->LoRaSetRFFrequency( RF_Frequency );	
		Radio->LoRaSetPreambleLength( preamble );
		Radio->LoRaSetTxPacketTimeout( Timeout );
		
		if(bufferLen > MAXLOADLEN)
		{
			Radio->SetTxPacket( buffer, MAXLOADLEN );		//开始发送
		}
		else
		{
			Radio->LoRaSetPayloadLength(bufferLen);
			Radio->SetTxPacket( buffer, bufferLen );		//开始发送
		}
		while( 1 )
	{
		result = Radio->Process( );

			if( result == RF_TX_DONE || result == RF_TX_TIMEOUT )
			break;
	}
	if( result == RF_TX_DONE )
	{

		return 1;
	}

		return 0;
}


int16_t LoraSpecialRead( void *Buffer ,uint16_t PreambleLength,uint32_t Timeout, int8_t *snr,int16_t *rssi )
{
	uint32_t result;
	uint16_t bufferLen = 0;
	Radio->LoRaSetRxSingleOn( 0 );
	Radio->LoRaSetRFFrequency( UPLINK_FREQUENCY );
	
	Radio->LoRaSetRxPacketTimeout( Timeout );
	
	Radio->LoRaSetPreambleLength( PreambleLength );
	
	Radio->LoRaSetPayloadLength( LENGTH_OF_DAT_PAYLOAD );

	Radio->StartRx( );
	while( 1 )
	{
		result = Radio->Process( );
		if( result == RF_RX_DONE || result == RF_RX_TIMEOUT )
		break;
	}
	if( result == RF_RX_DONE )
	{
			Radio->GetRxPacket( Buffer, ( uint16_t* )&bufferLen );
			*rssi = ( int16_t )( SX1276LoRaGetPacketRssi( ) + 0.5 );
			*snr = SX1276LoRaGetPacketSnr( );
		return bufferLen;
	}
	return -1;
}



void LoraSpecialWrite(uint8_t * buffer,uint16_t bufferLen,uint32_t RF_Frequency,uint16_t preamble,uint32_t Timeout )
{
		Radio->LoRaSetRFFrequency( RF_Frequency );	//设置发射频率 		参数来自宏定义
		Radio->LoRaSetPreambleLength( preamble );
		Radio->LoRaSetTxPacketTimeout( Timeout );
		if(bufferLen > MAXLOADLEN)
		{
			Radio->SetTxPacket( buffer, MAXLOADLEN );		//开始发送
		}
		else
		{
			Radio->LoRaSetPayloadLength(bufferLen);
			Radio->SetTxPacket( buffer, bufferLen );		//开始发送
		}
}
