/*
*********************************************************************************************************
*
*	模块名称 : 电压比较器模块(VC Base)
*	文件名称 : main.c
*	说    明 :
*            本程序演示了电压比较器(VC, Voltage Comparator)的基础功能。
*            VC0 配置为模拟比较器模式，比较正向输入端(INP_CH1: PC9)和
*            反向输入端(INN_CH0: PC8)的电压。
*
*            当正向输入电压 > 反向输入电压时，VC0 输出高电平，触发上升沿中断。
*            中断服务函数中清除中断标志，用户可在中断内设置断点观察比较结果。
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 将PC8(VC0_INN_CH0)连接到参考电压源(如GND或分压电阻)
*                3. 将PC9(VC0_INP_CH1)连接到被测电压信号
*                4. 在VC0_IRQHandler中断函数内设断点，观察何时触发比较中断
*                5. 改变输入电压极性，观察中断触发情况
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     /* 系统时钟配置函数(HSE+PLL: 168MHz) */
void RCC_Configuration(void);     /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);    /* GPIO管脚配置函数(VC0输入/输出管脚) */
void VC_Configuration(void);      /* VC0电压比较器配置函数(基础比较模式) */
void NVIC_Configuration(void);    /* 中断控制器配置函数(VC0中断) */


/*
*********************************************************************************************************
*	函 数 名: VC0_IRQHandler
*	功能说明: VC0电压比较器中断服务函数。
*             当VC0正向输入电压超过反向输入电压(上升沿触发)时进入此中断。
*             函数内清除中断标志，用户可在此设断点或添加自定义处理逻辑。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void VC0_IRQHandler()
{
    /*
     * 当VC0正向输入端电压(INP_CH1)超过反向输入端电压(INN_CH0)时，
     * 触发上升沿中断。实际应用中可在此处添加后续处理逻辑，
     * 如启动ADC采样、触发保护动作、记录事件等。
     */

    /* 清除VC0中断标志(包括INTF和FLTV标志) */
    VC_ClearFlag(VC0);
	
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 主程序入口函数。
*             初始化完成后进入空循环，所有比较工作由VC0硬件自动完成，
*             比较结果通过中断通知CPU。
*	形    参: 无
*	返 回 值: 无返回值(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    /* 使能外设时钟(GPIOA~D, DMA1, VC0~2) */
    RCC_Configuration();

    /* 配置系统时钟(HSE + PLL = 168MHz) */
    SYS_Configuration();

    /* 配置GPIO(VC0输入/输出复用管脚) */
    GPIO_Configuration();

    /* 配置VC0电压比较器(INN=CH0, INP=CH1, 上升沿中断) */
    VC_Configuration();

    /* 配置NVIC(VC0中断优先级) */
    NVIC_Configuration();

    /*
     * 主循环: VC0比较器由硬件自动运行，无需CPU干预。
     * 当VC_INP > VC_INN时，硬件自动触发VC0_IRQHandler中断。
     * 用户可在此循环中添加其他任务处理。
     */
    while(1)
    {
    }
}


