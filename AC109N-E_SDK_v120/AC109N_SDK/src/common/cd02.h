/***************************************************************************
 *                                - CD002.h -
 *
 * Special header for the XX Microcontroller.
 *
 ***************************************************************************/
#ifndef _CD002_H
#define _CD002_H

/***************************************************************************
 *                                - KEIL - C51 - A51
 ***************************************************************************/
#ifdef __KEIL__

#ifdef __CX51__
#ifndef	__C51__
#define __C51__
#endif	/*	__C51__	*/
#endif	/*	__CX51__	*/

#ifdef __AX51__
#ifndef	__A51__
#define __A51__
#endif	/*	__A51__	*/
#endif	/*	__AX51__	*/

/**********Standard SFR*************/
#define SFR(reg, addr)		sfr reg = addr
#define SFRC(reg, addr)     SFR(reg, addr)
#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
sfr reg = addr; \
sbit bit0 = reg^0; \
sbit bit1 = reg^1; \
sbit bit2 = reg^2; \
sbit bit3 = reg^3; \
sbit bit4 = reg^4; \
sbit bit5 = reg^5; \
sbit bit6 = reg^6; \
sbit bit7 = reg^7
#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#ifdef __C51__
/**********X SFR*************/
#define XSFR_ADDR(addr)  (*(volatile unsigned char xdata *)(addr))
#else
/**********X SFR*************/
#define XSFR_ADDR(addr)  (addr)

#endif	/*	__C51__	*/

#endif	/* __KEIL__ */

/***************************************************************************
 *                                - IAR - ICC
 ***************************************************************************/
#ifdef __IAR_SYSTEMS_ICC__
/**********Standard SFR*************/
#define SFR(reg,addr)	\
__sfr __no_init volatile unsigned char (reg) @ (addr)
#define SFRC(reg,addr)  SFR(reg,addr)

#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)	\
__sfr __no_init volatile union      \
{ \
    unsigned char reg; \
    struct \
    { \
		unsigned char bit0: 1; \
		unsigned char bit1: 1; \
		unsigned char bit2: 1; \
		unsigned char bit3: 1; \
		unsigned char bit4: 1; \
		unsigned char bit5: 1; \
		unsigned char bit6: 1; \
		unsigned char bit7: 1; \
    }; \
} @ addr
#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
/**********X SFR*************/
#define XSFR_ADDR(addr)  (*((volatile unsigned char __xdata *)(addr)))

#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#pragma language=extended
#endif	/* __IAR_SYSTEMS_ICC__ */


/***************************************************************************
 *                                - IAR - ASM
 ***************************************************************************/
/***************************************************************************
 *   Assembler definitions
 *
 *   The following SFRs are built in in the a8051.exe and can not be
 *   defined explicitly in this file:
 *     ACC,B,PSW,SP,DPL,DPH
 *   The PSW-bits are built-in in the a8051.exe
 *     OV,AC,F0,RS0,RS1,P
 *
 ***************************************************************************/
#ifdef __IAR_SYSTEMS_ASM__
#pragma language = default
/**********Standard SFR*************/
#define SFR(reg,addr)				reg DEFINE addr
#define SFRC(reg,addr)

SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
reg DEFINE addr
bit0 DEFINE addr.0
bit1 DEFINE addr.1
bit2 DEFINE addr.2
bit3 DEFINE addr.3
bit4 DEFINE addr.4
bit5 DEFINE addr.5
bit6 DEFINE addr.6
bit7 DEFINE addr.7
    ENDM
#define SFR_BIT(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)  \
SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SFR_BITC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
/**********X SFR*************/
#define XSFR_ADDR(addr) (addr)


#endif /* __IAR_SYSTEMS_ASM__*/


/***************************************************************************
 *                                SFR
 ***************************************************************************/
SFR_BIT(P0,     	0x80, P00, P01, P02, P03, P04, P05, P06, P07);
SFRC(SP ,		0x81);
SFR(DP0L ,		0x82);
SFR(DP0H ,		0x83);
SFR(DP1L ,		0x84);
SFR(DP1H ,		0x85);
SFR(DPCON ,		0x86);
SFR(SPH ,		0x87);
SFR(P0HD ,		0x88);
SFRC(SINT ,		0x89);
SFR(IP0L ,		0x8A);
SFR(IP0H ,		0x8B);
SFR(IP1L ,		0x8C);
SFR(IP1H ,		0x8d);
SFR(WKUPPND,		0x8e);
SFR(WKUPCON,		0x8f);

