/*! \page9 IRTC指南
 *
 * \section intro_sec_irtc IRTC 总体介绍
 *
 * - CD002 支持片内IRTC，除了作为普通时钟使用外，还可以用作唤醒和Capture 触发事件使用
 *
 * \subsection itrc1 IRTC 寄存器介绍
 * - RTC_CON0：\n
 *   Bit7: Read Only，IRTC WKUP标志位，当RTC计时器与闹铃寄存器相等时置1\n
 *   Bit6: WKUP中断允许，高有效\n
 *   Bit5: 512Hz时基标志，可以给CPU产生约2ms中断\n
 *   Bit4: 512Hz时基中断允许\n
 *   Bit3: 2Hz时基标志，可以给CPU产生500ms中断\n
 *   Bit2: 2Hz时基中断允许\n
 *   Bit1: Read Only，移位寄存器发送完成标志，高有效\n
 *   Bit0: 移位寄存器片选信号，高有效

 * - RTC_CON1：\n
 *   Bit7: 写"1"清除WKUPPending，读一直为0\n
 *   Bit5: 写"1"清除512Hz时基Pending，读一直为0\n
 *   Bit3: 写"1"清除2Hz时基Pending，读一直为0

 * - RTC_BUF：\n
 *   写这个寄存器会启动一次发数据（一个字节），通信完成后可以读这个寄存器得到接收的数据。\n

 * - 以下是收发命令的例程。\n
 *   #define IRTC_CSEN    IRTCON |= BIT(0)\n
 *   #define IRTC_CSDIS   IRTCON &= ~BIT(0)\n
 *   unsigned char irtc_sr_byte(unsigned char x)\n
 *   {\n
 *      IRTBUF = x;\n
 *   	while(!(IRTCON & BIT(1)));\n
 *      return IRTBUF;\n
 *   }\n
 *
 * \subsection itrc2 IRTC支持的命令：
 *   \image html rtc1.bmp
 *   \image html rtc2.bmp
 * - 连续写入或读出时，地址会自动增1，当地址增加至255后，可回绕至0继续写入或读出。
 * - 注：芯片内部共有256Byte SRAM
 *
 * - IRTC 内部控制寄存器0：\n
 *   Bit7: PDFLAG，当RTCVDD power up时写1，如果不是第一次上电，软件可以写0 indicate\n
 *   Bit6: X1MOE，IRTC 512Hz输出取代 1/60Hz输出\n
 *   Bit5: Alarm enable\n
 *   Bit4: WKPD，WakeUp pin pulldown enable，高有效\n
 *   Bit3: WakeUp pin DIR，0为输出，1为输入\n
 *   Bit2: WakeUp pin IO SEL，0为 WakeUp，1 为普通 IO\n
 *   Bit1: 当普通IO使用时此位为WakeUp pin Output register\n
 *   Bit0: WakeUp pin Input，Read Only
 *
 * - IRTC 内部控制寄存器1：\n
 *   Bit3: Alarm output，高有效，Read Only\n
 *   Bit2: High current OSC enable，高有效\n
 *   Bit1: 12MSEL，1：12MHz OSC,0:32KHz，复位值为0\n
 *   Bit0: OSCEN，0：OSC Disable 1：OSC Enable，复位值为1
 */
