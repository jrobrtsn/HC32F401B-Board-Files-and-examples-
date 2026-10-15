/*
********************************************************************************
*
*	模块名称：主程序模块（I2C发送模式）
*	文件名称：main.c
*	说    明：
*							此样例演示了I2C发送模式
*							使用芯片：HC32F401系列
*							 	实验步骤：
*									1. 编译并下载程序;
*									2. 系统初始化与I2C初始化
*
*注意事项:
*
*
*
*	Copyright (C), 2009~2026, 芯圣电子  www.holychip.cn
*
********************************************************************************
*/


#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);				/* 系统配置 */
void RCC_Configuration(void);				/* 时钟配置 */
void GPIO_Configuration(void);			/* GPIO配置 */
void I2C_Configuration(void);				/* I2C配置 */
void NVIC_Configuration(void);			/* 中断配置 */
void DMA_Configuration(void);				/* DMA配置 */

/* 变量定义 -----------------------------------------------*/
uint8_t 	u8State, n, m, flag = 1;

uint8_t		TX_BUFF[5] = {0x11, 0x12, 0x13, 0x14, 0x15};
uint8_t		RX_BUFF[5];

/*
********************************************************************************
*	函 数 名：Delay
*	功能说明：延时函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void Delay(uint32_t n)
{
	uint32_t i,j;
	for(i=0; i<n; i++)
	 for(j=0; j<n; j++);
}

/*
********************************************************************************
*	函 数 名：I2C1_EV_IRQHandler
*	功能说明：I2C1_EV_IRQHandler中断函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void I2C1_EV_IRQHandler()
{	
	u8State = I2C_GetITStatus(I2C1);
    
	switch (u8State){
			case 0x08: 									/* 启示信号发送完成 */
				I2C_SendData(I2C1, 0x02);	/* send slv(0x01)+w(0) */
				/* I2C1清零STA */	
				I2C_GenerateSTART(I2C1, DISABLE);	
				break;
			case 0x10:									/* 重复启示信号发送完成 */
				break;
			case 0x18:									/* SLV+W/R 发送完成,ACK */
				break;
			case 0x20:									/* SLV+W/R 发送完成,NACK */
				break;
			case 0x28:									/* DATA 发送完成,ACK */
				 /* I2C1向总线发送STOP停止信号 */
				I2C_GenerateSTOP(I2C1, ENABLE);
				flag = 1;
				break;
			case 0x30:									/* DATA 发送完成,NACK */
				break;
      default:
           break;
	}
		/* 清I2C1中断 */
    I2C_ClearFlag(I2C1);
}

/*
********************************************************************************
*	函 数 名：I2C2_EV_IRQHandler
*	功能说明：I2C2_EV_IRQHandler中断函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void I2C2_EV_IRQHandler()
{	
	u8State = I2C_GetITStatus(I2C2);

	switch (u8State){
		case 0x60: 									/* 被寻址成功，ACK */	
			break;
		case 0x80:									/* 数据被成功接收 ACK */
			break;
		case 0x88:									/* 数据被成功接收 NACK */	
			break;
		case 0xA0:									/* 接收到STOP/RESTART信号 */
			break;
		default:
			break;
	}
	/* 清I2C2中断 */
	I2C_ClearFlag(I2C2);
}

/*
********************************************************************************
*	函 数 名：DMA1_Stream0_IRQHandler
*	功能说明：DMA流0全局中断函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void DMA1_Stream0_IRQHandler()
{	
	/* 清DMA流0全局中断,数据流0上无传输完成事件 */
	DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);	
		/* 禁止DMA请求 */
	I2C_DMACmd(I2C1, DISABLE);
}

/*
********************************************************************************
*	函 数 名：DMA1_Stream1_IRQHandler
*	功能说明：DMA流1全局中断函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void DMA1_Stream1_IRQHandler()
{
	/* 清DMA流1全局中断,数据流1上无传输完成事件 */
	DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
	/* 清I2C2中断 */
	I2C_ClearFlag(I2C2);
	
	/* I2C2清零STA */
	I2C_GenerateSTART(I2C2, ENABLE);
	/* I2C2向总线发送START起始信号 */	
	I2C_GenerateSTART(I2C2, DISABLE);
}

/*
********************************************************************************
*	函 数 名：main
*	功能说明：程序主入口函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
int main(void)
{
	/* 时钟配置 */
	RCC_Configuration();
	/* 系统配置 */
	SYS_Configuration();
	/* GPIO配置 */
	GPIO_Configuration();
	/* I2C配置 */
	I2C_Configuration();
	/* DMA配置 */
	DMA_Configuration();
	/* 中断配置 */
	NVIC_Configuration();
	
	while(1)
	{
		if(flag)
		{
			flag = 0;
			m = 0;
			n = 0;
			
			/* 允许DMA请求 */
			I2C_DMACmd(I2C1, ENABLE);	
			/* I2C1清零STA */	
			I2C_GenerateSTART(I2C1, ENABLE);
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
	/* 使能PORTA/B/C/D时钟,使能DMA1时钟 */
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);
	/* 使能I2C1/I2C2时钟 */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
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
	
	/* 选择PIN2/3/11/12 */
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_2 |  GPIO_Pin_3 | GPIO_Pin_11 | GPIO_Pin_12;
	/* 端口复用功能模式 */
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;
	/* 端口开漏输出 */
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_OD;
	/* 端口无上拉或下拉 */
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;
	/* 端口高速输出速度 */
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;
	/* 保存PORTA端口配置 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* PORTA2复用I2C1_SCL */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_4);
	/* PORTA3复用I2C1_SDA */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_4);
	/* PORTA11复用I2C2_SCL */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_4);
	/* PORTA12复用I2C2_SDA */	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);
}

