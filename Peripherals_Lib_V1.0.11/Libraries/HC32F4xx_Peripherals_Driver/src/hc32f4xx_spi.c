/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_spi.h"
#include "hc32f4xx_rcc.h"


#define  SPI_CR0_SLV_OE         ((uint32_t)0x00000400)
#define  SPI_CR0_SRL            ((uint32_t)0x00000800)

#define  SPI_SER_EN            	((uint32_t)0x00000001)

#define  SPI_ENR_EN            	((uint32_t)0x00000001)

#define  SPI_IMR_MASK           ((uint32_t)0x0000003F)

/**
  * @brief  De-initialize the SPIx peripheral registers to their default reset values.
  * @param  SPIx: where x can be:1, 2, 3. 
  * @retval None
  */
void SPI_DeInit(SPI_TypeDef* SPIx)
{
  if(SPIx == SPI1)
  {
    /* Enable SPI1 reset state */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* Release SPI1 from reset state */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
  }
	else if(SPIx == SPI2)
  {
    /* Enable SPI2 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2_I2S2, ENABLE);
    /* Release SPI2 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2_I2S2, DISABLE);
  }
  else if (SPIx == SPI3)
  {
    /* Enable SPI3 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3_I2S3, ENABLE);
    /* Release SPI3 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3_I2S3, DISABLE);
  }
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified 
  *         parameters in the SPI_InitStruct.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{ 		
	uint32_t DataSize;
	
	/* SPI and SSP Protocol*/
	if(SPI_InitStruct->SPI_Protocol != SPI_Protocol_Microwire)
	{
		if(SPIx == SPI1)
		{
			SPIx->CR0 = (uint32_t)(SPI_InitStruct->SPI_FrameFormat | \
															SPI_InitStruct->SPI_Protocol | \
															SPI_InitStruct->SPI_CPOL | \
															SPI_InitStruct->SPI_CPHA | \
															SPI_InitStruct->SPI_Direction | \
															(SPI_InitStruct->SPI_DataSize << 16));
		}
		else
		{
			SPIx->CR0 = (uint32_t)(SPI_InitStruct->SPI_Protocol | \
															SPI_InitStruct->SPI_CPOL | \
															SPI_InitStruct->SPI_CPHA | \
															SPI_InitStruct->SPI_Direction | \
															SPI_InitStruct->SPI_DataSize);
		}
	}
	else /* Microwire Protocol*/
	{
		if(SPI_InitStruct->SPI_DataSize >= 0xA)
		{
			DataSize = 0xA;
		}
		SPIx->CR0 = (uint32_t)(SPI_InitStruct->SPI_Protocol | \
														SPI_InitStruct->SPI_Direction | \
														(DataSize << 12));
	}
	
	SPIx->BAUDR |= SPI_InitStruct->SPI_BaudRatePrescaler;
	SPIx->ENR = SPI_InitStruct->SPI_mode;
	SPIx->SER = SPI_SER_EN;
	SPIx->IMR = 0;
	
	I2S2->IER = 0;
	I2S3->IER = 0;
}


/**
  * @brief  Initializes the QSPI peripheral according to the specified 
						parameters in the SPI_InitStruct.
  * @param  QSPI_InitStruct: pointer to a SPI_InitTypeDef structure that 
						contains the configuration information for the specified SPI peripheral.
  * @note		QSPI_TransType
	*            @arg QSPI_TransType_ISAS: Instruction Standard Address Standard
	*            @arg QSPI_TransType_ISAC: Instruction Standard Address Custom
	*            @arg QSPI_TransType_ICAC: Instruction Custom   Address Custom
  * @note		QSPI_AddLength
	*            @arg QSPI_AddLength_8b .,. 60b
  * @note		QSPI_InstLength
	*            @arg QSPI_InstLength_0b .,. 16b
	* @note		QSPI_WaitCycles:This parameter can be  (0..31)
  * @retval None
  */
void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct)
{ 		
	SPI1->CR2 = 0;
	SPI1->CR2 = (uint32_t)(QSPI_InitStruct->QSPI_TransType | \
													(QSPI_InitStruct->QSPI_AddLength  << 2) | \
													(QSPI_InitStruct->QSPI_InstLength << 8) | \
													(QSPI_InitStruct->QSPI_WaitCycles << 11));
}


