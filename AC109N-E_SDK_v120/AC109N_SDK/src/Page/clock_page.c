/*! \page14 CLOCK 编程指南
 *
 * \section clock0 CLOCK 介绍
 * - CD02具备如下4个原生时钟源：\n
 *   1，rc_clk    来自片内振荡器，振荡频率约260KHz，且会随电压和温度变化。\n
 *   2，osc_clk    来自片内振荡器，外部需在X32KI和X32KO引脚挂载32.768KH.或1-12MHz晶振 \n
 *   3，htc_clk    来自片内振荡器，振荡频率约5MHz，受电压和温度影响较小。\n
 *   4，hosc_clk	来自片内振荡器，外部需在P00和P01引脚挂载1-12MHz晶振。\n
 *   CD02还具备如下1个衍生时钟源\n
 *   1，pll_clk    来自片内PLL，该PLL的输入参考时钟可由osc_clk, htc_clk或hosc_clk提供。
 * \subsection clock1 PLL部分电路框图如下
 *   \image html clock1.bmp
 * \subsection clock2 时钟部分电路框图如下
 *   \image html clock2.bmp
 * \section clock3 CLOCK 寄存器：
 * - CLK_CON0：可读可写\n
 *   Bit0：250KHz片内RC振荡器使能，高有效\n
 *   Bit1：HOSC振荡器使能，高有效，不使用时，P00/P01作为普通IO口使用，打开HOSC振荡器前，需将P00的方向设置为输入，P01的方向设置为输出。否则HOSC可能不能起振\n
 *   Bit2：PLL模块使能，高有效\n
 *   Bit3：osc_clk来源选择，0: 使用OSC振荡器(X32KI/X32KO)作为osc_clk，1: 使用HOSC振荡器(P00/P01)作为osc_clk\n
 *   Bit5~4：PLL参考时钟源选择\n
 *      00: 使用DPLL的输出(262.144KHz)作为PLL的参考时钟\n
 *      01: 使用HTC的输出(~5MHz)作为PLL的参考时钟\n
 *      10: 使用OSC的输出(32.768KHz/1-12MHz)作为PLL的参考时钟\n
 *      11: 使用HOSC的输出(1-12MHz)作为PLL的参考时钟\n
 *   Bit6：IO复位系统使能，0: 不使用P35或P07的'0'电平复位系统，1: 当P35或P07上出现超过4秒钟的'0'电平时，系统复位\n
 *   Bit7：IO复位系统选择，0: 使用P35的'0'电平复位系统，1: 使用P07的'0'电平复位系统
 *
 * - CLK_CON1：可读可写\n
 *   Bit1~0：主时钟来源选择\n
 *      00: 选择osc_clk\n
 *      01: 选择rc_clk\n
 *      10: 选择htc_clk\n
 *      11: 选择pll_clk\n
 *   Bit2：时钟2倍频使能，高有效\n
 *   Bit3：主时钟切换，0: 系统运行于RC时钟，1: 系统运行于其他时钟\n
 *   Bit4: 解码器时钟使能，低有效\n
 *   Bit5: OTP时钟使能，低有效\n
 *   Bit6：OTP时钟分频控制，0: OTP运行频率和系统一致，1: OTP运行频率为系统的一半\n
 *
 * - CLK_CON2：可读可写\n
 *   Bit0：DAC模块时钟选择，0: 使用PLL的2分频时钟来驱动DAC模块，1: 使用osc_clk来驱动DAC模块\n
 *   Bit1：LCD模块时钟选择，0: 使用osc_clk来驱动LCD模块，1: 使用wclk来驱动LCD模块\n
 *   Bit4~2：P05输出功能选择\n
 *      0xx: P05作为通用IO使用\n
 *      100: P05输出osc_clk\n
 *      101: P05输出sys_clk\n
 *      110: P05输出htc_clk\n
 *      111: P05输出pll_clk\n
 *   Bit5: DPLL禁止追踪，0: 允许DPLL追踪32.768KHz的晶振信号，1: 禁止DPLL追踪32.768KHz的晶振信号，此时DPLL处于自由运行状态\n
 *   Bit6：CPU断点功能使能，低有效\n
 *
 * - CLKCON3：\n
 *   SYS_DIV:	系统分频选择\n
 *   		0: 		系统1分频\n
 *   		1: 		系统2分频\n
 *   		2: 		系统3分频\n
 *   		……               \n
 *   		255:    系统256分频\n
 *
 * - CLK_GAT：\n
 *   Bit0：在不使用TIMER/IRDA时，可关闭以省电，低为打开驱动时钟\n
 *   Bit1：在不使用SD时，可关闭以省电，低为打开驱动时钟\n
 *   Bit2：在不使用IIC/SPI/UART时，可关闭以省电，低为打开驱动时钟\n
 *   Bit3: 在不使用ISP时，可关闭以省电，低为打开驱动时钟\n
 *   Bit4：在不使用USB时，可关闭以省电，低为打开驱动时钟\n
 *   Bit5：在不使用LCD时，可关闭以省电，低为打开驱动时钟\n
 *   Bit6: 在不使用DAC时，可关闭以省电，低为打开驱动时钟\n
 *   Bit7：在不使用DPLL时，可关闭以省电，低为打开驱动时钟\n
 *
 * - PLL_CON2：NF[9:2]\n
 * - PLL_CON1: \n
 *   Bit7~6：NF[1:0]\n
 *   Bit5~0: NR[13:8] \n
 * - PLL_CON0: NR[7:0] \n
 * - PLLCON2-0用于配置PLL的NF和NR，这3个寄存器均为只写，读出为不确定值。\n
 *   NF:		PLL的前分频系数，10bit宽度，允许写入的值为0-1023 \n
 *   0: 		前分频系数为1\n
 *   1: 		前分频系数为2\n
 *   2: 		前分频系数为3\n
 *   ……              \n
 *   1023: 	前分频系数为1024\n
 *   NR:		PLL的后分频系数，14bit宽度，允许写入的值为1-16383\n
 *   1: 		后分频系数为2\n
 *   2: 		后分频系数为3\n
 *   ……         \n
 *   16383: 	后分频系数为16384\n
 */
