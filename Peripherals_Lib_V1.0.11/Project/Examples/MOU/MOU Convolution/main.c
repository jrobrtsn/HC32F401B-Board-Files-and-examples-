/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(MOU)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片MOU一维卷积功能.
*
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序；
*              2. 系统初始化
*              3. 对序列与卷积核赋值
*              4. MOU初始化与使能
*              5. 配置卷积参数：卷积长度，步长，计算次数
*              6. 软件启动MOU运算
*              7. 等待MOU计算完成中断
*	
*	注意事项：
*
* 序列A		= {1,2,3,4,5}
* 卷积核 	= {1,2,3}
*
* 计算过程如下：
		窗口1（位置1-3）：1×1 + 2×2 + 3×3 = 1 + 4 + 9 = 14

		窗口2（位置2-4）：2×1 + 3×2 + 4×3 = 2 + 6 + 12 = 20

		窗口3（位置3-5）：3×1 + 4×2 + 5×3 = 3 + 8 + 15 = 26
		
*	卷积结果 = {14,20,26}
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);			/* 时钟配置函数 */
void SYS_Configuration(void);			/* 系统时钟配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */
void MOU_Configuration(void);			/* MOU配置函数 */


/* 序列 */
uint8_t 	Matrix_A[3] __attribute__((at(Matrix_A_Base))); 

/* 卷积核 */
uint8_t 	Matrix_B[5] __attribute__((at(Matrix_B_Base))); 

/* 卷积结果 */
uint32_t 	Matrix_C[3] __attribute__((at(Matrix_C_Base)));


uint8_t i;


/*
*********************************************************************************************************
*	函 数 名: MOU_IRQHandler
*	功能说明: MOU完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MOU_IRQHandler()
{
	/* 获取运算结果 */
	MOU_GetResult(Matrix_C, 3);	
	
	/* 清除计算完成标志位 */
	MOU_ClearFlag(MOU_FLAG_FCC);
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
	/* 使能外设时钟(GPIOA/B/C/D) MOU */
	RCC_Configuration();	
	
	/* 系统时钟配置为168MHz */
	SYS_Configuration();
	
	/* MOU卷积初始化 */
	MOU_Configuration();
	
	/* 配置MOU中断优先级使能 */
	NVIC_Configuration();
	
	/* 对序列与卷积核赋值 */
	for(i=0; i<3; i++)
		Matrix_A[i] =  i+1; 
	
	for(i=0; i<5; i++)
		Matrix_B[i] =  i+1; 
	
	/* 配置卷积参数:	序列偏移地址, 卷积核偏移地址， 卷积结果偏移地址， 
									卷积长度，步长，计算次数 */
	MOU_ConvConfig(0,0,0,3,1,3);
	
	/* MOU开始运算 */
	MOU_SoftwareStartConv();
		
	while(1)
	{

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
	//GPIOA B C D
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	//MOU
	RCC_AHB2PeriphClockCmd( RCC_AHB2Periph_MOU, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: MOU_Configuration
*	功能说明: MOU配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void MOU_Configuration(void)
{
  MOU_InitTypeDef   MOU_InitStructure;
	
	MOU_InitStructure.MOU_MODE  = MOU_MODE_CONV;			/* 卷积模式 */
	MOU_InitStructure.MOU_UNIT  = MOU_UNIT_BYTE;			/* 字节运算模式 */
	MOU_InitStructure.MOU_BIAS  = MOU_BIAS_DISABLE;		/* 偏置关闭（卷积模式无效） */
	MOU_InitStructure.MOU_TYPEA = MOU_TYPEA_UNSIGNED;	/* 矩阵A为无符号 */
	MOU_InitStructure.MOU_TYPEB = MOU_TYPEB_UNSIGNED;	/* 矩阵B为无符号 */
	MOU_Init(&MOU_InitStructure);
	
	MOU_ITConfig(MOU_IT_FCC, ENABLE);									/* 计算完成中断使能 */
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
*	功能说明: 中断优先级设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  NVIC_InitStructure.NVIC_IRQChannel = MOU_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
