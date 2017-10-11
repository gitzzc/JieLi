/*--------------------------------------------------------------------------*/
/**@file    LED.c
   @brief   LED 模块驱动接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "UI_API.h"

#ifdef LED_5X7
#include "device.h"
#include "get_music_file.h"
#include "main.h"
#include "fm_radio.h"
#include "hot_msg.h"
#include "music_play.h"
#include "RTC_API.h"
#include "play_file.h"
#include "IRTC.h"

_no_init LED5X7_VAR _idata LED5X7_var;

const u8 LED_NUMBER[10] AT(LED_5X7_TABLE_CODE)=
{
    /*0*/
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F),
    /*1*/
    (u8)(LED_B | LED_C),
    /*2*/
    (u8)(LED_A | LED_B | LED_D | LED_E | LED_G),
    /*3*/
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_G),
    /*4*/
    (u8)(LED_B | LED_C | LED_F | LED_G),
    /*5*/
    (u8)(LED_A | LED_C | LED_D | LED_F | LED_G),
    /*6*/
    (u8)(LED_A | LED_C | LED_D | LED_E | LED_F | LED_G),
    /*7*/
    (u8)(LED_A | LED_B | LED_C),
    /*8*/
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_E | LED_F | LED_G),
    /*9*/
    (u8)(LED_A | LED_B | LED_C | LED_D | LED_F | LED_G),
};

const u8 LED_LARGE_LETTER[26] AT(LED_5X7_TABLE_CODE)=
{
    0x77,0x40,0x39,0x3f,0x79,///<ABCDE
    0x71,0x40,0x76,0x06,0x40,///<FGHIJ
    0x40,0x38,0x40,0x37,0x3f,///<KLMNO
    0x73,0x40,0x50,0x6d,0x40,///<PQRST
    0x3e,0x3e,0x40,0x76,0x40,///<UVWXY
    0x40///<Z
};

const u8 LED_SMALL_LETTER[26] AT(LED_5X7_TABLE_CODE)=
{
    0x77,0x7c,0x58,0x5e,0x79,///<abcde
    0x71,0x40,0x40,0x40,0x40,///<fghij
    0x40,0x38,0x40,0x54,0x5c,///<klmno
    0x73,0x67,0x50,0x40,0x40,///<pqrst
    0x3e,0x3e,0x40,0x40,0x40,///<uvwxy
    0x40///<z
};

const u8 playmodestr[][5] AT(LED_5X7_TABLE_CODE)=             
{
    " ALL",
    
#ifdef FOLDER_PLAY_EN    
    "Fold",
#endif
    
    " ONE",
    
#ifdef RANDOM_PLAY_EN    
    " rAn",
#endif    
};

const u8 menu_string[][5] AT(LED_5X7_TABLE_CODE)=             
{
    "HI  ",
    "Lod ",
#ifdef USB_DEVICE_EN    
    " PC ",
    " UP ",
    " dN ",
#endif    
    " AUX",
#ifdef RTC_ALARM_EN 
    "-AL-"
#endif
};



