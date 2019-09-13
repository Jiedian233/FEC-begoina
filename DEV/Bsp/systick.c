
#include "stm32f0xx.h"
#include "usart.h"
#include "systick.h"

volatile uint32_t TickCounter = 0;
/**************************************************************************
 * systick_init
**************************************************************************/


void SysTickInit(uint16_t ms, uint8_t priority)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.SYSCLK_Frequency /(1000/ms));
  NVIC_SetPriority (SysTick_IRQn, priority);  
	
}


/**************************************************************************
 * SysTick_Handler
**************************************************************************/
void SysTick_Handler(void)
{

	USART_ReceiveOvertimeProcess();
	TickCounter++;
}


void delay_ms(uint32_t cnt)
{
    volatile uint32_t i = cnt * 4510;
    
    while(i--);
}

#define led_timeout_close_timeout 1500  //µ•Œª1ms  1.5√Î   
__IO uint32_t led_timeout_close_timeouts;

void reset_led_timeout_close_timeout(void)
{
	led_timeout_close_timeouts = 0;
}
uint8_t check_led_timeout_close_timeout(void)
{
	if(led_timeout_close_timeouts==0)
	{
		led_timeout_close_timeouts = TickCounter;
	}else if(led_timeout_close_timeouts+led_timeout_close_timeout<=TickCounter)
	{
		led_timeout_close_timeouts=0;
		return 1;
	}
	return 0;
}
