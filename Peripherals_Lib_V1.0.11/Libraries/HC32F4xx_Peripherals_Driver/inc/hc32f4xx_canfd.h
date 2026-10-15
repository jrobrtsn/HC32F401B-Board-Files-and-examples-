/**
  *
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HC32F4xx_CANFD_H
#define __HC32F4xx_CANFD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hc32f4xx.h"


#define CAN_FILTER_BASE				((uint32_t)0x4002C000)
#define CAN_RxFIFO0_BASE			((uint32_t)0x4002C080)
#define CAN_RxFIFO1_BASE			((uint32_t)0x4002C500)
#define CAN_RxBUFFER_BASE			((uint32_t)0x4002C980)
#define CAN_TxEVENTFIFO_BASE	((uint32_t)0x4002CB80)
#define CAN_TxBUFFER_BASE			((uint32_t)0x4002CDC0)

/* Time out for INAK bit */
#define INAK_TIMEOUT        ((uint32_t)0x0000FFFF)


#define CAN_CCCR_INIT				((uint32_t)0x00000001)
#define CAN_CCCR_CCE				((uint32_t)0x00000002)

#define CAN_CCCR_CSA				((uint32_t)0x00000008)
#define CAN_CCCR_CSR				((uint32_t)0x00000010)
#define CAN_CCCR_MON				((uint32_t)0x00000020)
#define CAN_CCCR_DAR				((uint32_t)0x00000040)
#define CAN_CCCR_TEST				((uint32_t)0x00000080)
#define CAN_CCCR_FDOE				((uint32_t)0x00000100)
#define CAN_CCCR_BRSE				((uint32_t)0x00000200)
#define CAN_CCCR_UTTSU			((uint32_t)0x00000400)
#define CAN_CCCR_WMM				((uint32_t)0x00000800)
#define CAN_CCCR_PXHD				((uint32_t)0x00001000)
#define CAN_CCCR_EFBI				((uint32_t)0x00002000)
#define CAN_CCCR_TXP				((uint32_t)0x00004000)


#define CAN_TSCC_TCP 				((uint32_t)0x000F0000)
#define CAN_TSCC_TSS 				((uint32_t)0x00000001)

#define CAN_TOCC_ETOC 			((uint32_t)0x00000001)


#define CAN_TXBC_TXSEQUENCEMASK 				0x4F00FFFF
#define CAN_TXBC_TXBUFFERMASK 					0x000FFFFF

#define CAN_TXESC_TXBUFFERMASK 					0x00000007

#define CAN_RXESC_F0DSMASK 							0x00000007
#define CAN_RXESC_F1DSMASK 							0x00000070
#define CAN_RXESC_RBDSMASK 							0x00000700

#define CAN_ILE_MASK										0x00000003

#define CAN_TXBTIE_MASK									0x0000000F

#define CAN_TXBCIE_MASK									0x0000000F

typedef struct
{
  uint16_t CAN_DPrescaler; 
  uint8_t  CAN_DLatencyCompensation;         
  uint8_t  CAN_DSJW;          
  uint8_t  CAN_DBS1;          
  uint8_t  CAN_DBS2; 
	
  uint16_t CAN_NPrescaler;     
  uint8_t  CAN_NSJW;          
  uint8_t  CAN_NBS1;          
  uint8_t  CAN_NBS2; 

  FunctionalState TransferPause;	
  FunctionalState EdgeFiltering;	
  FunctionalState NoErrorHandling;
  FunctionalState WideMessageMarker;	
  FunctionalState Timestamp;	
	FunctionalState BitRateSwitch;	
  FunctionalState CAN_FD;	
  FunctionalState CAN_Test;	
  FunctionalState CAN_DAR;																	
  FunctionalState CAN_BusMonitor;		
} CAN_InitTypeDef;


/**
 * @brief CAN Mismatch Storage Type 
 */
typedef enum
{
    CAN_StorageType_FIFO0 = 0, 	
    CAN_StorageType_FIFO1,
    CAN_StorageType_Reject
} CanStorageType;


/**
 * @brief CAN filter Disposal 
 */