/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 状态位缓存清除函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_clear_icon(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_clear_icon(void) AT(LED_5X7_CODE)
{
    LED5X7_var.bFlashChar = 0;
    LED5X7_var.bFlashIcon = 0;
    LED5X7_var.bShowBuff[4] = 0;
}

/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 显示坐标设置
   @param   x：显示横坐标
   @return  void
   @author  Change.tsai
   @note    void LED5X7_setX(u8 X)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_setX(u8 X) AT(LED_5X7_CODE)
{
    LED5X7_var.bCoordinateX = X;
}

/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 扫描函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_init(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_init(void) AT(LED_5X7_CODE)
{
    /*Com setting*/
    P3HD |= 0x1F;
    P3DIR &= ~0x1F;
    
#ifdef UI_FADE_EN    
    P3PD |= 0x1F;
#endif    

    /*Seg setting*/
    //P1HD |= 0x7F;
    P1DIR &= ~0x7F;
    
    /*Brightness*/
    PWM4_CON = 0xCF;
    /*PWM4 Enable*/
    IO_MC1 &= ~BIT(7);
}
/*----------------------------------------------------------------------------*/
/**@brief   LED 亮度设置
   @param   void
   @return  void
   @author  Change.tsai
   @note    void set_LED_brightness(u8 br)
*/
/*----------------------------------------------------------------------------*/
_near_func void set_LED_brightness(u8 br) AT(COMMON_CODE)
{
    if (br > 16)
        return;
    /**/
    if (br == 16)
        PWM4_CON = 0;
    else
        PWM4_CON = br | 0xd0;
    
}

/*----------------------------------------------------------------------------*/
/**@brief   设置LED亮度渐暗
   @param   void
   @return  void
   @author  Change.tsai
   @note    void set_LED_fade_out(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void set_LED_fade_out(void) AT(COMMON_CODE)
{
	if (LED5X7_var.bBrightness < 20)
	{
		LED5X7_var.bBrightness++;
		set_LED_brightness(23 - LED5X7_var.bBrightness);
	}
}
/*----------------------------------------------------------------------------*/
/**@brief   设置LED亮度全亮
   @param   void
   @return  void
   @author  Change.tsai
   @note    void set_LED_all_on(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void set_LED_all_on(void) AT(COMMON_CODE)
{
	set_LED_brightness(16);
	LED5X7_var.bBrightness = 0;
}

/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 单个字符显示函数
   @param   chardata：显示字符
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_char(u8 chardata)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_char(u8 chardata) AT(LED_5X7_CODE)
{
    //必须保证传入的参数符合范围，程序不作判断
    //if ((chardata < ' ') || (chardata > '~') || (LED5X7_var.bCoordinateX > 4))
    //{
    //    return;
    //}
    if ((chardata >= '0') && (chardata <= '9'))
    {
        LED5X7_var.bShowBuff[LED5X7_var.bCoordinateX++] = LED_NUMBER[chardata - '0'];
    }
    else if ((chardata >= 'a') && (chardata <= 'z'))
    {
        LED5X7_var.bShowBuff[LED5X7_var.bCoordinateX++] = LED_SMALL_LETTER[chardata - 'a'];
    }
    else if ((chardata >= 'A') && (chardata <= 'Z'))
    {
        LED5X7_var.bShowBuff[LED5X7_var.bCoordinateX++] = LED_LARGE_LETTER[chardata - 'A'];
    }
    else if (chardata == ':')
    {
        LED_STATUS |= LED_2POINT;
    }
    else if (chardata == ' ')
    {
        LED5X7_var.bShowBuff[LED5X7_var.bCoordinateX++] = 0;
    }
    else //if (chardata == '-')     //不可显示
    {
        LED5X7_var.bShowBuff[LED5X7_var.bCoordinateX++] = BIT(6);
    }    
}

/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 字符串显示函数
   @param   *str：字符串的指针   offset：显示偏移量
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_string(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_string(u8 *str) AT(LED_5X7_CODE)
{
    while (0 != *str)
    {
        LED5X7_show_char(*str++);
    } 
}


/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_string_menu(u8 menu) AT(LED_5X7_CODE)
{
    LED5X7_show_string((u8 *)menu_string[menu]);
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 设备显示
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_dev(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_dev(void) AT(LED_5X7_CODE)
{
    /*Music Device type*/
    if (device_active == DEVICE_UDISK)
      LED_STATUS |= LED_USB;
#ifdef SDMMCB_EN
    else if ((device_active == DEVICE_SDMMCA)||(device_active == DEVICE_SDMMCB))
#elif defined SDMMCA_EN
    if (device_active == DEVICE_SDMMCA)
#endif
      LED_STATUS |= LED_SD;
}

/*----------------------------------------------------------------------------*/
/**@brief   Music 播放文件号显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_filenumber(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_filenumber(void) AT(LED_5X7_CODE)
{  
    /*Music File Number info*/
    itoa4(playfile.given_file_number);
    LED5X7_show_string((u8 *)bcd_number);
    LED5X7_show_dev();      
}


