
#include "timer.h"


void TIMER_Initializes(void)
{

}


void TIMDelay_N10us(uint16_t Times)
{

}


void TIMDelay_Nms(uint16_t Times)
{
  while(Times--)
  {
    TIMDelay_N10us(100);
  }
}




/**** Copyright (C)2016 LianHuaMaNong. All Rights Reserved **** END OF FILE ****/