SFR_BIT(P1,		0x90, P10, P11, P12, P13, P14, P15, P16, P17);
SFR(PWR_CON ,		0x91);
SFR(WDT_CON ,		0x92);
SFR(BANK_SEL ,		0x93);
SFR(PBANK ,		0x94);
SFR(IE1 ,		0x95);
SFR(IO_MC0 ,		0x96);
SFR(IO_MC1 ,		0x97);
SFR(P1HD ,		0x98);
SFR(SPI_BUF,	    	0x99);	//R only
SFR(P2PD,	    	0x9A);
SFR(P3PD ,		0x9B);
SFR(P2PU ,		0x9C);
SFR(P3PU,	    	0x9D);
SFR(SD_CON0 ,		0x9E);
SFR(SD_CON1 ,		0x9F);

SFR_BIT(P2,     	0xA0, P20, P21, P22, P23, P24, P25, P26, P27);
SFR(IRFLT_CON ,		0xA1);
SFR(P0DIR ,		0xA2);
SFR(P1DIR ,		0xA3);
SFR(P2DIR ,	    	0xA4);
SFR(P3DIR ,    		0xA5);
SFR(LCD_CON0 ,    	0xA6);
SFR(LCD_CON1 ,		0xA7);
SFR_BIT(IE0,    0xA8, ET0, ET1, ET2, ET3, ERTC, ESOF, EUSB, EA);
SFR(SD_CON2 ,    	0xA9);
SFR(DAC_CON0 ,		0xAA);
SFR(DAC_CON1 ,		0xAB);
SFR(DAC_VLML ,		0xAC);	//W only
SFR(DAC_RDLL ,		0xAC);	//R only
SFR(DAC_VLMR ,		0xAD);	//W only
SFR(DAC_RDLH ,		0xAD);	//R only
SFR(SD_CPTR ,		0xAE);	//W only
SFR(DAC_RDRL ,		0xAE);	//R only
SFR(SD_DPTR ,	    	0xAF);	//W only
SFR(DAC_RDRH ,		0xAF);	//R only

SFR_BIT(P3,     	0xB0, P30, P31, P32, P33, P34, P35, P36, P37);
SFR(P4 ,		0xB1);
SFR(P4DIR ,		0xB2);
SFR(P4PU ,		0xB3);
SFR(P4PD ,		0xB4);
SFR(P4HD ,		0xB5);
SFR(P2HD ,		0xB6);
SFR(P3HD ,	    	0xB7);
SFR(P0DIE ,	    	0xB8);
SFR(P2DIE ,	    	0xB9);
SFR(P3DIE ,		0xBA);
SFR(P4DIE ,		0xBB);
SFR(CLK_CON0 ,		0xBC);
SFR(CLK_CON1 ,		0xBD);
SFR(CLK_CON2 ,		0xBE);
SFR(CLK_CON3 ,	    	0xBF);

SFR(P0PU ,		0xC0);
SFR(UART_CON ,		0xC1);
SFR(UART_STA ,		0xC2);
SFR(UART_BUF ,		0xC3);
SFR(MRES0 ,		0xC4);
SFR(MRES1 ,		0xC5);
SFR(MRES2 ,		0xC6);
SFR(MRES3 ,		0xC7);
SFR(P0PD ,		0xC8);
SFR(RTC_BUF ,		0xC9);
SFR(RTC_CON0,		0xCA);
SFR(RTC_CON1 ,		0xCB);	//W only
SFR(CHIP_VERSION,	0xCB);	//R only
SFR(ADC_CON0 ,		0xCC);
SFR(ADC_CON1 ,		0xCD); 	//W only
SFR(SPI_CON,		0xCE);
SFR(SPI_STA,		0xCF);

