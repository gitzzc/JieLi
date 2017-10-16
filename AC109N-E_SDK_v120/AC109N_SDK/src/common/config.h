/*--------------------------------------------------------------------------*/
/**@file     config.h
   @brief    一些基本参数的配置
   @author
   @date   2010-12-13
   @note   AC109N
*/
/*----------------------------------------------------------------------------*/
#ifndef _CONFIG_
#define _CONFIG_

//////////////////////////////////////////////////////////////////
#include "cd02.h"

#ifdef __IAR_SYSTEMS_ICC__
#define __SMART_CPU__
#define WDT_CLEAR()     WDT_CON |= BIT(6)

//#ifndef NDEBUG
#if 1

#define _near_func __near_func
#define _banked_func __banked_func

#define deg         my_printf
#define deg_puts    my_puts

#else
#define deg(...)
#define deg_puts(...)
#define _near_func
#define _banked_func
#endif              //end NDEBUG

#include "typedef.h"
#include "intrinsics.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "INT.h"
#include "uart.h"
#include "common.h"

#endif          // end __IAR_SYSTEMS_ICC__

/*---------System Configuration---------------*/

//---------------------------------------------------------------------------------------------------------------/
/*---------Clock Configuration*/
#define OSC_32768           32768L
#define OSC_12M             12000000L

#define OSC_CLK             OSC_32768

//SYSTEM CLOCK
enum
{
    CLK_512K = 0,            //512000L
    CLK_1M,              //1000000L
    CLK_2M,              //1000000L
    CLK_12M,             //12000000L
    CLK_24M,             //24000000L
    CLK_48M,             //48000000L
};


#define SYSTEM_CLK          CLK_24M
//#define CLK_USE_HTC
//#define CLK_USE_32K_WITH_HTC
#define CLK_USE_32K_NO_HTC
//#define CLK_USE_12M_NO_HTC
//#define CLK_USE_12M_WITH_HTC

//#define SHARE_RTC_OSC_TO_FM
//---------------------------------------------------------------------------------------------------------------/
#define WDT_EN              //是否允许看门狗复位
//---------------------------------------------------------------------------------------------------------------/
///在音乐模式下，音量调节的是数字音量，如果要减少音量级数，需要修改数字音量表，但必须保留最大值：16384
///如果需要减小音乐模式下的DAC输出音量，可以对输出音量作以下的限制
#define DECODE_VOL_LIMIT_L  31          //在音乐模式下，DAC输出音量最大值(0~31)
#define DECODE_VOL_LIMIT_R  31

//主音量级数
#define MAX_MAIN_VOL_L    31
#define MAX_MAIN_VOL_R    31


//DACVDD_VCOMO选择，DAC_VDD和VCOMO是同一PIN，同时只能选择其中一种功能，也可以两种都不选(DAC_VDD悬空)
#define DAC_VDD_EN                      //DAC_VDD外接电容，SNR可以提高1.5dB

//是否选择VCMO直推耳机
#ifndef DAC_VDD_EN
#define VCOMO_EN
#endif

//---------------------------------------------------------------------------------------------------------------/
/*---------UI Configuration*/
//#define LCD_96X32_SERIAL
//#define LCD_128X32_SERIAL
//#define LCD_128X64_SERIAL
//#define LCD_128X64_PARALLEL
//#define LCD_SEG_5X9
#define LED_5X7

#if defined LCD_96X32_SERIAL || defined LCD_128X32_SERIAL \
  || defined LCD_128X64_SERIAL || defined LCD_128X64_PARALLEL \
    || defined LCD_SEG_4X8 || defined LED_5X7 || defined LCD_SEG_5X9
#define UI_ENABLE           //UI 界面开关

#ifdef LED_5X7
#define UI_FADE_EN
#endif

#ifdef LCD_SEG_5X9
//#define STANDBY_EN   //段码屏RTC模式下走时推屏开关
#endif

#endif

#ifdef UI_FADE_EN
#define LED_FADE_ON()   set_LED_all_on()
#define LED_FADE_OFF()  set_LED_fade_out()
#else
#define LED_FADE_ON()
#define LED_FADE_OFF()
#endif


