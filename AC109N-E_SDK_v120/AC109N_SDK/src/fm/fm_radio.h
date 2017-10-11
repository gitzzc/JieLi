/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
    @date: 2012-06-13,15:51
*/
/*----------------------------------------------------------------------------*/
#ifndef __FM_RADIO_H__
#define __FM_RADIO_H__

#include "config.h"

#ifdef __SMART_CPU__

typedef struct _FM_MODE_VAR
{
    u16 wFreq;          ///<当前频点
    u8  bFreChannel; 	///<总台数
    u8  bTotalChannel;  ///<当前频道
    u8  bAddr;	        ///<在线的FM收量的地址指针
}FM_MODE_VAR;

extern FM_MODE_VAR _data fm_mode_var;
extern _no_init u8 _data scan_mode;
void fm_play(void);
void fm_mode(void);

#endif

#endif
