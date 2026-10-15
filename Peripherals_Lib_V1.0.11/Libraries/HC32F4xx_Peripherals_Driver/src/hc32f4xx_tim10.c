/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_tim10.h"
#include "hc32f4xx_rcc.h"


#define CR1_TOGEN							((uint16_t)0x8000) 

#define SMCR_TRGIPOL_MASK			((uint32_t)0x00020000) 	
#define SMCR_RSTIPOL_MASK			((uint32_t)0x00010000) 	
#define SMCR_TRGISRC_MASK			((uint32_t)0x00000780) 		
#define SMCR_RSTISRC_MASK			((uint32_t)0x00000078) 	
#define SMCR_SMS_MASK      		((uint32_t)0x00000007)
#define SMCR_MSM_MASK      		((uint32_t)0x00000800)
#define SMCR_BETR_MASK      	((uint32_t)0x00000F87)




/**
  * @brief  Deinitializes the TIM10 peripheral registers to their default reset values.
  * @retval None
  */
void BTIM_DeInit()
{
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM10, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM10, DISABLE);   
}


/**
  * @brief  Initializes the TIM10 Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be 10 to select the TIM peripheral.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef structure
  *         that contains the configuration information for the specified TIM peripheral.
  * @retval None
  */
void BTIM_TimeBaseInit(BTIM_TypeDef* TIMx, BTIM_TimeBaseInitTypeDef* BTIM_TimeBaseInitStruct) 
{
  /* Set the Autoreload value */
  TIMx->ARR = BTIM_TimeBaseInitStruct->BTIM_Period ;
 
  /* Set the Prescaler value */
  TIMx->PSC = BTIM_TimeBaseInitStruct->BTIM_Prescaler;
    
  /* Generate an update event to reload the Prescaler */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;          
}

/**
  * @brief  Enables or disables the specified TIM10 peripheral.
  * @param  TIMx: where x can be 10 to select the TIMx peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BTIM_Cmd(BTIM_TypeDef* TIMx, FunctionalState NewState)
{
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_CEN;
  }
}


/**
  * @brief  Enables or disables the TOG pin output waveform polarity Mutex
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  NewState: new state of the TIM interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BTIM_TOGMutexConfig(BTIM_TypeDef* TIMx, FunctionalState NewState)
{  
  if (NewState != DISABLE)
  {
    TIMx->CR1 |= CR1_TOGEN;
  }
  else
  {
    TIMx->CR1 &= (uint16_t)~CR1_TOGEN;
  }
}


/**
  * @brief  Selects the TIM10 One Pulse Mode.
  * @param  TIMx: where x can be 1 to 9 to select the TIM peripheral.
  * @param  TIM_OPMode: specifies the OPM Mode to be used.
  *          This parameter can be one of the following values:
  *            @arg TIM_OPMode_Single
  *            @arg TIM_OPMode_Repetitive
  * @retval None
  */
void BTIM_SelectOnePulseMode(BTIM_TypeDef* TIMx, uint16_t TIM_OPMode)
{
  /* Reset the OPM Bit */
  TIMx->CR1 &= (uint16_t)~TIM_CR1_OPM;

  /* Configure the OPM Mode */
  TIMx->CR1 |= TIM_OPMode;
}


/**
  * @brief  Enables or Disables the TIM10 Update event.
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx UDIS bit
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BTIM_UpdateDisableConfig(BTIM_TypeDef* TIMx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit */
    TIMx->CR1 |= TIM_CR1_UDIS;
  }
  else
  {
    /* Reset the Update Disable Bit */
    TIMx->CR1 &= (uint16_t)~TIM_CR1_UDIS;
  }
}


/**
  * @brief  Selects the TIM10 Trigger Output Mode.
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  TIM_TRGOSource: specifies the Trigger Output source.
  *   This parameter can be one of the following values:
  *            @arg BTIM_TRGOSource_Reset:  The UG bit in the TIM_EGR register is used as the trigger output(TRGO)
  *            @arg BTIM_TRGOSource_Enable: The Counter Enable CEN is used as the trigger output(TRGO)
  *            @arg BTIM_TRGOSource_Update: The update event is selected as the trigger output(TRGO)
  *
  * @retval None
  */
