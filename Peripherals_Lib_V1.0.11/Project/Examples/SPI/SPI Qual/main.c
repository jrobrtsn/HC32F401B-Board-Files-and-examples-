/*
********************************************************************************
*
*	模块名称：主程序模块（QSPI）
*	文件名称：main.c
*	说    明：
*							此样例演示了QSPI使用标准SPI模式传输，8位指令，24位地址长度
*							使用芯片：HC32F401系列
*							 	实验步骤：
*									1. 编译并下载程序;
*									2. 系统初始化与SPI初始化
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
void NVIC_Configuration(void);			/* 中断配置 */

/* 变量定义 -----------------------------------------------*/
uint8_t i,j,n,m;

uint8_t  tx_buff[8]  = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
//uint16_t tx_buff[8]  = {0x1121, 0x1222, 0x1323, 0x1424, 0x1525, 0x1626, 0x1727, 0x1828};
uint16_t rx_buff[8];

/*
********************************************************************************
*	函 数 名：SPI3_IRQHandler
*	功能说明：SPI3_IRQHandler中断服务函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void SPI1_IRQHandler(void)
{	
	/* 清SPI1中断 */	
	SPI_ClearFlag(SPI1);
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
	/* SPI配置 */
	SPI_Configuration();
	/* 中断配置 */
	NVIC_Configuration();
		
	SPI_SendByteFIFO(SPI1, 0x11);				//8b__Instruction
	SPI_SendWordFIFO(SPI1, 0x223344);		//24b_Address
	SPI_SendByteFIFO(SPI1, 0x15);				//8b__data
	SPI_SendByteFIFO(SPI1, 0x16);				//8b__data
	
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
	/* 使能PORTA/B/C/D时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);		
	/* 使能系统配置控制器时钟/SPI1时钟 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG | RCC_APB2Periph_SPI1, ENABLE);		
	
	/* SPI NSS引脚使能 */
	SYSCFG->AFIO_DBGR |= 1;	
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
	SPI_InitTypeDef 	SPI_InitStructure;
	QSPI_InitTypeDef  QSPI_InitStructure;
	
	/* QUAD SPI帧格式 */
	SPI_InitStructure.SPI_FrameFormat = SPI_FrameFormat_Quad;		
	/* National Microwire协议 */
	SPI_InitStructure.SPI_Protocol 		= SPI_Protocol_Motorola;	
	/* 空闲时，SCK保持低电平 */
	SPI_InitStructure.SPI_CPOL			= SPI_CPOL_Low;
	/* 从第一个时钟边沿开始采样数据 */
	SPI_InitStructure.SPI_CPHA			= SPI_CPHA_1Edge;
	/* 8位串行数据传输 */
	SPI_InitStructure.SPI_DataSize 		= SPI_DataSize_8b;
	/* 仅发送模式 */	
	SPI_InitStructure.SPI_Direction 	= SPI_Direction_TX;
	/* 波特率配置Fpclk/16*/
	SPI_InitStructure.SPI_BaudRatePrescaler = 16;
	/* 保存SPI1配置 */
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* 指令和地址均以标准SPI模式传输 */
	QSPI_InitStructure.QSPI_TransType  = QSPI_TransType_ISAS;
	/* 8位指令 */
	QSPI_InitStructure.QSPI_InstLength = QSPI_InstLength_8b;
	/* 24位地址宽度*/
	QSPI_InitStructure.QSPI_AddLength  = QSPI_AddLength_24b;
	/* 等待16个周期*/
	QSPI_InitStructure.QSPI_WaitCycles = 16;
	/* 保存QSPI配置 */
	QSPI_Init(&QSPI_InitStructure);
	
	/* SPI1发送FIFO阈值设置 */
	SPI_TXFIFOConfig(SPI1, 7);
	/* SPI1接收FIFO阈值设置 */	
	SPI_RXFIFOConfig(SPI1, 7);
	
	SPI1->CR2 |= (0X3 << 16);
				
	/* 使能SPI1 */
	SPI_Cmd(SPI1, ENABLE);
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

	/* Set SPI1 GPIO AF */	
	/* PORTB8复用SPI1_NSS */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,  GPIO_AF_9);
	/* PORTB13复用SPI1_SCK */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_9);
	/* PORTB9复用SPI1_IO0/MOSI */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_9);
	/* PORTB10复用SPI1_IO1/MISO */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_9);
	/* PORTB11复用SPI1_IO2/WP */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_9);
	/* PORTB12复用SPI1_IO3/HOLD */	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_9);
	
	/* Set SPI1 GPIO NSS SCK */
	/* 选择PIN8/13 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_13;
	/* 端口复用功能模式 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	/* 端口推挽输出 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	/* 端口上拉 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	/* 端口高速输出速度 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	/* 保存PORTB端口配置 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Set SPI1 GPIO IO0-3 */
	/* 选择PIN9/10/11/12 */
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	/* 端口无上拉或下拉 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	
	/* 保存PORTB端口配置 */	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
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
*             1. SPI1_IRQn      - SPI1全局中断
*           配置参数:
*             中断1(SPI1):
*               - 中断通道:      SPI1_IRQn
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

	/* 中断通道选择SPI1 */
  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	/* 中断抢占优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	/* 中断通道子优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	/* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
}
