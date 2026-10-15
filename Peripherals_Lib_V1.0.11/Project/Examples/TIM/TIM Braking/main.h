
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define TIM_No_TEST  	1  	// 1-TIM1  8-TIM8


#define CCR1_Val 			10500
#define CCR2_Val 			21000
#define CCR3_Val 			31500

#if (TIM_No_TEST == 1)
#define TIM_No								TIM1
#define TIM_IRQHandler				TIM1_BRK_IRQHandler
#define TIM_IRQn							TIM1_BRK_IRQn
#define TIM_GPIOx							GPIOC
#define TIM_CH1_Pin						GPIO_Pin_5 
#define TIM_CH2_Pin						GPIO_Pin_7
#define TIM_CH3_Pin						GPIO_Pin_9
#define TIM_CH1_PinSource			GPIO_PinSource5 
#define TIM_CH2_PinSource			GPIO_PinSource7
#define TIM_CH3_PinSource			GPIO_PinSource9

#define TIM_GPIOy							GPIOC
#define TIM_CH1N_Pin					GPIO_Pin_6 
#define TIM_CH2N_Pin					GPIO_Pin_8
#define TIM_CH3N_Pin					GPIO_Pin_10
#define TIM_CH1N_PinSource		GPIO_PinSource6 
#define TIM_CH2N_PinSource		GPIO_PinSource8
#define TIM_CH3N_PinSource		GPIO_PinSource10

#define TIM_GPIOz							GPIOB
#define TIM_BK1_Pin						GPIO_Pin_11
#define TIM_BK1_PinSource			GPIO_PinSource11

#define TIM_CHx_PinAF					GPIO_AF_1

#elif (TIM_No_TEST == 8)
#define TIM_No								TIM8
#define TIM_IRQHandler				TIM8_BRK_IRQHandler
#define TIM_IRQn							TIM8_BRK_IRQn
#define TIM_GPIOx							GPIOC
#define TIM_CH1_Pin						GPIO_Pin_10 
#define TIM_CH2_Pin						GPIO_Pin_12
#define TIM_CH3_Pin						GPIO_Pin_14
#define TIM_CH1_PinSource			GPIO_PinSource10 
#define TIM_CH2_PinSource			GPIO_PinSource12
#define TIM_CH3_PinSource			GPIO_PinSource14

#define TIM_GPIOy							GPIOC
#define TIM_CH1N_Pin					GPIO_Pin_11 
#define TIM_CH2N_Pin					GPIO_Pin_13
#define TIM_CH3N_Pin					GPIO_Pin_15
#define TIM_CH1N_PinSource		GPIO_PinSource11 
#define TIM_CH2N_PinSource		GPIO_PinSource13
#define TIM_CH3N_PinSource		GPIO_PinSource15

#define TIM_GPIOz							GPIOD
#define TIM_BK1_Pin						GPIO_Pin_2
#define TIM_BK1_PinSource			GPIO_PinSource2

#define TIM_CHx_PinAF					GPIO_AF_3

#endif


#endif /* __MAIN_H */