void BTIM_SelectOutputTrigger(BTIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{

  /* Reset the MMS Bits */
  TIMx->CR2 &= (uint16_t)~TIM_CR2_MMS;
  /* Select the TRGO source */
  TIMx->CR2 |=  TIM_TRGOSource;
}


/**
  * @brief  Selects the Input Trigger source
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  BTIM_InputResetSource: The Input Trigger Reset source.
  *          This parameter can be one of the following values:
  *            @arg BTIM_TS_ITR0: 	I2C1_SCL
  *            @arg BTIM_TS_ITR1: 	I2C2_SCL
  *            @arg BTIM_TS_ITR2: 	VC0_OUT
  *            @arg BTIM_TS_ITR3: 	VC1_OUT
  *            @arg BTIM_TS_ITR4: 	VC2_OUT
  *            @arg BTIM_TS_ITR5: 	UART4_RXD
  *            @arg BTIM_TS_ITR6:		SPI1_NSS
  *            @arg BTIM_TS_ITR7: 	TIM1_TRGO
  *            @arg BTIM_TS_ITR8: 	TIM8_TRGO
  *            @arg BTIM_TS_ITR9: 	TIM2_TRGO
  *            @arg BTIM_TS_ITR10: 	TIM3_TRGO
  *            @arg BTIM_TS_ITR11: 	TIM9_TRGO
  *            @arg BTIM_TS_ITR12: 	SPI2_NSS
  *            @arg BTIM_TS_ITR15: 	TIM10_ETR
  * @param  BTIM_ExtPolarity: The external Trigger Polarity.
  *          This parameter can be one of the following values:
  *            @arg BTIM_ExtRSTPolarity_Inverted: active low or falling edge active.
  *            @arg BTIM_ExtRSTPolarity_NonInverted: active high or rising edge active.
  * @retval None
  */
void BTIM_SelectInputResetConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_InputResetSource, uint32_t BTIM_ExtPolarity)
{
  uint32_t tmpsmcr = 0;

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMCR.RSTISRC Bits */
  tmpsmcr &= (uint32_t)~(SMCR_RSTISRC_MASK | SMCR_RSTIPOL_MASK);

  /* Set the Input Trigger source and Polarity*/
  tmpsmcr |= (uint32_t)((BTIM_InputResetSource << 3) | (uint32_t)(BTIM_ExtPolarity));

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}


/**
  * @brief  Selects the Input Trigger source
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  BTIM_InputTriggerSource: The Input Trigger source.
  *          This parameter can be one of the following values:
  *            @arg BTIM_TS_ITR0: 	I2C1_SCL
  *            @arg BTIM_TS_ITR1: 	I2C2_SCL
  *            @arg BTIM_TS_ITR2: 	VC0_OUT
  *            @arg BTIM_TS_ITR3: 	VC1_OUT
  *            @arg BTIM_TS_ITR4: 	VC2_OUT
  *            @arg BTIM_TS_ITR5: 	UART4_RXD
  *            @arg BTIM_TS_ITR6:		SPI1_NSS
  *            @arg BTIM_TS_ITR7: 	TIM1_TRGO
  *            @arg BTIM_TS_ITR8: 	TIM8_TRGO
  *            @arg BTIM_TS_ITR9: 	TIM2_TRGO
  *            @arg BTIM_TS_ITR10: 	TIM3_TRGO
  *            @arg BTIM_TS_ITR11: 	TIM9_TRGO
  *            @arg BTIM_TS_ITR12: 	SPI2_NSS
  *            @arg BTIM_TS_ITR15: 	TIM10_ETR

  * @retval None
  */
void BTIM_SelectInputTrigger(BTIM_TypeDef* TIMx, uint32_t BTIM_InputTriggerSource)
{
  uint32_t tmpsmcr = 0;

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMCR.TRGISRC Bits */
  tmpsmcr &= (uint32_t)~SMCR_TRGISRC_MASK;

  /* Set the Input Trigger source */
  tmpsmcr |= (uint32_t)(BTIM_InputTriggerSource << 7);

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}