/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
	SPI_InitStruct->SPI_mode			= SPI_Mode_Master;
	SPI_InitStruct->SPI_Protocol	= SPI_Protocol_Motorola;
	SPI_InitStruct->SPI_DataSize 	= SPI_DataSize_8b;
	SPI_InitStruct->SPI_Direction = SPI_Direction_FullDuplex;
	SPI_InitStruct->SPI_BaudRatePrescaler = 16;
}


/**
  * @brief  Enables or disables the specified SPI peripheral test mode.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
	* @param  MW_Order:
  *            @arg MW_Order_out
  *            @arg MW_Order_in
	* @param  MW_Handshake:
  *            @arg MW_Handshake_Disable
  *            @arg MW_Handshake_Enable
	* @param  MW_Direction:
  *            @arg MW_Direction_RX
  *            @arg MW_Direction_TX

  * @retval None
  */
void SPI_MicrowireConfig(SPI_TypeDef* SPIx,uint32_t MW_Order, uint32_t MW_Handshake, uint32_t MW_Direction)
{	
		SPIx->MWCR = MW_Order | MW_Handshake | MW_Direction;
}


/**
  * @brief  Configures the number of Receive data for the selected SPI RX or TX Mode.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_DataVolume: This parameter can be  (0..7)
  * @retval None
  */
void SPI_RXCapacityConfig(SPI_TypeDef* SPIx, uint32_t SPI_DataNumber)
{
	SPIx->CR1 = SPI_DataNumber;
}



/**
  * @brief  Enables or disables the specified SPI peripheral test mode.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_TestConfig(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		SPIx->CR0 |=  SPI_CR0_SRL;
  }
  else
  {
		SPIx->CR0 &= ~SPI_CR0_SRL;
  }
}


/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    SPIx->ENR |= SPI_ENR_EN;
  }
  else
  {
    SPIx->ENR &= ~SPI_ENR_EN;
  }
}


/**
  * @brief  Configures the TX FIFO for the selected SPI.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_TXFIFO_Threshold: This parameter can be  (0..7)
  * @retval None
  */
void SPI_TXFIFOConfig(SPI_TypeDef* SPIx, uint32_t SPI_TXFIFO_Threshold)
{
	SPIx->TXFTLR  = SPI_TXFIFO_Threshold;
}


/**
  * @brief  Configures the RX FIFO for the selected SPI.
  * @param  SPIx: where x can be 2, 3 to select the SPI peripheral.
  * @param  SPI_TXFIFO_Threshold: This parameter can be  (0..7)
  * @retval None
  */
void SPI_RXFIFOConfig(SPI_TypeDef* SPIx, uint32_t SPI_RXFIFO_Threshold)
{
	SPIx->RXFTLR  = SPI_RXFIFO_Threshold;
}


/**
  * @brief  Enables or disables Chip Select signal to initiate data transfer.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.  
  * @param  NewState: new state of the SPIx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_AssertSSCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		SPIx->SER |= SPI_SER_EN;
  }
  else
  {
		SPIx->SER &= ~SPI_SER_EN;
  }
}


/**
  * @brief  Load byte data into the transmit FIFO.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral. 
  * @param  Data: Data to be Load into the transmit FIFO
  * @retval None
  */
void SPI_SendByteFIFO(SPI_TypeDef* SPIx, uint8_t Data)
{
  SPIx->DR = Data;
}


/**
  * @brief  Load Half-Word data into the transmit FIFO.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral. 
  * @param  Data: Data to be Load into the transmit FIFO
  * @retval None
  */
void SPI_SendHalfWordFIFO(SPI_TypeDef* SPIx, uint16_t Data)
{
  SPIx->DR = Data;
}


/**
  * @brief  Load Word data into the transmit FIFO.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral. 
  * @param  Data: Data to be Load into the transmit FIFO
  * @retval None
  */
void SPI_SendWordFIFO(SPI_TypeDef* SPIx, uint32_t Data)
{
  SPIx->DR = Data;
}


