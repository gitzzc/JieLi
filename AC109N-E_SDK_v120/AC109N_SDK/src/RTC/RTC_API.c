/*******************************************************************************************
File Name:  RTC_API.c

Version:    1.00

Discription: RTC 模式接口函数

Author:     Caibingquan

Email:      change.tsai@gmail.com

Date:       2012.06.04

Copyright:(c) 2012 , All Rights Reserved.
*******************************************************************************************/
#include "config.h"
#include "RTC_API.h"
#include "msg.h"
#include "hot_msg.h"
#include "key.h"
#include "main.h"
#include "device.h"
#include "iic.h"
#include "UI_API.h"
#include "LCD.h"
#include "dac.h"
#include "IRTC.h"
#include "clock.h"

_no_init ALM_VOL_INFO _idata ALM_vol_info;

/*----------------------------------------------------------------------------*/
/**@brief   清空RTC RAM（在RTC 掉电后使用，避免数据错误）
   @param   void
   @return  void
   @author  Change.tsai
   @note    void reset_RTC_RAM(void)
*/
/*----------------------------------------------------------------------------*/
void reset_RTC_RAM(void) AT(RTC_CODE)
{
    u16 i;
    
    i = 0;
    while(i < 256)
    {
       write_IRTC_RAM(i++, 0x0); 
    }
}

#ifndef RTC_EN
/*----------------------------------------------------------------------------*/
/**@brief   IRTC 模块初始化函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void IRTC_init(void)
*/
/*----------------------------------------------------------------------------*/
void IRTC_init(void) AT(RTC_CODE)
{
    u8 reg;
    /*---------Power down reset RTC sfr*/
    reg = read_RTC_reg(READ_SR0);
    if (reg & BIT(7))           //判断校验位是否为0x55AA的方法也可以使用
    {
        reset_RTC_RAM();
    }
    write_RTC_reg(WRITE_SR0, reg & (~BIT(7)));
    
    /*---------IE Initial-------------*/
}

#else
const u8 month_tab1[] AT(BANK4_TABLE_CODE) = {31,28,31,30,31,30,31,31,30,31,30,31};		   ///<非闰年每月的天数
const u8 month_tab2[] AT(BANK4_TABLE_CODE) = {31,29,31,30,31,30,31,31,30,31,30,31};		   ///<闰年每月的天数

/*----------------------------------------------------------------------------*/
/**@brief	判断闰年
   @param 	year:年份
   @return  真假
   @author  Change.tsai
   @note	bool leapyear(u16 year)
*/
/*----------------------------------------------------------------------------*/
bool leapyear_app(u16 year) AT(RTC_CODE)
{
    if ((year%4==0 && year%100!=0)||(year%400==0))
        return TRUE;
    else
        return FALSE;
}
/*----------------------------------------------------------------------------*/
/**@brief   当月的天数(Maskrom API)
   @param 	nmonth：当月月份；nyear：当年年份
   @return  当月天数
   @note    u8 nowmonth(u8 nmonth,u16 nyear)
*/
/*----------------------------------------------------------------------------*/
u8 nowmonth_app(u8 nmonth,u16 nyear) AT(RTC_CODE)
{
    if (leapyear_app(nyear))
        return month_tab2[nmonth-1];
    else
        return month_tab1[nmonth-1];

}
/*----------------------------------------------------------------------------*/
/**@brief 	在位图中寻找下一个有效位
   @param 	cur_bit：当前位；bitmap：位图
   @return  下一个有效位图
   @author  Change.tsai
   @note  	u8 get_next_bit(u8 cur_bit, u8 bitmap)
*/
/*----------------------------------------------------------------------------*/
u8 get_next_bit8(u8 cur_bit, u8 bitmap) AT(RTC_CODE)
{
    u8 i, j;
    
    i = cur_bit;        //Search from current bit
    
    for (j = 0; j < 2; j++)
    {
        for (; i < 8; i++)
        {
            if (bitmap & BIT(i))
            {
                return i;       //Available bit in bitmap
            }
        }
        i = 1;      //Search for head
    }
    return 0xFF;    //Error
}
/*----------------------------------------------------------------------------*/
/**@brief 	设置下一个闹钟，并写入IRTC
   @param 	void
   @return  void
   @author  Change.tsai
   @note  	void write_next_ALM(void)
*/
/*----------------------------------------------------------------------------*/
void write_next_ALM(void) AT(RTC_CODE)
{
    u8 i;
    RTC_TIME alm_sec;

    if (curr_alarm.bmWeekdays == 0x0)
      return;
    
    read_RTC((RTC_TIME *)(&curr_time));
    
    for (i = 0;i < sizeof(RTC_TIME);i++)
    {
        ((u8 *)(&alm_sec))[i] = ((u8 *)(&curr_time))[i];
    }
   
    alm_sec.bSec = 0;
    alm_sec.bHour = curr_alarm.bHour;
    alm_sec.bMin = curr_alarm.bMin;
    /*Weekday alarm*/
#if 1
    u8 weekday;
    
    weekday = get_next_bit8(alm_sec.bWeekday, curr_alarm.bmWeekdays);
    if (weekday < alm_sec.bWeekday)
    {
        weekday += 7;
    } 
    else if (weekday == alm_sec.bWeekday)
    {
        if ((curr_alarm.bHour < curr_time.bHour)||((curr_alarm.bHour == curr_time.bHour)&&(curr_alarm.bMin <= curr_time.bMin)))
        {
            u8 temp;
            
            temp = weekday;
            if (temp == 7)
            {
                temp = 1;
                weekday = get_next_bit8(temp, curr_alarm.bmWeekdays);
                weekday += 7;
            }
            else
            {
                weekday = get_next_bit8(temp + 1, curr_alarm.bmWeekdays);
                if (weekday < temp)
                {
                    weekday += 7;
                }    
            }   
        }   
    }
    weekday -= alm_sec.bWeekday;
    alm_sec.bDay += weekday;         
/*Everyday alarm*/
#else
   if ((curr_alarm.bHour < curr_time.bHour)||((curr_alarm.bHour == curr_time.bHour)&&(curr_alarm.bMin <= curr_time.bMin)))
        alm_sec.bDay += 1; 
#endif
    //Attention！当闹钟没有有效闹钟日期时会写入错误闹钟时间
    write_ALM(&alm_sec);
    read_ALM(&alm_sec);
}

