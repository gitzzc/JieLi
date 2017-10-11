/*! \mainpage AC109N 开发包编程指南
 *
 * \section intro_sec_main 总体介绍
 *
 * \version 1.00
 *
 * \date 2012-8-30
 *
 * \section sec_descriptor 芯片资源介绍
 * - 8051兼容指令集
 * - 扩展指令集
 * - 最高工作频率：48M
 * - 256 byte DRAM\n
 *   8K byte XRAM\n
 *   16K byte OTP 
 * - 四级可嵌套中断优先级
 * - TIMER,UART,IIC,SPI,EMI,ADC(10bit),DAC(24bit)
 * - LCD 控制器
 * - USB OTG full speed 控制器
 *   支持ima ADPCM格式\n
 *   支持采样率(KHz):8/11.025/12/16/22.05/24/32/44.1/48\n
 * - 支持USB mass storage
 * - 支持片内IRTC
 * \section sec_descriptr 开发包介绍
 * - 目录划分
 *   -# common : 启动代码，库函数
 *   -# DAC: DAC 模块驱动，控制函数
 *   -# FM：FM 模块驱动，控制函数接口，模式主循环
 *   -# IIC：IIC 模块驱动，读/写接口
 *   -# key_msg：AD 按键初始化，扫描函数，红外遥控扫描函数，消息处理函数
 *   -# Line in：Line In控制函数接口，模式主循环
 *   -# Music：解码控制接口，音乐控制函数接口，音乐模式主循环
 *   -# RTC：RTC 模块初始化函数
 *   -# UI：LCD/LED/LCD SEG 屏幕驱动，显示界面
 *   -# USB Device：USB 从机主循环
 */
