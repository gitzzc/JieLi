/*--------------------------------------------------------------------------*/
/**@file    music_play.c
   @brief   音乐模式主循环
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "music_play.h"
#include "play_file.h"
#include "key.h"
#include "device.h"
#include "hot_msg.h"
#include "get_music_file.h"
#include "main.h"
#include "iic.h"
#include "sdmmc_api.h"
#include "usb_host.h"
#include "break_point.h"
#include "UI_API.h"
#include "dac.h"
#include "clock.h"
#include "virtual_key.h"

#include "bike.h"

//必须在公共区
__root const u8 EQTableCode[5][10] =
{
    {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},    //NORMAL
    {0xff,0xb9,0x87,0x65,0x54,0x56,0x78,0x9b,0xdf,0xcb},    //POP
    {0xa8,0xbc,0xce,0xff,0x46,0xb9,0x89,0xcc,0xff,0xbd},    //ROCK
    {0x80,0xc0,0xf0,0x88,0xff,0xba,0xaa,0xcc,0xdd,0xab},    //JAZZ
    {0xaf,0xff,0xa8,0x76,0x86,0xf6,0xa6,0x88,0xcc,0x9b},    //CLASSIC
};///<DECODER音效表


_no_init MUSIC_PLAY_VAR _idata Music_Play_var;
_no_init DEVICE_INFO _idata music_device_info[MAX_DEVICE + 1];

/*----------------------------------------------------------------------------*/
/** @brief: Music 模式信息初始化
    @param: void
    @return:void
    @author:Juntham
    @note:  void music_app_init(void)
*/
/*----------------------------------------------------------------------------*/
static void music_info_init(void) AT(MUSIC_PLAY)
{
    /*------------System configuration-----------*/
    Sys_IRInput = 1;
    Sys_Volume = 1;

    device_active = NO_DEVICE;

    if (playfile.given_device == NO_DEVICE)
    {
        playfile.given_device = (ENUM_DEVICE_SELECT_MODE)get_memory(MEM_ACTIVE_DEV);
        playfile.given_device = (playfile.given_device > MAX_DEVICE) ? DEVICE_UDISK : playfile.given_device;
    }

    if (playfile.given_device == DEVICE_UDISK)
    {
        playfile.given_file_method = PLAY_BREAK_POINT;
        playfile.given_device = DEVICE_UDISK;
        put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
    }
#ifdef SDMMCA_EN
    else if (playfile.given_device == DEVICE_SDMMCA)
    {
        playfile.given_file_method = PLAY_BREAK_POINT;
        playfile.given_device = DEVICE_SDMMCA;
        put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
    }
#endif
#ifdef SDMMCB_EN
    else if (playfile.given_device == DEVICE_SDMMCB)
    {
        playfile.given_file_method = PLAY_BREAK_POINT;
        playfile.given_device = DEVICE_SDMMCB;
        put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
    }
#endif
#ifdef SDMMCA_EN
    else
    {
        playfile.given_device = DEVICE_SDMMCA;
        playfile.given_file_method = PLAY_FIRST_FILE;
        put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
    }
#endif

    /*--------Music UI*/
    SET_UI_MAIN(MENU_MUSIC_MAIN);
    UI_menu(MENU_POWER_UP);
    UI_menu(MENU_MUTE);
    key_table_sel(0);
}

/*----------------------------------------------------------------------------*/
/** @brief: 文件播放函数
    @param: file_break_point：断点记忆控制位
    @return:播放成功标志位
    @author:Juntham
    @note:  bool play_device_file(bool file_break_point)
*/
/*----------------------------------------------------------------------------*/
bool play_device_file(bool file_break_point) AT(MUSIC_PLAY)
{
    bool bres;

    init_decode_api(f_read, f_seek);
    f_open(&music_file, &ff_api.dir_info);
    bres = decode_init(file_break_point);

    /*Main UI recovery*/
    SET_UI_MAIN(MENU_MUSIC_MAIN);
    if (bres)                                       //播放成功
    {
        set_eq(1, Music_Play_var.bEQ);
#ifdef MP3_SPECTRUM
        MP_CON3 |= BIT(2);
#endif
        start_decode();
        return true;
    }

    return false;
}
/*----------------------------------------------------------------------------*/
/** @brief: EQ 音效设置函数
    @param: eq_en：EQ使能控制位
    @param: eq：EQ序号
    @return:void
    @author:Juntham
    @note:  void set_eq(bool eq_en, u8 eq)
*/
/*----------------------------------------------------------------------------*/
void set_eq(bool eq_en, u8 eq) AT(MUSIC_PLAY)
{
    if (eq_en)
    {
        dec_msg.eq_table = EQTableCode[eq];
        MP_CON3 |= BIT(1);
    }
    else
        MP_CON3 &= ~BIT(1);
}