/*
*********************************************************************************************************
*	函 数 名: I2C_Configuration
*	功能说明: I2C_Configuration函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void I2C_Configuration(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	
	/* I2C主机模式 */
	I2C_InitStructure.I2C_MODE 	= I2C_Mode_Master;	
	/* I2C波特率计数器配置 */
	I2C_InitStructure.I2C_BRR 	= 209;
	/* I2C是否允许DMA请求使能 */
	I2C_InitStructure.I2C_DMAEN = I2C_DMA_DISABLE;	
	/* I2C滤波参数配置 */
	I2C_InitStructure.I2C_FLT		= I2C_FLT_DISABLE;	
	/* 保存I2C1配置 */
	I2C_Init(I2C1, &I2C_InitStructure);
	
	/* I2C从机模式 */
	I2C_InitStructure.I2C_MODE 	= I2C_Mode_Slave;
	/* 保存I2C2配置 */
	I2C_Init(I2C2, &I2C_InitStructure);
	
	/* I2C2从机地址0配置 */
	I2C_OwnAddress0Config(I2C2, 0x01);
	/* I2C2回复ACK */
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	/* I2C2使能 */
	I2C_Cmd(I2C2, ENABLE);

	/* I2C1回复ACK */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* I2C1使能 */
	I2C_Cmd(I2C1, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: DMA_Configuration
*	功能说明: DMA_Configuration函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;

	/* I2C1映射DMA通道15 */  
  DMA_InitStructure.DMA_Channel = DMA_Channel_15;
	/* I2C1数据寄存器清0 */ 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->DR;	
	
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TX_BUFF;
	/* 存储器到外设 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	
  DMA_InitStructure.DMA_BufferSize = (uint32_t)5;
	/* 外设递增模式 */ 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* 存储器递增模式 */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* 外设数据8位 */ 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
	/* 储存器数据8位 */ 
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* 使能循环模式 */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/* 优先级高 */ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/* 使能直接模式 */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	/* FIFO阈值满值 */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	/* 存储器单次传输 */ 
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	/* 外设单次传输 */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/* 保存DMA流0配置 */
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
    
	/* DMA流0传输完成中断 */	
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	/* 使能DMA流0全局中断 */
	DMA_Cmd(DMA1_Stream0, ENABLE);
	
	/* I2C2映射DMA通道18 */
  DMA_InitStructure.DMA_Channel = DMA_Channel_18;
  /* I2C2数据寄存器清0 */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C2->DR;
	
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)RX_BUFF;
	/* 外设到存储器 */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	/* 保存DMA流1配置 */
  DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    
	/* DMA流1传输完成中断 */
  DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
	/* 使能DMA流1全局中断 */
	DMA_Cmd(DMA1_Stream1, ENABLE);
	
}

/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数(本示例未调用，可按需使用)。
*           配置流程如下:
*             1. 开启外部高速晶振(HSE, 8MHz)
*             2. 等待HSE起振稳定
*             3. 关闭PLL，配置各总线分频系数
*                - HCLK  = SYSCLK / 1
*                - PCLK1 = HCLK / 4
*                - PCLK2 = HCLK / 2
*             4. 配置PLL: HSE(8MHz) / 8(M) × 336(N) / 2(P) = 168MHz
*             5. 使能PLL并等待锁定
*             6. 切换系统时钟源为PLL输出(168MHz)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
	/* HSE震荡器开启 */
	RCC_HSEConfig(RCC_HSE_ON);				
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 

	/* PLL关闭 */ 
	RCC_PLLCmd(DISABLE);  						
	/* 系统时钟不分频 */ 
	RCC_HCLKConfig(RCC_SYSCLK_Div1);	 
	/* AHB1时钟4分频 */
	RCC_PCLK1Config(RCC_HCLK_Div4);		  
	/* AHB2时钟2分频 */	
	RCC_PCLK2Config(RCC_HCLK_Div2);		 
		
	 /* PLL时钟配置168,PLLM,PLLN,PLLP,PLLQ */
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);   
	/* PLL开启 */  
	RCC_PLLCmd(ENABLE);  							
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};   
		
	/* 选择PLL做为系统时钟 */   
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   		 
	while (RCC_GetSYSCLKSource() != 0x08){} 
}

/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*           本示例配置了2个中断通道:
*             1. I2C1_IRQn      - I2C1全局中断
*             2. I2C2_IRQn      - I2C2全局中断
*             3. DMA1_Stream0_IRQn      - DMA流0全局中断
*             4. DMA1_Stream1_IRQn      - DMA流1全局中断
*           配置参数:
*             中断1(I2C1):
*               - 中断通道:      I2C1_IRQn
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*             中断2(I2C2):
*               - 中断通道:      I2C2_IRQn
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*             中断3(DMA1_Stream0):
*               - 中断通道:      DMA1_Stream0
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*             中断4(DMA1_Stream1):
*               - 中断通道:      DMA1_Stream1
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

	/* 中断通道选择I2C1 */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_IRQn;
	/* 中断抢占优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	/* 中断通道子优先级设置 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	/* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
	/* 中断通道选择I2C2 */
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_IRQn;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
	
	/* 中断通道选择DMA流0全局中断 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
	/* 中断通道选择DMA流1全局中断 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	/* 保存中断配置 */
  NVIC_Init(&NVIC_InitStructure);
}
