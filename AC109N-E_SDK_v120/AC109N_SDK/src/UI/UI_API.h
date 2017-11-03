#ifndef _LED_UI_API_H_
#define _LED_UI_API_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__ 

enum
{
    MENU_POWER_UP = 0,
    MENU_WAIT,
#ifdef USB_DEVICE_EN     
    MENU_PC_MAIN,
    MENU_PC_VOL_UP,
    MENU_PC_VOL_DOWN,
#endif    
    MENU_AUX_MAIN,
#ifdef RTC_ALARM_EN
    MENU_ALM_UP,
#endif

    MENU_PLAY,
    MENU_PLAYMODE,
    MENU_MAIN_VOL,
    MENU_EQ,
    MENU_NOFILE,
    MENU_NODEVICE,
    MENU_PLAY_TIME,
    MENU_FILENUM,
    MENU_INPUT_NUMBER,
    MENU_MUSIC_MAIN,
    MENU_PAUSE,
    MENU_FM_MAIN,
    MENU_FM_DISP_FRE,
    MENU_FM_FIND_STATION,
    MENU_FM_CHANNEL,
    MENU_USBREMOVE,
    MENU_SDREMOVE,
    MENU_SCAN_DISK,
    
    MENU_RTC_MAIN,
    MENU_RTC_SET,
    MENU_RTC_PWD,
    MENU_ALM_SET,   
	MENU_ALM_REQUEST,
    MENU_MUTE,
    MENU_STOP,

    MENU_200MS_REFRESH = 0x80,
    MENU_100MS_REFRESH,
    MENU_SET_EQ,
    MENU_SET_PLAY_MODE,
    MENU_HALF_SEC_REFRESH,
	MENU_POWER_DOWN,
    MENU_MAIN = 0xFF,
};


typedef struct _UI_VAR
{
    u8  bCurMenu;
    u8  bMainMenu;
    u8  bMenuReturnCnt;
}UI_VAR;

extern _near_func void set_LED_fade_out(void);
extern _near_func void set_LED_all_on(void);


_near_func void UI_menu_api(u8 menu);
#ifdef UI_ENABLE
#define UI_menu(x)      UI_menu_api(x)
#define SET_UI_MAIN(x)  UI_var.bMainMenu = x
#else
#define UI_menu(...)
#define SET_UI_MAIN(...)
#endif

extern UI_VAR _data UI_var;   /*UI œ‘ æ±‰¡ø*/  

#endif  /*  __IAR_SYSTEMS_ICC__ */

#define UI_RETURN       5

#endif  /*  _LED_UI_API_H_  */
