/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_adc.h"
#include "hc32f4xx_rcc.h"


 #define ADC_SQRxCH0_SampletimeMask 		0x0000000F
 #define ADC_SQRxCH1_SampletimeMask 		0x000000F0
 #define ADC_SQRxCH2_SampletimeMask 		0x00000F00
 #define ADC_SQRxCH3_SampletimeMask 		0x0000F000
 
 #define ADC_SQRxCH0_InputChannelMask 	0x0000003F
 #define ADC_SQRxCH1_InputChannelMask 	0x00000FC0
 #define ADC_SQRxCH2_InputChannelMask 	0x0003F000
 #define ADC_SQRxCH3_InputChannelMask 	0x00FC0000
 
 #define ADC_DMA_LoopOverMask 					0x00000808
 
 
/**
  * @brief  Deinitializes all ADCs peripherals registers to their default reset 
  *         values.
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @retval None
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  if(ADCx == ADC1)
  {
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
	}
	else if (ADCx == ADC2)
	{
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
	}
}


/**
  * @brief  Initializes the ADCx peripheral according to the specified parameters 
  *         in the ADC_InitStruct.  
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  ADC_InitStruct: pointer to an ADC_InitTypeDef structure that contains
  *         the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
	uint32_t patch;
	
	ADCx->CR = (ADC_InitStruct->ADC_ClkDiv << 6) | (ADC_InitStruct->ADC_Range) | (ADC_InitStruct->ADC_OverSamplingMode << 21) | \
							(ADC_InitStruct->ADC_OverSamplingTrigger << 30) | (ADC_InitStruct->ADC_OverSamplingRate << 23) | (ADC_InitStruct->ADC_OverSamplingOffset << 26) ; 
	
	/* FT Patch */
	patch = *(__IO uint32_t*)0x1FFFF078;
	
	if(patch == 0xFFFFFFFF)
	{
		ADC1->DCOFFSET  = *(__IO uint32_t*)0x1FFFF05C;
		ADC1->AMC 			= *(__IO uint32_t*)0x1FFFF060;
		ADC2->DCOFFSET 	= *(__IO uint32_t*)0x1FFFF064;
		ADC2->AMC 			= *(__IO uint32_t*)0x1FFFF068;
	}
	else
	{
	 if(ADC_InitStruct->ADC_Range == ADC_Range_VDDA)
		{
			ADC1->DCOFFSET 	= *(__IO uint32_t*)0x1FFFF05C;
			ADC1->AMC 			= *(__IO uint32_t*)0x1FFFF060;
			ADC2->DCOFFSET 	= *(__IO uint32_t*)0x1FFFF064;
			ADC2->AMC 			= *(__IO uint32_t*)0x1FFFF068;
		}
		else
		{
			ADC1->DCOFFSET 	= *(__IO uint32_t*)0x1FFFF078;
			ADC1->AMC 			= *(__IO uint32_t*)0x1FFFF07C;
			ADC2->DCOFFSET 	= *(__IO uint32_t*)0x1FFFF080;
			ADC2->AMC 			= *(__IO uint32_t*)0x1FFFF084;
		}
	}
}

/**
  * @brief  Initializes the ADCx Sequence Channel according to the specified parameters 
  *         in the ADC_InitStruct.  
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  ADC_SQRx: where x can be 0, 1, 2 or 3
  * @param  ADC_SQRx_TRIG: This parameter can be:
	*					@arg ADC_SQRxTRIG_Software				
	*					@arg ADC_SQRxTRIG_TIM1TRGO1				
	*					@arg ADC_SQRxTRIG_TIM1TRGO2				
	*					@arg ADC_SQRxTRIG_TIM1CC1				
	*					@arg ADC_SQRxTRIG_TIM1CC2				
	*					@arg ADC_SQRxTRIG_TIM1CC3					
	*					@arg ADC_SQRxTRIG_TIM1CC4
	*					@arg ADC_SQRxTRIG_TIM9TRGO
	*					@arg ADC_SQRxTRIG_TIM9CC1
	*					@arg ADC_SQRxTRIG_TIM9CC2
	*					@arg ADC_SQRxTRIG_TIM9CC3
	*					@arg ADC_SQRxTRIG_TIM9CC4
	*					@arg ADC_SQRxTRIG_TIM2TRGO
	*					@arg ADC_SQRxTRIG_TIM3TRGO
	*					@arg ADC_SQRxTRIG_TIM10TRGO
	*					@arg ADC_SQRxTRIG_GPIOLTRGO
	*					@arg ADC_SQRxTRIG_GPIOHTRGO
	*					@arg ADC_SQRxTRIG_VC0TRGO
	*					@arg ADC_SQRxTRIG_VC1TRGO
  * @param  ADC_Ens: This parameter can be: 
	*					@arg ADC_EnsClose
	*					@arg ADC_Ens0to0
	*					@arg ADC_Ens0to1
	*					@arg ADC_Ens0to2
	*					@arg ADC_Ens0to3
  * @retval None
  */
