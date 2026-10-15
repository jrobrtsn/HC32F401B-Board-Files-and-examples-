/**
  ******************************************************************************
  * 
  *
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_I2C_H
#define __HC32F4xx_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

#define I2C_Mode_Master			0x0001
#define I2C_Mode_Slave			0x0000

#define I2C_DMA_ENABLE			0x1
#define I2C_DMA_DISABLE			0x0

#define I2C_FLT_DISABLE			0x00
#define I2C_FLT_1CLK				0x01
#define I2C_FLT_8CLK				0x10
#define I2C_FLT_15CLK				0x11


#define I2C_DMALastTransfer 0x0200
#define I2C_ENABLE					0x0040
#define I2C_STA							0x0020
#define I2C_ST0							0x0010
#define I2C_AA							0x0004


typedef struct
{
  uint32_t I2C_MODE;
  uint32_t I2C_BRR;
  uint32_t I2C_DMAEN;
  uint32_t I2C_FLT;
}I2C_InitTypeDef;


/** @defgroup I2C_acknowledgement
  * @{
  */

#define I2C_Ack_Enable                  ((uint16_t)0x0400)
#define I2C_Ack_Disable                 ((uint16_t)0x0000)


/** @defgroup I2C_transfer_direction 
  * @{
  */

#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)


/** @defgroup I2C_acknowledged_address 
  * @{
  */

#define I2C_AcknowledgedAddress_7bit    ((uint16_t)0x4000)
#define I2C_AcknowledgedAddress_10bit   ((uint16_t)0xC000)


/** @defgroup I2C_registers 
  * @{
  */

#define I2C_Register_CR1                ((uint8_t)0x00)
#define I2C_Register_CR2                ((uint8_t)0x04)
#define I2C_Register_OAR1               ((uint8_t)0x08)
#define I2C_Register_OAR2               ((uint8_t)0x0C)
#define I2C_Register_DR                 ((uint8_t)0x10)
#define I2C_Register_SR1                ((uint8_t)0x14)
#define I2C_Register_SR2                ((uint8_t)0x18)
#define I2C_Register_CCR                ((uint8_t)0x1C)
#define I2C_Register_TRISE              ((uint8_t)0x20)


/** @defgroup I2C_NACK_position 
  * @{
  */

#define I2C_NACKPosition_Next           ((uint16_t)0x0800)
#define I2C_NACKPosition_Current        ((uint16_t)0xF7FF)


/** @defgroup I2C_PEC_position 
  * @{
  */

#define I2C_PECPosition_Next            ((uint16_t)0x0800)
#define I2C_PECPosition_Current         ((uint16_t)0xF7FF)


/** @defgroup I2C_interrupts_definition 
  * @{
  */

#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)


/** @defgroup I2C_interrupts_definition 
  * @{
  */

#define I2C_IT_SMBALERT                 ((uint32_t)0x01008000)
#define I2C_IT_TIMEOUT                  ((uint32_t)0x01004000)
#define I2C_IT_PECERR                   ((uint32_t)0x01001000)
#define I2C_IT_OVR                      ((uint32_t)0x01000800)
#define I2C_IT_AF                       ((uint32_t)0x01000400)
#define I2C_IT_ARLO                     ((uint32_t)0x01000200)
#define I2C_IT_BERR                     ((uint32_t)0x01000100)
#define I2C_IT_TXE                      ((uint32_t)0x06000080)
#define I2C_IT_RXNE                     ((uint32_t)0x06000040)
#define I2C_IT_STOPF                    ((uint32_t)0x02000010)
#define I2C_IT_ADD10                    ((uint32_t)0x02000008)
#define I2C_IT_BTF                      ((uint32_t)0x02000004)
#define I2C_IT_ADDR                     ((uint32_t)0x02000002)
#define I2C_IT_SB                       ((uint32_t)0x02000001)


/** @defgroup I2C_flags_definition 
  * @{
  */

/** 
  * @brief  SR2 register flags  
  */

#define I2C_FLAG_DUALF                  ((uint32_t)0x00800000)
#define I2C_FLAG_SMBHOST                ((uint32_t)0x00400000)
#define I2C_FLAG_SMBDEFAULT             ((uint32_t)0x00200000)
#define I2C_FLAG_GENCALL                ((uint32_t)0x00100000)
#define I2C_FLAG_TRA                    ((uint32_t)0x00040000)
#define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
#define I2C_FLAG_MSL                    ((uint32_t)0x00010000)

/** 
  * @brief  SR1 register flags  
  */

#define I2C_FLAG_SMBALERT               ((uint32_t)0x10008000)
#define I2C_FLAG_TIMEOUT                ((uint32_t)0x10004000)
#define I2C_FLAG_PECERR                 ((uint32_t)0x10001000)
#define I2C_FLAG_OVR                    ((uint32_t)0x10000800)
#define I2C_FLAG_AF                     ((uint32_t)0x10000400)
#define I2C_FLAG_ARLO                   ((uint32_t)0x10000200)
#define I2C_FLAG_BERR                   ((uint32_t)0x10000100)
#define I2C_FLAG_TXE                    ((uint32_t)0x10000080)
#define I2C_FLAG_RXNE                   ((uint32_t)0x10000040)
#define I2C_FLAG_STOPF                  ((uint32_t)0x10000010)
#define I2C_FLAG_ADD10                  ((uint32_t)0x10000008)
#define I2C_FLAG_BTF                    ((uint32_t)0x10000004)
#define I2C_FLAG_ADDR                   ((uint32_t)0x10000002)
#define I2C_FLAG_SB                     ((uint32_t)0x10000001)


void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);


uint8_t I2C_GetITStatus(I2C_TypeDef* I2Cx);
void I2C_ClearFlag(I2C_TypeDef* I2Cx);

uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);

void I2C_OwnAddress0Config(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_OwnAddress1Config(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address);
void I2C_GCAddressCmd(I2C_TypeDef* I2Cx);

void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

/*  Function used to set the I2C configuration to the default reset state *****/
void I2C_DeInit(I2C_TypeDef* I2Cx);

/* Initialization and Configuration functions *********************************/
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif 


/*****END OF FILE****/
