/*! \page5 ADC 编程指南
 *
 * \section adc ADC 介绍
 * - CD002 支持16 个ADC入口\n
 * \section adc1 ADC 寄存器
 * - ADC_CON0（可读写）\n
 *   Bit2~0，波特率选择，初始值为0\n
 *   000：系统时钟除6\n
 *   001：系统时钟除12\n
 *   010：系统时钟除24\n
 *   011：系统时钟除48\n
 *   1xx：系统时钟除1\n 
 *   Bit3，ADC 模拟模块Enable，初始值为0\n
 *   Bit4，ADC控制器Enable（数字电路部分），初始值为0\n
 *   Bit5，ADC中断允许，高有效，初始值为0\n
 *   Bit6，ADC Pending，初始值为1\n
 *   Bit7，ADC启动位，写1会启动ADC转换，并且对ADC Pending清0。读永远是0

 * - ADC_CON1（可读写）初始值为xx\n
 *   Bit3~0，通道选择\n
 *   0000~0111：选择P00~P07\n
 *   1000：		选择P20\n
 *   1001：		选择P21\n
 *   1010：		选择P22\n
 *   1011：		选择P35\n
 *   1100：		选择AMUX左声道\n
 *   1101:		选择AMUX右声道\n
 *   1110：		选择LDOIN的1/2\n
 *   1111：		选择LDO电压源(1.2V)\n
 *   Bit7~4，	启动延时控制，\n
 *   实际启动延时为这4Bit乘8个ADC时钟\n
 *   
 *
 * - ADC_DATH\n
 *   bit7~0 : 存放ADC采样结果的高 8 bit
 * - ADC_DATL\n
 *   bit7~6 : 存放ADC采样结果的低 2 bit
 */
