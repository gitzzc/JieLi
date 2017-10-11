#ifndef _LCD_H_
#define _LCD_H_

#include "config.h"


#ifdef __IAR_SYSTEMS_ICC__



//串口
#define LCD_DATA_OUT    P1DIR &= ~BIT(7); P1PU |= BIT(7)//7
#define	LCD_CLK_OUT	    P1DIR &= ~BIT(6); P1PU |= BIT(6)//6

//并口
#define LCD_DATA_OUT_PAR  P1DIR = 0x00;P1PU = 0xff;P1= 0XFF
#define LCD_CMD_OUT_PAR	  P3DIR &= ~0X1E;P3PU |= 0x1E;P3 |= 0X1E


#define LCDPORT_OUT()     LCD_DATA_OUT;LCD_CLK_OUT;LCD_A0_OUT;LCD_RES_OUT;LCD_CS_OUT
#define LCDPORT_H()       P1 |= ( BIT(7)|BIT(6) );LCD_A0_H; LCD_RES_H;/**/	LCD_CS_H

#define LCDPORT_OUT_PAR()   LCD_WR_OUT;LCD_DATA_OUT_PAR;LCD_CMD_OUT_PAR
#define LCDPORT_H_PAR()     P1 = 0xff; LCD_A0_H; /*LCD_RES_H;*/LCD_CS_H;LCD_WR_H



#define LCD_WR_OUT	  P3DIR &= ~BIT(3);P3PU |= BIT(3)
#define LCD_WR_L	  P33 = 0
#define LCD_WR_H	  P33 = 1


#define	LCD_A0_OUT	  P3DIR &= ~BIT(2);P3PU |= BIT(2)
#define	LCD_A0_L	  P32 = 0
#define	LCD_A0_H	  P32 = 1

#define LCD_RES_OUT	  P3DIR &= ~BIT(4);P3PU |= BIT(4)
#define LCD_RES_L	  P34=  0
#define LCD_RES_H	  P34 = 1

#define LCD_CS_OUT	  P3DIR &= ~BIT(1);P3PU |= BIT(1)
#define LCD_CS_L	  P31 = 0
#define LCD_CS_H	  P31 = 1

#define _nop_()     __no_operation()


typedef struct _LCD_VAR
{
    u8  bMaxPage;
    u8  bMaxColumn;
    u8  bBusy;
    u8  bCurPage;       //<当前页
    u8  bUpdateLength;  //<更新长度 
    u8  bCoordinateX;
    u8  bCoordinateY;
    u8  bFontSize;     //<字符大小
}LCD_VAR;

enum
{
    LCD_BMP_SDMMC0 = 0,
    LCD_BMP_SDMMC1,
    LCD_BMP_UDISK,
    LCD_BMP_AUX,
    LCD_BMP_FM,
    LCD_BMP_VOL,
    LCD_BMP_MP3,
    LCD_BMP_WMA,
    LCD_BMP_WAV,
    LCD_BMP_PM_ALL,
    LCD_BMP_PM_FOLDER,
    LCD_BMP_PM_ONE,
    LCD_BMP_PM_RANDOM,
    LCD_BMP_EQ_NOR,
    LCD_BMP_EQ_POP,
    LCD_BMP_EQ_RCK,
    LCD_BMP_EQ_JAZ,
    LCD_BMP_EQ_CLS,
};


_near_func void LCD_set_XY(u8 x,u8 y);
_near_func void LCD_clear_area(u8 p1,u8 p2);
_near_func void LCD_SET(u8 y, u8 x);
_near_func u16 display_sizex2(u8 chardata);
_near_func void LCDSendCmd(u8 cmd);


