/**
  ******************************************************************************
  * 
  *
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_I2S_H
#define __HC32F4xx_I2S_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define I2S_IER_IEN					((uint32_t)0x00000001)

#define I2S_TXFFR_MASK			((uint32_t)0x00000001)
#define I2S_RXFFR_MASK			((uint32_t)0x00000001)

/** 
  * @brief  SPI Init structure definition  
  */

typedef struct
{
	uint32_t I2S_Mode;
	uint32_t I2S_Direction;
	uint32_t I2S_ClkIdlePolarity;	
	uint32_t I2S_ClkSampleTime;
	uint32_t I2S_ClkGateTime;	
	uint32_t I2S_ClkPrescale;
	uint32_t I2S_RxBitDepth;
	uint32_t I2S_TxBitDepth;
}I2S_InitTypeDef;


/** @defgroup I2S_mode 
  * @{
  */
#define I2S_Mode_Slave                  ((uint32_t)0x00000000)
#define I2S_Mode_Master                 ((uint32_t)0x00000002)


/** @defgroup I2S_Direction 
  * @{
  */
#define I2S_Direction_FullDuplex 				((uint32_t)0x0000)
#define I2S_Direction_TX     						((uint32_t)0x0008)
#define I2S_Direction_RX          			((uint32_t)0x0018)


/** @defgroup I2S_ClkOut 
  * @{
  */
#define I2S_ClkOut_DISABLE               ((uint32_t)0x00000000)
#define I2S_ClkOut_ENABLE                ((uint32_t)0x00000001)


/** @defgroup I2S_ClkIdlePolarity 
  * @{
  */
#define I2S_ClkIdlePolarity_Low					((uint32_t)0x00000000)
#define I2S_ClkIdlePolarity_High				((uint32_t)0x00000020)


/** @defgroup I2S_ClkSampleTime 
  * @{
  */
#define I2S_ClkSampleTime_16					((uint32_t)0x0000)
#define I2S_ClkSampleTime_24					((uint32_t)0x0001)
#define I2S_ClkSampleTime_32					((uint32_t)0x0002)


/** @defgroup I2S_ClkGateTime 
  * @{
  */
#define I2S_ClkGateTime_OFF						((uint32_t)0x0000)
#define I2S_ClkGateTime_12						((uint32_t)0x0001)
#define I2S_ClkGateTime_16						((uint32_t)0x0002)
#define I2S_ClkGateTime_20						((uint32_t)0x0003)
#define I2S_ClkGateTime_24						((uint32_t)0x0004)


/** @defgroup I2S_BitDepth 
  * @{
  */
#define I2S_BitDepth_0								((uint32_t)0x0000)
#define I2S_BitDepth_12								((uint32_t)0x0001)
#define I2S_BitDepth_16								((uint32_t)0x0002)
#define I2S_BitDepth_20								((uint32_t)0x0002)
#define I2S_BitDepth_24								((uint32_t)0x0002)
#define I2S_BitDepth_32								((uint32_t)0x0002)


/** @defgroup I2S_IT 
  * @{
  */
#define I2S_IT_RXDA										((uint32_t)0x0001)
#define I2S_IT_RXFO										((uint32_t)0x0002)
#define I2S_IT_TXFE										((uint32_t)0x0010)
#define I2S_IT_TXFO										((uint32_t)0x0020)


/** @defgroup I2S_FIFOThreshold
 * @{
  */
#define I2S_FIFOThreshold_1					((uint32_t)0x0000)
#define I2S_FIFOThreshold_2					((uint32_t)0x0001)
#define I2S_FIFOThreshold_3					((uint32_t)0x0002)
#define I2S_FIFOThreshold_4					((uint32_t)0x0003)
#define I2S_FIFOThreshold_5					((uint32_t)0x0004)
#define I2S_FIFOThreshold_6					((uint32_t)0x0005)
#define I2S_FIFOThreshold_7					((uint32_t)0x0006)
#define I2S_FIFOThreshold_8					((uint32_t)0x0007)


/** @defgroup I2S_BLOCK
 * @{
  */
#define I2S_BLOCK_RX								((uint32_t)0x0001)
#define I2S_BLOCK_TX								((uint32_t)0x0002)
//#define I2S_CH_RX									((uint32_t)0x000A)
//#define I2S_CH_TX									((uint32_t)0x000B)


/** @defgroup I2S_FIFODir
 * @{
  */
#define I2S_FIFO_RX									((uint32_t)0x0012)
#define I2S_FIFO_TX									((uint32_t)0x0013)


/** @defgroup I2S_DMA
 * @{
  */
#define I2S_DMA_RX									((uint32_t)0x0001)
#define I2S_DMA_TX									((uint32_t)0x0100)


void I2S_DeInit(I2S_TypeDef* I2Sx);
void I2S_Init(I2S_TypeDef* I2Sx, I2S_InitTypeDef* I2S_InitStruct);
void I2S_Cmd(I2S_TypeDef* I2Sx, FunctionalState NewState);
void I2S_DMACmd(I2S_TypeDef* I2Sx, uint32_t I2S_DMA);
void I2S_CLKCmd(I2S_TypeDef* I2Sx, FunctionalState NewState);


void I2S_BlockCmd(I2S_TypeDef* I2Sx, uint32_t I2S_Block, FunctionalState NewState);
void I2S_Threshold_Config(I2S_TypeDef* I2Sx, uint32_t I2S_FIFODir, uint32_t I2S_FIFOThreshold);

uint32_t I2S_ReceiveLeftData(I2S_TypeDef* I2Sx);
uint32_t I2S_ReceiveRightData(I2S_TypeDef* I2Sx);
void I2S_SendLeftData(I2S_TypeDef* I2Sx, uint32_t data);
void I2S_SendRightData(I2S_TypeDef* I2Sx, uint32_t data);

void I2S_ITConfig(I2S_TypeDef* I2Sx, uint32_t I2S_IT, FunctionalState NewState);
ITStatus I2S_GetITStatus(I2S_TypeDef* I2Sx, uint8_t I2S_IT);
void I2S_ClearFlag(I2S_TypeDef* I2Sx, uint8_t I2S_IT);


#ifdef __cplusplus
}
#endif

#endif 


/*****END OF FILE****/
