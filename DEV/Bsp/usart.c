/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： usart.c
  * @作者       ： txdz
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月09日
  * @摘要       ： USART源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2016-05-09 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
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
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
 



/************************************************
函数名称 ： USART_GPIO_Configuration
功    能 ： USART所使用管脚输出输入定义
参    数 ： 无
返 回 值 ： 无
作    者 ： txdz
*************************************************/
void USART_GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 定义 USART-TX 引脚为复用输出 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                          //IO口的第9脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  //IO口速度
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                    //IO口复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART输出IO口

  /* 定义 USART-Rx 引脚为悬空输入 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                         //IO口的第10脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;              //IO口悬空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                             //USART输入IO口
}

/************************************************
函数名称 ： USART_Configuration
功    能 ： 配置USART
参    数 ： 无
返 回 值 ： 无
作    者 ： txdz
*************************************************/
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  /******************************************************************
   USART参数初始化:  波特率     传输位数   停止位数  校验位数
                    115200         8          1      0(NO)
  *******************************************************************/
  USART_InitStructure.USART_BaudRate = 115200;                       //设定传输速率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;        //设定传输数据位数
  USART_InitStructure.USART_StopBits = USART_StopBits_1;             //设定停止位个数
  USART_InitStructure.USART_Parity = USART_Parity_No ;               //不用校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不用流量控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //使用接收和发送功能
  USART_Init(USART1, &USART_InitStructure);                          //初始化USART1

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                     //使能USART1接收中断

  USART_Cmd(USART1, ENABLE);                                         //使能USART1
}

/************************************************
函数名称 ： USART_Initializes
功    能 ： 串口初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： txdz
*************************************************/
void USART_Initializes(void)
{
  USART_GPIO_Configuration();
  USART_Configuration();
}

/************************************************
函数名称 ： USART1_SendChar
功    能 ： 串口1发送一个字符
参    数 ： Data --- 数据
返 回 值 ： 无
作    者 ： txdz
*************************************************/
void USART1_SendByte(uint8_t Data)
{
  while((USART1->SR & USART_FLAG_TXE) == RESET);
  USART1->DR = (Data & (uint16_t)0x01FF);
}

/************************************************
函数名称 ： USART1_SendNByte
功    能 ： 串口1发送N个字符
参    数 ： pData ----- 字符串
            Length --- 长度
返 回 值 ： 无
作    者 ： txdz
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
函数名称 ： USART1_Printf
功    能 ： 串口1打印输出
参    数 ： string --- 字符串
返 回 值 ： 无
作    者 ： txdz
*************************************************/
void USART1_Printf(uint8_t *String)
{
  while((*String) != '\0')
  {
    USART1_SendByte(*String);
    String++;
  }
}

/*UART1中断处理函数*/
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
函数名称 ： USART_ReceiveOvertimeProcess
功    能 ： 串口超时接收
参    数 ： 
返 回 值 ： 无
作    者 ： txdz
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
