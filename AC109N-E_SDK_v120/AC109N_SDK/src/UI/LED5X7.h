#ifndef	_LED_H_
#define _LED_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__

typedef struct _LED5X7_VAR
{
    u8  bCoordinateX;       //<X ×ø±ê
    u8  bFlashChar;         //<×Ö·ûÎ»ÉÁË¸
    u8  bFlashIcon;         //<Í¼±êÉÁË¸
    u8  bShowBuff[5];       //<ÏÔÊ¾»º´æ
    u8  bBrightness;        //<ÁÁ¶È¿ØÖÆ
}LED5X7_VAR;


_near_func void set_LED_fade_out(void);
_near_func void set_LED_all_on(void);

void LED5X7_init(void);
_near_func void LED5X7_scan(void);
void LED5X7_clear_icon(void);
void LED5X7_show_char(u8 chardata);
void LED5X7_show_number(u8 number);
void LED5X7_show_Hi(void);
void LED5X7_show_music_main(void);
void LED5X7_show_RTC_main(void);
void LED5X7_show_filenumber(void);
void LED5X7_show_volume(void);
void LED5X7_show_fm_main(void);
void LED5X7_show_IR_number(void);
void LED5X7_show_pc_main(void);
void LED5X7_show_pc_vol_up(void);
void LED5X7_show_pc_vol_down(void);
void LED5X7_show_aux_main(void);
void LED5X7_show_eq(void);
void LED5X7_show_playmode(void);
void LED5X7_show_pause(void);
void LED5X7_show_fm_station(void);
void LED5X7_show_waiting(void);
void LED5X7_show_alarm(void);

extern LED5X7_VAR _idata LED5X7_var;

#define LED_STATUS  LED5X7_var.bShowBuff[4]

#define LED_A   BIT(0)
#define LED_B	BIT(1)
#define LED_C	BIT(2)
#define LED_D	BIT(3)
#define LED_E	BIT(4)
#define LED_F	BIT(5)
#define LED_G	BIT(6)
#define LED_H	BIT(7)

//for LED0
#define LED_PLAY	LED_A
#define LED_PAUSE	LED_B
#define LED_USB		LED_C
#define LED_SD		LED_D
#define LED_2POINT	LED_E
#define LED_MP3		LED_F
#define LED_MHZ	    LED_G
#define LED_FM		LED_H

#define LED_COM1   P30
#define LED_COM2   P31
#define LED_COM3   P32
#define LED_COM4   P33
#define LED_COM5   P34

#define LED_COM    P3

#define LED_COM_MASK   P3PU

#define LED_SEG    P1


#endif	/*	__IAR_SYSTEMS_ICC__	*/

#endif	/*	_LED_H_	*/
