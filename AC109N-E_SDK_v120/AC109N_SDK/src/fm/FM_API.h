#ifndef _FM_API_H_
#define _FM_API_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__

bool init_fm_rev(void);
bool set_fre(u8);
void fm_rev_powerdown(void);
void save_fre(u16 fre);
void fm_module_mute(u8 flag);
void fm_set_ch(u8 db);

u8 get_total_mem_channel(void);
u8 get_fre_via_channle(u8 channel);
u8 get_channel_via_fre(u8 fre);
void save_fm_point(u8 fre);
void clear_all_fm_point(void);
void ch_save(void);
void fm_scan_flag(bool flag);
void fm_info_init(void);
u8 fm_scan_msg(void);
bool fm_scan(u8 mode);

#endif  /*  __IAR_SYSTEMS_ICC__ */

#define FM_CUR_FRE          0
#define FM_FRE_DEC          1
#define FM_FRE_INC			2

#define FM_SCAN_STOP        0
#define FM_SCAN_PREV        1
#define FM_SCAN_NEXT		2
#define FM_SCAN_ALL         3

#define MAX_FRE    1080
#define MIN_FRE	   875
#define MAX_CHANNL (MAX_FRE - MIN_FRE + 1)

#endif  /*  _FM_API_H_   */
