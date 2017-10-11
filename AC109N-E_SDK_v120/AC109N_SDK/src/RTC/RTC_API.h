#ifndef _RTC_API_H_
#define _RTC_API_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__
enum
{
    RTC_SHOW = 0,
    RTC_SETTING,
    ALARM_SETTING,
};

typedef struct _RTC_SETTING_VAR
{
    u8  bMode;          //<0：RTC 显示模式 1：RTC 设置模式 2：Alarm 设置模式
    u8  bCoordinate;    //<设置坐标
    u8  bAlarmOn;       //<闹钟响闹
    u8  bLastWorkMode;  //<进入闹钟前的工作模式
    u8  bStandbyCnt;
}RTC_SETTING_VAR;

typedef struct _ALM_VOL_INFO
{
    u8  bRtcVolr;    //<响闹前的右声道音量
    u8  bRtcVoll;    //<响闹前的左声道音量
    u8  bMute;       //<1：mute  0:unmunt
}ALM_VOL_INFO;

#define IRTC_X2IE(x)		RTC_CON0 = ((RTC_CON0 & ~(BIT(2))) | (x & 0x01)<<2)
#define IRTC_X512IE(x)		RTC_CON0 = ((RTC_CON0 & ~(BIT(4))) | (x & 0x01)<<4)
#define IRTC_WKIE(x)	    RTC_CON0 = ((RTC_CON0 & ~(BIT(6))) | (x & 0x01)<<6)

#define IRTC_X2CLRCPND		RTC_CON1 = BIT(3)
#define IRTC_X512CLRCPND	RTC_CON1 = BIT(5)
#define IRTC_WKCLRPND       RTC_CON1 = BIT(7)


void write_next_ALM(void);
void reset_RTC_time(void);
void alarm_enable(u8 cmd);
void IRTC_init(void);
void RTC_info_init(void);
void LCD_RTC_coordinate_setting(u8);
void LCD_RTC_info_setting(u8);

void LED_RTC_coordinate_setting(u8);
void LED_RTC_info_setting(u8 dir);
void Alarm_setting_mode(u8 dir);
__monitor void standby_fun(void);
void alarm_on(void);
void alarm_off(void);



#ifdef STANDBY_EN
#define LCD_standby_fun()   standby_fun()
#else
#define LCD_standby_fun()
#endif

#ifdef UI_ENABLE
#if defined LCD_96X32_SERIAL || defined LCD_128X32_SERIAL \
  || defined LCD_128X64_SERIAL || defined LCD_128X64_PARALLEL 
#define RTC_coordinate_setting(x)   LCD_RTC_coordinate_setting(x)
#define RTC_info_setting(x)         LCD_RTC_info_setting(x)
#endif

#if defined LED_5X7 ||defined LCD_SEG_5X9
#define RTC_coordinate_setting(x)   LED_RTC_coordinate_setting(x)
#define RTC_info_setting(x)         LED_RTC_info_setting(x)
#endif

#if defined LCD_SEG_4X8 
#define RTC_coordinate_setting(x)   
#define RTC_info_setting(x)         
#endif

#else 
#define RTC_coordinate_setting(...)   
#define RTC_info_setting(...)         

#endif

void RTC_overflow_check(void);

extern RTC_SETTING_VAR _idata RTC_setting_var;
extern _no_init ALM_VOL_INFO _idata ALM_vol_info;


#endif  /*  __IAR_SYSTEMS_ICC__ */

#define WRITE_RTC 			0x50
#define READ_RTC  			0x40
#define WRITE_ALM 			0x38
#define READ_ALM  			0x39
#define WRITE_SR0 			0x3A
#define READ_SR0			0x3B
#define WRITE_RAM 			0x3C
#define READ_RAM			0x3D
#define WRITE_SR1 			0x3E
#define READ_SR1 			0x3F

#endif  /*  _RTC_API_H_   */
