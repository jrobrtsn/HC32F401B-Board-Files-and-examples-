/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_usart.h"
#include "hc32f4xx_rcc.h"


/**
  * @brief  Deinitializes the USARTx peripheral registers to their default reset values.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @retval None
  */
void USART_DeInit(USART_TypeDef* USARTx)
{
  if (USARTx == USART1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
  }
  else if (USARTx == USART2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
  }
  else if (USARTx == USART3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
  }    
  else if (USARTx == UART4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);
  }
  else if (USARTx == UART5)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);
  } 
}


/**
  * @brief  Initializes the USARTx peripheral according to the specified
  *         parameters in the USART_InitStruct .
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that contains
  *         the configuration information for the specified USART peripheral.
  * @retval None
  */
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
	USARTx->CR0 = (USART_InitStruct->USART_OVER << 9)  | \
								(USART_InitStruct->USART_MSBF << 7)  | \
								(USART_InitStruct->USART_WordLength << 6) | \
								(USART_InitStruct->USART_StopBits) 	 | \
								(USART_InitStruct->USART_Parity);
	
	USARTx->BRRI = USART_InitStruct->USART_BRRI;
	USARTx->BRRF = USART_InitStruct->USART_BRRF;
}


/**
  * @brief  Initializes the USARTx peripheral Clock according to the specified parameters in the USART_ClockInitStruct .
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @param  USART_ClockInitStruct: pointer to a USART_ClockInitTypeDef structure that
  *         contains the configuration information for the specified  USART peripheral.  
  * @retval None
  */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
	uint32_t temp;
	
	temp = USARTx->CR0;
	
	USARTx->CR0 = 0;
	USARTx->CR0 = ((USART_ClockInitStruct->USART_CPOL << 17) | \
								 (USART_ClockInitStruct->USART_CPHA << 16) | \
								 (USART_ClockInitStruct->USART_LastBit << 15));
	
	USARTx->CR0 |= temp; 
}


/**
  * @brief  Enables or disables the specified USART peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  NewState: new state of the USARTx peripheral.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_Cmd(USART_TypeDef* USARTx, uint32_t USART_MODE, FunctionalState NewState)
{ 
  if (NewState != DISABLE)
  {
    USARTx->CR0 |= USART_MODE;
  }
  else
  {
    USARTx->CR0 &= (uint32_t)~(USART_MODE);
	}
}


/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *          This parameter can be one of the following values:
  *            @arg USART_IT_RCONTE:
  *            @arg USART_IT_ORE: 		Error interrupt(overrun error)
  *            @arg USART_IT_NE				Error interrupt(noise error)
  *            @arg USART_IT_PE				Parity Error interrupt
  *            @arg USART_IT_FE				Error interrupt(Frame error)
  *            @arg USART_IT_RXBRK		LIN Break detection interrupt
  *            @arg USART_IT_RXIDLE		Idle line detection interrupt
  *            @arg USART_IT_RC				Receive complete interrupt
  *            @arg USART_IT_TC				Transmission complete interrupt
  *            @arg USART_IT_TXEN			Transmit Data Register empty interrupt
  * @param  NewState: new state of the specified USARTx interrupts.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_ITConfig(USART_TypeDef* USARTx, uint32_t USART_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->IER |= USART_IT;
  }
  else	
	{
    USARTx->IER &= (uint32_t)~(USART_IT);
	}
}


/**
  * @brief  Checks whether the specified USART flag is set or not.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5, 6, 7 or 8 to select the USART or 
  *         UART peripheral.
  * @param  USART_FLAG: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg USART_FLAG_RCONTE
  *            @arg USART_FLAG_ORE			Error flag(overrun error)
  *            @arg USART_FLAG_NE				Error flag(noise error)
  *            @arg USART_FLAG_PE				Parity Error flag
  *            @arg USART_FLAG_FE				Error flag(Frame error)
  *            @arg USART_FLAG_RXBRK		LIN Break detection flag
  *            @arg USART_FLAG_RXIDLE		Idle Line detection flag
  *            @arg USART_FLAG_RC				Receive complete flag
  *            @arg USART_FLAG_TC				Transmission complete flag
  *            @arg USART_FLAG_TXEN			Transmit Data Register empty flag
  * @retval The new state of USART_FLAG (SET or RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint32_t USART_FLAG)
{
  ITStatus bitstatus = RESET;
	
  if ((USARTx->ISR & USART_FLAG) != (uint32_t)RESET)
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
  * @brief  Clears the USARTx's pending flags.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_FLAG: specifies the flag to clear.
  *          This parameter can be any combination of the following values:
  *            @arg USART_FLAG_RCONTE
  *            @arg USART_FLAG_ORE			Error flag(overrun error)
  *            @arg USART_FLAG_NE				Error flag(noise error)
  *            @arg USART_FLAG_PE				Parity Error flag
  *            @arg USART_FLAG_FE				Error flag(Frame error)
  *            @arg USART_FLAG_RXBRK		LIN Break detection flag
  *            @arg USART_FLAG_RXIDLE		Idle Line detection flag
  *            @arg USART_FLAG_RC				Receive complete flag
  *            @arg USART_FLAG_TC				Transmission complete flag
  *            @arg USART_FLAG_TXEN			Transmit Data Register empty flag
  * @retval None
  */
void USART_ClearFlag(USART_TypeDef* USARTx, uint32_t USART_FLAG)
{
	USARTx->ICR &= ~USART_FLAG;
}


/**
  * @brief  Set the USART RX Received Count.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_RCNT: This parameter can be: 1~8.
  * @retval None
  */
void USART_RCNT_Config(USART_TypeDef* USARTx, uint32_t USART_RCNT)
{
	USARTx->RCNTCFG = USART_RCNT;
}


