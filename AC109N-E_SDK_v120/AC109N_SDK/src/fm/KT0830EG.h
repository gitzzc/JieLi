/*--------------------------------------------------------------------------*/
/**@file   KT0830EG.h
   @brief  KT0830驱动头文件
   @details
   @author
   @date   2011-9-8
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef _KT_0830_H_
#define _KT_0830_H_


#include "config.h"


#ifdef __SMART_CPU__

typedef struct _FM_KT0830_VAR
{
    u8  mem_afc[3];	
    u16 mem_freq[3]; 
    u8  mem_vol;
}FM_KT0830_VAR;

void KT_FMAntTuneOn(void);
void KT0830_PowerDown(void);

void init_KT0830(void);
bool set_fre_KT0830(u16 Frequen);
void KT0830_PowerDown(void);
void KT0830_mute(u8 flag);
bool KT0830_Read_ID(void);
void KT0830EG_setch(u8 db);


#define KT_IIC_readn(a, b, c, d)    \
  iic_readKT(a, b, c, d)



#endif


#define KTFMw_address 	0x6E
#define KTFMr_address 	0x6F

#define IND_MATCH  		90				// Unit is nH
#define CEXT_MATCH 		13000			// Unit if fF
#define INDUCTOR_Q 		1				// 3 for highest Q;0 for lowest Q
#define INIT_FAIL_TH 	3
#define SEEK_UP_LIMIT 	10800
#define SEEK_DOWN_LIMIT 8700
#define LONG_ANT 		1
#define SHORT_ANT 		0
#define CHANNEL_STEP 	10
#define AFCTH_PREV 		10//5				// Range from 0 to 127
#define AFCTH 			15					// Range from 0 to 127
#define AFCTH_NEXT 		10//5				// Range from 0 to 127
#define TST_TH 			0x4f00
#define SNR_TH 			0x30


#define SEEK_WITH_SNR				// Turn on only if SNR double check is required in SEEK process	
#define AUDIO_LEVEL_BOOST

#endif
