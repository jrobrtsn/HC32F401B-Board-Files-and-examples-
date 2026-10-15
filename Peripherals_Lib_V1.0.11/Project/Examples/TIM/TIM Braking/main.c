/*
*********************************************************************************************************
*
*	模块名称 : 通用定时器模块(TIM刹车功能)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了通用定时器(TIM)的刹车(Braking)功能。
*            TIM配置为3对互补PWM输出(CH1/CH1N、CH2/CH2N、CH3/CH3N)，(PC5/PC6、PC7/PC8 、PC9/PC10),
*            同时使能刹车输入和自动输出控制。
*            当刹车引脚(BK1-PB11)检测到有效电平时，TIM自动关闭所有PWM输出，
*            并触发刹车中断(Break)，实现电机控制中的紧急停止保护。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 用示波器观察CH1/CH1N/CH2/CH2N/CH3/CH3N的PWM互补输出波形
*                3. 给刹车引脚BK1施加有效电平，观察PWM输出是否被关断
*                4. 在刹车中断中设置断点，确认中断正常触发
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     /* 系统时钟配置函数 */
void RCC_Configuration(void);     /* 外设时钟使能配置函数(含SYSCFG) */
void GPIO_Configuration(void);    /* GPIO引脚配置函数(6路PWM + 刹车输入) */
void TIM_Configuration(void);     /* TIM刹车配置函数(互补PWM+死区+刹车) */
void NVIC_Configuration(void);    /* 中断控制器配置函数(TIM刹车中断) */


/*
*********************************************************************************************************
*	函 数 名: TIM_IRQHandler
*	功能说明: TIM中断服务函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_IRQHandler()
{
	/* 检查TIM刹车中断(Break)标志 */
	if (TIM_GetITStatus(TIM_No, TIM_IT_Break) != RESET)
	{
		TIM_ClearITPendingBit(TIM_No, TIM_IT_Break);  /* 清除刹车中断挂起位 */
		/* 用户可在此添加刹车事件的处理代码(如报警、状态记录等) */
	}
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 主程序入口函数
*	形    参：无
*	返 回 值: 无返回(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	/* 使能外设时钟(GPIOA~D, DMA1, TIM1~TIM9, SYSCFG) */
	RCC_Configuration();

	/* 配置系统时钟(HSE + PLL = 168MHz) */
	SYS_Configuration();

	/* 配置GPIO(3对互补PWM引脚 + 刹车BK1引脚复用功能) */
	GPIO_Configuration();

	/* 配置TIM(中心对齐PWM1模式 + 互补输出 + 死区 + 刹车) */
	TIM_Configuration();

	/* 配置NVIC(TIM刹车中断优先级) */
	NVIC_Configuration();

	/* 主循环: 硬件自动运行，刹车事件由中断处理 */
	while(1)
	{

	}
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
	/* 使能GPIOA~GPIOD及DMA1的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

	/* 使能TIM2~TIM7的APB1总线时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7, ENABLE);

	/* 使能TIM1/TIM8/TIM9及SYSCFG的APB2总线时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_TIM9 | RCC_APB2Periph_SYSCFG, ENABLE);
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

	/*--- 配置CH1/CH2/CH3主PWM输出引脚 PC5 PC7 PC9---*/
	GPIO_InitStructure.GPIO_Pin 	= TIM_CH1_Pin | TIM_CH2_Pin | TIM_CH3_Pin;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;           					/* 复用功能模式(PWM输出由TIM外设控制) */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          					/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;      				 	/* 无内部上下拉 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;        					/* 高速模式 */
	GPIO_Init(TIM_GPIOx, &GPIO_InitStructure);

	/* 映射CH1/CH2/CH3到TIM复用功能 */
	GPIO_PinAFConfig(TIM_GPIOx, TIM_CH1_PinSource, TIM_CHx_PinAF);
	GPIO_PinAFConfig(TIM_GPIOx, TIM_CH2_PinSource, TIM_CHx_PinAF);
	GPIO_PinAFConfig(TIM_GPIOx, TIM_CH3_PinSource, TIM_CHx_PinAF);

	/*--- 配置CH1N/CH2N/CH3N互补PWM输出引脚 PC6 PC8 PC10---*/
	GPIO_InitStructure.GPIO_Pin 	= TIM_CH1N_Pin | TIM_CH2N_Pin | TIM_CH3N_Pin;
	GPIO_Init(TIM_GPIOx, &GPIO_InitStructure);

	/* 映射CH1N/CH2N/CH3N到TIM复用功能(可能在不同GPIO组) */
	GPIO_PinAFConfig(TIM_GPIOy, TIM_CH1N_PinSource, TIM_CHx_PinAF);
	GPIO_PinAFConfig(TIM_GPIOy, TIM_CH2N_PinSource, TIM_CHx_PinAF);
	GPIO_PinAFConfig(TIM_GPIOy, TIM_CH3N_PinSource, TIM_CHx_PinAF);

	/*--- 配置BK1刹车输入引脚  PB11---*/
	GPIO_InitStructure.GPIO_Pin 	= TIM_BK1_Pin;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;           /* 内部上拉: 默认高电平(未刹车), 外部拉低=刹车 */
	GPIO_Init(TIM_GPIOz, &GPIO_InitStructure);

	/* 映射BK1到TIM刹车复用功能 */
	GPIO_PinAFConfig(TIM_GPIOz, TIM_BK1_PinSource, TIM_CHx_PinAF);
}


