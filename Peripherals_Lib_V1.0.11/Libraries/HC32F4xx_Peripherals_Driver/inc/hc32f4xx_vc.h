

#ifndef __HC32F4XX_VC_H
#define __HC32F4XX_VC_H

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32f4xx.h"
/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif


#define VC_CR0_EN 	 				((uint32_t)0x00000001)
#define VC_CR0_IE 	 				((uint32_t)0x00000008)
#define VC_CR0_Window 	 		((uint32_t)0x00000020)

#define VC_CR1_BLANKMASK  	((uint32_t)0x0003FF00)

typedef struct
{	
	uint32_t	VC_INN_CH;
	uint32_t	VC_INP_CH;	
	uint32_t	VC_OutPolarity;	
	uint32_t	VC_Hys;	
	uint32_t	VC_ResponseSpeed;	
	uint32_t	VC_FilterClkSoure;	   
	uint32_t	VC_FilterPrescaler;	         
}VC_InitTypeDef;


/** @defgroup VC_INN_CH
  * @{
  */
#define VC_INN_CH0								((uint32_t)0x0000)
#define VC_INN_CH1								((uint32_t)0x0001)
#define VC_INN_CH2								((uint32_t)0x0002)
#define VC_INN_CH3								((uint32_t)0x0003)
#define VC_INN_CH4								((uint32_t)0x0004)
#define VC_INN_DAC_OUT						((uint32_t)0x0005)
#define VC_INN_1V2								((uint32_t)0x0006)
#define VC_INN_VDDA_OneThird			((uint32_t)0x0007)
#define VC_INN_TemperatureSensor	((uint32_t)0x0008)
#define VC_INN_MID								((uint32_t)0x0009)


/** @defgroup VC_INP_CH
  * @{
  */
#define VC_INP_CH0								((uint32_t)0x0000)
#define VC_INP_CH1								((uint32_t)0x0001)
#define VC_INP_CH2								((uint32_t)0x0002)
#define VC_INP_CH3								((uint32_t)0x0003)
#define VC_INP_CH4								((uint32_t)0x0004)
#define VC_INP_DAC_OUT						((uint32_t)0x0005)
#define VC_INP_FDA0_INP						((uint32_t)0x0006)
#define VC_INP_FDA0_OUTP					((uint32_t)0x0007)
#define VC_INP_FDA1_INP						((uint32_t)0x0040)
#define VC_INP_FDA1_OUTP					((uint32_t)0x0041)


/** @defgroup VC_Hys
  * @{
  */
#define VC_Hysteresis_DISABLE								((uint32_t)0x0000)
#define VC_Hysteresis_ENABLE								((uint32_t)0x0001)


/** @defgroup VC_ResponseSpeed
  * @{
  */
#define VC_ResponseSpeed_Low								((uint32_t)0x0000)
#define VC_ResponseSpeed_High								((uint32_t)0x0001)


/** @defgroup VC_FilterClkSoure
  * @{
  */
#define VC_FilterClkSoure_LSI								((uint32_t)0x0000)
#define VC_FilterClkSoure_PCLK							((uint32_t)0x0001)


/** @defgroup VC_OutPolarity
  * @{
  */
#define  VC_OutPolarity_NonInverted					((uint32_t)0x0000)
#define  VC_OutPolarity_Inverted						((uint32_t)0x0001)


/** @defgroup VC_FilterPrescaler
  * @{
  */
#define VC_FilterPSC_OFF 										((uint32_t)0x0000)
#define VC_FilterPSC_DIV1_N2 								((uint32_t)0x0001)
#define VC_FilterPSC_DIV1_N4 								((uint32_t)0x0002)
#define VC_FilterPSC_DIV1_N6 								((uint32_t)0x0003)
#define VC_FilterPSC_DIV2_N6 								((uint32_t)0x0004)
#define VC_FilterPSC_DIV2_N8 								((uint32_t)0x0005)
#define VC_FilterPSC_DIV4_N6 								((uint32_t)0x0006)
#define VC_FilterPSC_DIV4_N8 								((uint32_t)0x0007)
#define VC_FilterPSC_DIV8_N6 								((uint32_t)0x0008)
#define VC_FilterPSC_DIV8_N8 								((uint32_t)0x0009)
#define VC_FilterPSC_DIV16_N5 							((uint32_t)0x000A)
#define VC_FilterPSC_DIV16_N6 							((uint32_t)0x000B)
#define VC_FilterPSC_DIV16_N8 							((uint32_t)0x000C)
#define VC_FilterPSC_DIV32_N5 							((uint32_t)0x000D)
#define VC_FilterPSC_DIV32_N6 							((uint32_t)0x000E)
#define VC_FilterPSC_DIV32_N8 							((uint32_t)0x000F)


/** @defgroup VC_BlankOutPolarity
  * @{
  */
#define VC_BlankOutPolarity_Low							((uint32_t)0x0000)
#define VC_BlankOutPolarity_High 						((uint32_t)0x0001)


/** @defgroup VC_BlankCLK
  * @{
  */
#define VC_BlankCLK_4												((uint32_t)0x0000)
#define VC_BlankCLK_8												((uint32_t)0x0001)
#define VC_BlankCLK_16											((uint32_t)0x0002)						
#define VC_BlankCLK_32											((uint32_t)0x0003)					
#define VC_BlankCLK_64											((uint32_t)0x0004)						
#define VC_BlankCLK_128											((uint32_t)0x0005)					
#define VC_BlankCLK_256											((uint32_t)0x0006)						
#define VC_BlankCLK_512											((uint32_t)0x0007)


/** @defgroup VC_TriggerSource
  * @{
  */
#define VC_TriggerSource_TIM1OC1REFC						((uint32_t)0x0100)
#define VC_TriggerSource_TIM1OC2REFC						((uint32_t)0x0200)
#define VC_TriggerSource_TIM1OC3REFC						((uint32_t)0x0400)
#define VC_TriggerSource_TIM1OC4REFC						((uint32_t)0x0800)


/** @defgroup VC_IT
  * @{
  */
#define VC_IT_Falling 											((uint32_t)0x0020)
#define VC_IT_Rising 												((uint32_t)0x0040)
#define VC_IT_HighLevel 										((uint32_t)0x0080)


/** @defgroup VC_FLAG
  * @{
  */
#define VC_FLAG_FLTV   											((uint8_t)0x02)                        
#define VC_FLAG_INTF  											((uint8_t)0x01)    


void VC_DeInit(VC_TypeDef* VCx);
void VC_Init(VC_TypeDef* VCx, VC_InitTypeDef* VC_InitStruct);
void VC_Cmd(VC_TypeDef* VCx, FunctionalState NewState);


void VC_ITConfig(VC_TypeDef* VCx, uint32_t VC_IT, FunctionalState NewState);
FlagStatus VC_GetFlagStatus(VC_TypeDef* VCx, uint8_t VC_FLAG);
void VC_ClearFlag(VC_TypeDef* VCx);


void VC_WindowCmd(VC_TypeDef* VCx, FunctionalState NewState);
void VC_BlankConfig(VC_TypeDef* VCx, uint32_t VC_BlankOutPolarity, uint32_t VC_BlankCLK, uint32_t VC_TriggerSource);





#ifdef __cplusplus
}
#endif

#endif
