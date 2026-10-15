/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(FDA)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片ADC采集FDA功能.
*
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序；
*              2. 系统初始化，GPIO，ADC中断初始化
*              3. FDA初始化与使能
*              4. ADC初始化与使能
*	
*	注意事项：
*
* 通过修改main.h 宏定义 TSET_FDA 选择FDA0/1
*	0 - FDA0
*     FDA0_INP0引脚为PA1  	FDA0_INN0引脚为PA2
*     FDA0_OUTP引脚为PB3  	FDA0_OUTN引脚为PB4
*	1 - FDA1
*     FDA1_INP0引脚为PB7  	FDA1_INN0引脚为PB8
*     FDA1_OUTP引脚为PB11	FDA1_OUTN引脚为PB14
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"


/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);			/* 时钟配置函数 */
void SYS_Configuration(void);			/* 系统时钟配置函数 */
void GPIO_Configuration(void);		/* GPIO配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */
void FDA_Configuration(void);			/* FDA配置函数 */
void ADC_Configuration(void);			/* ADC配置函数 */


uint32_t 	Flag = 0;
uint32_t 	ADC_DAT[4], ADC_Value[4];


/*
*********************************************************************************************************
*	函 数 名: ADC_IRQHandler
*	功能说明: ADC完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_IRQHandler(void)
{
	/* 获取FDA输入端放大后的电压差*/
	ADC_DAT[0] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel0);
	ADC_Value[0] = ADC_DAT[0]*330/2048; 

	/* 清除ADC转换完成标志位 */
	ADC_ClearFlag(ADC1, ADC_FLAG_EOS1);
	
	/* 置位ADC转换标志 */
	Flag = 1;
}


/*
*********************************************************************************************************
*	函 数 名: delay
*	功能说明: 延时
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void delay(void)
{
	uint32_t i,j;
	
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
	/* 使能外设时钟(GPIOA/B/C/D) ADC1/2 FDA0/1*/
	RCC_Configuration();	
	
	/* 系统时钟配置为168MHz */
	SYS_Configuration();
	
	/* 配置FDA输入输出引脚为模拟模式 */
	GPIO_Configuration();
	
	/* 配置ADC中断优先级使能 */
  NVIC_Configuration();
	
	/* FDA初始化 */
	FDA_Configuration();
	
	/* ADC初始化 */
	ADC_Configuration();
	
	while(1)
	{
		
		/* 连续软件触发ADC转换 */
		if(Flag)
		{
			Flag = 0;
			
			delay();
			ADC_SoftwareStartConv(ADC1);
		}
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_FDA0 | RCC_APB2Periph_FDA1, ENABLE);

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
	
	#if TSET_FDA
	//FDA1_INP  FDA1_INN
	GPIO_InitStructure.GPIO_Pin 	= FDA_INPUTPIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;				/* 模拟状态 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 	/* 内部无上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;		/* 高速输出 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#else
	//FDA0_INP INN
	GPIO_InitStructure.GPIO_Pin 	= FDA_INPUTPIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;				/* 模拟状态 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 	/* 内部无上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;		/* 高速输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	#endif
	
	//FDA_OUTP FDA_OUTN
	GPIO_InitStructure.GPIO_Pin 	= FDA_OUTPUTPIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/*
*********************************************************************************************************
*	函 数 名: FDA_Configuration
*	功能说明: FDA配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void FDA_Configuration(void)
{
	FDA_InitTypeDef  FDA_InitStructure;
	
	FDA_InitStructure.FDA_InputSignal = FDA_InputSignal_INP0_INN0;	/* 输入端为INP0_INN0 */
	FDA_InitStructure.FDA_OutputPin		= FDA_OutputPin_DISABLE;			/* 输出引脚不外接 */
	FDA_InitStructure.FDA_CKD					= FDA_CKD_170K_40K;						/* 配置放倍数 */
	FDA_Init(FDAn, &FDA_InitStructure);
	
	FDA_Cmd(FDAn, ENABLE);																					/* FDAx使能  */
}


/*
*********************************************************************************************************
*	函 数 名: ADC_Configuration
*	功能说明: ADC配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ADC_Configuration()
{
  ADC_InitTypeDef ADC_InitStruct;
   
  ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div12;																/* ADC时钟分频 */
	ADC_InitStruct.ADC_Range  = ADC_Range_VDDA;																/* 参考电压选择 */
	ADC_InitStruct.ADC_OverSamplingMode 		= ADC_OverSampling_Disable;				/* 过采样模式关闭 */
  ADC_InitStruct.ADC_OverSamplingTrigger 	= ADC_OverSamplingTrigger_Once;		/* 过采样模式触发模式 */
	ADC_InitStruct.ADC_OverSamplingRate 		= ADC_2X_OverSamplingRate;				/* 过采样率 */
	ADC_InitStruct.ADC_OverSamplingOffset 	= ADC_OverSamplingOffset_0;				/* 过采样移位 */
  ADC_Init(ADC1, &ADC_InitStruct);
	
	/* ADC 序列配置 */
	ADC_SQRInit(ADC1, ADC_SQR1, ADC_SQRxTRIG_Software, ADC_Ens0to0);
	
	/* ADC 序列的通道配置 */
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_FDAyINP0INN0, ADC_SampTime390Clk);
	
	/* ADC 中断配置 */
	ADC_ITConfig(ADC1, ADC_IT_EOS1, ENABLE);
	
	/* ADC 使能 */
  ADC_Cmd(ADC1,ENABLE);    
	
	/* ADC 软件触发启动转换 */	
	ADC_SoftwareStartConv(ADC1);
}
	
	
/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统倍频设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
	RCC_HSEConfig(RCC_HSE_ON);														/* HSE使能  */		
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; /* 等待HSE启动就绪完成  */
		
	RCC_PLLCmd(DISABLE);    															/* PLL关闭 */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);											/* 设置系统时钟分频 */
	RCC_PCLK1Config(RCC_HCLK_Div4);												/* 设置PCLK1时钟分频 */
	RCC_PCLK2Config(RCC_HCLK_Div2);												/* 设置PCLK2时钟分频 */
	
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    				/* PLL时钟配置为168MHz  M N P Q */ 
		
	RCC_PLLCmd(ENABLE);   																/* PLL使能 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; /* PLL倍频就绪完成 */  
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    				/* 系统时钟切换为PLL时钟源 */  
	while (RCC_GetSYSCLKSource() != 0x08){}    						/* 等待系统时钟切换完成 */ 
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

  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
