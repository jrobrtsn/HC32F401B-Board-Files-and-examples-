/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(PWR)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片低功耗睡眠（停机模式/待机模式）与唤醒
*            适用芯片: HC32F401系列
*              实验步骤：
*								1. 编译并下载程序
*								2. 系统初始化与GPIO初始化
*								3. PA8翻转10次后，进入低功耗睡眠
*								4. 按下按键S1 (PA0) 唤醒芯片
*								5.1 若芯片为停机模式，PA8持续翻转
*								5.2 若芯片为待机模式，芯片复位后运行
*	
*	注意事项：
*
*	通过调整宏定义  TEST_LOWPOWER 选择低功耗类型
*	0 - 待机模式
*	1 - 停机模式且电压调节器开启
*	2 - 停机模式且电压调节器关闭
*	
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/

#include "main.h"


/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);			/* 时钟配置函数 */
void GPIO_Configuration(void);		/* GPIO配置函数 */
void EXTI_Configuration(void);		/* EXTI中断配置函数 */
void NVIC_Configuration(void);		/* 中断优先级配置函数 */


//0 - 待机模式
//1 - 停机模式且电压调节器开启
//2 - 停机模式且电压调节器关闭
#define TEST_LOWPOWER		0	


uint32_t i;


/*
*********************************************************************************************************
*	函 数 名: EXTI0_IRQHandler
*	功能说明: PA0上升沿中断 用于停机模式时芯片唤醒
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI0_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0);		/* 清除外部中断标志位0 */
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

	/* 使能外设时钟(GPIOA/B/C/D) */
	RCC_Configuration();	
	
	/* 配置GPIO(PA0为数字输入 PA8为数字输出) */
	GPIO_Configuration();
	
	/* PA8翻转 用于待机模式唤醒时查看芯片是否复位运行*/
	for(i=0; i<10; i++)
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
	
	/* 
		DBG仿真使能 
		用于在线仿真时芯片进入睡眠模式时保持系统时钟不断连 方便查看唤醒后的现象
		测量睡眠功耗时需屏蔽		
	*/
	DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);	
	
	/* 设置唤醒引脚后进入低功耗模式 */
	#if (TEST_LOWPOWER == 0)
		PWR_ClearFlag(PWR_FLAG_WU | PWR_FLAG_SB);
		PWR_WakeUpPinCmd(ENABLE);
	#elif  ((TEST_LOWPOWER == 1) || (TEST_LOWPOWER == 2))
		EXTI_Configuration();
		NVIC_Configuration();
		EXTI_ClearITPendingBit(EXTI_Line0);	
	#endif		
	
	#if (TEST_LOWPOWER == 0)
		PWR_EnterSTANDBYMode();																		
	#elif  (TEST_LOWPOWER == 1)
		PWR_EnterSTOPMode(PWR_MainRegulator_ON, PWR_STOPEntry_WFI);
	#elif  (TEST_LOWPOWER == 2)
		PWR_EnterSTOPMode(PWR_LowPowerRegulator_ON, PWR_STOPEntry_WFI);		
	#endif
	
	/*
		按下按键S1 (PA0) 唤醒芯片:
			若芯片为停机模式，PA8持续翻转
			若芯片为待机模式，芯片复位后运行
	*/
	
	while(1)
	{
		/* PA8翻转 用于停机模式唤醒时查看芯片是否运行 */
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
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
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
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
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;			/* 数字输入 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN;		/* 内部下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;	/* 高速输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	#if ((TEST_LOWPOWER == 1) || (TEST_LOWPOWER == 2))
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);		/* 设置PA0为外部中断源 */
	#endif	
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;		/* 数字输入 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;	/* 内部无上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;	/* 高速输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
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
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			/* 中断请求 */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 	/* 上升沿触发 */
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;								/* 中断使能 */
  EXTI_Init(&EXTI_InitStructure);													/* 中断初始化 */
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

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;							
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;	/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;					/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								/* 中断通道使能	 */
  NVIC_Init(&NVIC_InitStructure);																/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/

