/*
*********************************************************************************************************
*
*	模块名称 : FLASH模块(读写FLASH)
*	文件名称 : main.c
*	说    明 :
*            	此样例演示FLASH读写操作
*            	适用芯片: HC32F401系列
*              		实验步骤:
*                	1. 编译下载程序并进入仿真模式；
*					2. 在GPIO_ToggleBits处打一个断点，点击全速运行；
*					3. 在watch窗口查看buf变量，就是写入后读取的值。
*	注意事项 :
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);		/* 外设时钟使能配置函数 */
void GPIO_Configuration(void);		/* GPIO引脚配置函数 */
void NVIC_Configuration(void);		/* 嵌套向量中断控制器(NVIC)配置函数 */

/* 全局变量 -----------------------------------------------*/
uint32_t buf[3];

/*
*********************************************************************************************************
*	函 数 名: FLASH_ReadByte
*	功能说明: 读取一个字节数据(8位)
*	形    参: Address :目标地址
*	返 回 值: 读取到的数据
*********************************************************************************************************
*/
uint8_t FLASH_ReadByte(uint32_t Address)
{
	return *(__IO uint8_t*)Address;
}

/*
*********************************************************************************************************
*	函 数 名: FLASH_ReadHalfWord
*	功能说明: 读取半字数据(16位)
*	形    参: Address :目标地址
*	返 回 值: 读取到的数据
*********************************************************************************************************
*/
uint16_t FLASH_ReadHalfWord(uint32_t Address)
{
	return *(__IO uint16_t*)Address;
}

/*
*********************************************************************************************************
*	函 数 名: FLASH_ReadWord
*	功能说明: 读取一个字数据(32位)
*	形    参: Address :目标地址
*	返 回 值: 读取到的数据
*********************************************************************************************************
*/
uint32_t FLASH_ReadWord(uint32_t Address)
{
	return *(__IO uint32_t*)Address;
}

/*
*********************************************************************************************************
*	函 数 名: FLASH_IRQHandler
*	功能说明: FLASH中断服务函数，编程结束EOP由硬件置1，写1清零
*	形    参:无
*	返 回 值: 无
*********************************************************************************************************
*/
void FLASH_IRQHandler()
{	
	/* 清零EOP标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP);
}

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
	
	/* 配置NVIC(FLASH中断优先级) */
	NVIC_Configuration();
	
	/* 解锁FLASH */
	FLASH_Unlock();
	
	/* 清PGSERR标志位和PGPERR标志位 */
	FLASH_ClearFlag(FLASH_FLAG_PGSERR | FLASH_FLAG_PGPERR);
	
	/* 使能操作结束中断 */
	FLASH_ITConfig(FLASH_IT_EOP, ENABLE);	
	
	/* 擦除0x08004000-0x08004FFF地址的数据，即1个扇区 */
	FLASH_EraseSector(0x08004000);
	
	/* (8位)字节编程，0x08004000写入0x12 */
	FLASH_ProgramByte(0x08004000, 0x12);
	
	/* (16位)半字编程，0x08004004写入0x1234 */
	FLASH_ProgramHalfWord(0x08004004, 0x1234);
	
	/* (32位)字编程，0x08004008写入0x12345678 */
	FLASH_ProgramWord(0x08004008, 0x12345678);	

	/* (8位)字节读取，读取0x08004000地址数据 */
	buf[0]=(uint32_t)FLASH_ReadByte(0x08004000);
	
	/* (16)半节读取，读取0x08004004地址数据 */
	buf[1]=(uint32_t)FLASH_ReadHalfWord(0x08004004);
	
	/* (32位)字读取，读取0x08004008地址数据 */
	buf[2]=(uint32_t)FLASH_ReadWord(0x08004008);
	
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
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, Bit_SET);						/* PA8置1 */
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
	NVIC_InitStructure.NVIC_IRQChannel = FLASH_IRQn;					/* FLASH全局中断通道 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 抢占优先级: 0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;				/* 子优先级:   0x0F(最低) */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					/* 使能该中断通道 */
	NVIC_Init(&NVIC_InitStructure);	
}
