/*
*********************************************************************************************************
*	Vol 1.0.11
*
*	Copyright (C), 2009-2026, Ð¾Ê¥µç×Ó  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hc32f4xx_i2c.h"
#include "hc32f4xx_rcc.h"



/**
  * @brief  Deinitialize the I2Cx peripheral registers to their default reset values.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @retval None
  */
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
  if (I2Cx == I2C1)
  {
    /* Enable I2C1 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* Release I2C1 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);    
  }
  else if (I2Cx == I2C2)
  {
    /* Enable I2C2 reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
    /* Release I2C2 from reset state */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);      
  }
}

/**
  * @brief  Initializes the I2Cx peripheral according to the specified 
  *         parameters in the I2C_InitStruct.         
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that contains 
  *         the configuration information for the specified I2C peripheral.
  * @retval None
  */
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct)
{
	I2Cx->BRREN = I2C_InitStruct->I2C_MODE;
	I2Cx->BRR		= I2C_InitStruct->I2C_BRR;
	I2Cx->CR	  = (I2C_InitStruct->I2C_DMAEN << 8) | (I2C_InitStruct->I2C_FLT);
}


/**
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
		I2Cx->CR |=  (uint32_t)I2C_ENABLE;
	}
  else
  {
    /* Disable the selected I2C peripheral */
		I2Cx->CR &= (uint32_t)(~I2C_ENABLE);
  }
}

/**
  * @brief  Enables or disables the specified I2C DMA peripheral.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2Cx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C peripheral */
		I2Cx->CR |=  (uint32_t)I2C_DMA_ENABLE<<8;
	}
  else
  {
    /* Disable the selected I2C peripheral */
		I2Cx->CR &= (uint32_t)(~(I2C_DMA_ENABLE<<8));
  }
}


/**
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @retval The status of I2C peripheral.
  */
uint8_t I2C_GetITStatus(I2C_TypeDef* I2Cx)
{	
	uint8_t temp;
	
	temp = I2Cx->STAT;
	
	return temp;
}


/**
  * @brief  Clears the I2Cx's pending flags.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @retval None
  */
void I2C_ClearFlag(I2C_TypeDef* I2Cx)
{
	I2Cx->ISR = 0;
}


/**
  * @brief  Sends a data byte through the I2Cx peripheral.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data)
{
	I2Cx->DR = Data;
}


/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @retval The value of the received data.
  */
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx)
{
	uint8_t temp;
	
	temp = I2Cx->DR;

	return temp;
}


/**
  * @brief  Generates I2Cx communication START condition.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C START condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		I2Cx->CR |=  (uint32_t)I2C_STA;    /* Enable START */
	}
  else
  {
		I2Cx->CR &= (uint32_t)(~I2C_STA);
  }
}


/**
  * @brief  Generates I2Cx communication STOP condition.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C STOP condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		I2Cx->CR |=  (uint32_t)I2C_ST0;    /* Enable STOP */
	}
  else
  {
		I2Cx->CR &= (uint32_t)(~I2C_ST0);
  }
}


/**
  * @brief  Enables or disables the specified I2C acknowledge feature.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C Acknowledgement.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		I2Cx->CR |=  (uint32_t)I2C_AA;    /* Enable ACK */
	}
  else
  {
		I2Cx->CR &= (uint32_t)(~I2C_AA);
  }
}


/**
  * @brief  Configures the specified I2C general call feature.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @retval None
  */
void I2C_GCAddressCmd(I2C_TypeDef* I2Cx)
{
	I2Cx->ADDR0 = 0x01;
}


/**
  * @brief  Configures the specified I2C own address0.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  Address: specifies the 7bit I2C own address0.
  * @retval None.
  */
void I2C_OwnAddress0Config(I2C_TypeDef* I2Cx, uint8_t Address)
{
	I2Cx->ADDR0 = Address << 1;
}


/**
  * @brief  Configures the specified I2C own address1.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  Address: specifies the 7bit I2C own address1.
  * @retval None.
  */
void I2C_OwnAddress1Config(I2C_TypeDef* I2Cx, uint8_t Address)
{
	I2Cx->ADDR1 = Address << 1;
}


/**
  * @brief  Configures the specified I2C own address2.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  Address: specifies the 7bit I2C own address2.
  * @retval None.
  */
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address)
{
	I2Cx->ADDR2 = Address << 1;
}


/**
  * @brief  Specifies that the next DMA transfer is the last one.
  * @param  I2Cx: where x can be 1, 2 to select the I2C peripheral.
  * @param  NewState: new state of the I2C DMA last transfer.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		I2Cx->CR |=  (uint32_t)I2C_DMALastTransfer;    /* Enable ACK */
	}
  else
  {
		I2Cx->CR &= (uint32_t)(~I2C_DMALastTransfer);
  }
}


/*****END OF FILE****/
