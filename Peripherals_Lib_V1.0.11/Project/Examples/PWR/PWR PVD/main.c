/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(PWR)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片对VDD的电压检测中断(PVD)
*            适用芯片: HC32F401系列
*              实验步骤：
*              	1. 编译并下载程序；
*              	2. 系统初始化与中断初始化
*              	3. PVD档位配置与使能。
*              	4. 降低或升高VDD电压，过PVD阈值。
*              	5. 芯片进入PVD中断，翻转IO。
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
void GPIO_Configuration(void);		/* GPIO配置函数 */
void EXTI_Configuration(void);		/* EXTI中断配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */


/*
*********************************************************************************************************
*	函 数 名: PVD_IRQHandler
*	功能说明: PVD电压检测中断
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PVD_IRQHandler()
{
    EXTI_ClearITPendingBit(EXTI_Line16);		/* 清除外部中断标志位16 */
		GPIO_ToggleBits(GPIOA, GPIO_Pin_9);
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 程序主入口函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{ 
	/* 使能外设时钟(GPIOA/B/C/D) */
	RCC_Configuration();	
	
	/* 配置GPIO( PA8/9为数字输出) */
	GPIO_Configuration();
	
	/* 配置外部中断16 即PVD中断 */
	EXTI_Configuration();
	NVIC_Configuration();
	
	/* PVD中断标志位清零 */
  EXTI_ClearITPendingBit(EXTI_Line16);	
	
	/* PVD档位设置和使能 */
	PWR_PVDLevelConfig(PWR_PVDLevel_3);	
	PWR_PVDCmd(ENABLE);	
	
	while(1)
	{
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
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
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;						/* 数字输出 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;							/* 内部上拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;					/* 高速输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
}


/*
*********************************************************************************************************
*	函 数 名: EXTI_Configuration
*	功能说明: 外部中断配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;							/* 中断请求 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  /* 上升沿和下降沿触发 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;												/* 中断使能 */
  EXTI_Init(&EXTI_InitStructure);																	/* 中断初始化 */
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

  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;									
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/

