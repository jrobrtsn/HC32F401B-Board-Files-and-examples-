/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(CANFD)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片CANFD环回模式下数据的发送与接收
*
*            适用芯片: HC32F401系列
*              实验步骤：
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. CANFD/中断初始化
*              4. 填充数据后发送
*              5. 待发送完成中断响应读取数据
*
*	注意事项：
*						由于CANFD模块时钟设计有误，使用CANFD时时钟源 ≤ 48MHz  且需保证 HCLK = PCLK  
*						所以需要使用CANFD模块时，系统最高频率为96MHz 例程中SYS_Configuration函数已配置好，直接调用即可
*
*						通过修改宏定义 TEST_BUFFER_FIFO 变更发送与接受模式
*						1 - BUFFER 
*						0 - FIFO
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
void CAN_Configuration(void);			/* CANFD配置函数 */


#define TEST_BUFFER_FIFO  1

uint32_t i, TxFIFO_State;


CanTxMessage TxMsg;						/* 发送帧结构体 */	
CanRxMessage RxMsg[8];						/* 接收帧结构体 */
CanSIDFilterMessage SIDFMsg;	/* 滤波器结构体 */


/*
*********************************************************************************************************
*	函 数 名: CAN1_INT0_IRQHandler
*	功能说明: CAN传输完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CAN1_INT0_IRQHandler(void)
{
	/* 清除发送完成标志位 */
	CAN_ClearITPendingBit(CAN1, CAN_IT_TC);
	
	/* 获取传输数据 */	
	/* RAM地址 = StartAddress + [取出下标]*[占用字节个数]*4 */
	for(i=0; i<8; i++)
	{
		#if TEST_BUFFER_FIFO	
			CAN_CopyRAMToRxMessage(CAN1, CAN_RxBUFFER_BASE + i*4*4,  &RxMsg[i]); 
		#else
			CAN_CopyRAMToRxMessage(CAN1, CAN_RxFIFO0_BASE + i*4*4,  &RxMsg[i]);
		#endif
	}

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
	
	/* 系统时钟配置为96MHz */
	SYS_Configuration();
	
	/* 配置CANFD引脚 PA2为CANFD_RX PA3为CANFD_TX */
	GPIO_Configuration();
	
	/* 配置CANFD参数	*/
	CAN_Configuration();
	
	/* 配置中断优先级 */
	NVIC_Configuration();
	
	while(1)
	{

	}
}


