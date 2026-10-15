
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define TIM_No_TEST  	1  	// 1-TIM1  2-TIM2 ... 9-TIM9


#define CCR1_Val 			10500
#define CCR2_Val 			21000


/* PWMÊä³öÒý½Å */
#define GENERAL_OCPWM_PIN             GPIO_Pin_4              
#define GENERAL_OCPWM_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_PINSOURCE				GPIO_PinSource4
#define GENERAL_OCPWM_AF							GPIO_AF_2


#if (TIM_No_TEST == 1)
#define TIM_No							TIM1
#define TIM_IRQHandler			TIM1_CC_IRQHandler
#define TIM_IRQn						TIM1_CC_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_2 
#define TIM_CH2_Pin					GPIO_Pin_3
#define TIM_CH1_PinSource		GPIO_PinSource2 
#define TIM_CH2_PinSource		GPIO_PinSource3
#define TIM_CHx_PinAF				GPIO_AF_1

#elif (TIM_No_TEST == 2)
#define TIM_No							TIM2
#define TIM_IRQHandler			TIM2_IRQHandler
#define TIM_IRQn						TIM2_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_11
#define TIM_CH2_Pin					GPIO_Pin_12
#define TIM_CH1_PinSource		GPIO_PinSource11 
#define TIM_CH2_PinSource		GPIO_PinSource12
#define TIM_CHx_PinAF				GPIO_AF_1

#elif (TIM_No_TEST == 3)
#define TIM_No							TIM3
#define TIM_IRQHandler			TIM3_IRQHandler
#define TIM_IRQn						TIM3_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_1
#define TIM_CH2_Pin					GPIO_Pin_2
#define TIM_CH1_PinSource		GPIO_PinSource1 
#define TIM_CH2_PinSource		GPIO_PinSource2
#define TIM_CHx_PinAF				GPIO_AF_2

#elif (TIM_No_TEST == 4)
#define TIM_No							TIM4
#define TIM_IRQHandler			TIM4_IRQHandler
#define TIM_IRQn						TIM4_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_4
#define TIM_CH2_Pin					GPIO_Pin_5
#define TIM_CH1_PinSource		GPIO_PinSource4 
#define TIM_CH2_PinSource		GPIO_PinSource5
#define TIM_CHx_PinAF				GPIO_AF_2

#elif (TIM_No_TEST == 5)
#define TIM_No							TIM5
#define TIM_IRQHandler			TIM5_IRQHandler
#define TIM_IRQn						TIM5_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_7
#define TIM_CH2_Pin					GPIO_Pin_8
#define TIM_CH1_PinSource		GPIO_PinSource7 
#define TIM_CH2_PinSource		GPIO_PinSource8
#define TIM_CHx_PinAF				GPIO_AF_2

#elif (TIM_No_TEST == 6)
#define TIM_No							TIM6
#define TIM_IRQHandler			TIM6_IRQHandler
#define TIM_IRQn						TIM6_IRQn
#define TIM_GPIOx						GPIOB
#define TIM_CH1_Pin					GPIO_Pin_14
#define TIM_CH2_Pin					GPIO_Pin_15
#define TIM_CH1_PinSource		GPIO_PinSource14 
#define TIM_CH2_PinSource		GPIO_PinSource15
#define TIM_CHx_PinAF				GPIO_AF_10

#elif (TIM_No_TEST == 7)
#define TIM_No							TIM7
#define TIM_IRQHandler			TIM7_IRQHandler
#define TIM_IRQn						TIM7_IRQn
#define TIM_GPIOx						GPIOA
#define TIM_CH1_Pin					GPIO_Pin_8
#define TIM_CH2_Pin					GPIO_Pin_9
#define TIM_CH1_PinSource		GPIO_PinSource8
#define TIM_CH2_PinSource		GPIO_PinSource9
#define TIM_CHx_PinAF				GPIO_AF_10

#elif (TIM_No_TEST == 8)
#define TIM_No							TIM8
#define TIM_IRQHandler			TIM8_CC_IRQHandler
#define TIM_IRQn						TIM8_CC_IRQn
#define TIM_GPIOx						GPIOC
#define TIM_CH1_Pin					GPIO_Pin_10 
#define TIM_CH2_Pin					GPIO_Pin_12
#define TIM_CH1_PinSource		GPIO_PinSource10
#define TIM_CH2_PinSource		GPIO_PinSource12
#define TIM_CHx_PinAF				GPIO_AF_3

#elif (TIM_No_TEST == 9)
#define TIM_No							TIM9
#define TIM_IRQHandler			TIM9_IRQHandler
#define TIM_IRQn						TIM9_IRQn
#define TIM_GPIOx						GPIOB
#define TIM_CH1_Pin					GPIO_Pin_7
#define TIM_CH2_Pin					GPIO_Pin_8
#define TIM_CH1_PinSource		GPIO_PinSource7
#define TIM_CH2_PinSource		GPIO_PinSource8
#define TIM_CHx_PinAF				GPIO_AF_3

#endif


#endif /* __MAIN_H */


