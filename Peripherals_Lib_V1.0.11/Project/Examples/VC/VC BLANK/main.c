/*
*********************************************************************************************************
*
*	模块名称 : 电压比较器模块(VC BLANK - 输出消隐功能)
*	文件名称 : main.c
*	说    明 :
*            本程序演示了电压比较器(VC)的输出消隐(BLANK)功能。
*            VC消隐功能可以在特定时间段内屏蔽比较器的输出变化，
*            常用于抑制PWM开关电源等应用中的开关噪声干扰。
*
*            工作原理:
*              TIM1产生两路PWM信号(CH1/CH2)，同时TIM1的比较输出
*              参考信号(OC1REFC/OC2REFC)作为VC的消隐触发源。
*              当消隐信号有效时，VC输出被强制锁定为消隐极性电平，
*              此时即使输入信号有毛刺也不会触发误中断。
*
*            本示例配置:
*              - TIM1 CH1(PC5, AF1): PWM输出，CCR1=21000(占空比50%)
*              - TIM1 CH2(PC7, AF1): PWM输出，CCR2=10500(占空比25%)
*              - VC0 INN_CH0(PC8): 反向输入端，接参考电压
*              - VC0 INP_CH1(PC9): 正向输入端，接被测信号
*              - VC0_OUT(PA3): 比较器输出
*              - 消隐源: TIM1 OC1REFC(定时器输出参考信号)
*              - 消隐极性: 低电平消隐(消隐期间VC输出强制为低)
*              - 消隐窗口: BLANKTIME=64个PCLK ≈ 762ns
*
*            适用芯片: HC32F401系列
*              实验步骤：
*                1. 请看Readme.txt
*                2. 将PC8(VC0_INN_CH0)连接到参考电压源
*                3. 将PC9(VC0_INP_CH1)连接到被测电压信号
*                4. 用示波器同时观察PC5(TIM1_CH1/PWM)和PA3(VC0_OUT)
*                   - 在PWM边沿附近(噪声区)，VC输出被消隐锁定，不受干扰
*                   - 在非消隐区，VC正常比较输入信号
*                5. 在VC0_IRQHandler和TIM1_CC_IRQHandler中断内设断点调试
*                6. 调整TIM1 CCR1值可改变消隐窗口触发位置(宽度由BLANKTIME决定)
*
*	Copyright (C), 2009-2026, 芯圣电子  www.holychip.cn
*
*********************************************************************************************************
*/
#include "main.h"

/* 函数声明 -----------------------------------------------*/
void SYS_Configuration(void);     /* 系统时钟配置函数(HSE+PLL: 168MHz) */
void RCC_Configuration(void);     /* 外设时钟使能配置函数 */
void GPIO_Configuration(void);    /* GPIO管脚配置函数(VC0+TIM1管脚) */
void VC_Configuration(void);      /* VC0电压比较器配置函数(含消隐功能) */
void TIM_Configuration(void);     /* TIM1定时器配置函数(产生PWM+消隐触发源) */
void NVIC_Configuration(void);    /* 中断控制器配置函数(VC0+TIM1中断) */


/*
*********************************************************************************************************
*	函 数 名: VC0_IRQHandler
*	功能说明: VC0电压比较器中断服务函数。
*             当VC0比较结果翻转(且不在消隐期内)时触发中断。
*             消隐期内VC输出被锁定，即使输入翻转也不会触发此中断。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void VC0_IRQHandler()
{
    /*
     * VC0中断触发条件(与VC_ITConfig配置相关):
     *   - 在非消隐期内，INP电压从低于变为高于INN(上升沿)时触发
     *   - 消隐期内VC输出被硬件锁定，不会触发中断
     *   - 消隐窗口由TIM1的OC1REFC信号控制
     *
     * 本示例中仅清除中断标志。实际应用可在消隐窗口之外
     * 捕获有效的比较结果，避免PWM开关边沿的毛刺干扰。
     */

    /* 清除VC0中断标志(INTF和FLTV标志位) */
    VC_ClearFlag(VC0);
}