//---------------------------------------------------------------------------------------------------------------/
/*---------FM Configuration*/
#define RDA5807
//#define BK1080
//#define KT0830EG
//#define QN8035
//#define AR1019

#if defined RDA5807 || defined BK1080 || defined KT0830EG || defined QN8035 || defined AR1019
#define FM_ENABLE           //FM 模式开关
#endif

//---------------------------------------------------------------------------------------------------------------/
/*---------Work Mode Configuration*/
//#define USB_DEVICE_EN       //Enable USB SLAVE MODE

//#define USB_VIR_KEY_EN      //Enable USB virtual key

//---------------------------------------------------------------------------------------------------------------/

/*---------System Charateristic Configuration v100---------------*/
//#define    IR_REMOTE_EN                 //是否需要红外遥控
///<音乐播放功能选择
//#define    LAST_MEM_FILE_PLAY_EN        //是否允许记忆文件序号播放功能
#ifdef     LAST_MEM_FILE_PLAY_EN
#define    BREAK_POINT_PLAY_EN			//是否允许断点播放功能
#endif

//#define   MUSIC_POWER_EN              //音乐能量谱显示
//#define 	FF_FR_MUSIC_EN               //在快进快退时，是否需要听到声音

//#define     GET_MUSIC_TOTAL_TIME         //是否获取音乐文件总时间
//#define     RANDOM_PLAY_EN               //是否支持随机播放功能
//#define 	FOLDER_PLAY_EN               //是否支持文件夹切换和播放功能

#define 	USB_DISK_EN      			 //是否可以读U盘
//#define 	SDMMC_IDLE_EN                //可以使SDMMC卡进入省电模式
#define 	UDISK_IDLE_EN                //有些(较少)U盘在此模式下，重新枚举会有问题


#define     USE_EEPROM_MEMORY          //使用EEPROM 作为存储器记忆
#ifndef     USE_EEPROM_MEMORY             //使用内部RTC RAM作记忆
#define     USE_IRTC_MEMORY
#endif

#ifdef      USE_EEPROM_MEMORY
#define     CHECK_EEPROM_ON_POWER_ON     //是否在上电时，校验eeprom
#endif


/*---------System Charateristic Configuration v101---------------*/
#define RTC_EN                  //<RTC 模式使能控制位
#define RTC_ALARM_EN            //<RTC 闹钟使能控制位
#if (!defined RTC_EN) && (defined RTC_ALARM_EN)
#error("RTC selectd err!")
#endif

#define 	KEY_VOICE_EN                 //按键音使能, 如果使用按键音，则不可以使用数据检测自动mute/unmute功能
#if !defined KEY_VOICE_EN && !defined RTC_ALARM_EN                  //没有按键音时，方可以使用音乐数据检测，否则按键在歌曲切换，暂停等情况下会无按键音
#define     MUSIC_ENERGY_DETECT          //用于检测解码后的数据大小,可据此判断自动mute/unmute DAC,有淡入淡出效果
#define     MAX_WAVEFORM        1        //检测解码后数据的限制范围,设定值为一个正负等距区间
//门槛值必须为非0值(1~255), 1:完全静音, 2: <-90.3dB, 3: < -84.29dB, 4: <-80.7dB, 5: <-78.26dB, 6: <-76.3dB
#endif

///<SD卡接口选择
//#define SDMMCA_EN
//#define SDMMCB_EN

#ifdef SDMMCA_EN                 //只有选择了A卡才可能选择B卡，如果是单卡，则统一使用A卡
#define SDMMCB_EN0
#endif
///<SD卡检测方式检测
///<A CARD
#ifdef SDMMCA_EN

#define SDMMCA_DET_CMD
//#define SDMMCA_DET_CLK
//#define SDMMCA_DET_IO
#endif

///<B CARD
#ifdef SDMMCB_EN
//#define SDMMCB_DET_CMD
#define SDMMCB_DET_CLK
//#define SDMMCB_DET_IO
#endif

#define P04_TOGGLE       P0DIR &= ~BIT(4); P04 = ~P04
#define P05_TOGGLE       P0DIR &= ~BIT(5); P05 = ~P05

#endif

