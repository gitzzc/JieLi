/*--------------------------------------------------------------------------*/
/**@file    uart.c
   @brief   串口模块接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "config.h"
#include "uart.h"

//_no_init void(*putchars)(u8) @ "XDATA_R0_7";


_near_func void uart_init(void) AT(MY_UART)
{
    P0PU = 0;
    P0PD = 0;

    UART_BAUD = (24000000/(8 * 115200)) - 1;	//25;//0x37;					/* 25: cpu 24M, 115200bps*/	/*77 for 384*/ /*0x137 for 9600*/

    UART_CON = 0x01;							/* enable uart */
    
    P0DIR &= 0xBF;							/* P06 for transmit */
    P0DIR |= 0x80;							/* P07 for receive */
    
    /*UART I/O-1
    P2DIR &= ~BIT(4);
    P2DIR |= BIT(5);
    IO_MC0 |= BIT(6);*/
    
    /*UART I/O-1
    P3DIR &= ~BIT(2);
    P3DIR |= BIT(3);
    IO_MC0 |= BIT(7);*/
    
    /*UART I/O-1
    USB_CON0 = 0x11;
    USB_CON2 &= ~BIT(1);
    P3DIR &= ~BIT(6);
    P3DIR |= BIT(7);
    IO_MC0 |= BIT(7) | BIT(6);*/
}

_near_func void putbyte(char c) AT(MY_UART)
{
    UART_BUF = c;
    while (!(UART_STA & 0x80))
    {
    }
}

_near_func void my_putchar(u8 c) AT(MY_UART)
{
    if (c == '\n')
    {
        putbyte(0x0d);
        putbyte(0x0a);
    }
    else
    {
        putbyte(c);
    }
}
/*
void get_hex_data(u8 dat)
{
   u8 dat0;
   if(dat<0x0a)
   {
       dat0 = dat+'0';
   }
   else
   {
       dat0 = dat+'A'-10;
   }
   putchar(dat0);
}

void printf_u32(u32 dat)
{
    putchar('\n');
    get_hex_data(((u8 *)(&dat))[0] >> 4);
    get_hex_data(((u8 *)(&dat))[0] & 0xf);

    get_hex_data(((u8 *)(&dat))[1] >> 4);
    get_hex_data(((u8 *)(&dat))[1] & 0xf);

    get_hex_data(((u8 *)(&dat))[2] >> 4);
    get_hex_data(((u8 *)(&dat))[2] & 0xf);

    get_hex_data(((u8 *)(&dat))[3] >> 4);
    get_hex_data(((u8 *)(&dat))[3] & 0xf);
    putchar('\n');
}

void printf_u8(u8 dat)
{
    get_hex_data(dat >> 4);
    get_hex_data(dat & 0xf);
    putchar(' ');
	putchar('\n');
}
*/
