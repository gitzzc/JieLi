/*--------------------------------------------------------------------------*/
/**@file    LCD.c
   @brief   LCD 点阵屏驱动接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "UI_API.h"

#if defined LCD_96X32_SERIAL || defined LCD_128X32_SERIAL \
  || defined LCD_128X64_SERIAL || defined LCD_128X64_PARALLEL

#include "main.h"
#include "lcd.h"
#include "LCD_char.c"
#include "device.h"
#include "get_music_file.h"
#include "hot_msg.h"
#include "RTC_API.h"
#include "play_file.h"
#include "fm_radio.h"
#include "music_play.h"
#include "IRTC.h"


u8 _xdata LCDBuff[4][96];     //<LCD 显示buffer
LCD_VAR _xdata LCD_var;
u8 _idata filename_len, filenameCnt;

const u8 lcd_init_table_ser[] =
{
    LCD_SOFT_RST,
    LCD_BIAS_SET | 0x00,      //lcd bias setting
    LCD_ADC_SEL,              //ADC invert display  //0x00 or 0x01
    LCD_COMMON_MODE,          //common output mode  //0xc0 or oxc8		
	LCD_V0_RATIO | 0x03,      //Resistor Ratio
    LCD_POWER_MODE,           //0x2f lcd power control
    LCD_START_LINE | 0,       //0x40
    LCD_ON,
    LCD_ELE_VOLUME,           //0X81 electronic Volume
};

const u8 lcd_init_table_para[] =
{
    LCD_BIAS_SET | 0x00,      //lcd bias setting
    LCD_ADC_SEL ,              //ADC invert display  //0x00 or 0x01
    LCD_COMMON_MODE,          //common output mode  //0xc0 or oxc8		
	LCD_V0_RATIO | 0x04,      //Resistor Ratio
    LCD_ELE_VOLUME,           //0X81 electronic Volume
    0x1b,                     //contrast
    LCD_POWER_MODE,           //0x2f lcd power control
    LCD_START_LINE | 0,       //0x40
    LCD_ON
};

const u8 lcd_bmp_size_table[][2] =
{
    {2, 15},
    {0, 0},
    {2, 15},
    {0, 0},
    {0, 0},
    {4, 31},
    {2, 22},
    {2, 23},
    {2, 22},
    {2, 11},
    {2, 11},
    {2, 11},
    {2, 11},
    {2, 17},
    {2, 17},
    {2, 17},
    {2, 17},
    {2, 17},
};

const u16 lcd_bmp_add_table[] =
{
    (u16)gImage_sd,
    0,
    (u16)gImage_udisk,
    0,
    0,
    (u16)gImage_vol,
    (u16)gImage_mp3,
    (u16)gImage_wma,
    (u16)gImage_wav,
    (u16)gImage_play_all,
    (u16)gImage_play_folder,
    (u16)gImage_play_one,
    (u16)gImage_play_random,
    (u16)gImage_eqnor,
    (u16)gImage_eqpop,
    (u16)gImage_eqrck,
    (u16)gImage_eqjaz,
    (u16)gImage_eqcls
};

/*----------------------------------------------------------------------------*/
/**@brief   LCD DMA 中断服务程序
   @param   void
   @return  void
   @author  Change.tsai
   @note    void lcd_dma_isr(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void lcd_dma_isr(void)  AT(LCD_CODE)
{
    if (SPI_STA & BIT(7))   //<共用中断入口，判断中断源
    {
        LCD_CS_H;
        _nop_();
        _nop_();
        _nop_();
        if (LCD_var.bUpdateLength == 0)
        {
            LCD_var.bBusy = 0;
            SPI_STA &= ~BIT(5);//SPI_IE(0);
            return;
        }
        SPI_STA |= BIT(6);
        LCD_var.bBusy = 1;
        LCD_SET(LCD_var.bCurPage, 0);
        _nop_();
        _nop_();
        _nop_();
        LCD_A0_H;
        _nop_();
        _nop_();
        _nop_();
        LCD_CS_L;
        _nop_();
        _nop_();
        _nop_();
        SPI_STA &= ~BIT(4);
        SPI_ADR = ((u16)(&LCDBuff[LCD_var.bCurPage][0])) >> 8;
        SPI_ADR = ((u16)(&LCDBuff[LCD_var.bCurPage][0]));
        SPI_CNT = 0;
        SPI_CNT = LCD_var.bMaxColumn;
    
        LCD_var.bCurPage++;
        LCD_var.bUpdateLength--;
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 发送命令函数
   @param   cmd：命令
   @return  void
   @author  Change.tsai 
   @note    void LCDSendCmd(u8 cmd)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCDSendCmd(u8 cmd) AT(LCD_CODE)
{
    if (SPI_STA & BIT(7))
        SPI_STA |= BIT(6);
    LCD_A0_L;    	//LCD_COMMAND
    _nop_();
    _nop_();
    _nop_();
    LCD_CS_L;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    SPI_STA &= ~BIT(4);     //Output
    SPI_BUF = cmd;
    while (!(SPI_STA & BIT(7))); //等待发送完成
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LCD_CS_H;
}

/*----------------------------------------------------------------------------*/
/**@brief  设置当前显示的坐标
   @param  x : 列坐标; y : 行坐标;
   @return void
   @author  Change.tsai
   @note   void LCD_set_XY(u8 x,u8 y)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_set_XY(u8 x,u8 y)  AT(LCD_CODE)
{
    LCD_var.bCoordinateX = x;
    LCD_var.bCoordinateY = y;
}

/*----------------------------------------------------------------------------*/
/**@brief   清全屏，只清BUFF但不更新
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_clear(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_clear(void) AT(LCD_CODE)
{
    while (LCD_var.bBusy);
    my_memset(&LCDBuff[0][0], 0x0, LCD_var.bMaxPage*LCD_var.bMaxColumn);
    LCD_set_XY(LCD_START_LUMN, 0);

}

/*----------------------------------------------------------------------------*/
/**@brief   清屏的指定区域，只清BUFF但不更新
   @param   p1：起始 page 
   @param   p2：结束 page 
   @return  void
   @author  Change.tsai
   @note    void LCD_clear_area(u8 p1,u8 p2)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_clear_area(u8 p1,u8 p2) AT(LCD_CODE)
{
    while (LCD_var.bBusy);
    for (; p1 <= p2; p1++)
    {
        my_memset(&LCDBuff[p1][0], 0, LCD_var.bMaxColumn);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   设置显示的起始坐标
   @param   x:列坐标
   @param   y:行坐标
   @return  void
   @author  Change.tsai
   @note    void LCD_SET(u8 y,u8 x)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_SET(u8 y, u8 x) AT(LCD_CODE)
{
    LCDSendCmd(LCD_COLUMN_HIGH | x>>4);
    LCDSendCmd(LCD_COLUMN_LOW | (0x0f & x));
    LCDSendCmd(LCD_PAGE_ADDR | y);
}

/*----------------------------------------------------------------------------*/
/**@brief   更新显存的内容到LCD
   @param   start_page ：更新开始的起始Page
   @param   page_len ：更新的Page数
   @return  void
   @author  Change.tsai
   @note    void updataLCD(u8 start_page, u8 page_len)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_Update(u8 start_page, u8 page_len) AT(LCD_CODE)
{
    while (LCD_var.bBusy);
    LCD_var.bCurPage = start_page;
    LCD_var.bUpdateLength = page_len;
    int_enter_pro[SPI_IIC_INT] = lcd_dma_isr;
    SPI_STA |= BIT(5);
    SPI_IP(1);
    SPI_IE(1);
}

/*----------------------------------------------------------------------------*/
/**@brief   字模放大到兩倍函數
   @param   八位的單列字模
   @return  放大到16位的列字模
   @author  Change.tsai
   @note    u16 display_sizex2(u8 chardata)
*/
/*----------------------------------------------------------------------------*/
_near_func u16 display_sizex2(u8 chardata) AT(LCD_CODE)
{
    u16 result = 0;
    u8 i;

    for (i = 0; i < 8; i++)
    {
        result <<= 1;
        if (chardata & 0x80)
            result++;
        result <<= 1;
        if (chardata & 0x80)
            result++;
        chardata <<= 1;
    }
    return result;
}