/*----------------------------------------------------------------------------*/
/**@brief   IRTC 时间复位函数
   @param   无
   @return  无
   @author  Change.tsai
   @note    void reset_RTC_time(void)
*/
/*----------------------------------------------------------------------------*/
void reset_RTC_time(void) AT(RTC_CODE)
{
    curr_time.dYear 	= 2012;
    curr_time.bMonth 	= 1;
    curr_time.bDay 		= 1;
    curr_time.bHour 	= 0;
    curr_time.bMin 		= 0;
    curr_time.bSec 		= 0;
    write_RTC((RTC_TIME *)(&curr_time));
}

/*----------------------------------------------------------------------------*/
/**@brief   日期超限校验
   @param 	void
   @return  void
   @author  Change.tsai
   @note    void RTC_overflow_check(void)
*/
/*----------------------------------------------------------------------------*/
void RTC_overflow_check(void) AT(RTC_CODE)
{
    if (curr_time.dYear > 2178)
    {
		reset_RTC_time();
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   IRTC 闹钟使能控制
   @param   cmd：0-关闭闹钟 1-打开闹钟
   @return  无
   @author  Change.tsai
   @note    void alarm_enable(u8 cmd)
*/
/*----------------------------------------------------------------------------*/
void alarm_enable(u8 cmd) AT(RTC_CODE)
{
    u8 reg;
    
    reg = read_RTC_reg(READ_SR0);
    if (cmd)
    {
        reg |= BIT(5);   
    }
    else
    {
        reg &= ~BIT(5);
    }
    write_RTC_reg(WRITE_SR0, reg);
}

/*----------------------------------------------------------------------------*/
/**@brief   IRTC 模块初始化
   @param   void
   @return  无
   @author  Change.tsai
   @note    void IRTC_init(void)
*/
/*----------------------------------------------------------------------------*/
void IRTC_init(void) AT(RTC_CODE)
{
    u8 reg;

    /*---------Power down reset RTC sfr*/
    reg = read_RTC_reg(READ_SR0);
    //deg("IRTC reg 0 : %02x\n", reg);
    if (reg & BIT(7))
    {
        reg &= ~BIT(7);
        reset_RTC_time();
        reset_RTC_RAM();
    }
#ifdef RTC_ALARM_EN    
    /*---------Alarm info*/
    curr_alarm.bSw = 0;		
    if (reg & BIT(5))
    {
        curr_alarm.bSw = 1;		    //Alarm Enable   
        //deg_puts("2-Alarm On\n");
    }
    write_RTC_reg(WRITE_SR0, reg);
    
    /*----------Alarm initial--------------*/
    curr_alarm.bHour = get_memory(MEM_ALARM_0_HOUR);
    curr_alarm.bHour = (curr_alarm.bHour >= 24) ? 0 : curr_alarm.bHour;
    curr_alarm.bMin = get_memory(MEM_ALARM_0_MIN);
    curr_alarm.bMin = (curr_alarm.bMin > 59) ? 0 : curr_alarm.bMin;
#if defined LED_5X7 || defined LCD_SEG_5X9
    curr_alarm.bmWeekdays = 0xfe;
#else    
    curr_alarm.bmWeekdays = get_memory(MEM_ALARM_0_WEEKDAY);   //必须初始化
    curr_alarm.bmWeekdays = (curr_alarm.bmWeekdays > 0xfe) ? 0 : curr_alarm.bmWeekdays;
#endif
    write_next_ALM();
    
    /*---------IE Initial-------------*/
    IRTC_X2CLRCPND;
    IRTC_X512CLRCPND;
    IRTC_WKCLRPND;
    
    IRTC_WKIE(1);
#endif
    //IRTC_X2IE(1);
    //IRTC_X512IE(1);
   
    //IRTC_IE(1);
    //IRTC_IP(1);
}

/*----------------------------------------------------------------------------*/
/**@brief   IRTC 模式信息初始化
   @param   无
   @return  无
   @author  Change.tsai
   @note    void RTC_info_init(void)
*/
/*----------------------------------------------------------------------------*/
void RTC_info_init(void)  AT(RTC_CODE)
{
    /*------------System configuration-----------*/
    Sys_IRInput = 0;
    Sys_Volume = 0;

    read_RTC((RTC_TIME *)(&curr_time));
    RTC_overflow_check();
    //deg("Date :%u/%u/%u  ", curr_time.dYear, (u16)curr_time.bMonth, (u16)curr_time.bDay);
    //deg("%u:%u:%u * %u\n", (u16)curr_time.bHour, (u16)curr_time.bMin, (u16)curr_time.bSec, (u16)curr_time.bWeekday);  
    
    /*---------RTC MAIN UI--------------*/
    SET_UI_MAIN(MENU_RTC_MAIN);
    if (RTC_setting_var.bAlarmOn == 0)
        UI_menu(MENU_RTC_MAIN);
    key_table_sel(0);
}

/*----------------------------------------------------------------------------*/
/**@brief   RTC 坐标设置界面
   @param   dir：1左移操作 0右移操作
   @return  无
   @author  Change.tsai
   @note    void LED_RTC_coordinate_setting(u8 dir)
*/
/*----------------------------------------------------------------------------*/
void LED_RTC_coordinate_setting(u8 dir) AT(RTC_CODE)
{
    if (dir)    //back
    {
        /*--------RTC Setting*/
        if (RTC_setting_var.bMode == RTC_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate <= 0) \
                ? 1 : RTC_setting_var.bCoordinate - 1;  
            UI_menu(MENU_RTC_SET);    
        }
        /*--------Alarm Setting*/
        else if (RTC_setting_var.bMode == ALARM_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate <= 0) \
                ? 2 : RTC_setting_var.bCoordinate - 1;  
            UI_menu(MENU_ALM_SET);
        } 
    }
    else    //forward
    {
        /*--------RTC Setting*/
        if (RTC_setting_var.bMode == RTC_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate >= 1) \
                ? 0 : RTC_setting_var.bCoordinate + 1;
            UI_menu(MENU_RTC_SET);    
        }
        /*--------Alarm Setting*/
        else if (RTC_setting_var.bMode == ALARM_SETTING)
        {  
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate >= 2) \
                ? 0 : RTC_setting_var.bCoordinate + 1;    
            UI_menu(MENU_ALM_SET);
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   RTC 设置模式
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void LED_RTC_setting_mode(void)
*/
/*----------------------------------------------------------------------------*/
void LED_RTC_setting_mode(u8 dir) AT(RTC_CODE)
{
    if (RTC_setting_var.bMode != RTC_SETTING)
        return;
    
    if (dir)
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_time.bHour = (curr_time.bHour >= 23)\
            ? 0 : curr_time.bHour + 1;
          break;
        case 1:
          curr_time.bMin = (curr_time.bMin >= 59)\
            ? 0 : curr_time.bMin + 1;
          break;

        default:
          break;
        }
    }
    else
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_time.bHour = (curr_time.bHour <= 0)\
            ? 23 : curr_time.bHour - 1;
          break;
        case 1:
          curr_time.bMin = (curr_time.bMin <= 0)\
            ? 59 : curr_time.bMin - 1;
          break;

        default:
          break;
        }
    }
    write_RTC((RTC_TIME *)(&curr_time));
    write_next_ALM();
}

