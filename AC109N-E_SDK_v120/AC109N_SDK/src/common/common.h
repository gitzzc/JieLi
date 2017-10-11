/*************************************************************/
/** @file:entry.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-06-09,15:27
    @note:
*/
/*************************************************************/
#ifndef _ENTRY_H
#define _ENTRY_H

#include "typedef.h"

#ifdef __SMART_CPU__

_near_func void CHANGE_CODE_BANK(u8);
_near_func void bank_switch(u8);
_near_func void dec_delay_counter(void);
_near_func void my_printf(const u8 __code *format, ...);
_near_func void my_memcpy(u8 _xdata *s1, u8 _xdata *s2, u16 len);
_near_func void my_memcpy_tail(u8 _xdata *s1, u8 _xdata *s2, u16 len);
_near_func void my_memset(u8 _xdata *s1, u8 volume, u16 len);
_near_func void delay8(u8);
_near_func void delay16(u16);
_near_func void delay_n10ms(u8);
_near_func _monitor void put_msg_fifo(u8 msg);
_near_func _monitor void put_msg_lifo(u8 msg);
_near_func _monitor u8 get_msg(void);
_near_func _monitor u8 get_msg_v10(void);
_near_func _monitor void flush_all_msg(void);
_near_func bool check_event(u8 event);
_near_func void put_event(u8 event);
_near_func void clear_one_event(u8 event);
_near_func void clear_all_event(void);
_near_func u8 get_event(void);
_near_func void printf_buf(u8 _xdata *buf, u16 len);
_near_func void set_putchar(void(_near_func *fun)(u8));
_near_func void set_debug_mode(bool flag);
_near_func u16 get_win_buffer_address(void);

_near_func u8 get_one_count(u8);
_near_func u16 get_crc16(u32 num);
_near_func void config_wdt(u8 dat);
_near_func void SET_DAC_OFFSET(s16);
_near_func bool music_detect(u8 energy);

_near_func u8 dummy_func8(u8);

_near_func void spi_read(void);
_near_func void set_Audio_volume_V10(u8 chl, u8 chr);
_near_func u16 usb_class_info_ptr(void);

#endif

#ifdef __IAR_SYSTEMS_ASM__
extern deg
#endif


#endif
