/*
*********************************************************************************************************
*
*	模块名称 : RTC模块(计时功能)
*	文件名称 : main.c
*	说    明 :
*            	此样例演示RTC计时功能，需外接32.768KHZ晶振(PC14和PC15)，每秒产生一次中断。
*            	适用芯片: HC32F401系列
*              		实验步骤：
*                	1. 编译下载程序；
*									2. 示波器测 PA9每秒翻转一次。
*					
*	注意事项 :	
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"


void RCC_Configuration(void);		/* 外设时钟使能配置函数 */
void SYS_Configuration(void);		/* 系统时钟配置函数 */
void GPIO_Configuration(void);		/* GPIO引脚配置函数 */
void RTC_Configuration(void);		/* RTC时钟配置函数 */
void NVIC_Configuration(void);		/* 嵌套向量中断控制器(NVIC)配置函数 */


/*
*********************************************************************************************************
*	函 数 名: RTC_WKUP_IRQHandler
*	功能说明: RTC中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_WKUP_IRQHandler(void)
{
	/* 检查秒中断标志 */
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{		
		RTC_ClearITPendingBit(RTC_IT_SEC);			/* 清除秒中断标志 */
		RTC_WaitForLastTask();						/* 等待RTC寄存器上的最后一次写操作完成 */

		GPIO_ToggleBits(GPIOA, GPIO_Pin_9);			/* 翻转PA9 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 程序主入口函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
	/* 使能外设时钟 */
	RCC_Configuration();	
	
	/* 配置系统时钟 */
	SYS_Configuration();
	
	/* 配置GPIO */
	GPIO_Configuration();

	/* 配置RTC */
	RTC_Configuration();
	
	/* 配置NVIC(TIM中断优先级) */
	NVIC_Configuration();	
	
	while(1)
	{


		
	}
}

/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: RTC_Configuration
*	功能说明: RTC时钟配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_Configuration(void)
{
	RCC_LSEConfig(RCC_LSE_ON);									/* 使能外部低速时钟 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){};		/* 等待LSE起振稳定 */
		
	RTC->CRH = (0x1 << 3) | (0x1 << 5);							/* LSE振荡器作为RTC时钟, RTC时钟使能 */
		
	RTC_WaitForSynchro();										/* 等待RTC寄存器APB1时钟同步 */
	RTC_WaitForLastTask();										/* 等待RTC寄存器上的最后一次写操作完成 */
		
	RTC_ITConfig(RTC_IT_SEC, ENABLE);							/* 使能秒中断 */
		
	RTC_SetPrescaler(32768-1);									/* RTC预分频值 fTR_CLK = fRTCCLK/(PRL[19:0]+1) = 32768/(32767+1) = 1 */
	RTC_WaitForLastTask();										/* 等待RTC寄存器上的最后一次写操作完成 */
		
	RTC_SetCounter(0);											/* 设置RTC计数器值 */
	RTC_WaitForLastTask();										/* 等待RTC寄存器上的最后一次写操作完成 */

	RTC_ExitConfigMode();										/* 退出RTC配置模式 */
		
	RCC_ClearFlag();											/* 清除RMVF复位标志 */
}

/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO引脚配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					/* PA9 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				/* 输出模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			/* 无内部上下拉 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;			/* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
	/* 开启外部高速晶振HSE(8MHz) */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* 等待HSE起振稳定 */
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 
		
	/* 关闭PLL，设置AHB和APB总线时钟分频 */
	RCC_PLLCmd(DISABLE);    
	RCC_HCLKConfig(RCC_SYSCLK_Div1);	/* AHB总线时钟(HCLK) = SYSCLK / 1 */
	RCC_PCLK1Config(RCC_HCLK_Div4); 	/* APB1总线时钟(PCLK1) = HCLK / 4 */
	RCC_PCLK2Config(RCC_HCLK_Div2);	 	/* APB2总线时钟(PCLK2) = HCLK / 2 */
	
	/* 配置PLL: PLLCLK = HSE(8MHz) / M(8) × N(336) / P(2) = 168MHz */
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    /* PLL时钟配置: 时钟源=HSE, M=8, N=336, P=2, Q=8 */
		
	/* 使能PLL并等待锁定 */
	RCC_PLLCmd(ENABLE);   
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};    
			
	/* 将系统时钟源切换为PLL输出(168MHz) */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   
	while (RCC_GetSYSCLKSource() != 0x08){} 
}

/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 配置RTC中断优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;						/* RTC中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 抢占优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				/* 子优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* 使能该中断通道 */
	NVIC_Init(&NVIC_InitStructure);
}

