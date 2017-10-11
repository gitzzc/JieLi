/*! \page7 PORT指南
 *
 * \section intro_sec_port PORT 总体介绍
 *
 * - CD002 支持7个I/O 中断和 1Hz、2Hz 和 1 Minute时基共同触发第8个I/O中断，同时可作为唤醒信号
 * \subsection port0 GPIO分布介绍
 *   \image html port.bmp
 * \subsection  寄存器介绍
 * - IO_MC0：IO mapping控制寄存器0\n
 *   bit7~6: UART模块IO re-mapping设置 \n
 *   00：TX:P06，RX:P07\n
 *   01：TX:P24，RX:P25\n
 *   10：TX:P32，RX:P33\n
 *   11：TX:P36，RX:P37\n
 *   \image html uart.bmp
 *   bit5~4: IIC模块IO re-mapping设置 \n
 *   00：SCK:P00，SDL:P01\n
 *   01：SCK:P26，SDL:P27\n
 *   10：SCK:P32，SDL:P33\n
 *   11：SCK:P36，SDL:P37\n
 *   \image html iic.bmp
 *   bit3:	SD模块占用IO使能，0：不允许SD模块占用IO，即使SD模块已打开，1:允许SD模块在打开时占用相应IO\n
 *   bit2:	SD模块IO re-mapping设置，0：SDCMD为P21，SDDAT为P22，1：SDCMD为P00，SDDAT为P01\n
 *   \image html sd.bmp
 *   bit1~0: SD模块CLK re-mapping设置\n
 *   00：SDCLK不可输出\n
 *   01：使用P20作为SDCLK\n
 *   10：使用P30作为SDCLK\n
 *   11：使用P35作为SDCLK\n
 *   \image html sd_clk.bmp
 *
 * - IO_MC1：IO mapping控制寄存器1\n
 *   bit7：PWM4输出控制 0：输出PWM4,1：输出RTC晶振时钟 \n
 *   bit6: isd 2wire 模块IO re-mapping设置 ，0：ISDCK为P24，ISDDAT为P25,1：ISDCK为P00，ISDDAT为P01\n
 *   \image html isd_2wire.bmp
 *   bit5: SPI模块占用IO使能，0：不允许SPI模块占用IO，即使SPI模块已打开，1:允许SPI模块在打开时占用相应IO\n
 *   bit4: SPI模块IO re-mapping设置\n
 *   0：\n
 *   2线：SPICK为P16，SPIDO为P17，SPIDI为P17\n
 *   3线：SPICK为P16，SPIDO为P17，SPIDI为P17\n
 *   1：\n
 *   2线：SPICK为P00，SPIDO为P01，SPIDI为P01\n
 *   3线：SPICK为P00，SPIDO为P01，SPIDI为P44\n
 *   \image html spi.bmp
 *   bit3~2: IRFLT输出选择，详情请参见IRFLT部分\n
 *   00：IRFLT的输出至timer0的捕获端\n
 *   01：IRFLT的输出至timer1的捕获端\n
 *   10：IRFLT的输出至timer2的捕获端\n
 *   11：IRFLT的输出至timer3的捕获端\n
 *   bit1~0: IRFLT输入选择，详情请参见IRFLT部分\n
 *   00：使用CAP0作为IRFLT的输入信号\n
 *   01：使用CAP1作为IRFLT的输入信号\n
 *   10：使用CAP2作为IRFLT的输入信号\n
 *   11：使用CAP3作为IRFLT的输入信号\n
 *
 * - WKUPPND：\n
 *   bit7:	只写，写入'1'将事件3清0 \n
 *   bit6:	只写，写入'1'将事件2清0 \n
 *   bit5:	只写，写入'1'将事件1清0 \n
 *   bit4:	只写，写入'1'将事件0清0 \n
 *   bit3:	只读，事件3发生时，此位将被硬件置1，并引发wakeup中断或唤醒系统，需软件清0\n
 *   bit2:	只读，事件2发生时，此位将被硬件置1，并引发wakeup中断或唤醒系统，需软件清0\n
 *   bit1:	只读，事件1发生时，此位将被硬件置1，并引发wakeup中断或唤醒系统，需软件清0\n
 *   bit0:	只读，事件0发生时，此位将被硬件置1，并引发wakeup中断或唤醒系统，需软件清0
 *
 * - WKUPCON：\n
 *   bit7:  事件3唤醒边沿选择，0: 上升沿    1: 下降沿\n
 *   bit6:  事件2唤醒边沿选择，0: 上升沿    1: 下降沿\n
 *   bit5:  事件1唤醒边沿选择，0: 上升沿    1: 下降沿\n
 *   bit4:  事件0唤醒边沿选择，0: 上升沿    1: 下降沿\n
 *   bit3:  事件3检测使能，0: 不检测事件3的发生	1: 检测事件3的发生，如果此位置1，且总中断置1，那么事件3的发生会引发wakeup中断\n
 *   bit2:  事件2检测使能，0: 不检测事件2的发生	1: 检测事件2的发生，如果此位置1，且总中断置1，那么事件3的发生会引发wakeup中断\n
 *   bit1:  事件1检测使能，0: 不检测事件1的发生	1: 检测事件1的发生，如果此位置1，且总中断置1，那么事件3的发生会引发wakeup中断\n
 *   bit0:	事件0检测使能，0: 不检测事件0的发生	1: 检测事件0的发生，如果此位置1，且总中断置1，那么事件3的发生会引发wakeup中断\n

 */





