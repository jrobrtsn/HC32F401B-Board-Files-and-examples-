/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_canfd.h"
#include "hc32f4xx_rcc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
  * @brief  Deinitializes the CAN peripheral registers to their default reset values.
  * @retval None.
  */
void CAN_DeInit(CAN_TypeDef* CANx)
{
  if (CANx == CAN1)
  {
    /* Enable CAN1 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);
    /* Release CAN1 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);
	}
}


/**
  * @brief  Enter initialization mode to configure the CAN module parameters.
  * @param  CANx:where x can be 1 to to select the CAN peripheral.
  * @retval Constant indicates initialization succeed which will be 
  *         CAN_InitStatus_Failed or CAN_InitStatus_Success.
  */
uint8_t CAN_EnterInitMode(CAN_TypeDef* CANx)
{
  uint8_t InitStatus = CAN_InitStatus_Failed;
  uint32_t wait_ack = 0x00000000;

  /* Exit from sleep mode */
  CANx->CCCR &= (~(uint32_t)CAN_CCCR_CSR);

//  /* Request initialisation */
  CANx->CCCR |= CAN_CCCR_INIT;

  /* Wait the acknowledge */
  while (((CANx->CCCR & CAN_CCCR_INIT) != CAN_CCCR_INIT) && (wait_ack != INAK_TIMEOUT))
  {
    wait_ack++;
  }

  /* Check acknowledge */
  if ((CANx->CCCR & CAN_CCCR_INIT) != CAN_CCCR_INIT)
  {
    InitStatus = CAN_InitStatus_Failed;
  }
  else 
  {
    InitStatus = CAN_InitStatus_Success;
		
		/* Set Configuration Enable */	
		CANx->CCCR |= CAN_CCCR_CCE;
	}
	
  return InitStatus;
}


/**
  * @brief  Exit initialization mode to configure the CAN module parameters.
  * @param  CANx:where x can be 1 to to select the CAN peripheral.
  * @retval Constant indicates initialization succeed which will be 
  *         CAN_InitStatus_Failed or CAN_InitStatus_Success.
  */
uint8_t CAN_ExitInitMode(CAN_TypeDef* CANx)
{
  uint8_t InitStatus = CAN_InitStatus_Failed;
  uint32_t wait_ack = 0x00000000;

	/* Request leave initialisation */
	CANx->CCCR &= ~(uint32_t)CAN_CCCR_INIT;

	/* Wait the acknowledge */
	wait_ack = 0;

	while (((CANx->CCCR & CAN_CCCR_INIT) == CAN_CCCR_INIT) && (wait_ack != INAK_TIMEOUT))
	{
		wait_ack++;
	}

	/* ...and check acknowledged */
	if ((CANx->CCCR & CAN_CCCR_INIT) == CAN_CCCR_INIT)
	{
		InitStatus = CAN_InitStatus_Failed;
	}
	else
	{
		InitStatus = CAN_InitStatus_Success ;
	}

  return InitStatus;
}


/**
  * @brief  Initializes the CAN peripheral according to the specified
  *         parameters in the CAN_InitStruct.
  * @param  CANx:           where x can be 1 to to select the CAN 
  *                         peripheral.
  * @param  CAN_InitStruct: pointer to a CAN_InitTypeDef structure that
  *                         contains the configuration information for the 
  *                         CAN peripheral.
  * @retval Constant indicates initialization succeed which will be 
  *         CAN_InitStatus_Failed or CAN_InitStatus_Success.
  */
void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
{
	/* Set the data bit timing register */
	CANx->DBTP = (uint32_t)((uint32_t)CAN_InitStruct->CAN_DLatencyCompensation << 23) | \
							((uint32_t)CAN_InitStruct->CAN_DSJW) | \
							((uint32_t)CAN_InitStruct->CAN_DBS1 << 8) | \
							((uint32_t)CAN_InitStruct->CAN_DBS2 << 4) | \
							((uint32_t)CAN_InitStruct->CAN_DPrescaler << 16);
	
	/* Set the Nominal bit timing register */
	CANx->NBTP =((uint32_t)CAN_InitStruct->CAN_NSJW << 25) | \
							((uint32_t)CAN_InitStruct->CAN_NBS1 << 8) | \
							((uint32_t)CAN_InitStruct->CAN_NBS2) | \
							((uint32_t)CAN_InitStruct->CAN_NPrescaler << 16);
	
	/* Set Transfer Pause: Insert a pause of two CAN bit times before initiating the next transmission*/
	if (CAN_InitStruct->TransferPause == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_TXP;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_TXP;
	}		
	
	/* Set EdgeFiltering*/
	if (CAN_InitStruct->EdgeFiltering == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_EFBI;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_EFBI;
	}	
	
	/* Set 	NoErrorHandling*/
	if (CAN_InitStruct->NoErrorHandling == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_PXHD;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_PXHD;
	}	
	
	/* Set WideMessageMarker*/
	if (CAN_InitStruct->WideMessageMarker == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_WMM;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_WMM;
	}
	
	/* Set Timestamp*/
	if (CAN_InitStruct->Timestamp == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_UTTSU;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_UTTSU;
	}
	
	/* Set BitRateSwitch*/
	if (CAN_InitStruct->BitRateSwitch == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_BRSE;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_BRSE;
	}
	
	/* Set CAN_FD*/
	if (CAN_InitStruct->CAN_FD == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_FDOE;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_FDOE;
	}		
	
	/* Set Test*/
	if (CAN_InitStruct->CAN_Test == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_TEST;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_TEST;
	}
	
	/* Set the no automatic retransmission */
	if (CAN_InitStruct->CAN_DAR == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_DAR;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_DAR;
	}
	
	/* Set Bus Monitor Mode£º TX->RX */
	if (CAN_InitStruct->CAN_BusMonitor == ENABLE)
	{
		CANx->CCCR |= CAN_CCCR_MON;
	}
	else
	{
		CANx->CCCR &= ~(uint32_t)CAN_CCCR_MON;
	}
}

/**
  * @brief  Configures for the CAN peripheral Timestamp mode 
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.		
  * @param  CAN_TTPrescaler: This parameter can be 
	*					@arg	CAN_TTPrescaler_0			
	*					@arg	CAN_TTPrescaler_1						
	*					@arg	CAN_TTPrescaler_2						
	*					@arg	CAN_TTPrescaler_3						
	*					@arg	CAN_TTPrescaler_4						
	*					@arg	CAN_TTPrescaler_5							
	*					@arg	CAN_TTPrescaler_6						
	*					@arg	CAN_TTPrescaler_7						
	*					@arg	CAN_TTPrescaler_8							
	*					@arg	CAN_TTPrescaler_9							
	*					@arg	CAN_TTPrescaler_10					
	*					@arg	CAN_TTPrescaler_11					
	*					@arg	CAN_TTPrescaler_12					
	*					@arg	CAN_TTPrescaler_13						
	*					@arg	CAN_TTPrescaler_14					
	*					@arg	CAN_TTPrescaler_15	
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAN_TimestampConfig(CAN_TypeDef* CANx, uint32_t CAN_TTPrescaler, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    CANx->TSCC = (uint32_t)CAN_TTPrescaler | CAN_TSCC_TSS;
  }
  else
  {
    CANx->TSCC = (uint32_t)CAN_TTPrescaler;
  }
}


/**
  * @brief  Configures for the CAN peripheral TimeOut mode 
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.		
  * @param  CAN_TTPrescaler: This parameter can be 
	*					@arg CAN_TOEvent_ContinuousProcess	
	*					@arg CAN_TOEvent_TXFIFO						
	*					@arg CAN_TOEvent_RXFIFO0						
	*					@arg CAN_TOEvent_RXFIFO1			
  * @param  CAN_TimeOutCount takes values from 0 to 65535
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAN_TimeOutConfig(CAN_TypeDef* CANx, uint32_t CAN_TimeOutEvent, uint32_t CAN_TimeOutCount, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		CANx->TOCC = (uint32_t)(CAN_TimeOutEvent << 1)| (uint32_t)(CAN_TimeOutCount << 16) | CAN_TOCC_ETOC;
	}
	else
	{
		CANx->TOCC = (uint32_t)(CAN_TimeOutEvent << 1)| (uint32_t)(CAN_TimeOutCount << 16);
	}
}


/**
  * @brief  Configures for the CAN peripheral RAM Watchdog mode 
  * @param  CANx: where x can be 1 to select the CAN peripheral.				
	* @param  CAN_RWDCount:  takes values from 1 to 255
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CAN_RamWatchdogConfig(CAN_TypeDef* CANx, uint32_t CAN_RWDCount, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		CANx->RWD = (uint32_t)(CAN_RWDCount);
	}
	else
	{
		CANx->RWD = 0;
	}
}


/**
  * @brief  Configures the CAN reception filter according to the specified
  *         parameters in the CAN_FilterInitStruct.
  * @param  CAN_FilterInitStruct: pointer to a CAN_FilterInitTypeDef structure that
  *         contains the configuration information.
  * @retval None
  */
void CAN_GlobalFilterInit(CAN_TypeDef* CANx, CAN_GlobalFilterInitTypeDef* CAN_FilterInitStruct)
{
	CANx->GFC = (uint32_t)((CAN_FilterInitStruct->CAN_MismatchStandardFrame << 4) | \
							(CAN_FilterInitStruct->CAN_MismatchExtendedFrame << 2) | \
							(CAN_FilterInitStruct->CAN_StandardRemoteFrame << 1) | \
							(CAN_FilterInitStruct->CAN_ExtendedRemoteFrame));
}

/**
  * @brief  Configures for the CAN Send Buffer 
  * @param  CANx: where x can be 1 to select the CAN peripheral.	
  * @param  CAN_TypeID
	*					@arg CAN_StdId
	*					@arg CAN_ExtId
  * @param  CAN_FilterInitStruct: 
	*					@arg CAN_Filter_Depth:				   CAN_StdId (0..32)
																						 CAN_ExtId (0..16)
	*					@arg CAN_SIDFilter_StartAddress: Valid lower 16 bits		
  * @retval None
  */
void CAN_FilterConfig(CAN_TypeDef* CANx, uint32_t CAN_TypeID, CAN_FilterInitTypeDef* CAN_FilterInitStruct)
{
	uint32_t *CAN_Ptr;
	
	CAN_Ptr  = (uint32_t*)CANx + CAN_TypeID + 33;
	*CAN_Ptr = (uint32_t)(CAN_FilterInitStruct->CAN_Filter_Depth << 16) | (CAN_FilterInitStruct->CAN_Filter_StartAddress);
}



/**
  * @brief  Configures for the CAN Send Buffer 
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.									
  * @param  CAN_RXBufferInitStruct: 
	*					@arg CAN_TXBuffer_Depth:				(0..8)
	*					@arg CAN_TXBuffer_StartAddress: Valid lower 16 bits					eg:0xCDC0 
	*					@arg CAN_TXBuffer_Width:				CAN_BufferWidth_8BYTES	
																						CAN_BufferWidth_12BYTES			
																						CAN_BufferWidth_16BYTES					
																						CAN_BufferWidth_20BYTES					
																						CAN_BufferWidth_24BYTES				
																						CAN_BufferWidth_32BYTES				
																						CAN_BufferWidth_48BYTES				
																						CAN_BufferWidth_64BYTES					
  * @retval None
  */
void CAN_SendBufferConfig(CAN_TypeDef* CANx, CAN_TXBufferInitTypeDef* CAN_TXBufferInitStruct)
{
	CANx->TXBC &= (uint32_t)(~CAN_TXBC_TXBUFFERMASK);
	CANx->TXBC |= (uint32_t)((CAN_TXBufferInitStruct->CAN_TXBuffer_Depth << 16) | \
														(CAN_TXBufferInitStruct->CAN_TXBuffer_StartAddress));
	CANx->TXESC &= (uint32_t)(~CAN_TXESC_TXBUFFERMASK);
	CANx->TXESC |= (uint32_t)(CAN_TXBufferInitStruct->CAN_TXBuffer_Width);
}


/**
  * @brief  Configures for the CAN Receive Buffer 
  * @param  CANx: where x can be 1 to select the CAN peripheral.									
  * @param  CAN_RXBufferInitStruct: 
	*					@arg CAN_RXBuffer_StartAddress: Valid lower 16 bits
	*					@arg CAN_RXBuffer_Width:				CAN_BufferWidth_8BYTES	
																						CAN_BufferWidth_12BYTES			
																						CAN_BufferWidth_16BYTES					
																						CAN_BufferWidth_20BYTES					
																						CAN_BufferWidth_24BYTES				
																						CAN_BufferWidth_32BYTES				
																						CAN_BufferWidth_48BYTES				
																						CAN_BufferWidth_64BYTES	
  * @retval None
  */
void CAN_ReceiveBufferConfig(CAN_TypeDef* CANx, CAN_RXBufferInitTypeDef* CAN_RXBufferInitStruct)
{
	CANx->RXBC   = (uint32_t)(CAN_RXBufferInitStruct->CAN_RXBuffer_StartAddress);
	CANx->RXESC &= (uint32_t)(~CAN_RXESC_RBDSMASK);
	CANx->RXESC |= CAN_RXBufferInitStruct->CAN_RXBuffer_Width;
}




/**
  * @brief  Configures for the CAN Send FIFO or Queue
  * @param  CANx: where x can be 1 to select the CAN peripheral.									
  * @param  CAN_TXSequenceInitStruct:
	*					@arg  CAN_TXSequence_mode		CAN_TXSequence_FIFO
																				CAN_TXSequence_Queue
	*					@arg CAN_TXSequence_Depth:	(0..8)
  * @retval None
  */
void CAN_SendSequenceConfig(CAN_TypeDef* CANx, CAN_TXSequenceInitTypeDef* CAN_TXSequenceInitStruct)
{
	CANx->TXBC &= (uint32_t)(~CAN_TXBC_TXSEQUENCEMASK);
	CANx->TXBC |= (uint32_t)((CAN_TXSequenceInitStruct->CAN_TXSequence_mode << 30) | \
														(CAN_TXSequenceInitStruct->CAN_TXSequence_Depth << 24) | \
														(CAN_TXSequenceInitStruct->CAN_TXSequence_StartAddress));
}


/**
  * @brief  Configures for the CAN Receive FiFO 
  * @param  CANx: where x can be 1  to select the CAN peripheral.		
	* @param  CAN_FIFOx:
  *            @arg CAN_FIFO_0	
  *            @arg CAN_FIFO_1									
  * @param  CAN_RXFIFOInitStruct: pointer to a CAN_RXFIFOInitTypeDef structure that contains
  *         the configuration information for the CAN peripheral.
  * @retval None
  */
void CAN_ReceiveFIFOConfig(CAN_TypeDef* CANx, uint32_t CAN_FIFOx, CAN_RXFIFOInitTypeDef* CAN_RXFIFOInitStruct)
{
	if(CAN_FIFOx == CAN_FIFO_0)
	{
		CANx->RXF0C = (uint32_t)((CAN_RXFIFOInitStruct->CAN_RXFIFO_Mode << 31) | \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_ThresholdAlarmLine << 24)| \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_Depth << 16)| \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_BaseAddress));
		CANx->RXESC &= (uint32_t)(~CAN_RXESC_F0DSMASK);
		CANx->RXESC |= CAN_RXFIFOInitStruct->CAN_RXFIFO_Width;
	}
	else if (CAN_FIFOx == CAN_FIFO_1)
	{
		CANx->RXF1C = (uint32_t)((CAN_RXFIFOInitStruct->CAN_RXFIFO_Mode << 31) | \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_ThresholdAlarmLine << 24)| \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_Depth << 16)| \
									(CAN_RXFIFOInitStruct->CAN_RXFIFO_BaseAddress));
		CANx->RXESC &= (uint32_t)(~CAN_RXESC_F1DSMASK);
		CANx->RXESC |= (uint32_t)(CAN_RXFIFOInitStruct->CAN_RXFIFO_Width << 4);
	}
}



