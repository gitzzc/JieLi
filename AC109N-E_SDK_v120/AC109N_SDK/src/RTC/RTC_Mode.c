/*******************************************************************************************
File Name:  RTC_Mode.c

Version:    1.00

Discription: RTC 模式主循环

Author:     Caibingquan

Email:      change.tsai@gmail.com

Date:       2012.06.04

Copyright:(c) 2012 , All Rights Reserved.
*******************************************************************************************/
#include "config.h"
#ifdef RTC_EN

#include "RTC_Mode.h"
#include "msg.h"
#include "hot_msg.h"
#include "key.h"
#include "main.h"
#include "device.h"
#include "iic.h"
#include "UI_API.h"
#include "LCD.h"
#include "RTC_API.h"
#include "dac.h"
#include "IRTC.h"
#include "clock.h"

_no_init RTC_TIME __idata curr_time;
_no_init ALM_TIME __idata curr_alarm;
_no_init RTC_SETTING_VAR _idata RTC_setting_var;


/*----------------------------------------------------------------------------*/
/**@brief   IRTC 模式主循环
   @param   无
   @return  无
   @note    void RTC_mode_loop(void)
*/
/*----------------------------------------------------------------------------*/
void RTC_mode_loop(void)  AT(RTC_CODE)
{
    u8 msg;
#ifdef STANDBY_EN
    RTC_setting_var.bStandbyCnt = 0;
#endif

    while (1)
    {
        msg = app_get_msg();

        switch (msg)
        {
        case MSG_MUSIC_NEW_DEVICE_IN:
            work_mode = MUSIC_MODE;
        case MSG_CHANGE_WORK_MODE:
            return;

#ifdef RTC_ALARM_EN
        case MSG_MUSIC_PP:
            if (RTC_setting_var.bAlarmOn)
            {
                alarm_off();
                break;
            }
            /*--------RTC setting mode*/
            RTC_setting_var.bMode = (RTC_setting_var.bMode >= ALARM_SETTING) ? RTC_SHOW : RTC_setting_var.bMode+1;
            RTC_setting_var.bCoordinate = 0;

            if (RTC_setting_var.bMode == RTC_SHOW)
                UI_menu(MENU_RTC_MAIN);
            else if (RTC_setting_var.bMode == RTC_SETTING)
                UI_menu(MENU_RTC_SET);
            else if (RTC_setting_var.bMode == ALARM_SETTING)
                UI_menu(MENU_ALM_SET); 
            break;
        
        case MSG_MUSIC_PREV_FILE:
            RTC_coordinate_setting(1);
            break;
        case MSG_MUSIC_NEXT_FILE:
            RTC_coordinate_setting(0);
            break;  
        
        case MSG_VOL_UP:
            flush_all_msg();
            RTC_info_setting(1);
            break;
        case MSG_VOL_DOWN:
            flush_all_msg();
            RTC_info_setting(0);
            break;
            
#endif  /*RTC_ALARM_EN*/
            
        case MSG_HALF_SECOND:
            LED_FADE_OFF();
            UI_menu(MENU_MAIN);  
            read_RTC((RTC_TIME *)(&curr_time));
            RTC_overflow_check();
            alarm_on();
#ifdef STANDBY_EN
            if(alm_on_flag ==0) //闹铃响起时不进行standby操作
            {
                RTC_setting_var.bStandbyCnt++;
                if(RTC_setting_var.bStandbyCnt == 20) //系统无操作10秒后将系统时钟恢复原来的时钟
                {
                   RTC_setting_var.bStandbyCnt = 0;
                   LCD_standby_fun();
                }
            }
#endif
            //deg("Date :%u/%u/%u  ", curr_time.dYear, (u16)curr_time.bMonth, (u16)curr_time.bDay);
            //deg("%u:%u:%u * %u\n", (u16)curr_time.bHour, (u16)curr_time.bMin, (u16)curr_time.bSec, (u16)curr_time.bWeekday);
            UI_menu(MENU_HALF_SEC_REFRESH);
            break;   
          
        default:
            ap_handle_hotkey(msg);
            break;
        }
    }

}

/*----------------------------------------------------------------------------*/
/**@brief   IRTC 模式
   @param   无
   @return  无
   @note    void RTC_mode(void)
*/
/*----------------------------------------------------------------------------*/
void RTC_mode(void) AT(RTC_CODE)
{
    RTC_info_init();
    dac_channel_sel(DAC_DECODER);
    system_clk_div(CLK_1M);
    RTC_mode_loop();

#ifdef RTC_ALARM_EN
    alarm_off();
#endif
}

#endif
