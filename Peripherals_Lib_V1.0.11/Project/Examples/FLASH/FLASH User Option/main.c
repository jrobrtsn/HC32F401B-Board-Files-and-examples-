/*
*********************************************************************************************************
*
*	模块名称 : 选项字配置程序
*	文件名称 : main.c
*	说    明 :
*            	
*            	适用芯片: HC32F401系列
*              	实验步骤：
*              	1. 编译下载程序；
*              	2. 重新上电，用户选项字节生效
* 
*	注意事项 :
*
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);		/* 外设时钟使能配置函数 */
void GPIO_Configuration(void);		/* GPIO引脚配置函数 */

/* 全局变量 -----------------------------------------------*/



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
	
	/* 配置GPIO */
	GPIO_Configuration();
		
	/* 解锁FLASH */
	FLASH_Unlock();
	
	/* 解锁OPTKEY */
	FLASH_OB_Unlock();
	
	/* 清除编程错误标志，PGSERR和PGPERR标志位 */
	FLASH_ClearFlag(FLASH_FLAG_PGSERR | FLASH_FLAG_PGPERR);
	
	/* SRAM1扩展模式:64KB，SRAM1可选64/128/160KB,SRAM2为32KB */
	FLASH_OB_EOPBConfig(OB_EOPB_64);
	
	/* 失能WRP1EN/WRP2EN写保护 */
	FLASH_OB_WRPConfig(OB_WRP1_DISABLE, OB_WRP2_DISABLE);
	
	/* 配置WRP1写保护范围:0x8000000-0x80007FF */
	FLASH_OB_WRP1_AddressConfig(0x8000000, 0x80007FF);
	
	/* 配置WRP2写保护范围:0x8004000-0x80047FF */
	FLASH_OB_WRP2_AddressConfig(0x8004000, 0x80047FF);
		
	/* 开启软件独立看门狗，进入停止模式不产生复位，进入待机模式不产生复位 */
	FLASH_OB_UserConfig(OB_IWDG_SW, OB_STOP_NoRST, OB_STDBY_NoRST);
	
	/* 锁定选项字 */
	FLASH_OB_Launch();	
	
	while(1)
	{
		/* PA8翻转 */
		GPIO_ToggleBits(GPIOA, GPIO_Pin_8);
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
	/* 使能GPIOA、GPIOB、GPIOC、GPIOD的AHB1总线时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
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
		
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;					/* PA8 */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;				/* 输出模式 */
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;				/* 推挽输出 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 			/* 无内部上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed   = GPIO_High_Speed;				/* 高速模式 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

