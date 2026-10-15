/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(USART1)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片USART1连续接收.
*
*            适用芯片: HC32F401系列
*              实验步骤：
*							 PC端->串口调试助手->USB转串口->MCU PA2/PA3(TX/RX)
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. USART/中断初始化
*              4. 等待PC端串口调试助手发送8个字节
*              5. USART接收完成中断
*	
*	注意事项：
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


uint8_t 	i;
uint8_t 	RX_BUFF[8];


/*
*********************************************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: 串口1接收中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void USART1_IRQHandler()
{	
	/* 等待串口1接收完成 */	
	if(USART_GetITStatus(USART1, USART_FLAG_RCONTE) == SET)
	{
		/* 清除接受完成标志位 */
		USART_ClearFlag(USART1, USART_FLAG_RCONTE);
		
		/* 获取接受数据 */
		for(i=0; i<8; i++)
			RX_BUFF[i] = USART_ReceiveData(USART1, i);	

		/* 清除连续接收标识位 */
		USART_ClearRCNT(USART1);
	}
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
	
	USART_RCNT_Config(USART1, 8);																	/* 连续接收次数为8 */
	
	USART_ITConfig(USART1, USART_IT_RC, ENABLE);									/* 接收完成中断使能 */
	
	USART_Cmd(USART1, USART_Mode_Rx | USART_Mode_Tx, ENABLE);			/* 接收发送使能 */
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

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
