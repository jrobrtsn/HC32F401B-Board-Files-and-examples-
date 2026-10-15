/**
  ******************************************************************************

  *
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_SPI_H
#define __HC32F4xx_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


/** 
  * @brief  SPI Init structure definition  
  */

typedef struct
{
	uint32_t SPI_FrameFormat;	/* Use for SPI1 */
	uint32_t SPI_mode;
  uint32_t SPI_CPOL;
  uint32_t SPI_CPHA;
  uint32_t SPI_Protocol;
  uint32_t SPI_DataSize;
  uint32_t SPI_Direction;
  uint32_t SPI_BaudRatePrescaler;
}SPI_InitTypeDef;


/** 
  * @brief  QSPI Init structure definition  
  */

typedef struct
{
	uint32_t QSPI_TransType;	
	uint32_t QSPI_AddLength;
  uint32_t QSPI_InstLength;
  uint32_t QSPI_WaitCycles;
}QSPI_InitTypeDef;


/** @defgroup SPI_FrameFormat 
  * @{
  */
#define SPI_FrameFormat_Standard        ((uint32_t)0x00000000)
#define SPI_FrameFormat_Dual        		((uint32_t)0x00200000)
#define SPI_FrameFormat_Quad        		((uint32_t)0x00400000)


/** @defgroup SPI_mode 
  * @{
  */
#define SPI_Mode_Slave                  ((uint32_t)0x00000000)
#define SPI_Mode_Master                 ((uint32_t)0x00000002)


/** @defgroup SPI_Protocol 
  * @{
  */ 
#define SPI_Protocol_Motorola           ((uint32_t)0x00)
#define SPI_Protocol_SSP           			((uint32_t)0x10)
#define SPI_Protocol_Microwire        	((uint32_t)0x20)


/** @defgroup SPI_Clock_Polarity 
  * @{
  */
#define SPI_CPOL_Low                    ((uint32_t)0x0000)
#define SPI_CPOL_High                   ((uint32_t)0x0080)


/** @defgroup SPI_Clock_Phase 
  * @{
  */
#define SPI_CPHA_1Edge                  ((uint32_t)0x0000)
#define SPI_CPHA_2Edge                  ((uint32_t)0x0040)


/** @defgroup SPI_data_size 
  * @{
  */
#define SPI_DataSize_4b                	((uint32_t)0x03)
#define SPI_DataSize_5b                 ((uint32_t)0x04)
#define SPI_DataSize_6b                 ((uint32_t)0x05)
#define SPI_DataSize_7b                 ((uint32_t)0x06)
#define SPI_DataSize_8b                 ((uint32_t)0x07)
#define SPI_DataSize_9b                 ((uint32_t)0x08)
#define SPI_DataSize_10b                ((uint32_t)0x09)
#define SPI_DataSize_11b                ((uint32_t)0x0A)
#define SPI_DataSize_12b                ((uint32_t)0x0B)
#define SPI_DataSize_13b                ((uint32_t)0x0C)
#define SPI_DataSize_14b                ((uint32_t)0x0D)
#define SPI_DataSize_15b                ((uint32_t)0x0E)
#define SPI_DataSize_16b                ((uint32_t)0x0F)

#define SPI_DataSize_17b                ((uint32_t)0x10) 	/* Use for SPI1 */
#define SPI_DataSize_18b                ((uint32_t)0x11) 	/* Use for SPI1 */
#define SPI_DataSize_19b                ((uint32_t)0x12) 	/* Use for SPI1 */
#define SPI_DataSize_2Ob                ((uint32_t)0x13) 	/* Use for SPI1 */
#define SPI_DataSize_21b                ((uint32_t)0x14) 	/* Use for SPI1 */
#define SPI_DataSize_22b                ((uint32_t)0x15) 	/* Use for SPI1 */
#define SPI_DataSize_23b                ((uint32_t)0x16) 	/* Use for SPI1 */
#define SPI_DataSize_24b                ((uint32_t)0x17) 	/* Use for SPI1 */
#define SPI_DataSize_25b                ((uint32_t)0x18) 	/* Use for SPI1 */
#define SPI_DataSize_26b                ((uint32_t)0x19) 	/* Use for SPI1 */
#define SPI_DataSize_27b                ((uint32_t)0x1A) 	/* Use for SPI1 */
#define SPI_DataSize_28b                ((uint32_t)0x1B) 	/* Use for SPI1 */
#define SPI_DataSize_29b                ((uint32_t)0x1C) 	/* Use for SPI1 */
#define SPI_DataSize_30b                ((uint32_t)0x1D) 	/* Use for SPI1 */
#define SPI_DataSize_31b                ((uint32_t)0x1E) 	/* Use for SPI1 */
#define SPI_DataSize_32b                ((uint32_t)0x1F) 	/* Use for SPI1 */


/** @defgroup SPI_data_direction 
  * @{
  */
#define SPI_Direction_FullDuplex 				((uint32_t)0x0000)
#define SPI_Direction_TX     						((uint32_t)0x0100)
#define SPI_Direction_RX          			((uint32_t)0x0200)
#define SPI_Direction_EEPROM          	((uint32_t)0x0300)


/** @defgroup SPI interrupts.
  * @{
  */