/**
  * @brief  Configures the TIMx Internal Trigger as External Clock
  * @param  TIMx: where x can be  1 to 9 to select the TIM peripheral.
  *         peripheral.
  * @param  TIM_InputTriggerSource: Trigger source.
  *          This parameter can be one of the following values:
  *            @arg BTIM_TS_ITR0: 	I2C1_SCL
  *            @arg BTIM_TS_ITR1: 	I2C2_SCL
  *            @arg BTIM_TS_ITR2: 	VC0_OUT
  *            @arg BTIM_TS_ITR3: 	VC1_OUT
  *            @arg BTIM_TS_ITR4: 	VC2_OUT
  *            @arg BTIM_TS_ITR5: 	UART4_RXD
  *            @arg BTIM_TS_ITR6:		SPI1_NSS
  *            @arg BTIM_TS_ITR7: 	TIM1_TRGO
  *            @arg BTIM_TS_ITR8: 	TIM8_TRGO
  *            @arg BTIM_TS_ITR9: 	TIM2_TRGO
  *            @arg BTIM_TS_ITR10: 	TIM3_TRGO
  *            @arg BTIM_TS_ITR11: 	TIM9_TRGO
  *            @arg BTIM_TS_ITR12: 	SPI2_NSS
  * @retval None
  */
void BTIM_ITRxExternalClockConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_InputTriggerSource)
{

  /* Select the Internal Trigger */
  BTIM_SelectInputTrigger(TIMx, BTIM_InputTriggerSource);

  /* Select SMS bit to the External clock mode */
  TIMx->SMCR |= BTIM_SlaveMode_External;
}


/**
  * @brief  Configures the TIM10 External Trigger (ETR).
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  BTIM_ExtTRGPrescaler: The external Trigger Prescaler and Sampling Point .
  *            @arg BTIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *            @arg BTIM_ExtTRGPSC_DIV1_N2: 	ETRP frequency divided = 1 , 	N = 2 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N4: 	ETRP frequency divided = 1 , 	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N6: 	ETRP frequency divided = 1 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N4: 	ETRP frequency divided = 4 ,	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N6: 	ETRP frequency divided = 4 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N4: 	ETRP frequency divided = 16 , N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N6: 	ETRP frequency divided = 16 , N = 6 .
  * @param  BTIM_ExtPolarity: The external Trigger Polarity.
  *          This parameter can be one of the following values:
  *            @arg BTIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *            @arg BTIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  *            @arg BTIM_ExtRSTPolarity_Inverted: active low or falling edge active.
  *            @arg BTIM_ExtRSTPolarity_NonInverted: active high or rising edge active.
  * @retval None
  */
void BTIM_ETRConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_ExtTRGPrescaler, uint32_t BTIM_ExtPolarity)
{
  uint32_t tmpsmcr = 0;

  tmpsmcr = TIMx->SMCR;

  /* Reset the MSM TRGISRC SMS Pol Bits */
  tmpsmcr &= ~(SMCR_BETR_MASK);

  /* Set the Prescaler, the Filter value and the Polarity */
  tmpsmcr |= (uint32_t)((uint32_t)(BTIM_ExtTRGPrescaler << 12) | (uint32_t)(BTIM_ExtPolarity));

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}



/**
  * @brief  Configures the External clock Mode
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  BTIM_ExtTRGPrescaler: The external Trigger Prescaler and Sampling Point .
  *            @arg BTIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *            @arg BTIM_ExtTRGPSC_DIV1_N2: 	ETRP frequency divided = 1 , 	N = 2 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N4: 	ETRP frequency divided = 1 , 	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N6: 	ETRP frequency divided = 1 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N4: 	ETRP frequency divided = 4 ,	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N6: 	ETRP frequency divided = 4 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N4: 	ETRP frequency divided = 16 , N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N6: 	ETRP frequency divided = 16 , N = 6 .
  * @param  TIM_ExtTRGPolarity: The external Trigger Polarity.
  *            @arg BTIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *            @arg BTIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  * @retval None
  */
