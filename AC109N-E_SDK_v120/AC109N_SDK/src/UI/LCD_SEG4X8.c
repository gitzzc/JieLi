/*--------------------------------------------------------------------------*/
/**@file    LCD_SEG4X8.c
   @brief   LCD 段码屏模块驱动接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "UI_API.h"

#if defined LCD_SEG_4X8
#include "LCD_SEG4X8.h"
#include "IRTC.h"
#include "device.h"
#include "get_music_file.h"
#include "main.h"
#include "fm_radio.h"
#include "hot_msg.h"
#include "music_play.h"
#include "play_file.h"


LCD_SEG4X8_VAR _idata LCD_SEG4X8_var;
LCDC_BUF __xdata LCDC_buf;

const u8 LCD_SEG_NUMBER[] AT(LCD_SEG_TABLE_CODE)=
{
    /*0*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF),
    /*1*/
    (u8)(SEGB | SEGC),
    /*2*/
    (u8)(SEGA | SEGB | SEGD | SEGE | SEGG),
    /*3*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGG),
    /*4*/
    (u8)(SEGB | SEGC | SEGF | SEGG),
    /*5*/
    (u8)(SEGA | SEGC | SEGD | SEGF | SEGG),
    /*6*/
    (u8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*7*/
    (u8)(SEGA | SEGB | SEGC),
    /*8*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*9*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
};

const u8 LCD_SEG_SMALL_NUMBER[] AT(LCD_SEG_TABLE_CODE)=
{
    /*a*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF),
    /*b*/
    (u8)(SEGB | SEGC),
    /*c*/
    (u8)(SEGA | SEGB | SEGD | SEGE | SEGG),
    /*d*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGG),
    /*e*/
    (u8)(SEGB | SEGC | SEGF | SEGG),
    /*f*/
    (u8)(SEGA | SEGC | SEGD | SEGF | SEGG),
    /*g*/
    (u8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*h*/
    (u8)(SEGA | SEGB | SEGC),
    /*i*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*j*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*k*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*l*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*m*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*n*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*o*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*p*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*q*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*r*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*s*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*t*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*u*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*v*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*w*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*x*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*y*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*z*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),  
};

const u8 LCD_SEG_LARGE_NUMBER[] AT(LCD_SEG_TABLE_CODE)=
{
    /*A*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF),
    /*B*/
    (u8)(SEGB | SEGC),
    /*C*/
    (u8)(SEGA | SEGB | SEGD | SEGE | SEGG),
    /*D*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGG),
    /*E*/
    (u8)(SEGB | SEGC | SEGF | SEGG),
    /*F*/
    (u8)(SEGA | SEGC | SEGD | SEGF | SEGG),
    /*G*/
    (u8)(SEGA | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*H*/
    (u8)(SEGA | SEGB | SEGC),
    /*I*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGE | SEGF | SEGG),
    /*J*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*K*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*L*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*M*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*N*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*O*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*P*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*Q*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*R*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*S*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*T*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*U*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*V*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*W*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*X*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*Y*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
    /*Z*/
    (u8)(SEGA | SEGB | SEGC | SEGD | SEGF | SEGG),
};

const u8 playmodestr[4][5] AT(LCD_SEG_TABLE_CODE)=             
{
    " ALL",
    "Inro",
    " ONE",
    " rAn",
};

/*----------------------------------------------------------------------------*/
/**@brief   LCD 段码屏初始化函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_init(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_init(void) AT(LCD_SEG_CODE)
{
    CLK_CON2 |= BIT(4);                 //[4]LCD Clk
    //CLK_GAT |= BIT(5);  
    LCD_CON0 = BIT(6)|BIT(5)|BIT(4);	//[6:4]:VLCDS 3.3V
    LCD_CON1 = BIT(7)|BIT(6);           //[7:6]:4 COM
    LCD_CON0 |= BIT(7)|BIT(3)|BIT(2)/**/;      	//[7]: Frame Frequency [3:2]:1/3 Bias
    //LCDCON0 |= BIT(1);	            //[1]:DOTEN COM0-SEG0
    //SEG_EN0 = 0xFF;
    SEG_EN1 = 0xFF;
    LCD_CON0 |= BIT(0);	                //[0]:LCDEN
    
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD_SEG 显示坐标设置
   @param   x：显示横坐标
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_setX(u8 X)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_setX(u8 X) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_var.bCoordinateX = X;
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD_SEG 屏幕清除函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_clear(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_clear_icon(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_var.bFlashChar = 0;
    LCD_SEG4X8_var.bFlashIcon = 0;
}


/*----------------------------------------------------------------------------*/
/**@brief   LCD_SEG 单个字符显示函数
   @param   chardata：显示字符
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_char(u8 chardata)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_char(u8 chardata) AT(LCD_SEG_CODE)
{
    if ((chardata < ' ') || (chardata > '~') || (LCD_SEG4X8_var.bCoordinateX > 4))
    {
        return;
    }
    if ((chardata >= '0') && (chardata <= '9'))
    {
        LCD_SEG4X8_var.bShowBuff[LCD_SEG4X8_var.bCoordinateX++] = LCD_SEG_NUMBER[chardata - '0'];
    }
    else if ((chardata >= 'a') && (chardata <= 'z'))
    {
        LCD_SEG4X8_var.bShowBuff[LCD_SEG4X8_var.bCoordinateX++] = LCD_SEG_SMALL_NUMBER[chardata - 'a'];
    }
    else if ((chardata >= 'A') && (chardata <= 'Z'))
    {
        LCD_SEG4X8_var.bShowBuff[LCD_SEG4X8_var.bCoordinateX++] = LCD_SEG_LARGE_NUMBER[chardata - 'A'];
    }
    else if (chardata == ':')
    {
        //LED_STATUS |= LED_2POINT;
    }
    else if (chardata == ' ')
    {
        LCD_SEG4X8_var.bCoordinateX++;
    }
    else if (chardata == '-')
    {
        LCD_SEG4X8_var.bShowBuff[LCD_SEG4X8_var.bCoordinateX++] = BIT(6);
    }  
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD_SEG 单个字符显示函数
   @param   chardata：显示字符
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_number(u8 number)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_number(u8 number) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_char(number + '0');
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD_SEG 字符串显示函数
   @param   *str：字符串的指针   offset：显示偏移量
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_string(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_string(u8 *str) AT(LCD_SEG_CODE)
{
    while (0 != *str)
    {
        LCD_SEG4X8_show_char(*str++);
    } 
}


/*----------------------------------------------------------------------------*/
/**@brief   LED5X7 开机显示界面（Hi）
   @param   *str：字符串的指针 
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_Hi(u8 *str)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_Hi(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)"HI  ");
}


/*----------------------------------------------------------------------------*/
/**@brief   设备播放等待界面
   @param   *str：字符串的指针 
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_waiting(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_waiting(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)"Lod ");
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_music_main(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_music_main(void) AT(LCD_SEG_CODE)
{
    u16 play_time;
    
    /*Music Play time info*/
    play_time = get_music_play_time();
    
    LCD_SEG4X8_show_number(play_time/60/10);
    LCD_SEG4X8_show_number(play_time/60%10);

    LCD_SEG4X8_show_number(play_time%60/10);
    LCD_SEG4X8_show_number(play_time%10%10);   
}

/*----------------------------------------------------------------------------*/
/**@brief   RTC 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_RTC_main(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_RTC_main(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_number(curr_time.bHour/10);
    LCD_SEG4X8_show_number(curr_time.bHour%10);

    LCD_SEG4X8_show_number(curr_time.bMin/10);
    LCD_SEG4X8_show_number(curr_time.bMin%10);    
}

/*----------------------------------------------------------------------------*/
/**@brief   Music 播放文件号显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_filenumber(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_filenumber(void) AT(LCD_SEG_CODE)
{
    /*Music File Number info*/
    LCD_SEG4X8_show_number(playfile.given_file_number/1000);
    LCD_SEG4X8_show_number(playfile.given_file_number%1000/100);
    LCD_SEG4X8_show_number(playfile.given_file_number%100/10);
    LCD_SEG4X8_show_number(playfile.given_file_number%10);        
}

/*----------------------------------------------------------------------------*/
/**@brief   音量显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_volume(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_volume(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_char('V');
    LCD_SEG4X8_setX(2);
    LCD_SEG4X8_show_number(main_vol_L/10);
    LCD_SEG4X8_show_number(main_vol_L%10);     
}

/*----------------------------------------------------------------------------*/
/**@brief   FM 模式主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_fm_main(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_fm_main(void) AT(LCD_SEG_CODE)
{
    /*FM - Frequency*/
    if (fm_mode_var.wFreq > 999)
        LCD_SEG4X8_show_number(fm_mode_var.wFreq/1000);
    LCD_SEG4X8_setX(1);
    LCD_SEG4X8_show_number(fm_mode_var.wFreq%1000/100);   
    LCD_SEG4X8_show_number(fm_mode_var.wFreq%100/10);
    LCD_SEG4X8_show_number(fm_mode_var.wFreq%10);
}

