/*
*********************************************************************************************************
*
*	模块名称 : 通用定时器模块(TIM输入捕获)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了通用定时器(TIM)的输入捕获(Input Capture)功能。
*            TIM的CC1和CC2通道分别捕获外部输入信号的边沿时刻，
*            在中断中读取捕获值存入capture_timer[]数组。
*            主循环中PA0持续翻转作为运行指示。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 将PA4信号接入TIM_CH1(PA2)和TIM_CH2(PA3)对应的捕获引脚。
*                3. 在仿真模式下中观察capture_timer[0]/[1]的捕获值变化，capture_period为周期，capture_high_time为高电平时间。
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     			/* 系统时钟配置函数(HSE+PLL→168MHz) */
void RCC_Configuration(void);     			/* 外设时钟使能配置函数 */
void GPIO_Configuration(void);    			/* GPIO引脚配置函数(TIM_CH1/CH2复用功能, PA0输出) */
void TIM_Configuration(void);     			/* TIM输入捕获配置函数(CC1/CC2双通道) */
void NVIC_Configuration(void);    			/* 中断控制器配置函数(TIM中断) */
void TIM_PWMOUTPUT_Config(void);			/* TIM4_CH1 PWM输出配置(PA4, 100kHz, 50%占空比) */


/* 全局变量 -----------------------------------------------*/
uint16_t capture_timer[2];        			/* 输入捕获值缓存: [0]=CC1捕获值, [1]=CC2捕获值 */

volatile uint16_t capture_period    = 0; 	/* PWM 周期 */
volatile uint16_t capture_high_time = 0; 	/* PWM 高电平时间 */
volatile uint8_t  capture_flag      = 0; 	/* 完成标志*/

/*
*********************************************************************************************************
*	函 数 名: TIM_IRQHandler
*	功能说明: TIM中断服务函数。
*            当TIM的CC1或CC2通道捕获到输入边沿时触发中断，
*            分别读取CC1和CC2的捕获值存入capture_timer[]数组。
*            捕获值反映从计数器启动到捕获事件发生之间的时钟周期数，
*            可用于测量输入信号的频率、占空比或脉冲宽度。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_IRQHandler()
{
    uint16_t current_cc1;

    /* 检查CC1捕获事件: 上升沿触发, 读取CC1捕获值 */
    if (TIM_GetITStatus(TIM_No, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM_No, TIM_IT_CC1 );        					/* 清除CC1中断挂起位 */
        current_cc1  = TIM_GetCapture1(TIM_No);         					/* 读取CC1捕获寄存器值 */
        capture_period   = current_cc1 - capture_timer[0];					/* 算周期：当前上升沿 - 上一次上升沿 (capture_timer[0]) */
        capture_timer[0] = current_cc1;										/* 更新上一次上升沿时刻为当前值 */
        capture_flag = 1; 													/* 标记一个完整的周期结算完成 */
    }

	/* 检查CC2捕获事件: 下降沿触发, 读取CC2捕获值 */
	if (TIM_GetITStatus(TIM_No, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM_No, TIM_IT_CC2);         					/* 清除CC2中断挂起位 */
		capture_timer[1] = (uint16_t)TIM_GetCapture2(TIM_No);				/* 读取 CC2 下降沿时刻存入 capture_timer[1] */
		capture_high_time = capture_timer[1] - capture_timer[0];			/* 算高电平时间：下降沿时刻 - 刚过去的上升沿时刻 */
		capture_flag = 1; 
	}
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 主程序入口函数。
*	形    参：无
*	返 回 值: 无返回(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    /* 使能外设时钟(GPIOA/B/C/D, DMA1, TIM1~TIM9) */
    RCC_Configuration();

    /* 配置系统时钟(HSE + PLL = 168MHz) */
    SYS_Configuration();

    /* 配置GPIO(TIM_CH1/CH2复用功能, PA0推挽输出) */
    GPIO_Configuration();

	/* TIM4定时器产生PWM配置 */
    TIM_PWMOUTPUT_Config();
	
    /* 配置TIM(向上计数模式, CC1上升沿捕获/CC2下降沿捕获, 滤波器1) */
    TIM_Configuration();

    /* 配置NVIC(TIM中断优先级) */
    NVIC_Configuration();

    /* 主程序循环: PA0持续翻转 */
    while(1)
    {

        /*
        *  高  +------+     +------+     +------
        *      |      |     |      |     |
        *  低  +      +-----+      +-----+
        *      ^      ^     ^      ^
        *     CC1    CC2   CC1    CC2
        *      ^      ^
        *      |--高--|     |--高--|
        *      |<-- 周期 -->| 
        */
        GPIO_ToggleBits(GPIOA,  GPIO_Pin_0);  /* 翻转PA0 提示 */

        /* 检查是否有新数据 */
        if (capture_flag == 1)
        {
            capture_flag = 0;
        }
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

    /* 使能TIM2~TIM7的APB1总线时钟(基本定时器和通用定时器) */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM5 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7, ENABLE);

    /* 使能TIM1/TIM8/TIM9的APB2总线时钟(高级定时器) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_TIM8 | RCC_APB2Periph_TIM9, ENABLE);
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

    /* 配置TIM_CH1和TIM_CH2引脚为复用功能模式(输入捕获)PA2 PA3   输出PWM PA4*/
    GPIO_InitStructure.GPIO_Pin 	= TIM_CH1_Pin | TIM_CH2_Pin | GENERAL_OCPWM_PIN;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;           								/* 复用功能模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          								/* 推挽输出(复用模式由外设控制) */
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;      						 		/* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;        								/* 高速模式 */
    GPIO_Init(TIM_GPIOx, &GPIO_InitStructure);

    GPIO_PinAFConfig(TIM_GPIOx, TIM_CH1_PinSource, TIM_CHx_PinAF);							/* 将CH1引脚映射到TIM的复用功能通道 */
    GPIO_PinAFConfig(TIM_GPIOx, TIM_CH2_PinSource, TIM_CHx_PinAF);							/* 将CH2引脚映射到TIM的复用功能通道 */
    GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT, GENERAL_OCPWM_PINSOURCE, GENERAL_OCPWM_AF);	/* 将PA5引脚映射到TIM的复用功能通道 */

    /* 配置PA0为推挽输出-PA0 */
    GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;											/* PA0 */
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;          								/* 输出模式 */
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;          								/* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;       								/* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;       	 								/* 高速模式 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*
*********************************************************************************************************
*	函 数 名: TIM_PWMOUTPUT_Config
*	功能说明: 通用定时器产生 PWM 配置，TIM4_CH1 PA4
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_PWMOUTPUT_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* TIM2-TIM7时钟源为APB1，当 APB1分频系数 ≠ 1 时，定时器时钟自动×2 = 42M*2 = 84M */
	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;						/* 预分频 PSC CK_CNT = fck / (psc[15:0] + 1) =84/(83+1)=1M */
    TIM_TimeBaseStructure.TIM_Period = 10 - 1;							/* 自动重装载值: 周期=10/1000000*1000000=10us即100KHZ */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				/* 输入时钟分频: 不分频 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			/* 递增计数*/
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);						/* 初始化定时器TIM4 */

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;					/* PWM模式1: CNT<CCR时输出有效电平 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		/* 主输出使能 */

    TIM_OCInitStructure.TIM_Pulse = 5;									/* 占空比50% */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			/* 有效电平=高(CNT<CCR时输出高) */
    TIM_OCInitStructure.TIM_OCIdleState	= TIM_OCIdleState_Reset;		/* 空闲状态: 输出低电平 */

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);							/* 初始化通道1 */
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);					/* 使能预装载 */		

    TIM_Cmd(TIM4, ENABLE);												/* 启动定时器 */
}