/*----------------------------------------------------------------------------*/
/**@brief   Alarm 设置模式
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void LED_Alarm_setting_mode(void)
*/
/*----------------------------------------------------------------------------*/
void LED_Alarm_setting_mode(u8 dir) AT(RTC_CODE)
{
    if (RTC_setting_var.bMode != ALARM_SETTING)
        return;
    
    if (dir)
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_alarm.bHour = (curr_alarm.bHour >= 23)\
            ? 0 : curr_alarm.bHour + 1;
          break;
        case 1:
          curr_alarm.bMin = (curr_alarm.bMin >= 59)\
            ? 0 : curr_alarm.bMin + 1;
          break;

        default:
          break;
        }
    }
    else
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_alarm.bHour = (curr_alarm.bHour <= 0)\
            ? 23 : curr_alarm.bHour - 1;
          break;
        case 1:
          curr_alarm.bMin = (curr_alarm.bMin <= 0)\
            ? 59 : curr_alarm.bMin - 1;
          break;

        default:
          break;
        }
    }
    if (RTC_setting_var.bCoordinate == 2)
    {
        curr_alarm.bSw = !curr_alarm.bSw;
        alarm_enable(curr_alarm.bSw); 
    }
    set_memory(MEM_ALARM_0_HOUR, curr_alarm.bHour);
    set_memory(MEM_ALARM_0_MIN, curr_alarm.bMin);
    write_next_ALM();
}

