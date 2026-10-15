/**
  ******************************************************************************
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_FLASH_H
#define __HC32F4xx_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"

#ifdef HC32F401BxE
	#define FLASH_ADDR_MAX 0x807CFFF	//FLASH 512KB(500+12)
#elif defined HC32F401BxG
	#define FLASH_ADDR_MAX 0x80FCFFF	//FLASH 1024KB(1012+12)
#else
	#define FLASH_ADDR_MAX 0x803CFFF //FLASH 256KB(244+12)
#endif


/** 
  * @brief FLASH Status  
  */ 
typedef enum
{ 
  FLASH_BUSY = 1,
  FLASH_ERROR_RD,
  FLASH_ERROR_PGS,
  FLASH_ERROR_PGP,
  FLASH_ERROR_PGA,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PROGRAM,
  FLASH_ERROR_OPERATION,
  FLASH_COMPLETE
}FLASH_Status;


/** @defgroup FLASH_Option_Bytes_Read_Protection 
  * @{
  */
#define OB_RDP_Level_0   ((uint8_t)0xAA)
#define OB_RDP_Level_1   ((uint8_t)0x55)
/*#define OB_RDP_Level_2   ((uint8_t)0xCC)*/ /*!< Warning: When enabling read protection level 2 
                                                  it's no more possible to go back to level 1 or 0 */
																									
/** @defgroup FLASH_Option_Bytes_Write_Protection_1
  * @{
  */																									
#define OB_WRP1_DISABLE 	((uint8_t)0x00)
#define OB_WRP1_ENABLE	 	((uint8_t)0x01)


/** @defgroup FLASH_Option_Bytes_Write_Protection_2
  * @{
  */																									
#define OB_WRP2_DISABLE 	((uint8_t)0x00)
#define OB_WRP2_ENABLE	 	((uint8_t)0x02)


/** @defgroup FLASH_Option_Bytes_EOPB
  * @{
  */
#define OB_EOPB_64				((uint8_t)0x00)
#define OB_EOPB_128				((uint8_t)0x40)
#define OB_EOPB_160				((uint8_t)0x80)


/** @defgroup FLASH_Option_Bytes_IWatchdog 
  * @{
  */ 
#define OB_IWDG_SW                     ((uint8_t)0x20)  /*!< Software IWDG selected */
#define OB_IWDG_HW                     ((uint8_t)0x00)  /*!< Hardware IWDG selected */


/** @defgroup FLASH_Option_Bytes_nRST_STOP 
  * @{
  */ 
#define OB_STOP_NoRST                  ((uint8_t)0x40) /*!< No reset generated when entering in STOP */
#define OB_STOP_RST                    ((uint8_t)0x00) /*!< Reset generated when entering in STOP */


/** @defgroup FLASH_Option_Bytes_nRST_STDBY 
  * @{
  */ 
#define OB_STDBY_NoRST                 ((uint8_t)0x80) /*!< No reset generated when entering in STANDBY */
#define OB_STDBY_RST                   ((uint8_t)0x00) /*!< Reset generated when entering in STANDBY */


/** @defgroup FLASH_Interrupts 
  * @{
  */ 
#define FLASH_IT_EOP                   ((uint32_t)0x01000000)  /*!< End of FLASH Operation Interrupt source */
#define FLASH_IT_ERR                   ((uint32_t)0x02000000)  /*!< Error Interrupt source */


/** @defgroup FLASH_Flags 
  * @{
  */ 
#define FLASH_FLAG_EOP                 ((uint32_t)0x00000001)  /*!< FLASH End of Operation flag               */
#define FLASH_FLAG_OPERR               ((uint32_t)0x00000002)  /*!< FLASH operation Error flag                */
#define FLASH_FLAG_WRPERR              ((uint32_t)0x00000010)  /*!< FLASH Write protected error flag          */
#define FLASH_FLAG_PGAERR              ((uint32_t)0x00000020)  /*!< FLASH Programming Alignment error flag    */
#define FLASH_FLAG_PGPERR              ((uint32_t)0x00000040)  /*!< FLASH Programming Parallelism error flag  */
#define FLASH_FLAG_PGSERR              ((uint32_t)0x00000080)  /*!< FLASH Programming Sequence error flag     */
#define FLASH_FLAG_RDERR               ((uint32_t)0x00000100)  /*!< Read Protection error flag (PCROP)        */
#define FLASH_FLAG_BSY                 ((uint32_t)0x00010000)  /*!< FLASH Busy flag                           */ 


