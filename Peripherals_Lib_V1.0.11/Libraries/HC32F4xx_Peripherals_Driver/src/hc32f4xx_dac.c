/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_dac.h"
#include "hc32f4xx_rcc.h"
#include "hc32f4xx_syscfg.h"


#define DAC_CR_TESLMask 0x000000E0


/**
  * @brief  Deinitializes the DAC peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void DAC_DeInit(void)
{
 
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
 
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
}


/**
  * @brief  Initializes the DAC peripheral according to the specified parameters
  *         in the DAC_InitStruct.
  * @param  DAC_InitStruct: pointer to a DAC_InitTypeDef structure that contains
  *         the configuration information for the  specified DAC channel.
  * @retval None
  */
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct)
{
	DAC->CR = (DAC_InitStruct->DAC_Ref << 1) | (DAC_InitStruct->DAC_out << 3);
}


/**
  * @brief  Enables or disables the specified DAC channel.
  * @param  NewState: new state of the DAC channel. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DAC_Cmd(FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		DAC->CR |= (uint32_t)DAC_CR_EN;
	}
	else
	{
		DAC->CR &= (uint32_t)(~DAC_CR_EN);
	}
}


/**
  * @brief  Set the specified data register value for DAC.
  * @param  Data: Data to be loaded in the selected data holding register.
  * @retval None
  */
void DAC_SetDACData(uint16_t Data)
{
	DAC->DR = Data;
}


/**
  * @brief  Configures for  the selected DAC DMA trigger singal.
  * @param  DAC_DMATrigger: This parameter can be
	*					@arg DAC_DMA_TIM6TRGO
	*					@arg DAC_DMA_TIM1TRGO		
	*					@arg DAC_DMA_TIM7TRGO		
	*					@arg DAC_DMA_TIM5TRGO		
	*					@arg DAC_DMA_TIM2TRGO		
	*					@arg DAC_DMA_TIM4TRGO		
	*					@arg DAC_DMA_EXTI9
  * @retval None
  */
void DAC_DMATriggerConfig(uint32_t DAC_DMATrigger)
{
	DAC->CR &= (uint32_t)(~DAC_CR_TESLMask);
	DAC->CR |= (DAC_DMATrigger << 5);
}


/**
  * @brief  Enables or disables the specified DAC channel DMA request.
  * @param  NewState: new state of the selected DAC channel DMA request.
  *          This parameter can be: ENABLE or DISABLE. 
  * @retval None
  */
void DAC_DMACmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    DAC->CR |= (uint32_t)(DAC_CR_DMAEN);
  }
  else
  {
    DAC->CR &= (uint32_t)(~DAC_CR_DMAEN);
  }
}
