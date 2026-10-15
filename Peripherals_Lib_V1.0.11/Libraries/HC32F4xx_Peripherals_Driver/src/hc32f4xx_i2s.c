/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_i2s.h"
#include "hc32f4xx_rcc.h"


/**
  * @brief  De-initialize the I2Sx peripheral registers to their default reset values.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @retval None
  */
void I2S_DeInit(I2S_TypeDef* I2Sx)
{
  if(I2Sx == I2S2)
  {
    /* Enable I2S2 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2_I2S2, ENABLE);
    /* Release I2S2 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2_I2S2, DISABLE);
  }
  else if (I2Sx == I2S3)
  {
    /* Enable I2S3 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3_I2S3, ENABLE);
    /* Release I2S3 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3_I2S3, DISABLE);
  }
}


/**
  * @brief  Initializes the I2Sx peripheral according to the specified 
  *         parameters in the I2S_InitStruct.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_InitStruct: pointer to a I2S_InitTypeDef structure that
  *         contains the configuration information for the specified I2S peripheral.
  * @retval None
  */
void I2S_Init(I2S_TypeDef* I2Sx, I2S_InitTypeDef* I2S_InitStruct)
{ 
	I2Sx->RER 	= DISABLE;		//RX_Channel
	I2Sx->TER 	= DISABLE;		//TX_Channel
	I2Sx->IRER 	= DISABLE;		//RX BLOCK disable
	I2Sx->ITER 	= DISABLE;		//RX BLOCK disable
	
	I2Sx->RXFFR = ENABLE;			//Clear FIFO
	I2Sx->TXFFR = ENABLE;			//Clear FIFO
	
	I2Sx->RCR = I2S_InitStruct->I2S_RxBitDepth;
	I2Sx->TCR = I2S_InitStruct->I2S_TxBitDepth;
	I2Sx->IER = I2S_InitStruct->I2S_Mode | I2S_InitStruct->I2S_Direction; 
	I2Sx->CCR = I2S_InitStruct->I2S_ClkIdlePolarity | I2S_InitStruct->I2S_ClkGateTime | (I2S_InitStruct->I2S_ClkSampleTime << 3);
	I2Sx->PR = I2S_InitStruct->I2S_ClkPrescale;
	if(I2S_InitStruct->I2S_Direction == I2S_Direction_FullDuplex)
	{
		I2Sx->RER 	= ENABLE;		//RX_Channel
		I2Sx->TER 	= ENABLE;		//TX_Channel
	}
	else if(I2S_InitStruct->I2S_Direction == I2S_Direction_TX)
	{
		I2Sx->RER 	= DISABLE;	//RX_Channel		
		I2Sx->TER 	= ENABLE;		//TX_Channel
	}
	else if(I2S_InitStruct->I2S_Direction == I2S_Direction_RX)
	{
		I2Sx->RER 	= ENABLE;		//RX_Channel
		I2Sx->TER 	= DISABLE;	//TX_Channel
	}	
}


/**
  * @brief  Enables or disables the specified I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  NewState: new state of the I2Sx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2S_Cmd(I2S_TypeDef* I2Sx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    I2Sx->IER |= I2S_IER_IEN;
  }
  else
  {
    I2Sx->IER &= ~I2S_IER_IEN;
  }
}


/**
  * @brief  Enables or disables the specified I2S CLK peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  NewState: new state of the I2Sx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2S_CLKCmd(I2S_TypeDef* I2Sx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    I2Sx->CER |= I2S_IER_IEN;
  }
  else
  {
    I2Sx->CER = 0;
  }
}


/**
  * @brief  Config DMA TX or RX to the specified I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_DMA
	*						 @arg I2S_DMA_TX
	*						 @arg I2S_DMA_RX
  * @retval None
  */
void I2S_DMACmd(I2S_TypeDef* I2Sx, uint32_t I2S_DMA)
{
	I2Sx->DMACR = I2S_DMA;
}


/**
  * @brief  Enables or disables BLOCK the specified I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_BLOCK: When calling this function, TX and RX cannot be configured simultaneously
  *            @arg I2S_BLOCK_RX: 
  *            @arg I2S_BLOCK_TX:						
  * @param  NewState: new state of the DMA Request sources.
  *          This parameter can be: ENABLE or DISABLE.   
  * @retval None
  */
void I2S_BlockCmd(I2S_TypeDef* I2Sx, uint32_t I2S_Block, FunctionalState NewState)
{
	uint32_t *I2S_Ptr;
	
	I2S_Ptr = (uint32_t*)I2Sx + I2S_Block;
	
  if (NewState != DISABLE)
  {
		*I2S_Ptr = 1;
  }
  else	
	{
    *I2S_Ptr = 0;
	}
}


/**
  * @brief  Config Threshold DMA / interrupts Request to the specified I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_FIFODir
	*						 @arg I2S_FIFO_RX
	*						 @arg I2S_FIFO_TX
  * @param  I2S_FIFOThreshold
	*						 @arg I2S_FIFOThreshold_1				
	*						 @arg I2S_FIFOThreshold_2					
	*						 @arg I2S_FIFOThreshold_3					
	*						 @arg I2S_FIFOThreshold_4					
	*						 @arg I2S_FIFOThreshold_5					
	*						 @arg I2S_FIFOThreshold_6					
	*						 @arg I2S_FIFOThreshold_7					
	*						 @arg I2S_FIFOThreshold_8				
  * @retval None
  */
void I2S_Threshold_Config(I2S_TypeDef* I2Sx, uint32_t I2S_FIFODir, uint32_t I2S_FIFOThreshold)
{
	uint32_t *I2S_Ptr;
	
	I2S_Ptr = (uint32_t*)I2Sx + I2S_FIFODir;
	
	*I2S_Ptr = I2S_FIFOThreshold;
}


/**
  * @brief  Returns Left Data by the I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @retval The received Left data.
  */
uint32_t I2S_ReceiveLeftData(I2S_TypeDef* I2Sx)
{
	uint32_t dat;	
	
	dat = I2Sx->LeftFIFO.LRBR;
	return dat;
}


/**
  * @brief  Send Left Data by the I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
	* @param  Data:
  * @retval The received Left data.
  */
void I2S_SendLeftData(I2S_TypeDef* I2Sx, uint32_t data)
{
	I2Sx->LeftFIFO.LTHR = data;
}


/**
  * @brief  Returns Right Data by the I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @retval The received Left data.
  */
uint32_t I2S_ReceiveRightData(I2S_TypeDef* I2Sx)
{
	uint32_t dat;	
	
	dat = I2Sx->RightFIFO.RRBR;
	return dat;
}


/**
  * @brief  Send Right Data by the I2S peripheral.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
	* @param  Data:
  * @retval The received Left data.
  */
void I2S_SendRightData(I2S_TypeDef* I2Sx, uint32_t data)
{
	I2Sx->RightFIFO.RTHR = data;
}


/**
  * @brief  Enables or disables the specified I2S interrupts.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_IT: 
	*						 @arg I2S_IT_RXDA					
	*						 @arg I2S_IT_RXFO						
	*						 @arg I2S_IT_TXFE								
	*						 @arg I2S_IT_TXFO							
  * @param  NewState: new state of the specified I2S interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2S_ITConfig(I2S_TypeDef* I2Sx, uint32_t I2S_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    I2Sx->IMR &= (uint32_t)~I2S_IT;		
  }
  else
  {
    I2Sx->IMR |= I2S_IT;
  }
}


/**
  * @brief  Checks whether the specified I2Sx interrupt has occurred or not.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_IT: 
	*						 @arg I2S_IT_RXDA					
	*						 @arg I2S_IT_RXFO					
	*						 @arg I2S_IT_TXFE								
	*						 @arg I2S_IT_TXFO
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus I2S_GetITStatus(I2S_TypeDef* I2Sx, uint8_t I2S_IT)
{
  ITStatus bitstatus = RESET;

  if ((I2Sx->ISR & I2S_IT) != (uint16_t)RESET)
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
  * @brief  Clears the I2S interrupt flag.
  * @param  I2Sx: where x can be 2, 3 to select the I2S peripheral.
  * @param  I2S_IT: 				
	*						 @arg I2S_IT_RXFO											
	*						 @arg I2S_IT_TXFO
  * @retval None
  */
void I2S_ClearFlag(I2S_TypeDef* I2Sx, uint8_t I2S_IT)
{
	uint32_t temp;
	
	if(I2S_IT == I2S_IT_RXFO)
		temp = I2Sx->ROR;
	else if(I2S_IT == I2S_IT_TXFO)
		temp = I2Sx->TOR;
	
	temp ++;
}


/*****END OF FILE****/
