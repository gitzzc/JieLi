/*--------------------------------------------------------------------------*/
/**@file     uart.h
   @brief    ´®¿Ú´òÓ¡Çý¶¯
   @details
   @author
   @date   2011-4-26
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _UART_
#define _UART_


#define UART0	1 		//<P00 P01
#define UART1	0		//<P10 P11
#define UART2	0		//<P64 P65
#define UART3	0		//<P66 P67

#ifdef __SMART_CPU__
_near_func void uart_init(void);
_near_func void putbyte (char c);
_near_func void printf_buf(u8 _xdata *buf, u16 len);
_near_func void printf_u8(u8 dat);
_near_func void deg_string(u8 *p);
_near_func void my_putchar (u8 c);
_near_func void my_puts(u8 _code *p);
_near_func void set_putchar(void(_near_func *fun)(u8));
#endif

#endif
