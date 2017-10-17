#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "bike.h"


extern void MenuUpdate(BIKE_STATUS* bike);
extern void Delay(unsigned long nCount);
void LCD_show_volume(void);
void LCD_show_dev(void);
void LCD_show_music_main(void);
void LCD_show_fm_main(void);
void LCD_show_fm_station(void);
void LCD_show_filenumber(void);
void LCD_mute(void);
void LCD_unmute(void);
void LCD_stop(void);
#endif