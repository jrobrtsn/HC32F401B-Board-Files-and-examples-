/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(ADC DMA信号采样)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了ADC配合DMA进行多序列多通道自动采样传输。
* 					
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序
*              2. 系统初始化
*              3. ADC初始化 				其中序列0触发源为TIM9 CC1 序列1触发源为TIM9 CC2
*              4. DMA与TIM9初始化 	其中TIM9 CC1/CC2通道输出PWM波，上升沿依次触发ADC 序列0/1启动转换
*              5. 等待DMA转换完成中断置起标志位，在主循环中计算全部通道电压值
*
*
*********************************************************************************************************
*	注意事项： 
*	
*							禁止4个序列的触发源为同一触发源同时开启转换

*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);     /* 时钟配置函数(含DMA时钟) */
void SYS_Configuration(void);     /* 系统时钟配置函数 */
void GPIO_Configuration(void);    /* GPIO配置函数 */
void NVIC_Configuration(void);    /* 中断向量配置函数(DMA中断) */
void ADC_Configuration(void);     /* ADC配置函数(含DMA使能) */
void DMA_Configuration(void);     /* DMA传输配置函数 */
void TIM_Configuration(void);     /* TIM9配置函数 */


/* 全局变量 -----------------------------------------------*/
uint32_t 	i = 0;   
uint16_t 	Flag;								 /* ADC序列0和序列1均转换完成标志位: 0-未完成, 1-已完成 */		
uint16_t 	ADC_DAT[5];          /* ADC原始转换结果缓冲区(11位分辨率, 范围0~2047) */
uint16_t 	ADC_Value[5];        /* ADC电压换算值缓冲区(单位: mV), 实际只有前3个通道使用 */



/*
*********************************************************************************************************
*	函 数 名: DMA1_Stream0_IRQHandler
*	功能说明: DMA1 Stream0中断服务函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA1_Stream0_IRQHandler()
{
	/* 清除DMA1 Stream0传输完成(TC)中断标志 */
	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
	
	Flag= 1;
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
	/* 使能外设时钟(GPIOA/B/C/D, DMA1, ADC1/2) */
	SYS_Configuration();
	RCC_Configuration();

	/* 配置GPIO(PA8/PA9/PA10 PC6/7为模拟输入) */
	GPIO_Configuration();

	/* 配置DMA1_Stream0(外设→内存, 循环模式, 传输3个半字) */
	DMA_Configuration();

	/* 配置ADC1(序列0/1, 三通道, DMA循环传输使能) */
	ADC_Configuration();
	
	/* 配置TIM9 CH0/2通道比较输出 上升沿分别触发ADC序列0/1 */
	TIM_Configuration();

	/* 配置NVIC(DMA1_Stream0中断优先级) */
	NVIC_Configuration();

	/* 主循环空闲，采样由DMA中断驱动自动完成 */
	while(1)
	{
		if(Flag)                      /* 等待ADC转换完成 */
		{
			/* 将原始ADC值换算为电压值(mV): ADC_Value = ADC_DAT × 330(mV) / 2048 */
			for(i=0; i<5; i++)
				ADC_Value[i] = ADC_DAT[i]*330/2048;   /* AN0通道电压值(mV) */
			
			Flag = 0;                  /* 清除转换完成标志 */
		}
		
	}
}