void BTIM_ETRClockModeConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_ExtTRGPrescaler, uint32_t BTIM_ExtTRGPolarity)
{
  uint32_t tmpsmcr = 0;

  /* Configure the ETR Clock source */
  BTIM_ETRConfig(TIMx, BTIM_ExtTRGPrescaler, BTIM_ExtTRGPolarity);
  
  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;

  /* Reset the SMS Bits */
  tmpsmcr &= (uint32_t)~TIM_SMCR_SMS;

  /* Select SMCR.SMS to the External clock mode */
  tmpsmcr |= BTIM_SlaveMode_External;

  /* Select the Trigger selection : ETRF */
  tmpsmcr |= SMCR_TRGISRC_MASK;

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}


/**
  * @brief  Configures the External Reset Mode
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  BTIM_ExtTRGPrescaler: The external Trigger Prescaler and Sampling Point .
  *            @arg BTIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *            @arg BTIM_ExtTRGPSC_DIV1_N2: 	ETRP frequency divided = 1 , 	N = 2 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N4: 	ETRP frequency divided = 1 , 	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV1_N6: 	ETRP frequency divided = 1 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N4: 	ETRP frequency divided = 4 ,	N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV4_N6: 	ETRP frequency divided = 4 , 	N = 6 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N4: 	ETRP frequency divided = 16 , N = 4 .
  *            @arg BTIM_ExtTRGPSC_DIV16_N6: 	ETRP frequency divided = 16 , N = 6 .
  * @param  TIM_ExtRSTPolarity: The external Trigger Polarity.
  *            @arg BTIM_ExtRSTPolarity_Inverted: active low or falling edge active.
  *            @arg BTIM_ExtRSTPolarity_NonInverted: active high or rising edge active.
  * @retval None
  */
void BTIM_ETRResetConfig(BTIM_TypeDef* TIMx, uint32_t BTIM_ExtTRGPrescaler, uint32_t BTIM_ExtRSTPolarity)
{
  uint32_t tmpsmcr = 0;

  /* Configure the ETR Clock source */
  BTIM_ETRConfig(TIMx, BTIM_ExtTRGPrescaler, BTIM_ExtRSTPolarity);
  
  /* Select the Trigger selection : ETRF */
  tmpsmcr |= SMCR_RSTISRC_MASK;

  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}


/**
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  TIM_IT: specifies the TIM interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg TIM_IT_Update:  TIM update Interrupt source
  *            @arg TIM_IT_Trigger: TIM Trigger Interrupt source 
  * @param  NewState: new state of the TIM interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void BTIM_ITConfig(BTIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  if (NewState != DISABLE)
  {
    TIMx->IER |= TIM_IT;
  }
  else
  {
    TIMx->IER &= (uint16_t)~TIM_IT;
  }
}


/**
  * @brief  Checks whether the TIM interrupt has occurred or not.
  * @param  TIMx: where x can be  1 to 9 to select the TIM peripheral.
  * @param  TIM_IT: specifies the TIM interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg TIM_IT_Update:  TIM update Interrupt source
  *            @arg TIM_IT_Trigger: TIM Trigger Interrupt source 
  * @retval The new state of the TIM_IT(SET or RESET).
  */
ITStatus BTIM_GetITStatus(BTIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;
   
  itstatus = TIMx->ISR & TIM_IT;
  
  itenable = TIMx->IER & TIM_IT;
  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}


/**
  * @brief  Clears the TIMx's interrupt pending bits.
  * @param  TIMx: where x can be  10 to select the TIM peripheral.
  * @param  TIM_IT: specifies the pending bit to clear.
  *          This parameter can be any combination of the following values:
  *            @arg TIM_IT_Update: TIM1 update Interrupt source
  *            @arg TIM_IT_Trigger: TIM Trigger Interrupt source    
  * @retval None
  */
void BTIM_ClearITPendingBit(BTIM_TypeDef* TIMx, uint16_t TIM_IT)
{

  /* Clear the IT pending Bit */
  TIMx->ISR = (uint16_t)~TIM_IT;
}


/*****END OF FILE****/
