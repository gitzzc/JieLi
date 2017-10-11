#ifndef _INT_H_
#define _INT_H_

#include "cd02.h"
//#ifdef __SMART_CPU__

/******************************************************/
/*
				Interrupt
*/
/******************************************************/
#define INT_VETOR		    0x0003L
#define VECTOR(n)		    (n*8 + INT_VETOR)

#define TIMER0_INT		    0
#define Timer0_IE(x)		ET0 = x
#define Timer0_IP(x)	    IP0L = (IP0L & ~BIT(0)) | (x & 0x1); \
                            IP0H = (IP0H & ~BIT(0)) | ((x & 0x2)>>1)
#define TIMER1_INT		    1
#define Timer1_IE(x)		ET1 = x
#define Timer1_IP(x)	    IP0L = (IP0L & ~BIT(1)) | ((x & 0x1)<<1); \
                            IP0H = (IP0H & ~BIT(1)) | (x & 0x2)
#define TIMER2_INT		    2
#define Timer2_IE(x)		ET2 = x
#define Timer2_IP(x)	    IP0L = (IP0L & ~BIT(2)) | ((x & 0x1)<<2); \
                            IP0H = (IP0H & ~BIT(2)) | ((x & 0x2)<<1)
#define TIMER3_INT		    3
#define Timer3_IE(x)		ET3 = x
#define Timer3_IP(x)	    IP0L = (IP0L & ~BIT(3)) | ((x & 0x1)<<3); \
                            IP0H = (IP0H & ~BIT(3)) | ((x & 0x2)<<2)
#define IRTC_INT            4
#define IRTC_IE(x)			ERTC = x
#define IRTC_IP(x)		    IP0L = (IP0L & ~BIT(4)) | ((x & 0x1)<<4); \
                            IP0H = (IP0H & ~BIT(4)) | ((x & 0x2)<<3)
#define USBSOF_INT	        5
#define USBSOF_IE(x)		ESOF = x
#define USBSOF_IP(x)		IP0L = (IP0L & ~BIT(5)) | ((x & 0x1)<<5); \
                            IP0H = (IP0H & ~BIT(5)) | ((x & 0x2)<<4)
#define USBCTL_INT		    6
#define USBCTL_IE(x)		EUSB = x
#define USBCTL_IP(x)        IP0L = (IP0L & ~BIT(6)) | ((x & 0x1)<<6); \
                            IP0H = (IP0H & ~BIT(6)) | ((x & 0x2)<<5)
/*  被EA 占据，没有单独使能位
#define INT_UART_BRK		    7
#define UART_IE(x)			ET7 = x
#define UART_IP(x)	        IP0L = (IP0L & ~BIT(7)) | ((x & 0x1)<<7); \
                            IP0H = (IP0H & ~BIT(7)) | ((x & 0x2)<<6)
*/
#define PORT_INT            8
#define PORT_IE(x)		    IE1 = (IE1 & ~BIT(0)) | (x & 0x1);//EPORT = x
#define PORT_IP(x)	        IP1L = (IP1L & ~BIT(0)) | (x & 0x1); \
                            IP1H = (IP1H & ~BIT(0)) | ((x & 0x2)>>1)
#define OBUF_MP3_INT            9
#define OBUF_IE(x)		    IE1 = (IE1 & ~BIT(1)) | ((x & 0x1)<<1);//EOBUF = x
#define OBUF_IP(x)	        IP1L = (IP1L & ~BIT(1)) | ((x & 0x1)<<1); \
                            IP1H = (IP1H & ~BIT(1)) | (x & 0x2)
#define SINT_INT            10
#define SINT_IE(x)		    IE1 = (IE1 & ~BIT(2)) | ((x & 0x1)<<2);//ESINT0 = x
#define SINT_IP(x)	        IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<2); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<1)
#define ISD2W_INT	            11
#define ISD2W_IE(x)			IE1 = (IE1 & ~BIT(3)) | ((x & 0x1)<<3);//EISD = x
#define ISD2W_IP(x)		    IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<3); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<2)
#define SDC_INT			    12
#define SDC_IE(x)			IE1 = (IE1 & ~BIT(4)) | ((x & 0x1)<<4);//ESD = x
#define SDC_IP(x)		    IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<4); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<3)
#define SPI_IIC_INT			    13
#define SPI_IE(x)			IE1 = (IE1 & ~BIT(5)) | ((x & 0x1)<<5);//ESPI_IIC = x
#define SPI_IP(x)		    IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<5); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<4)

#define LVD_WDT_OSC_ADC_INT			    14
#define ADC_IE(x)			IE1 = (IE1 & ~BIT(6)) | ((x & 0x1)<<6);//ELVD_WDT = x
#define ADC_IP(x)		    IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<6); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<5)
#define SOFT_INT 15
#define SOFT_IE(x)			IE1 = (IE1 & ~BIT(7)) | ((x & 0x1)<<7);//ESINT1 = x
#define SOFT_IP(x)		    IP1L = (IP1L & ~BIT(7)) | ((x & 0x1)<<7); \
                            IP1H = (IP1H & ~BIT(7)) | ((x & 0x2)<<6)


//#endif  /*  __IAR_SYSTEMS_ICC__ */

#endif  /*  _INT_H_ */
