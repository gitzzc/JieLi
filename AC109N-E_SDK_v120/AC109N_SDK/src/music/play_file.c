/*--------------------------------------------------------------------------*/
/**@file    play_file.c
   @brief   解码功能接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "play_file.h"
#include "msg.h"
#include "break_point.h"
#include "music_play.h"
#include "device.h"
#include "main.h"
#include "dac.h"
#include "usb_host.h"
#include "get_music_file.h"


_no_init u8 _xdata decode_buf[1968] AT(DEC_BUF);
_no_init DECODER_MSG _pdata dec_msg AT(DECODE_MSG_V);
_no_init _no_init FATFS _pdata fat_fs AT(FATFS_V);
_no_init FIL music_file;
_no_init u8 _data music_format;
_no_init bool _bit jl_flag;
_no_init volatile bool _bit dec_exit;
_no_init volatile bool _bit voice_playing;
_no_init u32 _xdata music_energy;

/*----------------------------------------------------------------------------*/
/** @brief: 用户控制退出解码函数
    @param: void
    @return:void
    @author:Juntham
    @note:  void user_exit_decoder(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void user_exit_decoder(void) AT(COMMON_CODE)
{
    if (Music_Play_var.bPlayStatus == MAD_PLAY)
    {
        dec_exit = 1;
        music_file.flag = FR_USER_END;
    }
}

/*----------------------------------------------------------------------------*/
/** @brief: Obuf 中断服务程序
    @param: void
    @return:void
    @author:Juntham
    @note:  void obuf_isr(void)
*/
/*----------------------------------------------------------------------------*/
#pragma vector=VECTOR(OBUF_MP3_INT)
_near_func
__interrupt void obuf_isr(void)
{
#ifdef MUSIC_POWER_EN
    static _no_init u8 pkt_cnt;

    pkt_cnt++;

    if ((pkt_cnt % 5) == 0)
    {
        pkt_cnt = 0;
        put_msg_fifo(MSG_DECODER_POWER);
    }
#endif
    if (music_format == WAV_FORMAT)
    {
        SOFT_IE(1);
    }
    else if (MP_CON3 & BIT(7))
    {
        MP_CON3 |= BIT(6);                  // clear MP3 decode pending
        SOFT_IE(1);
    }

    OBUF_CON &= ~BIT(1);                //
}



/*----------------------------------------------------------------------------*/
/** @brief: 解码 中断服务程序
    @param: void
    @return:void
    @author:Juntham
    @note:  void soft_isr(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void soft_isr(void)
{
    u8 res;

    SOFT_IE(0);

    res = decode_loop();

    if (res != 0)
    {
        //deg("Soft error %02x\n", res);

        if (res == FR_DISK_ERROR)
        {
            playfile.given_device = DEVICE_AUTO_NEXT;
            put_msg_lifo(MSG_MUSIC_SELECT_NEW_DEVICE);
        }
        else if (res == MAD_ERROR_FF_FR_FILE_START)           //快退到头
        {
        //    dec_msg.frame_counter = 0;//put_msg_fifo(MSG_MUSIC_PLAY_NEW_FILE0);
        //    f_seek(dec_msg.fp,0x01,0);
        }
        //else if (res == FR_USER_END)
        //{
        //    put_msg_fifo(MSG_DECODE_USER_END);
        //}
        else
        {
            put_msg_fifo(MSG_DECODE_FILE_END);
        }
    }
    else
    {
        if (music_format == WAV_FORMAT)
            OBUF_CON |= BIT(1);            //允许OBUF产生pending
        else
        {
            sideinfo_lib();
        }
    }
}


/*----------------------------------------------------------------------------*/
/** @brief: 暂停解码
    @param: void
    @return:void
    @author:Juntham
    @note:  u8 pause_decode(void)
*/
/*----------------------------------------------------------------------------*/
__monitor
u8 pause_decode(u8 type) AT(MUSIC_PLAY)
{
    u8 res;

    res = Music_Play_var.bPlayStatus;

    if (Music_Play_var.bPlayStatus != MAD_PLAY)
        return res;

    OBUF_IE(0);                     //禁止解码和OBUF中断
    SOFT_IE(0);                     //禁止软件中断
    OBUF_CON &= ~BIT(1);            //禁止OBUF产生pending

    if(1 == type)
    {
        Music_Play_var.bPlayStatus = MAD_PAUSE;
    }
    else if(0 == type)
    {
        Music_Play_var.bPlayStatus = MAD_PLAY_IDLE;
    }

    return res;
}