/*
*********************************************************************************************************
*	函 数 名: TIM_Configuration
*	功能说明: TIM刹车与互补PWM初始化配置函数。
*            配置TIM为中心对齐PWM1模式，3对互补输出通道，
*            使能刹车功能并配置死区时间和自动输出控制。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	TIM_BDTRInitTypeDef			TIM_BDTRInitStructure;

	/*--- 时基配置 ---*/
	/* TIM1/TIM8时钟源为APB2，当 APB2分频系数 ≠ 1 时，定时器时钟自动×2 = 84M*2 = 168M */
	TIM_TimeBaseStructure.TIM_Prescaler = 0x1;                   			/* 预分频 PSC CK_CNT = fck / (psc[15:0] + 1) =168/(1+1)=84M */
	TIM_TimeBaseStructure.TIM_Period 		= 42000-1;              	 	/* 自动重装载值: 42000/84000000*1000000=500us即1KHZ */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;             			/* 重复计数器: 0=单次 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      			/* 输入时钟分频: 不分频 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; /* 中心对齐模式1: 先增后减, 对称PWM */
	TIM_TimeBaseInit(TIM_No, &TIM_TimeBaseStructure);

	/*--- CH1和CH1N: PWM1互补输出配置 ---*/
	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;          	/* PWM模式1: CNT<CCR时输出有效电平 */
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;       /* 主输出使能 */
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;          /* 有效电平=高(CNT<CCR时输出高) */
	TIM_OCInitStructure.TIM_OCIdleState		= TIM_OCIdleState_Reset;        /* 空闲状态(刹车后): 输出低电平 */
	TIM_OCInitStructure.TIM_OutputNState	= TIM_OutputNState_Enable;      /* 互补输出使能 */
	TIM_OCInitStructure.TIM_OCNPolarity		= TIM_OCNPolarity_High;         /* 互补极性=高(与主输出同相, 由死区插入异相) */
	TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;       /* 互补空闲状态: 输出低电平 */

	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;                          		/* CCR1占空比值(由main.h定义) */
	TIM_OC1Init(TIM_No, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM_No, TIM_OCPreload_Enable);               		/* 使能CCR1预装载(更新事件时生效) */

	/*--- CH2和CH2N: PWM1互补输出配置 ---*/
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;                          		/* CCR2占空比值 */
	TIM_OC2Init(TIM_No, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM_No, TIM_OCPreload_Enable);

	/*--- CH3和CH3N: PWM1互补输出配置 ---*/
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;                         		 /* CCR3占空比值 */
	TIM_OC3Init(TIM_No, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM_No, TIM_OCPreload_Enable);

	/*--- 刹车与死区时间配置(BDTR寄存器) ---*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;        		 /* OSSR: 运行模式下关断状态使能 */
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;       		 /* OSSI: 空闲模式下关断状态使能 */
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;          		 /* LOCK: 不锁定, 允许软件修改BDTR */
	TIM_BDTRInitStructure.TIM_DeadTime = 0x8F;                         		 /* 死区时间: 0x8F(插入到互补通道切换之间, 防止桥臂直通) */
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;                		 /* 刹车功能: 使能(BK1引脚可触发刹车) */
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;   		 /* 刹车极性: 低电平有效(BK1拉低=刹车事件) */
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;  /* AOE: 使能(刹车解除后自动恢复PWM输出) */
	TIM_BDTRConfig(TIM_No, &TIM_BDTRInitStructure);

	/* 使能刹车(Break)中断 */
	TIM_ITConfig(TIM_No, TIM_IT_Break, ENABLE);

	/* 使能PWM主输出(必须调用, 否则PWM引脚无输出) */
	TIM_CtrlPWMOutputs(TIM_No, ENABLE);

	/* 使能TIM计数器, 开始输出PWM */
	TIM_Cmd(TIM_No, ENABLE);
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

	/* 配置PLL: PLLCLK = HSE(8MHz) / 8(M) × 336(N) / 2(P) = 168MHz */
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    /* PLL时钟配置: 时钟源=HSE, M=8, N=336, P=2, Q=8 */

	/* 使能PLL并等待锁定 */
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};

	/* 将系统时钟源切换为PLL输出(168MHz) */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08){}  /* 等待系统时钟切换完成(0x08表示PLL已为时钟源) */
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

  /* 配置TIM中断优先级(刹车中断) */
  NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQn;                      /* TIM中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);
}

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
