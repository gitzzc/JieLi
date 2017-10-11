/*! \page1  UART 编程指南
 *
 * \section intro0_sec UART 总体介绍
 * - CD002 UART有四个出口\n
 *   出口1：TX为P06、RX为P07\n
 *   出口2：TX为P24、RX为P25\n
 *   出口3：TX为P32、RX为P33\n
 *   出口4：TX为USBDP、RX为USBDM
 * \section uart0  UART 寄存器介绍
 * - UART_CON：UART控制寄存器\n
 *   Bit0：UART Enable，高有效，Reset值为0\n
 *   Bit1：9bit模式，高有效，Reset值为0\n
 *   Bit2：TX中断允许，高有效，Reset值为0，当TX Pending为1，而且TX中断允许为1，则会产生中断\n
 *   Bit3：RX中断允许，高有效，Reset值为0，当RX Pending为1，而且RX中断允许为1，则会产生中断\n
 *   Bit5~4：波特率控制寄存器高2位，Reset值为00 \n
 *   Bit7~6：Reserved，读为0\n
 *   注：串口中断地址为：0x003B;

 * - UART_STA： UART状态寄存器\n
 *   Bit0：9Bit模式时。RX接收到的第9位，Reset值为X\n
 *   Bit1：9Bit模式时，TX发送的第9位，Reset值为X\n
 *   Bit4: 清空RX Pending, 写1清空，写0无效\n
 *   Bit5: 清空TX Pending，写1清空，写0无效\n
 *   Bit6：RX Pending，Reset值为0，软件可清零，但不可置1。\n
 *   Bit7：TX Pending，Reset值为1，软件可清零，但不可置1。

 * - UTBAUD：波特率寄存器低8位，Reset值为X，用之前要初始化\n
 *   波特率计算公式：波特率 = Freq_sys / ((UTBAUD+1) *8)\n
 *   以上公式UTBAUD为10位寄存器

 * - UTBUF:\n
 *   当UART发送空闲时，写UTBUF可启动一次发送。发送的内容就是写UTBUF的内容。\n
 *   若RX Pending为1时，读UTBUF可读到接收的字节。注意，读完后要软件清RX Pending，否则后面收到的字节不会打进UTBUF，会丢失。
 */



