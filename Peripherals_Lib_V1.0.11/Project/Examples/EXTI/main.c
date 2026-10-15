/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(外部中断EXTI)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了外部中断(EXTI)的基本使用。
*            PA10配置为EXTI Line10下降沿触发输入引脚(内部上拉)，
*            当PA10检测到下降沿时进入EXTI15_10_IRQHandler中断服务函数，
*            在中断中翻转PA9引脚电平作为响应指示。
*            主循环中PA8引脚持续翻转，作为程序运行状态指示灯。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 将PA10引脚接地(产生下降沿)或接按键
*                3. 观察PA9的电平翻转(每次下降沿翻转一次)
*                4. 观察PA8的Toggle波形(运行指示)
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);     /* 时钟配置函数(GPIO + SYSCFG) */
void SYS_Configuration(void); 		/* 系统时钟配置函数 */
void GPIO_Configuration(void);    /* GPIO配置函数(PA8/PA9输出, PA10输入) */
void EXTI_Configuration(void);    /* 外部中断配置函数(PA10下降沿) */
void NVIC_Configuration(void);    /* 中断向量配置函数(EXTI15_10) */


/*
*********************************************************************************************************
*	函 数 名: EXTI15_10_IRQHandler
*	功能说明: 外部中断线15~10中断服务函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI15_10_IRQHandler()
{
    /* 清除EXTI Line10中断挂起标志(不清除会反复进入中断) */
    EXTI_ClearITPendingBit(EXTI_Line10);

    /* 翻转PA9引脚电平，作为中断响应的可视指示 */
    GPIO_ToggleBits(GPIOA, GPIO_Pin_9);
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
	/* 使能外设时钟(GPIOA/B/C/D + SYSCFG) */
	RCC_Configuration();
	/* 配置系统时钟(HSE + PLL = 168MHz) */
	SYS_Configuration();
	/* 配置GPIO(PA8输出-运行灯, PA9输出-响应灯, PA10输入-触发脚) */
	GPIO_Configuration();

	/* 配置EXTI(PA10对应Line10, 下降沿触发中断) */
	EXTI_Configuration();

	/* 配置NVIC(EXTI15_10中断优先级) */
	NVIC_Configuration();

	/* 主循环: PA8持续翻转, 指示程序正在运行 */
	while(1)
	{
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);  /* 翻转PA8, 运行状态指示灯 */
	}
}
/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数。
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
/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

	/* 使能SYSCFG的APB2总线时钟(EXTI引脚映射必须) */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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

	/* PA8和PA9为数字推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;   /* PA8: 运行灯, PA9: 响应灯 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             /* 数字输出模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            /* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;          /* 无内部上下拉 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;          /* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PA10为数字输入(EXTI下降沿触发引脚, 内部上拉) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                /* PA10: 外部触发输入引脚 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              /* 数字输入模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            /* 推挽输出(输入模式下可忽略) */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;              /* 内部上拉: 默认高, 接地→下降沿 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;          /* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 将PA10引脚映射到EXTI Line10(用于外部中断检测) */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);
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

  /* 配置EXTI Line10: 下降沿触发中断 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;              /* EXTI线10(对应PA10引脚) */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;       /* 中断模式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   /* 下降沿触发(高电平→低电平) */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                 /* 使能该EXTI线 */
  EXTI_Init(&EXTI_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* 配置EXTI15_10中断优先级(Line10~Line15共享此中断通道) */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                /* EXTI15_10中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