/**
  * @brief  Returns the most recent received data by the SPIx peripheral. 
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @retval The value of the received data.
  */
uint32_t SPI_ReceiveFIFO(SPI_TypeDef* SPIx)
{
  return SPIx->DR;
}


/**
  * @brief  Checks whether the specified SPIx flag is set or not.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_FLAG: specifies the SPI flag to check. 
  *            @arg SPI_FLAG_BSY: Busy flag.
	*            @arg SPI_FLAG_TFNF: Transmit FIFO not full flag.
	*            @arg SPI_FLAG_TFF:  Transmit FIFO full flag.
  *            @arg SPI_FLAG_RFNE: Receive FIFO not empty flag.
  *            @arg SPI_FLAG_RFF:  Receive FIFO full flag.
  *            @arg SPI_FLAG_TXE:  Transmit ERR flag.
  *            @arg SPI_FLAG_DCOL: Transmit Data Race flag. 
  * @retval The new state of SPI_I2S_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((SPIx->SR & SPI_FLAG) != (uint32_t)RESET)
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
  * @brief  Enables or disables the specified SPI interrupts.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_IT: 
	*						 @arg SPI_IT_TXE:	TX FIFO Empty
	*						 @arg SPI_IT_TXO: TX FIFO Overflow
	*						 @arg SPI_IT_RXU:	RX FIFO Underflow
	*						 @arg SPI_IT_RXO: RX FIFO Overflow
	*						 @arg SPI_IT_RXF: RX FIFO Full

  * @param  NewState: new state of the specified SPI interrupt.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SPI_TypeDef* SPIx, uint32_t SPI_IT, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    SPIx->IMR |= SPI_IT;
  }
  else
  {
    SPIx->IMR &= (uint32_t)~SPI_IT;
  }
}


/**
  * @brief  Checks whether the specified SPIx interrupt has occurred or not.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_IT:
	*						 @arg SPI_IT_TXE:	TX FIFO Empty
	*						 @arg SPI_IT_TXO: TX FIFO Overflow
	*						 @arg SPI_IT_RXU:	RX FIFO Underflow
	*						 @arg SPI_IT_RXO: RX FIFO Overflow
	*						 @arg SPI_IT_RXF: RX FIFO Full
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT)
{
  ITStatus bitstatus = RESET;

  if ((SPIx->ISR & SPI_IT) != (uint16_t)RESET)
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
  * @brief  Clears the SPIx interrupt flag.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @retval None
  */
void SPI_ClearFlag(SPI_TypeDef* SPIx)
{
	uint32_t temp;
	
  temp = SPIx->ICR;
	temp ++;
}


/**
  * @brief  Enables or disables the SPIx DMA interface.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_I2S_DMAReq: 
  *            @arg SPI_DMAReq_Tx: Tx FIFO DMA transfer request
  *            @arg SPI_DMAReq_Rx: Rx FIFO DMA transfer request
  * @param  NewState: new state of the selected SPI DMA transfer request.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    SPIx->DMACR |= SPI_I2S_DMAReq;
  }
  else
  {
    SPIx->DMACR &= (uint16_t)~SPI_I2S_DMAReq;
  }
}


/**
  * @brief  Configures the TX DMA for the selected SPI.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_TXFIFO_Threshold: This parameter can be  (0..7)
  * @retval None
  */
void SPI_TXDMAConfig(SPI_TypeDef* SPIx, uint32_t SPI_TXFIFO_Threshold)
{
	SPIx->DMATDLR  = SPI_TXFIFO_Threshold;
}


/**
  * @brief  Configures the RX DMA for the selected SPI.
  * @param  SPIx: where x can be 1, 2, 3 to select the SPI peripheral.
  * @param  SPI_TXFIFO_Threshold: This parameter can be  (0..7)
  * @retval None
  */
void SPI_RXDMAConfig(SPI_TypeDef* SPIx, uint32_t SPI_RXFIFO_Threshold)
{
	SPIx->DMARDLR  = SPI_RXFIFO_Threshold;
}


/*****END OF FILE****/
