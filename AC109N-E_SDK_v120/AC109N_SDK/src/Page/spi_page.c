/*! \page2 SPI 编程指南
 *
 * \section intro_sec_spi SPI 介绍
 *
 * -  SPI有两个出口：
 * - 出口1：CLK为P16、DO为P17、DI为P15
 * - 出口2：CLK为P00、DO为P01、DI为P44
 * - 当SPI被设置为两线模式时，DO同时为输入和输出。
 * \section spi1 SPI 寄存器：
 * - SPI_CON：（可写可读，Reset值为0x00）
 *   Bit0：SPI Enable，高有效，reset值为0\n
 *   Bit2：SPICS信号使能,高有效\n
 *   Bit3：两线模式选择，0为三线模式，1为两线模式\n
 *   Bit4：SPI时钟边沿选择，1为下降沿，0为上升沿\n
 *   Bit5：更新数据边沿选择，0为在SPICK的上升沿更新数据，1: 在SPICK的下降沿更新数据\n
 *   Bit6：SPICK信号极性选择，0为SPICK空闲时为0电平，SPICK空闲时为1电平。\n
 *   Bit7：SPICS信号极性选择，0为SPICS空闲时为0电平,  1为SPICS空闲时为1电平\n
（注：在两线模式下，SPI是输入还是输出由方向选择寄存器控制。另外方向选择寄存器在DMA时，也作为DMA方向选择寄存器）\n
 *
 * - SPI_BAUD：波特率寄存器，Reset值为x，用之前需要初始化\n
 *   波特率计算公式：波特率 = Freq_sys /(SPIBAUD+1)
 *
 * - SPI_BUF：SPI 数据缓冲区\n
 *   若在三线模式，写一次SPIBUF会启动一次收发操作，发出去的内容就是刚刚写入SPIBUF的内容，收到的内容则是DI输进来的内容。当Pending为1，收发完成，CPU可以读SPIBUF得到刚刚收到的数据。读完SPIBUF后，CPU要对Pending清零（也可以等下一次写SPIBUF清零）。\n
 *   若在两线模式，当方向寄存器设为输出，则写一次SPIBUF会启动一次发送操作，发出去的内容就是刚刚写入SPIBUF的内容，若方向寄存器设为输入，则写一次SPIBUF会启动一次接收操作，被写入SPIBUF的内容会被忽略。当Pending为1，收发完成，CPU可以读SPIBUF得到刚刚收到的数据。读完SPIBUF后，CPU要对Pending清零（也可以等下一次写SPIBUF清零）。\n
 *   SPI中断入口是13, 中断允许是IE1.5
 */