/**
  * @brief  Copy Tx message to the RAM.
  * @param  CANx: where x can be 1 to select the CAN peripheral.	
  * @retval none.
 */
void CAN_CopyTxMessageToRAM(CAN_TypeDef* CANx, uint32_t msg_addr, CanTxMessage* tx_msg)
{
	uint32_t T[18], dat_len;
	
	if(tx_msg->TypeId == CAN_StdId){
		T[0] = (uint32_t)(tx_msg->Err_State << 31) | (tx_msg->TypeId << 30) | (tx_msg->TypeFrame << 29) | (tx_msg->ID << 18); // CAN_StdId [28:18]
	}
	else{
		T[0] = (uint32_t)(tx_msg->Err_State << 31) | (tx_msg->TypeId << 30) | (tx_msg->TypeFrame << 29) | (tx_msg->ID);// CAN_ExtId [28:0]
	}
	
	T[1] = (uint32_t)(tx_msg->MesMarkerL << 24) | (tx_msg->EventFIFOStore << 23) | (tx_msg->FdMode << 21) | (tx_msg->BitRateSwitch << 20) | (tx_msg->DLC << 16) | (tx_msg->MesMarkerH << 8);
	
	dat_len = (tx_msg->DLC + 3) / 4;
	memcpy(&T[2], tx_msg->Data, dat_len*4);
	memcpy((void*)msg_addr, T, (dat_len + 2)*4);
}


