#ifndef _RDA_5807_H_
#define _RDA_5807_H_

#include "config.h"

#ifdef __SMART_CPU__

typedef struct _FM_RDA5807_VAR
{
    u8  rda5807_dat[12];
    u8  read_dat[10];
    u8  fm_type;
}FM_RDA5807_VAR;


void init_RDA5807(void);
bool set_fre_RDA5807(u16 fre);
void RDA5807_PowerDown(void);
void RDA5807_mute(u8 dir);
bool RDA5807_Read_ID(void);
void RDA5807_setch(u8 db);

extern u8 _idata rda5807_dat[12];
extern u8 _xdata read_dat[10];
extern u8 _xdata fm_type;

#define RDA5807_WR_ADDRESS  0x20       ///<RDA5807 写地址
#define RDA5807_RD_ADDRESS  0x21       ///<RDA5807 读地址

#define RDA5807_RSSI  0x4

#define rda5807_stc() (read_dat[0] & (1 << 6))       ///<锁频结束
#define rda5807_true() (read_dat[2] & (1 << 0))      ///<是否有台
#define rda5807_rssi() ((read_dat[2] >> 1))          ///<接收信号强度

#endif

#endif
