#include "hc32f4xx.h"


/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field. 
                                   This value must be a multiple of 0x200. */
																	 
																	 
/******************************************************************************/

/************************* PLL Parameters *************************************/
#define PLL_M      8
#define PLL_N      336
#define PLL_P      2

#define PLL_Q      2
#define PLL_R      2


uint32_t SystemCoreClock = 168000000;


/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the 
  *         SystemFrequency variable.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  
}

/*****END OF FILE****/
