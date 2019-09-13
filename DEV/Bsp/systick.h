#ifndef _systick_h
#define _systick_h

#include "stm32f0xx.h"


extern volatile uint32_t TickCounter;


void SysTickInit(uint16_t ms, uint8_t priority);
void delay_ms(uint32_t cnt);
uint8_t check_led_timeout_close_timeout(void);
#endif

