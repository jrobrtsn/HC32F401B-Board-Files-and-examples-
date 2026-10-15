/*
*********************************************************************************************************
*
*	模块名称 : 窗口看门狗(WWDG)模块
*	文件名称 : main.c
*	说    明 :
*            此样例演示了窗口看门狗(WWDG)的功能。
*            WWDG是一个7位递减计数器(0x7F~0x3F)，要求在特定"窗口"内喂狗:
*            太早(>0x5F)或太晚(<0x3F)喂狗都会触发复位。
*            利用EWI提前警告中断在0x40时重载计数器，PA0每次翻转指示喂狗完成。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 观察PA0是否周期性翻转(约6.14ms周期)
*                3. 若PA0停止翻转，说明WWDG因时序异常触发复位
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     /* 系统时钟配置函数(HSE+PLL→168MHz) */
void RCC_Configuration(void);     /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);    /* GPIO引脚配置函数(PA0推挽输出) */
void WWDG_Configuration(void);    /* WWDG窗口看门狗配置函数(窗口值0x5F, 初值0x7F) */
void NVIC_Configuration(void);    /* 中断控制器配置函数(WWDG提前警告中断) */

/* 全局变量 -----------------------------------------------*/
uint8_t i;                        /* 循环计数变量 */

/*
*********************************************************************************************************
*	函 数 名: WWDG_IRQHandler
*	功能说明: WWDG提前警告中断服务函数(EWI, Early Wakeup Interrupt)。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void WWDG_IRQHandler()
{
    /*
     * 当WWDG计数器递减至0x40时触发此中断，此时距复位(0x3F)仅剩1个计数。
     * 在中断中清除标志、重载计数器并翻转PA0引脚。
     * 注意: 必须在计数器降到0x3F之前重载，否则WWDG复位。
     */

    /* 清除WWDG提前警告中断标志 */
    WWDG_ClearFlag();

    /* 重载WWDG计数器为0x7F(127)，重新开始递减 */
    WWDG_SetCounter(0x7F);			

    /* 翻转PA0引脚，指示一次喂狗完成 */
    GPIO_ToggleBits(GPIOA, GPIO_Pin_0);
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 主程序入口函数。
*	形    参：无
*	返 回 值: 无返回(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    /* 使能外设时钟(GPIOA~GPIOD) */
    RCC_Configuration();

    /* 配置系统时钟(HSE + PLL = 168MHz) */
    SYS_Configuration();

    /* 配置GPIO(PA0推挽输出) */
    GPIO_Configuration();

    /* 配置WWDG窗口看门狗(预分频=1, 窗口=0x5F, 初值=0x7F) */
    WWDG_Configuration();

    /* 配置NVIC(WWDG中断优先级) */
    NVIC_Configuration();

    /* 主循环: 死循环，所有动作在WWDG_IRQHandler中断中完成 */
    while(1)
    {	
    }
}


/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
    /* 使能GPIOA~GPIOD的AHB1总线时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

    /* 使能WWDG的APB1总线时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: WWDG_Configuration
*	功能说明: 窗口看门狗(WWDG)配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void WWDG_Configuration(void)
{
    /*
     * WWDG是一个7位递减计数器，基于PCLK1时钟经固定4096分频后运行。
     *
     * 时钟计算:
     *   WWDG_CLK = PCLK1 / 4096 / Prescaler
     *            = 42MHz / 4096 / 1
     *            ≈ 10254 Hz (约97.5μs/计数)
     *
     * 窗口机制:
     *   - 计数器范围: 0x7F(127) → 0x3F(63)，7位递减
     *   - 窗口上限(0x5F=95): 计数器>0x5F时喂狗 → 触发复位(喂太早)
     *   - 窗口下限(0x3F=63): 计数器<0x3F时 → 触发复位(喂太晚/未喂)
     *   - 有效喂狗窗口: 0x5F ≥ 计数器 ≥ 0x40(64)
     *   - EWI提前警告中断: 计数器=0x40时触发
     *
     */

    /* 设置WWDG预分频 = 1 */
    /* 计数时钟 = 42MHz / 4096 / 1 ≈ 10.25kHz
			每步时间 = 1/10254 ≈ 97.5μs
	
			计数器递减过程：
			起始值 0x7F = 127  →  EWI 中断值 0x40 = 64

			每次循环递减步数 = 127 - 64 = 63 步
			一次喂狗间隔 = 63 × 97.5μs ≈ 6.14m
		*/
    WWDG_SetPrescaler(WWDG_Prescaler_1);

    /* 设置WWDG窗口上限值 = 0x5F (95) */
    /* 计数器值 > 0x5F 时不可喂狗(喂太早会被拒绝并触发复位) */
    WWDG_SetWindowValue(0x5F);

    /* 使能WWDG提前警告中断(EWI) */
    /* 当计数器降到0x40时触发中断，在此中断中喂狗 */
    WWDG_EnableIT();

    /* 使能WWDG并设置计数器初值 = 0x7F (127) */
    /* 使能后计数器立即从0x7F开始递减 */
    WWDG_Enable(0x7F);
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

    /*--- PA0: 推挽输出(喂狗指示，每次喂狗翻转一次) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;         /* 选择PA0引脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      /* 输出模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   /* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;    /* 高速模式 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);
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
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET) {};

    /* 关闭PLL，配置AHB及APB总线时钟分频 */
    RCC_PLLCmd(DISABLE);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     /* AHB总线时钟(HCLK) = SYSCLK / 1 = 168MHz */
    RCC_PCLK1Config(RCC_HCLK_Div4);      /* APB1总线时钟(PCLK1) = HCLK / 4 = 42MHz */
    RCC_PCLK2Config(RCC_HCLK_Div2);      /* APB2总线时钟(PCLK2) = HCLK / 2 = 84MHz */

    /* 配置PLL: PLLCLK = HSE(8MHz) / 8(M) × 336(N) / 2(P) = 168MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 2, 8);    /* PLL时钟配置: 时钟源=HSE, M=8, N=336, P=2, Q=8 */

    /* 使能PLL并等待锁定 */
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {};

    /* 将系统时钟源切换为PLL输出 */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08) {}  /* 等待系统时钟切换完成(0x08表示PLL已为时钟源) */
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

    /* 配置WWDG中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;                      /* WWDG中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
