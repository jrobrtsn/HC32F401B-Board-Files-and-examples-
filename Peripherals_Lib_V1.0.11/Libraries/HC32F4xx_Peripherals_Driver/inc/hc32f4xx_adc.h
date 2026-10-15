#ifndef __HC32F4xx_ADC_H
#define __HC32F4xx_ADC_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define ADC_CR_EN 	 				0x00000001
#define ADC_CR_TSEN					0x00000004
#define ADC_CR_DMAEN				0x00000400


#define ADC_START_EN 				0x00000001


typedef struct
{	
	uint32_t	ADC_ClkDiv;
	uint32_t	ADC_Range;		
	uint32_t	ADC_OverSamplingMode;	
	uint32_t	ADC_OverSamplingTrigger;	
	uint32_t	ADC_OverSamplingRate;	
	uint32_t	ADC_OverSamplingOffset;          
}ADC_InitTypeDef;


/** @defgroup ADC_ClkDiv
  * @{
  */
#define ADC_Clk_Div2									((uint32_t)0x0001)
#define ADC_Clk_Div4     							((uint32_t)0x0002)
#define ADC_Clk_Div8									((uint32_t)0x0003)
#define ADC_Clk_Div12									((uint32_t)0x0004)
#define ADC_Clk_Div16									((uint32_t)0x0005)
#define ADC_Clk_Div24									((uint32_t)0x0006)
#define ADC_Clk_Div32									((uint32_t)0x0007)
#define ADC_Clk_Div48									((uint32_t)0x0008)
#define ADC_Clk_Div64									((uint32_t)0x0009)
#define ADC_Clk_Div128								((uint32_t)0x000A)


/** @defgroup ADC_Range @ InRefEN
  * @{
  */
#define ADC_Range_1V6									((uint32_t)0x02)
#define ADC_Range_2V4									((uint32_t)0x12)
#define ADC_Range_VDDA								((uint32_t)0x20)


/** @defgroup ADC_TriggerSamplingMode
  * @{
  */
#define ADC_OverSamplingTrigger_Once	((uint32_t)0x0)
#define ADC_OverSamplingTrigger_Each	((uint32_t)0x1)


/** @defgroup ADC_OverSamplingMode
  * @{
  */
#define ADC_OverSampling_Disable			((uint32_t)0x0)
#define ADC_OverSampling_Enable				((uint32_t)0x1)	

	
/** @defgroup ADC_DAMOverflow
  * @{
  */	
#define ADC_DMAOverflow_Disable				((uint32_t)0x1)	
#define ADC_DMAOverflow_Enable				((uint32_t)0x0)


/** @defgroup ADC_DMALoop
  * @{
  */	
#define ADC_DMALoop_Disable						((uint32_t)0x0)
#define ADC_DMALoop_Enable						((uint32_t)0x1)
	
	
/** @defgroup ADC_OverSamplingRate
  * @{
  */
#define ADC_2X_OverSamplingRate				((uint32_t)0x000)
#define ADC_4X_OverSamplingRate				((uint32_t)0x001)
#define ADC_8X_OverSamplingRate				((uint32_t)0x002)
#define ADC_16X_OverSamplingRate			((uint32_t)0x003)
#define ADC_32X_OverSamplingRate			((uint32_t)0x004)
#define ADC_64X_OverSamplingRate			((uint32_t)0x005)
#define ADC_128X_OverSamplingRate			((uint32_t)0x006)
#define ADC_256X_OverSamplingRate			((uint32_t)0x007)


/** @defgroup ADC_OverSamplingOffset
  * @{
  */
#define ADC_OverSamplingOffset_0			((uint32_t)0x0000)
#define ADC_OverSamplingOffset_1			((uint32_t)0x0001)
#define ADC_OverSamplingOffset_2			((uint32_t)0x0002)
#define ADC_OverSamplingOffset_3			((uint32_t)0x0003)
#define ADC_OverSamplingOffset_4			((uint32_t)0x0004)
#define ADC_OverSamplingOffset_5			((uint32_t)0x0005)
#define ADC_OverSamplingOffset_6			((uint32_t)0x0006)
#define ADC_OverSamplingOffset_7			((uint32_t)0x0007)
#define ADC_OverSamplingOffset_8			((uint32_t)0x0008)


/** @defgroup ADC_SQRx
  * @{
  */
