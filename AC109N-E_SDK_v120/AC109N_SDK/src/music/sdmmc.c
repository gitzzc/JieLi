/*--------------------------------------------------------------------------*/
/**@file    sdmmc.c
   @brief   SD 接口函数
   @details
   @author  bingquan Cai
   @date    2012-8-30
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/

#include "sdmmc_api.h"
#include "main.h"
#include "device.h"
#include "play_file.h"


_no_init bool __bit sd_a_clk;
_no_init bool __bit sd_b_clk;
_no_init u8 _data cnt_online_a;
_no_init u8 _data cnt_online_b;
_no_init u8 sdmmc_ctler_flag;

/*----------------------------------------------------------------------------*/
/**@brief	 sdmmc command 检测函数
   @param	 无
   @return   无
   @note	 void sdmmc_command_detect(void)
*/
/*-----------------------------------------------------------------------------*/
_near_func void sdmmc_cmd_det(void) AT(COMMON_CODE)
{
    u8 tmp;

    tmp = sd_online_check();

    if (tmp == 0xff)        //状态没有改变
        return;

#ifdef SDMMCA_EN
    if (1 ==(tmp & 0xf))
    {
        insert_sd(SDMMCA_ID);
    }
    else  if (0 ==(tmp & 0xf))
    {
        remove_sd(SDMMCA_ID);
    }
#endif

#ifdef SDMMCB_EN
    if (0x10 ==(tmp & 0xf0))
    {
        insert_sd(SDMMCB_ID);
    }
    else  if (0 ==(tmp & 0xf0))
    {
        remove_sd(SDMMCB_ID);
    }
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief	 使用sdmmc clock引脚检测SD在线状态的函数
   @param	 无
   @return   无
   @note	 void sdmmc_clock_detect(void)
*/
/*-----------------------------------------------------------------------------*/
_near_func void sdmmc_clk_det0(void)
{
//    u8 tmp;
#ifdef SDMMCA_EN
    tmp = sdmmc_clkio_chk( SDMMCA_ID );
    if( (SDMMCA_ID != tmp) && (SDMMCA_ID != sdmmc_ctler_flag))
    {
        P2PU |= 0x01;
        P2DIR |= 0x01;
        sd_a_clk = 1;
    }
#endif

#ifdef SDMMCB_EN
    tmp = sdmmc_clkio_chk(SDMMCB _ID );
    if ((SDMMCB_ID != tmp) && (SDMMCB_ID != sdmmc_ctler_flag))
    {
        //P3PU |= 0x01;
        //P3DIR |= 0x01;
        P3PU |= 0x20;
        P3DIR |= 0x20;
        sd_b_clk = 1;
    }
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief	 使用 sdmmc clock 引脚检测SD在线状态的函数
   @param	 无
   @return   无
   @note	 void deal_sdmmc_clock_detect(void)
*/
/*-----------------------------------------------------------------------------*/
_near_func  void sdmmc_clk_det1(void)
{
#ifdef SDMMCA_EN
    if (sd_a_clk)
    {
        sd_a_clk = 0;

        if (P20)
        {
            cnt_online_a = 0;
            remove_sd(SDMMCA_ID);
        }
        else
        {
            if (cnt_online_a < 35)
            {
                cnt_online_a++;
            }

            if (cnt_online_a == 30)
            {

                insert_sd(SDMMCA_ID);
            }
        }
        if(set_sd_clk)
        {
            P2DIR &=  ~0x01;
            P20 = 1;
        }

    }
#endif


#ifdef SDMMCB_EN
    if (sd_b_clk)
    {
        sd_b_clk = 0;

        if (P35)
        {
            cnt_online_b = 0;
            remove_sd(SDMMCB_ID);
        }
        else
        {
            if (cnt_online_b < 35)
            {
                cnt_online_b++;
            }
            if (cnt_online_b == 30)
            {
                insert_sd(SDMMCB_ID);
            }
        }

        if(set_sd_clk)
        {
            P3DIR &=  ~0x20;
            P35 = 1;
        }
    }
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief	 使用独立IO口检测SD在线状态的函数
   @param	 无
   @return   无
   @note	 void sdmmc_io_detect(void)
*/
/*-----------------------------------------------------------------------------*/
_near_func void sdmmc_io_detect(void)
{
#ifdef SDMMCA_EN
    ///<A
    P1PU |=  0x01;
    P1DIR |=  0x01;

    if ( P10 )
    {
        cnt_online_a = 0;
        remove_sd(SDMMCA_ID);
    }
    else
    {
        if (cnt_online_a < 30)
        {
            cnt_online_a++;
        }
        else
        {
            insert_sd(SDMMCA_ID);
        }
    }
#endif

#ifdef SDMMCB_EN
    ///<B
    P1PU |=  0x02;
    P1DIR |=  0x02;

    if ( P11 )
    {
        cnt_online_b = 0;
        remove_sd(SDMMCB_ID);
    }
    else
    {
        if (cnt_online_b < 30)
        {
            cnt_online_b++;
        }
        else
        {
            insert_sd(SDMMCB_ID);
        }
    }
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
_monitor u8 check_sd_controller(void)
{
    return get_sd_ctller_status(1);
}

/*----------------------------------------------------------------------------*/
/** @brief:
    @param:
    @return:
    @author:Juntham
    @note:
*/
/*----------------------------------------------------------------------------*/
bool sdmmc_idle(u8 dev)
{
    if (0 == check_sd_controller())
    {
        u8 p_status;

        p_status = pause_decode(0);
        sdmmc_ctler_flag = dev;
        suspend_sdmmc(dev);
        unlock_cmd_chk();
        sdmmc_ctler_flag = 0;
        if (p_status == MAD_PLAY)
            start_decode();
        return true;
    }
    else                            //此次写失败
        return false;
}



/*----------------------------------------------------------------------------*/
/**@brief	 初始化sdmmc模块
   @param	 无
   @return   无
   @note	 void sdmmc_init(void)
*/
/*-----------------------------------------------------------------------------*/
void sdmmc_init(void)
{
    init_sd_ctller(SDMMCA_CLK,SDMMCA_CMD_DATA,SDMMCB_CLK,SDMMCB_CMD_DATA);

#ifdef SDMMCA_EN
//////////////////<sdmmc p20 clock
    P2PU |=  0x01;
    P2DIR &= 0xfe;
//////////////////<sdmmc cmd p21  data p22
    P2 |=  0x06;
    P2PU |=  0x06;
    P2DIR &= 0xf9;
    P2HD |= 0x7;
#endif

#ifdef SDMMCB_EN
//////////////////<sdmmc clock p30
    //P3PU |=  0x01;
    //P3DIR &= 0xfe;
    P3PU |=  0x20;
    P3DIR &= 0xdf;
//////////////////<sdmmc cmd p00  data p01
    P0 |=  0x03;
    P0PU |=  0x03;
    P0DIR &= 0xfc;
    P0HD |= 0x03;
#endif

    sd_speed_init(2,100);
    sd_chk_ctl(SET_SD_CHK_STEP,20);
    int_enter_pro[SDC_INT] = sdisr_fun;
    SDC_IP(1);
    SDC_IE(1);
}