void ADC_SQRInit(ADC_TypeDef* ADCx, uint8_t ADC_SQRx, uint32_t ADC_SQRx_TRIG, uint32_t ADC_Ens)
{
	ADCx->SQRxBOX[ADC_SQRx].SQR_TRIGGER = ADC_SQRx_TRIG;
	ADCx->SQRxBOX[ADC_SQRx].SQR_CFR = (ADC_Ens << 24);
}


/**
  * @brief  Configures for the selected ADC Sequence channel and its sample time. 
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  ADC_SQRx: where x can be 0, 1, 2 or 3
  * @param  ADC_Channelx: where x can be 0, 1, 2 or 3
  * @param  ADC_InputChannel: This parameter can be:		
	*					@arg  ADC_InputChannel_AN0-29
	*					@arg  ADC_InputChannel_VERF1P2
	*					@arg  ADC_InputChannel_VDDA_OneThird
	*					@arg  ADC_InputChannel_TemperatureSensor
	*					@arg  ADC_InputChannel_DACOUT
	*					@arg  ADC_InputChannel_VSSA
	*					@arg  ADC_InputChannel_FDAyINP0INN0
	*					@arg  ADC_InputChannel_FDAyINP1INN1
	*					@arg  ADC_InputChannel_FDAyINP2INN2
  * @param  ADC_SQR_Channel_sampletime: This parameter can be:  
	*					@arg ADC_SampTime6Clk                     
	*					@arg ADC_SampTime7Clk                     
	*					@arg ADC_SampTime9Clk                     
	*					@arg ADC_SampTime12Clk                   
	*					@arg ADC_SampTime18Clk                  
	*					@arg ADC_SampTime24Clk                  
	*					@arg ADC_SampTime30Clk                 
	*					@arg ADC_SampTime42Clk                   
	*					@arg ADC_SampTime54Clk                  
	*					@arg ADC_SampTime70Clk                  
	*					@arg ADC_SampTime102Clk                   
	*					@arg ADC_SampTime134Clk                  
	*					@arg ADC_SampTime166Clk                  
	*					@arg ADC_SampTime198Clk                  
	*					@arg ADC_SampTime262Clk                 
	*					@arg ADC_SampTime390Clk   
  * @retval None
  */
void ADC_SQRSampleConfig(ADC_TypeDef* ADCx, uint32_t ADC_SQRx, uint32_t ADC_Channelx, uint32_t ADC_InputChannel, uint32_t ADC_SQR_Channel_sampletime)
{	
	switch(ADC_Channelx)
	{
		case 0: 
						ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		&= ~ADC_SQRxCH0_InputChannelMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	&= ~ADC_SQRxCH0_SampletimeMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		|= ADC_InputChannel;
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	|= ADC_SQR_Channel_sampletime;
						break;
		case 1:	ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		&= ~ADC_SQRxCH1_InputChannelMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	&= ~ADC_SQRxCH1_SampletimeMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		|= (ADC_InputChannel << 6);
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR  |= (ADC_SQR_Channel_sampletime << 4);
						break;
		case 2:	ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		&= ~ADC_SQRxCH2_InputChannelMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	&= ~ADC_SQRxCH2_SampletimeMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		|= (ADC_InputChannel << 12);
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR  |= (ADC_SQR_Channel_sampletime << 8);
						break;
		case 3:	ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		&= ~ADC_SQRxCH3_InputChannelMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	&= ~ADC_SQRxCH3_SampletimeMask;
						ADCx->SQRxBOX[ADC_SQRx].SQR_CFR 		|= (ADC_InputChannel << 18);
						ADCx->SQRxBOX[ADC_SQRx].SQR_SMPCFR 	|= (ADC_SQR_Channel_sampletime << 12);
						break;
		default:
						break;
	}
}


/**
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    ADCx->CR |= (uint32_t)ADC_CR_EN;
  }
  else
  {
    ADCx->CR &= (uint32_t)(~ADC_CR_EN);
  }
}


/**
  * @brief  Enables the selected ADC software start conversion of the regular channels.
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @retval None
  */
void ADC_SoftwareStartConv(ADC_TypeDef* ADCx)
{
  ADCx->START = (uint32_t)ADC_START_EN;
}