/*
*********************************************************************************************************
*	函 数 名: TIM1_CC_IRQHandler
*	功能说明: TIM1捕获/比较中断服务函数。
*             处理TIM1 CH1(CC1)和CH2(CC2)的比较匹配事件。
*             这两个通道的比较输出作为VC消隐触发源。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM1_CC_IRQHandler()
{
    /*
     * 检查并清除TIM1 CH1(CC1)比较匹配中断标志。
     * 当TIM1计数器CNT = CCR1(21000)时触发，对应PWM占空比50%位置。
     */
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
    }

    /*
     * 检查并清除TIM1 CH2(CC2)比较匹配中断标志。
     * 当TIM1计数器CNT = CCR2(10500)时触发，对应PWM占空比25%位置。
     */
    if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    }
}


/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 主程序入口函数。
*             初始化VC0消隐功能、TIM1 PWM/消隐触发源，然后进入空循环。
*             所有比较和消隐操作由VC0+TIM1硬件协同完成。
*	形    参: 无
*	返 回 值: 无返回值(无需处理)
*********************************************************************************************************
*/
int main(void)
{
    /* 使能外设时钟(GPIOA~D, DMA1, TIM1, VC0~2) */
    RCC_Configuration();

    /* 配置系统时钟(HSE + PLL = 168MHz) */
    SYS_Configuration();

    /* 配置GPIO(VC0模拟输入+输出, TIM1 PWM输出管脚) */
    GPIO_Configuration();

    /*
     * 配置TIM1: 先配置TIM1再配置VC，确保消隐触发源已就绪。
     * TIM1 CH1/CH2产生PWM输出，同时OC1REFC作为VC消隐触发源。
		*/
    TIM_Configuration();

    /*
     * 配置VC0: 比较器参数 + 消隐功能。
     * 消隐源 = TIM1 OC1REFC, 消隐极性 = 低电平, 消隐时钟 = 64分频。
     */
    VC_Configuration();

    /* 配置NVIC(VC0中断 + TIM1 CC中断优先级) */
    NVIC_Configuration();

    /*
     * 主循环: 比较器与消隐功能由硬件自动协同工作。
     * - TIM1持续运行，产生PWM输出和消隐触发参考信号
     * - VC0根据消隐窗口自动屏蔽/允许输出翻转
     * - 消隐期内VC输出被锁定，消隐期外正常比较
     * - 中断服务函数中可添加自定义处理
     */
    while(1)
    {
    }
}


/*
*********************************************************************************************************
*	函 数 名: RCC_Configuration
*	功能说明: 外设时钟使能配置函数。
*             使能GPIOA~D和DMA1的AHB1总线时钟，
*             以及TIM1和VC0~2的APB2总线时钟。
*             TIM1和VC均挂在APB2总线上，时钟为84MHz。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RCC_Configuration(void)
{
    /* 使能GPIOA~GPIOD和DMA1的AHB1总线时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1, ENABLE);

    /* 使能TIM1和VC0~2的APB2总线时钟(APB2 = 84MHz) */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_VC0 | RCC_APB2Periph_VC1 | RCC_APB2Periph_VC2, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: GPIO_Configuration
*	功能说明: GPIO管脚配置函数。
*             TIM1 PWM输出管脚:
*               - CH1: PC5, 复用功能AF1(TIM1_CH1)
*               - CH2: PC7, 复用功能AF1(TIM1_CH2)
*             VC0 管脚:
*               - VC0_OUT: PA3, 复用功能AF11
*               - INN_CH0: PC8, 模拟输入
*               - INP_CH1: PC9, 模拟输入
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*--- PC5/PC7: TIM1 PWM输出管脚(CH1, CH2) ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7;  /* 选择PC5和PC7管脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;              /* 复用功能模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             /* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;          /* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;           /* 高速输出模式 */
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 将PC5映射到AF1: TIM1_CH1(PWM输出通道1) */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_1);
    /* 将PC7映射到AF1: TIM1_CH2(PWM输出通道2) */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_1);

    /*--- PA3: VC0输出管脚(复用功能AF11), 可外接示波器观察比较/消隐波形 ---*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;           /* 选择PA3管脚 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;         /* 复用功能模式 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        /* 推挽输出 */
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;     /* 无内部上下拉 */
    GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;      /* 高速输出模式 */
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
*	功能说明: VC0电压比较器配置函数(含输出消隐功能)。
*             在基础比较器功能之上，增加BLANK消隐配置:
*               - 消隐输出极性: 低电平(VBlank期间输出强制拉低)
*               - 消隐时钟分频: VC_BlankCLK_64(消隐时间基准时钟)
*               - 消隐触发源: TIM1 OC1REFC(定时器1通道1输出参考信号)
*
*             消隐时序:
     *               消隐窗口时长 = 2^(BLANKTIME+2) 个PCLK周期 = 64个PCLK ≈ 762ns
     *               窗口由OC1REFC上升沿触发启动, 时长由BLANKTIME固定
     *               (CCR1匹配决定OC1REFC翻转时刻 = 窗口触发位置)