#define  SPI_IT_TXE 										((uint32_t)0x0001)
#define  SPI_IT_TXO 										((uint32_t)0x0002)
#define  SPI_IT_RXU 										((uint32_t)0x0004)
#define  SPI_IT_RXO 										((uint32_t)0x0008)
#define  SPI_IT_RXF 										((uint32_t)0x0010)


/** @defgroup SPI Status.
  * @{
  */
#define  SPI_FLAG_BSY 									((uint32_t)0x0001)
#define  SPI_FLAG_TFNF 									((uint32_t)0x0002)
#define  SPI_FLAG_TFE 									((uint32_t)0x0004)
#define  SPI_FLAG_RFNE 									((uint32_t)0x0008)
#define  SPI_FLAG_RFF 									((uint32_t)0x0010)
#define  SPI_FLAG_TXE 									((uint32_t)0x0020)
#define  SPI_FLAG_DCOL 									((uint32_t)0x0040)


/** @defgroup SPI_DMA_transfer_requests 
  * @{
  */
#define  SPI_DMAReq_Rx 									((uint32_t)0x0001)
#define  SPI_DMAReq_Tx 									((uint32_t)0x0002)


/** @defgroup MW_Order 
  * @{
  */
#define  MW_Order_out										((uint32_t)0x0000)
#define  MW_Order_in										((uint32_t)0x0001)


/** @defgroup MW_Order 
  * @{
  */
#define  MW_Handshake_Disable 					((uint32_t)0x0000)
#define  MW_Handshake_Enable						((uint32_t)0x0001)
	
	
/** @defgroup MW_Direction 
  * @{
  */
#define  MW_Direction_RX 								((uint32_t)0x0000)
#define  MW_Direction_TX								((uint32_t)0x0001)


/** @defgroup QSPI_TransType 
  * @{
  */
#define QSPI_TransType_ISAS							((uint32_t)0x00000000)
#define QSPI_TransType_ISAC							((uint32_t)0x00000001)
#define QSPI_TransType_ICAC							((uint32_t)0x00000002)


/** @defgroup QSPI_AddLength
  * @{
  */
#define QSPI_AddLength_0b								((uint32_t)0x00000000)
#define QSPI_AddLength_4b								((uint32_t)0x00000001)
#define QSPI_AddLength_8b								((uint32_t)0x00000002)
#define QSPI_AddLength_12b							((uint32_t)0x00000003)
#define QSPI_AddLength_16b							((uint32_t)0x00000004)
#define QSPI_AddLength_20b							((uint32_t)0x00000005)
#define QSPI_AddLength_24b							((uint32_t)0x00000006)
#define QSPI_AddLength_28b							((uint32_t)0x00000007)
#define QSPI_AddLength_32b							((uint32_t)0x00000008)
#define QSPI_AddLength_36b							((uint32_t)0x00000009)
#define QSPI_AddLength_40b							((uint32_t)0x0000000A)
#define QSPI_AddLength_44b							((uint32_t)0x0000000B)
#define QSPI_AddLength_48b							((uint32_t)0x0000000C)
#define QSPI_AddLength_52b							((uint32_t)0x0000000D)
#define QSPI_AddLength_56b							((uint32_t)0x0000000E)
#define QSPI_AddLength_60b							((uint32_t)0x0000000F)


/** @defgroup QSPI_InstLength
  * @{
  */
#define QSPI_InstLength_0b							((uint32_t)0x00000000)
#define QSPI_InstLength_4b							((uint32_t)0x00000001)
#define QSPI_InstLength_8b							((uint32_t)0x00000002)
#define QSPI_InstLength_16b							((uint32_t)0x00000003)


void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
void QSPI_Init(QSPI_InitTypeDef* QSPI_InitStruct);
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);

void SPI_RXCapacityConfig(SPI_TypeDef* SPIx, uint32_t SPI_DataNumber);
void SPI_RXFIFOConfig(SPI_TypeDef* SPIx, uint32_t SPI_RXFIFO_Threshold);
void SPI_TXFIFOConfig(SPI_TypeDef* SPIx, uint32_t SPI_TXFIFO_Threshold);

uint32_t SPI_ReceiveFIFO(SPI_TypeDef* SPIx);

void SPI_SendByteFIFO(SPI_TypeDef* SPIx, uint8_t Data);
void SPI_SendHalfWordFIFO(SPI_TypeDef* SPIx, uint16_t Data);
void SPI_SendWordFIFO(SPI_TypeDef* SPIx, uint32_t Data);

void SPI_AssertSSCmd(SPI_TypeDef* SPIx, FunctionalState NewState);

void SPI_TXDMAConfig(SPI_TypeDef* SPIx, uint32_t SPI_TXFIFO_Threshold);
void SPI_RXDMAConfig(SPI_TypeDef* SPIx, uint32_t SPI_RXFIFO_Threshold);
void SPI_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState);


FlagStatus SPI_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_FLAG);
void SPI_ITConfig(SPI_TypeDef* SPIx, uint32_t SPI_IT, FunctionalState NewState);
ITStatus SPI_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_IT);
void SPI_ClearFlag(SPI_TypeDef* SPIx);


void SPI_TestConfig(SPI_TypeDef* SPIx, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */

/**
  * @}
  */

/*****END OF FILE****/
