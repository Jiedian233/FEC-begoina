#include "USART1.h"


uint8_t USART1_receive_buf[USART1_BUF_SIZE],USART1_ready_buf[USART1_BUF_SIZE];
static int32_t USART1_ReceiveTimeCounter = 0;
volatile uint16_t USART1_receive_index=0; 
volatile uint8_t USART1_ready_buf_ok = 0;
volatile uint16_t USART1_ready_buf_len = 0;


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=ch;
  return (ch);
}
#endif 

/* USART初始化 */
void USART_Initializes(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//使能USART的时钟
	/* USART1的端口配置 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//配置PA9成第二功能引脚	TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//配置PA10成第二功能引脚  RX	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1的基本配置 */
	USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;              //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //使能接收中断
	USART_Cmd(USART1, ENABLE);                             //使能USART1
	
	/* USART1的NVIC中断配置 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}


void USART1_SendByte(uint8_t Data)
{
	while(!((USART1->ISR)&(1<<7))){}
  USART1->TDR=Data;
}


void USART1_SendNByte(uint8_t *pData, uint16_t Length)
{
  while(Length--)
  {
    USART1_SendByte(*pData);
    pData++;
  }
}


void USART1_Printf(uint8_t *String)
{
  while((*String) != '\0')
  {
    USART1_SendByte(*String);
    String++;
  }
}

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
