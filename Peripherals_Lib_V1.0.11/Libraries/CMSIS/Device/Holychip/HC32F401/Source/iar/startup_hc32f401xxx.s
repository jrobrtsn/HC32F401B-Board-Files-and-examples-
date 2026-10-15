
;*******************************************************************************/
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler

        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

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
        DCD     RFU1_IRQHandler                                                             
        DCD     RFU2_IRQHandler                                                             
        DCD     CAN1_INT0_IRQHandler              ; CAN1 INT0                                               
        DCD     CAN1_INT1_IRQHandler              ; CAN1 INT1                                              
        DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s                                    
        DCD     TIM1_BRK_IRQHandler               ; TIM1 Break                   
        DCD     TIM1_UP_IRQHandler                ; TIM1 Update                 
        DCD     TIM1_TRG_COM_IRQHandler           ; TIM1 Trigger and Commutation
        DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare                                   
        DCD     TIM2_IRQHandler                   ; TIM2                                            
        DCD     TIM3_IRQHandler                   ; TIM3                                            
        DCD     TIM4_IRQHandler                   ; TIM4                                            
        DCD     I2C1_EV_IRQHandler                ; I2C1 Event                                             
        DCD     RFU3_IRQHandler                                                         
        DCD     I2C2_EV_IRQHandler                ; I2C2 Event                                             
        DCD     RFU4_IRQHandler                                                           
        DCD     SPI1_IRQHandler                   ; SPI1                                            
        DCD     SPI2_IRQHandler                   ; SPI2                                            
        DCD     USART1_IRQHandler                 ; USART1                                          
        DCD     USART2_IRQHandler                 ; USART2                                          
        DCD     USART3_IRQHandler                 ; USART3                                          
        DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s                                  
        DCD     RTC_Alarm_IRQHandler              ; RTC Alarm (A and B) through EXTI Line                  
        DCD     USB_WKUP_IRQHandler               ; USB FS Wakeup through EXTI line                        
        DCD     TIM8_BRK_IRQHandler               ; TIM8 Break                  
        DCD     TIM8_UP_IRQHandler                ; TIM8 Update               
        DCD     TIM8_TRG_COM_IRQHandler           ; TIM8 Trigger and Commutation 
        DCD     TIM8_CC_IRQHandler                ; TIM8 Capture Compare                                   
        DCD     DMA1_Stream7_IRQHandler           ; DMA1 Stream7                                           
        DCD     FSMC_IRQHandler                   ; FSMC                                            
        DCD     SDIO_IRQHandler                   ; SDIO                                            
        DCD     TIM5_IRQHandler                   ; TIM5                                            
        DCD     SPI3_IRQHandler                   ; SPI3                                            
        DCD     UART4_IRQHandler                  ; UART4                                           
        DCD     UART5_IRQHandler                  ; UART5                                           
        DCD     TIM6_IRQHandler                   ; TIM6              
        DCD     TIM7_IRQHandler                   ; TIM7                   
        DCD     TIM8_IRQHandler                   ; TIM8                                  
        DCD     TIM9_IRQHandler                   ; TIM9                                 
        DCD     TIM10_IRQHandler                  ; TIM10                                  
        DCD     RFU5_IRQHandler                                  
        DCD     VC0_IRQHandler                    ; VC0                                                 
        DCD     VC1_IRQHandler                    ; VC1                                                         
        DCD     VC2_IRQHandler                    ; VC2                                   
        DCD     VC_Wakeup_IRQHandler                                                       
        DCD     RFU6_IRQHandler                                                      
        DCD     RFU7_IRQHandler                                                             
        DCD     RFU8_IRQHandler                                                             
        DCD     RFU9_IRQHandler                                           
        DCD     USB_LP_IRQHandler                                            
        DCD     USB_HP_IRQHandler                                           
        DCD     RFU10_IRQHandler                            
        DCD     RFU11_IRQHandler                                                      
        DCD     RFU12_IRQHandler                                                         
        DCD     RFU13_IRQHandler                                                     
        DCD     RFU14_IRQHandler                              
        DCD     RFU15_IRQHandler                               
        DCD     RFU16_IRQHandler                               
        DCD     RFU17_IRQHandler                                              
        DCD     RFU18_IRQHandler                                              
        DCD     RFU19_IRQHandler                                   
        DCD     RFU20_IRQHandler  
        DCD     FPU_IRQHandler                    ; FPU
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler

        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B SysTick_Handler

        PUBWEAK WWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WWDG_IRQHandler  
        B WWDG_IRQHandler

        PUBWEAK PVD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PVD_IRQHandler  
        B PVD_IRQHandler

        PUBWEAK TAMP_STAMP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TAMP_STAMP_IRQHandler  
        B TAMP_STAMP_IRQHandler

        PUBWEAK RTC_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RTC_WKUP_IRQHandler  
        B RTC_WKUP_IRQHandler

        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FLASH_IRQHandler  
        B FLASH_IRQHandler

        PUBWEAK RCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RCC_IRQHandler  
        B RCC_IRQHandler

        PUBWEAK EXTI0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI0_IRQHandler  
        B EXTI0_IRQHandler

        PUBWEAK EXTI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI1_IRQHandler  
        B EXTI1_IRQHandler

        PUBWEAK EXTI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI2_IRQHandler  
        B EXTI2_IRQHandler

        PUBWEAK EXTI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EXTI3_IRQHandler
        B EXTI3_IRQHandler

        PUBWEAK EXTI4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
