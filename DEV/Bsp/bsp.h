/**
  ********************************  STM32F10x  *********************************
  * @文件名     ： bsp.c
  * @作者       ： LianHuaMaNong
  * @库版本     ： V3.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年05月09日
  * @摘要       ： BSP板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm32f0xx.h"


/* 宏定义 --------------------------------------------------------------------*/
#define PORT_LED                  GPIOB
#define PIN_LED                   GPIO_Pin_0

/* LED开关 */
#define LED_ON                    (PORT_LED->BRR  = PIN_LED)
#define LED_OFF                   (PORT_LED->BSRR = PIN_LED)
#define LED_TOGGLE                (PORT_LED->ODR ^= PIN_LED)

#define GUNZHU      (GPIOA->IDR & GPIO_Pin_7)
#define KEY_IN      (GPIOA->IDR & GPIO_Pin_6)
#define LTTLCK      (GPIOB->IDR & GPIO_Pin_9)
typedef  void (*pFunction)(void);

/* 函数申明 ------------------------------------------------------------------*/
void BSP_Initializes(void);
void SysReboot(void);

#endif /* _BSP_H */

/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
