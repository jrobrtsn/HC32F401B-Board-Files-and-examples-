/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(ADC信号采样)
*	文件名称 : main.c
*	说    明 :
*            此样例演示了ADC的序列1对3个通道(AN0/AN1/AN2)进行循环切换采样，
*            并将ADC转换结果换算为电压值(mV)。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 将ADC输入通道连接到被测信号源
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);     /* 时钟配置函数 */
void SYS_Configuration(void);     /* 系统时钟配置函数 */
void GPIO_Configuration(void);    /* GPIO配置函数 */
void NVIC_Configuration(void);    /* 中断向量配置函数 */
void ADC_Configuration(void);     /* ADC配置函数 */

/* 全局变量 -----------------------------------------------*/
uint32_t 	Flag = 0;            /* ADC转换完成标志位: 0-未完成, 1-已完成 */
uint32_t 	ADC_DAT[4];          /* ADC原始转换结果缓冲区(11位分辨率, 范围0~2047) */
uint32_t 	ADC_Value[4];        /* ADC电压换算值缓冲区(单位: mV), 实际只有前3个通道使用 */


/*
*********************************************************************************************************
*	函 数 名: ADC_IRQHandler
*	功能说明: ADC中断服务函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_IRQHandler()
{
	/* 读取ADC1序列1三个通道的原始转换结果(11位分辨率，范围0~2047) */
	ADC_DAT[0] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel0);  /* 通道0: AN0 */
	ADC_DAT[1] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel1);  /* 通道1: AN1 */
	ADC_DAT[2] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel2);  /* 通道2: AN2 */

	/* 将原始ADC值换算为电压值(mV): ADC_Value = ADC_DAT × 330(mV) / 2048 */
	ADC_Value[0] = ADC_DAT[0]*330/2048;   /* AN0通道电压值(mV) */
	ADC_Value[1] = ADC_DAT[1]*330/2048;   /* AN1通道电压值(mV) */
	ADC_Value[2] = ADC_DAT[2]*330/2048;   /* AN2通道电压值(mV) */

	/* 清除ADC1序列1转换结束中断标志 */
	ADC_ClearFlag(ADC1, ADC_FLAG_EOS1);

	/* 置位标志，通知主循环转换已完成 */
	Flag = 1;
}

/*
*********************************************************************************************************
*	函 数 名: delay
*	功能说明: 简易软件延时函数。
*           通过双层循环实现短延时，用于ADC两次采样之间的时间间隔。
*           延时时间取决于系统时钟频率，在168MHz主频下约延时若干毫秒。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void delay(void)
{
	uint32_t i,j;

	/* 双层循环实现软件延时 */
	for( i = 0; i < 200; i++)
		for( j = 0; j < 200; j++);
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
	/* 使能外设时钟(GPIOA/B/C/D, ADC1/2) */
	RCC_Configuration();

	/* 配置系统时钟(HSE + PLL = 168MHz) */
	SYS_Configuration();

	/* 配置GPIO(PA8/PA9/PA10为模拟输入) */
	GPIO_Configuration();

	/* 配置ADC1(序列1, 三通道, 软件触发) */
	ADC_Configuration();

	/*  配置NVIC(ADC中断优先级) */
	NVIC_Configuration();

	/* 进入主程序循环体 */
	while(1)
	{
		if(Flag)                      /* 等待ADC转换完成 */
		{
			Flag = 0;                  /* 清除转换完成标志 */

			delay();                   /* 延时等待，确保ADC稳定 */
			ADC_SoftwareStartConv(ADC1); /* 软件触发启动下一次ADC转换 */
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

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
*	功能说明: ADC初始化配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStruct;

  /* ADC基础参数配置 */
  ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div12;                              /* ADC时钟12分频 */
	ADC_InitStruct.ADC_Range  = ADC_Range_VDDA;                            /* 参考电压为VDDA(3.3V) */
	ADC_InitStruct.ADC_OverSamplingMode 		= ADC_OverSampling_Disable;    /* 禁用过采样 */
  ADC_InitStruct.ADC_OverSamplingTrigger 	= ADC_OverSamplingTrigger_Once;/* 过采样触发模式(禁用时无效) */
	ADC_InitStruct.ADC_OverSamplingRate 		= ADC_2X_OverSamplingRate;     /* 过采样率 */
	ADC_InitStruct.ADC_OverSamplingOffset 	= ADC_OverSamplingOffset_0;    /* 过采样偏移 */
  ADC_Init(ADC1, &ADC_InitStruct);

	/* 配置ADC1序列1(SQR1): 软件触发, 使能Channel0~Channel2共3个通道 */
	ADC_SQRInit(ADC1, ADC_SQR1, ADC_SQRxTRIG_Software, ADC_Ens0to2);

	/* 配置序列1各通道对应的输入引脚和采样时间 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_AN0, ADC_SampTime7Clk);   /* 通道0: AN0, 7个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel1, ADC_InputChannel_AN1, ADC_SampTime9Clk);   /* 通道1: AN1, 9个采样周期 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel2, ADC_InputChannel_AN2, ADC_SampTime12Clk);  /* 通道2: AN2, 12个采样周期 */

	/* 使能ADC1序列1转换结束(EOS1)中断 */
	ADC_ITConfig(ADC1, ADC_IT_EOS1, ENABLE);

  /* 使能ADC1模块 */
  ADC_Cmd(ADC1,ENABLE);

	/* 软件触发启动首次ADC转换 */
	ADC_SoftwareStartConv(ADC1);
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
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* 配置ADC中断优先级 */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;                       /* ADC全局中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;         /* 抢占优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;                /* 子优先级: 0x0F(最低) */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                      /* 使能该中断通道 */
  NVIC_Init(&NVIC_InitStructure);
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
