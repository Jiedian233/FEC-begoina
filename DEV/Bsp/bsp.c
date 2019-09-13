
#include "bsp.h"
#include "systick.h"
//#include "usart.h"

void RCC_Configuration(void)
{
  /* 使能APB2时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA  | RCC_AHBPeriph_GPIOB , ENABLE);

  /* 使能APB1时钟 */

  /* 使能APB时钟 */
}


void GPIO_Basic_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	/*按键*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN ;//浮空输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
	
}



void NVIC_Configuration(void)
{

}


void BSP_Initializes(void)
{
  RCC_Configuration();                           //时钟配置
  GPIO_Basic_Configuration();                    //IO配置
	SysTickInit(1, 1);
  NVIC_Configuration();                          //中断配置
}


/**
*@brief		STM32系统软复位函数
*@param		无
*@return	无
*/
void SysReboot(void)
{
  NVIC_SystemReset();// 复位
}

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
