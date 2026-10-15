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

/* 变量定义 -----------------------------------------------*/
uint8_t 	u8State, n, m, flag = 1;

uint8_t		TX_BUFF[5] = {0x10, 0x11, 0x12, 0x13, 0x14};
uint8_t		RX_BUFF[5];

/*
********************************************************************************
*	函 数 名：I2C1_EV_IRQHandler
*	功能说明：I2C1_EV_IRQHandler函数
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
				I2C_SendData(I2C1, TX_BUFF[n++]);
				break;
			case 0x20:									/* SLV+W/R 发送完成,NACK */
				break;
			case 0x28:									/* DATA 发送完成,ACK */		
				if(n<5)
					I2C_SendData(I2C1, TX_BUFF[n++]);
				else
					/* I2C1向总线发送SRART停止信号 */	
					I2C_GenerateSTOP(I2C1, ENABLE);	 
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
*	功能说明：I2C2_EV_IRQHandler函数
*	形		参：无
*	返 回 值：无
********************************************************************************
*/
void I2C2_EV_IRQHandler()
{	
	u8State = I2C_GetITStatus(I2C2);

	switch (u8State){
		case 0x60: 									//被寻址成功，ACK */	
			break;
		case 0x80:									//数据被成功接收 ACK */
			RX_BUFF[m++] = I2C_ReceiveData(I2C2);
			break;
		case 0x88:									//数据被成功接收 NACK */
			break;
		case 0xA0:									//接收到STOP/RESTART信号 */
			flag = 1;			
			break;			
		default:
			break;
	}
	/* 清I2C2中断 */
	I2C_ClearFlag(I2C2);				
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
	/* 中断配置 */
	NVIC_Configuration();
	
	while(1)
	{
		if(flag)
		{
			flag = 0;
			m = 0;
			n = 0;
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
	/* PORTA2复用I2C1_SDA */		
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
*           配置参数:
*             中断1(I2C1):
*               - 中断通道:      I2C1_IRQn
*               - 抢占优先级:    0x0F(最低)
*               - 子优先级:      0x0F(最低)
*             中断2(I2C2):
*               - 中断通道:      I2C2_IRQn
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
}