/**
  * @brief  Copy the RAM to Rx message.
  * @param  CANx: where x can be 1 to select the CAN peripheral.	
  * @retval none.
 */
void CAN_CopyRAMToRxMessage(CAN_TypeDef* CANx, uint32_t msg_addr, CanRxMessage* rx_msg)
{
	uint32_t R[18], dat_len;
	
	memcpy(R, (void*)msg_addr, 8);
	
	rx_msg->Err_State = (R[0] & 0x80000000 ) >> 31;
	rx_msg->TypeId    = (R[0] & 0x40000000 ) >> 30;
	rx_msg->TypeFrame = (R[0] & 0x20000000 ) >> 29;
	if(	rx_msg->TypeId == CAN_StdId){
		rx_msg->ID = (R[0] & 0x1FFFFFFF ) >> 18;
	}
	else{
		rx_msg->ID = (R[0] & 0x1FFFFFFF);
	}
	
	rx_msg->NoMatchFrame  = (R[1] & 0x80000000 ) >> 31;
	rx_msg->FilterIndex   = (R[1] & 0x07000000 ) >> 24;
	rx_msg->FdMode			  = (R[1] & 0x00200000 ) >> 21;
	rx_msg->BitRateSwitch = (R[1] & 0x00100000 ) >> 20;
	rx_msg->DLC 					= (R[1] & 0x000F0000 ) >> 16;
	
	dat_len = (rx_msg->DLC + 3) / 4;
	if(dat_len){
		memcpy(rx_msg->Data, (void*)(msg_addr + 0x8), dat_len*4);
	}
}


