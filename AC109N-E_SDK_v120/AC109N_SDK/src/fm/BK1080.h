/*--------------------------------------------------------------------------*/
/**@file     bk1080.h
   @brief    BK1080 ’“Ù
   @details
   @author
   @date   2011-3-30
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _BK_1080_H_
#define _BK_1080_H_

#include "config.h"

#ifdef __SMART_CPU__

#define XTAL_CLOCK			0
#define CHIP_DEV_ID 		0x80


void init_BK1080(void);
void BK1080_SetFreq(u16 curFreq);
bool set_fre_BK1080(u16 freq);
void BK1080_PowerDown(void);
void BK1080_mute(u8 flag);
bool BK1080_Read_ID(void);
void BK1080_setch(u8 db);

#endif		/*  __C51__ */

#endif		/*  _BK_1080_H_ */
