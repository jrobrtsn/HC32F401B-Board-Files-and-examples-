#include "hc32f4xx_mou.h"
#include "hc32f4xx_rcc.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
  * @brief  De-initializes the MOU peripheral registers to their default reset values.
  * @retval None
  */
void MOU_DeInit(void)
{
    RCC_AHB2PeriphResetCmd(RCC_AHB2Periph_MOU, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_MOU, DISABLE);
}


/**
  * @brief  Clear the matrix data in SRAM2.
  * @retval None
  */
void Clear_Matrix(void)
{	
	memset((uint8_t*)Matrix_A_Base, 0, 8*1024);
	memset((uint8_t*)Matrix_B_Base, 0, 8*1024);
	memset((uint8_t*)Matrix_C_Base, 0, 16*1024);
}


/**
  * @brief  Initializes the MOU peripheral according to the specified parameters 
  *         in the MOU_InitStruct.  
  * @param  MOU_InitStruct: pointer to an MOU_InitTypeDef structure that contains
  *         the configuration information for the specified MOU peripheral.
  * @retval None
  */
void MOU_Init(MOU_InitTypeDef* MOU_InitStruct)
{
	MOU->CR = MOU_InitStruct->MOU_MODE  | MOU_InitStruct->MOU_BIAS | MOU_InitStruct->MOU_UNIT | \
						MOU_InitStruct->MOU_TYPEA | MOU_InitStruct->MOU_TYPEB;
}


/**
  * @brief  Enables the selected MOU software start .
  * @retval None
  */
void MOU_SoftwareStartConv(void)
{
	MOU->CR |= (uint32_t)MOU_START_EN;
}


/**
  * @brief  Enables or disables the selected MOU software Pause.
  * @param  NewState: new state of the ADCx peripheral. 
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void MOU_PauseCmd(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
		MOU->CR |= (uint32_t)MOU_CR_PAUSE;
  }
  else
  {
		MOU->CR &= (uint32_t)(~MOU_CR_PAUSE);
  }
}


/**
  * @brief  Config the selected MOU Matrix.
  * @param  
  * @retval None
  */
void MOU_MatrixConfig(uint32_t Matrix_A_OffsetAddr, uint32_t Matrix_B_OffsetAddr, uint32_t Matrix_C_OffsetAddr, \
											uint32_t Matrix_A_Row, 	uint32_t Matrix_A_Column, uint32_t Matrix_C_Column)
{
		MOU->ADDRA = Matrix_A_OffsetAddr;
		MOU->ADDRB = Matrix_B_OffsetAddr;
		MOU->ADDRC = Matrix_C_OffsetAddr;
		MOU->MR		 = Matrix_A_Row;
		MOU->KR		 = Matrix_A_Column;
		MOU->NR		 = Matrix_C_Column;
}


/**
  * @brief  Config the selected MOU Conv.
  * @param  
  * @retval None
  */
void MOU_ConvConfig(uint32_t Matrix_A_OffsetAddr, uint32_t Matrix_B_OffsetAddr, uint32_t Matrix_C_OffsetAddr, \
											uint32_t Conv_Kernel, uint32_t Conv_Stride, uint32_t Conv_Count)
{
		MOU->ADDRA  = Matrix_A_OffsetAddr;
		MOU->ADDRB  = Matrix_B_OffsetAddr;
		MOU->ADDRC  = Matrix_C_OffsetAddr;
		MOU->KR		  = Conv_Kernel;
		MOU->STRIDE	= Conv_Stride;
		MOU->COUNT	= Conv_Count;
}


/**
  * @brief  MOU_GetResult
  * @param  
  * @retval None
  */
void MOU_GetResult(uint32_t *Result, uint32_t length)
{
	memcpy(Result, (uint32_t*)Matrix_C_Base, length*4);
}


/**
  * @brief  Enables or disables the specified MOU interrupts.
  * @param  MOU_IT: specifies the MOU interrupt sources to be enabled or disabled. 
  *          This parameter can be one of the following values:
  *            @arg MOU_IT_PCC                          
  *            @arg MOU_IT_RCC                        
  *            @arg MOU_IT_FCC                                                               
  * @param  NewState:
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void MOU_ITConfig(uint32_t MOU_IT, FunctionalState NewState)  
{
	if (NewState != DISABLE)
	{
		MOU->CR |= (uint32_t)MOU_IT;
	}
	else
	{
		MOU->CR &= (uint32_t)(~MOU_IT);
	}
}


/**
  * @brief  Checks whether the specified MOU IT Status is set or not.
  * @param  MOU_FLAG: specifies the flag to check. 
  *            @arg MOU_FLAG_PCC                          
  *            @arg MOU_FLAG_RCC                        
  *            @arg MOU_FLAG_FCC                                                
  * @retval The new state of MOU_FLAG (SET or RESET).
  */
FlagStatus MOU_GetFlagStatus(uint32_t MOU_FLAG)
{
  FlagStatus bitstatus = RESET;

  if ((MOU->SR & MOU_FLAG) != (uint8_t)RESET)
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
  * @brief  Clears the MOU pending flags.
  * @param  MOU_FLAG: specifies the flag to check. 
  *            @arg MOU_FLAG_PCC                          
  *            @arg MOU_FLAG_RCC                        
  *            @arg MOU_FLAG_FCC                         
  * @retval None
  */
void MOU_ClearFlag(uint32_t MOU_FLAG)
{
  MOU->SR |= (uint32_t)MOU_FLAG;
}
