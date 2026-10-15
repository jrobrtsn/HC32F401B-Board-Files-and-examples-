#ifndef __HC32F4xx_TIM10_H
#define __HC32F4xx_TIM10_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

typedef struct
{
  uint16_t BTIM_Prescaler; 
  uint32_t BTIM_Period;
} BTIM_TimeBaseInitTypeDef; 



/** @defgroup BTIM_Slave_Mode 
  * @{
  */
#define BTIM_SlaveMode_Trigger              ((uint16_t)0x0001)
#define BTIM_SlaveMode_Gated                ((uint16_t)0x0002)
#define BTIM_SlaveMode_External            	((uint16_t)0x0003)



/** @defgroup TIM_interrupt_sources 
  * @{
  */

#define TIM_IT_Update                      ((uint16_t)0x0001)
#define TIM_IT_Trigger                     ((uint16_t)0x0040)



/** @defgroup BTIM_Trigger_Output_Source 
  * @{
  */
#define BTIM_TRGOSource_Reset               ((uint16_t)0x0000)
#define BTIM_TRGOSource_Enable              ((uint16_t)0x0010)
#define BTIM_TRGOSource_Update              ((uint16_t)0x0020)


/** @defgroup BTIM_Trigger_Selection 
  * @{
  */
#define  BTIM_TS_ITR0                      ((uint16_t)0x0000)
#define  BTIM_TS_ITR1                      ((uint16_t)0x0001)
#define  BTIM_TS_ITR2                      ((uint16_t)0x0002)
#define  BTIM_TS_ITR3                      ((uint16_t)0x0003)
#define  BTIM_TS_ITR4                      ((uint16_t)0x0004)
#define  BTIM_TS_ITR5                      ((uint16_t)0x0005)
#define  BTIM_TS_ITR6                      ((uint16_t)0x0006)
#define  BTIM_TS_ITR7                      ((uint16_t)0x0007)
#define  BTIM_TS_ITR8                      ((uint16_t)0x0008)
#define  BTIM_TS_ITR9                      ((uint16_t)0x0009)
#define  BTIM_TS_ITR10                     ((uint16_t)0x000A)
#define  BTIM_TS_ITR11                     ((uint16_t)0x000B)
#define  BTIM_TS_ITR12                     ((uint16_t)0x000C)
#define  BTIM_TS_ITR15                     ((uint16_t)0x000F)


/** @defgroup BTIM_External_Trigger_Prescaler 
  * @{
  */
#define  BTIM_ExtTRGPSC_OFF								 	((uint16_t)0x0000)
#define  BTIM_ExtTRGPSC_DIV1_N2 						((uint16_t)0x0001)
#define  BTIM_ExtTRGPSC_DIV1_N4 						((uint16_t)0x0002)
#define  BTIM_ExtTRGPSC_DIV1_N6 						((uint16_t)0x0003)
#define  BTIM_ExtTRGPSC_DIV4_N4 						((uint16_t)0x0004)
#define  BTIM_ExtTRGPSC_DIV4_N6 						((uint16_t)0x0005)
#define  BTIM_ExtTRGPSC_DIV16_N4 						((uint16_t)0x0006)
#define  BTIM_ExtTRGPSC_DIV16_N6 						((uint16_t)0x0007)
	
	
/** @defgroup BTIM_External_Trigger_Polarity 
  * @{
  */ 	
#define  BTIM_ExtTRGPolarity_NonInverted 		((uint32_t)0x00000000)
#define  BTIM_ExtTRGPolarity_Inverted 			((uint32_t)0x00020000)
#define  BTIM_ExtRSTPolarity_NonInverted 		((uint32_t)0x00000000)
#define  BTIM_ExtRSTPolarity_Inverted 			((uint32_t)0x00010000)



void BTIM_TimeBaseInit(BTIM_TypeDef* TIMx, BTIM_TimeBaseInitTypeDef* BTIM_TimeBaseInitStruct);
void BTIM_TOGMutexConfig(BTIM_TypeDef* TIMx, FunctionalState NewState);
void BTIM_Cmd(BTIM_TypeDef* TIMx, FunctionalState NewState);


void BTIM_ITConfig(BTIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState);
void BTIM_ClearITPendingBit(BTIM_TypeDef* TIMx, uint16_t TIM_IT);
ITStatus BTIM_GetITStatus(BTIM_TypeDef* TIMx, uint16_t TIM_IT);


void BTIM_SelectOnePulseMode(BTIM_TypeDef* TIMx, uint16_t TIM_OPMode);
void BTIM_UpdateDisableConfig(BTIM_TypeDef* TIMx, FunctionalState NewState);


/* Clocks management **********************************************************/
void BTIM_ITRxExternalClockConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_InputTriggerSource);
void BTIM_ETRClockModeConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_ExtTRGPrescaler, uint32_t BTIM_ExtTRGPolarity);

void BTIM_ETRResetConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_ExtTRGPrescaler, uint32_t BTIM_ExtRSTPolarity);
void BTIM_SelectInputResetConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_InputResetSource, uint32_t BTIM_ExtPolarity);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE****/
