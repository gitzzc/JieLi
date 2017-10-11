/*************************************************************/
/** @file:hot_msg.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-06-13,8:53
    @note:
*/
/*************************************************************/
#ifndef _HOT_MSG_
#define _HOT_MSG_

#include "config.h"


#ifdef __SMART_CPU__
void ap_handle_hotkey(u8 key);

extern u16 _data input_number;
#endif

#endif
