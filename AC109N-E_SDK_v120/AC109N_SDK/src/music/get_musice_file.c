/*--------------------------------------------------------------------------*/
/**@file    get_music_file.c
   @brief   文件查找接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "config.h"
#include "music_play.h"
#include "get_music_file.h"
#include "device.h"
#include "iic.h"
#include "play_file.h"

_no_init SELECT_PLAY_FILE _data playfile;
_no_init FF_APIS _xdata ff_api;
_no_init FF_APIS ff_api_tmp AT(FF_API_TEMP);                ///<在没有解码时使用
_no_init bool _bit find_break_point_dir_flag;

/*Path search example*/
const u8 Specific_path[] =
{
    "/1       /B???????MP3",
};

const u8 Specific_path1[] =
{
    "/1       /B*MP3",
};
/*----------------------------------------------------------------------------*/
/**@brief   获取一个随机数
   @param   无
   @return  获取到的随机数
   @author: Juntham
   @note    u16 get_u16_random()
*/
/*----------------------------------------------------------------------------*/
u16 get_u16_random(void) AT(GET_MUSIC_FILE)
{
    u16 *ptr;
    u16 res;

    ptr = (u16 *)0x200;
    res = *ptr++;
    res = *ptr + TMR2_CNTL;

    return res;
}

/*----------------------------------------------------------------------------*/
/** @brief: 获取文件夹内文件总数函数（仅在没有解码时使用）
    @param: void
    @return:void
    @author:Juntham
    @note:  void get_dir_file_total(void)
*/
/*----------------------------------------------------------------------------*/
//#ifdef FOLDER_PLAY_EN
static void get_dir_file_total(void) AT(GET_MUSIC_FILE)
{
    if (ff_api.fs_msg.fileTotalInDir == 0)
    {
        stop_decode();
        my_memcpy((u8 *)&ff_api_tmp, (u8 *)&ff_api, sizeof(FF_APIS));
        ff_api.fs_msg.fileTotalInDir = fs_getfile_totalindir(&ff_api_tmp);
    }
}
//#endif



u16 file_search(u16 fileNumber)
{
    u16 dirNum;

    get_dir_file_total();
    //my_printf("file :%u  file In/Out:%u/%u\n", ff_api.fs_msg.fileNumber,ff_api.fs_msg.fileTotalInDir ,ff_api.fs_msg.fileTotalOutDir);
    //my_printf("file :%u  %u\n",ff_api.fs_msg.fileNumber, fileNumber);
    if (fileNumber == 0)                        //查找指定的文件
    {
        //my_puts("spec file\n");
        dirNum = 0;
    }
    else if (fileNumber == ff_api.fs_msg.fileNumber)        //当前文件与打开的文件相同
    {
        dirNum = 0;
    }
    else if ((fileNumber > ff_api.fs_msg.fileNumber) && (ff_api.fs_msg.fileNumber != 0))    //新文件序号比当前文件大，可以接上次的搜索
    {
        //my_puts("con-scan\n");
        if (ff_api.fs_msg.fileNumber == ff_api.fs_msg.fileTotalOutDir + ff_api.fs_msg.fileTotalInDir)
            dirNum = 0;
        else
            dirNum = 1;
    }
    else                                        //需重新搜索
    {
        //my_puts("rescan\n");
        dirNum = 0;
    }
    return dirNum;
}

bool get_file_search_effectDir(void)
{
  bool bres;
  u16 dirNum;
  static u16 dirCounter,MuiscDir;

  stop_decode();
  //deg("\nbreak point : %04x-%04x\n",ff_api.fs_msg.sclust_id, ff_api.fs_msg.fsize_id);
  dirNum = file_search(playfile.given_file_number);
  bres = fs_getfile_bynumber(&ff_api, playfile.given_file_number);
  playfile.given_file_number = ff_api.fs_msg.fileNumber;

  if(find_break_point_dir_flag == 1)
  {
    find_break_point_dir_flag = 0;
    ff_api.fs_msg.fileNumber = 0;
    bres = fs_getfile_bynumber(&ff_api, playfile.given_file_number);
    playfile.given_file_number = ff_api.fs_msg.fileNumber;
  }

  //my_printf("--Dir0 number:%u/%u\n", ff_api.fs_msg.musicDirCounter ,ff_api.fs_msg.dirCounter);
  /*文件夹总数没有发生变化,保持有效文件夹不变*/
  if (ff_api.fs_msg.dirCounter == dirCounter)
  {
    //my_puts("1...");
    /*根目录有效*/
    if ((ff_api.fs_msg.dirCounter == 0x0)
    && (ff_api.fs_msg.musicDirCounter))
    {
      ff_api.fs_msg.musicDirCounter = 1;
      MuiscDir = 1;
    }
    else
      ff_api.fs_msg.musicDirCounter = MuiscDir;
  }
  else
  {
    //my_puts("2...");
    dirCounter = ff_api.fs_msg.dirCounter;
    if (ff_api.fs_msg.dirCounter)
    {
      //my_puts("3...");
      ff_api.fs_msg.musicDirCounter -= dirNum;
    }
    MuiscDir = ff_api.fs_msg.musicDirCounter;
  }
  //my_printf("--Dir1 number:%u/%u\n", dirCounter ,MuiscDir);
  return bres;
}