EXTI4_IRQHandler  
        B EXTI4_IRQHandler

        PUBWEAK DMA1_Stream0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream0_IRQHandler  
        B DMA1_Stream0_IRQHandler

        PUBWEAK DMA1_Stream1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream1_IRQHandler  
        B DMA1_Stream1_IRQHandler

        PUBWEAK DMA1_Stream2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream2_IRQHandler  
        B DMA1_Stream2_IRQHandler

        PUBWEAK DMA1_Stream3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream3_IRQHandler  
        B DMA1_Stream3_IRQHandler

        PUBWEAK DMA1_Stream4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream4_IRQHandler  
        B DMA1_Stream4_IRQHandler

        PUBWEAK DMA1_Stream5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream5_IRQHandler  
        B DMA1_Stream5_IRQHandler

        PUBWEAK DMA1_Stream6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream6_IRQHandler  
        B DMA1_Stream6_IRQHandler

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler  
        B ADC_IRQHandler

        PUBWEAK RFU1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
RFU1_IRQHandler  
        B RFU1_IRQHandler

        PUBWEAK RFU2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RFU2_IRQHandler  
        B RFU2_IRQHandler

        PUBWEAK CAN1_INT0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
CAN1_INT0_IRQHandler  
        B CAN1_INT0_IRQHandler

        PUBWEAK CAN1_INT1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
CAN1_INT1_IRQHandler  
        B CAN1_INT1_IRQHandler

        PUBWEAK EXTI9_5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
EXTI9_5_IRQHandler  
        B EXTI9_5_IRQHandler

        PUBWEAK TIM1_BRK_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM1_BRK_IRQHandler  
        B TIM1_BRK_IRQHandler

        PUBWEAK TIM1_UP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM1_UP_IRQHandler  
        B TIM1_UP_IRQHandler

        PUBWEAK TIM1_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM1_TRG_COM_IRQHandler  
        B TIM1_TRG_COM_IRQHandler
        
        PUBWEAK TIM1_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM1_CC_IRQHandler  
        B TIM1_CC_IRQHandler

        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM2_IRQHandler  
        B TIM2_IRQHandler

        PUBWEAK TIM3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM3_IRQHandler  
        B TIM3_IRQHandler

        PUBWEAK TIM4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM4_IRQHandler  
        B TIM4_IRQHandler

        PUBWEAK I2C1_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
I2C1_EV_IRQHandler  
        B I2C1_EV_IRQHandler

        PUBWEAK RFU3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
RFU3_IRQHandler  
        B RFU3_IRQHandler

        PUBWEAK I2C2_EV_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
I2C2_EV_IRQHandler  
        B I2C2_EV_IRQHandler

        PUBWEAK RFU4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
RFU4_IRQHandler  
        B RFU4_IRQHandler

        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_IRQHandler  
        B SPI1_IRQHandler

        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI2_IRQHandler  
        B SPI2_IRQHandler

        PUBWEAK USART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART1_IRQHandler  
        B USART1_IRQHandler

        PUBWEAK USART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART2_IRQHandler  
        B USART2_IRQHandler

        PUBWEAK USART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
USART3_IRQHandler  
        B USART3_IRQHandler

        PUBWEAK EXTI15_10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)   
EXTI15_10_IRQHandler  
        B EXTI15_10_IRQHandler

        PUBWEAK RTC_Alarm_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)   
