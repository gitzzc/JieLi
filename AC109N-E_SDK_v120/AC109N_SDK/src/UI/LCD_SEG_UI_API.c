/*--------------------------------------------------------------------------*/
/**@file    LCD_SEG_UI_API.c
   @brief   LCD ��������ʾ����ӿں���
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "UI_API.h"
#include "device.h"

#if defined LCD_SEG_5X9
#include "msg.h"
#include "get_music_file.h"
#include "hot_msg.h"
#include "UI_common.c"
#include "LCD_SEG5X9.h"
#include "LCD_SEG5X9.c"
#include "RTC_API.h"

_no_init UI_VAR _data UI_var;   /*UI ��ʾ����*/

/*----------------------------------------------------------------------------*/
/**@brief   UI ��ʾ���洦����
   @param   menu����Ҫ��ʾ�Ľ���
   @return  ��
   @note    void UI_menu_api(u8 menu)
*/
/*----------------------------------------------------------------------------*/
_near_func void UI_menu_api(u8 menu) AT (COMMON_CODE)
{
    /*��������-���������Զ�����*/
    if (menu == MENU_MAIN)
    {
        if (UI_var.bMenuReturnCnt)
        {
            UI_var.bMenuReturnCnt++;
            if (UI_var.bMenuReturnCnt > UI_RETURN)
            {
                if (UI_var.bCurMenu == MENU_INPUT_NUMBER)
                    put_msg_fifo(MSG_INPUT_TIMEOUT);    //���볬ʱ
                else
                    UI_var.bCurMenu = UI_var.bMainMenu;
                UI_var.bMenuReturnCnt = 0;
            }
        }
        else
        {
            /*�ȴ����治�ظ�ˢ�½���*/
            if (UI_var.bCurMenu == UI_var.bMainMenu)
                return;
            UI_var.bCurMenu = UI_var.bMainMenu;
        }
    }
    else
    {
        if (menu > 0x80)    //���ڵ�ǰ����Ϊ������ʱˢ�½���,���磺��������ˢ�²���ʱ��
        {
            if (UI_var.bCurMenu != UI_var.bMainMenu)
              return;
        }
        else
        {
            UI_var.bCurMenu = menu;
            if (menu != UI_var.bMainMenu)
            {
                UI_var.bMenuReturnCnt = 0x1;   //�������濪ʼ���ؼ���
            }
            if (menu != MENU_INPUT_NUMBER)
              input_number = 0;
        }
    }
    LCD_SEG5X9_clear();

    if(MENU_HALF_SEC_REFRESH != menu)
    {
        LCD_CON0 &= ~BIT(1);	            //[1]:DOTEN COM0-SEG0
        LCDC5X9_P0buf.bCom0Seg0 = 0;        //Ϩ��":"�����
    }

    switch(UI_var.bCurMenu)
    {
    /*-----System Power On UI*/
    case MENU_AUX_MAIN:
        LCD_SEG5X9_var.state[0] |= AUX_ICON; //AUXͼ��
    case MENU_POWER_UP:
    case MENU_WAIT:
#ifdef USB_DEVICE_EN
    case MENU_PC_MAIN:
    case MENU_PC_VOL_UP:
    case MENU_PC_VOL_DOWN:
#endif
#ifdef RTC_ALARM_EN
    case MENU_ALM_UP:
#endif
      LCD_SEG5X9_show_string_menu(UI_var.bCurMenu);
      break;

    case MENU_PAUSE:
        LCDC5X9_P0buf.bCom0Seg0 = 0x01; //����":"�����
        LCD_SEG5X9_show_music_main();
    break;

    case MENU_INPUT_NUMBER:
      LCD_SEG5X9_show_IR_number();
      break;

    /*-----Common Info UI*/
    case MENU_MAIN_VOL:
      LCD_SEG5X9_show_volume();
      break;

    /*-----Music Related UI*/
    case MENU_MUSIC_MAIN:
      LCD_SEG5X9_show_music_main();
      break;

    case MENU_EQ:
      LCD_SEG5X9_show_eq();
      break;

    case MENU_PLAYMODE:
      LCD_SEG5X9_show_playmode();
      break;

    case MENU_FILENUM:
      LCD_SEG5X9_show_filenumber();
      break;

   /*-----FM Related UI*/
#ifdef FM_ENABLE
    case MENU_FM_MAIN:
    case MENU_FM_DISP_FRE:
      LCD_SEG5X9_show_fm_main();
      break;
    case MENU_FM_FIND_STATION:
    case MENU_FM_CHANNEL:
      LCD_SEG5X9_show_fm_station();
      break;
#endif
   /*-----RTC Related UI*/
#ifdef RTC_EN
    case MENU_RTC_MAIN:
      RTC_setting_var.bMode = 0;    //ģʽ�����ͬ������
    case MENU_RTC_SET:
       LCD_SEG5X9_show_RTC_main();
      break;
#ifdef RTC_ALARM_EN
    case MENU_ALM_SET:
      LCD_SEG5X9_show_alarm();
      break;
#endif
#endif
    default:
      break;
    }
    LCD_drive5X9_12();
}

