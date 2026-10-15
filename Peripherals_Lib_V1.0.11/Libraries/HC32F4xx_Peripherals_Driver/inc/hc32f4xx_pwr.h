/**
  ******************************************************************************
  *
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_PWR_H
#define __HC32F4xx_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

#define  PWR_CR_LPDS                    ((uint32_t)0x00000001)     			/*!< Low-Power Deepsleep                 */
#define  PWR_CR_PDDS                    ((uint32_t)0x00000002)     			/*!< Power Down Deepsleep                */


#define  PWR_CSR_WUF                    ((uint32_t)0x00000001)     			/*!< Wakeup Flag                                      */
#define  PWR_CSR_SBF                    ((uint32_t)0x00000002)     			/*!< Standby Flag                                     */
#define  PWR_CSR_PVDO                   ((uint32_t)0x00000004)    	 		/*!< PVD Output                                       */
#define  PWR_CSR_BRR                    ((uint32_t)0x00000008)     			/*!< Backup regulator ready                           */
#define  PWR_CSR_EWUP                   ((uint32_t)0x00000100)     			/*!< Enable WKUP pin                                  */


 
/** @defgroup PWR_PVD_detection_level 
  * @{
  */ 
#define PWR_PVDLevel_0                  ((uint32_t)0x00000000)     			/*!< PVD level 0 */
#define PWR_PVDLevel_1                  ((uint32_t)0x00000020)     			/*!< PVD level 1 */
#define PWR_PVDLevel_2                  ((uint32_t)0x00000040)     			/*!< PVD level 2 */
#define PWR_PVDLevel_3                  ((uint32_t)0x00000060)     			/*!< PVD level 3 */
#define PWR_PVDLevel_4                  ((uint32_t)0x00000080)     			/*!< PVD level 4 */
#define PWR_PVDLevel_5                  ((uint32_t)0x000000A0)     			/*!< PVD level 5 */
#define PWR_PVDLevel_6                  ((uint32_t)0x000000C0)     			/*!< PVD level 6 */
#define PWR_PVDLevel_7                  ((uint32_t)0x000000E0)    		 	/*!< PVD level 7 */

  
/** @defgroup PWR_Regulator_state_in_STOP_mode 
  * @{
  */
#define PWR_MainRegulator_ON                        ((uint32_t)0x00000000)
#define PWR_LowPowerRegulator_ON                    PWR_CR_LPDS

/* --- PWR_Legacy ---*/
#define PWR_Regulator_ON                            PWR_MainRegulator_ON
#define PWR_Regulator_LowPower                      PWR_LowPowerRegulator_ON


/** @defgroup PWR_STOP_mode_entry 
  * @{
  */
#define PWR_STOPEntry_WFI               ((uint8_t)0x01)
#define PWR_STOPEntry_WFE               ((uint8_t)0x02)


/** @defgroup PWR_Regulator_Voltage_Scale 
  * @{
  */
#define PWR_Regulator_Voltage_Scale1    ((uint32_t)0x0000C000)
#define PWR_Regulator_Voltage_Scale2    ((uint32_t)0x00008000)
#define PWR_Regulator_Voltage_Scale3    ((uint32_t)0x00004000)


/** @defgroup PWR_Flag 
  * @{
  */
#define PWR_FLAG_WU                     PWR_CSR_WUF
#define PWR_FLAG_SB                     PWR_CSR_SBF
#define PWR_FLAG_PVDO                   PWR_CSR_PVDO
#define PWR_FLAG_BRR                    PWR_CSR_BRR
             

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/ 

/* Function used to set the PWR configuration to the default reset state ******/ 
void PWR_DeInit(void);

/* PVD configuration functions ************************************************/ 
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_PVDCmd(FunctionalState NewState);

/* WakeUp pins configuration functions ****************************************/ 
void PWR_WakeUpPinCmd(FunctionalState NewState);

/* Main and Backup Regulators configuration functions *************************/ 
void PWR_MainRegulatorModeConfig(uint32_t PWR_Regulator_Voltage);

/* Low Power modes configuration functions ************************************/ 
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry);
void PWR_EnterSTANDBYMode(void);

/* Flags management functions *************************************************/ 
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

#ifdef __cplusplus
}
#endif

#endif


/*****END OF FILE****/
