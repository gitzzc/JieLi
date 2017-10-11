/*************************************************************/
/** @file:sdmmc_api.h
    @brief:
    @details:
    @author:Juntham
    @date: 2012-07-25,11:42
    @note:
*/
/*************************************************************/
#ifndef _SDMMC_API_H_
#define _SDMMC_API_H_

#include "config.h"

#ifdef __IAR_SYSTEMS_ICC__

typedef enum
{
    SD_CLK_DISABLE = 0x00,
    SD_CLK_P20,
    SD_CLK_P30,
    SD_CLK_P35,
}sdmmc_clock_io;

typedef enum
{
    SD_CMDP21_DATP22 = 0x00,
    SD_CMDP00_DATP01 = 0x04,
}sdmmc_dat_cmd_io;


typedef enum
{
    SDMMCA_ID = 0x01,                                    ///<SD卡不在线
    SDMMCB_ID = 0x02,//1                                   ///<SD卡刚插入,未进行初始化
}sdmmc_card_id;



typedef enum
{
    DIS_SD_CMD_CHK = 0,
    EN_SD_CMD_CHK,
    SET_SD_CHK_STEP,

}Sd_api_cmd;


typedef struct __strfor_card_io
{
    volatile u8 clk;
    volatile u8 datcmd;
}SDMMC_IO;


/*******************************************/
/*
            Maskrom API
                --SD Part
*/
/*******************************************/
_near_func u8 get_sdmmc_online_status(u8 sd_id);
_near_func __monitor bool suspend_sdmmc(u8 sd_id);
_near_func bool api_remove_sd(u8 sd_id);          ///< SDMMC_STR _idata * curr_card);
_near_func u8 sdmmc_read( u8 _xdata *buf,u32 lba,u8 sd_id);
u8 sdmmc_write( u8 _xdata *buf,u32 lba,u8 sd_id);
_near_func u8 sdmmc_clkio_chk(u8 sd_id);       ///<SDMMC在线检测 CLK
_near_func void sd_chk_ctl(u8 flag,u8);
_near_func void sdisr_fun(void);
_near_func void init_sd_ctller(u8 aclk,u8 adc,u8 bclk,u8 bdc);    ///<初始化SDMMC控制器
_near_func void sd_speed_init(u8,u8);
_near_func u8 sd_online_check(void);             ///<SDMMC在线检测 CMD
_near_func u8 identif_sdmmc( u8 card_id );       ///<鉴定SDMMC卡
_near_func u8 sdmmcA_read( u8 _xdata *,u32 );    ///<A CARD的解码读
_near_func u8 sdmmcB_read( u8 _xdata *,u32 );    ///<B CARD的解码读
_near_func u8 get_sd_ctller_status(bool flag);
_near_func void lock_cmd_chk(void);
_near_func void unlock_cmd_chk(void);
_near_func void remove_sd(u8 sd_id);
_near_func void insert_sd(u8 sd_id);
_near_func void sdmmc_time_out(void);

////<app fun
_near_func void sdmmc_cmd_det(void);
_near_func void sdmmc_clk_det0(void);
_near_func  void sdmmc_clk_det1(void);
_near_func void sdmmc_io_detect(void);
void sdmmc_init(void);
_monitor u8 check_sd_controller(void);
bool sdmmc_idle(u8 dev);

/////SD接口选择
//card A
#ifdef SDMMCA_EN
#define SDMMCA_CLK  SD_CLK_P20
#define SDMMCA_CMD_DATA  SD_CMDP21_DATP22

#if (defined(SDMMCA_DET_CMD) && defined(SDMMCA_DET_CLK)) || \
    (defined(SDMMCA_DET_CMD) && defined(SDMMCA_DET_IO))  || \
    (defined(SDMMCA_DET_CLK) && defined(SDMMCA_DET_IO))
#error("SDMMCA detect select err")
#endif

#else
#define SDMMCA_CLK  0
#define SDMMCA_CMD_DATA 0
#endif
////card B
#ifdef SDMMCB_EN
#define SDMMCB_CLK  SD_CLK_P35 //SD_CLK_P30
#define SDMMCB_CMD_DATA  SD_CMDP00_DATP01

#if (defined(SDMMCB_DET_CMD) && defined(SDMMCB_DET_CLK)) || \
    (defined(SDMMCB_DET_CMD) && defined(SDMMCB_DET_IO))  || \
    (defined(SDMMCB_DET_CLK) && defined(SDMMCB_DET_IO))
#error("SDMMCB detect select err")
#endif

#else
#define SDMMCB_CLK  0
#define SDMMCB_CMD_DATA 0
#endif

////cmd 检测
#if  defined (SDMMCA_DET_CMD) || defined (SDMMCB_DET_CMD)
#define sdmmc_command_detect()  sdmmc_cmd_det()
#else
#define sdmmc_command_detect()
#endif

////是否需要在SDMMC卡不工作时进入等待模式
#if defined (SDMMC_IDLE_EN) && defined (SDMMCA_EN)
#define sdmmca_idle_mode()    sdmmc_idle(SDMMCA_ID)
#define sdmmca_force_idle()   while(!sdmmc_idle(SDMMCA_ID))
#else
#define sdmmca_idle_mode()
#define sdmmca_force_idle()
#endif

#if defined (SDMMC_IDLE_EN) && defined (SDMMCB_EN)
#define sdmmcb_idle_mode()    sdmmc_idle(SDMMCB_ID)
#define sdmmcb_force_idle()   while(!sdmmc_idle(SDMMCB_ID))
#else
#define sdmmcb_idle_mode()
#define sdmmcb_force_idle()
#endif

/////////////////

#endif



#endif