RTC_Alarm_IRQHandler  
        B RTC_Alarm_IRQHandler

        PUBWEAK USB_WKUP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
USB_WKUP_IRQHandler  
        B USB_WKUP_IRQHandler
      
        PUBWEAK TIM8_BRK_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM8_BRK_IRQHandler  
        B TIM8_BRK_IRQHandler

        PUBWEAK TIM8_UP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM8_UP_IRQHandler  
        B TIM8_UP_IRQHandler

        PUBWEAK TIM8_TRG_COM_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM8_TRG_COM_IRQHandler  
        B TIM8_TRG_COM_IRQHandler

        PUBWEAK TIM8_CC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
TIM8_CC_IRQHandler  
        B TIM8_CC_IRQHandler

        PUBWEAK DMA1_Stream7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
DMA1_Stream7_IRQHandler  
        B DMA1_Stream7_IRQHandler

        PUBWEAK FSMC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
FSMC_IRQHandler  
        B FSMC_IRQHandler

        PUBWEAK SDIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SDIO_IRQHandler  
        B SDIO_IRQHandler

        PUBWEAK TIM5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIM5_IRQHandler  
        B TIM5_IRQHandler

        PUBWEAK SPI3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SPI3_IRQHandler  
        B SPI3_IRQHandler

        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART4_IRQHandler  
        B UART4_IRQHandler

        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART5_IRQHandler  
        B UART5_IRQHandler

        PUBWEAK TIM6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)   
TIM6_IRQHandler  
        B TIM6_IRQHandler

        PUBWEAK TIM7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)   
TIM7_IRQHandler  
        B TIM7_IRQHandler

        PUBWEAK TIM8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM8_IRQHandler  
        B TIM8_IRQHandler

        PUBWEAK TIM9_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM9_IRQHandler  
        B TIM9_IRQHandler

        PUBWEAK TIM10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
TIM10_IRQHandler  
        B TIM10_IRQHandler

        PUBWEAK RFU5_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
RFU5_IRQHandler  
        B RFU5_IRQHandler

        PUBWEAK VC0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
VC0_IRQHandler  
        B VC0_IRQHandler

        PUBWEAK VC1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
VC1_IRQHandler  
        B VC1_IRQHandler

        PUBWEAK VC2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
VC2_IRQHandler  
        B VC2_IRQHandler

        PUBWEAK VC_Wakeup_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
VC_Wakeup_IRQHandler  
        B VC_Wakeup_IRQHandler

        PUBWEAK RFU6_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RFU6_IRQHandler  
        B RFU6_IRQHandler

        PUBWEAK RFU7_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RFU7_IRQHandler  
        B RFU7_IRQHandler

        PUBWEAK RFU8_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RFU8_IRQHandler  
        B RFU8_IRQHandler

        PUBWEAK RFU9_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RFU9_IRQHandler  
        B RFU9_IRQHandler

        PUBWEAK USB_LP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
USB_LP_IRQHandler  
        B USB_LP_IRQHandler

        PUBWEAK USB_HP_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
USB_HP_IRQHandler  
        B USB_HP_IRQHandler

        PUBWEAK RFU10_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
RFU10_IRQHandler  
        B RFU10_IRQHandler

        PUBWEAK RFU11_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RFU11_IRQHandler  
        B RFU11_IRQHandler

        PUBWEAK RFU12_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
RFU12_IRQHandler  
        B RFU12_IRQHandler

        PUBWEAK RFU13_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1) 
RFU13_IRQHandler  
        B RFU13_IRQHandler

        PUBWEAK RFU14_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
RFU14_IRQHandler  
        B RFU14_IRQHandler

        PUBWEAK RFU15_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
RFU15_IRQHandler  
        B RFU15_IRQHandler

        PUBWEAK RFU16_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)    
RFU16_IRQHandler  
        B RFU16_IRQHandler

        PUBWEAK RFU17_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RFU17_IRQHandler  
        B RFU17_IRQHandler

        PUBWEAK RFU18_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RFU18_IRQHandler  
        B RFU18_IRQHandler

        PUBWEAK RFU19_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RFU19_IRQHandler  
        B RFU19_IRQHandler

        PUBWEAK RFU20_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
RFU20_IRQHandler  
        B RFU20_IRQHandler

        PUBWEAK FPU_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)  
FPU_IRQHandler  
        B FPU_IRQHandler

        END
/*****END OF FILE****/