/**
  * @brief  Get the ADCx conversion result data of Sequence channel.
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @retval The Data conversion value.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx, uint32_t ADC_SQRx, uint32_t ADC_Channelx)
{
  uint16_t tmpreg = 0;
	
	switch(ADC_Channelx)
	{
		case 0: tmpreg = ADCx->SQRxBOX[ADC_SQRx].SQR_RESULT0;
						break;
		case 1:	tmpreg = ADCx->SQRxBOX[ADC_SQRx].SQR_RESULT1;
						break;
		case 2:	tmpreg = ADCx->SQRxBOX[ADC_SQRx].SQR_RESULT2;
						break;
		case 3:	tmpreg = ADCx->SQRxBOX[ADC_SQRx].SQR_RESULT3;
						break;
		default:
						break;
	}
	
	if(tmpreg == 0x0800) tmpreg = 0;
	
  return (uint16_t)tmpreg;
}


 /**
  * @brief  Enables or disables the specified ADC DMA Loop mode.
  * @param  ADCx: where x can be 1, 2to select the ADC peripheral.
	* @param  ADC_DMALOOP:This parameter can be: 
	*					@arg ADC_DMALoop_Disable
	*					@arg ADC_DMALoop_Enable
	* @param  ADC_DMAOVER:This parameter can be: 
	*					@arg ADC_DMAOverflow_Disable
	*					@arg ADC_DMAOverflow_Enable
  * @retval None
  */
void ADC_DMAConfig(ADC_TypeDef* ADCx, uint32_t ADC_DMALOOP, uint32_t ADC_DMAOVER)
{	
	ADCx->CR &= ~ADC_DMA_LoopOverMask;
  ADCx->CR |= (ADC_DMALOOP << 11) | (ADC_DMAOVER << 3);
}


 /**
  * @brief  Enables or disables the specified ADC DMA request.
  * @param  ADCx: where x can be 1, 2to select the ADC peripheral.
  * @param  NewState: new state of the selected ADC DMA transfer.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    ADCx->CR |= (uint32_t)ADC_CR_DMAEN;
  }
  else
  {
    ADCx->CR &= (uint32_t)(~ADC_CR_DMAEN);
  }	
}


/**
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled. 
  *          This parameter can be one of the following values:
  *            @arg ADC_IT_EOC                          
  *            @arg ADC_IT_EOS0                        
  *            @arg ADC_IT_EOS1                         
  *            @arg ADC_IT_EOS2                        
  *            @arg ADC_IT_EOS3                        
  *            @arg ADC_IT_AWDTL                        
  *            @arg ADC_IT_AWDTH                         
  *            @arg ADC_IT_AWDTR                         
  *            @arg ADC_IT_TRIGERR                       
  *            @arg ADC_IT_EOSMP                    
  *            @arg ADC_IT_OVER                                            
  * @param  NewState: new state of the specified ADC interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState)  
{
	if (NewState != DISABLE)
	{
		ADCx->IER |= (uint32_t)ADC_IT;
	}
	else
	{
		ADCx->IER &= (uint32_t)(~ADC_IT);
	}
}


/**
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @param  ADC_FLAG: specifies the flag to check. 
  *            @arg ADC_FLAG_EOC                          
  *            @arg ADC_FLAG_EOS0                        
  *            @arg ADC_FLAG_EOS1                         
  *            @arg ADC_FLAG_EOS2                        
  *            @arg ADC_FLAG_EOS3                        
  *            @arg ADC_FLAG_AWDTL                        
  *            @arg ADC_FLAG_AWDTH                         
  *            @arg ADC_FLAG_AWDTR                         
  *            @arg ADC_FLAG_TRIGERR                       
  *            @arg ADC_FLAG_EOSMP                    
  *            @arg ADC_FLAG_OVER                                                  
  * @retval The new state of ADC_FLAG (SET or RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((ADCx->ISR & ADC_FLAG) != (uint8_t)RESET)
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
  * @brief  Clears the ADCx's pending flags.
  * @param  ADCx: where x can be 1, 2 to select the ADC peripheral.
  * @param  ADC_FLAG: specifies the flag to clear. 
  *            @arg ADC_FLAG_EOC                          
  *            @arg ADC_FLAG_EOS0                        
  *            @arg ADC_FLAG_EOS1                         
  *            @arg ADC_FLAG_EOS2                        
  *            @arg ADC_FLAG_EOS3                        
  *            @arg ADC_FLAG_AWDTL                        
  *            @arg ADC_FLAG_AWDTH                         
  *            @arg ADC_FLAG_AWDTR                         
  *            @arg ADC_FLAG_TRIGERR                       
  *            @arg ADC_FLAG_EOSMP                    
  *            @arg ADC_FLAG_OVER                          
  * @retval None
  */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  ADCx->ICR &= (uint32_t)(~ADC_FLAG);
}


