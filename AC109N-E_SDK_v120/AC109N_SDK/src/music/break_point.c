/*--------------------------------------------------------------------------*/
/**@file    break_point.c
   @brief   断点播放功能接口
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "break_point.h"
#include "play_file.h"
#include "music_play.h"
#include "device.h"
#include "get_music_file.h"
#include "main.h"
#include "iic.h"
#include "IRTC.h"

const u8 break_point_table[MAX_DEVICE+1] AT(BANK1_TABLE_CODE)= 
{
#ifdef USB_DISK_EN
    MEM_USB_PLAYPOINT_ID1_0,
#endif
    
#ifdef SDMMCA_EN    
    MEM_SDA_PLAYPOINT_ID1_0,  
#endif     

#ifdef SDMMCB_EN    
    MEM_SDB_PLAYPOINT_ID1_0,
#endif    
};

_no_init BREAK_POINT_INFO _idata music_break_info;   ///<设备播放信息


/*----------------------------------------------------------------------------*/
/** @brief: 断点信息清除函数
    @param: dev：需要清除信息的设备
    @return:void
    @author:Juntham
    @note:  void clear_music_break_point(u8 dev) 
*/
/*----------------------------------------------------------------------------*/
void clear_music_break_point(u8 dev) AT(MUSIC_PLAY)
{
    u8 i, j;
    
    j = break_point_table[dev];

    for (i = 0; i < sizeof(BREAK_POINT_INFO); i++)
    {
        set_memory(i + j, 0);
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   记忆设备断点信息到存储器eerprm
   @param   dev：需要记忆信息的设备
   @return  void
   @author: Juntham
   @note    void save_music_break_point(u8 dev)
*/
/*----------------------------------------------------------------------------*/
void save_music_break_point(u8 dev) AT(MUSIC_PLAY)
{
    u8 i, j;

    if(Music_Play_var.bPlayStatus == MAD_STOP)
        return;

    music_break_info.wFsizeID = get_crc16(dec_msg.fp->fsize);
    music_break_info.wSclustID = get_crc16(dec_msg.fp->org_clust);
    music_break_info.dFrameCounter = dec_msg.frame_counter;
    music_break_info.dNextFrame = dec_msg.next_frame;
    
    j = break_point_table[dev];

    for (i = 0; i < sizeof(BREAK_POINT_INFO); i++)
    {
        set_memory(i + j, ((u8 _idata *)&music_break_info)[i]);       
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   记忆设备断点信息到存储器rtc ram
   @param   dev：需要记忆信息的设备
   @return  void
   @author: Juntham
   @note    void save_music_break_point(u8 dev)
*/
/*----------------------------------------------------------------------------*/
void save_music_break_point_rtc_ram(u8 dev) AT(MUSIC_PLAY)
{
    u8 i, j;

    if(Music_Play_var.bPlayStatus == MAD_STOP)
        return;

    music_break_info.wFsizeID = get_crc16(dec_msg.fp->fsize);
    music_break_info.wSclustID = get_crc16(dec_msg.fp->org_clust);
    music_break_info.dFrameCounter = dec_msg.frame_counter;
    music_break_info.dNextFrame = dec_msg.next_frame;
    
    j = break_point_table[dev];

    for (i = 0; i < sizeof(BREAK_POINT_INFO); i++)
    {
        write_IRTC_RAM(i + j, ((u8 _idata *)&music_break_info)[i]);      
    }
}

/*----------------------------------------------------------------------------*/
/**@brief   从存储器读取设备断点信息
   @param   dev：需要读取信息的设备
   @return  读取断点信息成功标志
   @author: Juntham
   @note    _bool load_music_break_point(u8 dev)
*/
/*----------------------------------------------------------------------------*/
_bool load_music_break_point(u8 dev) AT(MUSIC_PLAY)
{
    u8 i,j;
    
    j = break_point_table[dev];

    for (i = 0; i < sizeof(BREAK_POINT_INFO); i++)
    {
        ((u8 _idata *)&music_break_info)[i] = get_memory(i + j);
    }
    
    //判断断点是否有效
    if ((music_break_info.wFsizeID != get_crc16(dec_msg.fp->fsize)) || 
        (music_break_info.wSclustID != get_crc16(dec_msg.fp->org_clust)))
    {
        return false;
    }
    else
    {
        return true;
    }
}
