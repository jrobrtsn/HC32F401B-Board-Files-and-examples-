#ifndef __HC32F4XX_MOU_H
#define __HC32F4XX_MOU_H

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f4xx.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

#define Matrix_A_Base 	0x20028000
#define Matrix_B_Base 	0x2002A000
#define Matrix_C_Base 	0x2002C000


#define MOU_START_EN 		0x00000001
#define MOU_CR_PAUSE 		0x00000008


#define MOU_IT_PCC			0x00004000
#define MOU_IT_RCC			0x00002000
#define MOU_IT_FCC			0x00001000


#define MOU_FLAG_PCC		0x00004000
#define MOU_FLAG_RCC		0x00002000
#define MOU_FLAG_FCC		0x00001000

typedef struct
{	
	uint32_t	MOU_MODE;
	uint32_t	MOU_UNIT;	
	uint32_t	MOU_BIAS;
	uint32_t	MOU_TYPEA;		
	uint32_t	MOU_TYPEB;	       
}MOU_InitTypeDef;


/** @defgroup MOU_MODE
  * @{
  */
#define MOU_MODE_MATRIX					  ((uint32_t)0x00000)
#define MOU_MODE_CONV     				((uint32_t)0x00010)


/** @defgroup MOU_UNIT
  * @{
  */
#define MOU_UNIT_WORD					 		((uint32_t)0x00000)
#define MOU_UNIT_BYTE     				((uint32_t)0x00020)

/** @defgroup MOU_BIAS
  * @{
  */
#define MOU_BIAS_DISABLE					 ((uint32_t)0x00000)
#define MOU_BIAS_ENABLE     			 ((uint32_t)0x00040)


/** @defgroup MOU_TYPEA
  * @{
  */
#define MOU_TYPEA_UNSIGNED				((uint32_t)0x00000)
#define MOU_TYPEA_SIGNED     			((uint32_t)0x00004)


/** @defgroup MOU_TYPEB
  * @{
  */
#define MOU_TYPEB_UNSIGNED				((uint32_t)0x00000)
#define MOU_TYPEB_SIGNED     			((uint32_t)0x00002)



void MOU_DeInit(void);
void Clear_Matrix(void);
void MOU_Init(MOU_InitTypeDef* MOU_InitStruct);
void MOU_SoftwareStartConv(void);
void MOU_PauseCmd(FunctionalState NewState);


void MOU_MatrixConfig(uint32_t Matrix_A_OffsetAddr, uint32_t Matrix_B_OffsetAddr, uint32_t Matrix_C_OffsetAddr, \
											uint32_t Matrix_A_Row, 	uint32_t Matrix_A_Column, uint32_t Matrix_C_Column);
void MOU_ConvConfig(uint32_t Matrix_A_OffsetAddr, uint32_t Matrix_B_OffsetAddr, uint32_t Matrix_C_OffsetAddr, \
											uint32_t Conv_Kernel, uint32_t Conv_Stride, uint32_t Conv_Count);
void MOU_GetResult(uint32_t *Result, uint32_t length);

void MOU_ITConfig(uint32_t MOU_IT, FunctionalState NewState);
FlagStatus MOU_GetFlagStatus(uint32_t MOU_FLAG);
void MOU_ClearFlag(uint32_t MOU_FLAG);

#ifdef __cplusplus
}
#endif

#endif
