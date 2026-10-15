/**
  ******************************************************************************

  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_CONF_H
#define __HC32F4xx_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment the line below to enable peripheral header file inclusion */
#include "hc32f4xx_rcc.h" 
#include "hc32f4xx_rtc.h" 
#include "hc32f4xx_gpio.h"
#include "hc32f4xx_exti.h"
#include "hc32f4xx_syscfg.h"
#include "hc32f4xx_flash.h"
#include "hc32f4xx_fda.h"
#include "hc32f4xx_tim.h"
#include "hc32f4xx_tim10.h"
#include "hc32f4xx_adc.h"
#include "hc32f4xx_dac.h"
#include "hc32f4xx_vc.h"
#include "hc32f4xx_dma.h"
#include "hc32f4xx_crc.h"
#include "hc32f4xx_canfd.h"
#include "hc32f4xx_pwr.h"
#include "hc32f4xx_spi.h"
#include "hc32f4xx_i2s.h"
#include "hc32f4xx_i2c.h"
#include "hc32f4xx_mou.h"
#include "hc32f4xx_iwdg.h"
#include "hc32f4xx_wwdg.h"
#include "hc32f4xx_usart.h"
#include "hc32f4xx_dbgmcu.h"
#include "misc.h"

#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif 

/*****END OF FILE****/
