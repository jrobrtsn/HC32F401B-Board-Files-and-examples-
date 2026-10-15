/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(I2S)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片I2S利用DAM发送.
*
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. I2S/DMA初始化
*              4. I2S时钟生成使能 开始循环发送tx_Buff
*	
*	注意事项：
*
*			I2S 最高发送与接收频率 ≤ 12MHz
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
void I2S_Configuration(void);			/* I2S配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */
void DMA_Configuration(void);			/* DMA配置函数 */

uint8_t i,j;

uint16_t tx_Buff[8]  = {0x1122, 0x3344, 0x5566, 0x7788, 0x9900, 0xAABB, 0xCCDD, 0xEEFF};
uint16_t rx_Buff[8];


/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream0_IRQHandler
*	功能说明: DMA发送完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA1_Stream0_IRQHandler()
{
	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
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
	/* 使能外设时钟(GPIOA/B/C/D) I2S */
	RCC_Configuration();	
	
	/* 系统时钟配置为168MHz I2S时钟为48MHz*/
	SYS_Configuration();
	
	/* GPIO配置*/
	GPIO_Configuration();
	
	/* DMA配置*/
  DMA_Configuration();
	
	/* I2S2配置 */
	I2S_Configuration();
	
	/* 配置中断优先级使能 */
	NVIC_Configuration();
		
	/* I2S发送时钟使能 开始传输*/
	I2S_CLKCmd(I2S2, ENABLE);

	while(1)
	{
	}
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 程序主入口函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD| RCC_AHB1Periph_DMA1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2_I2S2 | RCC_APB1Periph_SPI3_I2S3, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: I2S_Configuration
*	功能说明: I2S配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2S_Configuration(void)
{
	I2S_InitTypeDef I2S_InitStructure;
	
	I2S_InitStructure.I2S_Mode 						= I2S_Mode_Master;						/* I2S2为主模式  */
	I2S_InitStructure.I2S_Direction 			= I2S_Direction_FullDuplex;		/* I2S2为全双工模式  */
	I2S_InitStructure.I2S_RxBitDepth			= I2S_BitDepth_16;						/* 接收分辨率为16bit */
	I2S_InitStructure.I2S_TxBitDepth			= I2S_BitDepth_16;						/* 发送分辨率为16bit */
	I2S_InitStructure.I2S_ClkSampleTime 	= I2S_ClkSampleTime_16;				/* 字选择线为16CLK */
	I2S_InitStructure.I2S_ClkGateTime			= I2S_ClkGateTime_OFF;				/* 门控周期关闭 */
	I2S_InitStructure.I2S_ClkPrescale			= 6;													/* I2S时钟6分频 48/6 = 8MHz*/						
	I2S_Init(I2S2, &I2S_InitStructure);
	
	I2S_Threshold_Config(I2S2, I2S_FIFO_TX, I2S_FIFOThreshold_8);				/* 发送阈值为8 */
	I2S_DMACmd(I2S2, I2S_DMA_TX);																				/* I2S 发送DMA使能 */
	
	I2S_BlockCmd(I2S2, I2S_BLOCK_TX, ENABLE);														/* I2S发送块使能 */											
	
	I2S_Cmd(I2S2, ENABLE);																							/* I2S模块使能 */	
}


/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;

  DMA_InitStructure.DMA_Channel = DMA_Channel_11;  																/* 配置通道11 SPI2_I2S2_TX */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2S2->TXDMA;							/* 配置外设寄存器地址 */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_Buff;											/* 配置存储器地址 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;													/* 配置传输方向 存储器传向外设 */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)8;																	/* buffer深度为8 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;								/* 外设地址不自增 */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;													/* 存储器地址自增 */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			/* 外设数据为半字 */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;							/* 存储器数据为半字 */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;																	/* 循环模式 */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;															/* 优先级高 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        								 	/* FIFO模式关闭 */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;										/* FIFO模式阈值满（无关） */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;											/* 存储器突发传输模式（无关） */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;							/* 外设突发传输模式（无关） */
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
    
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);																	/* DMA传输完成中断使能 */
	
	DMA_Cmd(DMA1_Stream0, ENABLE);																									/* DMA1_Stream0 使能 */
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
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_5);	//Set PA0 AS I2S2_WS
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_5);	//Set PA2 AS I2S2_CK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_5);	//Set PA3 AS I2S2_SDIN
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_5);	//Set PA4 AS I2S2_SDOUT
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
	RCC_HSEConfig(RCC_HSE_ON);															/* HSE使能  */		
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 	/* 等待HSE启动就绪完成  */
		
	RCC_PLLCmd(DISABLE);    																/* PLL关闭 */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);												/* 设置系统时钟分频 */
	RCC_PCLK1Config(RCC_HCLK_Div4);													/* 设置PCLK1时钟分频 */
	RCC_PCLK2Config(RCC_HCLK_Div2);													/* 设置PCLK2时钟分频 */
	
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);    					/* PLL时钟配置为168MHz  M N P Q */ 
		
	RCC_PLLI2SConfig(PLLI2SSource_PLLR);										/* 设置I2S时钟源 来自PLL.VCO */
	RCC_PLLRConfig(RCC_PLLCFGR_PLLR_7);											/* 设置I2S时钟源分频 PLL.VCO / PLLR = 336/7 = 48MHz */
		
	RCC_PLLCmd(ENABLE);   																	/* PLL使能 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; 	/* PLL倍频就绪完成 */  
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    					/* 系统时钟切换为PLL时钟源 */  
	while (RCC_GetSYSCLKSource() != 0x08){}    							/* 等待系统时钟切换完成 */ 
}


/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 外部中断优先级配置函数。
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
  NVIC_Init(&NVIC_InitStructure);							
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