/*----------------------------------------------------------------------------*/
/**@brief   RTC 信息设置
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void RTC_info_setting(u8 dir)
*/
/*----------------------------------------------------------------------------*/
void LED_RTC_info_setting(u8 dir) AT(RTC_CODE)
{
    /*--------RTC Setting*/
    if (RTC_setting_var.bMode == RTC_SETTING)
    {
        LED_RTC_setting_mode(dir);
        UI_menu(MENU_RTC_SET);    
    }
    else if (RTC_setting_var.bMode == ALARM_SETTING)
    {
        LED_Alarm_setting_mode(dir);
        UI_menu(MENU_ALM_SET);    
    }  
}



/*----------------------------------------------------------------------------*/
/**@brief   RTC 坐标设置界面
   @param   dir：1左移操作 0右移操作
   @return  无
   @author  Change.tsai
   @note    void LCD_RTC_coordinate_setting(u8 dir)
*/
/*----------------------------------------------------------------------------*/
void LCD_RTC_coordinate_setting(u8 dir) AT(RTC_CODE)
{
    if (dir)    //back
    {
        /*--------RTC Setting*/
        if (RTC_setting_var.bMode == RTC_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate <= 0) \
                ? 5 : RTC_setting_var.bCoordinate - 1;  
            UI_menu(MENU_RTC_SET);    
        }
        /*--------Alarm Setting*/
        else if (RTC_setting_var.bMode == ALARM_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate <= 0) \
                ? 9 : RTC_setting_var.bCoordinate - 1;  
            UI_menu(MENU_ALM_SET);
        } 
    }
    else    //forward
    {
        /*--------RTC Setting*/
        if (RTC_setting_var.bMode == RTC_SETTING)
        {
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate >= 5) \
                ? 0 : RTC_setting_var.bCoordinate + 1;
            UI_menu(MENU_RTC_SET);    
        }
        /*--------Alarm Setting*/
        else if (RTC_setting_var.bMode == ALARM_SETTING)
        {  
            RTC_setting_var.bCoordinate = \
              (RTC_setting_var.bCoordinate >= 9) \
                ? 0 : RTC_setting_var.bCoordinate + 1;    
            UI_menu(MENU_ALM_SET);
        }
    }
}