/*
*********************************************************************************************************
*	函 数 名: CAN_Configuration
*	功能说明: CAN传输完成中断
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void CAN_Configuration(void)
{
  CAN_InitTypeDef 							CAN_InitStruct;							/* CAN通用初始化结构体   配置波特率与工作模式 */
	CAN_FilterInitTypeDef 				CAN_FilterInitStruct;				/* CAN过滤器初始化结构体 选择标准ID/扩展ID过滤 与 过滤首地址和个数*/
	
	#if TEST_BUFFER_FIFO
		CAN_TXBufferInitTypeDef 		CAN_TXBufferInitStruct;			/* CAN发送buffer结构体 */
		CAN_RXBufferInitTypeDef     CAN_RXBufferInitStruct;			/* CAN接收buffer结构体 */
	#else
		CAN_GlobalFilterInitTypeDef CAN_GlobalFilterInitStruct;	/* CAN全局过滤器结构体	配置过滤器处理模式*/
		CAN_TXSequenceInitTypeDef 	CAN_TXSequenceInitStruct;		/* CAN发送序列结构体 	选择FIFO/队列模式*/
	  CAN_RXFIFOInitTypeDef 			CAN_RXFIFOInitStruct;				/* CAN接收FIFO结构体 */
	#endif
	
	CAN_EnterInitMode(CAN1);													/* 进入CAN初始化 */
	
	CAN_InitStruct.CAN_DLatencyCompensation = 0;			/* 发送器延迟补偿关闭 */
	CAN_InitStruct.CAN_DPrescaler = 11;								/* 数据位预分频					波特率为 fPLK 	/ ((3+BS1+BS2)*(Prescaler+1) */
  CAN_InitStruct.CAN_DSJW = 1; 											/* 数据位同步跳转宽度							48	/	((3+1+4)*(11+1))	= 0.5MHz	*/
	CAN_InitStruct.CAN_DBS1 = 4; 											/* 数据位采样点前时间段 */
	CAN_InitStruct.CAN_DBS2 = 1;	 										/* 数据位采样点后时间段 */
	CAN_InitStruct.CAN_NPrescaler = 11;								/* 标称位预分频 */
  CAN_InitStruct.CAN_NSJW = 1;  										/* 标称位同步跳转宽度 */
	CAN_InitStruct.CAN_NBS1 = 4;											/* 标称位采样点前时间段 */
	CAN_InitStruct.CAN_NBS2 = 1;											/* 标称位采样点后时间段 */
	
	CAN_InitStruct.TransferPause     = DISABLE;				/* 传输暂停关闭 */				
	CAN_InitStruct.EdgeFiltering     = DISABLE;				/* 总线边沿过滤关闭 */	
	CAN_InitStruct.NoErrorHandling   = DISABLE;				/* 协议异常处理禁用关闭 */
	CAN_InitStruct.WideMessageMarker = DISABLE;				/* 宽消息标记关闭 */
	CAN_InitStruct.BitRateSwitch     = DISABLE;				/* 比特速率切换关闭 */
	CAN_InitStruct.CAN_FD     			 = DISABLE;				/* CANFD模式关闭 */
	CAN_InitStruct.CAN_Test   			 = ENABLE;				/* 测试模式使能 */
	CAN_InitStruct.CAN_DAR     			 = DISABLE;				/* 自动重传禁用关闭 */														
	CAN_InitStruct.CAN_BusMonitor    = DISABLE;				/* 总线监控模式关闭 */	
	CAN_Init(CAN1, &CAN_InitStruct);
	
	CAN1->TEST |= (0x1 << 4); 												/* 环回模式使能 (TX->TX_PIN & RX) */
	
	CAN_FilterInitStruct.CAN_Filter_Depth = 8;								/* 滤波器个数为8 */
	CAN_FilterInitStruct.CAN_Filter_StartAddress = 0xC000;		/* 滤波器首地址配置 */
	CAN_FilterConfig(CAN1, CAN_StdId, &CAN_FilterInitStruct);	
	
	#if TEST_BUFFER_FIFO		
																																											/* RXBuffer 默认深度为 20 */
		CAN_RXBufferInitStruct.CAN_RXBuffer_Width = CAN_BufferWidth_8BYTES;								/* RXBuffer 宽度为8 bytes */
		CAN_RXBufferInitStruct.CAN_RXBuffer_StartAddress = 0xC980;												/* RXBuffer 首地址配置*/
		CAN_ReceiveBufferConfig(CAN1, &CAN_RXBufferInitStruct);		
		
		CAN_TXBufferInitStruct.CAN_TXBuffer_Depth = 8;																		/* TXBuffer 深度配置 */
		CAN_TXBufferInitStruct.CAN_TXBuffer_Width = CAN_BufferWidth_8BYTES;								/* TXBuffer 宽度为8 bytes */
		CAN_TXBufferInitStruct.CAN_TXBuffer_StartAddress = 0xCDC0;												/* TXBuffer 首地址配置*/
		CAN_SendBufferConfig(CAN1, &CAN_TXBufferInitStruct);	
	#else
		CAN_GlobalFilterInitStruct.CAN_MismatchStandardFrame  = CAN_StorageType_FIFO1;		/* 接收不匹配的标准帧存储位置 */
		CAN_GlobalFilterInitStruct.CAN_MismatchExtendedFrame  = CAN_StorageType_FIFO1;		/* 接收不匹配的扩展帧存储位置*/
		CAN_GlobalFilterInitStruct.CAN_StandardRemoteFrame		= CanFilterDisposal_Filter;	/* 过滤标准帧*/
		CAN_GlobalFilterInitStruct.CAN_ExtendedRemoteFrame		= CanFilterDisposal_Filter;	/* 过滤扩展帧*/
		CAN_GlobalFilterInit(CAN1, &CAN_GlobalFilterInitStruct);	

		CAN_RXFIFOInitStruct.CAN_RXFIFO_Mode  = CAN_FIFOMode_Overwrite;				/* 接收FIFO为覆盖模式 */						
		CAN_RXFIFOInitStruct.CAN_RXFIFO_Depth = 8;														/* 接收FIFO深度配置 */		
		CAN_RXFIFOInitStruct.CAN_RXFIFO_Width = CAN_FIFOWidth_8BYTES;					/* 接收FIFO宽度配置 */		
		CAN_RXFIFOInitStruct.CAN_RXFIFO_ThresholdAlarmLine = 8;								/* 接收FIFO阈值配置 */		
		CAN_RXFIFOInitStruct.CAN_RXFIFO_BaseAddress = 0xC080;									/* 接收FIFO首地址 */		
		CAN_ReceiveFIFOConfig(CAN1, CAN_FIFO_0, &CAN_RXFIFOInitStruct);

		CAN_TXSequenceInitStruct.CAN_TXSequence_mode 	= CAN_TXSequence_FIFO;	/* 发送为FIFO模式 */				
		CAN_TXSequenceInitStruct.CAN_TXSequence_Depth = 8;										/* 发送FIFO深度配置 */			
		CAN_TXSequenceInitStruct.CAN_TXSequence_StartAddress = 0xCDC0;				/* 发送FIFO首地址配置 */			
		CAN_SendSequenceConfig(CAN1, &CAN_TXSequenceInitStruct);	
	#endif
	
	CAN_ITConfig(CAN1, CAN_IT_TC, CAN_INT0, ENABLE);												/* CAN 发送完成中断 */
	CAN_ExitInitMode(CAN1);																									/* CAN退出初始化 */
	
	SIDFMsg.FilterType 		 = CAN_FilterType_SFID1andSFID2;									/* SFID1/2双滤波 */
	#if TEST_BUFFER_FIFO	
	SIDFMsg.FilterDisposal = CAN_FilterDisposal_BUFFER;											/* 过滤匹配时存于BUFFER */	
	SIDFMsg.SFID1					 = 0xA5;																					/* 标准过滤ID1 为0xA5 	*/
	SIDFMsg.SFID2					 = 0;																							/* 当BUFFER接收模式时，SFID2为BUFFER存入下标*/
	for(i = 0; i < 8; i++)
	{
		CAN_CopySIDFilterMessageToRAM(CAN1, CAN_FILTER_BASE  + i*4, &SIDFMsg);	
		SIDFMsg.SFID2 ++;
	}	
	#else
	SIDFMsg.FilterDisposal = CAN_FilterDisposal_FIFO0;											/* 过滤匹配时存于FIFO0 */			
	SIDFMsg.SFID1					 = 0xA5;																					/* 标准过滤ID1 为0xA5 	*/
	SIDFMsg.SFID2					 = 0;																							/* 当FIFO接收模式时，	SFID2为标准过滤ID2*/
	for(i = 0; i < 8; i++)
	{
		CAN_CopySIDFilterMessageToRAM(CAN1, CAN_FILTER_BASE  + i*4, &SIDFMsg);	
	}	
	#endif

	TxMsg.TypeId 		 = CAN_StdId;																						/* 发送帧为标准帧 */
	TxMsg.TypeFrame  = CAN_DataFrame;																				/* 数据帧 */
	TxMsg.ID				 = 0xA5;																								/* 标准数据帧ID */
	TxMsg.DLC				 = 8; 																									/* 标准数据长度 */
	TxMsg.Data[0]		 = 0x11223344;																					/* 标准数据内容 */
	TxMsg.Data[1]		 = 0x55667788;																					/* 标准数据内容 */
	TxMsg.FdMode		 = DISABLE;																							/* 经典CAN协议 非CANFD */
	TxMsg.BitRateSwitch  = DISABLE;																					/* 比特速率切换关闭 */
	TxMsg.EventFIFOStore = DISABLE;																					/* 不保存发送事件 */
	TxMsg.MesMarkerL = 0;																										/* 发送事假消息标志 */
	TxMsg.MesMarkerH = 0;
	CAN_CopyTxMessageToRAM(CAN1, CAN_TxBUFFER_BASE, &TxMsg);								/* 将标准帧数据填充至RAM */ 
	
	/* 将其余7个标准帧数据填充至RAM */ 
	for(i = 1; i < 8; i++)
	{
		TxMsg.Data[0]		 = 0x11223344 + i*2;
		TxMsg.Data[1]		 = 0x55667788 + i*2;
		CAN_CopyTxMessageToRAM(CAN1, (CAN_TxBUFFER_BASE + i*4*4), &TxMsg);
	}

	CAN_RequestConfig(CAN1, CAN_RequestType_Transfer, CAN_Index_ALL);					/* 发送请求 */
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
	//CAN
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1, ENABLE);
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
	
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_2 | GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;							/* 复用模式 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;							/* 推挽模式 */
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL; 					/* 内部无上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;						/* 高速输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_8);				/* 复用功能配置为 CAN RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_8);				/* 复用功能配置为 CAN TX */
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
	RCC_PCLK1Config(RCC_HCLK_Div1);												/* 设置PCLK1时钟分频 */
	RCC_PCLK2Config(RCC_HCLK_Div1);												/* 设置PCLK2时钟分频 */
	
	RCC_PLLConfig(RCC_PLLSource_HSE,8,192,2,4);    				/* PLL时钟配置为96MHz  M N P Q */
		
	RCC_PLCANFDConfig(PLLCANFDSource_PLLR);								/* 设置CANFD时钟源 来自PLL.VCO */
	RCC_PLLRConfig(RCC_PLLCFGR_PLLR_4);										/* 设置I2S时钟源分频 PLL.VCO / PLLR = 192/4 = 48MHz */									
		
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

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_INT0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;		/* 设置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;						/* 设置响应优先级 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									/* 中断通道使能 */
  NVIC_Init(&NVIC_InitStructure);																	/* 中断通道初始化 */
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
