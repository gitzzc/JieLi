/*! \page11 LCD 编程指南
 *
 * \section intro_sec_lcd LCD 介绍
 * - CD002 LCD模块控制器，主要负责控制推LCD屏幕，最大可以推6COM & 16SEG的屏幕，模块使用IRTC 1KHz，模块可以使用RTC晶振时钟32KHz或内部RC(约250KHz/8)作为模块时钟
 * \section lcd1 LCD 寄存器 列表：
 *   \image html lcd.bmp
 * \section lcd2 LCD 寄存器：
 * - LCD_CON0：\n
 *   Bit0: 使能这个模块\n
 *   Bit1: COM0, SEG0输出一个1Hz的信号，可作为"跳秒"脚\n
 *   Bit3~2：偏压选择\n
 *    00：模拟模块工作禁止\n	
 *    01:  1/2 bias\n
 *    10:  1/3 bias \n
 *    11:  1/4 bias\n
 *   Bit6~Bit4：VLCD控制\n
 *    000：2.6V\n
 *    001：2.7V\n
 *    ......  \n
 *    110：3.2V\n
 *    111：3.3V\n
 *   Bit7：帧频率控制\n
 *    0：FLCD = 32KHz/128\n
 *    1：FLCD = 32KHz/64\n
 *   FLCD为模块状态切换时钟频率

 * - LCD_CON1：\n
 *   Bit3~0：交替充电模式下强充电占的Cycle数（按32KHz时钟）\n
 *   Bit5~4：充电模式控制\n
 *    00：一直用弱充电模式\n
 *    01：一直用强充电模式\n
 *    10：交替充电模式A\n
 *    11：交替充电模式B\n
 *   Bit7~6：选择COM的数目(Duty)\n
 *    00：3COM\n
 *    01：4COM\n
 *    10：5COM\n
 *    11：6COM\n
 *    1/Duty = 1/(COMCNT + 3)\n
 *   (注：交替充电模式A在状态切换时开始转强驱动，经过CHGDUTY+1个32KHz时钟后撤销强驱动\n
 *   交替充电模式B在状态切换前半个周期（32KHz时钟）开始转强驱动，经过CHGDUTY+1.5个32KHz时钟后撤销强驱动)

 * - LCD_CON2：\n
 *   Bit4：选择LCD的时钟，0：IRTC晶振时钟作为LCD模块工作时钟 ，1: RC时钟作为LCD模块工作时钟

 * - 使用说明：\n
 *   1.首先选择LCD模块工作时钟，如果系统有32K晶振，则选32KHz晶振时钟，否则选内部RC时钟。\n
 *   2.其次设定LCD电压（VLCDS）、偏置（BIAS）、充电模式（CHGMOD、CHGDUTY）等\n
 *   3.再次选择COM和SEG的位置，例如选择需要4个COM，选择COM0(P30), COM1(P31), COM2(P32), COM3(P33), 然后选择SEG8~SEG15(P1)，组成4COMX8SEG。那么控制器选择，如下：\n
 *     COMCNT =  01;	//选择COM  \n
 *     SEGEN1 = 0xff;	//选择SEG   \n
 *   4.然后打开LCD模块\n
 *     LCDCCON0 |= 0x01;	     //打开LCD \n
 *   5.填写数据; 共有6个COM，每个SEG需要16bit，填写数据分别是\n
 *     SEG0:{P1DIE, P0DIE}\n
 *     SEG1:{P1HD, P0HD}\n
 *     SEG2:{P1PD, P0PD}\n
 *     SEG3:{P1PU, P0PU} \n
 *     SEG4:{P1, P0}\n
 *     SEG5:{P1DIR, P0DIR}\n

 */
