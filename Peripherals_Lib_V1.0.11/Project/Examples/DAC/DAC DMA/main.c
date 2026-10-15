/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(DAC DMA波形输出)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了DAC配合DMA和EXTI实现自动循环波形输出。
*            DMA以循环模式从内存数组TX_BUFF[]搬运数据到DAC数据寄存器，
*            每次DAC转换由EXTI9外部中断(PA9下降沿)触发。
*            工作流程:
*              EXTI9触发 → DAC转换一次 → DMA搬运下一个数据到DAC
*              → DMA搬运完4个数据后TC中断 → 自动循环回到数组开头
*            主循环中PC10引脚以Toggle方式指示程序运行状态。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 在PA9引脚输入下降沿触发信号
*                3. 用示波器观察PA4/PA8的DAC输出波形
*                4. 用示波器观察PC10的数字Toggle波形(运行指示)
*							注意事项：
*									1. TX_BUFF[4] = {0x10, 0x100, 0x200, 0x300}，对应输出电压约18.75mV, 300mV, 600mV, 900mV（Vref=1.2V）
*									2. DAC DMA触发源配置为EXTI9，即PA9下降沿触发DAC转换和DMA请求
*									3. DMA配置为循环模式(Circular)，4个数据搬运完成后自动回到TX_BUFF[0]重新开始
*									4. DMA通道为DMA1_Stream0_Channel2，数据宽度为字(32位)
*									5. 需通过SYSCFG->OUTSEL配置DAC输出引脚映射，并通过SYSCFG_EXTILineConfig映射PA9到EXTI Line9
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);     /* 时钟配置函数(含DAC/DMA/SYSCFG) */
void SYS_Configuration(void);     /* 系统时钟配置函数 */
void GPIO_Configuration(void);    /* GPIO配置函数(PA4模拟, PA8数字输出, PA9输入) */
void EXTI_Configuration(void);    /* 外部中断配置函数(PA9下降沿触发) */
void NVIC_Configuration(void);    /* 中断向量配置函数(EXTI9_5 + DMA1_Stream0) */
void DAC_Configuration(void);     /* DAC配置函数(EXTI9触发DMA) */
void DMA_Configuration(void);     /* DMA配置函数(内存→DAC, 循环模式) */

/* 全局变量 -----------------------------------------------*/
/* DMA源数据缓冲区: 4个12位DAC数据, 循环输出产生波形
 * 数据值: 0x010(16), 0x100(256), 0x200(512), 0x300(768)
 * 对应电压(参考1.2V): 18.75mV, 300mV, 600mV, 900mV  */
 
uint32_t TX_BUFF[4] = {0x10, 0x100, 0x200, 0x300};


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
}

/*
*********************************************************************************************************
*	函 数 名: EXTI9_5_IRQHandler
*	功能说明: 外部中断线9~5中断服务函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI9_5_IRQHandler()
{
  /* 检查是否为EXTI Line9中断 */
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
    /* 清除EXTI Line9中断挂起标志 */
    EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