/**
  * @brief  Copy SID Filter Message to the RAM.
  * @param  CANx: where x can be 1 to select the CAN peripheral.	
  * @retval none.
 */
void CAN_CopySIDFilterMessageToRAM(CAN_TypeDef* CANx, uint32_t msg_addr, CanSIDFilterMessage* Filter_msg)
{
	uint32_t S0;
	
	S0 = (uint32_t)(Filter_msg->FilterType << 30) | (Filter_msg->FilterDisposal << 27) | (Filter_msg->SFID1 << 16) | (Filter_msg->SFID2);
	memcpy((void*)msg_addr, &S0, 4);
}


/**
  * @brief  Configures for the CAN Request 
  * @param  CANx: where x can be 1 to select the CAN peripheral.	
  * @param  CAN_RequestType 
	*					@arg CAN_RequestType_Transfer
	*					@arg CAN_RequestType_Abort
  * @param  CAN_Index
	*					@arg 	CAN_Index_0
	*					@arg 	CAN_Index_1
						...
	*					@arg 	CAN_Index_7
  * @retval None
  */
void CAN_RequestConfig(CAN_TypeDef* CANx, uint32_t CAN_RequestType, uint32_t Index)
{
	uint32_t *CAN_Ptr;
	
	CAN_Ptr  = (uint32_t*)CANx + CAN_RequestType + 52;
	*CAN_Ptr |= Index;
}


