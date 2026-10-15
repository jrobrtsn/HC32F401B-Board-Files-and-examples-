/*
*********************************************************************************************************
*
*	模块名称 : TIM1-TIM9定时器模块(PWM输出功能)
*	文件名称 : main.c
*	说    明 :
*            	此样例演示TIM1-TIM9输出中心对齐PWM功能
*            	适用芯片: HC32F401系列
*              		实验步骤：
*                	1. 编译下载程序；
*									2. 示波器测 PA2和PA3输出500HZ方波，呈现中心对齐输出。
*					
*	注意事项 :	
*				通过修改宏定义TIM_No_TEST可以切换不同TIM输出PWM
*				定时器时钟来源为APB1或者APB2，切换不用的TIM后，定时时间需要重新调整
*
*				当 APB1 分频系数 ≠ 1 时，定时器时钟 = APB1×2
*				当 APB2 分频系数 ≠ 1 时，定时器时钟 = APB2×2
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);	 /* 系统时钟配置函数 */
void RCC_Configuration(void);	 /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);	 /* GPIO引脚配置函数 */
void TIM_Configuration(void);	 /* TIM初始化配置函数 */
void NVIC_Configuration(void);	 /* 嵌套向量中断控制器(NVIC)配置函数 */

/* 全局变量 -----------------------------------------------*/


/*
*********************************************************************************************************
*	函 数 名: TIM_IRQHandler
*	功能说明: TIM中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_IRQHandler()
{	
	/* 检查捕获比较1中断标志 */
	if (TIM_GetITStatus(TIM_No, TIM_IT_CC1) != RESET)
	{
		/* 清除捕获比较1中断标志 */
		TIM_ClearITPendingBit(TIM_No, TIM_IT_CC1);
	}
	
	/* 检查捕获比较2中断标志 */
	if (TIM_GetITStatus(TIM_No, TIM_IT_CC2) != RESET)
	{
		/* 清除捕获比较2中断标志 */
		TIM_ClearITPendingBit(TIM_No, TIM_IT_CC2);
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
	
	/* 配置TIM */
	TIM_Configuration();
	
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
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD和DMA1的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);
	
	/* 使能TIM2-TIM27的APB1总线时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7, ENABLE);
	
	/* 使能TIM1、TIM8、TIM9的APB2总线时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_TIM9, ENABLE);
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
		
	GPIO_InitStructure.GPIO_Pin 	= TIM_CH1_Pin | TIM_CH2_Pin;	/* PWM输出通道配置 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;					/* 复用模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;				/* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;				/* 高速模式 */
	GPIO_Init(TIM_GPIOx, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(TIM_GPIOx, TIM_CH1_PinSource, TIM_CHx_PinAF);	/* IO复用配置 */
	GPIO_PinAFConfig(TIM_GPIOx, TIM_CH2_PinSource, TIM_CHx_PinAF);	/* IO复用配置 */
}


/*
*********************************************************************************************************
*	函 数 名: TIM_Configuration
*	功能说明: TIM初始化配置函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
	/* TIM2-TIM7时钟源为APB1，当 APB1分频系数 ≠ 1 时，定时器时钟自动×2 = 42M*2 = 84M */
	/* TIM1/TIM8/TIM9时钟源为APB2，当 APB2分频系数 ≠ 1 时，定时器时钟自动×2 = 84M*2 = 168M */
	
#if ((TIM_No_TEST == 2) || (TIM_No_TEST == 3) || (TIM_No_TEST == 4) || (TIM_No_TEST == 5) || (TIM_No_TEST == 6) || (TIM_No_TEST == 7))
	TIM_TimeBaseStructure.TIM_Prescaler	    = 0x1;							/* 预分频 PSC CK_CNT = fck / (psc[15:0] + 1) =84/(1+1)=42M */
#else /* (TIM_No_TEST == 1) || (TIM_No_TEST == 8) || (TIM_No_TEST == 9) */
	TIM_TimeBaseStructure.TIM_Prescaler	    = 0x3;							/* 预分频 PSC CK_CNT = fck / (psc[15:0] + 1) =168/(3+1)=42M */
#endif
	
	TIM_TimeBaseStructure.TIM_Period 		= 42000-1;						/* 自动重装载值: 42000/42000000*1000000=1000us即500HZ */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						/* 重复计数器: 0=单次 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 				/* 输入时钟分频: 不分频 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; /* 中心对齐模式1: 先增后减, 对称PWM */
	TIM_TimeBaseInit(TIM_No, &TIM_TimeBaseStructure); 
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;				/* PWM模式1: CNT<CCR时输出有效电平 */
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;		/* 主输出使能 */
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;			/* 有效电平=高(CNT<CCR时输出高) */
	TIM_OCInitStructure.TIM_OCIdleState		= TIM_OCIdleState_Reset;		/* 空闲状态(刹车后): 输出低电平 */
	#if ((TIM_No_TEST == 1) || (TIM_No_TEST == 8))
	TIM_OCInitStructure.TIM_OutputNState 	= TIM_OutputNState_Enable;		/* 互补输出使能 */	
	TIM_OCInitStructure.TIM_OCNPolarity 	= TIM_OCNPolarity_High;			/* 互补极性=高(与主输出同相, 由死区插入异相) */
	TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;		/* 互补空闲状态: 输出低电平 */
	#endif
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;								/* CCR1占空比值(由main.h定义) */			
	TIM_OC1Init(TIM_No, &TIM_OCInitStructure);								/* 初始化通道1 */
	TIM_OC1PreloadConfig(TIM_No, TIM_OCPreload_Enable);						/* 使能预装载 */		

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;								/* CCR2占空比值 */
	TIM_OC2Init(TIM_No, &TIM_OCInitStructure);								/* 初始化通道2 */
	TIM_OC2PreloadConfig(TIM_No, TIM_OCPreload_Enable);						/* 使能预装载 */
	
	TIM_ITConfig(TIM_No, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);					/* 使能TIM_IT_CC1、TIM_IT_CC2中断 */
	
	#if ((TIM_No_TEST == 1) || (TIM_No_TEST == 8))
	TIM_CtrlPWMOutputs(TIM_No, ENABLE);										/* MOE主输出使能 */
	#endif
	
	TIM_Cmd(TIM_No, ENABLE); 												/* 启动定时器 */
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
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* 配置TIM中断优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQn;						/* TIM中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 抢占优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				/* 子优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* 使能该中断通道 */
	NVIC_Init(&NVIC_InitStructure);	
}