/**
  * @brief  Clear the USART RX Received Count.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_RCNT: This parameter can be: 1~8.
  * @retval None
  */
void USART_ClearRCNT(USART_TypeDef* USARTx)
{
	USARTx->RCNT = 0;
}


/**
  * @brief  Returns the most recent received data by the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
	* @param  USART_RDRx:This parameter can be:
							@arg USART_RDR0
							@arg USART_RDR1
							@arg USART_RDR2
							@arg USART_RDR3
							@arg USART_RDR4
							@arg USART_RDR5
							@arg USART_RDR6
							@arg USART_RDR7
  * @retval The received data.
  */
uint16_t USART_ReceiveData(USART_TypeDef* USARTx, uint16_t USART_RDRx)
{
	uint16_t dat;
	
	switch(USART_RDRx)
	{
		case 0:
			dat = (USARTx->RDR0 & (uint16_t)0x01FF);
			break;		
		case 1:
			dat = (USARTx->RDR1 & (uint16_t)0x01FF);
			break;
		case 2:
			dat = (USARTx->RDR2 & (uint16_t)0x01FF);
			break;
		case 3:
			dat = (USARTx->RDR2 & (uint16_t)0x01FF);
			break;
		case 4:
			dat = (USARTx->RDR4 & (uint16_t)0x01FF);
			break;
		case 5:
			dat = (USARTx->RDR5 & (uint16_t)0x01FF);
			break;
		case 6:
			dat = (USARTx->RDR6 & (uint16_t)0x01FF);
			break;
		case 7:
			dat = (USARTx->RDR7 & (uint16_t)0x01FF);
			break;
		default:
			break;
	}
	
	return dat;
}


/**
  * @brief  Transmits single data through the USARTx peripheral.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  Data: the data to transmit.
  * @retval None
  */
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
	USARTx->TDR = (Data & (uint16_t)0x01FF);
}


/**
  * @brief  Enables or disables the USART RX Signal Polarity.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  NewState: This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_RX_INVConfig(USART_TypeDef* USARTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR1 |= USART_RX_INV;
  }
  else	
	{
    USARTx->CR1 &= (uint32_t)~(USART_RX_INV);
	}
}


/**
  * @brief  Enables or disables the USART TX Signal Polarity.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  NewState: This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_TX_INVConfig(USART_TypeDef* USARTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR1 |= USART_TX_INV;
  }
  else	
	{
    USARTx->CR1 &= (uint32_t)~(USART_TX_INV);
	}
}


/**
  * @brief  Enables or disables the USART's DMA interface.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART or UART peripheral.
  * @param  USART_DMAReq: specifies the DMA request.
  *          This parameter can be any combination of the following values:
  *            @arg USART_DMAReq_Tx: USART DMA transmit request
  *            @arg USART_DMAReq_Rx: USART DMA receive request
  * @param  NewState: new state of the DMA Request sources.
  *          This parameter can be: ENABLE or DISABLE.   
  * @retval None
  */
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR2 |= USART_DMAReq;
  }
  else	
	{
    USARTx->CR2 &= (uint32_t)~(USART_DMAReq);
	}
}

/**
  * @brief  Sets the USART LIN Break detection length.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @param  USART_LINBreakDetectLength: specifies the LIN break detection length.
  *          This parameter can be one of the following values:
  *            @arg USART_LINBreakDetectLength_10b: 10-bit break detection
  *            @arg USART_LINBreakDetectLength_11b: 11-bit break detection
  * @retval None
  */
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength)
{	
	uint32_t temp;
	
	temp = USARTx->CR2;
	
	USARTx->CR2 = USART_LINBreakDetectLength;
	
	USARTx->CR2 |= temp;
}


/**
  * @brief  Enables or disables the USART's LIN mode.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART.
  * @param  NewState: new state of the USART LIN mode.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR2 |= USART_LIN;
  }
  else	
	{
    USARTx->CR2 &= (uint32_t)~(USART_LIN);
	}
}


/**
  * @brief  Transmits break characters.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART peripheral.
	* @param  BreakLen: length of break characters: 13~15
  * @retval None
  */
void USART_SendBreak(USART_TypeDef* USARTx, uint8_t BreakLen)
{
	USARTx->TDR = USART_SEND_BREAK | BreakLen;
}


/**
  * @brief  Transmits Idle characters.
  * @param  USARTx: where x can be 1, 2, 3, 4, 5 to select the USART peripheral.
  * @retval None
  */
void USART_SendIdle(USART_TypeDef* USARTx)
{
	USARTx->TDR = USART_SEND_IDLE;

}


/**
  * @brief  Set Address of the USART peripheral.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @retval None
  */
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address)
{
	USARTx->ADDR = USART_Address;
}


/**
  * @brief  Set Address Mask of the USART peripheral.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @retval None
  */
void USART_SetAddressMASK(USART_TypeDef* USARTx, uint8_t USART_mask)
{
	USARTx->MASK = USART_mask;
}


/**
  * @brief  Enables or disables the USART Address.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @param  NewState: This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_AddressCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR1 |= USART_ADDREN;
  }
  else	
	{
    USARTx->CR1 &= (uint32_t)~(USART_ADDREN);
	}
}

/**
  * @brief  Enables or disables the USART Synchronous Transmits.
  * @param  USARTx: where x can be 1, 2, 3 to select the USART peripheral.
  * @param  NewState: This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void USART_SYNCCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		USARTx->CR0 |= USART_SYNC;
  }
  else	
	{
    USARTx->CR0 &= (uint32_t)~(USART_SYNC);
	}
}


/*****END OF FILE****/
