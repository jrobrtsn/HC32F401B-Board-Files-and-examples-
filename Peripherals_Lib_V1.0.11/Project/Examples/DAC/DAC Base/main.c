/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(DAC基础输出)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了DAC的基础使用：输出固定的模拟电压值。
*            DAC输出配置为PA4/PA8引脚，参考电压为内部1.2V基准，
*            输出数字量0x200(512)，对应输出电压约:
*              512 / 1024 × 1.2V = 0.6V
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 用万用表或示波器测量PA4引脚输出电压
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);     /* 时钟配置函数(含DAC/SYSCFG时钟) */
void SYS_Configuration(void);     /* 系统时钟配置函数 */
void GPIO_Configuration(void);    /* GPIO配置函数(PA4/PA8模拟输出) */
void DAC_Configuration(void);     /* DAC配置函数 */


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
	/* 使能外设时钟(GPIOA/B/C/D, DMA1, DAC, SYSCFG) */
	SYS_Configuration();
	RCC_Configuration();

	/* 配置GPIO(PA4/PA8为模拟输出) */
	GPIO_Configuration();

	/* 配置DAC(输出使能, 参考电压1.2V) */
	DAC_Configuration();

	/* 设置DAC输出数据 = 0x200(512), 输出电压 = 512/1024 × 1.2V = 0.6V  */
	DAC_SetDACData(0X200);

	/* 主循环空闲，DAC持续输出设置的电压值 */
	while(1)
	{

	}
}

/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能及DAC输出引脚配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD以及DMA1的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

	/* 使能DAC的APB1总线时钟(DAC挂在APB1总线上) */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE);

	/* 使能SYSCFG的APB2总线时钟(用于配置DAC输出引脚映射) */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);

	/* 配置DAC输出引脚映射: OUTSEL=0 → DAC输出到PA4  1→ PA8 */
	SYSCFG->OUTSEL = 0;
}

/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO引脚配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 配置PA4和PA8为模拟模式(DAC输出引脚) */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_4 |  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AN;           /* 模拟模式(DAC输出) */
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;          /* 推挽输出(模拟模式下此配置可忽略) */
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;       /* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;        /* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: DAC_Configuration
*	功能说明: DAC初始化配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DAC_Configuration(void)
{
	DAC_InitTypeDef DAC_InitStructure;

	/* 配置DAC输出使能 */
	DAC_InitStructure.DAC_out = DAC_Out_Enable;       /* 使能DAC输出 */
	/* 配置DAC参考电压为内部1.2V基准 */
	DAC_InitStructure.DAC_Ref = DAC_Ref1P2;            /* 参考电压: 内部1.2V */
	DAC_Init(&DAC_InitStructure);

	/* 使能DAC模块，开始输出模拟电压 */
	DAC_Cmd(ENABLE);
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

	/* 关闭PLL，配置AHB及APB总线时钟分频 */
	RCC_PLLCmd(DISABLE);
	RCC_HCLKConfig(RCC_SYSCLK_Div1);     /* AHB总线时钟(HCLK) = SYSCLK / 1 */
	RCC_PCLK1Config(RCC_HCLK_Div4);      /* APB1总线时钟(PCLK1) = HCLK / 4 */
	RCC_PCLK2Config(RCC_HCLK_Div2);      /* APB2总线时钟(PCLK2) = HCLK / 2 */

	/* 配置PLL: PLLCLK = HSE(8MHz) / M(8) × N(336) / P(2) = 168MHz */
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    /* PLL时钟配置: 时钟源=HSE, M=8, N=336, P=2, Q=8 */

	/* 使能PLL并等待锁定 */
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};

	/* 将系统时钟源切换为PLL输出 */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08){}  /* 等待系统时钟切换完成(0x08表示PLL已为时钟源) */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