SFR_BITC(PSW,   	0xD0, P, Z, OV, RS0, RS1, F1, AC, CY);
SFR_BIT(PSW0,   	0xD0, PP, ZZ, OV1, RS00, RS11, SS, AC0, CY0);
SFR(USB_CON2 ,		0xD1);
SFR(TMR0_CON0 ,	    	0xD2);
SFR(TMR0_CON1 ,		0xD3);
SFR(TMR0_CNT ,   	0xD4);
SFR(TMR0_PRD ,   	0xD5);
SFR(TMR0_PWM0 ,		0xD6);
SFR(TMR0_PWM1 ,		0xD7);
SFR(P1DIE ,		0xD8);
SFR(CRC_FIFO ,		0xD9);	//W only
SFR(TMR1_CON0 ,	    	0xDA);
SFR(TMR1_CON1 ,		0xDB);
SFR(TMR1_CNT ,   	0xDC);
SFR(TMR1_PRD ,   	0xDD);
SFR(TMR1_PWM0 ,		0xDE);
SFR(TMR1_PWM1 ,		0xDF);

SFRC(ACC ,		0xE0);
SFR(CLK_GAT ,		0xE1);
SFR(TMR2_CON ,		0xE2);
SFR(TMR2_CNTL ,		0xE3);
SFR(TMR2_CNTH ,		0xE4);
SFR(TMR2_PRDL ,		0xE5);
SFR(TMR2_PRDH ,   	0xE6);
SFR(USB_DAT ,   	0xE7);
SFR(P1PU ,	     	0xE8);
SFR(USB_CON1 ,		0xE9); 	//W only
SFR(TMR3_CON ,		0xEA);
SFR(TMR3_CNTL ,		0xEB);
SFR(TMR3_CNTH ,		0xEC);
SFR(TMR3_PRDL ,		0xED);
SFR(TMR3_PRDH ,		0xEE);
SFR(DAA_CON1 ,		0xEF);

SFRC(BREG ,		0xF0);
SFR(HBITS ,		0xF1); 	//W only
SFR(ADC_DATL ,		0xF1);	//R only
SFR(MP_CON0 ,		0xF2); 	//W only
SFR(ADC_DATH ,		0xF2);	//R only
SFR(MP_CON1 ,		0xF3);
SFR(MP_CON2 ,		0xF4);
SFR(MP_CON3 ,		0xF5);
SFR(WPTRL ,		0xF6);
SFR(WPTRH ,		0xF7);
SFR(P1PD,    		0xF8);
SFR(DAA_CON2 ,		0xF9);
SFR(HUFF_CON0 ,		0xFA);
SFR(HUFF_CON1 ,		0xFB);
SFR(VLENL ,		0xFC);
SFR(VLENH ,		0xFD);
SFR(SFTOUTH ,		0xFE);	//R only
SFR(SFTOUTL ,		0xFF);	//R only

/***************************************************************************
 *                                XSFR
 ***************************************************************************/
#define OTP_CMD1        XSFR_ADDR(0xFF00 + 0x00) 	//W only 
#define OTP_CMD0        XSFR_ADDR(0xFF00 + 0x01)
#define OSC_DET_CON     XSFR_ADDR(0xFF00 + 0x03)
#define UART_BAUD       XSFR_ADDR(0xFF00 + 0x04)     	//W only
#define SD_CON3         XSFR_ADDR(0xFF00 + 0x05)    	//W only
#define LVD_CON         XSFR_ADDR(0xFF00 + 0x06)
#define STACK_LMT       XSFR_ADDR(0xFF00 + 0x07) 	//FPGA only/W only
#define ISD_CON         XSFR_ADDR(0xFF00 + 0x08)
#define ISD_BUF         XSFR_ADDR(0xFF00 + 0x09)
#define ISD_ADR         XSFR_ADDR(0xFF00 + 0x0a)	//W only
#define ISD_CNT         XSFR_ADDR(0xFF00 + 0x0b)	//W only
#define MODE_CON	XSFR_ADDR(0xFF00 + 0x0c)
#define PWM4_CON	XSFR_ADDR(0xFF00 + 0x0d)        //W only
#define CRC_REGL  	XSFR_ADDR(0xFF00 + 0x0e)
#define CRC_REGH  	XSFR_ADDR(0xFF00 + 0x0f)


