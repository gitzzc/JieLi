#ifndef	_LCD_SEG5X9_H_
#define _LCD_SEG5X9_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__

typedef struct _LCDC5X9_P1BUF	//<LCD P1ÏÔÊ¾»º´æ
{
    u8 	bCom0Seg1;
    u8 	bCom1Seg1;
    u8 	bCom2Seg1;
    u8 	bCom3Seg1;
    u8 	bCom4Seg1;
}LCDC5X9_P1BUF;

typedef struct _LCDC5X9_P0BUF	//<LCD P0ÏÔÊ¾»º´æ
{
    u8 	bCom0Seg0;
    u8 	bCom1Seg0;
    u8 	bCom2Seg0;
    u8 	bCom3Seg0;
    u8 	bCom4Seg0;
}LCDC5X9_P0BUF;

typedef struct _LCD_SEG5X9_VAR
{
    u8  bCoordinateX;       //<X ×ø±ê
    u8  bShowBuff[4];       //<ÏÔÊ¾»º´æ
    u8  state[3];
    u8  bFlashChar;         //×Ö·ûÎ»ÉÁË¸
    u8  bFlashIcon;         //<Í¼±êÉÁË¸
}LCD_SEG5X9_VAR;

_near_func void LCD_SEG5X9_init(void);
_near_func void LCD_drive5X9_12(void);
_near_func void LCD_SEG5X9_clear(void);
_near_func void LCD_SEG5X9_show_Hi(void);
void LCD_SEG5X9_show_music_main(void);
void LCD_SEG5X9_show_filenumber(void);
void LCD_SEG5X9_show_volume(void);
void LCD_SEG5X9_show_waiting(void);
void LCD_SEG5X9_show_IR_number(void);
void LCD_SEG5X9_show_eq(void);
void LCD_SEG5X9_show_playmode(void);
void LCD_SEG5X9_show_pause(void);
void LCD_SEG5X9_show_pc_main(void);
void LCD_SEG5X9_show_pc_vol_up(void);
void LCD_SEG5X9_show_pc_vol_down(void);
void LCD_SEG5X9_show_aux_main(void);
void LCD_SEG5X9_show_RTC_main(void);
void LCD_SEG5X9_show_fm_main(void);
void LCD_SEG5X9_show_fm_station(void);
_near_func void LCD_SEG5X9_show_alarm(void);
void LCD_SEG5X9_show_string_menu(u8 menu);

extern LCD_SEG5X9_VAR _idata LCD_SEG5X9_var;

#define LCD_STATUS  LCD_SEG5X9_var.bShowBuff[4]

#define COM0_SEG00(x)	P0DIE = (P0DIE & ~BIT(0)) | ((x & 0x01))
#define COM0_SEG01(x)	P1DIE = x

#define COM1_SEG00(x)	P0HD = (P0HD & ~BIT(0)) | ((x & 0x01))
#define COM1_SEG01(x)	P1HD = x

#define COM2_SEG00(x)	P0PD = (P0PD & ~BIT(0)) | ((x & 0x01))
#define COM2_SEG01(x)	P1PD = x

#define COM3_SEG00(x)	P0PU = (P0PU & ~BIT(0)) | ((x & 0x01))
#define COM3_SEG01(x)	P1PU = x

#define COM4_SEG00(x)	P0 = (P0 & ~BIT(0)) | ((x & 0x01))
#define COM4_SEG01(x)	P1 = x

#define COM5_SEG00(x)	P0DIR = (P0DIR & ~BIT(0)) | ((x & 0x01))
#define COM5_SEG01(x)	P1DIR = x



#ifdef LCD_SEG_5X9
/*------------------LCD 5 X 9 1/3 Bias--------------*/
/*
			SEG BIT MAP
	0	|	1	|	2	|	3	|	4	|	5 	|	6
	A		F		B		G		C		E		D
*/
/*-------------------------------------------------*/
//COM0
#define SEGA	BIT(0)
#define SEGF	BIT(1)

//COM1
#define SEGB	BIT(2)
#define SEGG	BIT(3)

//COM2
#define SEGC	BIT(4)
#define SEGE	BIT(5)

//COM3
#define SEGD	BIT(6)

#define SEGH	BIT(7)

#define ALARM_ICON	BIT(5)
#define SD_ICON	    BIT(7)
#define USB_ICON	BIT(7)
#define COL_ICON	BIT(3)
#define REP_ICON	BIT(2)
#define FM_ICON	    BIT(3)
#define ONE_ICON	BIT(1)
#define MHZ_ICON	BIT(6)
#define AUX_ICON	BIT(3)

#endif  /*  LCD_SEG_4X8 */
#endif  /*  LCD_SEG_4X8 */
#endif  /*  LCD_SEG_4X8 */