/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数。
*           使能GPIOA~GPIOD的AHB1总线时钟、DMA1的AHB1总线时钟，
*           以及ADC1、ADC2的APB2总线时钟。
*           注意: DMA1挂在AHB1总线上，使用前必须使能其时钟。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD以及DMA1的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

	/* 使能ADC1、ADC2的APB2总线时钟 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);
	
	/* 使能TIM9的APB2总线时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);	
	
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

	/* 配置PA8、PA9、PA10为模拟输入引脚，对应ADC通道AN0/AN1/AN2 */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AN;           /* 模拟输入模式 */
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;          /* 推挽输出(模拟模式下此配置可忽略) */
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;       /* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;        /* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* 配置PC6、PC7模拟输入引脚，对应ADC通道AN28 AN29*/	
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/*
*********************************************************************************************************
*	函 数 名: ADC_Configuration
*	功能说明: ADC初始化配置函数(含DMA使能)。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStruct;

  /* ADC基础参数配置 */
  ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div12;                               /* ADC时钟12分频 */
	ADC_InitStruct.ADC_Range  = ADC_Range_VDDA;                              /* 参考电压为VDDA(3.3V) */
	ADC_InitStruct.ADC_OverSamplingMode 		= ADC_OverSampling_Disable;      /* 禁用过采样 */
  ADC_InitStruct.ADC_OverSamplingTrigger 	= ADC_OverSamplingTrigger_Once;  /* 过采样触发模式(禁用时无效) */
	ADC_InitStruct.ADC_OverSamplingRate 		= ADC_2X_OverSamplingRate;       /* 过采样率(禁用时无效) */
	ADC_InitStruct.ADC_OverSamplingOffset 	= ADC_OverSamplingOffset_0;      /* 过采样偏移(禁用时无效) */
  ADC_Init(ADC1, &ADC_InitStruct);

	/* 配置ADC1序列0(SQR0): TIM9 CC1触发, 使能Channel0/1 2个通道 */
	ADC_SQRInit(ADC1, ADC_SQR0, ADC_SQRxTRIG_TIM9CC1, ADC_Ens0to1);

	/* 配置序列1各通道对应的输入引脚和采样时间 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR0, ADC_Channel0, ADC_InputChannel_AN0, ADC_SampTime7Clk);   /* 通道0: AN0, 7个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR0, ADC_Channel1, ADC_InputChannel_AN1, ADC_SampTime9Clk);   /* 通道1: AN1, 9个采样周期 */
	
	/* 配置ADC1序列1(SQR1): TIM9 CC2触发, 使能Channel0/1/2 3个通道 */
	ADC_SQRInit(ADC1, ADC_SQR1, ADC_SQRxTRIG_TIM9CC2, ADC_Ens0to2);
	
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_AN2, ADC_SampTime7Clk); 	/* 通道0: AN0, 7个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel1, ADC_InputChannel_AN28, ADC_SampTime9Clk); /* 通道1: AN0, 9个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel2, ADC_InputChannel_AN29, ADC_SampTime9Clk); /* 通道2: AN0, 9个采样周期 */
	
	/* 配置ADC DMA: 循环模式使能(每轮转换完成自动触发DMA), 溢出检测禁用 */
	ADC_DMAConfig(ADC1, ADC_DMALoop_Enable, ADC_DMAOverflow_Disable);

	/* 使能ADC的DMA请求，每次EOS1转换结束后触发DMA搬运数据 */
	ADC_DMACmd(ADC1, ENABLE);

  /* 使能ADC1模块 */
  ADC_Cmd(ADC1,ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA传输配置函数。
*           工作流程:
*             ADC序列1完成 → DMA搬运3×16bit → TC中断 → 重新开始
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;

  DMA_InitStructure.DMA_Channel = DMA_Channel_0;                                   /* DMA通道0(对应ADC1外设请求) */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->ADMADR;              /* 外设源地址: ADC1数据寄存器 */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_DAT;                       /* 内存目标地址: ADC_DAT数组 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                          /* 传输方向: 外设→内存 */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)5;                                  /* 单次传输数量: 5个数据单元 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  /* 外设地址固定(始终读同一个ADC结果寄存器) */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                          /* 内存地址自增(依次写入ADC_DAT[0],[1],[2]) */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;       /* 外设数据宽度: 16位(半字) */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;               /* 内存数据宽度: 16位(半字) */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                  /* 循环模式: 写满后自动回到数组起始地址 */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                              /* DMA优先级: 高 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                           /* FIFO禁用(直通模式, 外设收到数据立即写出) */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;                     /* FIFO阈值(FIFO禁用时无效) */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                      /* 内存突发传输: 单次(每次传输1个数据) */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;               /* 外设突发传输: 单次 */
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);                                      /* 初始化DMA1 Stream0 */

  /* 使能DMA传输完成(TC)中断 */
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

	/* 使能DMA1 Stream0, 开始等待外设DMA请求 */
	DMA_Cmd(DMA1_Stream0, ENABLE);
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
	
	/* TIM9时钟源为APB2，当 APB2分频系数 ≠ 1 时，定时器时钟自动×2 = 84M*2 = 168M */
	TIM_TimeBaseStructure.TIM_Prescaler	    = 0x3;									/* 预分频 PSC CK_CNT = fck / (psc[15:0] + 1) =168/(3+1)=42M */
	TIM_TimeBaseStructure.TIM_Period 		= 42000-1;									/* 自动重装载值: 42000/42000000*1000000=1000us即500HZ */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;								/* 重复计数器: 0=单次 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 				/* 输入时钟分频: 不分频 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; 
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); 
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;						/* PWM模式1: CNT<CCR时输出有效电平 */
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;		/* 主输出使能 */
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;			/* 有效电平=高(CNT<CCR时输出高) */
	TIM_OCInitStructure.TIM_OCIdleState		= TIM_OCIdleState_Reset;		/* 空闲状态(刹车后): 输出低电平 */
	TIM_OCInitStructure.TIM_Pulse = 21000;														/* CCR1占空比值	8 */	
	TIM_OC1Init(TIM9, &TIM_OCInitStructure);													/* 初始化通道1 */
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);									/* 使能预装载 */		

	/* PWM1 Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_Pulse = 10500;														/* CCR2占空比值 */
	TIM_OC2Init(TIM9, &TIM_OCInitStructure);													/* 初始化通道2 */
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);									/* 使能预装载 */
		
	TIM_Cmd(TIM9, ENABLE); 												/* 启动定时器 */
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

  /* 配置DMA1_Stream0中断优先级 */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;                /* DMA1 Stream0中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;           /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;                  /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
