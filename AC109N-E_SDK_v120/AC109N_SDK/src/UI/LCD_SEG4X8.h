#ifndef	_LCD_SEG_H_
#define _LCD_SEG_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__

typedef struct _LCDC_BUF	//<LCD ÏÔÊ¾»º´æ
{
    u8 	bCom0Seg1;
    u8 	bCom1Seg1;
    u8 	bCom2Seg1;
    u8 	bCom3Seg1;
    u8 	bCom4Seg1;
    u8 	bCom5Seg1;
}LCDC_BUF;

typedef struct _LCD_SEG4X8_VAR
{
    u8  bCoordinateX;       //<X ×ø±ê
    u8  bShowBuff[5];       //<ÏÔÊ¾»º´æ
    u8  bFlashChar;         //<×Ö·ûÎ»ÉÁË¸
    u8  bFlashIcon;         //<Í¼±êÉÁË¸
}LCD_SEG4X8_VAR;



void LCD_SEG4X8_init(void);
_near_func void LCD_SEG4X8_scan(void);
void LCD_SEG4X8_setX(u8 X);
void LCD_SEG4X8_clear_icon(void);
void LCD_SEG4X8_show_char(u8 chardata);
void LCD_SEG4X8_show_number(u8 number);
void LCD_SEG4X8_show_string(u8 *str);
void LCD_SEG4X8_show_Hi(void);
void LCD_SEG4X8_show_music_main(void);
void LCD_SEG4X8_show_RTC_main(void);
void LCD_SEG4X8_show_filenumber(void);
void LCD_SEG4X8_show_volume(void);
void LCD_SEG4X8_show_fm_main(void);
void LCD_SEG4X8_show_IR_number(void);
void LCD_SEG4X8_show_pc_main(void);
void LCD_SEG4X8_show_pc_vol_up(void);
void LCD_SEG4X8_show_pc_vol_down(void);
void LCD_SEG4X8_show_aux_main(void);
void LCD_SEG4X8_show_eq(void);
void LCD_SEG4X8_show_playmode(void);
void LCD_SEG4X8_show_pause(void);
void LCD_SEG4X8_show_fm_station(void);
void LCD_SEG4X8_show_waiting(void);


extern LCD_SEG4X8_VAR _idata LCD_SEG4X8_var;


extern LCDC_BUF __xdata LCDC_buf;


#define COM0_SEG0(x)	P0DIE = x
#define COM0_SEG1(x)	P1DIE = x

#define COM1_SEG0(x)	P0HD = x
#define COM1_SEG1(x)	P1HD = x

#define COM2_SEG0(x)	P0PD = x
#define COM2_SEG1(x)	P1PD = x

#define COM3_SEG0(x)	P0PU = x
#define COM3_SEG1(x)	P1PU = x

#define COM4_SEG0(x)	P0 = x
#define COM4_SEG1(x)	P1 = x

#define COM5_SEG0(x)	P0DIR = x
#define COM5_SEG1(x)	P1DIR = x



#ifdef LCD_SEG_4X8
/*------------------LCD 4 X 8 1/3 Bias--------------*/
/*
			SEG BIT MAP
	0	|	1	|	2	|	3	|	4	|	5 	|	6
	A		F		B		G		C		E		D
*/
/*-------------------------------------------------*/
//COM0
#define SEGA	BIT(0)

//COM1
#define SEGF	BIT(1)
#define SEGB	BIT(2)

//COM2
#define SEGG	BIT(3)
#define SEGC	BIT(4)

//COM3
#define SEGE	BIT(5)
#define SEGD	BIT(6)

#define SEGH	BIT(7)

#endif  /*  LCD_SEG_4X8 */

#ifdef LCD_SEG_3X9
/*------------------LCD 4 X 8 1/2 Bias--------------*/
/*
			SEG BIT MAP
	0	|	1	|	2	|	3	|	4	|	5 	|	6
	A		F		B		G		C		E		D
*/
/*-------------------------------------------------*/
//COM0
#define SEGA	BIT(0)
#define SEGB	BIT(1)

//COM1
#define SEGF	BIT(2)
#define SEGG	BIT(3)

//COM2
#define SEGE	BIT(4)
#define SEGC	BIT(5)

//COM3
#define SEGD	BIT(6)


#define SEGH	BIT(7)

#endif  /*  LCD_SEG_3X9 */

#endif	/*	__IAR_SYSTEMS_ICC__	*/

#endif	/*	_LCD_SEG_H_	*/