typedef enum
{
    CanFilterDisposal_Filter = 0, 	
    CanFilterDisposal_Reject
} CanFilterDisposal;


/** 
  * @brief  CAN filter init structure definition
  */
typedef struct
{
	CanStorageType 			CAN_MismatchStandardFrame;
	CanStorageType 			CAN_MismatchExtendedFrame;
	CanFilterDisposal 	CAN_StandardRemoteFrame;
	CanFilterDisposal 	CAN_ExtendedRemoteFrame;
} CAN_GlobalFilterInitTypeDef;


/** 
  * @brief  CAN SIDFilter init structure definition
  */
typedef struct
{
	uint32_t 	CAN_Filter_Depth;
	uint16_t 	CAN_Filter_StartAddress;
} CAN_FilterInitTypeDef;


/** 
  * @brief  CAN TXBuffer init structure definition
  */
typedef struct
{
	uint32_t 	CAN_TXBuffer_Depth;
	uint32_t 	CAN_TXBuffer_Width;
	uint16_t 	CAN_TXBuffer_StartAddress;
} CAN_TXBufferInitTypeDef;	


/** 
  * @brief  CAN RXBuffer init structure definition
  */
typedef struct
{
	uint32_t 	CAN_RXBuffer_Width;
	uint32_t 	CAN_RXBuffer_StartAddress;
} CAN_RXBufferInitTypeDef;	


/** 
  * @brief  CAN TX FIFO/Queue init structure definition
  */
typedef struct
{
	uint32_t 	CAN_TXSequence_mode;
	uint32_t 	CAN_TXSequence_Depth;
	uint32_t 	CAN_TXSequence_StartAddress;
} CAN_TXSequenceInitTypeDef;	


/** 
  * @brief  CAN FIFO init structure definition
  */
typedef struct
{
	uint32_t 	CAN_RXFIFO_Mode;
	uint32_t 	CAN_RXFIFO_ThresholdAlarmLine;
	uint32_t 	CAN_RXFIFO_Depth;
	uint32_t 	CAN_RXFIFO_Width;
	uint32_t 	CAN_RXFIFO_BaseAddress;
} CAN_RXFIFOInitTypeDef;	


/**
 * @brief CAN Identifier type
 */
typedef enum
{
    CAN_StdId = 0, 	/*!< Standard Id */
    CAN_ExtId		    /*!< Extended Id */
} Can_TypeId;
/**
 * @brief CAN_remote_transmission_request
 */
typedef enum
{
    CAN_DataFrame   = 0, 
    CAN_RemoteFrame
} Can_TypeFrame;


/** 
  * @brief  CAN Tx message structure definition  
*/
typedef struct
{
	FunctionalState		Err_State;  		//T0.ESI	
	
	Can_TypeId     		TypeId;					//T0.XTD (11bit StdId / 29bit ExtId)	
	
	Can_TypeFrame			TypeFrame;			//T0.RTR (Data Frame / Remote Frame)

	uint32_t 					ID;							//T0.ID[28:0] (11bit StdId[28:18] / 29bit ExtId)	
		
	uint8_t						MesMarkerL;			//T1.MM[7:0]
	
	FunctionalState		EventFIFOStore;	//T1.EFC
	
	FunctionalState		FdMode;					//T1.FD (Standard CAN /  CANFD)
	
	FunctionalState		BitRateSwitch; 	//T1.BRS 
	
	uint8_t						DLC;						//T1.DLC[3:0]

	uint8_t						MesMarkerH;			//T1.MM[15:8] only CCCR.WMM = 1
	
  uint32_t 					Data[16];				//T2...Tn: n(2..17)  Depending on the size (TXESC) configuration 
} CanTxMessage;


