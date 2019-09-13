/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� usart.c
  * @����       �� txdz
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��09��
  * @ժҪ       �� USARTԴ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2016-05-09 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "usart.h"


/********************uart1****************************/
uint8_t USART1_receive_buf[USART1_BUF_SIZE],USART1_ready_buf[USART1_BUF_SIZE] = {
0x44,0x5a,0,01,02
};
static int32_t USART1_ReceiveTimeCounter = 0;
volatile uint16_t USART1_receive_index=0; 
volatile uint8_t USART1_ready_buf_ok = 0;
volatile uint16_t USART1_ready_buf_len = 5;



#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
 



/************************************************
�������� �� USART_GPIO_Configuration
��    �� �� USART��ʹ�ùܽ�������붨��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ���� USART-TX ����Ϊ������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                          //IO�ڵĵ�9��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //IO���ٶ�
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    //IO�ڸ����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART���IO��

  /* ���� USART-Rx ����Ϊ�������� */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                         //IO�ڵĵ�10��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;              //IO����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART����IO��
}

/************************************************
�������� �� USART_Configuration
��    �� �� ����USART
��    �� �� ��
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  /******************************************************************
   USART������ʼ��:  ������     ����λ��   ֹͣλ��  У��λ��
                    115200         8          1      0(NO)
  *******************************************************************/
  USART_InitStructure.USART_BaudRate = 115200;                       //�趨��������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //�趨��������λ��
  USART_InitStructure.USART_StopBits = USART_StopBits_1;             //�趨ֹͣλ����
  USART_InitStructure.USART_Parity = USART_Parity_No ;               //����У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//������������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //ʹ�ý��պͷ��͹���
  USART_Init(USART1, &USART_InitStructure);                          //��ʼ��USART1

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     //ʹ��USART1�����ж�

  USART_Cmd(USART1, ENABLE);                                         //ʹ��USART1
}

/************************************************
�������� �� USART_Initializes
��    �� �� ���ڳ�ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART_Initializes(void)
{
  USART_GPIO_Configuration();
  USART_Configuration();
}

/************************************************
�������� �� USART1_SendChar
��    �� �� ����1����һ���ַ�
��    �� �� Data --- ����
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART1_SendByte(uint8_t Data)
{
  while((USART1->SR & USART_FLAG_TXE) == RESET);
  USART1->DR = (Data & (uint16_t)0x01FF);
}

/************************************************
�������� �� USART1_SendNByte
��    �� �� ����1����N���ַ�
��    �� �� pData ----- �ַ���
            Length --- ����
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART1_SendNByte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    USART1_SendByte(*pData);
    pData++;
  }
}

/************************************************
�������� �� USART1_Printf
��    �� �� ����1��ӡ���
��    �� �� string --- �ַ���
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART1_Printf(uint8_t *String)
{
  while((*String) != '\0')
  {
    USART1_SendByte(*String);
    String++;
  }
}

/*UART1�жϴ�����*/
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		USART1_ReceiveTimeCounter = USART1_RECEIVE_OVERTIME;
		if(USART1_receive_index >= USART1_BUF_SIZE)
			USART1_receive_index = 0;
		
		USART1_receive_buf[USART1_receive_index++] = (uint8_t)USART_ReceiveData(USART1);
		
	}
		
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		(void)USART_ReceiveData(USART1);
	}
	USART_ClearFlag(USART1,USART_FLAG_TC); 
}

/************************************************
�������� �� USART_ReceiveOvertimeProcess
��    �� �� ���ڳ�ʱ����
��    �� �� 
�� �� ֵ �� ��
��    �� �� txdz
*************************************************/
void USART_ReceiveOvertimeProcess(void)
{
	uint16_t i = 0;	
	/********************uart1****************************/
	if(USART1_ReceiveTimeCounter>=SYSTEMTICK_PERIOD_MS)
	{
		USART1_ReceiveTimeCounter -= SYSTEMTICK_PERIOD_MS;

		if(USART1_ReceiveTimeCounter<SYSTEMTICK_PERIOD_MS)
		{
			if(USART1_ready_buf_ok == 0)
			{
				USART1_ready_buf_len = USART1_receive_index;

				for(i = 0;i <= USART1_ready_buf_len; i ++)
					USART1_ready_buf[i] = USART1_receive_buf[i];
				USART1_ready_buf[USART1_ready_buf_len] = '\0';
				USART1_ready_buf_ok = 1;
				USART1_receive_index=0;
				USART1_ReceiveTimeCounter = 0;
			}
			else
			{
				 USART1_ReceiveTimeCounter = 0;
			}
		}
		
	}
}

void USART_printfHex(const uint8_t* SendChars,uint16_t len)
{
	uint16_t i = 0;
//	uint8_t temp = 0;
	for(i=0;i<len;i++)
	{
		printf("%02X",SendChars[i]);
	}
	printf("\r\n");
}

/**** Copyright (C)2016 txdz. All Rights Reserved **** END OF FILE ****/
