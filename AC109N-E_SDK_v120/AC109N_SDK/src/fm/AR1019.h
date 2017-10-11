#ifndef _AR1019_H_
#define _AR1019_H_

#include "config.h"

#ifdef __SMART_CPU__

void init_AR1019(void);
bool set_freq_AR1019(u16 freq);
void AR1019_mute(u8 mute);
bool AR1019_Read_ID(void);
void AR1019_powerdown(void);
void AR1019_set_vol (u8 vol);
void AR1019_setch(u8 db);
#endif

//#define RDA5807_WR_ADDRESS  0x20       ///<RDA5807 写地址
//#define RDA5807_RD_ADDRESS  0x21       ///<RDA5807 读地址
#define UEBAND			0x0000 // US & Europe BAND 87.5MHz to 108MHz
#define JBAND			0x1000 // Japen BAND 76MHz to 90MHz
#define JWBAND			0x1800 // Japen BAND (wide) 76 MHz to 108MHz

#define BAND_USA		0x01		// default: Eur & U.S
#ifdef  BAND_USA						// Eur & U.S
//#define MIN_FREQUENCE		875
//#define MAX_FREQUENCE		1080
#else								// Japan
//#define MIN_FREQUENCE		760
//#define MAX_FREQUENCE		910
#endif


//#define SPACE100K		1
//#define SPACE200K		0
//#define SEEKUP			1
//#define SEEKDOWN 		0

/* General propose constant */
#define AR1018_WRITE          	0x20
#define AR1018_READ           	0x21
#define ADDR_STATUS		0x13 // the address of status register
#define MASK_STC 		0x0020  // Seek/Tune/PowerOn complete  D5 in  adress 13H 
#define MASK_SF	 		0x0010  //  Seek Fail D4 in address 13H
//#define MASK_READCHAN 0xFF80 // D7~D15 in address 13H
//#define SHIFT_READCHAN 7

//#define ADDR_RSSI		0x12 
//#define MASK_RSSI 		0xFE00
//#define SHIFT_RSSI  		9

#define FM_CLOCK                    USE_CRYSTAL_AS_FM_CLOCK
#define USE_P05_AS_FM_CLOCK           1
#define USE_CRYSTAL_AS_FM_CLOCK       2  //共用晶振
#define USE_MODULE                    3  //使用模块,FM芯片自起振晶振的情况都属于这个模式

/*FM Modul*/
#define SYS_CRY     CRY_32K

#define CRY_32K     1
#define CRY_12M     2
#define CRY_24M     3

#endif
