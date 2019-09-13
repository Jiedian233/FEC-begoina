
#include "bsp.h"
#include "systick.h"
//#include "usart.h"

void RCC_Configuration(void)
{
  /* ʹ��APB2ʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA  | RCC_AHBPeriph_GPIOB , ENABLE);

  /* ʹ��APB1ʱ�� */

  /* ʹ��APBʱ�� */
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
	/*����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN ;//��������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);		
	
}



void NVIC_Configuration(void)
{

}


void BSP_Initializes(void)
{
  RCC_Configuration();                           //ʱ������
  GPIO_Basic_Configuration();                    //IO����
	SysTickInit(1, 1);
  NVIC_Configuration();                          //�ж�����
}


/**
*@brief		STM32ϵͳ��λ����
*@param		��
*@return	��
*/
void SysReboot(void)
{
  NVIC_SystemReset();// ��λ
}

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
