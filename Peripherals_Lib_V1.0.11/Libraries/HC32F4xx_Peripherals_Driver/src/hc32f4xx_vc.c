/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_vc.h"
#include "hc32f4xx_rcc.h"




/**
  * @brief  De-initializes the VCx peripheral registers to their default reset values.
  * @param  VCx: where x can be 1, 2, 3 to select the VC peripheral. 
  * @retval None
  */
void VC_DeInit(VC_TypeDef* VCx)
{

  if (VCx == VC0)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_VC0, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_VC0, DISABLE);
  }
  else if (VCx == VC1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_VC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_VC1, DISABLE);
  }
  else if (VCx == VC2)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_VC2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_VC2, DISABLE);
  }
}


/**
  * @brief  Initializes the VCx peripheral according to the specified parameters 
  *         in the VC_InitStruct.  
  * @param  VCx: where x can be 1, 2, 3 to select the VC peripheral.
  * @param  VC_InitStruct: pointer to an VC_InitTypeDef structure that contains
  *         the configuration information for the specified VC peripheral.
  * @retval None
  */
void VC_Init(VC_TypeDef* VCx, VC_InitTypeDef* VC_InitStruct)
{
	VCx->CR0 = (uint32_t)((VC_InitStruct->VC_INN_CH << 9) | ( VC_InitStruct->VC_INP_CH << 6) |\
												(VC_InitStruct->VC_Hys << 2) | (VC_InitStruct->VC_ResponseSpeed << 1) | \
												(VC_InitStruct->VC_OutPolarity << 4));
	
	VCx->CR1 = (uint32_t)((VC_InitStruct->VC_FilterClkSoure << 4) | (VC_InitStruct->VC_FilterPrescaler));
}


/**
  * @brief  Enables or disables the specified VC peripheral.
  * @param  VCx: where x can be 1, 2, 3 to select the VC peripheral.
  * @param  NewState: new state of the VCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void VC_Cmd(VC_TypeDef* VCx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    VCx->CR0 |= (uint32_t)VC_CR0_EN;
  }
  else
  {
    VCx->CR0 &= (uint32_t)(~VC_CR0_EN);
  }
}


/**
  * @brief  Enables or disables Window to the specified VC peripheral.
  * @param  VCx: where x can be 2, 3 to select the VC peripheral.
  * @param  NewState: new state of the VCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void VC_WindowCmd(VC_TypeDef* VCx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    VCx->CR0 |= (uint32_t)VC_CR0_Window;
  }
  else
  {
    VCx->CR0 &= (uint32_t)(~VC_CR0_Window);
  }
}


/**
  * @brief  Enables or disables the specified VC peripheral.
  * @param  VCx: where x can be 1, 2, 3 to select the VC peripheral.
  * @param  VC_BlankOutPolarity: 
  *            @arg VC_BlankOutPolarity_Low
  *            @arg VC_BlankOutPolarity_High
  * @param  VC_BlankCLK
  *            @arg VC_BlankCLK_4
  *            @arg VC_BlankCLK_8
  *            @arg VC_BlankCLK_16
  *            @arg VC_BlankCLK_32
  *            @arg VC_BlankCLK_64
  *            @arg VC_BlankCLK_128
  *            @arg VC_BlankCLK_256
  *            @arg VC_BlankCLK_512
  * @param  VC_TriggerSource: 
  *            @arg VC_TriggerSource_TIM1OC1REFC
  *            @arg VC_TriggerSource_TIM1OC2REFC
  *            @arg VC_TriggerSource_TIM1OC3REFC
  *            @arg VC_TriggerSource_TIM1OC4REFC
  * @retval None
  */
void VC_BlankConfig(VC_TypeDef* VCx, uint32_t VC_BlankOutPolarity, uint32_t VC_BlankCLK, uint32_t VC_TriggerSource)
{
	  VCx->CR1 &= (uint32_t)~VC_CR1_BLANKMASK;
    VCx->CR1 |= (uint32_t)((VC_BlankOutPolarity << 17) |  (VC_BlankCLK << 14) | (VC_TriggerSource));
}



/**
  * @param  VCx: where x can be 0, 1, 2 to select the VC peripheral.
  * @param  VC_IT: specifies the VC interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg VC_IT_Rising:  
  *            @arg VC_IT_Falling:
  *            @arg VC_IT_HighLevel
  * @param  NewState: new state of the VC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void VC_ITConfig(VC_TypeDef* VCx, uint32_t VC_IT, FunctionalState NewState)
{  
  if (NewState != DISABLE)
  {
    VCx->CR1 |= VC_IT;
		VCx->CR0 |= VC_CR0_IE;
  }
  else
  {
    VCx->CR1 &= (uint32_t)~VC_IT;
    VCx->CR0 &= (uint32_t)~VC_CR0_IE;
  }
}

/**
  * @brief  Checks whether the specified VC flag is set or not.
  * @param  VCx: where x can be 0, 1, 2  to select the VC peripheral.
  * @param  VC_FLAG: specifies the flag to check. 
  *            @arg VC_FLAG_FLTV                          
  *            @arg VC_FLAG_INTF                                                                         
  * @retval The new state of VC_FLAG (SET or RESET).
  */
FlagStatus VC_GetFlagStatus(VC_TypeDef* VCx, uint8_t VC_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((VCx->SR & VC_FLAG) != (uint8_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return  bitstatus;
}


/**
  * @brief  Clears the VCx's pending flags.
  * @param  VCx: where x can be 0, 1, 2  to select the VC peripheral.                         
  * @retval None
  */
void VC_ClearFlag(VC_TypeDef* VCx)
{
  VCx->SR = 0;
}