/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数。
*             使能GPIOA~D的AHB1总线时钟、DMA1的AHB1总线时钟，
*             以及VC0~2的APB2总线时钟。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
    /* 使能GPIOA~GPIOD和DMA1的AHB1总线时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

    /* 使能VC0、VC1、VC2的APB2总线时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_VC0 | RCC_APB2Periph_VC1 | RCC_APB2Periph_VC2, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO管脚配置函数。
*             VC0_OUT输出管脚: PA3, 复用功能AF11(VC0_OUT)。
*             VC0_INN_CH0反向输入: PC8, 模拟输入模式。
*             VC0_INP_CH1正向输入: PC9, 模拟输入模式。
*
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*--- PA3: VC0输出管脚(复用功能AF11) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;          /* 选择PA3管脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;        /* 复用功能模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       /* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    /* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;     /* 高速输出模式 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 将PA3映射到AF11: VC0_OUT(电压比较器0输出) */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_11);

    /*--- PC8/PC9: VC0模拟输入通道(INN_CH0, INP_CH1) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;  /* 选择PC8和PC9管脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;              /* 模拟输入模式(ADC/VC模拟通道) */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            /* 推挽输出(模拟模式此项可忽略) */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;         /* 无内部上下拉(模拟输入要求) */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;          /* 高速模式 */
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/*
*********************************************************************************************************
*	函 数 名: VC_Configuration
*	功能说明: VC0电压比较器基础配置函数。
*             配置VC0为模拟比较器模式:
*               - 反向输入: INN_CH0 (PC8)
*               - 正向输入: INP_CH1 (PC9)
*               - 输出极性: 同相输出(INP>INN时输出高电平)
*               - 迟滞: 禁用(提高灵敏度)
*               - 响应速度: 高速(响应快但功耗稍高)
*               - 数字滤波: 关闭(滤波时钟=PCLK, 预分频=OFF)
*               - 中断: 上升沿触发(INP超过INN时触发)
*
*             输入通道可选说明:
*               INN(反向)可选: CH0~CH4, DAC_OUT, 1.2V内部基准, VDDA/3, 温度传感器, MID
*               INP(正向)可选: CH0~CH4, DAC_OUT, FDA0_INP/OUTP, FDA1_INP/OUTP
*
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void VC_Configuration(void)
{
    VC_InitTypeDef  VC_InitStructure;

    /* VC0比较器基本参数配置 */
    VC_InitStructure.VC_INN_CH         = VC_INN_CH0;               /* 反向输入端: CH0 (PC8, 接参考电压) */
    VC_InitStructure.VC_INP_CH         = VC_INP_CH1;               /* 正向输入端: CH1 (PC9, 接被测信号) */
    VC_InitStructure.VC_OutPolarity    = VC_OutPolarity_NonInverted; /* 输出极性: (VINP>VINN时输出1) */
    VC_InitStructure.VC_Hys            = VC_Hysteresis_DISABLE;    /* VC_Hysteresis_DISABLE 迟滞: 禁用(输入差值很小时也能翻转, 灵敏度高) */
    VC_InitStructure.VC_ResponseSpeed  = VC_ResponseSpeed_High;    /* 响应速度: 高速(传播延迟小, 适合快速信号检测) */
    VC_InitStructure.VC_FilterClkSoure = VC_FilterClkSoure_PCLK;   /* 滤波时钟源: PCLK(APB2=84MHz) */
    VC_InitStructure.VC_FilterPrescaler = VC_FilterPSC_OFF;        /* 滤波预分频: 关闭数字滤波功能 */
    VC_Init(VC0, &VC_InitStructure);
	
	
		VC_ClearFlag(VC0);
    /*
     * 使能VC0上升沿中断。
     * 可选中断类型:
     *   VC_IT_Rising    - 上升沿触发(INP超过INN时)
     *   VC_IT_Falling   - 下降沿触发(INP低于INN时)
     *   VC_IT_HighLevel - 高电平触发
     */
    VC_ITConfig(VC0, VC_IT_Rising, ENABLE);

    /* 使能VC0模块(使能后比较器开始工作) */
    VC_Cmd(VC0, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SYS_Configuration(void)
{
    /* 开启外部高速晶振HSE(8MHz) */
    RCC_HSEConfig(RCC_HSE_ON);

    /* 等待HSE起振稳定 */
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){};

    /* 关闭PLL，配置AHB和APB总线时钟分频 */
    RCC_PLLCmd(DISABLE);
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     /* AHB总线时钟(HCLK) = SYSCLK / 1 = 168MHz */
    RCC_PCLK1Config(RCC_HCLK_Div4);      /* APB1总线时钟(PCLK1) = HCLK / 4 = 42MHz */
    RCC_PCLK2Config(RCC_HCLK_Div2);      /* APB2总线时钟(PCLK2) = HCLK / 2 = 84MHz */

    /* 配置PLL: PLLCLK = HSE(8MHz) / M(8) × N(336) / P(2) = 168MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    /* PLL时钟配置: 时钟源=HSE, M=8, N=336, P=2, Q=8 */

    /* 使能PLL并等待锁定 */
    RCC_PLLCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};

    /* 将系统时钟源切换为PLL输出 */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08){}  /* 等待系统时钟切换完成(0x08表示PLL作为时钟源) */
}


/*
*********************************************************************************************************
*	函 数 名: NVIC_Configuration
*	功能说明: 嵌套向量中断控制器(NVIC)配置函数。
*             配置VC0中断通道的抢占优先级和子优先级。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef   NVIC_InitStructure;

    /* 配置VC0中断优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = VC0_IRQn;                       /* VC0中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
