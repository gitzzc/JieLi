/*************************************************************/
/** @file:main.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-06-13,8:43
    @note:
*/
/*************************************************************/
#ifndef _MAIN_
#define _MAIN_

#include "config.h"

#ifdef __SMART_CPU__

typedef enum
{
    MUSIC_MODE = 0,
#ifdef USB_DEVICE_EN
    USB_DEVICE_MODE,
#endif
#ifdef FM_ENABLE 
    FM_RADIO_MODE,
#endif    
    AUX_MODE,
#ifdef RTC_EN     
    RTC_MODE,
#endif     
    MAX_WORK_MODE,
}ENUM_WORK_MODE;


extern _no_init bool _bit Sys_Volume;
extern _no_init bool _bit Sys_IRInput;
extern _no_init bool _bit Sys_HalfSec;
extern _no_init bool _bit alm_on_flag;
extern _no_init bool _bit set_sd_clk;


extern ENUM_WORK_MODE _data work_mode;
extern u8 _data main_vol_L, main_vol_R;
extern void (_near_func * _pdata int_enter_pro[16])(void);

#endif

#endif