/** 
  * @brief  CAN Rx message structure definition  
*/	
typedef struct
{
	uint8_t						Err_State;  		//R0.ESI
	
	uint8_t     			TypeId;					//R0.XTD (11bit StdId / 29bit ExtId)	
	
	uint8_t						TypeFrame;			//R0.RTR (Data Frame / Remote Frame)
	
	uint32_t 					ID;							//R0.ID[28:0] (11bit StdId[28:18] / 29bit ExtId)	
		
	uint8_t						NoMatchFrame;		//R1.ANMF
	
	uint8_t						FilterIndex;		//R1.FIDX[2:0]
	
	uint8_t						FdMode;					//R1.FD (Standard CAN /  CANFD)
	
	uint8_t						BitRateSwitch; 	//R1.BRS
	
	uint8_t						DLC;						//R1.DLC[3:0]
	
	uint16_t					Timesample;			//R1.RXTS[15:0]
	
  uint32_t 					Data[16];				// R2...Rn: n(2..17)  Depending on the size (RXESC) configuration 
} CanRxMessage;


/** @defgroup CAN_FilterType 
  * @{
  */
#define CAN_FilterType_SFID1toSFID2  						(uint32_t)0x0
#define CAN_FilterType_SFID1andSFID2  					(uint32_t)0x1
#define CAN_FilterType_SFID1Filter_SFID2Block  	(uint32_t)0x2
#define CAN_FilterType_SFIDDisable  						(uint32_t)0x3


/** @defgroup CAN_FilterDisposal 
  * @{
  */
#define CAN_FilterDisposal_Disable							(uint32_t)0x0
#define CAN_FilterDisposal_FIFO0								(uint32_t)0x1
#define CAN_FilterDisposal_FIFO1								(uint32_t)0x2
#define CAN_FilterDisposal_RejectID							(uint32_t)0x3
#define CAN_FilterDisposal_PriorityOnly					(uint32_t)0x4
#define CAN_FilterDisposal_PriorityFIFO0				(uint32_t)0x5
#define CAN_FilterDisposal_PriorityFIFO1				(uint32_t)0x6
#define CAN_FilterDisposal_BUFFER								(uint32_t)0x7




typedef struct 
{
	uint32_t FilterType;			//STF;	
	uint32_t FilterDisposal; 	//SEFC					
	uint32_t SFID1;						//Standard Filter Id2           
	uint32_t SFID2;			 			//Standard Filter Id2                   
}CanSIDFilterMessage;


/** @defgroup CAN_TTPrescaler
  * @{
  */
#define	CAN_TTPrescaler_0							(uint32_t)0x00000000
#define	CAN_TTPrescaler_1							(uint32_t)0x00010000
#define	CAN_TTPrescaler_2							(uint32_t)0x00020000 
#define	CAN_TTPrescaler_3							(uint32_t)0x00030000  
#define	CAN_TTPrescaler_4							(uint32_t)0x00040000
#define	CAN_TTPrescaler_5							(uint32_t)0x00050000  
#define	CAN_TTPrescaler_6							(uint32_t)0x00060000  
#define	CAN_TTPrescaler_7							(uint32_t)0x00070000  
#define	CAN_TTPrescaler_8							(uint32_t)0x00080000  
#define	CAN_TTPrescaler_9							(uint32_t)0x00090000  
#define	CAN_TTPrescaler_10						(uint32_t)0x000A0000  
#define	CAN_TTPrescaler_11						(uint32_t)0x000B0000  
#define	CAN_TTPrescaler_12						(uint32_t)0x000C0000  
#define	CAN_TTPrescaler_13						(uint32_t)0x000D0000  
#define	CAN_TTPrescaler_14						(uint32_t)0x000E0000  
#define	CAN_TTPrescaler_15						(uint32_t)0x000F0000 

/** @defgroup CAN_sleep_constants 
  * @{
  */
#define CAN_InitStatus_Failed              ((uint8_t)0x00) /*!< CAN initialization failed */
#define CAN_InitStatus_Success             ((uint8_t)0x01) /*!< CAN initialization OK */


/** @defgroup CAN_buffer_Width
  * @{
  */ 
#define CAN_BufferWidth_8BYTES					((uint32_t)0x0)
#define CAN_BufferWidth_12BYTES					((uint32_t)0x1)
#define CAN_BufferWidth_16BYTES					((uint32_t)0x2)
#define CAN_BufferWidth_20BYTES					((uint32_t)0x3)
#define CAN_BufferWidth_24BYTES					((uint32_t)0x4)
#define CAN_BufferWidth_32BYTES					((uint32_t)0x5)
#define CAN_BufferWidth_48BYTES					((uint32_t)0x6)
#define CAN_BufferWidth_64BYTES					((uint32_t)0x7)