#ifdef MUSIC_POWER_EN
u8 get_bit_index(u32 data) AT(MUSIC_PLAY)
{
    u8 index;
    u32 bit;

    data >>= 6;
    bit = 0x80000000;
    for (index = 32; index < 255; index--)
    {
        //my_printf("bit : %d/%08lx", index, bit);
        if ((data & bit) == bit)
          break;
        bit>>=1;
    }
    if ((index == 255)||(index == 0))
    {
        index = 1;
    }
    else if (index > 16)
    {
        index = 16;
    }
    return index;
}
#endif


void OFF_mode(void) AT(MUSIC_PLAY)
{
    while (1)
    {
        u8 key;

        key = app_get_msg();

        switch (key)
        {
        case MSG_STOP:
            deg("MSG_STOP\n");
            work_mode = get_memory(MEM_MEDIAMODE);
          return;
        case MSG_100MS:
            UI_menu(MENU_STOP);
            bike_task();
            break;
        default :
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/** @brief: Music 模式主循环
    @param: void
    @return:void
    @author:Juntham
    @note:  void music_app_loop(void)
*/
/*----------------------------------------------------------------------------*/
void music_app_loop(void) AT(MUSIC_PLAY)
{
    u8 res;
    bool bres;
    static u8 half_sec=0;

    UI_menu(MENU_MAIN);

    while (1)
    {
        u8 key;

        sdmmca_idle_mode();     //SDMMC进入IDLE待机模式。
        sdmmcb_idle_mode();

        key = app_get_msg();

        switch (key)
        {
        case MSG_CHANGE_WORK_MODE:
            //deg("MP3 MSG_CHANGE_WORK_MODE\n");
            work_mode = FM_RADIO_MODE;
#if defined BREAK_POINT_PLAY_EN && !defined USE_IRTC_MEMORY
            save_music_break_point(device_active);
#endif
            return;

        case MSG_MUSIC_NEW_DEVICE_IN:							//有新设备接入
#if defined BREAK_POINT_PLAY_EN && !defined USE_IRTC_MEMORY
            save_music_break_point(device_active);
#endif
        case MSG_MUSIC_SELECT_NEW_DEVICE:					    //重新选择设备
            //deg("0-MSG_MUSIC_SELECT_NEW_DEVICE %02x\n", playfile.given_device);
            UI_menu(MENU_WAIT);
            stop_decode();
            res = find_device(playfile.given_device);
            if ((res == DEV_INIT_ERR) ||
                    (res == NO_DEFINE_DEV))                    //指定的设备不在线，或初始化失败
            {
                playfile.given_device = DEVICE_AUTO_NEXT;			   //自动选择下一个设备
                put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
                break;
            }
            else if ((res == NO_EFFECTIVE_DEV) ||
                     (res == NO_DEV_ONLINE))                    //无可播放的设备
            {
                put_msg_lifo(MSG_NEXT_WORKMODE);
                break;
            }
            //找到可用设备
        case MSG_MUSIC_SELECT_NEW_FILE:
            if (device_check() == 0)                                //当前播放设备已经不在线,则自动选取下一个设备
            {
                playfile.given_device = DEVICE_AUTO_NEXT;
                put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
                break;
            }

            res = fs_get_filenum(playfile.play_mode, playfile.given_file_method);

            //deg("1-MSG_MUSIC_SELECT_NEW_FILE : %02x\n", res);
            if (res == GET_DEVICE_END)								//下一曲到设备的结束，查找下一个设备的第一首
            {
                playfile.given_file_method = PLAY_FIRST_FILE;
                playfile.given_device = DEVICE_AUTO_NEXT;
                put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
                //music_device_info[device_active].wBreak_point_filenum = 1;
                clear_music_break_point(device_active);
                break;
            }
            else if (res == GET_DEVICE_HEAD)								//上一曲到设备的结束，查找下一个设备的最后一首
            {
                playfile.given_file_method = PLAY_LAST_FILE;
                playfile.given_device = DEVICE_AUTO_PREV;
                put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
                break;
            }
#if defined BREAK_POINT_PLAY_EN && !defined USE_IRTC_MEMORY
        case MSG_MUSIC_PLAY_NEW_FILE0:
                save_music_break_point(device_active);
#endif
        case MSG_MUSIC_PLAY_NEW_FILE:
            bres = get_file_search_effectDir();

            //my_printf("---Dir3 number:%u/%u\n", ff_api.fs_msg.dirCounter ,ff_api.fs_msg.musicDirCounter);
            //my_printf("----file number:%u\n", playfile.given_file_number);
            //my_printf("------file name:%s\n", ff_api.dir_info.fname);
            if (!bres)
            {
                if (playfile.given_file_method <= PLAY_BREAK_POINT) 		//重新找下一首
                {
                    playfile.given_file_method = PLAY_NEXT_FILE;
                    put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
                }
                else											//重新找上一首
                {
                    playfile.given_file_method = PLAY_PREV_FILE;
                    put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
                }
                break;
            }

            flush_all_msg();
            UI_menu(MENU_FILENUM);
            if (playfile.given_file_method == PLAY_BREAK_POINT)
            {
                bres = play_device_file(1);
            }
            else
            {
                bres = play_device_file(0);
            }

            if (!bres)
            {
                if (playfile.given_file_method <= PLAY_BREAK_POINT) 		//重新找下一首
                {
                    playfile.given_file_method = PLAY_NEXT_FILE;
                    put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
                }
                else											            //重新找上一首
                {
                    playfile.given_file_method = PLAY_PREV_FILE;
                    put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
                }
                break;
            }
            else
            {
                set_memory(MEM_ACTIVE_DEV, device_active);
                //deg("0-Get Device %02x\n", get_memory(MEM_ACTIVE_DEV));
                music_device_info[device_active].wBreak_point_filenum = playfile.given_file_number;
#ifdef LAST_MEM_FILE_PLAY_EN
#ifdef USE_EEPROM_MEMORY
                save_music_break_point(device_active);
#endif
#ifdef USE_IRTC_MEMORY
                save_music_break_point_rtc_ram(device_active);
#endif
#endif
            }
            break;

        case MSG_MUSIC_CONTINUE_PLAY_FILE:                      //继续播放文件
            play_device_file(1);
            break;

        case MSG_DECODE_FILE_END:                               //解码自动至下一首
            playfile.given_file_method = PLAY_AUTO_NEXT;
            put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
            break;

        case MSG_MUSIC_PREV_FILE:												//选择上一个文件进行播放;
	#ifdef USB_VIR_KEY_EN
          if(send_vir_key(VIR_PREV))
           {
                break;
           }
	#endif
            playfile.given_file_method = PLAY_PREV_FILE;
            put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
            break;

        case MSG_MUSIC_NEXT_FILE:												//选择下一个文件进行播放
	#ifdef USB_VIR_KEY_EN
            if(send_vir_key(VIR_NEXT))
            {
               break;
            }
	#endif
            playfile.given_file_method = PLAY_NEXT_FILE;
            put_msg_lifo(MSG_MUSIC_SELECT_NEW_FILE);
            break;

        case MSG_MUSIC_PP:	                                                    //暂停和播放
	#ifdef USB_VIR_KEY_EN
             if(send_vir_key(VIR_PLAY))
            {
               break;
            }
	#endif
#ifdef UI_ENABLE										
            if (UI_var.bCurMenu == MENU_INPUT_NUMBER)
            {
                put_msg_lifo(MSG_INPUT_TIMEOUT);
                break;
            }
#endif
            if (Music_Play_var.bPlayStatus == MAD_PLAY)
            {
                pause_decode(1);
                USB_read_remain();
                SET_UI_MAIN(MENU_PAUSE);
                UI_menu(MENU_PAUSE);
            }
            else if (Music_Play_var.bPlayStatus == MAD_PAUSE)
            {
                music_fffr(0xFF, 0);
                start_decode();
                SET_UI_MAIN(MENU_MUSIC_MAIN);
                UI_menu(MENU_MUSIC_MAIN);
            }
            break;

        case MSG_MUSIC_FF:
            music_fffr(MAD_FAST_FORWARD, get_1s_musice()*2);
            UI_menu(MENU_HALF_SEC_REFRESH);
            break;

        case MSG_MUSIC_FR:
            music_fffr(MAD_FAST_REVERASE, get_1s_musice()*2);
            UI_menu(MENU_HALF_SEC_REFRESH);
            break;

        case MSG_MUSIC_NEXT_EQ:
            if (Music_Play_var.bPlayStatus == MAD_PAUSE)
                break;

            Music_Play_var.bEQ++;
            if (Music_Play_var.bEQ > CLASSIC)
            {
                Music_Play_var.bEQ = NORMAL;
            }
            set_eq(1, Music_Play_var.bEQ);
            //deg("play_EQ : %d\n", Music_Play_var.bEQ);
            UI_menu(MENU_EQ);
            break;

        case MSG_NEXT_PLAYMODE:
            playfile.play_mode++;
            //deg("play_mode : %02x\n", playfile.play_mode);
            if (playfile.play_mode >= MAX_PLAY_MODE)
            {
                playfile.play_mode = REPEAT_ALL;
            }
            UI_menu(MENU_PLAYMODE);
            break;

#ifdef MUSIC_POWER_EN
        case MSG_DECODER_POWER:
            music_energy = count_energy((u8 *)(0x200), 50);
            break;
#endif
        case MSG_100MS:
            bike_task();
            break;
        case MSG_HALF_SECOND:
            //usb_timebase_detect();
            LED_FADE_OFF();
            UI_menu(MENU_MAIN);
#ifdef BREAK_POINT_PLAY_EN
#ifndef USE_EEPROM_MEMORY
            if (Music_Play_var.bPlayStatus == MAD_PLAY)                            //每半秒将断点信息写入到内部 RTC RAM 中，
                save_music_break_point_rtc_ram(device_active);
#endif
#endif
            if (Music_Play_var.bPlayStatus != MAD_PLAY)
                music_energy = 0;
            if ( half_sec ++ == 20 ){
                half_sec  = 0;
                save_music_break_point(device_active);
            }
            UI_menu(MENU_HALF_SEC_REFRESH);
            break;

        case MSG_INPUT_TIMEOUT:
            /*由红外界面返回*/
            if ((input_number <= music_device_info[device_active].wfileTotal)&&(input_number != 0))
            {
                playfile.given_file_number = input_number;
                playfile.given_file_method = PLAY_FIRST_FILE;
                put_msg_lifo(MSG_MUSIC_PLAY_NEW_FILE);
            }
            else
                UI_menu(MENU_MUSIC_MAIN);

            input_number = 0;
            break;
        case MSG_STOP:
            deg("MSG_STOP\n");
            dac_mute(1);
            set_memory(MEM_MEDIAMODE,work_mode);
            save_music_break_point(device_active);
            work_mode = OFF_MODE;
            UI_menu(MENU_STOP);
          return;
        default :
            ap_handle_hotkey(key);
            break;
        }
    }
}

/*----------------------------------------------------------------------------*/
/** @brief: Music 模式
    @param: void
    @return:void
    @author:Juntham
    @note:  void music_app(void)
*/
/*----------------------------------------------------------------------------*/
void music_app(void) AT(MUSIC_PLAY)
{
#ifdef SDMMCA_DET_CLK
#ifdef SDMMCA_EN
    P2DIR &=  ~0x01;
#endif
#ifdef SDMMCB_EN
    P3DIR &=  ~0x20;
#endif
    set_sd_clk = 1;
#endif

    system_clk_div(CLK_24M);
    dac_mute(0);
    music_info_init();
    dac_channel_sel(DAC_DECODER);
    playfile.play_mode = REPEAT_ALL;
    music_app_loop();
    stop_decode();
    udisk_force_idle();
    sdmmca_force_idle();
    sdmmcb_force_idle();
    playfile.given_device = NO_DEVICE;

#ifdef SDMMCA_DET_CLK
    IO_MC0 &= ~(BIT(1)|BIT(0));
    set_sd_clk = 0;
#endif
}
