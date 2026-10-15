
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

#define TSET_FDA 0 //0-FDA0 1-FDA1

#if TSET_FDA 
#define FDAn						FDA1
#define FDA_INPUTPIN		GPIO_Pin_7 	| GPIO_Pin_8
#define FDA_OUTPUTPIN		GPIO_Pin_11 | GPIO_Pin_14

#else

#define FDAn						FDA0
#define FDA_INPUTPIN		GPIO_Pin_1 	| GPIO_Pin_2
#define FDA_OUTPUTPIN		GPIO_Pin_3  | GPIO_Pin_4
#endif

#endif /* __MAIN_H */


