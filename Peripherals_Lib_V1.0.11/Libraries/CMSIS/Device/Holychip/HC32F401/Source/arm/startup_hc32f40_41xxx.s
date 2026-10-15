;*******************************************************************************
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                   ; Window WatchDog                                        
                DCD     PVD_IRQHandler                    ; PVD through EXTI Line detection                        
                DCD     TAMP_STAMP_IRQHandler             ; Tamper and TimeStamps through the EXTI line            
                DCD     RTC_WKUP_IRQHandler               ; RTC Wakeup through the EXTI line                       
                DCD     FLASH_IRQHandler                  ; FLASH                                           
                DCD     RCC_IRQHandler                    ; RCC                                             
                DCD     EXTI0_IRQHandler                  ; EXTI Line0                                             
                DCD     EXTI1_IRQHandler                  ; EXTI Line1                                             
                DCD     EXTI2_IRQHandler                  ; EXTI Line2                                             
                DCD     EXTI3_IRQHandler                  ; EXTI Line3                                             
                DCD     EXTI4_IRQHandler                  ; EXTI Line4                                             
                DCD     DMA1_Stream0_IRQHandler           ; DMA1 Stream 0                                   
                DCD     DMA1_Stream1_IRQHandler           ; DMA1 Stream 1                                   
                DCD     DMA1_Stream2_IRQHandler           ; DMA1 Stream 2                                   
                DCD     DMA1_Stream3_IRQHandler           ; DMA1 Stream 3                                   
                DCD     DMA1_Stream4_IRQHandler           ; DMA1 Stream 4                                  
                DCD     DMA1_Stream5_IRQHandler           ; DMA1 Stream 5                                   
                DCD     DMA1_Stream6_IRQHandler           ; DMA1 Stream 6                                   
                DCD     ADC_IRQHandler                    ; ADC1, ADC2 and ADC3s                            
                DCD     RFU1_IRQHandler                	  ; Reserved                                               
                DCD     RFU2_IRQHandler               	  ; Reserved                                                
                DCD     CAN1_INT0_IRQHandler              ; CAN1 INT0                                               
                DCD     CAN1_INT1_IRQHandler              ; CAN1 INT1                                              
                DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s                                    
                DCD     TIM1_BRK_IRQHandler          	  ; TIM1 Break                   
                DCD     TIM1_UP_IRQHandler          	  ; TIM1 Update                 
                DCD     TIM1_TRG_COM_IRQHandler           ; TIM1 Trigger and Commutation 
                DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare                                   
                DCD     TIM2_IRQHandler                   ; TIM2                                            
                DCD     TIM3_IRQHandler                   ; TIM3                                            
                DCD     TIM4_IRQHandler                   ; TIM4                                            
                DCD     I2C1_EV_IRQHandler                ; I2C1 Event                                             
                DCD     RFU3_IRQHandler                   ; Reserved                                            
                DCD     I2C2_EV_IRQHandler                ; I2C2 Event                                             
                DCD     RFU4_IRQHandler                	  ; Reserved                                               
                DCD     SPI1_IRQHandler                   ; SPI1                                            
                DCD     SPI2_I2S2_IRQHandler              ; SPI2                                            
                DCD     USART1_IRQHandler                 ; USART1                                          
                DCD     USART2_IRQHandler                 ; USART2                                          
                DCD     USART3_IRQHandler                 ; USART3                                          
                DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s                                  
                DCD     RTC_Alarm_IRQHandler              ; RTC Alarm (A and B) through EXTI Line                  
                DCD     USBWakeUp_IRQHandler              ; USB Wakeup through EXTI line                        
                DCD     TIM8_BRK_IRQHandler         	  ; TIM8 Break               
                DCD     TIM8_UP_IRQHandler                ; TIM8 Update                
                DCD     TIM8_TRG_COM_IRQHandler		      ; TIM8 Trigger and Commutation
                DCD     TIM8_CC_IRQHandler                ; TIM8 Capture Compare                                   
                DCD     DMA1_Stream7_IRQHandler           ; DMA1 Stream7                                           
                DCD     FSMC_IRQHandler                   ; FSMC                                            
                DCD     SDIO_IRQHandler                   ; SDIO                                            
                DCD     TIM5_IRQHandler                   ; TIM5                                          
                DCD     SPI3_I2S3_IRQHandler              ; SPI3                                            
                DCD     UART4_IRQHandler                  ; UART4                                           
                DCD     UART5_IRQHandler                  ; UART5                                           
                DCD     TIM6_IRQHandler               	  ; TIM6                   
                DCD     TIM7_IRQHandler                   ; TIM7                   
                DCD     TIM9_IRQHandler           		  ; TIM9                                
                DCD     TIM10_IRQHandler           		  ; TIM10                                   
                DCD     RFU5_IRQHandler           		  ; Reserved                                  
                DCD     VC0_IRQHandler           		  ; VC0                                   
                DCD     VC1_IRQHandler           		  ; VC1                                
                DCD     VC2_IRQHandler                    ; VC2                                        
                DCD     VC_Wakeup_IRQHandler              ; VC Wakeup                      
                DCD     RFU6_IRQHandler           		  ; Reserved                                                
                DCD     RFU7_IRQHandler           		  ; Reserved                                               
                DCD     RFU8_IRQHandler           		  ; Reserved                                               
                DCD     RFU9_IRQHandler           		  ; Reserved                                               
                DCD     USB_LP_IRQHandler                 ; USB LP                                   
                DCD     USB_HP_IRQHandler           	  ; USB HP                                   
                DCD     RFU10_IRQHandler           		  ; Reserved                                  
                DCD     RFU11_IRQHandler           		  ; Reserved                                
                DCD     RFU12_IRQHandler           		  ; Reserved                                        
                DCD     RFU13_IRQHandler           		  ; Reserved                                              
                DCD     RFU14_IRQHandler           		  ; Reserved                                            
                DCD     RFU15_IRQHandler           		  ; Reserved                     
                DCD     RFU16_IRQHandler           		  ; Reserved                   
                DCD     RFU17_IRQHandler           		  ; Reserved                    
                DCD     RFU18_IRQHandler           		  ; Reserved                                     
                DCD     RFU19_IRQHandler           		  ; Reserved                                          
                DCD     RFU20_IRQHandler           		  ; Reserved                                 
                DCD     MOU_IRQHandler               	  ; MOU
                DCD     FPU_IRQHandler                    ; FPU
                                         
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]                                        
                EXPORT  PVD_IRQHandler                    [WEAK]                      
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]         
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]                     
                EXPORT  FLASH_IRQHandler                  [WEAK]                                         
                EXPORT  RCC_IRQHandler                    [WEAK]                                            
                EXPORT  EXTI0_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI1_IRQHandler                  [WEAK]                                             
                EXPORT  EXTI2_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI3_IRQHandler                  [WEAK]                                           
                EXPORT  EXTI4_IRQHandler                  [WEAK]                                            
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK]                                
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  ADC_IRQHandler                    [WEAK]                         
                EXPORT  RFU1_IRQHandler                	  [WEAK]                                                
                EXPORT  RFU2_IRQHandler               	  [WEAK]                                               
                EXPORT  CAN1_INT0_IRQHandler              [WEAK]                                                
                EXPORT  CAN1_INT1_IRQHandler              [WEAK]                                                
                EXPORT  EXTI9_5_IRQHandler                [WEAK]                                    
                EXPORT  TIM1_BRK_IRQHandler          	  [WEAK]                  
                EXPORT  TIM1_UP_IRQHandler          	  [WEAK]                
                EXPORT  TIM1_TRG_COM_IRQHandler     	  [WEAK] 
                EXPORT  TIM1_CC_IRQHandler                [WEAK]                                   
                EXPORT  TIM2_IRQHandler                   [WEAK]                                            
                EXPORT  TIM3_IRQHandler                   [WEAK]                                            
                EXPORT  TIM4_IRQHandler                   [WEAK]                                            
                EXPORT  I2C1_EV_IRQHandler                [WEAK]                                             
                EXPORT  RFU3_IRQHandler                	  [WEAK]                                             
                EXPORT  I2C2_EV_IRQHandler                [WEAK]                                            
                EXPORT  RFU4_IRQHandler                	  [WEAK]                                               
                EXPORT  SPI1_IRQHandler                   [WEAK]                                           
                EXPORT  SPI2_I2S2_IRQHandler              [WEAK]                                            
                EXPORT  USART1_IRQHandler                 [WEAK]                                          
                EXPORT  USART2_IRQHandler                 [WEAK]                                          
                EXPORT  USART3_IRQHandler                 [WEAK]                                         
                EXPORT  EXTI15_10_IRQHandler              [WEAK]                                  
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]                  
                EXPORT  USBWakeUp_IRQHandler              [WEAK]                        
                EXPORT  TIM8_BRK_IRQHandler         	  [WEAK]                 
                EXPORT  TIM8_UP_IRQHandler          	  [WEAK]                 
                EXPORT  TIM8_TRG_COM_IRQHandler     	  [WEAK] 
                EXPORT  TIM8_CC_IRQHandler                [WEAK]                                   
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]                                          
                EXPORT  FSMC_IRQHandler                   [WEAK]                                             
                EXPORT  SDIO_IRQHandler                   [WEAK]                                             
                EXPORT  TIM5_IRQHandler                   [WEAK]                                             
                EXPORT  SPI3_I2S3_IRQHandler              [WEAK]                                             
                EXPORT  UART4_IRQHandler                  [WEAK]                                            
                EXPORT  UART5_IRQHandler                  [WEAK]                                            
                EXPORT  TIM6_IRQHandler               	  [WEAK]                   
                EXPORT  TIM7_IRQHandler                   [WEAK]                    
                EXPORT  TIM9_IRQHandler           		  [WEAK]                                  
                EXPORT  TIM10_IRQHandler           		  [WEAK]                                   
                EXPORT  RFU5_IRQHandler           		  [WEAK]                                    
                EXPORT  VC0_IRQHandler           		  [WEAK]                                    
                EXPORT  VC1_IRQHandler           		  [WEAK]                                 
                EXPORT  VC2_IRQHandler                    [WEAK]                                         
                EXPORT  VC_Wakeup_IRQHandler              [WEAK]                     
                EXPORT  RFU6_IRQHandler                	  [WEAK]                                               
                EXPORT  RFU7_IRQHandler               	  [WEAK]                                               
                EXPORT  RFU8_IRQHandler               	  [WEAK]                                               
                EXPORT  RFU9_IRQHandler               	  [WEAK]                                               
                EXPORT  USB_LP_IRQHandler                 [WEAK]                                       
                EXPORT  USB_HP_IRQHandler           	  [WEAK]                                   
                EXPORT  RFU10_IRQHandler           		  [WEAK]                                   
                EXPORT  RFU11_IRQHandler           		  [WEAK]                                   
                EXPORT  RFU12_IRQHandler                  [WEAK]                                           
                EXPORT  RFU13_IRQHandler                  [WEAK]                                              
                EXPORT  RFU14_IRQHandler                  [WEAK]                                              
                EXPORT  RFU15_IRQHandler         		  [WEAK]                      
                EXPORT  RFU16_IRQHandler          		  [WEAK]                      
                EXPORT  RFU17_IRQHandler            	  [WEAK]                        
                EXPORT  RFU18_IRQHandler                  [WEAK]                                      
                EXPORT  RFU19_IRQHandler                  [WEAK]                                             
                EXPORT  RFU20_IRQHandler                  [WEAK]                                     
                EXPORT  MOU_IRQHandler               	  [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]

