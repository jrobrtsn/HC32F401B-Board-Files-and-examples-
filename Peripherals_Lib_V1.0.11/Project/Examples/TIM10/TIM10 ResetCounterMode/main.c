/*
*********************************************************************************************************
*
*	模块名称 : TIM10定时器模块(内部触发和外部触发复位计数器功能)
*	文件名称 : main.c
*	说    明 :
*            	此样例演示TIM10内部触发和外部触发复位计数器功能
*
*	内部触发复位计数器: 当ExternalResetSoure=0时，TIM1作为触发信号(125us)，当TIM1触发中断时就会重置TIM10.CNT从0开始计数，
*				       PA9、PA8每125us翻转一次。
*	外部触发复位计数器: 当ExternalResetSoure=1时，外部ETR(PC2)引脚作为触发源，外部给PC2高电平则会重置TIM10.CNT从0开始计数,
*				       PA8每500us翻转一次。
*				  
*            	适用芯片: HC32F401系列
*              		实验步骤：
*                	1. 编译下载程序；
*					2. 示波器测PA8、PA9。
*					
*	注意事项 :	
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
void TIM_Configuration(void);	 /* TIM1初始化配置 */
void BTIM_Configuration(void);	 /* TIM10初始化配置 */
void NVIC_Configuration(void);	 /* 嵌套向量中断控制器(NVIC)配置函数 */

/* 全局变量 -----------------------------------------------*/


/* 宏定义   -----------------------------------------------*/
#define  ExternalResetSoure 		0   //1 - ETR PIN		0 - TIM1_TRGO

/*
*********************************************************************************************************
*	函 数 名: TIM10_IRQHandler
*	功能说明: TIM10中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM10_IRQHandler()
{	
	/* 检查更新中断标志 */
	if (BTIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)
	{
		/* 清除更新中断标志 */
		BTIM_ClearITPendingBit(TIM10, TIM_IT_Update);
		
		/* PA8翻转 */
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
	}
}


#if (ExternalResetSoure == 0)
/*
*********************************************************************************************************
*	函 数 名: TIM1_UP_IRQHandler
*	功能说明: TIM1中断服务函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM1_UP_IRQHandler()
{	
	/* 检查更新中断标志 */
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		/* 清除更新中断标志 */
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 
		
		/* PA9翻转 */
		GPIO_ToggleBits(GPIOA, GPIO_Pin_9);
	}
}
#endif


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
	
	#if (ExternalResetSoure == 0)	
	/* 配置TIM1 */
	TIM_Configuration();
	#endif
	
	/* 配置TIM10 */
	BTIM_Configuration();
	
	/* 配置NVIC(TIM中断优先级) */
	NVIC_Configuration();
	
	while(1)
	{
		#if (ExternalResetSoure == 1)
		{
			GPIO_ToggleBits(GPIOA, GPIO_Pin_9);
		}
		#endif
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
	
	/* 使能TIM1和TIM10的APB2总线时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM10, ENABLE);
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
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8 | GPIO_Pin_9;		/* PA8 PA9 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;				/* 输出模式 */
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;				/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 			/* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed   = GPIO_High_Speed;				/* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_2;					/* PC2 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN;				/* 内部下拉 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;					/* 复用模式 */
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_3);			/* IO复用配置 */
}

/*
*********************************************************************************************************
*	函 数 名: BTIM_Configuration
*	功能说明: TIM10初始化配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BTIM_Configuration(void)
{
	BTIM_TimeBaseInitTypeDef  BTIM_TimeBaseStructure;
	
	/* 时钟分频，当 APB2分频系数 ≠ 1 时，定时器时钟自动×2 = 84M*2 = 168M */
	BTIM_TimeBaseStructure.BTIM_Prescaler = 0x1;										/* 预分频寄存器 PSC CK_CNT = fck / (psc[15:0] + 1) =168/(1+1)=84M */
	BTIM_TimeBaseStructure.BTIM_Period 		= 42000-1;									/* 定时时间=1000us/(84000/42000)=500us */
	BTIM_TimeBaseInit(TIM10, &BTIM_TimeBaseStructure);

	#if ExternalResetSoure
		BTIM_ETRResetConfig(TIM10, BTIM_ExtTRGPSC_OFF, BTIM_ExtRSTPolarity_NonInverted);/* ETRFLT:0 无滤波，计数值复位信号正相，上升沿有效 */
	#else
		BTIM_SelectInputResetConfig(TIM10, BTIM_TS_ITR7, BTIM_ExtRSTPolarity_Inverted); /* 触发输入信号(TRGI)来源TIM1_Trgo，计数值复位信号反相，下降沿有效 */
	#endif
	
	BTIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);										/* 使能TIM中断 */
	
	BTIM_Cmd(TIM10, ENABLE); 															/* 启动定时器 */
}

/*
*********************************************************************************************************
*	函 数 名: TIM_Configuration
*	功能说明: TIM1初始化配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* 时钟分频，当 APB2分频系数 ≠ 1 时，定时器时钟自动×2 = 84M*2 = 168M */
	TIM_TimeBaseStructure.TIM_Prescaler = 0x1;								/* 预分频寄存器 PSC CK_CNT = fck / (psc[15:0] + 1) =168/(1+1)=84M */
	TIM_TimeBaseStructure.TIM_Period 		= 10500-1;						/* 定时时间=1000us/(84000/10500)=125us */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						/* 重复计数器值:0  在TIM1和TIM8才有 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 				/* 死区及采样时钟分频:1分频 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;			/* 递减计数 */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	
	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);					/* 选择更新事件作为触发输出(TRGO) */
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);								/* 使能TIM中断 */
	
	TIM_Cmd(TIM1, ENABLE); 													/* 启动定时器 */
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
	NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;					/* TIM10中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 抢占优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				/* 子优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						/* 使能该中断通道 */
	NVIC_Init(&NVIC_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;					/* TIM1中断通道 */
	NVIC_Init(&NVIC_InitStructure);
}

