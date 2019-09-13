/**
  ********************************  STM32F10x  *********************************
  * @�ļ���     �� bsp.c
  * @����       �� LianHuaMaNong
  * @��汾     �� V3.5.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2016��05��09��
  * @ժҪ       �� BSP�弶֧�ְ�ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm32f0xx.h"


/* �궨�� --------------------------------------------------------------------*/
#define PORT_LED                  GPIOB
#define PIN_LED                   GPIO_Pin_0

/* LED���� */
#define LED_ON                    (PORT_LED->BRR  = PIN_LED)
#define LED_OFF                   (PORT_LED->BSRR = PIN_LED)
#define LED_TOGGLE                (PORT_LED->ODR ^= PIN_LED)

#define GUNZHU      (GPIOA->IDR & GPIO_Pin_7)
#define KEY_IN      (GPIOA->IDR & GPIO_Pin_6)
#define LTTLCK      (GPIOB->IDR & GPIO_Pin_9)
typedef  void (*pFunction)(void);

/* �������� ------------------------------------------------------------------*/
void BSP_Initializes(void);
void SysReboot(void);

#endif /* _BSP_H */

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