/** @defgroup FLASH_Program_Parallelism   
  * @{
  */
#define FLASH_PSIZE_BYTE           ((uint32_t)0x00000000)
#define FLASH_PSIZE_HALF_WORD      ((uint32_t)0x00000100)
#define FLASH_PSIZE_WORD           ((uint32_t)0x00000200)
#define CR_PSIZE_MASK              ((uint32_t)0xFFFFFCFF)


/** @defgroup FLASH_Keys 
  * @{
  */ 
#define RDP_KEY                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
#define FLASH_OPT_KEY1           ((uint32_t)0x08192A3B)
#define FLASH_OPT_KEY2           ((uint32_t)0x4C5D6E7F)


/** 
  * @brief   ACR register byte 0 (Bits[7:0]) base address  
  */ 
#define ACR_BYTE0_ADDRESS           ((uint32_t)0x40023C00) 
/** 
  * @brief   OPTCR register byte 0 (Bits[7:0]) base address  
  */ 
#define OPTCR_BYTE0_ADDRESS         ((uint32_t)0x40023C14)
/** 
  * @brief   OPTCR register byte 1 (Bits[15:8]) base address  
  */ 
#define OPTCR_BYTE1_ADDRESS         ((uint32_t)0x40023C15)
/** 
  * @brief   OPTCR register byte 2 (Bits[23:16]) base address  
  */ 
#define OPTCR_BYTE2_ADDRESS         ((uint32_t)0x40023C16)
/** 
  * @brief   OPTCR register byte 3 (Bits[31:24]) base address  
  */ 
#define OPTCR_BYTE3_ADDRESS         ((uint32_t)0x40023C17)


/* FLASH Interface configuration functions ************************************/
void FLASH_PrefetchBufferCmd(FunctionalState NewState);
void FLASH_InstructionCacheCmd(FunctionalState NewState);
void FLASH_InstructionCacheReset(void);
void FLASH_DataCacheCmd(FunctionalState NewState);
void FLASH_DataCacheReset(void);

/* FLASH Memory Programming functions *****************************************/   
void         FLASH_Unlock(void);
void         FLASH_Lock(void);
FLASH_Status FLASH_EraseSector(uint32_t address);
FLASH_Status FLASH_EraseAllSectors(void);
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);
FLASH_Status FLASH_ProgramByte(uint32_t Address, uint8_t Data);

/* Option Bytes Programming functions *****************************************/ 
void         FLASH_OB_Unlock(void);
void         FLASH_OB_Lock(void);
void 				 FLASH_OB_EOPBConfig(uint8_t OB_EOPB);
void 				 FLASH_OB_WRPConfig(uint32_t OB_WRP1, uint32_t OB_WRP2);
void 				 FLASH_OB_WRP1_AddressConfig(uint32_t OB_WRP_StartAddress, uint32_t OB_WRP_SopAddress);
void 				 FLASH_OB_WRP2_AddressConfig(uint32_t OB_WRP_StartAddress, uint32_t OB_WRP_SopAddress);
void         FLASH_OB_RDPConfig(uint8_t OB_RDP);
void         FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY);
FLASH_Status FLASH_OB_Launch(void);
uint8_t      FLASH_OB_GetUser(void);
uint16_t     FLASH_OB_GetWRP(void);
FlagStatus   FLASH_OB_GetRDP(void);

/* Interrupts and flags management functions **********************************/
void         FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState);
FlagStatus   FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void         FLASH_ClearFlag(uint32_t FLASH_FLAG);
FLASH_Status FLASH_GetStatus(void);
FLASH_Status FLASH_WaitForLastOperation(void);

#ifdef __cplusplus
}
#endif

#endif

/*****END OF FILE****/
