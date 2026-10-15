/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(USART1)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片USART1 利用DMA发送与接收.
*
*            适用芯片: HC32F401系列
*              实验步骤：
*							 PC端->串口调试助手->USB转串口->MCU PA2/PA3(TX/RX)
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. USART/DMA/中断初始化
*              4.1 若为接收测试 等待PC端串口调试助手发送8个字节，进DMA接收完成中断
*              4.2 若为发送测试 等待串口发送8个字节，进DMA发送完成中断

*	
*	注意事项：
* 					通过修改宏定义 TEST_DMA_DIR 
*						0 - USART 利用DMA接受
*						1 - USART 利用DMA发送
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"


/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);			/* 时钟配置函数 */
void SYS_Configuration(void);			/* 系统时钟配置函数 */
void GPIO_Configuration(void);		/* GPIO配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */
void USART_Configuration(void);		/* USART配置函数 */
void DMA_Configuration(void);			/* DMA配置函数 */


#define 	TEST_DMA_DIR	1				//0-SEND	1-RECEIVE


uint8_t 	TX_BUFF[8] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
uint8_t 	RX_BUFF[8];


/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream0_IRQHandler
*	功能说明: DMA1 Stream0 完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA1_Stream0_IRQHandler()
{	
	/* 清除DMA完成中断 */
	DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
	
	/* 接收完成查看RX_BUFF */
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 程序主入口函数。
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{ 
	
	/* 使能外设时钟(GPIOA/B/C/D) USART */
	RCC_Configuration();	
	
	/* 系统时钟配置为168MHz */
	SYS_Configuration();
	
	/* GPIO配置PA2为TX PA3为RX */
	GPIO_Configuration();
	
	/* DMA配置发送/接受通道 */
	DMA_Configuration();
	
	/* 串口1配置 */
	USART_Configuration();
	
	/* 配置中断优先级使能 */	
	NVIC_Configuration();
	
	while(1)
	{

	}
}


/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;							/* 复用功能 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;					/* 内部无上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;					/* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_6); 			//PA2为USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_6);			//PA3为USART1_RX
}


/*
*********************************************************************************************************
*	函 数 名: USART_Configuration
*	功能说明: 串口设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART_Configuration(void)
{
	USART_InitTypeDef 			USART_InitStructure;
	
	USART_InitStructure.USART_OVER = USART_OVER_16;								/* 16倍采样 波特率公式 Baud = PCLK/(BRRI×16+BRRF) */
	USART_InitStructure.USART_BRRI = 546;													/* 波特率整数部分 BRRI = 84000000 / 9600 / 16 ≈ 546.875 取 546 */
	USART_InitStructure.USART_BRRF = 14;													/* 波特率整数部分 BRRF = 0.875 * 16 = 14 */
	USART_InitStructure.USART_MSBF = USART_LSB;										/* 数据帧LSB */			
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		/* 数据帧LSB */	
	USART_InitStructure.USART_Parity = USART_Parity_No;						/* 无校验位 */	
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				/* 1bit停止位 */
	USART_Init(USART1, &USART_InitStructure);
	
	#if TEST_DMA_DIR
		USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);							/* USART1 DMA RX 使能 */
	#else
		USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);							/* USART1 DMA TX 使能 */
	#endif
	
	USART_Cmd(USART1, USART_Mode_Rx | USART_Mode_Tx, ENABLE);			/* 接收发送使能 */
}


/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;

	#if (TEST_DMA_DIR == 1)
		DMA_InitStructure.DMA_Channel = DMA_Channel_4;												/* 配置通道4 USART1_RX */  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->RDR0;		/* 配置外设寄存器地址 */
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_BUFF;						/* 配置存储器地址 */
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;								/* 配置传输方向 外设传向存储器 */
	#else
		DMA_InitStructure.DMA_Channel = DMA_Channel_3; 												/* 配置通道4 USART1_TX */  
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->TDR;		/* 配置外设寄存器地址 */
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TX_BUFF;						/* 配置存储器地址 */
		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;								/* 配置传输方向 存储器传向外设 */
	#endif	
	
  DMA_InitStructure.DMA_BufferSize = (uint32_t)8;													/* buffer深度为8 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				/* 外设地址不自增 */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									/* 存储器地址自增 */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	/* 外设数据为字节 */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					/* 存储器数据为字节 */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													/* 循环模式 */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											/* 优先级高 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        					/* FIFO模式关闭 */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;						/* FIFO模式阈值满（无关） */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;							/* 存储器突发传输模式（无关） */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			/* 外设突发传输模式（无关） */
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
    
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);													/* DMA传输完成中断使能 */
	
	DMA_Cmd(DMA1_Stream0, ENABLE);																					/* DMA1_Stream0 使能 */
}


/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统倍频设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
	RCC_HSEConfig(RCC_HSE_ON);														/* HSE使能  */		
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; /* 等待HSE启动就绪完成  */
		
	RCC_PLLCmd(DISABLE);    															/* PLL关闭 */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);											/* 设置系统时钟分频 */
	RCC_PCLK1Config(RCC_HCLK_Div4);												/* 设置PCLK1时钟分频 */
	RCC_PCLK2Config(RCC_HCLK_Div2);												/* 设置PCLK2时钟分频 */
	
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    				/* PLL时钟配置为168MHz  M N P Q */ 
		
	RCC_PLLCmd(ENABLE);   																/* PLL使能 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; /* PLL倍频就绪完成 */  
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    				/* 系统时钟切换为PLL时钟源 */  
	while (RCC_GetSYSCLKSource() != 0x08){}    						/* 等待系统时钟切换完成 */ 
}


/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 中断优先级设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
