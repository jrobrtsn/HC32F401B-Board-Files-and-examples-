#ifndef __SYSTEM_HC32F4XX_H
#define __SYSTEM_HC32F4XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "hc32f4xx.h"

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */


extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */
  
/**
  * @}
  */  
/*****END OF FILE****/
