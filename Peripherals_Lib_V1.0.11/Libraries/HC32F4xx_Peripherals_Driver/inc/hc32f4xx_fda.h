#ifndef __HC32F4xx_FDA_H
#define __HC32F4xx_FDA_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define FDA_CR_EN 														0x00000001


#define FDA_CFG_FIN 													0x00000010
#define FDA_CFG_CAL_MASK 											0x00000020
#define FDA_CFG_TRIM_MASK 										0x0000000E
#define FDA_CFG_TRIMEN 												0x00000001

typedef struct
{
  uint32_t FDA_InputSignal; 
  uint32_t FDA_OutputPin; 
  uint32_t FDA_CKD; 
}FDA_InitTypeDef;


/** @defgroup FDA_InputSignal
  * @{
  */
#define FDA_InputSignal_ADC									(uint32_t)0x1000
#define FDA_InputSignal_INP0_INN0						(uint32_t)0x0000	
#define FDA_InputSignal_INP1_INN1						(uint32_t)0x2000	
#define FDA_InputSignal_INP2_INN2						(uint32_t)0x4000	


/** @defgroup FDA_OutputPin
  * @{
  */
#define FDA_OutputPin_DISABLE								(uint32_t)0x0000
#define FDA_OutputPin_ENABLE								(uint32_t)0x8000


/** @defgroup FDA_CKD
  * @{
  */
#define FDA_CKD_200K_10K										(uint32_t)0x0000
#define FDA_CKD_190K_20K										(uint32_t)0x0040
#define FDA_CKD_180K_30K										(uint32_t)0x0080
#define FDA_CKD_170K_40K										(uint32_t)0x00C0



void FDA_DeInit(FDA_TypeDef* FDAx);
void FDA_Init(FDA_TypeDef* FDAx, FDA_InitTypeDef* FDA_InitStruct);
void FDA_Cmd(FDA_TypeDef* FDAx, FunctionalState NewState);

void FDA_FloatingInputCmd(FDA_TypeDef* FDAx, FunctionalState NewState);
void FDA_CalibrateCmd(FDA_TypeDef* FDAx, FunctionalState NewState);
void FDA_CalibrateSet(FDA_TypeDef* FDAx, uint32_t FDA_TRIM);
FlagStatus FDA_GetCalibrateCompleteStatus(FDA_TypeDef* FDAx);

#ifdef __cplusplus
}
#endif

#endif 



/******END OF FILE****/











																			
																			
																			
																			
																			
																			
																			








