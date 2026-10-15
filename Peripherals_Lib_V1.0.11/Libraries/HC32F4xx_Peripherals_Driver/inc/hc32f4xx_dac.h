#ifndef __HC32F4xx_DAC_H
#define __HC32F4xx_DAC_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define DAC_CR_EN 			0x00000001
#define DAC_CR_DMAEN 		0x00000100

typedef struct
{
  uint32_t DAC_Ref; 
  uint32_t DAC_out; 
}DAC_InitTypeDef;

  
void DAC_DeInit(void);
void DAC_Cmd(FunctionalState NewState);


/** @defgroup DAC_Ref
  * @{
  */
#define DAC_Ref1P2				0x0
#define DAC_Ref3P0				0x1


/** @defgroup DAC_Out
  * @{
  */
#define DAC_Out_Disable		0x0
#define DAC_Out_Enable		0x1


/** @defgroup DAC_DMATrigger
  * @{
  */
#define DAC_DMA_TIM6TRGO		0x0
#define DAC_DMA_TIM1TRGO		0x1
#define DAC_DMA_TIM7TRGO		0x2
#define DAC_DMA_TIM5TRGO		0x3
#define DAC_DMA_TIM2TRGO		0x4
#define DAC_DMA_TIM4TRGO		0x5
#define DAC_DMA_EXTI9				0x6


void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);
void DAC_Cmd(FunctionalState NewState);
void DAC_SetDACData(uint16_t Data);
void DAC_DMATriggerConfig(uint32_t DAC_DMATrigger);
void DAC_DMACmd(FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif 



/******END OF FILE****/
