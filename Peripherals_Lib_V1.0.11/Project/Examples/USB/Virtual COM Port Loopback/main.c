/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块(USB)
*	文件名称 : main.c
*	说    明 :
*            此样例演示芯片USB 虚拟串口的功能
*
*            适用芯片: HC32F401系列
*             实验步骤：
*							PC端->串口调试助手->USB连接
*              1. 编译并下载程序
*              2. 系统/GPIO初始化 
*              3. USB/中断初始化
*              4. 待串口调试助手发送任意数据 
*              5. USB回传相同数据 
*
*	注意事项：
*						
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"


extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;

uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;


/* 函数声明 -----------------------------------------------*/
void RCC_Configuration(void);			/* 时钟配置函数 */
void SYS_Configuration(void);			/* 系统时钟配置函数 */


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
	/* 使能外设时钟(GPIOA/B/C/D) PWR */
	RCC_Configuration();	
	
	/* 系统时钟配置为168MHz */
	SYS_Configuration();

	/* USB GPIO初始化 */
  Set_System();
	
	/* USB 时钟源配置 */ 
  Set_USBClock();
	
	/* USB 中断配置 */ 
  USB_Interrupts_Config();
	
	/* USB 初始化 */
  USB_Init();
  
  while (1)
  {
		/* 轮询USB是否连接 */
    if (bDeviceState == CONFIGURED)
    {
			/* 获取接收数据 */
      CDC_Receive_DATA();
			
			/* 若获取成功，则回传*/
      if (Receive_length  != 0)
      {
        if (packet_sent == 1)
          CDC_Send_DATA ((unsigned char*)Receive_Buffer, Receive_length);
        Receive_length = 0;
      }
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	
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
	
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,7);    				/* PLL时钟配置为168MHz  M N P Q */ 
		
	RCC_PLLCmd(ENABLE);   																/* PLL使能 */
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}; /* PLL倍频就绪完成 */  
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);    				/* 系统时钟切换为PLL时钟源 */  
	while (RCC_GetSYSCLKSource() != 0x08){}    						/* 等待系统时钟切换完成 */ 
}
/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/

