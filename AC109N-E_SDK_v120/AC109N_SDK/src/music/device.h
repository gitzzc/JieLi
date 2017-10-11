/*--------------------------------------------------------------------------*/
/**@file    DEVICE.h
   @brief   设备控制接口
   @details
   @author
   @date   2010-12-13
   @note
*/
/*----------------------------------------------------------------------------*/
#ifndef __DEVICE_H__
#define __DEVICE_H__


#include "config.h"


#ifdef __SMART_CPU__

typedef enum
{   
    DEVICE_UDISK,               //必须占住0位置，不管是否有读U盘的功能

#ifdef SDMMCA_EN    
    DEVICE_SDMMCA,
#endif    

#ifdef SDMMCB_EN    
    DEVICE_SDMMCB,
#endif
    
#ifdef SDMMCB_EN    
    MAX_DEVICE = DEVICE_SDMMCB,
#elif  defined (SDMMCA_EN)
    MAX_DEVICE = DEVICE_SDMMCA,
#else
    MAX_DEVICE = DEVICE_UDISK,
#endif        
    
    DEVICE_AUTO_NEXT = 0x81,
    DEVICE_AUTO_PREV = 0x82,
    NO_DEVICE = 0xff,
}ENUM_DEVICE_SELECT_MODE;

enum
{
    FIND_DEV_OK      = 0,				//成功找到设备
    NO_DEFINE_DEV,    					//指定设备未找到
    DEV_INIT_ERR,     					//找到设备，但初始化失败
    NO_EFFECTIVE_DEV,					//无有效可以使用的设备
    NO_DEV_ONLINE,    					//无在线设备
};

typedef struct _DEVICE_INFO
{
    u16 wBreak_point_filenum;
    u16 wfileTotal;
}DEVICE_INFO;

u8 find_device(ENUM_DEVICE_SELECT_MODE);
u8 device_init(u8 dev);
u8 device_check(void);
_bool send_vir_key(u8 key);

extern u8 _data device_active;
extern u8 _data device_online;
extern _no_init bool _bit find_break_point_file_flag;
extern _no_init DEVICE_INFO _idata music_device_info[MAX_DEVICE + 1];
extern _no_init bool _bit have_vir_key;
#endif


#endif
