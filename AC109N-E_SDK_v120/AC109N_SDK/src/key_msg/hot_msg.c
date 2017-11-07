/*--------------------------------------------------------------------------*/
/**@file    hot_msg.c
   @brief   系统公共消息处理函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "hot_msg.h"
#include "msg.h"
#include "device.h"
#include "get_music_file.h"
#include "main.h"
#include "key.h"
#include "iic.h"
#include "DAC.h"
#include "music_play.h"
#include "UI_API.h"
#include "RTC_API.h"
#include "play_file.h"
#include "break_point.h"
#include "dac.h"
#include "IRTC.h"


extern ENUM_WORK_MODE _data work_mode;
extern u8 _data main_vol_L, main_vol_R;
extern _no_init bool _bit mute;

_no_init u16 _data input_number;	    ///<输入的数字的实际值

/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
static void new_device_in(u8 dev) AT(CSTART)
{
    LED_FADE_ON();
    dev -= MSG_USB_DISK_IN;
    music_device_info[dev].wBreak_point_filenum = 0;
    music_device_info[dev].wfileTotal = 0;
    playfile.given_file_method = PLAY_BREAK_POINT;
    playfile.given_device = (ENUM_DEVICE_SELECT_MODE)dev;
    put_msg_lifo(MSG_MUSIC_NEW_DEVICE_IN);
}
/*----------------------------------------------------------------------------*/
/**@brief   公共消息集中处理函数
   @param   key： 需要处理的消息
   @return  void
   @author: Juntham
   @note    void ap_handle_hotkey(u8 key)
*/
/*----------------------------------------------------------------------------*/
void ap_handle_hotkey(u8 key) AT(CSTART)
{
    switch (key)
    {
#ifdef RTC_EN
    case MSG_ALM_ON:
#ifdef STANDBY_EN
        read_RTC((RTC_TIME *)(&curr_time));

        if((curr_time.bHour == curr_alarm.bHour)
            &&(curr_time.bMin == curr_alarm.bMin)
              &&(curr_alarm.bSw))
#endif
        {
#ifdef STANDBY_EN
            alm_on_flag = 1;
#endif
            UI_menu(MENU_ALM_UP);
            RTC_setting_var.bAlarmOn = 1;
            write_next_ALM();
            RTC_setting_var.bLastWorkMode = work_mode;
            if (work_mode != RTC_MODE)
            {
                work_mode = RTC_MODE;
                put_msg_lifo(MSG_CHANGE_WORK_MODE);     //必须使用高优先级消息，避免与解码错误消息冲突
            }
        }

        break;
#endif

    case MSG_MUTE:												//静音
        if (!Sys_Volume)
            break;
        dac_mute_toggle();
		UI_menu(MENU_MUTE);
        break;

    case MSG_USB_DISK_OUT:
    case MSG_SDMMCA_OUT:
#ifdef SDMMCB_EN
    case MSG_SDMMCB_OUT:
#endif
        LED_FADE_ON();
        /*当前活动设备不在线*/
        if ((device_check() == 0x0)&&(work_mode == MUSIC_MODE))
        {
            playfile.given_file_method = PLAY_BREAK_POINT;
            playfile.given_device = DEVICE_AUTO_NEXT;
            put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
            /*记忆断点信息*/
//#if defined BREAK_POINT_PLAY_EN && defined USE_EEPROM_MEMORY
#if defined BREAK_POINT_PLAY_EN && !defined USE_IRTC_MEMORY
            save_music_break_point(device_active);
#endif
        }
        set_memory(MEM_ACTIVE_DEV,NO_DEVICE);
        break;

#ifdef USB_DEVICE_EN
    case MSG_USB_PC_IN:

        LED_FADE_ON();
        work_mode = USB_DEVICE_MODE;
        put_msg_lifo(MSG_CHANGE_WORK_MODE);
        break;

#endif
    case MSG_AUX_IN :
        LED_FADE_ON();
#ifdef USB_DEVICE_EN
        if (work_mode != USB_DEVICE_MODE)
#endif
        {
            work_mode = AUX_MODE;
            put_msg_lifo(MSG_CHANGE_WORK_MODE);
        }
        break;

    case MSG_AUX_OUT :
        LED_FADE_ON();
        break;

#ifdef SDMMCA_EN
    case MSG_SDMMCA_IN :
#endif

#ifdef SDMMCB_EN
    case MSG_SDMMCB_IN :
#endif

#ifdef USB_DISK_EN
    case MSG_USB_DISK_IN  :
#endif
        new_device_in(key);
        break;

    case MSG_NEXT_WORKMODE:
        work_mode++;
        put_msg_lifo(MSG_CHANGE_WORK_MODE);
        break;

    case MSG_VOL_UP:
        flush_all_msg();
        if (!Sys_Volume)
            break;
        main_vol_L++;
        main_vol_L++;
    case MSG_VOL_DOWN:
        flush_all_msg();
        if (!Sys_Volume)
            break;
        main_vol_L--;
        if (main_vol_L == 255)
            main_vol_L = 0;
        else if (main_vol_L > MAX_MAIN_VOL_L)
            main_vol_L = MAX_MAIN_VOL_L;

        main_vol_R = main_vol_L;
        set_main_vol(main_vol_L, main_vol_R);
        dac_mute(0);
        //deg("Vol-L %u/R %u\n", main_vol_L, main_vol_R);
        set_memory(MEM_VOL_L, main_vol_L);
        set_memory(MEM_VOL_R, main_vol_R);
        UI_menu(MENU_MAIN_VOL);
		UI_menu(MENU_MUTE);
        break;

    case MSG_0:
    case MSG_1:
    case MSG_2:
    case MSG_3:
    case MSG_4:
    case MSG_5:
    case MSG_6:
    case MSG_7:
    case MSG_8:
    case MSG_9:
        if (!Sys_IRInput)
            break;

        if (input_number > 999)
            input_number = 0;

        input_number = input_number * 10 + key;
        UI_menu(MENU_INPUT_NUMBER);
        break;
    }
}

