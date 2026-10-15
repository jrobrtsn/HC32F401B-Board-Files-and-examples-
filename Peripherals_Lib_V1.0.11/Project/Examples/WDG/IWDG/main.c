/*
*********************************************************************************************************
*
*	模块名称 : 独立看门狗(IWDG)模块
*	文件名称 : main.c
*	说    明 :
*            此样例演示了独立看门狗(IWDG)的功能。
*            IWDG基于LSI时钟(标称40kHz, 实测约29.4kHz)独立运行，上电后PA0翻转20次指示启动，
*            之后在死循环中通过PA1按键喂狗。若不喂狗，约1.113s后IWDG超时复位系统(LSI偏差导致, 标称值819ms)。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 上电后观察PA0翻转20次(启动指示)
*                3. 不按PA1键，观察芯片约1.113s后自动复位(标称819ms, LSI偏差)
*                4. 按PA1键喂狗，观察是否能阻止复位
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     /* 系统时钟配置函数(HSE+PLL→168MHz) */
void RCC_Configuration(void);     /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);    /* GPIO引脚配置函数(PA0推挽输出, PA1上拉输入) */
void IWDG_Configuration(void);    /* IWDG独立看门狗配置函数(实测约1.113s超时) */

/* 全局变量 -----------------------------------------------*/
uint8_t i;                        /* 循环计数变量(PA0翻转20次用) */

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

    /* 配置GPIO(PA0推挽输出, PA1上拉输入) */
    GPIO_Configuration();

    /*
     * 配置IWDG独立看门狗并启动
     * prv=1(4×2^1=8分频), rlv=0x0FFF(4095)
     * 理论Tout≈819ms(f_lsi=40kHz), 实测≈1.113s(f_lsi≈29.4kHz)
     */
    IWDG_Configuration();

    /* 启动指示: PA0翻转20次，用于演示IWDG复位后程序会重新执行 */
    for(i = 0; i < 20; i++)
        GPIO_ToggleBits(GPIOA, GPIO_Pin_0);  /* 翻转PA0引脚电平 */

    /* 主循环: 检测PA1按键，按下则喂狗 */
    while(1)
    {
        /* 检测PA1是否为低电平(按键按下) */
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_RESET)
        {
            /* 重载IWDG计数器(喂狗)，防止看门狗复位 */
            IWDG_ReloadCounter();
        }
        /* 若PA1未按下，不喂狗 → IWDG超时 → 系统复位 */
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
    /* IWDG使用LSI独立时钟，不需要额外使能总线时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: IWDG_Configuration
*	功能说明: 独立看门狗(IWDG)配置函数。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IWDG_Configuration(void)
{
    /*
     * IWDG使用LSI内部低速RC振荡器作为时钟源(标称40kHz)，
     * 独立于系统时钟，即使系统时钟停止也能正常工作。
     * 一旦使能IWDG，软件无法关闭，只有系统复位才能停止。
     *
     * 超时时间计算公式:
     *   Tout = (4 × 2^prv) / f_lsi × rlv  (ms)
     *   prv = 预分频器寄存器值(PR[2:0]), rlv = 重装载寄存器值(RL[11:0])
     *   f_lsi = LSI实际频率(kHz),40kHz--30KHZ-60khz变化。
     *
     *   prv与分频系数对照:
     *     prv=0: 4×2^0=4     prv=1: 4×2^1=8  (当前配置)
     *     prv=2: 4×2^2=16    prv=3: 4×2^3=32
     *     prv=4: 4×2^4=64    prv=5: 4×2^5=128
     *     prv=6: 4×2^6=256
     *   注意: LSI精度约±30%, 实际超时以实测为准, 不可用于精确定时
     */
	
    /* 使能IWDG写访问(解锁寄存器保护) */
    /* 必须先解锁才能修改预分频和重载值 */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* 设置IWDG预分频: prv=1 → 4×2^1 = 8分频 */
    IWDG_SetPrescaler(IWDG_Prescaler_8);

    /* 设置IWDG重载值 rlv = 0x0FFF (4095) */
    IWDG_SetReload(0x0FFF);

    /* 首次重载计数器(启动计数) */
    IWDG_ReloadCounter();

    /* 使能IWDG(使能后不可软件关闭，只有复位才能停止IWDG) */
    IWDG_Enable();
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

    /*--- PA0: 推挽输出(启动/复位指示) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;         /* 选择PA0引脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;      /* 输出模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      /* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   /* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;    /* 高速模式 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*--- PA1: 上拉输入(按键，低电平有效，按下喂狗) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;         /* 选择PA1引脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;       /* 输入模式 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;       /* 内部上拉(未按下时为高，按下为低) */
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

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