/*
*********************************************************************************************************
*	函 数 名: TIM_Configuration
*	功能说明: TIM输入捕获初始化配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
	
    /*--- 时基配置 ---*/
    TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;              		/* 预分频---168mhz /((预分频值 + 1))    */
    TIM_TimeBaseStructure.TIM_Period 	= 0xFFFF;                		/* 自动重装载值: 决定计数周期*/
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            		/* 重复计数器: 0=单次计数 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     		/* 输入时钟分频: 不分频 */
    //TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1; /* 中心对齐模式1: 先增后减 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM_No, &TIM_TimeBaseStructure);

    /*--- CC1通道: 输入捕获配置 ---*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;             		/* 选择通道1 */
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  		/* 捕获极性: 上升沿(CK_CNT→CCR1) */
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	/* 输入选择: 直接映射TI1 */
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;        		/* 输入预分频: 不分频(每个边沿都捕获) */
    TIM_ICInitStructure.TIM_ICFilter = 0x1;                      		/* 输入滤波器: 1(滤除短于1个采样周期的毛刺) */
    TIM_ICInit(TIM_No, &TIM_ICInitStructure);

    /*--- CC2通道: 下降沿捕获(极性不同于CC1), 其余参数同CC1 ---*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;             		/* 选择通道2 */
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;  		/* 捕获极性: 下降沿 */
    TIM_ICInit(TIM_No, &TIM_ICInitStructure);

    /* 使能CC1和CC2捕获中断 */
    TIM_ITConfig(TIM_No, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    /* 使能TIM计数器, 开始计数 */
    TIM_Cmd(TIM_No, ENABLE);
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
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* 配置TIM中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQn;                      /* TIM中断通道(IRQn在main.h中根据TIM_No映射) */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