/**
  * @brief  Enables or disables the specified CANx interrupts.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @param  CAN_IT: specifies the CAN interrupt sources to be enabled or disabled.
  *          This parameter can be: 
  *            @arg	CAN_IT_RF0N / CAN_IT_RF0W / CAN_IT_RF0F / CAN_IT_RF0L 							
  *            @arg	CAN_IT_RF1N / CAN_IT_RF1W / CAN_IT_RF1F / CAN_IT_RF1L 								
  *            @arg	CAN_IT_HPM	/ CAN_IT_TC 	/ CAN_IT_TCF  / CAN_IT_TFE	 								
  *            @arg	CAN_IT_TEFN / CAN_IT_TEFW / CAN_IT_TEFF / CAN_IT_TEFL 								
  *            @arg	CAN_IT_TSW	/ CAN_IT_MRAF / CAN_IT_TOO 	/ CAN_IT_DRX	 								
  *            @arg	CAN_IT_BEC 	/ CAN_IT_BEU 	/ CAN_IT_ELO	/ CAN_IT_EP 									
  *            @arg	CAN_IT_EW 	/ CAN_IT_BO 	/ CAN_IT_WDI	/ CAN_IT_PEA 									
  *            @arg	CAN_IT_PED 	/ CAN_IT_ARA 										
	* @param  CAN_INTx:
  *            @arg CAN_INT0
  *            @arg CAN_INT1
  * @param  NewState: new state of the CAN interrupts.
  * @retval None
  */
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, uint32_t CAN_INTx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		CANx->IE  |= CAN_IT;
  }
  else
  {
		CANx->IE  &= (uint32_t)~CAN_IT;
  }
	
	if(CAN_INTx != RESET)
	{
		CANx->ILS |= CAN_IT;
	}
	else
	{
		CANx->ILS &= (uint32_t)~CAN_IT;
	}
	
	CANx->ILE			= CAN_ILE_MASK;
	CANx->TXBTIE 	= CAN_TXBTIE_MASK;
	CANx->TXBCIE 	= CAN_TXBCIE_MASK;	
}



