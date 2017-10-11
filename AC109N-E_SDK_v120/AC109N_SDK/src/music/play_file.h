/*************************************************************/
/** @file:play_file.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-07-31,11:09
    @note:
*/
/*************************************************************/
#ifndef __PLAY_FILE_H__
#define __PLAY_FILE_H__
#include "config.h"
#include "tff.h"

//OBUF复位及buffer大小设置
#define OBUF_REST       0x0
#define OBUF_0          0x1
#define OBUF_1          0x2
#define OBUF_2          0x3
#define OBUF_3          0x4

#ifdef __SMART_CPU__

enum
{
    MP_LAYER1 = 1,
    MP_LAYER2,
    MP_LAYER3,
    WAV_FORMAT = 8,
    WMA_FORMAT = 16,
};

enum
{
    UNKNOW_FORMAT = 0,
    IS_WAV,
    IS_MP3,
    NOT_SUPPORT,
    //IS_WMA,
};

enum 							//config command
{
    MAD_STOP = 0x00,
    MAD_PLAY = 0x01,
    MAD_PLAY_IDLE=0x11,
    MAD_PAUSE = 0x02,
    MAD_FAST_FORWARD = 0x03,
    MAD_FAST_REVERASE = 0x04,
    MAD_INIT = 0x05,
    MAD_FF_FR_END = 0x06,
    MAD_VOICE_STOP = 0x07,

};



enum
{
    MAD_ERROR_NONE	   = 0x00,	/* no error */
    MAD_ERROR_USER_EXIT = 0x01,
    MAD_ERROR_BUFFER_LEN         = 0x02,
    MAD_ERROR_NEXT_FRAME      = 0x03,

    MAD_ERROR_LOSTSYNC	   = 0x11,	/* lost synchronization */
    MAD_ERROR_BADLAYER	   = 0x12,	/* reserved header layer value */
    MAD_ERROR_BADBITRATE	   = 0x13,	/* forbidden bitrate value */
    MAD_ERROR_BADSAMPLERATE  = 0x14,	/* reserved sample frequency value */
    MAD_ERROR_BADEMPHASIS	   = 0x15,	/* reserved emphasis value */

    MAD_ERROR_BADCRC	   = 0x21,	/* CRC check failed */
    MAD_ERROR_BADBITALLOC	   = 0x022,	/* forbidden bit allocation value */
    MAD_ERROR_BADSCALEFACTOR = 0x23,	/* bad scalefactor index */
    MAD_ERROR_BADMODE        = 0x24,	/* bad bitrate/mode combination */
    MAD_ERROR_BADFRAMELEN	   = 0x25,	/* bad frame length */
    MAD_ERROR_BADBIGVALUES   = 0x26,	/* bad big_values count */
    MAD_ERROR_BADBLOCKTYPE   = 0x27,	/* reserved block_type */
    MAD_ERROR_BADSCFSI	   = 0x28,	/* bad scalefactor selection info */
    MAD_ERROR_BADDATAPTR	   = 0x29,	/* bad main_data_begin pointer */
    MAD_ERROR_BADPART3LEN	   = 0x2A,	/* bad audio data length */
    MAD_ERROR_BADHUFFTABLE   = 0x2B,	/* bad Huffman table select */
    MAD_ERROR_BADHUFFDATA	   = 0x2C,	/* Huffman data overrun */
    MAD_ERROR_BADSTEREO	   = 0x2D,	/* incompatible block_type for JS */
    MAD_ERROR_BADNCH	   = 0x2E,	/* incompatible block_type for JS */

    MAD_ERROR_FILE_END         = 0x40,
    MAD_ERROR_DISK_ERR         = 0x41,
    MAD_ERROR_SYNC_LIMIT       = 0x42,
    MAD_ERROR_FF_FR_FILE_END   = 0x43,
    MAD_ERROR_FF_FR_END        = 0x44,
    MAD_ERROR_FF_FR_FILE_START = 0x45,
    MAD_ERROR_LIMIT            = 0x46,
};

typedef struct _DECODER_MSG
{
    u16 (_near_func *read) (FIL _xdata *, u8 _xdata *buffer, u16 len);
    FRESULT (_near_func *seek) (FIL _xdata *, u8, u32);
    FIL _xdata * fp;
    u8 const __code * eq_table;
    u8 _xdata *buffer;      /*解码所使用的buffer*/
    u8  format;             /*音频格式：1，2，3：MP3， 8~15：WAV，16~23：WMA*/
    u32 frame_counter;	    /*当前播放的帧号计数器，在WMA格式下，对应包的边界*/
    u16 total_time;          /*歌曲总时间*/
    u16 play_time;          /*当前播放时间，单位为秒*/
    u8 bit_rate;            /*当前帧的编码率*/
    u16 sample_rate;        /*当前帧的采样率*/
    u32 tag_len;            /*文件头信息的长度，一个文件可能同时有WAV头和ID3头*/
    u32 next_frame;
    u8  err;
    u8  err_counter;
    u16 ff_fr_counter;
}DECODER_MSG;


typedef struct _BREAK_POINT_INFO
{
    u16 wFsizeID;
    u16 wSclustID;
    u32 dFrameCounter;
    u32 dNextFrame;
}BREAK_POINT_INFO;


__monitor u8 pause_decode(u8 type);
__monitor void start_decode(void);
void stop_decode(void);
bool decode_init(bool break_point);
void init_decode_api(u16 (_near_func *read)(FIL _xdata *,u8 _xdata* , u16), FRESULT (_near_func *seek) (FIL _xdata *, u8, u32));
_near_func void user_exit_decoder(void);

extern _no_init u8 _data music_format;
extern _no_init bool _bit jl_flag;
extern _no_init volatile bool _bit dec_exit;
extern _no_init volatile bool _bit voice_playing;
extern _no_init u8 _xdata decode_buf[];
extern _no_init DECODER_MSG _pdata dec_msg;
extern _no_init BREAK_POINT_INFO _idata music_break_info;


/*******************************************/
/*
            Maskrom API
                --Decoder Part
*/
/*******************************************/
_near_func u8 wav_format_check(u8 _xdata *buf);
_near_func void dac_set_sample_rate(u8 sample);
_near_func void obuf_setting(u8 mode);
_near_func u16 get_obuf_len(void);
_near_func void wav_decode(void);
_near_func u16 get_music_play_time(void);

_near_func u8 mp3_decode(void);
_near_func bool get_mp3_total_time(void);
_near_func _monitor void music_fffr(u8 cmd, u16 frame);
_near_func bool music_decode_init(u32 frame, u32 f_ptr);
_near_func void mp3_init(void);
_near_func void get_play_log(void);
_near_func void play_log_backup(void);
_near_func void get_mp3_play_time(void);
_near_func u16 cal_frame_len(u8 bit_rate);
_near_func u8 decode_loop(void);
_near_func u32 get_ff_id3v2_len(u8 _xdata *buf);
_near_func u8 get_1s_musice(void);
_near_func void sideinfo_lib(void);

#endif

#endif
