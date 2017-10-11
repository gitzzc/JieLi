/*! \page12 IRFLT 编程指南
 *
 * \section intro_sec_irflt IRFLT 介绍
 * - CD002 IRFLT是一个专用的硬件模块，用于去除掉红外接收头信号上的窄脉冲信号，提升红外接收解码的质量
 * \subsection irflt1 连接情况请示意图：
 *   \image html irflt.bmp
 * \section irflt2 IRFLT 寄存器：
 * - IRFLT_CON：可读可写\n
 *   Bit0：IRFLT使能,高有效\n
 *   Bit3~2：时基发生器驱动源选择\n
 *   00: 选择系统时钟来驱动时基发生器\n
 *   01: 选择OSC时钟来驱动时基发生器\n
 *   1x: 选择HTC时钟来驱动时基发生器\n
 *   Bit7~4：时基发生器分频选择\n
 *   0000: 分频倍数为1\n
 *   0001: 分频倍数为2 \n
 *   ......      \n
 *   1110: 分频倍数为16384\n
 *   1111: 分频倍数为32768\n
 *
 * - PSEL选定的分频倍数N和TSRC选定的驱动时钟的周期Tc共同决定了IRFLT用于采样红外接收信号的时基Ts\n
 *   Ts = Tc * N\n
 *   例如，当选择32KHz的OSC时钟，并且分频倍数为1时，Ts = 30.5uS。根据IRFLT的工作规则，所有小于(30.5*4=122uS)的窄脉冲信号，均会被滤除。\n
 *   又如，当选择48MHz的系统时钟，并且分频倍数为1024时，Ts = 21.3uS。根据IRFLT的工作规则，所有小于(21.3*4=85uS)的窄脉冲信号，均会被滤除\n

 */