#define USB_CON0    	XSFR_ADDR(0xFF00 + 0x10)
#define USB_EP0_ADR     XSFR_ADDR(0xFF00 + 0x14) 	//W only	
#define USB_EP0_CNT     XSFR_ADDR(0xFF00 + 0x15)	//W only
#define USB_EPX_CNT     XSFR_ADDR(0xFF00 + 0x16)	//W only
#define USB_EP1TX_ADR   XSFR_ADDR(0xFF00 + 0x17)	//W only
#define USB_EP1RX_ADR   XSFR_ADDR(0xFF00 + 0x18)    	//W only
#define EP1_RX_ADDRL    XSFR_ADDR(0xFF00 + 0x17)	//R only
#define EP1_RX_ADDRH    XSFR_ADDR(0xFF00 + 0x18)	//R only
#define USB_EP2TX_ADR   XSFR_ADDR(0xFF00 + 0x19)    	//W only
#define USB_EP2RX_ADR   XSFR_ADDR(0xFF00 + 0x1a)	//W only

#define LOFC_CON        XSFR_ADDR(0xFF00 + 0x20)
#define LOFC_PR         XSFR_ADDR(0xFF00 + 0x21)	//W only
#define LOFC_CNTH       XSFR_ADDR(0xFF00 + 0x22)	//R only		
#define LOFC_CNTM       XSFR_ADDR(0xFF00 + 0x23)	//R only		
#define LOFC_CNTL       XSFR_ADDR(0xFF00 + 0x24)	//R only		

#define BP_ADRL		XSFR_ADDR(0xFF00 + 0x26)	//W only
#define BP_ADRH		XSFR_ADDR(0xFF00 + 0x27)	//W only
#define PLL_CON0	XSFR_ADDR(0xFF00 + 0x28)	//W only
#define PLL_CON1	XSFR_ADDR(0xFF00 + 0x29)	//W only
#define PLL_CON2	XSFR_ADDR(0xFF00 + 0x2a)	//W only
#define SEG_EN0        	XSFR_ADDR(0xFF00 + 0x2b)	//W only
#define SEG_EN1        	XSFR_ADDR(0xFF00 + 0x2c)	//W only


#define DAA_CON0        XSFR_ADDR(0xFF00 + 0x30)
#define DAA_CON3        XSFR_ADDR(0xFF00 + 0x33)
#define DAA_CON4        XSFR_ADDR(0xFF00 + 0x34)
#define DAC_TRML        XSFR_ADDR(0xFF00 + 0x36) 	//W only
#define DAC_TRMR        XSFR_ADDR(0xFF00 + 0x37) 	//W only

#define OBUF_NUM        XSFR_ADDR(0xFF00 + 0x40)	//R only
#define OBUF_CON        XSFR_ADDR(0xFF00 + 0x41)
#define OBUF_DATL       XSFR_ADDR(0xFF00 + 0x42)	//W only, 16bit XSFR, accessable by ext instructions only
#define OBUF_DATR       XSFR_ADDR(0xFF00 + 0x44)	//W only, 16bit XSFR, accessable by ext instructions only
#define KV_DAT       	XSFR_ADDR(0xFF00 + 0x46)	//W only, 16bit XSFR, accessable by ext instructions only
#define KV_START        XSFR_ADDR(0xFF00 + 0x48) 	//W only
#define KV_END          XSFR_ADDR(0xFF00 + 0x49) 	//W only
#define KV_CNT          XSFR_ADDR(0xFF00 + 0x4a) 	//W only
#define KV_VLM          XSFR_ADDR(0xFF00 + 0x4b) 	//W only
#define IIC_CON         XSFR_ADDR(0xFF00 + 0x4c)
#define IIC_STA         XSFR_ADDR(0xFF00 + 0x4d)
#define IIC_BAUD        XSFR_ADDR(0xFF00 + 0x4e) 	//W only
#define IIC_BUF         XSFR_ADDR(0xFF00 + 0x4f)

#define SPI_BAUD	XSFR_ADDR(0xFF00 + 0x52)	//W only
#define SPI_CNT		XSFR_ADDR(0xFF00 + 0x54)	//W only
#define SPI_ADR		XSFR_ADDR(0xFF00 + 0x55)	//W only
#define LDO_CON		XSFR_ADDR(0xFF00 + 0x56)
#define HTC_CON		XSFR_ADDR(0xFF00 + 0x57)

#endif	/*	_CD002_H	*/