/** @defgroup CAN_FIFO_Width
  * @{
  */ 
#define CAN_FIFOWidth_8BYTES					((uint32_t)0x0)
#define CAN_FIFOWidth_12BYTES					((uint32_t)0x1)
#define CAN_FIFOWidth_16BYTES					((uint32_t)0x2)
#define CAN_FIFOWidth_20BYTES					((uint32_t)0x3)
#define CAN_FIFOWidth_24BYTES					((uint32_t)0x4)
#define CAN_FIFOWidth_32BYTES					((uint32_t)0x5)
#define CAN_FIFOWidth_48BYTES					((uint32_t)0x6)
#define CAN_FIFOWidth_64BYTES					((uint32_t)0x7)


/** @defgroup CAN_FIFO_MODE
  * @{
  */ 
#define CAN_FIFOMode_Block					((uint32_t)0x0)
#define CAN_FIFOMode_Overwrite			((uint32_t)0x1)

/** @defgroup CAN TX Sequence mode 
  * @{
  */ 

#define	CAN_TXSequence_FIFO							((uint32_t)0x0)
#define	CAN_TXSequence_Queue						((uint32_t)0x1)


///** @defgroup CAN_Mode 
//  * @{
//  */

//#define CAN_Mode_Normal             ((uint8_t)0x00)  /*!< normal mode */
//#define CAN_Mode_LoopBack           ((uint8_t)0x01)  /*!< loopback mode */
//#define CAN_Mode_Silent             ((uint8_t)0x02)  /*!< silent mode */
//#define CAN_Mode_Silent_LoopBack    ((uint8_t)0x03)  /*!< loopback combined with silent mode */

/** @defgroup CAN_FIFOx
  * @{
  */ 
#define CAN_FIFO_0										((uint32_t)0x0)
#define CAN_FIFO_1										((uint32_t)0x1)    


/** @defgroup CAN_interrupts_line
  * @{
  */
#define	CAN_INT0											(uint32_t)0x0
#define	CAN_INT1											(uint32_t)0x1


/** @defgroup CAN_interrupts 
  * @{
  */ 
#define	CAN_IT_RF0N 									((uint32_t)0x00000001)
#define	CAN_IT_RF0W 									((uint32_t)0x00000002)
#define	CAN_IT_RF0F 									((uint32_t)0x00000004)
#define	CAN_IT_RF0L 									((uint32_t)0x00000008)
#define	CAN_IT_RF1N 									((uint32_t)0x00000010)
#define	CAN_IT_RF1W 									((uint32_t)0x00000020)
#define	CAN_IT_RF1F 									((uint32_t)0x00000040)
#define	CAN_IT_RF1L 									((uint32_t)0x00000080)
#define	CAN_IT_HPM	 									((uint32_t)0x00000100)
#define	CAN_IT_TC 										((uint32_t)0x00000200)
#define	CAN_IT_TCF 										((uint32_t)0x00000400)
#define	CAN_IT_TFE	 									((uint32_t)0x00000800)
#define	CAN_IT_TEFN 									((uint32_t)0x00001000)
#define	CAN_IT_TEFW 									((uint32_t)0x00002000)
#define	CAN_IT_TEFF 									((uint32_t)0x00004000)
#define	CAN_IT_TEFL 									((uint32_t)0x00008000)
#define	CAN_IT_TSW	 									((uint32_t)0x00010000)
#define	CAN_IT_MRAF 									((uint32_t)0x00020000)
#define	CAN_IT_TOO 										((uint32_t)0x00040000)
#define	CAN_IT_DRX	 									((uint32_t)0x00080000)
#define	CAN_IT_BEC 										((uint32_t)0x00100000)
#define	CAN_IT_BEU 										((uint32_t)0x00200000)
#define	CAN_IT_ELO										((uint32_t)0x00400000)
#define	CAN_IT_EP 										((uint32_t)0x00800000)
#define	CAN_IT_EW 										((uint32_t)0x01000000)
#define	CAN_IT_BO 										((uint32_t)0x02000000)
#define	CAN_IT_WDI										((uint32_t)0x04000000)
#define	CAN_IT_PEA 										((uint32_t)0x08000000)
#define	CAN_IT_PED 										((uint32_t)0x10000000)
#define	CAN_IT_ARA 										((uint32_t)0x20000000)