WWDG_IRQHandler                                                       
PVD_IRQHandler                                      
TAMP_STAMP_IRQHandler                  
RTC_WKUP_IRQHandler                                
FLASH_IRQHandler                                                       
RCC_IRQHandler                                                            
EXTI0_IRQHandler                                                          
EXTI1_IRQHandler                                                           
EXTI2_IRQHandler                                                          
EXTI3_IRQHandler                                                         
EXTI4_IRQHandler                                                          
DMA1_Stream0_IRQHandler                                       
DMA1_Stream1_IRQHandler                                          
DMA1_Stream2_IRQHandler                                          
DMA1_Stream3_IRQHandler                                          
DMA1_Stream4_IRQHandler                                          
DMA1_Stream5_IRQHandler                                          
DMA1_Stream6_IRQHandler                                          
ADC_IRQHandler                                         
RFU1_IRQHandler                                                            
RFU2_IRQHandler                                                          
CAN1_INT0_IRQHandler                                                           
CAN1_INT1_IRQHandler                                                           
EXTI9_5_IRQHandler                                                
TIM1_BRK_IRQHandler                        
TIM1_UP_IRQHandler                      
TIM1_TRG_COM_IRQHandler  
TIM1_CC_IRQHandler                                               
TIM2_IRQHandler                                                           
TIM3_IRQHandler                                                           
TIM4_IRQHandler                                                           
I2C1_EV_IRQHandler                                                         
RFU3_IRQHandler                                                         
I2C2_EV_IRQHandler                                                        
RFU4_IRQHandler                                                           
SPI1_IRQHandler                                                          
SPI2_I2S2_IRQHandler                                                           
USART1_IRQHandler                                                       
USART2_IRQHandler                                                       
USART3_IRQHandler                                                      
EXTI15_10_IRQHandler                                            
RTC_Alarm_IRQHandler                            
USBWakeUp_IRQHandler                                
TIM8_BRK_IRQHandler                      
TIM8_UP_IRQHandler                       
TIM8_TRG_COM_IRQHandler  
TIM8_CC_IRQHandler                                               
DMA1_Stream7_IRQHandler                                                 
FSMC_IRQHandler                                                            
SDIO_IRQHandler                                                            
TIM5_IRQHandler                                                            
SPI3_I2S3_IRQHandler                                                            
UART4_IRQHandler                                                          
UART5_IRQHandler                                                          
TIM6_IRQHandler                            
TIM7_IRQHandler                              
TIM9_IRQHandler                                         
TIM10_IRQHandler                                          
RFU5_IRQHandler                                           
VC0_IRQHandler                                           
VC1_IRQHandler                                        
VC2_IRQHandler                                                         
VC_Wakeup_IRQHandler                                
RFU6_IRQHandler                                                           
RFU7_IRQHandler                                                          
RFU8_IRQHandler                                                          
RFU9_IRQHandler                                                          
USB_LP_IRQHandler                                                    
USB_HP_IRQHandler                                          
RFU10_IRQHandler                                          
RFU11_IRQHandler                                          
RFU12_IRQHandler                                                        
RFU13_IRQHandler                                                          
RFU14_IRQHandler                                                          
RFU15_IRQHandler                           
RFU16_IRQHandler                            
RFU17_IRQHandler                                
RFU18_IRQHandler                                                   
RFU19_IRQHandler                                                            
RFU20_IRQHandler                                                    
MOU_IRQHandler
FPU_IRQHandler
   
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;*****END OF FILE*****