/*----------------------------------------------------------------------------*/
/** @brief: 停止解码
    @param: void
    @return:void
    @author:Juntham
    @note:  void stop_decode(void)
*/
/*----------------------------------------------------------------------------*/
void stop_decode(void) AT(MUSIC_PLAY)
{
    pause_decode(1);

    while (get_obuf_len() != 0)         //等待DAC数据播放结束
    {
        if (!(DAC_CON0 & BIT(4)))
            break;
    }

    MP_CON3 &= ~(BIT(3) | BIT(0));   //disable MP3 decoder
    MP_CON3 |= BIT(6);               //清除MP3解码pending
    USB_read_remain();
    Music_Play_var.bPlayStatus = MAD_STOP;
}

/*----------------------------------------------------------------------------*/
/** @brief: 开始解码
    @param: void
    @return:void
    @author:Juntham
    @note:  void start_decode(void)
*/
/*----------------------------------------------------------------------------*/
__monitor
void start_decode(void) AT(MUSIC_PLAY)
{
    if (Music_Play_var.bPlayStatus == MAD_PLAY)
        return;

    if (music_format == WAV_FORMAT)
        OBUF_CON |= BIT(1);            //允许OBUF产生pending
    else if (!(MP_CON3 & BIT(0)))
    {
        SOFT_IE(1);
    }

    OBUF_IE(1);

    Music_Play_var.bPlayStatus = MAD_PLAY;
}

/*----------------------------------------------------------------------------*/
/** @brief: 设置解码文件接口
    @param: read：文件系统读接口
    @param: seek：文件系统查找接口
    @return:void
    @author:Juntham
    @note:  void init_decode_api(u16 (_near_func *read)(FIL _xdata *,u8 _xdata* , u16), FRESULT (_near_func *seek) (FIL _xdata *, u8, u32))
*/
/*----------------------------------------------------------------------------*/
void init_decode_api(u16 (_near_func *read)(FIL _xdata *,u8 _xdata* , u16), FRESULT (_near_func *seek) (FIL _xdata *, u8, u32)) AT(MUSIC_PLAY)
{
    my_memset((u8 _xdata*)&dec_msg, 0, sizeof(DECODER_MSG));
    my_memset((u8 *)&music_file, 0 , sizeof(FIL));
    dec_msg.read = read;
    dec_msg.seek = seek;
    dec_msg.fp = &music_file;
    dec_msg.buffer = decode_buf;
    jl_flag = 0;
}


/*----------------------------------------------------------------------------*/
/** @brief: 解码模块初始化函数
    @param: break_point：断点信息控制位
    @return:解码成功标志位
    @author:Juntham
    @note:  bool decode_init(bool break_point)
*/
/*----------------------------------------------------------------------------*/
bool decode_init(bool break_point) AT(MUSIC_PLAY)
{
    u8 res;

    obuf_setting(OBUF_2);

    res = wav_format_check(decode_buf);

    //deg("X-%02x\n", res);
    if (res == IS_WAV)
    {
        //deg_puts("find wav\n");
    }
#ifdef NDEBUG
    else if (NOT_SUPPORT != res)
    {
        //deg_puts("try mp3\n");

        if (dec_msg.format == 3)
            jl_flag = 1;

        dec_msg.tag_len += get_ff_id3v2_len(decode_buf);

#ifdef GET_MUSIC_TOTAL_TIME
        get_mp3_total_time();
        //deg("----Total Time-%u:%u\n", (u16)dec_msg.total_time/60, (u16)dec_msg.total_time%60);
#endif

        dec_msg.err_counter = 0;
    }
#else
    else
        return false;
#endif
    //else
    //    return false;

#ifdef BREAK_POINT_PLAY_EN
    if (jl_flag)
        break_point = 0;

    if (break_point)
    {
        break_point = load_music_break_point(device_active);
    }

    if (break_point)
    {
        if (!music_decode_init(music_break_info.dFrameCounter, music_break_info.dNextFrame))
          music_decode_init(0, dec_msg.tag_len);
    }
    else
    {
        music_decode_init(0, dec_msg.tag_len);
    }
#else
    {
        break_point = break_point;
        music_decode_init(0, dec_msg.tag_len);
    }
#endif
    music_format = dec_msg.format;
    int_enter_pro[SOFT_INT] = soft_isr;
    MP_CON3 &= ~(BIT(3) | BIT(0));
    return true;
}