/** @defgroup CAN_RequestType
  * @{
  */ 
#define	CAN_RequestType_Transfer			0
#define	CAN_RequestType_Abort					1


/** @defgroup CAN_Index
  * @{
  */ 
#define CAN_Index_0			0x01
#define CAN_Index_1			0x02
#define CAN_Index_2			0x04
#define CAN_Index_3			0x08
#define CAN_Index_4			0x10
#define CAN_Index_5			0x20
#define CAN_Index_6			0x40
#define CAN_Index_7			0x80
#define CAN_Index_ALL		0xFF


/*  Function used to set the CAN configuration to the default reset state *****/ 
void CAN_DeInit(CAN_TypeDef* CANx);

/* Initialization and Configuration functions *********************************/ 
uint8_t CAN_EnterInitMode(CAN_TypeDef* CANx);
uint8_t CAN_ExitInitMode(CAN_TypeDef* CANx);
void CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
void CAN_TimestampConfig(CAN_TypeDef* CANx, uint32_t CAN_TTPrescaler, FunctionalState NewState);
void CAN_TimeOutConfig(CAN_TypeDef* CANx, uint32_t CAN_TimeOutEvent, uint32_t CAN_TimeOutCount, FunctionalState NewState);
void CAN_RamWatchdogConfig(CAN_TypeDef* CANx, uint32_t CAN_RWDCount, FunctionalState NewState);

void CAN_GlobalFilterInit(CAN_TypeDef* CANx, CAN_GlobalFilterInitTypeDef* CAN_FilterInitStruct);
void CAN_FilterConfig(CAN_TypeDef* CANx, uint32_t CAN_TypeID, CAN_FilterInitTypeDef* CAN_FilterInitStruct);

void CAN_SendBufferConfig(CAN_TypeDef* CANx, CAN_TXBufferInitTypeDef* CAN_TXBufferInitStruct);
void CAN_ReceiveBufferConfig(CAN_TypeDef* CANx, CAN_RXBufferInitTypeDef* CAN_RXBufferInitStruct);

void CAN_SendSequenceConfig(CAN_TypeDef* CANx, CAN_TXSequenceInitTypeDef* CAN_TXSequenceInitStruct);
void CAN_ReceiveFIFOConfig(CAN_TypeDef* CANx, uint32_t CAN_FIFOx, CAN_RXFIFOInitTypeDef* CAN_RXFIFOInitStruct);

///* Transmit functions *********************************************************/
void CAN_CopySIDFilterMessageToRAM(CAN_TypeDef* CANx, uint32_t msg_addr, CanSIDFilterMessage* Filter_msg);

void CAN_CopyTxMessageToRAM(CAN_TypeDef* CANx, uint32_t msg_addr, CanTxMessage* tx_msg);
void CAN_CopyRAMToRxMessage(CAN_TypeDef* CANx, uint32_t msg_addr, CanRxMessage* rx_msg);

void CAN_RequestConfig(CAN_TypeDef* CANx, uint32_t CAN_RequestType, uint32_t Index);
///* Operation modes functions **************************************************/


///* Error management functions *************************************************/


///* Interrupts and flags management functions **********************************/
void CAN_ITConfig(CAN_TypeDef* CANx, uint32_t CAN_IT, uint32_t CAN_INTx, FunctionalState NewState);
ITStatus CAN_GetITStatus(CAN_TypeDef* CANx, uint32_t CAN_IT);
void CAN_ClearITPendingBit(CAN_TypeDef* CANx, uint32_t CAN_IT);

#ifdef __cplusplus
}
#endif

#endif 

/******END OF FILE****/