/**
  * @brief  Enables or disables the temperature sensor channels.
  * @param  NewState: new state of the temperature sensor and Vrefint channels.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_TempSensorCmd(ADC_TypeDef* ADCx, FunctionalState NewState)                
{
  if (NewState != DISABLE)
  {
    ADCx->CR |= (uint32_t)ADC_CR_TSEN;
  }
  else
  {
    ADCx->CR &= (uint32_t)(~ADC_CR_TSEN);
  }	
}


/**
  * @brief  Enables or disables the analog watchdog channel
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @param  ADC_AnalogWatchdog: the ADC analog watchdog configuration.
  *         This parameter can be one of the following values:
  *            @arg ADC_AnalogWatchdog_AN0							
  *            @arg ADC_AnalogWatchdog_AN1								
  *            @arg ADC_AnalogWatchdog_AN2							
  *            @arg ADC_AnalogWatchdog_AN3					
  *            @arg ADC_AnalogWatchdog_AN4						
  *            @arg ADC_AnalogWatchdog_AN5								
  *            @arg ADC_AnalogWatchdog_AN6							
  *            @arg ADC_AnalogWatchdog_AN7							
  *            @arg ADC_AnalogWatchdog_AN8						
  *            @arg ADC_AnalogWatchdog_AN9							
  *            @arg ADC_AnalogWatchdog_AN10							
  *            @arg ADC_AnalogWatchdog_AN11						
  *            @arg ADC_AnalogWatchdog_AN12								
  *            @arg ADC_AnalogWatchdog_AN13							
  *            @arg ADC_AnalogWatchdog_AN14						
  *            @arg ADC_AnalogWatchdog_AN15					
  *            @arg ADC_AnalogWatchdog_CORE1P2						
  *            @arg ADC_AnalogWatchdog_VDDAOneThird				
  *            @arg ADC_AnalogWatchdog_TemperatureSensor	
  *            @arg ADC_AnalogWatchdog_FDAyOUTP						
  *            @arg ADC_AnalogWatchdog_FDAyOUTN						

  * @param  NewState: new state of the temperature sensor and Vrefint channels.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None	  
  */
void ADC_AnalogWatchdog0Cmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog_Channel, FunctionalState NewState)
{
	if (NewState != DISABLE)
  {
		ADCx->AWDCR0 |= (uint32_t)ADC_AnalogWatchdog_Channel;
	}
	else
	{
		ADCx->AWDCR0 &= (uint32_t)(~ADC_AnalogWatchdog_Channel);
	}
}


/**
  * @brief  Enables or disables the analog watchdog channel
  * @param  ADCx: where x can be 1, 2 or 3 to select the ADC peripheral.
  * @param  ADC_AnalogWatchdog: the ADC analog watchdog configuration.
  *         This parameter can be one of the following values:
  *            @arg ADC_AnalogWatchdog_AN16								
  *            @arg ADC_AnalogWatchdog_AN17								
  *            @arg ADC_AnalogWatchdog_AN18								
  *            @arg ADC_AnalogWatchdog_AN19								
  *            @arg ADC_AnalogWatchdog_AN20								
  *            @arg ADC_AnalogWatchdog_AN21								
  *            @arg ADC_AnalogWatchdog_AN22								
  *            @arg ADC_AnalogWatchdog_AN23								
  *            @arg ADC_AnalogWatchdog_AN24								
  *            @arg ADC_AnalogWatchdog_AN25								
  *            @arg ADC_AnalogWatchdog_AN26								
  *            @arg ADC_AnalogWatchdog_AN27								
  *            @arg ADC_AnalogWatchdog_AN28								
  *            @arg ADC_AnalogWatchdog_AN29							
  * @param  NewState: new state of the temperature sensor and Vrefint channels.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None	  
  */
void ADC_AnalogWatchdog1Cmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog_Channel, FunctionalState NewState)
{
	if (NewState != DISABLE)
  {
		ADCx->AWDCR1 |= (uint32_t)ADC_AnalogWatchdog_Channel;
	}
	else
	{
		ADCx->AWDCR1 &= (uint32_t)(~ADC_AnalogWatchdog_Channel);
	}
}


/**
  * @brief  Configures the high and low thresholds of the ADC analog watchdog.
  * @param  ADCx: where x can be 1, 2  to select the ADC peripheral.
  * @param  HighThreshold:This parameter must be a 12-bit value.
  * @param  LowThreshold: This parameter must be a 12-bit value.
  * @retval None
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold)
{
  ADCx->AWDVTH = HighThreshold;
  ADCx->AWDVTL = LowThreshold;
}
