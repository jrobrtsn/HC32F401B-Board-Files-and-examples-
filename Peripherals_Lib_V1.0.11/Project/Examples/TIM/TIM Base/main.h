
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define TIM_No_TEST  2  	// 1-TIM1  2-TIM2 ... 9-TIM9


#if (TIM_No_TEST == 1)
#define TIM_No						TIM1
#define TIM_IRQHandler		TIM1_UP_IRQHandler
#define TIM_IRQn					TIM1_UP_IRQn

#elif (TIM_No_TEST == 2)
#define TIM_No						TIM2
#define TIM_IRQHandler		TIM2_IRQHandler
#define TIM_IRQn					TIM2_IRQn
#elif (TIM_No_TEST == 3)
#define TIM_No						TIM3
#define TIM_IRQHandler		TIM3_IRQHandler
#define TIM_IRQn					TIM3_IRQn
#elif (TIM_No_TEST == 4)
#define TIM_No						TIM4
#define TIM_IRQHandler		TIM4_IRQHandler
#define TIM_IRQn					TIM4_IRQn
#elif (TIM_No_TEST == 5)
#define TIM_No						TIM5
#define TIM_IRQHandler		TIM5_IRQHandler
#define TIM_IRQn					TIM5_IRQn
#elif (TIM_No_TEST == 6)
#define TIM_No						TIM6
#define TIM_IRQHandler		TIM6_IRQHandler
#define TIM_IRQn					TIM6_IRQn
#elif (TIM_No_TEST == 7)
#define TIM_No						TIM7
#define TIM_IRQHandler		TIM7_IRQHandler
#define TIM_IRQn					TIM7_IRQn
#elif (TIM_No_TEST == 8)
#define TIM_No						TIM8
#define TIM_IRQHandler		TIM8_UP_IRQHandler
#define TIM_IRQn					TIM8_UP_IRQn
#elif (TIM_No_TEST == 9)
#define TIM_No						TIM9
#define TIM_IRQHandler		TIM9_IRQHandler
#define TIM_IRQn					TIM9_IRQn

#endif

#endif /* __MAIN_H */