#else if defined LCD_BL55072

#include "msg.h"
#include "get_music_file.h"
#include "hot_msg.h"
#include "UI_common.c"
#include "stdlib.h"
#include "display.h"
#include "bl55072.c"
#include "display_8587.c"
#include "RTC_API.h"

_no_init UI_VAR _data UI_var;   /*UI ��ʾ����*/
extern _no_init bool _bit mute;	//dac.c

/*----------------------------------------------------------------------------*/
/**@brief   UI ��ʾ���洦����
   @param   menu����Ҫ��ʾ�Ľ���
   @return  ��
   @note    void UI_menu_api(u8 menu)
*/
/*----------------------------------------------------------------------------*/
_near_func void UI_menu_api(u8 menu) AT (COMMON_CODE)
{
    /*��������-���������Զ�����*/
    if (menu == MENU_MAIN)
    {
        if (UI_var.bMenuReturnCnt)
        {
            UI_var.bMenuReturnCnt++;
             //deg_puts("bMenuReturnCnt ++\n");
            if (UI_var.bMenuReturnCnt > UI_RETURN)
            {
                if (UI_var.bCurMenu == MENU_INPUT_NUMBER)
                    put_msg_fifo(MSG_INPUT_TIMEOUT);    //���볬ʱ
                else
                    UI_var.bCurMenu = UI_var.bMainMenu;
                UI_var.bMenuReturnCnt = 0;
            }
        }
        else
        {
            /*�ȴ����治�ظ�ˢ�½���*/
            //if (UI_var.bCurMenu == UI_var.bMainMenu)
            //    return;
            UI_var.bCurMenu = UI_var.bMainMenu;
        }
    }
    else
    {
        if (menu > 0x80)    //���ڵ�ǰ����Ϊ������ʱˢ�½���,���磺��������ˢ�²���ʱ��
        {
            if (UI_var.bCurMenu != UI_var.bMainMenu)
              return;
        }
        else
        {
            UI_var.bCurMenu = menu;
            if (menu != UI_var.bMainMenu)
            {
                //deg_puts("bMenuReturnCnt = 1\n");
                UI_var.bMenuReturnCnt = 0x1;   //�������濪ʼ���ؼ���
            }
            if (menu != MENU_INPUT_NUMBER)
              input_number = 0;
        }
    }
    //LCD_SEG5X9_clear();

    /*if(MENU_HALF_SEC_REFRESH != menu)
    {
        LCD_CON0 &= ~BIT(1);	            //[1]:DOTEN COM0-SEG0
        LCDC5X9_P0buf.bCom0Seg0 = 0;        //Ϩ��":"�����
    }*/

    switch(UI_var.bCurMenu)
    {
    /*-----System Power On UI*/
    case MENU_AUX_MAIN:
        //LCD_SEG5X9_var.state[0] |= AUX_ICON; //AUXͼ��
      break;
    case MENU_POWER_UP:
      LCD_show_music_wait();
      break;
    case MENU_WAIT:
#ifdef USB_DEVICE_EN
    case MENU_PC_MAIN:
    case MENU_PC_VOL_UP:
    case MENU_PC_VOL_DOWN:
#endif
#ifdef RTC_ALARM_EN
    case MENU_ALM_UP:
#endif
      //LCD_SEG5X9_show_string_menu(UI_var.bCurMenu);
      break;

    case MENU_PAUSE:
        //LCDC5X9_P0buf.bCom0Seg0 = 0x01; //����":"�����
        LCD_show_music_main();
    break;

    case MENU_INPUT_NUMBER:
      //LCD_SEG5X9_show_IR_number();
      break;

    /*-----Common Info UI*/
    case MENU_MAIN_VOL:
      LCD_show_volume();
      break;

    /*-----Music Related UI*/
    case MENU_MUSIC_MAIN:
      LCD_show_music_main();
      break;

    case MENU_EQ:
      //LCD_SEG5X9_show_eq();
      break;

    case MENU_PLAYMODE:
      //LCD_show_playmode();
      break;

    case MENU_FILENUM:
      LCD_show_file_number();
      break;

   /*-----FM Related UI*/
#ifdef FM_ENABLE
    case MENU_FM_MAIN:
    case MENU_FM_DISP_FRE:
      LCD_show_fm_main();
      break;
    case MENU_FM_FIND_STATION:
    case MENU_FM_CHANNEL:
      LCD_show_fm_station();
      break;
#endif
   /*-----RTC Related UI*/
#ifdef RTC_EN
    case MENU_RTC_MAIN:
      RTC_setting_var.bMode = 0;    //ģʽ�����ͬ������
    case MENU_RTC_SET:
       //LCD_SEG5X9_show_RTC_main();
      break;
#ifdef RTC_ALARM_EN
    case MENU_ALM_SET:
      LCD_SEG5X9_show_alarm();
      break;
#endif
#endif
    case MENU_MUTE:
      LCD_mute(mute);
      break;
    case MENU_STOP:
      LCD_stop();
      break;
    default:
      break;
    }
    //LCD_drive5X9_12();
}
#endif
