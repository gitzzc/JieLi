/*************************************************************/
/** @file:break_point.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-07-25,15:24
    @note:
*/
/*************************************************************/
#ifndef __BREAK_POINT_H__
#define __BREAK_POINT_H__
#include "config.h"


#ifdef __SMART_CPU__
void clear_music_break_point(u8 dev);
void save_music_break_point(u8 dev);
void save_music_break_point_rtc_ram(u8 dev);
_bool load_music_break_point(u8 dev);
void load_music_last_file(u8 dev);
void save_music_break_point_halfsec_refresh(u8 dev);
#endif

#endif