#define ADC_SQR0											((uint8_t)0x0000)
#define ADC_SQR1											((uint8_t)0x0001)
#define ADC_SQR2											((uint8_t)0x0002)
#define ADC_SQR3											((uint8_t)0x0003)


/** @defgroup ADC_Ens
  * @{
  */
#define ADC_EnsClose									((uint32_t)0x0000)
#define ADC_Ens0to0										((uint32_t)0x0001)
#define ADC_Ens0to1										((uint32_t)0x0002)
#define ADC_Ens0to2										((uint32_t)0x0003)
#define ADC_Ens0to3										((uint32_t)0x0004)


/** @defgroup ADC_SQRxTRIG
  * @{
  */
#define ADC_SQRxTRIG_Software					((uint32_t)0x00000001)
#define ADC_SQRxTRIG_TIM1TRGO1				((uint32_t)0x00000002)
#define ADC_SQRxTRIG_TIM1TRGO2				((uint32_t)0x00000004)
#define ADC_SQRxTRIG_TIM1CC1					((uint32_t)0x00000008)
#define ADC_SQRxTRIG_TIM1CC2					((uint32_t)0x00000010)
#define ADC_SQRxTRIG_TIM1CC3					((uint32_t)0x00000020)
#define ADC_SQRxTRIG_TIM1CC4					((uint32_t)0x00000040)
#define ADC_SQRxTRIG_TIM9TRGO					((uint32_t)0x00000200)
#define ADC_SQRxTRIG_TIM9CC1					((uint32_t)0x00000400)
#define ADC_SQRxTRIG_TIM9CC2					((uint32_t)0x00000800)
#define ADC_SQRxTRIG_TIM9CC3					((uint32_t)0x00001000)
#define ADC_SQRxTRIG_TIM9CC4					((uint32_t)0x00002000)
#define ADC_SQRxTRIG_TIM2TRGO 				((uint32_t)0x00004000)
#define ADC_SQRxTRIG_TIM3TRGO 				((uint32_t)0x00008000)
#define ADC_SQRxTRIG_TIM10TRGO 				((uint32_t)0x00010000)
#define ADC_SQRxTRIG_GPIOLTRGO				((uint32_t)0x00020000)
#define ADC_SQRxTRIG_GPIOHTRGO 				((uint32_t)0x00040000)
#define ADC_SQRxTRIG_VC0TRGO					((uint32_t)0x00080000)
#define ADC_SQRxTRIG_VC1TRGO					((uint32_t)0x00100000)


/** @defgroup ADC_Channel
  * @{
  */
#define  ADC_Channel0									((uint8_t)0x0)
#define  ADC_Channel1									((uint8_t)0x1)	
#define  ADC_Channel2									((uint8_t)0x2)	
#define  ADC_Channel3									((uint8_t)0x3)	

/** @defgroup ADC_InputChannel
  * @{
  */
#define  ADC_InputChannel_AN0									((uint32_t)0x000000)
#define  ADC_InputChannel_AN1									((uint32_t)0x000001)
#define  ADC_InputChannel_AN2									((uint32_t)0x000002)
#define  ADC_InputChannel_AN3									((uint32_t)0x000003)
#define  ADC_InputChannel_AN4									((uint32_t)0x000004)
#define  ADC_InputChannel_AN5									((uint32_t)0x000005)				
#define  ADC_InputChannel_AN6									((uint32_t)0x000006)
#define  ADC_InputChannel_AN7									((uint32_t)0x000007)
#define  ADC_InputChannel_AN8									((uint32_t)0x000008)
#define  ADC_InputChannel_AN9									((uint32_t)0x000009)
#define  ADC_InputChannel_AN10								((uint32_t)0x00000A)
#define  ADC_InputChannel_AN11								((uint32_t)0x00000B)
#define  ADC_InputChannel_AN12								((uint32_t)0x00000C)
#define  ADC_InputChannel_AN13								((uint32_t)0x00000D)
#define  ADC_InputChannel_AN14								((uint32_t)0x00000E)
#define  ADC_InputChannel_AN15								((uint32_t)0x00000F)
#define  ADC_InputChannel_AN16								((uint32_t)0x000010)
#define  ADC_InputChannel_AN17								((uint32_t)0x000011)
#define  ADC_InputChannel_AN18								((uint32_t)0x000012)
#define  ADC_InputChannel_AN19								((uint32_t)0x000013)
#define  ADC_InputChannel_AN20								((uint32_t)0x000014)
#define  ADC_InputChannel_AN21								((uint32_t)0x000015)
#define  ADC_InputChannel_AN22								((uint32_t)0x000016)
#define  ADC_InputChannel_AN23								((uint32_t)0x000017)
#define  ADC_InputChannel_AN24								((uint32_t)0x000018)
#define  ADC_InputChannel_AN25								((uint32_t)0x000019)
#define  ADC_InputChannel_AN26								((uint32_t)0x00001A)
#define  ADC_InputChannel_AN27								((uint32_t)0x00001B)
#define  ADC_InputChannel_AN28								((uint32_t)0x00001C)
#define  ADC_InputChannel_AN29								((uint32_t)0x00001D)
#define  ADC_InputChannel_VERF1P2							((uint32_t)0x000021)
#define  ADC_InputChannel_VDDA_OneThird				((uint32_t)0x000022)
#define  ADC_InputChannel_TemperatureSensor 	((uint32_t)0x000023)
#define  ADC_InputChannel_DACOUT 							((uint32_t)0x000024)
#define  ADC_InputChannel_VSSA								((uint32_t)0x000025)
#define  ADC_InputChannel_FDAyINP0INN0 				((uint32_t)0x000030)
#define  ADC_InputChannel_FDAyINP1INN1				((uint32_t)0x000031)
#define  ADC_InputChannel_FDAyINP2INN2				((uint32_t)0x000032)


