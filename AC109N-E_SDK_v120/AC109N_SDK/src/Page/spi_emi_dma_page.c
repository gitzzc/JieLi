/*! \page3 SPI/EMI DMA 编程指南
 *
 * \section dma0 SPI&EMI DMA 介绍
 * - SPI & EMI DMA是一个双向DMA通道，可用于SPI或EMI的DMA操作，但两个外设不能同时DMA。
 *
 * \section dma1 SPI&EMI DMA 寄存器 介绍
 *
 * - SPI_STA：（可写可读）\n
 *   Bit3：EMI模式使能，0: 使用SPI模式，1：使用EMI模式，因为EMI模式只支持发送，所以在此模式下使用dma一定要将DIR设置为'0'\n
 *   Bit4：在2线模式或dma操作时设置传输的方向，0: 发送数据，1: 接收数据\n
 *   Bit5：SPI 中断使能，高有效\n
 *   Bit6：软件在此位写入'1'将清除PND中断请求标志\n
 *   Bit7：DMA Pending，中断请求标志，当1Byte传输完成或dma传输完成时会被硬件置1。\n
 *   有3种方法清除此标志\n
 *   1，向PCLR写入'1'\n
 *   2，写SPIBUF寄存器来启动一次传输\n
 *   3，写SPICNT寄存器两次来启动一次dma
 * - SPI_ADR：spi dma起始地址寄存器，只写，读出为不确定值\n
 *   设置dma起始地址时，需向此寄存器写入两次，先写入地址的高8位，后写入地址的低8位。
 *   如，需设置dma起始地址为0x1234，先写入0x12，再写入0x34。\n
 *
 * - SPI_CNT：spi dma计数寄存器，只写，读出为不确定值\n
 *   此寄存器用于设置dma操作的数目（按Byte计）并启动dma传输，需写入两次，先写入计数值的高8位，后写入计数值的低8位。第二次的写入将自动启动dma传输。
 *   如，需启动一次512Byte的dma传输，先写入0x02，再写入0x00

 */
