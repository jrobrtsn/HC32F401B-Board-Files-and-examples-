/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(ADC DMA信号采样)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了ADC配合DMA进行多通道自动采样传输。
*            ADC序列1每完成一次三通道(AN0/AN1/AN2)转换后，由DMA自动
*            将转换结果搬运到内存数组ADC_DAT[]中，无需CPU干预。
*            DMA传输完成(3个半字)后触发中断，在中断中重新启动ADC转换，
*            形成"ADC转换→DMA搬运→DMA中断→重启ADC"的循环采样流水线。
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序；
*              2. 系统初始化、GPIO初始化、DMA初始化、ADC初始化
*              3. ADC的序列1 依次采集通道 AN0(PA8) 、AN1(PA9) 、AN2(PA10) 的电压值
*              4. 每完成一个通道转换，DMA自动将12位ADC数据搬运到ADC_DAT[]数组
*              5. DMA完成3个数据搬运后触发传输完成中断(TC)
*              6. 在DMA中断中清除标志位，并再次软件触发ADC，形成循环采集
*
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

/* 全局变量 -----------------------------------------------*/
/* ADC DMA目标缓冲区: 3通道 × 半字(16位) = 6字节, DMA循环填充 */
uint16_t 	ADC_DAT[4] = {0, 0, 0, 0};  /* 索引0~2分别对应Channel0~2, 索引3保留 */


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

	/* 重新启动ADC软件转换，开始下一轮采样 */
	ADC_SoftwareStartConv(ADC1);
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

	/* 配置GPIO(PA8/PA9/PA10为模拟输入) */
	GPIO_Configuration();

	/* 配置DMA1_Stream0(外设→内存, 循环模式, 传输3个半字) */
	DMA_Configuration();

	/* 配置ADC1(序列1, 三通道, DMA循环传输使能) */
	ADC_Configuration();

	/* 配置NVIC(DMA1_Stream0中断优先级) */
	NVIC_Configuration();

	/* 主循环空闲，采样由DMA中断驱动自动完成 */
	while(1)
	{

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

	/* 配置ADC1序列1(SQR1): 软件触发, 使能Channel0~Channel2共3个通道 */
	ADC_SQRInit(ADC1, ADC_SQR1, ADC_SQRxTRIG_Software, ADC_Ens0to2);

	/* 配置序列1各通道对应的输入引脚和采样时间 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_AN0, ADC_SampTime7Clk);   /* 通道0: AN0, 7个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel1, ADC_InputChannel_AN1, ADC_SampTime9Clk);   /* 通道1: AN1, 9个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel2, ADC_InputChannel_AN2, ADC_SampTime12Clk);  /* 通道2: AN2, 12个采样周期 */

	/* 配置ADC DMA: 循环模式使能(每轮转换完成自动触发DMA), 溢出检测禁用 */
	ADC_DMAConfig(ADC1, ADC_DMALoop_Enable, ADC_DMAOverflow_Disable);

	/* 使能ADC的DMA请求，每次EOS1转换结束后触发DMA搬运数据 */
	ADC_DMACmd(ADC1, ENABLE);

  /* 使能ADC1模块 */
  ADC_Cmd(ADC1,ENABLE);

	/* 软件触发启动首次ADC转换，后续由DMA中断循环触发 */
	ADC_SoftwareStartConv(ADC1);
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
  DMA_InitStructure.DMA_BufferSize = (uint32_t)3;                                  /* 单次传输数量: 3个数据单元 */
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
