/*--------------------------------------------------------------------------*/
/**@file    Line_in_mode.c
   @brief   Line-In 模式主循环
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "config.h"
#include "Line_in_mode.h"
#include "msg.h"
#include "hot_msg.h"
#include "key.h"
#include "main.h"
#include "device.h"
#include "iic.h"
#include "UI_API.h"
#include "LCD.h"
#include "dac.h"
#include "clock.h"

/*----------------------------------------------------------------------------*/
/**@brief   Line-In 模式信息初始化
   @param   void
   @return  void
   @author  Change.tsai
   @note    void Line_in_info_init(void)
*/
/*----------------------------------------------------------------------------*/
void Line_in_info_init(void) AT(LINE_IN_CODE)
{
    /*------------System configuration-----------*/
    Sys_IRInput = 0;
    Sys_Volume = 1;
  
    /*---------AUX MAIN UI--------------*/
    SET_UI_MAIN(MENU_AUX_MAIN);
    UI_menu(MENU_AUX_MAIN);
    key_table_sel(0);
}


/*----------------------------------------------------------------------------*/
/**@brief   Line-In 模式主循环
   @param   void
   @return  void
   @author  Change.tsai
   @note    void Line_in_loop(void)
*/
/*----------------------------------------------------------------------------*/
void Line_in_mode_loop(void) AT(LINE_IN_CODE)
{
    u8 msg;
    
    while (1)
    {
        msg = app_get_msg();

        switch (msg)
        {
        case MSG_AUX_IN:
            break;
        case MSG_AUX_OUT :
            work_mode++;
            return;   
        case MSG_MUSIC_NEW_DEVICE_IN:
            work_mode = MUSIC_MODE;
        case MSG_CHANGE_WORK_MODE:
            //deg_puts("Exit RTC mode\n");
            return;

        case MSG_HALF_SECOND:           
            LED_FADE_OFF();
            UI_menu(MENU_MAIN);
            break;   
          
        default:
            ap_handle_hotkey(msg);
            break;
        }
    }

}

/*----------------------------------------------------------------------------*/
/**@brief   Line-In 模式
   @param   void
   @return  void
   @author  Change.tsai
   @note    void Line_in_mode(void)
*/
/*----------------------------------------------------------------------------*/
void Line_in_mode(void) AT(LINE_IN_CODE)
{
    Line_in_info_init();
    dac_channel_sel(DAC_AMUX1);
    system_clk_div(CLK_1M);
    Line_in_mode_loop();
}