/*----------------------------------------------------------------------------*/
/**@brief   RTC 设置模式
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void RTC_setting_mode(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_RTC_setting_mode(u8 dir) AT(RTC_CODE)
{
    if (RTC_setting_var.bMode != RTC_SETTING)
        return;
    
    if (dir)
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_time.dYear = (curr_time.dYear >= 2135)\
            ? 2000 : curr_time.dYear + 1;
          break;
        case 1:
          curr_time.bMonth = (curr_time.bMonth >= 12)\
            ? 1 : curr_time.bMonth + 1;
          break;
        case 2:
          curr_time.bDay = (curr_time.bDay >= nowmonth(curr_time.bMonth, curr_time.dYear))\
            ? 1 : curr_time.bDay + 1;
          break;
        case 3:
          curr_time.bHour = (curr_time.bHour >= 23)\
            ? 0 : curr_time.bHour + 1;
          break;
        case 4:
          curr_time.bMin = (curr_time.bMin >= 59)\
            ? 0 : curr_time.bMin + 1;
          break;
        case 5:
          curr_time.bSec = (curr_time.bSec >= 59)\
            ? 0 : curr_time.bSec + 1;
          break;

        default:
          break;
        }
    }
    else
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_time.dYear = (curr_time.dYear <= 2000)\
            ? 2135 : curr_time.dYear - 1;
          break;
        case 1:
          curr_time.bMonth = (curr_time.bMonth <= 1)\
            ? 12 : curr_time.bMonth - 1;
          break;
        case 2:
          curr_time.bDay = (curr_time.bDay <= 1)\
            ? nowmonth(curr_time.bMonth, curr_time.dYear) : curr_time.bDay - 1;
          break;
        case 3:
          curr_time.bHour = (curr_time.bHour <= 0)\
            ? 23 : curr_time.bHour - 1;
          break;
        case 4:
          curr_time.bMin = (curr_time.bMin <= 0)\
            ? 59 : curr_time.bMin - 1;
          break;
        case 5:
          curr_time.bSec = (curr_time.bSec <= 0)\
            ? 59 : curr_time.bSec - 1;
          break;

        default:
          break;
        }
    }
    write_RTC((RTC_TIME *)(&curr_time));
    write_next_ALM();
}

/*----------------------------------------------------------------------------*/
/**@brief   Alarm 设置模式
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void Alarm_setting_mode(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_Alarm_setting_mode(u8 dir) AT(RTC_CODE)
{
    if (RTC_setting_var.bMode != ALARM_SETTING)
        return;
    
    if (dir)
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_alarm.bHour = (curr_alarm.bHour >= 23)\
            ? 0 : curr_alarm.bHour + 1;
          break;
        case 1:
          curr_alarm.bMin = (curr_alarm.bMin >= 59)\
            ? 0 : curr_alarm.bMin + 1;
          break;

        default:
          break;
        }
    }
    else
    {
        switch(RTC_setting_var.bCoordinate) 
        {
        case 0:
          curr_alarm.bHour = (curr_alarm.bHour <= 0)\
            ? 23 : curr_alarm.bHour - 1;
          break;
        case 1:
          curr_alarm.bMin = (curr_alarm.bMin <= 0)\
            ? 59 : curr_alarm.bMin - 1;
          break;

        default:
          break;
        }
    }
    u8 i;
    i = RTC_setting_var.bCoordinate - 2;
    switch(i)
    {
    case 0:
      curr_alarm.bSw = !curr_alarm.bSw;
      alarm_enable(curr_alarm.bSw);
      break;
    case 1:  
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      curr_alarm.bmWeekdays = (curr_alarm.bmWeekdays & BIT(i))\
        ? curr_alarm.bmWeekdays & ~BIT(i) : curr_alarm.bmWeekdays | BIT(i);
      break;
    }
    set_memory(MEM_ALARM_0_HOUR, curr_alarm.bHour);
    set_memory(MEM_ALARM_0_MIN, curr_alarm.bMin);
    set_memory(MEM_ALARM_0_WEEKDAY, curr_alarm.bmWeekdays);
    write_next_ALM();
}

/*----------------------------------------------------------------------------*/
/**@brief   RTC 信息设置
   @param   dir：0-减操作 1-加操作
   @return  无
   @author  Change.tsai
   @note    void RTC_info_setting(u8 dir)
*/
/*----------------------------------------------------------------------------*/
void LCD_RTC_info_setting(u8 dir) AT(RTC_CODE)
{
    /*--------RTC Setting*/
    if (RTC_setting_var.bMode == RTC_SETTING)
    {
        LCD_RTC_setting_mode(dir);
        UI_menu(MENU_RTC_SET);    
    }
    else if (RTC_setting_var.bMode == ALARM_SETTING)
    {
        LCD_Alarm_setting_mode(dir);
        UI_menu(MENU_ALM_SET);    
    }  
}


