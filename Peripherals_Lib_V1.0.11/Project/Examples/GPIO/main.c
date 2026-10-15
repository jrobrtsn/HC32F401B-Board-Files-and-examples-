/*
*********************************************************************************************************
*
*	模块名称 : GPIO模块(GPIO输出和时钟输出功能)
*	文件名称 : main.c
*	说    明 :
*            	此样例演示GPIO输出和时钟输出功能。
*				  
*            	适用芯片: HC32F401系列
*              		实验步骤：
*                	1. 编译下载程序；
*									2. 示波器测PA8输出2MHZ方波、PC10翻转。
*					
*	注意事项 :	
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);	 /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);	 /* GPIO引脚配置函数 */
void MCO_Configuration(void);	 /* 时钟输出配置函数 */
void SYS_Configuration(void);	 /* 系统时钟配置函数 */

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
	
	/* 配置MCO */
	MCO_Configuration();
	
	while(1)
	{
		/* PC10翻转 */
		GPIO_ToggleBits(GPIOC, GPIO_Pin_10);
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
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO引脚配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					/* PC10 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				/* 输出模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				/* 内部上拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;			/* 快速模式 */
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: MCO_Configuration
*	功能说明: 时钟输出配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MCO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					/* PA8 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				/* 复用模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			/* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;			/* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,  GPIO_AF_0);		/* IO复用配置 */
	
	RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_4);			/* 外部高速晶振(8MHZ)4分频后输出到PA8 */
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