/** @defgroup ADC_SampleTime
  * @{
  */
#define ADC_SampTime5Clk                     ((uint32_t)0x00000000)
#define ADC_SampTime7Clk                     ((uint32_t)0x00000001)
#define ADC_SampTime9Clk                     ((uint32_t)0x00000002)
#define ADC_SampTime12Clk                    ((uint32_t)0x00000003)
#define ADC_SampTime18Clk                    ((uint32_t)0x00000004)
#define ADC_SampTime24Clk                    ((uint32_t)0x00000005)
#define ADC_SampTime30Clk                    ((uint32_t)0x00000006)
#define ADC_SampTime42Clk                    ((uint32_t)0x00000007)
#define ADC_SampTime54Clk                    ((uint32_t)0x00000008)
#define ADC_SampTime70Clk                    ((uint32_t)0x00000009)
#define ADC_SampTime102Clk                   ((uint32_t)0x0000000A)
#define ADC_SampTime134Clk                   ((uint32_t)0x0000000B)
#define ADC_SampTime166Clk                   ((uint32_t)0x0000000C)
#define ADC_SampTime198Clk                   ((uint32_t)0x0000000D)
#define ADC_SampTime262Clk                   ((uint32_t)0x0000000E)
#define ADC_SampTime390Clk                   ((uint32_t)0x0000000F)

 
/** @defgroup ADC_interrupts_definition
  * @{
  */
#define ADC_IT_EOC                           ((uint16_t)0x0001)
#define ADC_IT_EOS0                          ((uint16_t)0x0002)
#define ADC_IT_EOS1                          ((uint16_t)0x0004)
#define ADC_IT_EOS2                          ((uint16_t)0x0008)
#define ADC_IT_EOS3                          ((uint16_t)0x0010)
#define ADC_IT_AWDTL                         ((uint16_t)0x0020)
#define ADC_IT_AWDTH                         ((uint16_t)0x0040)
#define ADC_IT_AWDTR                         ((uint16_t)0x0080)
#define ADC_IT_TRIGERR                       ((uint16_t)0x0100)
#define ADC_IT_EOSMP                      	 ((uint16_t)0x0200)
#define ADC_IT_OVER                          ((uint16_t)0x0400)


/** @defgroup ADC_interrupts_FLAG
  * @{
  */
#define ADC_FLAG_EOC                           ((uint16_t)0x0001)
#define ADC_FLAG_EOS0                          ((uint16_t)0x0002)
#define ADC_FLAG_EOS1                          ((uint16_t)0x0004)
#define ADC_FLAG_EOS2                          ((uint16_t)0x0008)
#define ADC_FLAG_EOS3                          ((uint16_t)0x0010)
#define ADC_FLAG_AWDTL                         ((uint16_t)0x0020)
#define ADC_FLAG_AWDTH                         ((uint16_t)0x0040)
#define ADC_FLAG_AWDTR                         ((uint16_t)0x0080)
#define ADC_FLAG_TRIGERR                       ((uint16_t)0x0100)
#define ADC_FLAG_EOSMP                      	 ((uint16_t)0x0200)
#define ADC_FLAG_OVER                          ((uint16_t)0x0400)


