/**
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_DBGMCU_H
#define __HC32F4xx_DBGMCU_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

/** @addtogroup DBGMCU
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup DBGMCU_Exported_Constants
  * @{
  */ 
#define DBGMCU_SLEEP                 ((uint32_t)0x00000001)
#define DBGMCU_STOP                  ((uint32_t)0x00000002)
#define DBGMCU_STANDBY               ((uint32_t)0x00000004)

#define DBGMCU_TIM2_STOP             ((uint32_t)0x00000001)
#define DBGMCU_TIM3_STOP             ((uint32_t)0x00000002)
#define DBGMCU_TIM4_STOP             ((uint32_t)0x00000004)
#define DBGMCU_TIM5_STOP             ((uint32_t)0x00000008)
#define DBGMCU_TIM6_STOP             ((uint32_t)0x00000010)
#define DBGMCU_TIM7_STOP             ((uint32_t)0x00000020)
#define DBGMCU_TIM12_STOP            ((uint32_t)0x00000040)
#define DBGMCU_TIM13_STOP            ((uint32_t)0x00000080)
#define DBGMCU_TIM14_STOP            ((uint32_t)0x00000100)
#define DBGMCU_RTC_STOP              ((uint32_t)0x00000400)
#define DBGMCU_WWDG_STOP             ((uint32_t)0x00000800)
#define DBGMCU_IWDG_STOP             ((uint32_t)0x00001000)


#define DBGMCU_TIM1_STOP             ((uint32_t)0x00000001)
#define DBGMCU_TIM8_STOP             ((uint32_t)0x00000002)
#define DBGMCU_TIM9_STOP             ((uint32_t)0x00010000)
#define DBGMCU_TIM10_STOP            ((uint32_t)0x00020000)

/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 
uint32_t DBGMCU_GetREVID(void);
uint32_t DBGMCU_GetDEVID(void);
void DBGMCU_Config(uint32_t DBGMCU_Periph, FunctionalState NewState);
void DBGMCU_APB1PeriphConfig(uint32_t DBGMCU_Periph, FunctionalState NewState);
void DBGMCU_APB2PeriphConfig(uint32_t DBGMCU_Periph, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif



/*****END OF FILE****/
