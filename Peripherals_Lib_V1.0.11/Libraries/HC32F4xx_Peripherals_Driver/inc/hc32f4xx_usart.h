/**
  ******************************************************************************
  * 
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_USART_H
#define __HC32F4xx_USART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define USART_OVER_16 		0x0
#define USART_OVER_8		0x1
#define USART_OVER_4		0x2

#define USART_LSB			0x0
#define USART_MSB			0x1

#define USART_RDR0			0
#define USART_RDR1			1
#define USART_RDR2			2
#define USART_RDR3			3
#define USART_RDR4			4
#define USART_RDR5			5
#define USART_RDR6			6
#define USART_RDR7			7

#define USART_TX_INV		0x0020
#define USART_RX_INV		0x0010

#define USART_LIN					0x1
#define USART_ADDREN			0X1

#define USART_SEND_BREAK	0x400
#define USART_SEND_IDLE		0x200

#define USART_BRKL_10BIT 0x0
#define USART_BRKL_11BIT 0x1

  
typedef struct
{
	uint16_t USART_OVER;
	
	uint16_t USART_BRRI;
	
	uint16_t USART_BRRF;
	
	uint16_t USART_MSBF;	
	
	uint16_t USART_WordLength;     

  uint16_t USART_StopBits;            

  uint16_t USART_Parity;     

} USART_InitTypeDef;

/** 
  * @brief  USART Clock Init Structure definition
  */ 
  
typedef struct
{

  uint32_t USART_CPOL;    /*!< Specifies the steady state of the serial clock.
                               This parameter can be a value of @ref USART_Clock_Polarity */

  uint32_t USART_CPHA;    /*!< Specifies the clock transition on which the bit capture is made.
                               This parameter can be a value of @ref USART_Clock_Phase */

  uint32_t USART_LastBit; /*!< Specifies whether the clock pulse corresponding to the last transmitted
                               data bit (MSB) has to be output on the SCLK pin in synchronous mode.
                               This parameter can be a value of @ref USART_Last_Bit */
} USART_ClockInitTypeDef;

/* Exported constants --------------------------------------------------------*/
/** @defgroup USART_Word_Length 
  * @{
  */ 
  
#define USART_WordLength_8b                  ((uint16_t)0x0)
#define USART_WordLength_9b                  ((uint16_t)0x1)
                                    

/** @defgroup USART_Stop_Bits 
  * @{
  */ 
  
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_1_5                   ((uint16_t)0x0010)
#define USART_StopBits_2                   	 ((uint16_t)0x0020)


/** @defgroup USART_Parity 
  * @{
  */ 
  
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0800)
#define USART_Parity_Odd                     ((uint16_t)0x0C00) 


/** @defgroup USART_Mode 
  * @{
  */ 
  
#define USART_Mode_Rx                        ((uint16_t)0x0002)
#define USART_Mode_Tx                        ((uint16_t)0x0001)


/** @defgroup USART_Hardware_Flow_Control 
  * @{
  */ 
#define USART_ASYNC       									((uint16_t)0x0000)
#define USART_SYNC    										  ((uint16_t)0x4000)


/** @defgroup USART_Clock_Polarity 
  * @{
  */
  
#define USART_CPOL_Low                       ((uint16_t)0x0)
#define USART_CPOL_High                      ((uint16_t)0x1)


/** @defgroup USART_Clock_Phase
  * @{
  */

#define USART_CPHA_1Edge                     ((uint16_t)0x0)
#define USART_CPHA_2Edge                     ((uint16_t)0x1)


/** @defgroup USART_Last_Bit
  * @{
  */

#define USART_LastBit_Disable                ((uint16_t)0x0)
#define USART_LastBit_Enable                 ((uint16_t)0x1)


/** @defgroup USART_Interrupt_definition 
  * @{
  */
  
#define USART_IT_RCONTE                    	  ((uint32_t)0x20000)
#define USART_IT_ORE	                     	  ((uint32_t)0x00800)
#define USART_IT_NE                       		((uint32_t)0x00400)
#define USART_IT_PE                     			((uint32_t)0x00200)
#define USART_IT_FE                     			((uint32_t)0x00100)
#define USART_IT_RXBRK                   			((uint32_t)0x00010)
#define USART_IT_RXIDLE                   		((uint32_t)0x00008)
#define USART_IT_RC                   				((uint32_t)0x00004)
#define USART_IT_TC                   				((uint32_t)0x00002)
#define USART_IT_TXEN                  				((uint32_t)0x00001)


#define USART_FLAG_RCONTE                     ((uint32_t)0x20000)
#define USART_FLAG_ORE	                      ((uint32_t)0x00800)
#define USART_FLAG_NE                       	((uint32_t)0x00400)
#define USART_FLAG_PE                     		((uint32_t)0x00200)
#define USART_FLAG_FE                     		((uint32_t)0x00100)
#define USART_FLAG_RXBRK                   		((uint32_t)0x00010)
#define USART_FLAG_RXIDLE                   	((uint32_t)0x00008)
#define USART_FLAG_RC                   			((uint32_t)0x00004)
#define USART_FLAG_TC                   			((uint32_t)0x00002)
#define USART_FLAG_TXEN                  			((uint32_t)0x00001)


/** @defgroup USART_DMA_Requests 
  * @{
  */
#define USART_DMAReq_Tx                      	((uint16_t)0x0008)
#define USART_DMAReq_Rx                      	((uint16_t)0x0004)


#define USART_LINBreakDetectLength_10b      	((uint16_t)0x0000)
#define USART_LINBreakDetectLength_11b      	((uint16_t)0x0020)


void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_Cmd(USART_TypeDef* USARTx, uint32_t USART_MODE, FunctionalState NewState);
void USART_ITConfig(USART_TypeDef* USARTx, uint32_t USART_IT, FunctionalState NewState);

ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint32_t USART_IT);
void USART_ClearFlag(USART_TypeDef* USARTx, uint32_t USART_FLAG);

void USART_RX_INVConfig(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_TX_INVConfig(USART_TypeDef* USARTx, FunctionalState NewState);

uint16_t USART_ReceiveData(USART_TypeDef* USARTx, uint16_t USART_RDRx);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);

void USART_ClearRCNT(USART_TypeDef* USARTx);
void USART_RCNT_Config(USART_TypeDef* USARTx, uint32_t USART_RCNT);
void USART_ClearRCNT(USART_TypeDef* USARTx);
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);

void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendBreak(USART_TypeDef* USARTx, uint8_t BreakLen);
void USART_SendIdle(USART_TypeDef* USARTx);


void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address);
void USART_SetAddressMASK(USART_TypeDef* USARTx, uint8_t USART_mask);
void USART_AddressCmd(USART_TypeDef* USARTx, FunctionalState NewState);


void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_SYNCCmd(USART_TypeDef* USARTx, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif



/*****END OF FILE****/