/*----------------------------------------------------------------------------*/
/**@brief   红外输入文件号显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_IR_number(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_IR_number(void) AT(LCD_SEG_CODE)
{
    /*IR File Number info*/
    LCD_SEG4X8_show_number(input_number/1000);
    LCD_SEG4X8_show_number(input_number%1000/100);
    LCD_SEG4X8_show_number(input_number%100/10);
    LCD_SEG4X8_show_number(input_number%10);
}

/*----------------------------------------------------------------------------*/
/**@brief   AUX 显示主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_pc_main(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_pc_main(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)" PC ");   
}

/*----------------------------------------------------------------------------*/
/**@brief   AUX 显示主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_pc_vol_up(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_pc_vol_up(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)" UP ");   
}

/*----------------------------------------------------------------------------*/
/**@brief   AUX 显示主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_pc_vol_down(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_pc_vol_down(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)" dN ");   
}

/*----------------------------------------------------------------------------*/
/**@brief   PC 显示主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_aux_main(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_aux_main(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)" AUX");  
}

/*----------------------------------------------------------------------------*/
/**@brief   EQ显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_eq(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_eq(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_char('E');
    LCD_SEG4X8_show_char('q');
    LCD_SEG4X8_setX(3);
    LCD_SEG4X8_show_number(Music_Play_var.bEQ%10); 
}

/*----------------------------------------------------------------------------*/
/**@brief   循环模式显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_playmode(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_playmode(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)&playmodestr[playfile.play_mode][0]);
}

/*----------------------------------------------------------------------------*/
/**@brief   暂停显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_pause(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_pause(void) AT(LCD_SEG_CODE)
{
    LCD_SEG4X8_show_string((u8 *)"PAU  ");
}

/*----------------------------------------------------------------------------*/
/**@brief   FM 模式主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_show_fm_station(void)
*/
/*----------------------------------------------------------------------------*/
void LCD_SEG4X8_show_fm_station(void) AT(LCD_SEG_CODE)
{
    /*FM - Station*/
    LCD_SEG4X8_show_char('P');
    LCD_SEG4X8_setX(2);  
    LCD_SEG4X8_show_number(fm_mode_var.bFreChannel/10);
    LCD_SEG4X8_show_number(fm_mode_var.bFreChannel%10);
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 段码屏4X8 1/3bias 扫描驱动
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_SEG4X8_scan(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_SEG4X8_scan(void) AT(COMMON_CODE)
{
    u8 dat[6];
    
    my_memcpy(dat, (u8 *)LCD_SEG4X8_var.bShowBuff, 6);

    //deg("Seg : %02bx-%02bx-%02bx\n", dat[1], dat[2], dat[3]);
    LCDC_buf.bCom2Seg1 = ((dat[1] & SEGA)<<2) | ((dat[2] & SEGA)<<4) | ((dat[3] & SEGA)<<6);
    LCDC_buf.bCom3Seg1 = ((dat[1] & (SEGF|SEGB))) | ((dat[2] & (SEGF|SEGB))<<2) | ((dat[3] & (SEGF|SEGB))<<4);
    LCDC_buf.bCom4Seg1 = ((dat[1] & (SEGG|SEGC))>>2) | ((dat[2] & (SEGG|SEGC))) | ((dat[3] & (SEGG|SEGC))<<2);
    LCDC_buf.bCom5Seg1 = ((dat[1] & (SEGE|SEGD))>>4) | ((dat[2] & (SEGE|SEGD))>>2) | ((dat[3] & (SEGE|SEGD)));

    //deg("Seg Data: %02bx-%02bx-", LCDC_buf.bCom2Seg1, LCDC_buf.bCom3Seg1);
    //deg("%02bx-%02bx\n", LCDC_buf.bCom4Seg1, LCDC_buf.bCom5Seg1);
    /*
    COM2_SEG1(LCDC_buf.bCom2Seg1);
    COM3_SEG1(LCDC_buf.bCom3Seg1);
    COM4_SEG1(LCDC_buf.bCom4Seg1);
    COM5_SEG1(LCDC_buf.bCom5Seg1);*/
    _no_init static _data u8 index;
    
    COM0_SEG1(0xAA/*BIT(index)*/);
    COM1_SEG1(0x55/*BIT(index)*/);
    COM2_SEG1(0xAA/*BIT(index)*/);
    COM3_SEG1(0x55/*BIT(index)*/);
    COM4_SEG1(0xAA/*BIT(index)*/);
    COM5_SEG1(0xFF/*BIT(index)*/);
      
    index = (index > 7) ? 0 : index+1;
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 段码屏4X8 1/2bias 扫描驱动
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_drive4X8_12(void) 
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_drive4X8_12(void) AT(COMMON_CODE)
{
    u8 dat[6];

    my_memcpy(dat, (u8 *)LCD_SEG4X8_var.bShowBuff, 6);

    //deg("Seg : %02bx-%02bx-%02bx\n", dat[1], dat[2], dat[3]);
    LCDC_buf.bCom0Seg1 = ((dat[1] & (SEGA|SEGB))<<2) | ((dat[2] & (SEGA|SEGB))<<4) | ((dat[3] & (SEGA|SEGB))<<6);
    LCDC_buf.bCom1Seg1 = ((dat[1] & (SEGF|SEGG))) | ((dat[2] & (SEGF|SEGG))<<2) | ((dat[3] & (SEGF|SEGG))<<4);
    LCDC_buf.bCom2Seg1 = ((dat[1] & (SEGE|SEGC))>>2) | ((dat[2] & (SEGE|SEGC))) | ((dat[3] & (SEGE|SEGC))<<2);
    LCDC_buf.bCom3Seg1 = ((dat[1] & (SEGD))>>4) | ((dat[2] & (SEGD))>>2) | ((dat[3] & (SEGD)));

//  deg("Seg Data: %02bx-%02bx-", LCDC_buf.bCom0Seg1, LCDC_buf.bCom1Seg1);
//  deg("%02bx-%02bx\n", LCDC_buf.bCom2Seg1, LCDC_buf.bCom3Seg1);

    COM0_SEG1(LCDC_buf.bCom0Seg1);
    COM1_SEG1(LCDC_buf.bCom1Seg1);
    COM2_SEG1(LCDC_buf.bCom2Seg1);
    COM3_SEG1(LCDC_buf.bCom3Seg1);
}

#endif  /*  LCD_SEG_4X8 */