/*----------------------------------------------------------------------------*/
/**@brief   闹钟响闹关闭
   @param   无
   @return  无
   @author  Change.tsai
   @note    void alarm_off(void)
*/
/*----------------------------------------------------------------------------*/
void alarm_off(void) AT(RTC_CODE)
{
    if (RTC_setting_var.bAlarmOn == 0)
        return;

    RTC_setting_var.bAlarmOn = 0;
#ifdef STANDBY_EN
    alm_on_flag = 0;
#endif
    /*Recover keytone*/
    // deg("ALM_vol_info.rtc_volr = %u\n",ALM_vol_info.rtc_volr);
     //deg("ALM_vol_info.rtc_voll = %u\n",ALM_vol_info.rtc_voll);

     main_vol_R = ALM_vol_info.bRtcVolr;
     main_vol_L = ALM_vol_info.bRtcVoll;

    set_main_vol (main_vol_L,main_vol_R);
    set_digital_vol(main_vol_L, main_vol_R);

    if(ALM_vol_info.bMute ==1 )
    {
        ALM_vol_info.bMute =0;
        dac_mute(1);
    }
    if (work_mode != RTC_MODE);
    else if (RTC_setting_var.bLastWorkMode != RTC_MODE)
    {
        work_mode = (ENUM_WORK_MODE)RTC_setting_var.bLastWorkMode;
        put_msg_fifo(MSG_CHANGE_WORK_MODE);
    }
    else
    {
        UI_menu(MENU_RTC_MAIN);
    }

}

/*----------------------------------------------------------------------------*/
/**@brief   闹钟响闹关闭
   @param   无
   @return  无
   @author  Change.tsai
   @note    void alarm_on(void)
*/
/*----------------------------------------------------------------------------*/
void alarm_on(void) AT(RTC_CODE)
{
    if (RTC_setting_var.bAlarmOn == 0x0)
        return;

    RTC_setting_var.bMode = RTC_SHOW;
    UI_menu(MENU_ALM_UP);

    if(RTC_setting_var.bAlarmOn == 1)  //设置最大音量
    {

        if(mute == 1)
        {
           ALM_vol_info.bMute = 1;
        }
        ALM_vol_info.bRtcVolr = main_vol_R;
        ALM_vol_info.bRtcVoll = main_vol_L;

        set_main_vol(30, 30);
        KV_VLM = 31;
        set_analog_vol(30, 30);
    }

    RTC_setting_var.bAlarmOn++;

    if (RTC_setting_var.bAlarmOn > 60)
        alarm_off();
    set_key_tone();
}



#ifdef STANDBY_EN
/*----------------------------------------------------------------------------*/
/**@brief   芯片进入Standby模式
   @param   无
   @return  无
   @author  Change.tsai
   @note    void standby_fun(void)
*/
/*----------------------------------------------------------------------------*/
__monitor void standby_fun(void)
{
    RTC_TIME one_min_alm;
    u8 i;

#ifdef WDT_EN
    config_wdt(0x40);  //关掉看门狗
#endif
    /**/
    alarm_enable(1);
    for (i = 0;i < sizeof(RTC_TIME);i++)
    {
        ((u8 *)(&one_min_alm))[i] = ((u8 *)(&curr_time))[i];
    }

    one_min_alm.bSec = 0;
    one_min_alm.bHour = curr_time.bHour;
    one_min_alm.bMin = curr_time.bMin+1;

    if(one_min_alm.bMin>59)
    {
        one_min_alm.bMin = 0;
        one_min_alm.bHour = curr_time.bHour+1;
        if(one_min_alm.bHour>23)
        {
            one_min_alm.bHour = 0;
            one_min_alm.bDay = curr_time.bDay+1;
        }
    }

    write_ALM(&one_min_alm);

    P0DIR |= BIT(7);
    P0PD &= ~BIT(7);
    P0PU |= BIT(7);
    P0DIE |=BIT(7);
    WKUPCON = 0x44; //下降沿
    WKUPPND |= BIT(6);

    SYS_STANDBY(1);

    if(curr_alarm.bSw == 0)
    {
        alarm_enable(0);
    }


#ifdef WDT_EN
    config_wdt(0x5D);
#endif
    AD_init();
}
#endif
#endif
