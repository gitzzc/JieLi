/*--------------------------------------------------------------------------*/
/**@file     main.c
   @brief    主流程
   @details
   @author
   @date    2011-3-7
   @note    AC109N
*/
/*----------------------------------------------------------------------------*/
#include "main.h"
#include "clock.h"
#include "music_play.h"
#include "fm_radio.h"
#include "USB_Device_Mode.h"
#include "key.h"
#include "RTC_Mode.h"
#include "RTC_API.h"
#include "msg.h"
#include "iic.h"
#include "device.h"
#include "DAC.h"
#include "get_music_file.h"
#include "usb_host.h"
#include "sdmmc_api.h"
#include "LCD.h"
#include "LED5X7.h"
#include "UI_API.h"
#include "Line_in_mode.h"
#include "LCD_SEG4X8.h"
#include "SPI.h"
#include "LCD_SEG5X9.h"


_no_init void (_near_func * _pdata int_enter_pro[16])(void) AT(INTERRUPT_TABLE);
_no_init ENUM_WORK_MODE _data work_mode;
_root _no_init u8 _data B_CURRENTBANK @ 0x1F;
_no_init bool _bit Sys_Volume;
_no_init bool _bit Sys_IRInput;
_no_init bool _bit Sys_HalfSec;
_no_init bool _bit alm_on_flag;
_no_init bool _bit set_sd_clk; ///<sd clk检测用，要操作卡时要设置为1,不操作卡时要设置为0，用途是不读卡时把clk脚保持一个稳定的电平
/*----------------------------------------------------------------------------*/
/**@brief   显示设备扫描函数
   @param   void
   @return  void
   @author  Change.tsai
   @note    void monitor_scan(void)
*/
/*----------------------------------------------------------------------------*/
_near_func void monitor_scan(void) AT(COMMON_CODE)
{
#ifdef LED_5X7
    LED5X7_scan();
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief: Timer1 中断服务程序
    @param: void
    @return:void
    @author:Change.tsai
    @note:  void Timer1_ISR(void)
*/
/*----------------------------------------------------------------------------*/
_near_func __root void Timer1_ISR(void) AT(COMMON_CODE)
{
    static _no_init u8 _data cnt;

    TMR1_CON0 |= BIT(6);            //clear pending

    dac_check();

    /*UI Update*/
    //monitor_scan();

    /*Device Scan*/
    USB_online_otp();

#ifdef SDMMCA_DET_CLK
     sdmmc_clk_det0();
#endif
#ifdef SDMMCA_DET_CMD
    sdmmc_command_detect();
#endif
    ADC_scan();

#ifdef IR_REMOTE_EN
    /*IR overflow*///if (IR_code.boverflow)
    {
        IR_code.boverflow++;
        if (IR_code.boverflow > 56) //56*2ms = 112ms
        {
            //IR_code.boverflow = 0;
            /*IR Signal Clear*/
            IR_code.bState = 0;
        }
    }
#endif

    cnt++;
    if ((cnt % 5) == 0)             //10ms
    {
        //USB_online_otp();
        key_scan();
        dec_delay_counter();
    }
    if ((cnt % 50) == 0) 
    {
        put_msg_fifo(MSG_100MS);      
    }
    if ((cnt % 250) == 0)
    {
        cnt = 0;
        //USB_online_otp();
        Sys_HalfSec = !Sys_HalfSec;
        put_msg_fifo(MSG_HALF_SECOND);
    }
#ifdef RTC_ALARM_EN
    if (RTC_CON0 & BIT(7))
    {
        IRTC_WKCLRPND;
        put_event(EVENT_ALM_ON);
    }
#endif
#ifdef SDMMCA_DET_CLK
    sdmmc_clk_det1();
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief   Timer2 中断服务程序
   @param   void
   @return  void
   @author  Change.tsai
   @note    void Timer1_ISR(void)
*/
/*----------------------------------------------------------------------------*/
const u16 timer2_1ms_table[] AT(TABLE_CODE)=
{
    0x20,0x40,0x80,0x2f0,0x178,0x2f0
};
_near_func __root void Timer2_ISR(void) AT(COMMON_CODE)
{
    u16 bCap2;
    u8 cap;

    TMR2_CON |= BIT(6);

    bCap2 = (TMR2_PRDH << 8) | TMR2_PRDL;

    cap = bCap2 / timer2_1ms_table[system_clk];

    if (cap == 1)
    {
        IR_code.wData >>= 1;
        IR_code.bState++;
        IR_code.boverflow = 0;
    }
    else if (cap == 2)
    {
        IR_code.wData >>= 1;
        IR_code.bState++;
        IR_code.wData |= 0x8000;
        IR_code.boverflow = 0;
    }
    /*13ms-Sync*/
    else if ((cap == 13) || (cap < 8) || (cap > 108))
    {
        IR_code.bState = 0;
    }
    else
        IR_code.boverflow = 0;

    if (IR_code.bState == 16)
    {
        IR_code.wUserCode = IR_code.wData;
    }
}
/*----------------------------------------------------------------------------*/
/** @brief: 串口模块初始化
    @param: void
    @return:void
    @author:Juntham
    @note:  void uart_setup(void)
*/
/*----------------------------------------------------------------------------*/
void uart_setup(void) AT(CSTART)
{
    uart_init();
    set_putchar(my_putchar);
    my_puts(__DATE__);
    my_puts("\n");
    my_puts(__TIME__);
    my_puts("\n");
}



/*----------------------------------------------------------------------------*/
/**@brief   UI 初始化
   @param   void
   @return  void
   @author  Change.tsai
   @note    void ui_init(void)
*/
/*----------------------------------------------------------------------------*/
static void ui_init(void) AT(CSTART)
{
#if defined LED_5X7
    LED5X7_init();
#endif

#if defined LCD_SEG_4X8 || LCD_SEG_3X9
    LCD_SEG4X8_init();
#endif

#if defined LCD_SEG_5X9
    LCD_SEG5X9_init();
#endif

#if defined LCD_96X32_SERIAL || defined LCD_128X32_SERIAL   \
    || defined LCD_128X64_SERIAL || defined LCD_128X64_PARALLEL
    LCD_init();
#endif
}


/*----------------------------------------------------------------------------*/
/**@brief   系统初始化
   @param   void
   @return  void
   @author  Change.tsai
   @note    void sys_init(void)
*/
/*----------------------------------------------------------------------------*/
static void sys_init(void) AT(CSTART)
{
    /*IIC I/O init*/
    iic_init();
/*    P0DIR |= (1<<2)|(1<<0);
    P0PU |= (1<<2)|(1<<0);
    P00 = 1;
    P02 = 1;*/

    /*UI init*/
    ui_init();

    /*Time Base init*/
    int_enter_pro[TIMER1_INT] = Timer1_ISR;

#ifdef IR_REMOTE_EN
    /*IR Capture init*/
    int_enter_pro[TIMER2_INT] = Timer2_ISR;
#endif
    /*Timebase interrupt init*/
    IRTC_init();

    /*SD I/O & Controller init*/
    //set_sd_clk = 1;
    //sdmmc_init();

    /*ADKey init*/
    AD_init();

    /*Hello UI*/
    UI_menu(MENU_POWER_UP);

    /*ALL interrupt*/
    EA = 1;

    /*Wait Device steady Plug In...*/
    delay_n10ms(20);

    /*PLL初始化，会关总中断*/
	pll_init();
    uart_setup();
    /*EEPROM verify*/
	eeprom_verify();
	/*DAC 初始化*/
    DAC_init();
    work_mode =(ENUM_WORK_MODE)get_memory(MEM_SYSMODE);
    playfile.given_device = NO_DEVICE;
}

/*----------------------------------------------------------------------------*/
/** @brief: 系统主循环
    @param: void
    @return:void
    @author:Juntham
    @note:  void main(void)
*/
/*----------------------------------------------------------------------------*/
void main(void) AT(CSTART)
{
#ifndef NDEBUG
    set_debug_mode(1);          //进入2线调试模式
#else
    set_debug_mode(0);
#endif

#ifdef WDT_EN
    config_wdt(0x5D);           //enable WDT 8S,如果改成4S,有些慢U盘会导致看门狗复位
#endif
    sys_init();
    uart_setup();
    clear_all_event();

    while (1)
    {
        flush_all_msg();
        switch (work_mode)
        {
        case MUSIC_MODE:
            deg_puts("-Music Mode\n");
            music_app();
            break;

#ifdef USB_DEVICE_EN
        case USB_DEVICE_MODE:
            //deg_puts("-PC Mode\n");
            USB_device_mode();
            break;
#endif

#ifdef FM_ENABLE
        case FM_RADIO_MODE:
            deg_puts("-FM Mode\n");
            fm_mode();
            break;
#endif
/*
        case AUX_MODE:
            deg_puts("-AUX Mode\n");
            Line_in_mode();
            break;
*/
#ifdef RTC_EN
        case RTC_MODE:
            deg_puts("-RTC Mode\n");
            RTC_mode();
            break;
#endif
        case OFF_MODE:
          deg_puts("OFF_MODE\n");
          OFF_mode();
          break;
        default:
            work_mode = OFF_MODE;
            break;
        }
        set_memory(MEM_SYSMODE, work_mode);
    }
}
