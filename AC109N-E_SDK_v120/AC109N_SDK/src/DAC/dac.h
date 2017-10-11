/*******************************************************************************************
File Name:  DAC.h

Version:    1.00

Discription: DAC 模块头文件

Author:     Caibingquan

Email:      change.tsai@gmail.com

Date:       2012.06.04

Copyright:(c) 2012 , All Rights Reserved.
*******************************************************************************************/
#ifndef	_DAC_H_
#define _DAC_H_


#include "config.h"

#define DAC_DECODER 0
#define DAC_AMUX0   1
#define DAC_AMUX1   2

#ifdef __SMART_CPU__

extern _no_init bool _bit music_mute;

_near_func void Timer0_init(u8 sys_clk);
void key_tone_init(void);
void DAC_init(void);
void set_digital_vol(u8 L_vol, u8 R_vol);
void set_main_vol(u8 L_vol, u8 R_vol);
void set_analog_vol(u8 L_vol, u8 R_vol);
_near_func void set_key_tone(void);
_near_func void dac_check(void);
_near_func void dac_fade_in(void);
_near_func void dac_fade_out(void);
void dac_on(bool vcom_outen, bool dac_vdden);
void dac_on_asm(void);
_near_func void dac_channel_sel(u8 channel);
void dac_mute(bool flag);
void dac_mute_toggle(void);
void DAC_setting(void);
void dac_off(bool foating);

extern _no_init u8 _data system_clk;
extern _no_init bool _bit mute;
#ifdef KEY_VOICE_EN
#define kick_key_voice()        set_key_tone()
#else
#define kick_key_voice()
#endif


#endif


#endif