/*----------------------------------------------------------------------------*/
/**@brief   红外输入文件号显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_IR_number(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_IR_number(void) AT(LED_5X7_CODE)
{
    /*IR File Number info*/
    itoa4(input_number);
    LED5X7_show_string((u8 *)bcd_number);
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_music_main(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_music_main(void) AT(LED_5X7_CODE)
{
    u16 play_time;
    
    /*Music Play time info*/
    play_time = get_music_play_time();
    
    itoa2(play_time/60);
    LED5X7_show_string((u8 *)bcd_number);
    
    itoa2(play_time%60);
    LED5X7_show_string((u8 *)bcd_number);
    
    LED5X7_show_dev();
    LED_STATUS |= LED_2POINT | LED_MP3;
    if (Music_Play_var.bPlayStatus == MAD_PLAY)
        LED_STATUS |= LED_PLAY;
    else if (Music_Play_var.bPlayStatus == MAD_PAUSE)
        LED_STATUS |= LED_PAUSE;
}

/*----------------------------------------------------------------------------*/
/**@brief   EQ显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_volume(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_eq(void) AT(LED_5X7_CODE)
{
    LED5X7_show_string((u8 *)"Eq ");
    LED5X7_show_char(Music_Play_var.bEQ%10 + '0');   
}

/*----------------------------------------------------------------------------*/
/**@brief   循环模式显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_volume(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_playmode(void) AT(LED_5X7_CODE)
{
    LED5X7_show_string((u8 *)&playmodestr[playfile.play_mode][0]);
}


/*----------------------------------------------------------------------------*/
/**@brief   音量显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_volume(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_volume(void) AT(LED_5X7_CODE)
{   
    LED5X7_show_string((u8 *)"V ");
    itoa2(main_vol_L);
    LED5X7_show_string((u8 *)bcd_number);
}

/*----------------------------------------------------------------------------*/
/**@brief   FM 模式主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_fm_main(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_fm_main(void) AT(LED_5X7_CODE)
{
    /*FM - Frequency*/
    itoa4(fm_mode_var.wFreq);
    if (fm_mode_var.wFreq <= 999)
        bcd_number[0] = ' ';
         
    LED5X7_show_string((u8 *)bcd_number);
    LED_STATUS |= LED_MHZ;
}

/*----------------------------------------------------------------------------*/
/**@brief   FM 模式主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_fm_main(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_fm_station(void) AT(LED_5X7_CODE)
{
    /*FM - Station*/
    LED5X7_show_string((u8 *)"P ");
    itoa2(fm_mode_var.bFreChannel);
    LED5X7_show_string((u8 *)bcd_number);
}

/*----------------------------------------------------------------------------*/
/**@brief   RTC 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_RTC_main(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_RTC_main(void) AT(LED_5X7_CODE)
{
    itoa2(curr_time.bHour);
    LED5X7_show_string((u8 *)bcd_number);
    itoa2(curr_time.bMin);
    LED5X7_show_string((u8 *)bcd_number);
    
    LED5X7_var.bFlashIcon |= LED_2POINT;
    LED_STATUS |= LED_2POINT;

    if (UI_var.bCurMenu == MENU_RTC_SET)
    {
        if (RTC_setting_var.bCoordinate)
            LED5X7_var.bFlashChar = BIT(2)|BIT(3);
        else
            LED5X7_var.bFlashChar = BIT(0)|BIT(1);
    }
    else 
        LED5X7_var.bFlashChar = 0;
}   

/*----------------------------------------------------------------------------*/
/**@brief   Alarm 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_alarm(void)
*/
/*----------------------------------------------------------------------------*/
void LED5X7_show_alarm(void) AT(LED_5X7_CODE) 
{
    itoa2(curr_alarm.bHour);
    LED5X7_show_string((u8 *)bcd_number);
    itoa2(curr_alarm.bMin);
    LED5X7_show_string((u8 *)bcd_number);
    
    LED_STATUS |= LED_2POINT;
    
    if (RTC_setting_var.bCoordinate == 0)
        LED5X7_var.bFlashChar = BIT(0)|BIT(1);
    else if (RTC_setting_var.bCoordinate == 1)
        LED5X7_var.bFlashChar = BIT(2)|BIT(3);
    else if (RTC_setting_var.bCoordinate == 2)
        LED5X7_var.bFlashChar |= 0xF;
    
    /*Alarm info - Switch On/Off*/
    if (curr_alarm.bSw)
    {
        LED_STATUS |= LED_PLAY;   
    }
}


/*----------------------------------------------------------------------------*/
/**@brief   LED扫描函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_scan(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LED5X7_scan(void)  AT(COMMON_CODE)//AT(LED_5X7_CODE)
{
    _no_init static u8 _data cnt;
    
    u8 seg;
   
    seg = LED5X7_var.bShowBuff[cnt];
    
    if (Sys_HalfSec)
    {
        if ((LED5X7_var.bFlashIcon) && (cnt == 4))
        {
            seg = LED_STATUS & (~LED5X7_var.bFlashIcon);  
        }
        else if (LED5X7_var.bFlashChar & BIT(cnt))
        {
            seg = 0x0;
        }
    }

    /*0~4*/
    LED_COM &= ~0x1f;
    LED_SEG |= 0x7f;
    
#ifdef UI_FADE_EN    
    P3PU &= ~0x1F;
    P3PU |= BIT(cnt);
#endif

    LED_COM |= BIT(cnt);
    LED_SEG &= ~seg;
    
    cnt++;
    if(cnt == 5)
        cnt = 0;
}

#endif
