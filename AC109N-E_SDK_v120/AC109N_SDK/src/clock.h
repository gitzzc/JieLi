#ifndef _CLOCK_H
#define _CLOCK_H

#include "config.h"
///*-----------------------------------------------------------
//////osc_out_sel
#define RTC_OSC             0
#define P00P01_OSC          1

/////pll_in_sel
#define PLLIN_32K           0
#define PLLIN_HTC           (1 << 4)
#define PLLIN_RTC_12M       (2 << 4)
#define PLLIN_P00P01_12M    (3 << 4)

/////system_clk_prein
#define PRE_SYSCLK_RTC_CLK  0
#define PRE_SYSCLK_RC256K   1
#define PRE_SYSCLK_HTC_CLK  2
#define PRE_SYSCLK_PLL_CLK  3

/////system_clk_sel
#define SYSCLK_RC256K       0
#define SYSCLK_MAIN_CLK     1

/////P05 config
#define P05_OSC_CLK          (0 << 2)
#define P05_SYS_CLK          (1 << 2)   
#define P05_HTC_CLK          (2 << 2)
#define P05_PLL_CLK          (3 << 2)
#define P05_NORMAL_IO        (4 << 2)

#define SYSTEM_MAIN_CLK         0x0
#define SYSTEM_CLK_DECELERATE   0x1

#define SYS_IDLE(X)         PWR_CON &= ~BIT(0);PWR_CON |= ((X & 0X1)<<0)
#define SYS_STANDBY(X)      PWR_CON &= ~BIT(1);PWR_CON |= ((X & 0X1)<<1)
#define SYS_SLEEP(X)        PWR_CON &= ~BIT(2);PWR_CON |= ((X & 0X1)<<2)


#ifdef __ICC8051__


_monitor void pll_init(void);
_near_func void pll_nrnf_cfg(u16 nr, u16 nf);
_near_func void system_clk_div(u8 div);
_near_func void otp_clk_div2(bool en);
_near_func void P05_config(u8 cfg);
_near_func void Timer0_init(u8 sys_clk);

void check_htc_otp(void);
void check_htc_32k(void);
void check_htc_12M(void);
bool check_htc_sof(u8 timeout);




#endif

#ifdef __IAR_SYSTEMS_ASM__
//extern pll_init
#endif


#endif
