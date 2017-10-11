/*! \page6 TIMER 编程指南
 *
 * \section intro_sec_timer TIMER 介绍
 * - CD002 一共有4个TIMER，其中TIMER0和TIMER1是8位的TEMER;TIMER2和TIMER3是16位的TIMER。
 * \section timera timer0(8位) 介绍
 * - TMR0_CON0：可读可写\n
 *   Bit1~0：Timer0 Mode\n
 *     00: timer关闭\n
 *     01: 定时/计数/PWM模式\n
 *     10: IO口上升沿捕获模式（测量'0'电平宽度）\n
 *     11: IO口下降沿捕获模式（测量'1'电平宽度）\n
 *   Bit3~2：timer驱动源选择\n
 *     00: 使用系统时钟作为timer的驱动源\n
 *     01: 使用IO口信号作为timer的驱动源\n
 *     10: 使用OSC时钟作为timer的驱动源\n
 *     11: 使用HTC时钟作为timer的驱动源\n
 *   Bit5~4：这两位和TMR0_CON1中的PSET2一起构成预分频选择位\n
 *     000:		预分频1\n
 *     001:		预分频4\n
 *     010:		预分频16\n
 *     011:		预分频64\n
 *     100:		预分频256\n
 *     101:		预分频1024\n
 *     110:		预分频4096\n
 *     111:		预分频16384\n
 *   Bit6：软件在此位写入'1'将清除PND中断请求标志。\n
 *   Bit7：中断请求标志，当timer溢出或产生捕获动作时会被硬件置1，需要由软件清0\n
 * - TMR0_CON1：只写\n
 *   Bit0：PWM0信号输出使能。此位置1后，相应IO口的功能将会被PWM0信号输出替代\n
 *   Bit1：PWM0信号输出反向\n
 *   Bit4：PWM1信号输出使能。此位置1后，相应IO口的功能将会被PWM1信号输出替代\n
 *   Bit5：PWM1信号输出反向\n
 *   Bit6：和TMR0_CON0中的PSET1/0一起构成预分频选择位\n
 * - TMR0_CNT：可读可写\n
 *   Timer计数器
 * - TMR0_PRD：只写\n
 *   Timer计数上限
 * - TMR0_PWM0：只写\n
 *   在PWM模式下，此寄存器的值决定PWM0输出的占空比。占空比N的计算公式如下\n
 *   N = (TxPWM0 / TxPR) * 100%\n
 *   此寄存器不带有缓冲，任何写此寄存器的动作都会立即将PWM0输出信号清0，以防止由于不同步状态产生的PWM0波形占空比过大的问题\n

 * \section timerb timer1(8位) 介绍
 * - TMR1_CON0：可读可写\n
 *   Bit1~0：Timer0 Mode\n
 *     00: timer关闭\n
 *     01: 定时/计数/PWM模式\n
 *     10: IO口上升沿捕获模式（测量'0'电平宽度）\n
 *     11: IO口下降沿捕获模式（测量'1'电平宽度）\n
 *   Bit3~2：timer驱动源选择\n
 *     00: 使用系统时钟作为timer的驱动源\n
 *     01: 使用IO口信号作为timer的驱动源\n
 *     10: 使用OSC时钟作为timer的驱动源\n
 *     11: 使用HTC时钟作为timer的驱动源\n
 *   Bit5~4：这两位和TxCON1中的PSET2一起构成预分频选择位\n
 *     000:		预分频1\n
 *     001:		预分频4\n
 *     010:		预分频16\n
 *     011:		预分频64\n
 *     100:		预分频256\n
 *     101:		预分频1024\n
 *     110:		预分频4096\n
 *     111:		预分频16384\n
 *   Bit6：软件在此位写入'1'将清除PND中断请求标志。\n
 *   Bit7：中断请求标志，当timer溢出或产生捕获动作时会被硬件置1，需要由软件清0\n
 * - TMR1_CON1：只写\n
 *   Bit0：PWM0信号输出使能。此位置1后，相应IO口的功能将会被PWM0信号输出替代\n
 *   Bit1：PWM0信号输出反向\n
 *   Bit4：PWM1信号输出使能。此位置1后，相应IO口的功能将会被PWM1信号输出替代\n
 *   Bit5：PWM1信号输出反向\n
 *   Bit6：和TxCON0中的PSET1/0一起构成预分频选择位\n
 * - TMR1_CNT：可读可写\n
 *   Timer计数器
 * - TMR1_PRD：只写\n
 *   Timer计数上限
 * - TMR1_PWM0：只写\n
 *   在PWM模式下，此寄存器的值决定PWM0输出的占空比。占空比N的计算公式如下\n
 *   N = (TxPWM0 / TxPR) * 100%\n
 *   此寄存器不带有缓冲，任何写此寄存器的动作都会立即将PWM0输出信号清0，以防止由于不同步状态产生的PWM0波形占空比过大的问题\n

 * \section timerC  timer2(16位) 介绍
 * - TMR2_CON：可读可写\n
 *   Bit1~0：Timer0 Mode\n
 *     00: timer关闭\n
 *     01: 定时/计数模式\n
 *     10: IO口上升沿捕获模式（测量'0'电平宽度）\n
 *     11: IO口下降沿捕获模式（测量'1'电平宽度）\n
 *   Bit3~2：timer驱动源选择\n
 *     00: 使用系统时钟作为timer的驱动源\n
 *     01: 使用IO口信号作为timer的驱动源\n
 *     10: 使用OSC时钟作为timer的驱动源\n
 *     11: 使用HTC时钟作为timer的驱动源\n
 *   Bit5~4： 预分频选择位\n
 *     00:   预分频1\n
 *     01:   预分频4\n
 *     10:   预分频16\n
 *     11:   预分频64\n
 *   Bit6：软件在此位写入'1'将清除PND中断请求标志\n
 *   Bit7：中断请求标志，当timer溢出或产生捕获动作时会被硬件置1，需要由软件清0
 * - TMR2_CNTL、TMR2_CNTH：可读可写\n
 *   Timer计数器
 * - TMR2_PRDL、TMR2_PRDH：只写\n
 *   通过T2PRD控制，先写高位再写低位\n
 *   Timer计数上限\n
 *   在定时/计数模式下，当TxCNT == TxPR时，TxCNT会被清0。\n
 *   在上升沿/下降沿捕获模式下，TxPR是作为捕获寄存器使用的，当捕获发生时，TxCNT的值会被复制到TxPR中。而此时TxCNT自由的由0-65535-0计数，不会和TxPR进行比较清0

 * \section timerD  timer3(16位) 介绍
 * - TMR3_CON：可读可写\n
 *   Bit1~0：Timer3 Mode\n
 *     00: timer关闭\n
 *     01: 定时/计数模式\n
 *     10: IO口上升沿捕获模式（测量'0'电平宽度）\n
 *     11: IO口下降沿捕获模式（测量'1'电平宽度）\n
 *   Bit3~2：timer驱动源选择\n
 *     00: 使用系统时钟作为timer的驱动源\n
 *     01: 使用IO口信号作为timer的驱动源\n
 *     10: 使用OSC时钟作为timer的驱动源\n
 *     11: 使用HTC时钟作为timer的驱动源\n
 *   Bit5~4： 预分频选择位\n
 *     00:   预分频1\n
 *     01:   预分频4\n
 *     10:   预分频16\n
 *     11:   预分频64\n
 *   Bit6：软件在此位写入'1'将清除PND中断请求标志\n
 *   Bit7：中断请求标志，当timer溢出或产生捕获动作时会被硬件置1，需要由软件清0
 * - TMR3_CNTL、TMR3_CNTH：可读可写\n
 *   Timer计数器
 * - TMR3_PRDL、TMR3_PRDH：只写\n
 *   通过T3PRD控制，先写高位再写低位\n
 *   Timer计数上限\n
 *   在定时/计数模式下，当TxCNT == TxPR时，TxCNT会被清0。\n
 *   在上升沿/下降沿捕获模式下，TxPR是作为捕获寄存器使用的，当捕获发生时，TxCNT的值会被复制到TxPR中。而此时TxCNT自由的由0-65535-0计数，不会和TxPR进行比较清0
 */