/**
  * @brief  Checks whether the specified CANx interrupt has occurred or not.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @param  CAN_IT: specifies the CAN interrupt source to check.
  *          This parameter can be one of the following values:
  *            @arg	CAN_IT_RF0N / CAN_IT_RF0W / CAN_IT_RF0F / CAN_IT_RF0L 							
  *            @arg	CAN_IT_RF1N / CAN_IT_RF1W / CAN_IT_RF1F / CAN_IT_RF1L 								
  *            @arg	CAN_IT_HPM	/ CAN_IT_TC 	/ CAN_IT_TCF  / CAN_IT_TFE	 								
  *            @arg	CAN_IT_TEFN / CAN_IT_TEFW / CAN_IT_TEFF / CAN_IT_TEFL 								
  *            @arg	CAN_IT_TSW	/ CAN_IT_MRAF / CAN_IT_TOO 	/ CAN_IT_DRX	 								
  *            @arg	CAN_IT_BEC 	/ CAN_IT_BEU 	/ CAN_IT_ELO	/ CAN_IT_EP 									
  *            @arg	CAN_IT_EW 	/ CAN_IT_BO 	/ CAN_IT_WDI	/ CAN_IT_PEA 									
  *            @arg	CAN_IT_PED 	/ CAN_IT_ARA 	
  * @retval The current state of CAN_IT (SET or RESET).
  */
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT)
{
  ITStatus itstatus = RESET;
 
  if ((CANx->IR & CAN_IT) != (uint32_t)RESET)
  {
    itstatus = SET;
  }
  else
  {
    itstatus = RESET;
  }
  return  itstatus;
}


/**
  * @brief  Clears the CANx's interrupt pending bits.
  * @param  CANx: where x can be 1 or 2 to select the CAN peripheral.
  * @param  CAN_IT: specifies the interrupt pending bit to clear.
  *          This parameter can be one of the following values:
  *            @arg	CAN_IT_RF0N / CAN_IT_RF0W / CAN_IT_RF0F / CAN_IT_RF0L 							
  *            @arg	CAN_IT_RF1N / CAN_IT_RF1W / CAN_IT_RF1F / CAN_IT_RF1L 								
  *            @arg	CAN_IT_HPM	/ CAN_IT_TC 	/ CAN_IT_TCF  / CAN_IT_TFE	 								
  *            @arg	CAN_IT_TEFN / CAN_IT_TEFW / CAN_IT_TEFF / CAN_IT_TEFL 								
  *            @arg	CAN_IT_TSW	/ CAN_IT_MRAF / CAN_IT_TOO 	/ CAN_IT_DRX	 								
  *            @arg	CAN_IT_BEC 	/ CAN_IT_BEU 	/ CAN_IT_ELO	/ CAN_IT_EP 									
  *            @arg	CAN_IT_EW 	/ CAN_IT_BO 	/ CAN_IT_WDI	/ CAN_IT_PEA 									
  *            @arg	CAN_IT_PED 	/ CAN_IT_ARA 	
  * @retval None
  */
void CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT)
{
	CANx->IR = CAN_IT;
}



/*****END OF FILE****/
