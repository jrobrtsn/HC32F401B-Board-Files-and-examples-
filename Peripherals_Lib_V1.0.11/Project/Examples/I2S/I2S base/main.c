/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(I2S)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片I2S2主机发送与I2S3从机接收.
*
*            适用芯片: HC32F401系列
*              实验步骤：
* 						 I2S2与I2S3的WS、CK、SDIN、SDOUT一一对应链接
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. I2S/中断初始化
*              4. I2S2填充左右声道数据后发送
*              5. 等待I2S3接收中断后读取
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


uint8_t i,j;

uint32_t tx_LeftBuff[8]  = {0x1122, 0x3344, 0x5566, 0x7788, 0x99AA, 0xBBCC, 0xDDEE, 0xAA55};
uint32_t tx_RightBuff[8] = {0x1112, 0x1314, 0x1516, 0x1718, 0x191A, 0x1B1C, 0x1D1E, 0x55AA};
uint32_t rx_LeftBuff[8],rx_RightBuff[8];


/*
*********************************************************************************************************
*	函 数 名: SPI2_I2S2_IRQHandler
*	功能说明: I2S2发送FIFO阈值中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI2_I2S2_IRQHandler(void)
{
	uint8_t n;	
	
	/* 依次向发送FIFO填充数据*/
	for(n = 0; n < 4; n++)
	{
		I2S_SendLeftData(I2S2, n);
		I2S_SendRightData(I2S2, n+4);
	}
}


/*
*********************************************************************************************************
*	函 数 名: SPI3_I2S3_IRQHandler
*	功能说明: I2S3接收FIFO阈值中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPI3_I2S3_IRQHandler(void)
{
	uint8_t m;	
	
	/* 依次向从FIFO读取数据*/
	for(m = 0; m < 4; m++)
	{
		rx_LeftBuff[m] = I2S_ReceiveLeftData(I2S3);
		rx_RightBuff[m] = I2S_ReceiveRightData(I2S3);
	}
	I2S_ClearFlag(I2S3, I2S_IT_RXFO);
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
	
	/* 系统时钟配置为168MHz */
	SYS_Configuration();
	
	/* GPIO配置*/
	GPIO_Configuration();
	
	/* I2S2 I2S3配置 */
	I2S_Configuration();
	
	/* 配置中断优先级使能 */
	NVIC_Configuration();
	
	/* 向I2S左右声道FIFO填充数据 */
	for(i = 0; i < 8; i++)
	{
		I2S_SendLeftData(I2S2, tx_LeftBuff[i]);
		I2S_SendRightData(I2S2, tx_RightBuff[i]);
	}
	
	/* I2S发送块使能 */
	I2S_BlockCmd(I2S2, I2S_BLOCK_TX, ENABLE);
	
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
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
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
	I2S_InitStructure.I2S_ClkPrescale			= 6;													/* I2S时钟6分频 48/6 = 8MHz */						
	I2S_Init(I2S2, &I2S_InitStructure);
	
	I2S_Threshold_Config(I2S2, I2S_FIFO_TX, I2S_FIFOThreshold_4);				/* 发送阈值为4 */
	I2S_ITConfig(I2S2, I2S_IT_TXFE, ENABLE);														/* 发送阈值触发中断 */
	
	I2S_Cmd(I2S2, ENABLE);																							/* I2S模块使能 */
	
	I2S_InitStructure.I2S_Mode 						= I2S_Mode_Slave;							/* I2S2为从模式 其他配置与主机保持一致 */
	I2S_Init(I2S3, &I2S_InitStructure);
	
	I2S_Threshold_Config(I2S3, I2S_FIFO_RX, I2S_FIFOThreshold_4);				/* 接收阈值为4 */
	I2S_ITConfig(I2S3, I2S_IT_RXDA, ENABLE);														/* 接收阈值触发中断 */
	
	I2S_Cmd(I2S3, ENABLE);																							/* I2S模块使能 */												
	
	I2S_BlockCmd(I2S3, I2S_BLOCK_RX, ENABLE);														/* I2S模块接收使能 */		
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

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;		
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_13);	//Set PB5 AS I2S3_WS
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_13);	//Set PB6 AS I2S3_CK
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_13);	//Set PB7 AS I2S3_SDIN
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_13);	//Set PB8 AS I2S3_SDOUT
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
*	功能说明: 中断优先级设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = SPI2_I2S2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
	
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_I2S3_IRQn;
  NVIC_Init(&NVIC_InitStructure);	
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
