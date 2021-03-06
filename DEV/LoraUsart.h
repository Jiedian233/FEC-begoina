/**
  ********************************  STM32F0xx  *********************************
  * @文件名     ： Lorausart.h
  * @作者       ： LianHuaMaNong
  * @库版本     ： V1.5.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2016年06月10日
  * @摘要       ：Lorausart头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _LORAUSART_H
#define _LORAUSART_H

/* 包含的头文件 --------------------------------------------------------------*/

//GPRS
void UsartToLora(void );
void getdata(void);
void LORA_START(void);
void keyworking(void);
#endif
