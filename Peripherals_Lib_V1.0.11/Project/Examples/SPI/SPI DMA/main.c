/*
********************************************************************************
*
*	模块名称：主程序模块（SPI_DMA配置）
*	文件名称：main.c
*	说    明：
*							此样例演示了SPI_DMA配置，全双工模式下
*							使用芯片：HC32F401系列
*							 	实验步骤：
*									1. 编译并下载程序;
*									2. 系统初始化与SPI初始化及DMA初始化
*
*注意事项:
*
*
*
*	Copyright (C), 2009~2026, 芯圣电子  www.holychip.cn
*
********************************************************************************
*/

#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);				/* 系统配置 */
void RCC_Configuration(void);				/* 时钟配置 */
void GPIO_Configuration(void);			/* GPIO配置 */
void SPI_Configuration(void);				/* SPI配置 */
void DMA_Configuration(void);				/* DMA配置 */
void NVIC_Configuration(void);			/* 中断配置 */

/* 变量定义 -----------------------------------------------*/
uint16_t i,j;

uint8_t tx_buff[16] = {	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 
												0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16};
uint8_t rx_buff[8];

/*
********************************************************************************
*	函 数 名：SPI2_IRQHandler
*	功能说明：SPI2_IRQHandler断服务函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/												
void SPI2_I2S2_IRQHandler(void)
{	
	/* 清SPI2中断 */
	SPI_ClearFlag(SPI2);
}

/*
********************************************************************************
*	函 数 名：DMA1_Stream0_IRQHandler
*	功能说明：DMA1_Stream0_IRQHandler断服务函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void DMA1_Stream0_IRQHandler(void)
{
	/* 清DMA流0全局中断,数据流0上无传输完成事件 */
	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);		
}

/*
********************************************************************************
*	函 数 名：main
*	功能说明：程序主入口函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
int main(void)
{ 
	/* 时钟配置 */
	RCC_Configuration();
	/* 系统配置 */
	SYS_Configuration();
	/* GPIO配置 */
	GPIO_Configuration();
	/* DMA配置 */
	DMA_Configuration();
	/* SPI配置 */
	SPI_Configuration();
	/* 中断配置 */
	NVIC_Configuration();
			
	while(1)
	{
		i++;
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
	/* 使能PORTA/B/C/D时钟，使能DMA1时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);		
	/* 使能SPI2_I2S2/SPI3_I2S3时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2_I2S2 | RCC_APB1Periph_SPI3_I2S3, ENABLE);		
}

/*
*********************************************************************************************************
*	函 数 名: SPI_Configuration
*	功能说明: SPI配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	
	/* 主机模式 */
	SPI_InitStructure.SPI_mode			= SPI_Mode_Master;
	/* National Microwire协议 */
	SPI_InitStructure.SPI_Protocol	= SPI_Protocol_Motorola;
	/* 空闲时，SCK保持低电平 */	
	SPI_InitStructure.SPI_CPOL			= SPI_CPOL_Low;
	/* 从第一个时钟边沿开始采样数据 */
	SPI_InitStructure.SPI_CPHA			= SPI_CPHA_1Edge;
	/* 8位串行数据传输 */
	SPI_InitStructure.SPI_DataSize 	= SPI_DataSize_8b;
	/* 全双工传输模式 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_FullDuplex;
	/* 波特率配置Fpclk/16*/
	SPI_InitStructure.SPI_BaudRatePrescaler = 16;
	/* 保存SPI2配置 */
	SPI_Init(SPI2, &SPI_InitStructure);	
	
	/* SPI2发送FIFO阈值设置0 */ 
	SPI_TXFIFOConfig(SPI2, 0);
	/* SPI2发送DMA数据级别设置0 */
	SPI_TXDMAConfig(SPI2, 0);
	/* SPI2使能发送DMA通道 */ 
	SPI_DMACmd(SPI2, SPI_DMAReq_Tx, ENABLE);
	/* 使能SPI2 */
	SPI_Cmd(SPI2, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA_Configuration配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;

	/* SPI2_TX映射DMA通道11 */ 
  DMA_InitStructure.DMA_Channel = DMA_Channel_11;
	/* SPI2数据寄存器清0 */ 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;	
  
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_buff;
  /* SPI2的DMA */
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  
	DMA_InitStructure.DMA_BufferSize = (uint32_t)16;
  /* 外设递增模式 */ 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* 存储器递增模式 */ 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* 外设数据8位 */ 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
  /* 储存器数据8位 */ 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* 禁止循环模式 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  /* 优先级高 */
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* 使能直接模式 */
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  /* FIFO阈值满值 */
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  /* 存储器单次传输 */
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  /* 外设单次传输 */
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  /* 保存DMA流0配置 */
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);
  
  /* DMA流0传输完成中断 */
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	/* 使能DMA流0全局中断 */
	DMA_Cmd(DMA1_Stream0, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO引脚多功能配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Set SPI2 GPIO NSS SCK MOSI */
	/* 选择PIN0/2/4 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4;	
	/* 端口复用功能模式 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	/* 端口推挽输出 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* 端口上拉 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	/* 端口高速输出速度 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	/* 保存PORTA端口配置 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Set SPI2 GPIO MIS0 */
	/* 选择PIN3 */
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_3;
	/* 端口无上拉或下拉 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	/* 保存PORTA端口配置 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* PORTA0复用SPI2_NSS */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_5);
	/* PORTA2复用SPI2_SCK */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_5);
	/* PORTA3复用SPI2_MISO */		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_5);
	/* PORTA4复用SPI2_MOSI */		
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_5);
}

/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数(本示例未调用，可按需使用)。
*           配置流程如下:
*             1. 开启外部高速晶振(HSE, 8MHz)
*             2. 等待HSE起振稳定
*             3. 关闭PLL，配置各总线分频系数
*                - HCLK  = SYSCLK / 1
*                - PCLK1 = HCLK / 4
*                - PCLK2 = HCLK / 2
*             4. 配置PLL: HSE(8MHz) / 8(M) × 336(N) / 2(P) = 168MHz
*             5. 使能PLL并等待锁定
*             6. 切换系统时钟源为PLL输出(168MHz)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
	/* HSE震荡器开启 */
	RCC_HSEConfig(RCC_HSE_ON);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 
	
	/* PLL关闭 */
	RCC_PLLCmd(DISABLE);
	/* 系统时钟不分频 */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	/* AHB1时钟4分频 */
	RCC_PCLK1Config(RCC_HCLK_Div4);
	/* AHB2时钟2分频 */ 	
	RCC_PCLK2Config(RCC_HCLK_Div2);	
	/* PLL时钟配置168,PLLM,PLLN,PLLP,PLLQ */ 
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);
		
	/* PLL开启 */ 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};
	
	/* 选择PLL做为系统时钟 */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08){}
}

/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*           本示例配置了1个中断通道:
*             1. DMA1_Stream0_IRQn      - DMA流0全局中断
*           配置参数:
*             中断1(DMA1_Stream0):
*               - 中断通道:      DMA1_Stream0_IRQn
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

	/* 中断通道选择DMA流0全局中断 */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	/* 中断抢占优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	/* 中断通道子优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	/* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
}