*
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void VC_Configuration(void)
{
    VC_InitTypeDef  VC_InitStructure;

    /* VC0比较器基本参数配置*/
    VC_InitStructure.VC_INN_CH         = VC_INN_CH0;               /* 反向输入端: CH0 (PC8, 接参考电压) */
    VC_InitStructure.VC_INP_CH         = VC_INP_CH1;               /* 正向输入端: CH1 (PC9, 接被测信号) */
    VC_InitStructure.VC_OutPolarity    = VC_OutPolarity_NonInverted; /* 输出极性: 同相(VINP>VINN时输出1) */
    VC_InitStructure.VC_Hys            = VC_Hysteresis_DISABLE;    /* 迟滞: 禁用(灵敏度最高) */
    VC_InitStructure.VC_ResponseSpeed  = VC_ResponseSpeed_High;    /* 响应速度: 高速(传播延迟约几十ns) */
    VC_InitStructure.VC_FilterClkSoure = VC_FilterClkSoure_PCLK;   /* 滤波时钟源: PCLK(APB2=84MHz) */
    VC_InitStructure.VC_FilterPrescaler = VC_FilterPSC_OFF;        /* 滤波预分频: 关闭数字滤波 */
    VC_Init(VC0, &VC_InitStructure);

    /*
     * 配置VC0输出消隐(Blank)功能:
     *
     * VC_BlankConfig参数说明:
     *   参数1: VC_BlankOutPolarity_Low  - 消隐期间输出强制为低电平
     *           (可选High: 消隐期间输出强制为高电平)
     *   参数2: VC_BlankCLK_64           - 消隐窗口时长 = 2^(BLANKTIME+2) 个PCLK周期
     *            本例 BLANKTIME=100b → 64个PCLK(84MHz) ≈ 762ns
     *            可选: 4/8/16/32/64/128/256/512 个PCLK
     *   参数3: VC_TriggerSource_TIM1OC1REFC - 消隐触发源 = TIM1 CH1输出参考信号
     *            每次OC1REFC上升沿触发一个固定时长的消隐窗口
     *            可选源: OC1REFC~OC4REFC(对应CR1.BLANKCH1~BLANKCH4)
     *
     * 消隐窗口特性:
     *   - 窗口时长由BLANKTIME固定, 与CCR1无关
     *   - CCR1只决定OC1REFC翻转时刻(=窗口触发位置)
     *   - 中心对齐模式下OC1REFC每周期翻转两次 → 每500us触发一次消隐
     */
			VC_BlankConfig(VC0, VC_BlankOutPolarity_Low, VC_BlankCLK_64, VC_TriggerSource_TIM1OC1REFC);

    /*
     * 使能VC0上升沿中断。
     * 注意: 消隐期间VC输出被锁定, 即使输入信号翻转也不会触发中断。
     * 中断仅在非消隐期内、且INP > INN时触发。
     */
    VC_ITConfig(VC0, VC_IT_Rising, ENABLE);

    /* 使能VC0模块(使能后比较器+消隐功能同时生效) */
    VC_Cmd(VC0, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: TIM_Configuration
*	功能说明: TIM1定时器配置函数。
*             配置TIM1中心对齐PWM模式，产生两路PWM输出并作为VC消隐触发源。
*
*             时序参数:
*               TIM1时钟 = PCLK2 = 84MHz
*               预分频器(PSC) = 1 → 计数器时钟 = 84MHz / (1+1) = 42MHz
*               自动重装载(ARR) = 42000
*               PWM周期 = 42000 / 42MHz = 1ms (频率 = 1kHz)
*               CCR1 = 21000 → CH1占空比 = 50%
*               CCR2 = 10500 → CH2占空比 = 25%
*
*             消隐触发关系:
     *               TIM1 OC1REFC信号在CNT=CCR1匹配时翻转(上升沿)
     *               → 触发VC0进入固定时长(64 PCLK)的消隐窗口
     *               → 改CCR1调窗口触发位置, 改BLANKTIME才能调窗口宽度
*
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure;

    /*
     * TIM1时基配置(中心对齐模式1):
     *   中心对齐模式: 计数器从0递增到ARR再递减到0，形成对称三角波
     *   优点: PWM边沿对称分布，适合电机控制、消隐窗口等应用
     *   CK_CNT = PCLK2 / (PSC + 1) = 84MHz / 2 = 42MHz
     *   每个计数周期 = 1/42MHz ≈ 23.8ns
     */
    TIM_TimeBaseStructure.TIM_Prescaler        = 0x1;                          /* 预分频器: PSC=1, CK_CNT=42MHz */
    TIM_TimeBaseStructure.TIM_Period           = 42000;                         /* 自动重装载值(ARR): PWM周期=1ms@1kHz */
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                            /* 重复计数器(RCR): 不使用 */
    TIM_TimeBaseStructure.TIM_ClockDivision    = TIM_CKD_DIV1;                  /* 时钟分频(CKD): 不分频 */
    TIM_TimeBaseStructure.TIM_CounterMode      = TIM_CounterMode_CenterAligned1; /* 计数器模式: 中心对齐模式1 */
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /*
     * TIM1通道1(CH1) PWM配置(中心对齐PWM模式1):
     *   中心对齐模式下PWM占空比与CCR值的关系:
     *     - 递增阶段: CNT < CCR 时输出有效电平
     *     - 递减阶段: CNT > CCR 时输出有效电平
     *   CCR1 = 21000 → 占空比 = 21000/42000 = 50%
     */
    TIM_OCInitStructure.TIM_OCMode        = TIM_OCMode_APWM1;            /* 输出模式: 非对称PWM模式1 */
    TIM_OCInitStructure.TIM_OutputState   = TIM_OutputState_Enable;      /* 输出状态: 使能(CH1输出到PC5) */
    TIM_OCInitStructure.TIM_OCPolarity    = TIM_OCPolarity_High;         /* 输出极性: 高电平有效 */
    TIM_OCInitStructure.TIM_OCIdleState   = TIM_OCIdleState_Reset;       /* 空闲状态: 复位(输出低电平) */
    TIM_OCInitStructure.TIM_OutputNState  = TIM_OutputNState_Disable;    /* 互补输出: 禁用 */
    TIM_OCInitStructure.TIM_OCNPolarity   = TIM_OCNPolarity_High;        /* 互补输出极性: 高(互补输出禁用时忽略) */
    TIM_OCInitStructure.TIM_OCNIdleState  = TIM_OCNIdleState_Reset;      /* 互补空闲状态: 复位 */
    TIM_OCInitStructure.TIM_Pulse         = 21000;                        /* CCR1值: 21000(占空比50%, 决定消隐触发位置) */
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);                     /* 使能CCR1预装载(更新事件时加载) */

    /*
     * TIM1通道2(CH2) PWM配置:
     *   CCR2 = 10500 → 占空比 = 10500/42000 = 25%
     *   CH2可作为第二个消隐触发源(通过VC_TriggerSource_TIM1OC2REFC选择)
     */
    TIM_OCInitStructure.TIM_Pulse = 10500;                                 /* CCR2值: 10500(占空比25%) */
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);                     /* 使能CCR2预装载 */

    /* 使能TIM1 CH1和CH2的比较匹配中断(用于调试和事件记录) */
    TIM_ITConfig(TIM1, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

    /*
     * 使能TIM1主输出(MOE)。
     * 注意: 高级定时器TIM1必须使能MOE后PWM输出才会生效。
     */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* 使能TIM1计数器(启动定时器) */
    TIM_Cmd(TIM1, ENABLE);
}


/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 系统时钟配置函数。
*             时钟链: HSE(8MHz) → PLL → 168MHz → SYSCLK
*             AHB(HCLK) = 168MHz, APB1(PCLK1) = 42MHz, APB2(PCLK2) = 84MHz
*             TIM1和VC模块均挂在APB2总线上，输入时钟为84MHz。
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
*             配置VC0和TIM1 CC中断通道的抢占优先级和子优先级。
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

    /* 配置TIM1 CC中断优先级(TIM1 CH1/CH2比较匹配共用此中断向量) */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;                  /* TIM1捕获/比较中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;        /* 抢占优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;               /* 子优先级: 0x0F(最低) */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     /* 使能该中断通道 */
    NVIC_Init(&NVIC_InitStructure);
}

/***************************** 芯圣电子  www.holychip.cn  (END OF FILE) *********************************/