/*----------------------------------------------------------------------------*/
/**@brief   将单列的字模放入到指定的坐标中
   @param   y：行坐标；
   @param   x：列坐标；
   @param   dat:八位的字模
   @return  void
   @author  Change.tsai
   @note    void to_lcdbuff(u8 y,u8 x, u8 dat)
*/
/*----------------------------------------------------------------------------*/
_near_func void to_lcdbuff(u8 y,u8 x, u8 dat) AT(LCD_CODE)
{
    if ( (x >= LCD_var.bMaxColumn ) || (y >= LCD_var.bMaxPage ) )
    {
        return;
    }
    LCDBuff[y][x] = dat;
}
/*----------------------------------------------------------------------------*/
/**@brief   将一个指定大小的图片放在现存中
   @param   y:行尺寸；
   @param   x：列尺寸；
   @param   *bmp一个指向图片模型的指针（图片放在CODE区） 
   @return  void
   @author  Change.tsai
   @note    void LCD_show_bmp(u8 y,u8 x,u8 _code *bmp)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_bmp(u8 y,u8 x,const u8 *bmp) AT(LCD_CODE)
{
    u8 i,j;

    for (i = 0;i < x;i++)
    {
        for (j = 0;j < y;j++)
        {	  
            if (LCD_var.bFontSize == 3)
                to_lcdbuff(LCD_var.bCoordinateY+j,LCD_var.bCoordinateX,*(bmp+j*x+i) | 0x01);
            else
                to_lcdbuff(LCD_var.bCoordinateY+j,LCD_var.bCoordinateX,*(bmp+j*x+i));
        }
        LCD_var.bCoordinateX++;
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   LCD显示根据id号显示图片
   @param   id:图片号
   @return  void
   @author  Change.tsai
   @note    void lcd_disp_bmp_via_id(u8 id)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_bmp_via_id(u8 id) AT(LCD_CODE)
{
    LCD_show_bmp(lcd_bmp_size_table[id][0], lcd_bmp_size_table[id][1], (const u8 *)lcd_bmp_add_table[id]);
}


/*----------------------------------------------------------------------------*/
/**@brief   LCD显示字符
   @param   chardata：字符数据
   @return  void
   @author  Change.tsai
   @note    void LCD_show_char (u8 chardata)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_char(u8 chardata) AT(LCD_CODE)
{
    u8 i;
    u16 temp;

    if (( chardata < ' ') || ( chardata > 127))
    {
        chardata = '~';
    }
    if (chardata == '\n')							//换行
    {
        LCD_var.bCoordinateX++;
        if (LCD_var.bCoordinateY >= LCD_var.bMaxPage)
            LCD_var.bCoordinateY = 0;
        LCD_var.bCoordinateX = LCD_START_LUMN;
    }
    else if ((LCD_var.bFontSize == 2) || (LCD_var.bFontSize == 3))
    {
        if (chardata == ':')
        {
            LCD_show_bmp(1, 3, (const u8 *)gImage_small_dot);
        }
        else
        {
            LCD_show_bmp(1, 6, (const u8 *)(gImage_number + ((chardata - '0') * 6)));
        }
    }
    else if (LCD_var.bFontSize == 1)
    {
        for (i = 0;i<8;i++)
        {
            if (LCD_var.bCoordinateX >= (LCD_var.bMaxColumn - 2))
            {
                return ;
            }
            temp = display_sizex2(CharsTable[16*(chardata - ' ')+i]);
            to_lcdbuff(LCD_var.bCoordinateY, LCD_var.bCoordinateX, temp);
            to_lcdbuff(LCD_var.bCoordinateY, LCD_var.bCoordinateX + 1, temp);
            to_lcdbuff(LCD_var.bCoordinateY + 1, LCD_var.bCoordinateX, temp>>8);
            to_lcdbuff(LCD_var.bCoordinateY + 1, LCD_var.bCoordinateX+1, temp>>8);

            temp = display_sizex2(CharsTable[16*(chardata - ' ')+i+8]);
            to_lcdbuff(LCD_var.bCoordinateY + 2,LCD_var.bCoordinateX,temp);
            to_lcdbuff(LCD_var.bCoordinateY + 2,LCD_var.bCoordinateX + 1,temp);
            to_lcdbuff(LCD_var.bCoordinateY +2+1,LCD_var.bCoordinateX,temp>>8);
            to_lcdbuff(LCD_var.bCoordinateY +2+1,LCD_var.bCoordinateX + 1,temp>>8);
            LCD_var.bCoordinateX += 2;
        }
    }
    else
    {
        for (i = 0; i < 8; i++)
        {
            if (LCD_var.bCoordinateX >= LCD_var.bMaxColumn)
            {
                return ;
            }
            to_lcdbuff(LCD_var.bCoordinateY, LCD_var.bCoordinateX, CharsTable[16*(chardata - ' ')+i]);
            to_lcdbuff(LCD_var.bCoordinateY+1, LCD_var.bCoordinateX, CharsTable[16*(chardata - ' ')+i+8]);
            LCD_var.bCoordinateX++;
        }
    }

}

/*----------------------------------------------------------------------------*/
/**@brief    显示一个字符串
   @param    *str：数组指针;
   @param    size：数组的字符显示大小;
   @return   void
   @author  Change.tsai
   @note     void LCD_show_string(u8 *str,u8 size)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_string(u8 *str,u8 size) AT(LCD_CODE)
{
    LCD_var.bFontSize = size;
    while (0 != *str)
    {
        LCD_show_char(*str++);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 模块初始化函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_init(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_init(void) AT(LCD_CODE)
{
    u8 i;
    
#ifdef LCD_128X64_PARALLEL
    LCDPORT_OUT_PAR();
    LCDPORT_H_PAR();
#else
    LCDPORT_OUT();
    LCDPORT_H();
#endif

    LCD_RES_L;
    delay16(1000);
    LCD_RES_H;
    delay16(100);
    
#ifdef LCD_128X64_PARALLEL    
    SPI_BAUD = 21;
    SPI_CON = BIT(6)|BIT(0);			//Enable SPI/2 Wire/Output
    SPI_STA &= ~BIT(4);
    SPI_STA |= BIT(3);
#else    
    SPI_BAUD = 4;
    SPI_CON = BIT(6)|BIT(4)|BIT(0);		//Enable SPI/2 Wire/Output
    SPI_STA &= ~BIT(4);
    IO_MC1 |= BIT(5);
    IO_MC1 &= ~BIT(4);
#endif
    
    LCD_var.bBusy = 0;

#if defined LCD_128X64_PARALLEL
    for (i = 0; i < sizeof(lcd_init_table_para); i++)
    {
        LCDSendCmd(lcd_init_table_para[i]);
        delay8(10);
    }
    LCD_var.bMaxPage = 8;
    LCD_var.bMaxColumn = 132;
#else
    for (i = 0; i < sizeof(lcd_init_table_ser); i++)
    {
        LCDSendCmd(lcd_init_table_ser[i]);
        delay8(10);
    }
    LCDSendCmd(LCD_CONTRAST_12832);
    LCD_var.bMaxPage = 4;
    LCD_var.bMaxColumn = 96;
#endif
    
    my_memset((u8 *)&LCDBuff[0][0], 0x00, LCD_var.bMaxColumn*LCD_var.bMaxPage);
}
/*----------------------------------------------------------------------------*/
/**@brief   LCD 开机显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_hello(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_hello(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*5)) / 2,0);
    LCD_show_string((u8 *)"HELLO", 1);
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 设备读取等待界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_waiting(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_waiting(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (10 * 8)) / 2,2);
    LCD_show_string((u8 *)"waiting...", 0);
}

/*----------------------------------------------------------------------------*/
/**@brief   用最小的数字字体显示当前播放歌曲的曲号和当前设备的总歌曲数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_small_filenumber(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_small_filenumber(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 2;  
    LCD_set_XY(FILENUM_COLUMN,0);
    LCD_show_char(music_device_info[device_active].wBreak_point_filenum/1000 + '0');
    LCD_show_char(music_device_info[device_active].wBreak_point_filenum%1000/100 + '0');
    LCD_show_char(music_device_info[device_active].wBreak_point_filenum%100/10 + '0');
    LCD_show_char(music_device_info[device_active].wBreak_point_filenum%10 + '0');
    LCD_set_XY(FILENUM_COLUMN,1);
    LCD_show_char(music_device_info[device_active].wfileTotal/1000 + '0');
    LCD_show_char(music_device_info[device_active].wfileTotal%1000/100 + '0');
    LCD_show_char(music_device_info[device_active].wfileTotal%100/10 + '0');
    LCD_show_char(music_device_info[device_active].wfileTotal%10 + '0');
}
/*----------------------------------------------------------------------------*/
/**@brief   用最小的数字字体显示当前播放的时间，以及当前文件的总时间
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_music_time(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_music_time(void) AT(LCD_CODE)
{
    u8 sec;
    u8 min;
    u32 file_play_time;
    
    LCD_var.bFontSize = 2; 
    file_play_time = get_music_play_time();
    min = (file_play_time/60);
    sec = file_play_time % 60;
    
    LCD_set_XY(PLAYTIME_COLUMN,0);    
    LCD_show_char(min/10 + '0');
    LCD_show_char(min%10 + '0');
    LCD_show_char(':');
    LCD_show_char(sec/10 + '0');
    LCD_show_char(sec%10 + '0');
    
    LCD_set_XY(PLAYTIME_COLUMN,1);
    min = dec_msg.total_time/60;
    sec = dec_msg.total_time % 60;
    LCD_show_char(min/10 + '0');
    LCD_show_char(min%10 + '0');
    LCD_show_char(':'); 
    LCD_show_char(sec/10 + '0');
    LCD_show_char(sec%10 + '0');
}

/*----------------------------------------------------------------------------*/
/**@brief   解码暂停界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_pause(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_pause(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN-(16*5))/2,0);
    LCD_show_string((u8 *)"PAUSE", 1);
}
/*----------------------------------------------------------------------------*/
/**@brief   显示文件名（可能会拼接有ID3V2信息）
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_filename(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_filename(void) AT(LCD_CODE)
{
    if (filename_len <= (LCD_END_LUMN / 8))
        filenameCnt = 0;

    if (filenameCnt == 0)
    {
        LCD_set_XY(0,2);
        LCD_show_string(ff_api.dir_info.fname,0);
    }
    else
    {
        my_memcpy( &LCDBuff[2][0],&LCDBuff[2][1] ,LCD_END_LUMN-1);
        my_memcpy( &LCDBuff[3][0],&LCDBuff[3][1] ,LCD_END_LUMN-1);
        LCDBuff[2][LCD_END_LUMN - 1] = 0;
        LCDBuff[3][LCD_END_LUMN - 1] = 0;
        LCD_set_XY(LCD_END_LUMN - 8 - (u8)filenameCnt % 8,2);
        LCD_show_string(ff_api.dir_info.fname + (filenameCnt / 8) + (LCD_END_LUMN-8) / 8,0);

    }

    filenameCnt++;
    if (filenameCnt >= (filename_len*8))
    {
        filenameCnt = 0;
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   Music模式 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_music_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_music_main(void) AT(LCD_CODE)
{
    LCD_set_XY(DEVICE_COLUMN,0);
    if (device_active == DEVICE_SDMMCA)
    {
        LCD_show_bmp_via_id(LCD_BMP_SDMMC0);
    }
    else
        LCD_show_bmp_via_id(LCD_BMP_UDISK);
    
    LCD_show_music_time();
    LCD_show_small_filenumber();
    LCD_set_XY(EQ_COLUMN,0);
    LCD_show_bmp_via_id(LCD_BMP_EQ_NOR + Music_Play_var.bEQ);
    filenameCnt = 0;
    LCD_show_filename();
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 文件号显示
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_filenumber(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_filenumber(u16 filenum) AT(LCD_CODE)
{
    LCD_var.bFontSize = 1;  
    LCD_set_XY((LCD_END_LUMN - (4 * 16)) / 2,0); 
    LCD_show_char(filenum/1000 + '0');
    LCD_show_char(filenum%1000/100 + '0');
    LCD_show_char(filenum%100/10 + '0');
    LCD_show_char(filenum%10 + '0');
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 音量显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_volume(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_volume(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 1;  
    LCD_set_XY((LCD_END_LUMN - (8*8)) / 2,0);
    LCD_show_bmp_via_id(LCD_BMP_VOL);
    LCD_show_char(main_vol_L/10 + '0');
    LCD_show_char(main_vol_L%10 + '0');
}

/*----------------------------------------------------------------------------*/
/**@brief   Music模式 循环模式显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_playmode(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_playmode(void) AT(LCD_CODE)
{  
    switch (playfile.play_mode)
    {
    case REPEAT_ALL:
      LCD_set_XY((LCD_END_LUMN-3*8)/2, 1);
      LCD_show_string((u8 *)"ALL", 0);
      break;
#ifdef  FOLDER_PLAY_EN      
    case REPEAT_FOLDER:
      LCD_set_XY((LCD_END_LUMN-6*8)/2, 1);
      LCD_show_string((u8 *)"FOLDER", 0);
      break;
#endif      
    case REPEAT_ONE:
      LCD_set_XY((LCD_END_LUMN-3*8)/2, 1);
      LCD_show_string((u8 *)"ONE", 0);
      break;
#ifdef  RANDOM_PLAY_EN       
    case REPEAT_RANDOM:
      LCD_set_XY((LCD_END_LUMN-6*8)/2, 1);
      LCD_show_string((u8 *)"RANDOM", 0);
      break;
#endif      
    default:
      break;
    }
    
    
}
/*----------------------------------------------------------------------------*/
/**@brief   红外输入文件号显示函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LED5X7_show_IR_number(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_IR_number(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 1;  
    LCD_set_XY((LCD_END_LUMN - (4 * 16)) / 2,0); 
    LCD_show_char(input_number/1000 + '0');
    LCD_show_char(input_number%1000/100 + '0');
    LCD_show_char(input_number%100/10 + '0');
    LCD_show_char(input_number%10 + '0');
}

/*----------------------------------------------------------------------------*/
/**@brief   FM 显示主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_fm_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_fm_main(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 0; 
    LCD_set_XY(FM_CHANNL_COLUMN,0);
    
    /*FM - Current Channel info*/
    LCD_show_char(fm_mode_var.bFreChannel/10 + '0');
    LCD_show_char(fm_mode_var.bFreChannel%10 + '0');
    
    LCD_show_char(' ');
    
    /*FM - Total Channel info*/
    LCD_show_char(fm_mode_var.bTotalChannel/10 + '0');
    LCD_show_char(fm_mode_var.bTotalChannel%10 + '0');
    
    
    if (fm_mode_var.wFreq > 999)
    {
        LCD_set_XY(FRE_4_COLUMN,2);
        LCD_show_char(fm_mode_var.wFreq/1000 + '0');
    }
    else
    {
        LCD_set_XY(FRE_3_COLUMN,2);
    }

    LCD_show_char(fm_mode_var.wFreq%1000/100 + '0');
    LCD_show_char(fm_mode_var.wFreq%100/10 + '0');
    
    LCD_show_char('.');
    LCD_show_char(fm_mode_var.wFreq%10 + '0');
    LCD_show_string((u8 *)"MHz", 0);
}
/*----------------------------------------------------------------------------*/
/**@brief   AUX模式 主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_aux_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_aux_main(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*3)) / 2,0);
    LCD_show_string((u8 *)"AUX", 1);
}

/*----------------------------------------------------------------------------*/
/**@brief   PC模式 主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_aux_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_pc_main(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*2)) / 2,0);
    LCD_show_string((u8 *)"PC", 1);   
}

/*----------------------------------------------------------------------------*/
/**@brief   PC模式 主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_aux_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_pc_vol_up(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*2)) / 2,0);
    LCD_show_string((u8 *)"UP", 1);   
}

/*----------------------------------------------------------------------------*/
/**@brief   PC模式 主界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_aux_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_pc_vol_down(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*2)) / 2,0);
    LCD_show_string((u8 *)"DN", 1);   
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 界面选中-显示颠倒
   @param   x：X坐标 
   @param   y：Y坐标
   @param   x_size：X坐标区域大小 
   @param   y_size：Y坐标区域大小
   @return  void
   @author  Change.tsai 
   @note    void LCD_show_reverse_area(u8 x, u8 y, u8 x_size, u8 y_size)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_reverse_area(u8 x, u8 y, u8 x_size, u8 y_size) AT(LCD_CODE)
{
    u8 i, j;
    
    for (j = 0; j < y_size; j++)
    {
        for (i = 0; i < x_size; i++) 
        {
            LCDBuff[y + j][x + i]= ~LCDBuff[y + j][x + i];    
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   LCD RTC设置模式
   @param   sel：0-Year/1-Month/2-Day/3-Hour/4-Min/5-Sec
   @return  void
   @author  Change.tsai 
   @note    void RTC_UI_select(u8 sel)
*/
/*----------------------------------------------------------------------------*/
_near_func void RTC_UI_select(u8 sel) AT(LCD_CODE)
{   
    switch(sel)
    {
    case 0:
      LCD_show_reverse_area(RTC_YEAR, 0, 8*4, 2);
      break;
    case 1:
      LCD_show_reverse_area(RTC_MONTH, 0, 8*2, 2);
      break;
    case 2:
      LCD_show_reverse_area(RTC_DAY, 0, 8*2, 2);
      break;
    case 3:
      LCD_show_reverse_area(RTC_HOUR, 2, 8*2, 2);
      break;
    case 4:
      LCD_show_reverse_area(RTC_MIN, 2, 8*2, 2);
      break;
    case 5:
      LCD_show_reverse_area(RTC_SEC, 2, 8*2, 2);
      break;
    default:
      break;
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   RTC 显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_RTC_main(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_RTC_main(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 0; 

    /*RTC info - year*/
    LCD_set_XY(RTC_YEAR, 0);
    LCD_show_char(curr_time.dYear/1000 + '0');
    LCD_show_char(curr_time.dYear%1000/100 + '0');
    LCD_show_char(curr_time.dYear%100/10 + '0');
    LCD_show_char(curr_time.dYear%10 + '0');
      
    LCD_show_char('/');

    /*RTC info - month*/
    LCD_show_char(curr_time.bMonth/10 + '0');
    LCD_show_char(curr_time.bMonth%10 + '0');
    LCD_show_char('/');

    /*RTC info - day*/
    LCD_show_char(curr_time.bDay/10 + '0');
    LCD_show_char(curr_time.bDay%10 + '0');

    /*RTC info - hour*/
    LCD_set_XY(RTC_HOUR, 2);
    LCD_show_char(curr_time.bHour/10 + '0');
    LCD_show_char(curr_time.bHour%10 + '0');
    LCD_show_char(':');

    /*RTC info - minutes*/
    LCD_show_char(curr_time.bMin/10 + '0');
    LCD_show_char(curr_time.bMin%10 + '0');
    LCD_show_char(':');

    /*RTC info - second*/
    LCD_show_char(curr_time.bSec/10 + '0');
    LCD_show_char(curr_time.bSec%10 + '0');
    LCD_show_char('*');

    /*RTC info - Weekday*/
    LCD_show_char(curr_time.bWeekday%10 + '0'); 

    if (UI_var.bCurMenu == MENU_RTC_SET)
        RTC_UI_select(RTC_setting_var.bCoordinate);
}



/*----------------------------------------------------------------------------*/
/**@brief   LCD 界面选中-显示方框
   @param   x：X坐标 
   @param   y：Y坐标 
   @param   x_size：X坐标区域大小 
   @param   y_size：Y坐标区域大小
   @return  void
   @author  Change.tsai
   @note    void LCD_show_window(u8 x, u8 y, u8 x_size, u8 y_size)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_window(u8 x, u8 y, u8 x_size, u8 y_size) AT(LCD_CODE)
{
    u8 i, j;
    
    for (j = 0; j < y_size; j++)
    {
        for (i = 0; i < x_size; i++) 
        {
            if ((i == 0)||(i == (x_size - 1)))
            {
                LCDBuff[y + j][x + i] |= 0xFF;   
            }
            /**/
            else if (j == 0)
            {
                LCDBuff[y + j][x + i] |= 0x01;    
            }
            else if (j == (y_size - 1))
            {
                LCDBuff[y + j][x + i] |= 0x80;    
            }
            
        }
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   LCD 闹钟设置模式
   @param   sel：0-Year/1-Month/2-Day/3-Hour/4-Min/5-Sec
   @return  void
   @author  Change.tsai
   @note    void Alarm_UI_select(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void Alarm_UI_select(u8 sel) AT(LCD_CODE)
{   
    switch(sel)
    {
    case 0:
      LCD_show_reverse_area(ALM_HOUR, 0, 8*2, 2);
      break;
    case 1:
      LCD_show_reverse_area(ALM_MIN, 0, 8*2, 2);
      break;
    case 2:
      LCD_show_reverse_area(ALM_SWITCH, 0, 8*3, 2);
      break;
      
    case 3:
      LCD_show_reverse_area(ALM_MONDAY, 2, 10, 2);
      break;
    case 4:
      LCD_show_reverse_area(ALM_TUESDAY, 2, 10, 2);
      break;
    case 5:
      LCD_show_reverse_area(ALM_WEDNESDAY, 2, 10, 2);
      break;
    case 6:
      LCD_show_reverse_area(ALM_THRUSDAY, 2, 10, 2);
      break;
    case 7:
      LCD_show_reverse_area(ALM_FRIDAY, 2, 10, 2);
      break;
    case 8:
      LCD_show_reverse_area(ALM_SATURDAY, 2, 10, 2);
      break;
    case 9:
      LCD_show_reverse_area(ALM_SUNDAY, 2, 10, 2);
      break;
    default:
      break;
    }
}
/*----------------------------------------------------------------------------*/
/**@brief   LCD 周闹钟显示界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_week_alarm(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_week_alarm(void) AT(LCD_CODE)
{
    LCD_var.bFontSize = 0; 
    /*Alarm info - hour*/
    LCD_set_XY(ALM_HOUR, 0);
    LCD_show_char(curr_alarm.bHour/10 + '0');
    LCD_show_char(curr_alarm.bHour%10 + '0');
    LCD_show_char(':');

    /*Alarm info - minutes*/
    LCD_show_char(curr_alarm.bMin/10 + '0');
    LCD_show_char(curr_alarm.bMin%10 + '0');
    
    /*Alarm info - Switch On/Off*/
    if (curr_alarm.bSw)
    {
        LCD_set_XY(ALM_SWITCH, 0);
        LCD_show_string((u8 *)"ON", 0);   
    }
    else
    {
        LCD_set_XY(ALM_SWITCH, 0);
        LCD_show_string((u8 *)"OFF", 0);    
    }
    /*Alarm info - Weekday*/  
    LCD_set_XY(ALM_MONDAY, 2);
    LCD_show_char('1');
    if (curr_alarm.bmWeekdays & MONDAY)
        LCD_show_window(ALM_MONDAY, 2, 10, 2); 
      
    LCD_set_XY(ALM_TUESDAY, 2);
    LCD_show_char('2');
    if (curr_alarm.bmWeekdays & TUESDAY)
        LCD_show_window(ALM_TUESDAY, 2, 10, 2);
    
    
    LCD_set_XY(ALM_WEDNESDAY, 2);
    LCD_show_char('3');
    if (curr_alarm.bmWeekdays & WEDNESDAY)
        LCD_show_window(ALM_WEDNESDAY, 2, 10, 2);
    
      
    LCD_set_XY(ALM_THRUSDAY, 2);
    LCD_show_char('4');
    if (curr_alarm.bmWeekdays & THURSDAY)
        LCD_show_window(ALM_THRUSDAY, 2, 10, 2);
          
    
    LCD_set_XY(ALM_FRIDAY, 2);
    LCD_show_char('5');
    if (curr_alarm.bmWeekdays & FRIDAY)
        LCD_show_window(ALM_FRIDAY, 2, 10, 2);
    
    
    LCD_set_XY(ALM_SATURDAY, 2);
    LCD_show_char('6');
    if (curr_alarm.bmWeekdays & SATURDAY)
        LCD_show_window(ALM_SATURDAY, 2, 10, 2); 
      
    LCD_set_XY(ALM_SUNDAY, 2);
    LCD_show_char('7');
    if (curr_alarm.bmWeekdays & SUNDAY)
        LCD_show_window(ALM_SUNDAY, 2, 10, 2);
   
    Alarm_UI_select(RTC_setting_var.bCoordinate);
}

/*----------------------------------------------------------------------------*/
/**@brief   LCD 闹钟响闹界面
   @param   void
   @return  void
   @author  Change.tsai
   @note    void LCD_show_alarm_up(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void LCD_show_alarm_up(void) AT(LCD_CODE)
{
    LCD_set_XY((LCD_END_LUMN - (16*5)) / 2,0);
    LCD_show_string((u8 *)"ALARM", 1);    
}


#endif