/*
*********************************************************************************************************
*	函 数 名: delay
*	功能说明: 简易软件延时函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void delay(void)
{
	uint32_t i,j;

	/* 双层循环实现软件延时 */
	for( i = 0; i < 1000; i++)
		for( j = 0; j < 1000; j++);
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
	/* 使能外设时钟(GPIOA/B/C/D, DMA1, DAC, SYSCFG) */
	RCC_Configuration();

	/* 配置GPIO(PA4模拟输出, PC10数字输出, PA9数字输入) */
	GPIO_Configuration();

	/* 配置EXTI(PA9下降沿触发中断) */
	EXTI_Configuration();

	/* 配置DMA(内存→DAC, 循环模式, 4个字) */
	DMA_Configuration();

	/* 配置DAC(输出使能, 参考1.2V, DMA触发源=EXTI9) */
	DAC_Configuration();

	/* 配置NVIC(EXTI9_5中断 + DMA1_Stream0中断) */
	NVIC_Configuration();

	/* 主循环: PC10引脚Toggle作为运行状态指示 */
	while(1)
	{
			GPIO_ToggleBits(GPIOC, GPIO_Pin_10);  /* 翻转PC10电平, 指示程序运行中 */
			delay();  
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

	/* 使能DAC的APB1总线时钟 */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC, ENABLE);

	/* 使能SYSCFG的APB2总线时钟(用于EXTI引脚映射和DAC引脚映射) */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SYSCFG, ENABLE);

	/* 配置DAC输出引脚映射: OUTSEL=0 → DAC输出到PA4和PA8引脚 */
	SYSCFG->OUTSEL = 0;
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

	/* PA4和PA8为模拟模式(DAC输出)--根据寄存器进行选择 */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_4 |  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AN;           /* 模拟模式(DAC输出引脚) */
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;          /* 推挽输出(模拟模式下可忽略) */
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;       /* 无内部上下拉 */
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;        /* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PC10为数字推挽输出(运行状态指示灯) */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_OUT;          /* 数字输出模式 */
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;          /* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_UP;           /* 内部上拉 */
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;        /* 高速模式 */
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PA9为数字输入(EXTI外部触发输入引脚) */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_IN;           /* 数字输入模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 将PA9引脚连接到EXTI Line9(用于下降沿触发检测) */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);
}

/*
*********************************************************************************************************
*	函 数 名: DAC_Configuration
*	功能说明: DAC初始化配置函数(含DMA触发配置)。
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

	/* 配置DAC DMA触发源为EXTI9: 每次EXTI9下降沿触发一次DMA传输 */
	DAC_DMATriggerConfig(DAC_DMA_EXTI9);

	/* 使能DAC的DMA请求 */
	DAC_DMACmd(ENABLE);

	/* 使能DAC模块 */
	DAC_Cmd(ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA传输配置函数(内存→DAC)。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
  DMA_InitTypeDef  DMA_InitStructure;

  DMA_InitStructure.DMA_Channel = DMA_Channel_2;                                   /* DMA通道2(对应DAC外设请求) */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DR;                   /* 外设目标地址: DAC数据寄存器 */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TX_BUFF;                       /* 内存源地址: TX_BUFF数组 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                          /* 传输方向: 内存→外设 */
  DMA_InitStructure.DMA_BufferSize = (uint32_t)4;                                  /* 单次传输数量: 4个数据单元 */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                  /* 外设地址固定(始终写同一个DAC数据寄存器) */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                          /* 内存地址自增(依次读TX_BUFF[0]~[3]) */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;           /* 外设数据宽度: 32位(字) */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;                   /* 内存数据宽度: 32位(字) */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                  /* 循环模式: 自动回到起始地址 */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                              /* DMA优先级: 高 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                           /* FIFO禁用(直通模式) */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;                     /* FIFO阈值(FIFO禁用时无效) */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;                      /* 内存突发传输: 单次 */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;               /* 外设突发传输: 单次 */
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);                                      /* 初始化DMA1 Stream0 */

  /* 使能DMA传输完成(TC)中断 */
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

  /* 使能DMA1 Stream0, 等待外设DMA请求(由DAC在EXTI9触发后发出) */
  DMA_Cmd(DMA1_Stream0, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数(本示例未调用，可按需使用)。
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

	/*  将系统时钟源切换为PLL输出 */
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08){}  /* 等待系统时钟切换完成(0x08表示PLL已为时钟源) */
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

  /* 配置EXTI Line9: 下降沿触发中断 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;              /* EXTI线9(对应PA9引脚) */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;      /* 中断模式 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  /* 下降沿触发 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                /* 使能该EXTI线 */
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

  /* 配置EXTI9_5中断优先级(PA9下降沿触发) */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;                /* EXTI9_5中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;      /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;             /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);

  /* 配置DMA1_Stream0中断优先级(传输完成) */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;           /* DMA1 Stream0中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;      /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;             /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                   /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