/** @defgroup ADC_AnalogWatchdog_Channel
  * @{
  */
#define ADC_AnalogWatchdog_AN0								((uint32_t)0x00000001)
#define ADC_AnalogWatchdog_AN1								((uint32_t)0x00000002)
#define ADC_AnalogWatchdog_AN2								((uint32_t)0x00000004)
#define ADC_AnalogWatchdog_AN3								((uint32_t)0x00000008)
#define ADC_AnalogWatchdog_AN4								((uint32_t)0x00000010)
#define ADC_AnalogWatchdog_AN5								((uint32_t)0x00000020)
#define ADC_AnalogWatchdog_AN6								((uint32_t)0x00000040)
#define ADC_AnalogWatchdog_AN7								((uint32_t)0x00000080)
#define ADC_AnalogWatchdog_AN8								((uint32_t)0x00000100)
#define ADC_AnalogWatchdog_AN9								((uint32_t)0x00000200)
#define ADC_AnalogWatchdog_AN10								((uint32_t)0x00000400)
#define ADC_AnalogWatchdog_AN11								((uint32_t)0x00000800)
#define ADC_AnalogWatchdog_AN12								((uint32_t)0x00001000)
#define ADC_AnalogWatchdog_AN13								((uint32_t)0x00002000)
#define ADC_AnalogWatchdog_AN14								((uint32_t)0x00004000)
#define ADC_AnalogWatchdog_AN15								((uint32_t)0x00008000)
#define ADC_AnalogWatchdog_AN15								((uint32_t)0x00008000)

#define ADC_AnalogWatchdog_AN16								((uint32_t)0x00000001)
#define ADC_AnalogWatchdog_AN17								((uint32_t)0x00000002)
#define ADC_AnalogWatchdog_AN18								((uint32_t)0x00000004)
#define ADC_AnalogWatchdog_AN19								((uint32_t)0x00000008)
#define ADC_AnalogWatchdog_AN20								((uint32_t)0x00000010)
#define ADC_AnalogWatchdog_AN21								((uint32_t)0x00000020)
#define ADC_AnalogWatchdog_AN22								((uint32_t)0x00000040)
#define ADC_AnalogWatchdog_AN23								((uint32_t)0x00000080)
#define ADC_AnalogWatchdog_AN24								((uint32_t)0x00000100)
#define ADC_AnalogWatchdog_AN25								((uint32_t)0x00000200)
#define ADC_AnalogWatchdog_AN26								((uint32_t)0x00000400)
#define ADC_AnalogWatchdog_AN27								((uint32_t)0x00000800)
#define ADC_AnalogWatchdog_AN28								((uint32_t)0x00001000)
#define ADC_AnalogWatchdog_AN29								((uint32_t)0x00002000)

#define ADC_AnalogWatchdog_CORE1P2						((uint32_t)0x00020000)
#define ADC_AnalogWatchdog_VDDAOneThird				((uint32_t)0x00040000)
#define ADC_AnalogWatchdog_TemperatureSensor	((uint32_t)0x00080000)
#define ADC_AnalogWatchdog_FDA0OUT						((uint32_t)0x00400000)
#define ADC_AnalogWatchdog_FDA1OUT						((uint32_t)0x00800000)


void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_SQRInit(ADC_TypeDef* ADCx, uint8_t ADC_SQRx, uint32_t ADC_SQRx_TRIG, uint32_t ADC_Ens);
void ADC_SQRSampleConfig(ADC_TypeDef* ADCx, uint32_t ADC_SQRx, uint32_t ADC_Channelx, uint32_t ADC_InputChannel, uint32_t ADC_SQR_Channel_sampletime);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_SoftwareStartConv(ADC_TypeDef* ADCx);
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx, uint32_t ADC_SQRx, uint32_t ADC_Channelx);
void ADC_DMAConfig(ADC_TypeDef* ADCx, uint32_t ADC_DMALOOP, uint32_t ADC_DMAOVER);
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);  
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_AnalogWatchdog0Cmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog_Channel, FunctionalState NewState);
void ADC_AnalogWatchdog1Cmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog_Channel, FunctionalState NewState);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);
void ADC_TempSensorCmd(ADC_TypeDef* ADCx, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif
