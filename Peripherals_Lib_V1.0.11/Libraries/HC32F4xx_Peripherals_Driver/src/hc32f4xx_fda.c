/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_fda.h"
#include "hc32f4xx_rcc.h"


/**
  * @brief  Deinitializes the FDAx peripheral registers to their default reset values.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @retval None
  */
void FDA_DeInit(FDA_TypeDef* FDAx)
{
	 if (FDAx == FDA0)
  {
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_FDA0, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_FDA0, DISABLE);
  } 
	else if (FDAx == FDA1)
  {      
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_FDA1, ENABLE);
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_FDA1, DISABLE);  
   } 
}


/**
  * @brief  Initializes the FDA peripheral according to the specified parameters
  *         in the FDA_InitStruct.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @param  FDA_InitStruct: pointer to a FDA_InitTypeDef structure that contains
  *         the configuration information for the  specified FDA channel.
  * @retval None
  */
void FDA_Init(FDA_TypeDef* FDAx, FDA_InitTypeDef* FDA_InitStruct)
{
	uint32_t FDA_Trim;
	
	FDAx->CR = (uint32_t)(FDA_InitStruct->FDA_InputSignal | FDA_InitStruct->FDA_CKD | FDA_InitStruct->FDA_OutputPin );
	
	if (FDAx == FDA0)
	{
		FDA_Trim = *(__IO uint32_t*)0x1FFFF024;
		FDA0->CAILCFG |= ( FDA_Trim & 0x0007 )<< 1;
	}
	else if (FDAx == FDA1)
	{
		FDA_Trim = *(__IO uint32_t*)0x1FFFF028;
		FDA1->CAILCFG |= ( FDA_Trim & 0x0007 ) << 1;
	}
}


/**
  * @brief  Enables or disables the specified FDA peripheral.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @param  NewState: new state of the FDA peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FDA_Cmd(FDA_TypeDef* FDAx, FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		FDAx->CR |= (uint32_t)FDA_CR_EN;
	}
	else
	{
		FDAx->CR &= (uint32_t)(~FDA_CR_EN);
	}
}

/**
  * @brief  Enables or disables the specified FDA Floating Input.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @param  NewState: new state of the FDA peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FDA_FloatingInputCmd(FDA_TypeDef* FDAx, FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		FDAx->CAILCFG |= (uint32_t)FDA_CFG_FIN;
	}
	else
	{
		FDAx->CAILCFG &= (uint32_t)(~FDA_CFG_FIN);
	}
}


/**
  * @brief  Enables or disables Calibrate the specified FDA peripheral.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @param  NewState: new state of the FDA peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FDA_CalibrateCmd(FDA_TypeDef* FDAx, FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		FDAx->CAILCFG |= (uint32_t)FDA_CFG_TRIMEN;
	}
	else
	{
		FDAx->CAILCFG &= (uint32_t)(~FDA_CFG_TRIMEN);
	}
}

/**
  * @brief  Set Calibrate the specified FDA peripheral.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @param  FDA_TRIM: (0..7)
  * @retval None
  */
void FDA_CalibrateSet(FDA_TypeDef* FDAx, uint32_t FDA_TRIM)
{
	FDAx->CAILCFG &= (uint32_t)(~FDA_CFG_TRIM_MASK);
	FDAx->CAILCFG |= (uint32_t)(FDA_TRIM << 1);
}


/**
  * @brief  Get Calibrate the specified FDA peripheral Status.
  * @param  FDAx: where x can be 0, 1 to select the FDA peripheral.
  * @retval None
  */
FlagStatus FDA_GetCalibrateCompleteStatus(FDA_TypeDef* FDAx)
{
	uint32_t FLAG;	
  FlagStatus bitstatus = RESET;
	
	FLAG = FDAx->CAILCFG;	
	
  if ((FLAG & FDA_CFG_CAL_MASK) != (uint8_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  return  bitstatus;
}





