/*----------------------------------------------------------------------------*/
/**@brief   获取文件号
   @param   playmode   循环播放模式
   @param   searchMode  0：上一个文件；1：下一个文件；2：在强行REPEAT_ONE模式時在REPEAT_ALL模式下查找文件
   @return  0:fail 1:success
   @author:Juntham
   @note    u8 fs_get_filenum(ENUM_PLAY_MODE playmode, ENUM_FILE_SELECT_MODE searchMode)
*/
/*----------------------------------------------------------------------------*/
u8 fs_get_filenum(ENUM_PLAY_MODE playmode, ENUM_FILE_SELECT_MODE searchMode) AT(GET_MUSIC_FILE)
{
    u16 fileTotal = music_device_info[device_active].wfileTotal;

    if (searchMode == PLAY_FIRST_FILE)				//播放第一个文件
    {
        playfile.given_file_number = 1;
        return GET_FILE_OK;
    }
    else if (searchMode == PLAY_LAST_FILE)				//播放最后一个文件
    {
        playfile.given_file_number = fileTotal;
        return GET_FILE_OK;
    }
    else if (searchMode == PLAY_BREAK_POINT)				//播放记忆的文件序号和断点
    {
#ifdef LAST_MEM_FILE_PLAY_EN
        if (find_break_point_file_flag)                 /*记忆文件有效*/
        {
            find_break_point_dir_flag = 1;
            find_break_point_file_flag = 0;
            playfile.given_file_number = 0;
        }
        else
        {
            playfile.given_file_number = music_device_info[device_active].wBreak_point_filenum;
            if (playfile.given_file_number == 0)
                playfile.given_file_number = 1;
        }
#else
        playfile.given_file_number = 1;
#endif
        return GET_FILE_OK;
    }

    if ((playmode == REPEAT_ONE) && (searchMode != PLAY_AUTO_NEXT))
    {
        playmode = REPEAT_ALL;					//在单曲循环模式下，转换成全循环模式
    }
    switch (playmode)
    {
#ifdef RANDOM_PLAY_EN
    case REPEAT_RANDOM:
        playfile.given_file_number = get_u16_random();
        playfile.given_file_number = (playfile.given_file_number % fileTotal) + 1;
        break;
#endif

    case REPEAT_ALL:
        if (searchMode == PLAY_PREV_FILE)					//prev file
        {
            playfile.given_file_number--;
            if (playfile.given_file_number == 0)
            {
                return GET_DEVICE_HEAD;
            }
        }
        else					                //next file
        {
            playfile.given_file_number++;
            if (playfile.given_file_number > fileTotal)
            {
                playfile.given_file_number = 1;
                return GET_DEVICE_END;
            }
        }
        break;
#ifdef FOLDER_PLAY_EN
    case REPEAT_FOLDER:
        {
            get_dir_file_total();
            //deg("X-playfile.given_file_number : %04x\n", playfile.given_file_number);
            if (searchMode == PLAY_PREV_FILE)
            {
                playfile.given_file_number--;
                if (playfile.given_file_number == ff_api.fs_msg.fileTotalOutDir)
                    playfile.given_file_number = ff_api.fs_msg.fileTotalOutDir + ff_api.fs_msg.fileTotalInDir;
                //deg("2-playfile.given_file_number : %04x\n", playfile.given_file_number);
            }
            else
            {
                playfile.given_file_number++;
                if (playfile.given_file_number > (ff_api.fs_msg.fileTotalOutDir + ff_api.fs_msg.fileTotalInDir))
                    playfile.given_file_number = ff_api.fs_msg.fileTotalOutDir + 1;
                //deg("3-playfile.given_file_number : %04x\n", playfile.given_file_number);
            }
        }
        break;
#endif
    }

    return GET_FILE_OK;
}