_near_func void LCD_init(void);
_near_func void LCD_clear(void);
_near_func void LCD_Update(u8 start_page, u8 page_len);
_near_func void LCD_show_hello(void);
_near_func void LCD_show_music_main(void);
_near_func void LCD_show_filenumber(u16);
_near_func void LCD_show_volume(void);
_near_func void LCD_show_IR_number(void);
_near_func void LCD_show_pause(void);
_near_func void LCD_show_aux_main(void);
_near_func void LCD_show_RTC_main(void);
_near_func void LCD_show_pc_main(void);
_near_func void LCD_show_pc_vol_up(void);
_near_func void LCD_show_pc_vol_down(void);
_near_func void LCD_show_playmode(void);
_near_func void LCD_show_reverse_area(u8 x, u8 y, u8 x_size, u8 y_size);
_near_func void LCD_show_week_alarm(void);
_near_func void LCD_show_alarm_up(void);
_near_func void LCD_show_waiting(void);
_near_func void LCD_show_fm_main(void);

extern LCD_VAR _xdata LCD_var;

#define LCD_START_LUMN          0
#define LCD_END_LUMN            LCD_var.bMaxColumn

#endif  /*  __IAR_SYSTEMS_ICC__ */

#define LCD_COLUMN_LOW          0x00
#define LCD_COLUMN_HIGH         0x10
#define LCD_V0_RATIO            0x20
#define LCD_POWER_MODE          0x2f
#define LCD_START_LINE          0x40
#define LCD_ELE_VOLUME          0x81
#define LCD_STATIC_IND_OFF      0xac
#define LCD_ON                  0xaf
#define LCD_OFF                 0xae
#define LCD_ALL_ON              0xa5
#define LCD_ALL_NORMAL          0xa4
#define LCD_STATIC_IND_ON       0xad
#define LCD_ADC_SEL             0xa0
#define LCD_DISP_MODE           0xa6
#define LCD_BIAS_SET            0xa2
#define LCD_PAGE_ADDR           0xb0
#define LCD_COMMON_MODE         0xc8
#define LCD_PAGE_BLINK          0xd5
#define LCD_DRIV_MODE           0xd2
#define LCD_NOP                 0xe3
#define LCD_OSC_SET             0xe4
#define LCD_END                 0xee
#define LCD_SOFT_RST            0xe2

#define LCD_CONTRAST_12832			0x44                  //contrast
#define LCD_CONTRAST_12864			0x34//0x20                  //contrast

/*UI - Music 界面定位*/
#define DEVICE_COLUMN 1
#define PLAYTIME_COLUMN (DEVICE_COLUMN+16+2)

#define EQ_COLUMN       (DEVICE_COLUMN+16+2+29)
#define FILENUM_COLUMN  (DEVICE_COLUMN+16+2+29+21)


/*UI - RTC 界面定位*/
#define RTC_YEAR	   	(LCD_END_LUMN-10*8)/2+0
#define RTC_MONTH	    (LCD_END_LUMN-10*8)/2+40
#define RTC_DAY 		(LCD_END_LUMN-10*8)/2+64
#define RTC_HOUR		(LCD_END_LUMN-10*8)/2+0
#define RTC_MIN		    (LCD_END_LUMN-10*8)/2+24
#define RTC_SEC		    (LCD_END_LUMN-10*8)/2+48

#define ALM_SWITCH      (LCD_END_LUMN-8*8)/2+48
#define ALM_HOUR	    (LCD_END_LUMN-8*8)/2+0
#define ALM_MIN		    (LCD_END_LUMN-8*8)/2+24

#define ALM_MONDAY      (LCD_END_LUMN-7*10)/2+0
#define ALM_TUESDAY	    (LCD_END_LUMN-7*10)/2+10
#define ALM_WEDNESDAY	(LCD_END_LUMN-7*10)/2+20
#define ALM_THRUSDAY	(LCD_END_LUMN-7*10)/2+30
#define ALM_FRIDAY  	(LCD_END_LUMN-7*10)/2+40
#define ALM_SATURDAY  	(LCD_END_LUMN-7*10)/2+50
#define ALM_SUNDAY  	(LCD_END_LUMN-7*10)/2+60

#define FRE_4_COLUMN    ((LCD_END_LUMN - 8*8)/2)
#define FRE_3_COLUMN    ((LCD_END_LUMN - 7*8)/2)
#define FM_CHANNL_COLUMN    1

#endif  /*  _LCD_H_ */  

