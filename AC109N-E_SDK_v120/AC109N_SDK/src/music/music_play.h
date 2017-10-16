/*************************************************************/
/** @file:music_play.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-06-01,14:07
    @note:
*/
/*************************************************************/
#ifndef __MUSIC_PLAY_H__
#define __MUSIC_PLAY_H__
#include "config.h"

//OBUF复位及buffer大小设置
#define OBUF_REST       0x0
#define OBUF_0          0x1
#define OBUF_1          0x2
#define OBUF_2          0x3
#define OBUF_3          0x4

#ifdef __SMART_CPU__


enum
{
    NORMAL = 0,
    POP,
    ROCK,
    JAZZ,
    CLASSIC,
};


typedef struct _MUSIC_PLAY_VAR
{
    u8  bPlayStatus;    //<播放状态
    u8  bEQ;            //<EQ 音效
}MUSIC_PLAY_VAR;


void music_app(void);
void OFF_mode(void);
void set_eq(bool eq_en, u8 eq);
bool play_device_file(bool file_break_point);
extern u32 count_energy(u8* addr,u8 len);

extern _no_init bool _bit mem_update;
extern _no_init MUSIC_PLAY_VAR _idata Music_Play_var;
extern _no_init u32 _xdata music_energy;
#endif

#endif
